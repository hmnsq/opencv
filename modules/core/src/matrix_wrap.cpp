// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/mat.hpp"

નામાકાશ cv {

પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 5> Vec5i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 7> Vec7i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 9> Vec9i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 10> Vec10i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 11> Vec11i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 12> Vec12i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 13> Vec13i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 14> Vec14i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 15> Vec15i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 16> Vec16i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 32> Vec32i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 64> Vec64i;
પ્રકારવ્યાખ્યા Vec<પૂર્ણાંક, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    પસંદગી( esz ) \
    { \
    સ્થિતિ 1: func(uchar); \
    સ્થિતિ 2: func(Vec2b); \
    સ્થિતિ 3: func(Vec3b); \
    સ્થિતિ 4: func(પૂર્ણાંક); \
    સ્થિતિ 6: func(Vec3s); \
    સ્થિતિ 8: func(Vec2i); \
    સ્થિતિ 12: func(Vec3i); \
    સ્થિતિ 16: func(Vec4i); \
    સ્થિતિ 20: func(Vec5i); \
    સ્થિતિ 24: func(Vec6i); \
    સ્થિતિ 28: func(Vec7i); \
    સ્થિતિ 32: func(Vec8i); \
    સ્થિતિ 36: func(Vec9i); \
    સ્થિતિ 40: func(Vec10i); \
    સ્થિતિ 44: func(Vec11i); \
    સ્થિતિ 48: func(Vec12i); \
    સ્થિતિ 52: func(Vec13i); \
    સ્થિતિ 56: func(Vec14i); \
    સ્થિતિ 60: func(Vec15i); \
    સ્થિતિ 64: func(Vec16i); \
    સ્થિતિ 128: func(Vec32i); \
    સ્થિતિ 256: func(Vec64i); \
    સ્થિતિ 512: func(Vec128i); \
    મૂળભૂત: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat આગત_Array_2::getMat_(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    જો( k == MAT )
    {
        અચલ Mat* m = (અચલ Mat*)obj;
        જો( i < 0 )
            પાછા *m;
        પાછા m->row(i);
    }

    જો( k == UMAT )
    {
        અચલ UMat* m = (અચલ UMat*)obj;
        જો( i < 0 )
            પાછા m->getMat(accessFlags);
        પાછા m->getMat(accessFlags).row(i);
    }

    જો (k == MATX)
    {
        CV_Assert( i < 0 );
        પાછા Mat(sz, flags, obj);
    }

    જો( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        પૂર્ણાંક t = CV_8U;
        અચલ std::vector<તાર્કિક>& v = *(અચલ std::vector<તાર્કિક>*)obj;
        પૂર્ણાંક j, n = (પૂર્ણાંક)v.size();
        જો( n == 0 )
            પાછા Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        માટે( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        પાછા m;
    }

    જો( k == NONE )
        પાછા Mat();

    જો( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        પૂર્ણાંક t = CV_MAT_TYPE(flags);
        પૂર્ણાંક esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            જો (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } નહીંતર { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            પૂર્ણાંક v_sz = પૂર્ણાંક(v->size()); \
            પાછા v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& v = *(અચલ std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (પૂર્ણાંક)v.size() );

        પાછા v[i];
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* v = (અચલ Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        પાછા v[i];
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& v = *(અચલ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (પૂર્ણાંક)v.size() );

        પાછા v[i].getMat(accessFlags);
    }

    જો( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    જો( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    જો( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    જો( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        અચલ cuda::HostMem* cuda_mem = (અચલ cuda::HostMem*)obj;

        પાછા cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat આગત_Array_2::getUMat(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    જો( k == UMAT )
    {
        અચલ UMat* m = (અચલ UMat*)obj;
        જો( i < 0 )
            પાછા *m;
        પાછા m->row(i);
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& v = *(અચલ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (પૂર્ણાંક)v.size() );

        પાછા v[i];
    }

    જો( k == MAT )
    {
        અચલ Mat* m = (અચલ Mat*)obj;
        જો( i < 0 )
            પાછા m->getUMat(accessFlags);
        પાછા m->row(i).getUMat(accessFlags);
    }

    પાછા getMat(i).getUMat(accessFlags);
}

શૂન્ય આગત_Array_2::getMatVector(std::vector<Mat>& mv) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    જો( k == MAT )
    {
        અચલ Mat& m = *(અચલ Mat*)obj;
        પૂર્ણાંક n = (પૂર્ણાંક)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        માટે( પૂર્ણાંક i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (શૂન્ય*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (શૂન્ય*)m.ptr(i), &m.step[1]);
        પાછા;
    }

    જો (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        પાછા;
    }

    જો( k == STD_VECTOR )
    {
        અચલ std::vector<uchar>& v = *(અચલ std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        પૂર્ણાંક t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (શૂન્ય*)(v.data() + esz*i));
        પાછા;
    }

    જો( k == NONE )
    {
        mv.clear();
        પાછા;
    }

    જો( k == STD_VECTOR_VECTOR )
    {
        પૂર્ણાંક typ = CV_MAT_TYPE(flags);
        પૂર્ણાંક esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            અચલ std::vector<std::vector<T > >* vv = (અચલ std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            માટે (size_t i = 0; i < n; i++) { \
                અચલ std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                પૂર્ણાંક ni = (પૂર્ણાંક)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (શૂન્ય*)vi.data()) : Mat(); \
            } \
        } \
        તોડો

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        પાછા;
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& v = *(અચલ std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        પાછા;
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* v = (અચલ Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        પાછા;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& v = *(અચલ std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        પાછા;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

શૂન્ય આગત_Array_2::getUMatVector(std::vector<UMat>& umv) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    જો( k == NONE )
    {
        umv.clear();
        પાછા;
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& v = *(અચલ std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        પાછા;
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* v = (અચલ Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        પાછા;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& v = *(અચલ std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        માટે( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        પાછા;
    }

    જો( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        પાછા;
    }
    જો( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        પાછા;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat આગત_Array_2::getGpuMat() અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();

    જો (k == CUDA_GPU_MAT)
    {
        અચલ cuda::GpuMat* d_mat = (અચલ cuda::GpuMat*)obj;
        પાછા *d_mat;
    }

    જો (k == CUDA_HOST_MEM)
    {
        અચલ cuda::HostMem* cuda_mem = (અચલ cuda::HostMem*)obj;
        પાછા cuda_mem->createGpuMatHeader();
    }

    જો (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    જો (k == NONE)
        પાછા cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#નહીંતર
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
શૂન્ય આગત_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();
    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#નહીંતર
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
શૂન્ય આગત_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();
    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#નહીંતર
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND આગત_Array_2::getGpuMatND() અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();

    જો (k == CUDA_GPU_MATND)
    {
        અચલ cuda::GpuMatND* d_mat = (અચલ cuda::GpuMatND*)obj;
        પાછા *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#નહીંતર
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer આગત_Array_2::getOGlBuffer() અચલ
{
    આગત_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    અચલ ogl::Buffer* gl_buf = (અચલ ogl::Buffer*)obj;
    પાછા *gl_buf;
}

પૂર્ણાંક આગત_Array_2::rows_(પૂર્ણાંક i) અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();
    જો (k == CUDA_GPU_MATND)
    {
        અચલ cuda::GpuMatND& _gpuMatND = *(અચલ cuda::GpuMatND*)obj;
        પાછા (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    પાછા size_(i).height;
}

પૂર્ણાંક આગત_Array_2::cols_(પૂર્ણાંક i) અચલ
{
#ifdef HAVE_CUDA
    આગત_Array_2::KindFlag k = kind();
    જો (k == CUDA_GPU_MATND)
    {
        અચલ cuda::GpuMatND& _gpuMatND = *(અચલ cuda::GpuMatND*)obj;
        પાછા (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    પાછા size_(i).width;
}

Size આગત_Array_2::size_(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        અચલ Mat* m = (અચલ Mat*)obj;
        CV_Assert(m->dims <= 2);
        પાછા Size(m->cols, m->rows);
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        અચલ UMat* m = (અચલ UMat*)obj;
        CV_Assert(m->dims <= 2);
        પાછા Size(m->cols, m->rows);
    }

    જો (k == MATX)
    {
        CV_Assert( i < 0 );
        પાછા sz;
    }

    જો( k == NONE )
        પાછા Size();

    જો( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        પાછા Size((પૂર્ણાંક)n, 1);
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        જો( i < 0 )
            પાછા vv.empty() ? Size() : Size((પૂર્ણાંક)vv.size(), 1);
        CV_Assert( i < (પૂર્ણાંક)vv.size() );

        પાછા vv[i].size();
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        જો( i < 0 )
            પાછા sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        પાછા vv[i].size();
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        અચલ std::vector<cuda::GpuMat>& vv = *(અચલ std::vector<cuda::GpuMat>*)obj;
        જો (i < 0)
            પાછા vv.empty() ? Size() : Size((પૂર્ણાંક)vv.size(), 1);
        CV_Assert(i < (પૂર્ણાંક)vv.size());
        પાછા vv[i].size();
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        જો (i < 0)
            પાછા vv.empty() ? Size() : Size((પૂર્ણાંક)vv.size(), 1);
        CV_Assert(i < (પૂર્ણાંક)vv.size());
        અચલ cuda::GpuMatND& m = vv[i];
        જો (m.dims == 0)
            પાછા Size();
        જો (m.dims == 1)
            પાછા Size(1, m.size[0]);
        પાછા Size(m.size[1], m.size[0]);
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        જો( i < 0 )
            પાછા vv.empty() ? Size() : Size((પૂર્ણાંક)vv.size(), 1);
        CV_Assert( i < (પૂર્ણાંક)vv.size() );

        પાછા vv[i].size();
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        જો (i < 0)
            પાછા vv.empty() ? Size() : Size((પૂર્ણાંક)vv.size(), 1);
        CV_Assert(i < (પૂર્ણાંક)vv.size());
        અચલ cuda::GpuMatND& m = vv[i];
        જો (m.dims == 0)
            પાછા Size();
        જો (m.dims == 1)
            પાછા Size(1, m.size[0]);
        પાછા Size(m.size[1], m.size[0]);
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        અચલ ogl::Buffer* buf = (અચલ ogl::Buffer*)obj;
        પાછા buf->size();
    }

    જો( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        અચલ cuda::GpuMat* d_mat = (અચલ cuda::GpuMat*)obj;
        પાછા d_mat->size();
    }

    જો( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        અચલ cuda::HostMem* cuda_mem = (અચલ cuda::HostMem*)obj;
        પાછા cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

પૂર્ણાંક આગત_Array_2::sizend(પૂર્ણાંક* arrsz, પૂર્ણાંક i) અચલ
{
    પૂર્ણાંક j, d = 0;
    આગત_Array_2::KindFlag k = kind();

    જો( k == NONE )
        ;
    નહીંતર જો( k == MAT )
    {
        CV_Assert( i < 0 );
        અચલ Mat& m = *(અચલ Mat*)obj;
        d = m.dims;
        જો(arrsz)
            માટે(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    નહીંતર જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        અચલ UMat& m = *(અચલ UMat*)obj;
        d = m.dims;
        જો(arrsz)
            માટે(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    નહીંતર જો( k == STD_VECTOR_MAT && i >= 0 )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        અચલ Mat& m = vv[i];
        d = m.dims;
        જો(arrsz)
            માટે(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    નહીંતર જો( k == STD_ARRAY_MAT && i >= 0 )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert( i < sz.height );
        અચલ Mat& m = vv[i];
        d = m.dims;
        જો(arrsz)
            માટે(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    નહીંતર જો( k == STD_VECTOR_UMAT && i >= 0 )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        અચલ UMat& m = vv[i];
        d = m.dims;
        જો(arrsz)
            માટે(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    નહીંતર જો (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        જો(arrsz)
        {
            arrsz[0] = પૂર્ણાંક(n);
        }
    }
    નહીંતર
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        જો(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    પાછા d;
}

તાર્કિક આગત_Array_2::empty(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો (i >= 0) {
        જો (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<અચલ std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            પાછા mv->at(i).empty();
        }
        નહીંતર જો (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<અચલ std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            પાછા umv->at(i).empty();
        }
        નહીંતર જો (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            પાછા n == 0;
        } નહીંતર {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    પાછા empty();
}

MatShape આગત_Array_2::shape(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    MatShape shape;

    જો( k == NONE )
        ;
    નહીંતર જો( k == MAT )
    {
        CV_Assert( i < 0 );
        અચલ Mat& m = *(અચલ Mat*)obj;
        shape = m.size;
    }
    નહીંતર જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        અચલ UMat& m = *(અચલ UMat*)obj;
        shape = m.size;
    }
    નહીંતર જો( k == STD_VECTOR_MAT && i >= 0 )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        અચલ Mat& m = vv[i];
        shape = m.size;
    }
    નહીંતર જો( k == STD_ARRAY_MAT && i >= 0 )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert( i < sz.height );
        અચલ Mat& m = vv[i];
        shape = m.size;
    }
    નહીંતર જો( k == STD_VECTOR_UMAT && i >= 0 )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        અચલ UMat& m = vv[i];
        shape = m.size;
    }
    નહીંતર
    {
        પૂર્ણાંક sizes[CV_MAX_DIM];
        પૂર્ણાંક dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    પાછા shape;
}

તાર્કિક આગત_Array_2::sameSize(અચલ આગત_Array_2& arr) અચલ
{
    આગત_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    જો( k1 == MAT )
    {
        અચલ Mat* m = ((અચલ Mat*)obj);
        જો( k2 == MAT )
            પાછા m->size == ((અચલ Mat*)arr.obj)->size;
        જો( k2 == UMAT )
            પાછા m->size == ((અચલ UMat*)arr.obj)->size;
        જો( m->dims > 2 )
            પાછા ખોટું;
        sz1 = m->size();
    }
    નહીંતર જો( k1 == UMAT )
    {
        અચલ UMat* m = ((અચલ UMat*)obj);
        જો( k2 == MAT )
            પાછા m->size == ((અચલ Mat*)arr.obj)->size;
        જો( k2 == UMAT )
            પાછા m->size == ((અચલ UMat*)arr.obj)->size;
        જો( m->dims > 2 )
            પાછા ખોટું;
        sz1 = m->size();
    }
    નહીંતર જો ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        પાછા ((અચલ cuda::GpuMatND*)obj)->size == ((અચલ cuda::GpuMatND*)arr.obj)->size;
    }
    નહીંતર
        sz1 = size();

    જો( arr.dims() > 2 )
        પાછા ખોટું;
    પાછા sz1 == arr.size();
}

પૂર્ણાંક આગત_Array_2::dims_(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ Mat*)obj)->dims;
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ UMat*)obj)->dims;
    }

    જો (k == MATX)
    {
        CV_Assert( i < 0 );
        પાછા 2;
    }

    જો( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        પાછા 1;
    }

    જો( k == NONE )
        પાછા 0;

    જો( k == STD_VECTOR_VECTOR )
    {
        જો( i < 0 )
            પાછા 1;
        CV_Assert( size_t(i) < total() );
        પાછા 1;
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        જો( i < 0 )
            પાછા 1;
        CV_Assert( i < (પૂર્ણાંક)vv.size() );

        પાછા vv[i].dims;
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        જો( i < 0 )
            પાછા 1;
        CV_Assert( i < sz.height );

        પાછા vv[i].dims;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        જો( i < 0 )
            પાછા 1;
        CV_Assert( i < (પૂર્ણાંક)vv.size() );

        પાછા vv[i].dims;
    }

    જો( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        પાછા 2;
    }

    જો( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        પાછા 2;
    }

    જો( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ cuda::GpuMatND*)obj)->dims;
    }

    જો( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        પાછા 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t આગત_Array_2::total(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ Mat*)obj)->total();
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ UMat*)obj)->total();
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        જો( i < 0 )
            પાછા vv.size();

        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        પાછા vv[i].total();
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        જો( i < 0 )
            પાછા sz.height;

        CV_Assert( i < sz.height );
        પાછા vv[i].total();
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        જો( i < 0 )
            પાછા vv.size();

        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        પાછા vv[i].total();
    }

    જો( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        અચલ cuda::GpuMatND& _gpuMatND = *((અચલ cuda::GpuMatND*)obj);
        જો (_gpuMatND.dims > 0)
        {
            res = 1;
            માટે(પૂર્ણાંક d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            પાછા res;
        }
    }

    જો (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        પૂર્ણાંક esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            જો (k == STD_VECTOR_VECTOR) { \
                અચલ std::vector<std::vector<T > >* vv = (અચલ std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                જો (i < 0) \
                    પાછા n; \
                CV_Assert((size_t)i < n); \
                પાછા vv->at(i).size(); \
            } નહીંતર \
                પાછા ((અચલ std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    પાછા size(i).area();
}

પૂર્ણાંક આગત_Array_2::type_(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
        પાછા ((અચલ Mat*)obj)->type();

    જો( k == UMAT )
        પાછા ((અચલ UMat*)obj)->type();

    જો( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        પાછા CV_MAT_TYPE(flags);

    જો( k == NONE )
        પાછા -1;

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        જો( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            પાછા CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        પાછા vv[i >= 0 ? i : 0].type();
    }

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        જો( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            પાછા CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (પૂર્ણાંક)vv.size() );
        પાછા vv[i >= 0 ? i : 0].type();
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        જો( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            પાછા CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        પાછા vv[i >= 0 ? i : 0].type();
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        અચલ std::vector<cuda::GpuMat>& vv = *(અચલ std::vector<cuda::GpuMat>*)obj;
        જો (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            પાછા CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (પૂર્ણાંક)vv.size());
        પાછા vv[i >= 0 ? i : 0].type();
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        જો (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            પાછા CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (પૂર્ણાંક)vv.size());
        પાછા vv[i >= 0 ? i : 0].type();
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == OPENGL_BUFFER )
        પાછા ((અચલ ogl::Buffer*)obj)->type();

    જો( k == CUDA_GPU_MAT )
        પાછા ((અચલ cuda::GpuMat*)obj)->type();

    જો( k == CUDA_GPU_MATND )
        પાછા ((અચલ cuda::GpuMatND*)obj)->type();

    જો( k == CUDA_HOST_MEM )
        પાછા ((અચલ cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

પૂર્ણાંક આગત_Array_2::depth_(પૂર્ણાંક i) અચલ
{
    પાછા CV_MAT_DEPTH(type(i));
}

તાર્કિક આગત_Array_2::empty_() અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
        પાછા ((અચલ Mat*)obj)->empty();

    જો( k == UMAT )
        પાછા ((અચલ UMat*)obj)->empty();

    જો (k == MATX)
        પાછા ખોટું;

    જો( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        પાછા n == 0;
    }

    જો( k == NONE )
        પાછા સાચું;

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        પાછા vv.empty();
    }

    જો( k == STD_ARRAY_MAT )
    {
        પાછા sz.height == 0;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        પાછા vv.empty();
    }

    જો( k == OPENGL_BUFFER )
        પાછા ((અચલ ogl::Buffer*)obj)->empty();

    જો( k == CUDA_GPU_MAT )
        પાછા ((અચલ cuda::GpuMat*)obj)->empty();

    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        અચલ std::vector<cuda::GpuMat>& vv = *(અચલ std::vector<cuda::GpuMat>*)obj;
        પાછા vv.empty();
    }

    જો( k == CUDA_GPU_MATND )
        પાછા ((અચલ cuda::GpuMatND*)obj)->empty();

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        પાછા vv.empty();
    }

    જો( k == CUDA_HOST_MEM )
        પાછા ((અચલ cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

તાર્કિક આગત_Array_2::isContinuous(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
        પાછા i < 0 ? ((અચલ Mat*)obj)->isContinuous() : સાચું;

    જો( k == UMAT )
        પાછા i < 0 ? ((અચલ UMat*)obj)->isContinuous() : સાચું;

    જો( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        પાછા સાચું;

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].isContinuous();
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        પાછા vv[i].isContinuous();
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].isContinuous();
    }

    જો( k == CUDA_GPU_MAT )
      પાછા i < 0 ? ((અચલ cuda::GpuMat*)obj)->isContinuous() : સાચું;

    જો( k == CUDA_GPU_MATND )
        પાછા i < 0 ? ((અચલ cuda::GpuMatND*)obj)->isContinuous() : સાચું;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

તાર્કિક આગત_Array_2::isSubmatrix(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
        પાછા i < 0 ? ((અચલ Mat*)obj)->isSubmatrix() : ખોટું;

    જો( k == UMAT )
        પાછા i < 0 ? ((અચલ UMat*)obj)->isSubmatrix() : ખોટું;

    જો( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        પાછા ખોટું;

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].isSubmatrix();
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        પાછા vv[i].isSubmatrix();
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].isSubmatrix();
    }

    જો( k == CUDA_GPU_MATND )
    {
        પાછા ((અચલ cuda::GpuMatND*)obj)->isSubmatrix();
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t આગત_Array_2::offset(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        અચલ Mat * અચલ m = ((અચલ Mat*)obj);
        પાછા (size_t)(m->ptr() - m->datastart);
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ UMat*)obj)->offset;
    }

    જો( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        પાછા 0;

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (પૂર્ણાંક)vv.size() );

        પાછા (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        પાછા (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].offset;
    }

    જો( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        અચલ cuda::GpuMat * અચલ m = ((અચલ cuda::GpuMat*)obj);
        પાછા (size_t)(m->data - m->datastart);
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        અચલ std::vector<cuda::GpuMat>& vv = *(અચલ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t આગત_Array_2::step(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ Mat*)obj)->step;
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ UMat*)obj)->step;
    }

    જો( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        પાછા 0;

    જો( k == STD_VECTOR_MAT )
    {
        અચલ std::vector<Mat>& vv = *(અચલ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (પૂર્ણાંક)vv.size() );
        પાછા vv[i].step;
    }

    જો( k == STD_ARRAY_MAT )
    {
        અચલ Mat* vv = (અચલ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        પાછા vv[i].step;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        અચલ std::vector<UMat>& vv = *(અચલ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].step;
    }

    જો( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        પાછા ((અચલ cuda::GpuMat*)obj)->step;
    }
    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        અચલ std::vector<cuda::GpuMat>& vv = *(અચલ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        પાછા vv[i].step;
    }
    જો( k == CUDA_GPU_MATND )
    {
        અચલ cuda::GpuMatND& _gpuMatND = *(અચલ cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        પાછા _gpuMatND.step[i];
    }

    જો (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        અચલ std::vector<cuda::GpuMatND>& vv = *(અચલ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        અચલ cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        પાછા _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

શૂન્ય આગત_Array_2::copyTo(અચલ _OutputArray& arr) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == NONE )
        arr.release();
    નહીંતર જો( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    નહીંતર જો( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    નહીંતર જો (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    નહીંતર
        CV_Error(Error::StsNotImplemented, "");
}

શૂન્ય આગત_Array_2::copyTo(અચલ _OutputArray& arr, અચલ આગત_Array_2 & mask) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == NONE )
        arr.release();
    નહીંતર જો( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    નહીંતર જો( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    નહીંતર જો (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    નહીંતર
        CV_Error(Error::StsNotImplemented, "");
}

તાર્કિક _OutputArray::fixedSize() અચલ
{
    પાછા (flags & FIXED_SIZE) == FIXED_SIZE;
}

તાર્કિક _OutputArray::fixedType() અચલ
{
    પાછા (flags & FIXED_TYPE) == FIXED_TYPE;
}

શૂન્ય _OutputArray::create_(Size _sz, પૂર્ણાંક mtype, પૂર્ણાંક i, તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        પાછા;
    }
    જો( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        પાછા;
    }
    જો( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    જો( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    જો( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    જો( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    પૂર્ણાંક sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

શૂન્ય _OutputArray::create_(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક mtype, પૂર્ણાંક i, તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        પાછા;
    }
    જો( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        પાછા;
    }
    જો( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    જો( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    જો( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    જો( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    પૂર્ણાંક sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

શૂન્ય _OutputArray::create(પૂર્ણાંક d, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક mtype, પૂર્ણાંક i,
                          તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    પૂર્ણાંક size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    આગત_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    જો( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        જો (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            નહીંતર
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        જો(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        પાછા;
    }

    જો( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        જો (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            નહીંતર
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        જો(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        પાછા;
    }

    જો( k == MATX )
    {
        CV_Assert( i < 0 );
        પૂર્ણાંક type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        જો (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            પૂર્ણાંક total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        નહીંતર
        {
            જો (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            નહીંતર
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        પાછા;
    }

    જો( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        પૂર્ણાંક esz = CV_ELEM_SIZE(flags);

        જો( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            પૂર્ણાંક type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            જો (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                જો (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    પાછા; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } નહીંતર { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        તોડો

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        પાછા;
    }

    જો( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    જો( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        જો( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            જો( fixedType() )
            {
                પૂર્ણાંક _type = CV_MAT_TYPE(flags);
                માટે( size_t j = len0; j < len; j++ )
                {
                    જો( v[j].type() == _type )
                        ચાલુ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            પાછા;
        }

        CV_Assert( i < (પૂર્ણાંક)v.size() );
        Mat& m = v[i];

        જો( allowTransposed )
        {
            જો( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            જો( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            નહીંતર
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        જો(fixedSize())
        {
            CV_Assert(m.dims == d);
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        પાછા;
    }

    જો( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        જો( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            જો( fixedType() )
            {
                પૂર્ણાંક _type = CV_MAT_TYPE(flags);
                માટે( size_t j = len0; j < len; j++ )
                {
                    જો( v[j].type() == _type )
                        ચાલુ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            પાછા;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        જો( allowTransposed )
        {
            જો( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            જો( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            નહીંતર
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        જો(fixedSize())
        {
            CV_Assert(m.dims == d);
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        પાછા;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        જો( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            જો( fixedType() )
            {
                પૂર્ણાંક _type = CV_MAT_TYPE(flags);
                માટે( size_t j = len0; j < len; j++ )
                {
                    જો( v[j].type() == _type )
                        ચાલુ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            પાછા;
        }

        CV_Assert( i < (પૂર્ણાંક)v.size() );
        UMat& m = v[i];

        જો( allowTransposed )
        {
            જો( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            જો( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            નહીંતર
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        જો(fixedSize())
        {
            CV_Assert(m.dims == d);
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        પાછા;
    }

    જો ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        પાછા;
    }

    જો( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

શૂન્ય _OutputArray::create(અચલ MatShape& shape, પૂર્ણાંક mtype, પૂર્ણાંક i,
                          તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    જો (shape.dims < 0) {
        release();
    } નહીંતર {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(પૂર્ણાંક mtype) અચલ
{
    mtype = CV_MAT_TYPE(mtype);
    પાછા getMat().reinterpret(mtype);
}

શૂન્ય _OutputArray::createSameSize(અચલ આગત_Array_2& arr, પૂર્ણાંક mtype) અચલ
{
    create(arr.shape(), mtype);
}

શૂન્ય _OutputArray::fit(Size _sz, પૂર્ણાંક mtype, પૂર્ણાંક i,
                       તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    પૂર્ણાંક sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

શૂન્ય _OutputArray::fit(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક mtype, પૂર્ણાંક i,
                       તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    પૂર્ણાંક sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

શૂન્ય _OutputArray::fit(અચલ MatShape& shape, પૂર્ણાંક mtype, પૂર્ણાંક i,
                       તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    જો (shape.dims < 0) {
        release();
        પાછા;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    આગત_Array_2::KindFlag k = kind();
    જો ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } નહીંતર જો ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

શૂન્ય _OutputArray::fitSameSize(અચલ આગત_Array_2& arr, પૂર્ણાંક mtype) અચલ
{
    fit(arr.shape(), mtype);
}

શૂન્ય _OutputArray::fit(પૂર્ણાંક d, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક mtype, પૂર્ણાંક i,
                       તાર્કિક allowTransposed, _OutputArray::DepthMask fixedDepthMask) અચલ
{
    પૂર્ણાંક size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    આગત_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    જો( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        જો (k == MAT)
            m = (Mat*)obj;
        નહીંતર {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        જો (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            નહીંતર
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        જો(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        પાછા;
    }

    જો( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        જો (k == UMAT)
            m = (UMat*)obj;
        નહીંતર {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        જો (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            પાછા;
        }

        જો(fixedType())
        {
            જો(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            નહીંતર
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        જો(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            માટે(પૂર્ણાંક j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        પાછા;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

શૂન્ય _OutputArray::release() અચલ
{
    CV_Assert(!fixedSize());

    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        ((Mat*)obj)->release();
        પાછા;
    }

    જો( k == UMAT )
    {
        ((UMat*)obj)->release();
        પાછા;
    }

    જો( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    જો( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    જો( k == NONE )
        પાછા;

    જો( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        જો (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } નહીંતર { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        તોડો

        પૂર્ણાંક esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        પાછા;
    }

    જો( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        પાછા;
    }

    જો( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        માટે( પૂર્ણાંક i = 0; i < sz.height; i++ )
            v[i].release();
        પાછા;
    }

    જો( k == STD_BOOL_VECTOR )
    {
        ((std::vector<તાર્કિક>*)obj)->clear();
        પાછા;
    }

    જો( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        પાછા;
    }
    જો (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        પાછા;
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

શૂન્ય _OutputArray::clear() અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        પાછા;
    }

    release();
}

તાર્કિક _OutputArray::needed() અચલ
{
    પાછા kind() != NONE;
}

Mat& _OutputArray::getMatRef(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો( i < 0 )
    {
        CV_Assert( k == MAT );
        પાછા *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    જો( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (પૂર્ણાંક)v.size() );
        પાછા v[i];
    }
    નહીંતર
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        પાછા v[i];
    }
}

UMat& _OutputArray::getUMatRef(પૂર્ણાંક i) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો( i < 0 )
    {
        CV_Assert( k == UMAT );
        પાછા *(UMat*)obj;
    }
    નહીંતર
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (પૂર્ણાંક)v.size() );
        પાછા v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    પાછા *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    પાછા *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    પાછા *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    પાછા *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    પાછા *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() અચલ
{
    આગત_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    પાછા *(cuda::HostMem*)obj;
}

શૂન્ય _OutputArray::setTo(અચલ આગત_Array_2& arr, અચલ આગત_Array_2 & mask) અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == NONE )
        ;
    નહીંતર જો (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    નહીંતર જો( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    નહીંતર જો( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), આગત_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<દ્વિગુણ, 4>(value.ptr<દ્વિગુણ>())), mask);
#નહીંતર
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    નહીંતર
        CV_Error(Error::StsNotImplemented, "");
}

શૂન્ય _OutputArray::setZero() અચલ
{
    આગત_Array_2::KindFlag k = kind();

    જો( k == NONE )
        ;
    નહીંતર જો (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    નહીંતર
    {
        setTo(Scalar::all(0), noArray());
    }
}

શૂન્ય _OutputArray::assign(અચલ UMat& u) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    નહીંતર જો (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    નહીંતર જો (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


શૂન્ય _OutputArray::assign(અચલ Mat& m) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    નહીંતર જો (k == MAT)
    {
        *(Mat*)obj = m;
    }
    નહીંતર જો (k == MATX)
    {
        m.copyTo(getMat());
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


શૂન્ય _OutputArray::move(UMat& u) અચલ
{
    જો (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        પાછા;
    }
    પૂર્ણાંક k = kind();
    જો (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    નહીંતર જો (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    નહીંતર જો (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


શૂન્ય _OutputArray::move(Mat& m) અચલ
{
    જો (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        પાછા;
    }
    પૂર્ણાંક k = kind();
    જો (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    નહીંતર જો (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    નહીંતર જો (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


શૂન્ય _OutputArray::assign(અચલ std::vector<UMat>& v) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        માટે (size_t i = 0; i < v.size(); i++)
        {
            અચલ UMat& m = v[i];
            UMat& this_m = this_v[i];
            જો (this_m.u != ખાલી && this_m.u == m.u)
                ચાલુ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    નહીંતર જો (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        માટે (size_t i = 0; i < v.size(); i++)
        {
            અચલ UMat& m = v[i];
            Mat& this_m = this_v[i];
            જો (this_m.u != ખાલી && this_m.u == m.u)
                ચાલુ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


શૂન્ય _OutputArray::assign(અચલ std::vector<Mat>& v) અચલ
{
    આગત_Array_2::KindFlag k = kind();
    જો (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        માટે (size_t i = 0; i < v.size(); i++)
        {
            અચલ Mat& m = v[i];
            UMat& this_m = this_v[i];
            જો (this_m.u != ખાલી && this_m.u == m.u)
                ચાલુ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    નહીંતર જો (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        માટે (size_t i = 0; i < v.size(); i++)
        {
            અચલ Mat& m = v[i];
            Mat& this_m = this_v[i];
            જો (this_m.u != ખાલી && this_m.u == m.u)
                ચાલુ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    નહીંતર
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


સ્થિર આગત_Output_Array_2 _none;
આગત_Output_Array noArray() { પાછા _none; }

} // cv::
