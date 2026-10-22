// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/mat.hpp"

നാമാകാശ cv {

തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 5> Vec5i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 7> Vec7i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 9> Vec9i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 10> Vec10i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 11> Vec11i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 12> Vec12i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 13> Vec13i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 14> Vec14i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 15> Vec15i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 16> Vec16i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 32> Vec32i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 64> Vec64i;
തരനിർവചനം Vec<പൂർണ്ണസംഖ്യ, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    തിരഞ്ഞെടുപ്പ്( esz ) \
    { \
    അവസ്ഥ 1: func(uchar); \
    അവസ്ഥ 2: func(Vec2b); \
    അവസ്ഥ 3: func(Vec3b); \
    അവസ്ഥ 4: func(പൂർണ്ണസംഖ്യ); \
    അവസ്ഥ 6: func(Vec3s); \
    അവസ്ഥ 8: func(Vec2i); \
    അവസ്ഥ 12: func(Vec3i); \
    അവസ്ഥ 16: func(Vec4i); \
    അവസ്ഥ 20: func(Vec5i); \
    അവസ്ഥ 24: func(Vec6i); \
    അവസ്ഥ 28: func(Vec7i); \
    അവസ്ഥ 32: func(Vec8i); \
    അവസ്ഥ 36: func(Vec9i); \
    അവസ്ഥ 40: func(Vec10i); \
    അവസ്ഥ 44: func(Vec11i); \
    അവസ്ഥ 48: func(Vec12i); \
    അവസ്ഥ 52: func(Vec13i); \
    അവസ്ഥ 56: func(Vec14i); \
    അവസ്ഥ 60: func(Vec15i); \
    അവസ്ഥ 64: func(Vec16i); \
    അവസ്ഥ 128: func(Vec32i); \
    അവസ്ഥ 256: func(Vec64i); \
    അവസ്ഥ 512: func(Vec128i); \
    സ്ഥിരസ്ഥിതി: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat നിവേശം_Array_2::getMat_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    എങ്കിൽ( k == MAT )
    {
        സ്ഥിരം Mat* m = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ *m;
        തിരികെ m->row(i);
    }

    എങ്കിൽ( k == UMAT )
    {
        സ്ഥിരം UMat* m = (സ്ഥിരം UMat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ m->getMat(accessFlags);
        തിരികെ m->getMat(accessFlags).row(i);
    }

    എങ്കിൽ (k == MATX)
    {
        CV_Assert( i < 0 );
        തിരികെ Mat(sz, flags, obj);
    }

    എങ്കിൽ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        പൂർണ്ണസംഖ്യ t = CV_8U;
        സ്ഥിരം std::vector<തർക്കം>& v = *(സ്ഥിരം std::vector<തർക്കം>*)obj;
        പൂർണ്ണസംഖ്യ j, n = (പൂർണ്ണസംഖ്യ)v.size();
        എങ്കിൽ( n == 0 )
            തിരികെ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        വേണ്ടി( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        തിരികെ m;
    }

    എങ്കിൽ( k == NONE )
        തിരികെ Mat();

    എങ്കിൽ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        പൂർണ്ണസംഖ്യ t = CV_MAT_TYPE(flags);
        പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            എങ്കിൽ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } അല്ലെങ്കിൽ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            പൂർണ്ണസംഖ്യ v_sz = പൂർണ്ണസംഖ്യ(v->size()); \
            തിരികെ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& v = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (പൂർണ്ണസംഖ്യ)v.size() );

        തിരികെ v[i];
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* v = (സ്ഥിരം Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        തിരികെ v[i];
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& v = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (പൂർണ്ണസംഖ്യ)v.size() );

        തിരികെ v[i].getMat(accessFlags);
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    എങ്കിൽ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        സ്ഥിരം cuda::HostMem* cuda_mem = (സ്ഥിരം cuda::HostMem*)obj;

        തിരികെ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat നിവേശം_Array_2::getUMat(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    എങ്കിൽ( k == UMAT )
    {
        സ്ഥിരം UMat* m = (സ്ഥിരം UMat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ *m;
        തിരികെ m->row(i);
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& v = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (പൂർണ്ണസംഖ്യ)v.size() );

        തിരികെ v[i];
    }

    എങ്കിൽ( k == MAT )
    {
        സ്ഥിരം Mat* m = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ m->getUMat(accessFlags);
        തിരികെ m->row(i).getUMat(accessFlags);
    }

    തിരികെ getMat(i).getUMat(accessFlags);
}

ശൂന്യം നിവേശം_Array_2::getMatVector(std::vector<Mat>& mv) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    എങ്കിൽ( k == MAT )
    {
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)obj;
        പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (ശൂന്യം*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (ശൂന്യം*)m.ptr(i), &m.step[1]);
        തിരികെ;
    }

    എങ്കിൽ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR )
    {
        സ്ഥിരം std::vector<uchar>& v = *(സ്ഥിരം std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        പൂർണ്ണസംഖ്യ t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (ശൂന്യം*)(v.data() + esz*i));
        തിരികെ;
    }

    എങ്കിൽ( k == NONE )
    {
        mv.clear();
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_VECTOR )
    {
        പൂർണ്ണസംഖ്യ typ = CV_MAT_TYPE(flags);
        പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            സ്ഥിരം std::vector<std::vector<T > >* vv = (സ്ഥിരം std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            വേണ്ടി (size_t i = 0; i < n; i++) { \
                സ്ഥിരം std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                പൂർണ്ണസംഖ്യ ni = (പൂർണ്ണസംഖ്യ)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (ശൂന്യം*)vi.data()) : Mat(); \
            } \
        } \
        നിർത്തുക

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& v = *(സ്ഥിരം std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        തിരികെ;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* v = (സ്ഥിരം Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& v = *(സ്ഥിരം std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        തിരികെ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ശൂന്യം നിവേശം_Array_2::getUMatVector(std::vector<UMat>& umv) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    എങ്കിൽ( k == NONE )
    {
        umv.clear();
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& v = *(സ്ഥിരം std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* v = (സ്ഥിരം Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& v = *(സ്ഥിരം std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        വേണ്ടി( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        തിരികെ;
    }

    എങ്കിൽ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        തിരികെ;
    }
    എങ്കിൽ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        തിരികെ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat നിവേശം_Array_2::getGpuMat() സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ (k == CUDA_GPU_MAT)
    {
        സ്ഥിരം cuda::GpuMat* d_mat = (സ്ഥിരം cuda::GpuMat*)obj;
        തിരികെ *d_mat;
    }

    എങ്കിൽ (k == CUDA_HOST_MEM)
    {
        സ്ഥിരം cuda::HostMem* cuda_mem = (സ്ഥിരം cuda::HostMem*)obj;
        തിരികെ cuda_mem->createGpuMatHeader();
    }

    എങ്കിൽ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    എങ്കിൽ (k == NONE)
        തിരികെ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#അല്ലെങ്കിൽ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ശൂന്യം നിവേശം_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ശൂന്യം നിവേശം_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND നിവേശം_Array_2::getGpuMatND() സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ (k == CUDA_GPU_MATND)
    {
        സ്ഥിരം cuda::GpuMatND* d_mat = (സ്ഥിരം cuda::GpuMatND*)obj;
        തിരികെ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#അല്ലെങ്കിൽ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer നിവേശം_Array_2::getOGlBuffer() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    സ്ഥിരം ogl::Buffer* gl_buf = (സ്ഥിരം ogl::Buffer*)obj;
    തിരികെ *gl_buf;
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::rows_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == CUDA_GPU_MATND)
    {
        സ്ഥിരം cuda::GpuMatND& _gpuMatND = *(സ്ഥിരം cuda::GpuMatND*)obj;
        തിരികെ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    തിരികെ size_(i).height;
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::cols_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
#ifdef HAVE_CUDA
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == CUDA_GPU_MATND)
    {
        സ്ഥിരം cuda::GpuMatND& _gpuMatND = *(സ്ഥിരം cuda::GpuMatND*)obj;
        തിരികെ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    തിരികെ size_(i).width;
}

Size നിവേശം_Array_2::size_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം Mat* m = (സ്ഥിരം Mat*)obj;
        CV_Assert(m->dims <= 2);
        തിരികെ Size(m->cols, m->rows);
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം UMat* m = (സ്ഥിരം UMat*)obj;
        CV_Assert(m->dims <= 2);
        തിരികെ Size(m->cols, m->rows);
    }

    എങ്കിൽ (k == MATX)
    {
        CV_Assert( i < 0 );
        തിരികെ sz;
    }

    എങ്കിൽ( k == NONE )
        തിരികെ Size();

    എങ്കിൽ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        തിരികെ Size((പൂർണ്ണസംഖ്യ)n, 1);
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ vv.empty() ? Size() : Size((പൂർണ്ണസംഖ്യ)vv.size(), 1);
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );

        തിരികെ vv[i].size();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        തിരികെ vv[i].size();
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        സ്ഥിരം std::vector<cuda::GpuMat>& vv = *(സ്ഥിരം std::vector<cuda::GpuMat>*)obj;
        എങ്കിൽ (i < 0)
            തിരികെ vv.empty() ? Size() : Size((പൂർണ്ണസംഖ്യ)vv.size(), 1);
        CV_Assert(i < (പൂർണ്ണസംഖ്യ)vv.size());
        തിരികെ vv[i].size();
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        എങ്കിൽ (i < 0)
            തിരികെ vv.empty() ? Size() : Size((പൂർണ്ണസംഖ്യ)vv.size(), 1);
        CV_Assert(i < (പൂർണ്ണസംഖ്യ)vv.size());
        സ്ഥിരം cuda::GpuMatND& m = vv[i];
        എങ്കിൽ (m.dims == 0)
            തിരികെ Size();
        എങ്കിൽ (m.dims == 1)
            തിരികെ Size(1, m.size[0]);
        തിരികെ Size(m.size[1], m.size[0]);
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ vv.empty() ? Size() : Size((പൂർണ്ണസംഖ്യ)vv.size(), 1);
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );

        തിരികെ vv[i].size();
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        എങ്കിൽ (i < 0)
            തിരികെ vv.empty() ? Size() : Size((പൂർണ്ണസംഖ്യ)vv.size(), 1);
        CV_Assert(i < (പൂർണ്ണസംഖ്യ)vv.size());
        സ്ഥിരം cuda::GpuMatND& m = vv[i];
        എങ്കിൽ (m.dims == 0)
            തിരികെ Size();
        എങ്കിൽ (m.dims == 1)
            തിരികെ Size(1, m.size[0]);
        തിരികെ Size(m.size[1], m.size[0]);
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം ogl::Buffer* buf = (സ്ഥിരം ogl::Buffer*)obj;
        തിരികെ buf->size();
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം cuda::GpuMat* d_mat = (സ്ഥിരം cuda::GpuMat*)obj;
        തിരികെ d_mat->size();
    }

    എങ്കിൽ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം cuda::HostMem* cuda_mem = (സ്ഥിരം cuda::HostMem*)obj;
        തിരികെ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::sizend(പൂർണ്ണസംഖ്യ* arrsz, പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ j, d = 0;
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == NONE )
        ;
    അല്ലെങ്കിൽ എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)obj;
        d = m.dims;
        എങ്കിൽ(arrsz)
            വേണ്ടി(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം UMat& m = *(സ്ഥിരം UMat*)obj;
        d = m.dims;
        എങ്കിൽ(arrsz)
            വേണ്ടി(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_VECTOR_MAT && i >= 0 )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        സ്ഥിരം Mat& m = vv[i];
        d = m.dims;
        എങ്കിൽ(arrsz)
            വേണ്ടി(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_ARRAY_MAT && i >= 0 )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert( i < sz.height );
        സ്ഥിരം Mat& m = vv[i];
        d = m.dims;
        എങ്കിൽ(arrsz)
            വേണ്ടി(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        സ്ഥിരം UMat& m = vv[i];
        d = m.dims;
        എങ്കിൽ(arrsz)
            വേണ്ടി(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    അല്ലെങ്കിൽ എങ്കിൽ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        എങ്കിൽ(arrsz)
        {
            arrsz[0] = പൂർണ്ണസംഖ്യ(n);
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        എങ്കിൽ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    തിരികെ d;
}

തർക്കം നിവേശം_Array_2::empty(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (i >= 0) {
        എങ്കിൽ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<സ്ഥിരം std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            തിരികെ mv->at(i).empty();
        }
        അല്ലെങ്കിൽ എങ്കിൽ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<സ്ഥിരം std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            തിരികെ umv->at(i).empty();
        }
        അല്ലെങ്കിൽ എങ്കിൽ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            തിരികെ n == 0;
        } അല്ലെങ്കിൽ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    തിരികെ empty();
}

MatShape നിവേശം_Array_2::shape(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    MatShape shape;

    എങ്കിൽ( k == NONE )
        ;
    അല്ലെങ്കിൽ എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)obj;
        shape = m.size;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം UMat& m = *(സ്ഥിരം UMat*)obj;
        shape = m.size;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_VECTOR_MAT && i >= 0 )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        സ്ഥിരം Mat& m = vv[i];
        shape = m.size;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_ARRAY_MAT && i >= 0 )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert( i < sz.height );
        സ്ഥിരം Mat& m = vv[i];
        shape = m.size;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        സ്ഥിരം UMat& m = vv[i];
        shape = m.size;
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ sizes[CV_MAX_DIM];
        പൂർണ്ണസംഖ്യ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    തിരികെ shape;
}

തർക്കം നിവേശം_Array_2::sameSize(സ്ഥിരം നിവേശം_Array_2& arr) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    എങ്കിൽ( k1 == MAT )
    {
        സ്ഥിരം Mat* m = ((സ്ഥിരം Mat*)obj);
        എങ്കിൽ( k2 == MAT )
            തിരികെ m->size == ((സ്ഥിരം Mat*)arr.obj)->size;
        എങ്കിൽ( k2 == UMAT )
            തിരികെ m->size == ((സ്ഥിരം UMat*)arr.obj)->size;
        എങ്കിൽ( m->dims > 2 )
            തിരികെ അസത്യം;
        sz1 = m->size();
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k1 == UMAT )
    {
        സ്ഥിരം UMat* m = ((സ്ഥിരം UMat*)obj);
        എങ്കിൽ( k2 == MAT )
            തിരികെ m->size == ((സ്ഥിരം Mat*)arr.obj)->size;
        എങ്കിൽ( k2 == UMAT )
            തിരികെ m->size == ((സ്ഥിരം UMat*)arr.obj)->size;
        എങ്കിൽ( m->dims > 2 )
            തിരികെ അസത്യം;
        sz1 = m->size();
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        തിരികെ ((സ്ഥിരം cuda::GpuMatND*)obj)->size == ((സ്ഥിരം cuda::GpuMatND*)arr.obj)->size;
    }
    അല്ലെങ്കിൽ
        sz1 = size();

    എങ്കിൽ( arr.dims() > 2 )
        തിരികെ അസത്യം;
    തിരികെ sz1 == arr.size();
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::dims_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം Mat*)obj)->dims;
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം UMat*)obj)->dims;
    }

    എങ്കിൽ (k == MATX)
    {
        CV_Assert( i < 0 );
        തിരികെ 2;
    }

    എങ്കിൽ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        തിരികെ 1;
    }

    എങ്കിൽ( k == NONE )
        തിരികെ 0;

    എങ്കിൽ( k == STD_VECTOR_VECTOR )
    {
        എങ്കിൽ( i < 0 )
            തിരികെ 1;
        CV_Assert( size_t(i) < total() );
        തിരികെ 1;
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ 1;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );

        തിരികെ vv[i].dims;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ 1;
        CV_Assert( i < sz.height );

        തിരികെ vv[i].dims;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ 1;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );

        തിരികെ vv[i].dims;
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        തിരികെ 2;
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        തിരികെ 2;
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം cuda::GpuMatND*)obj)->dims;
    }

    എങ്കിൽ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        തിരികെ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t നിവേശം_Array_2::total(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം Mat*)obj)->total();
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം UMat*)obj)->total();
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ vv.size();

        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        തിരികെ vv[i].total();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ sz.height;

        CV_Assert( i < sz.height );
        തിരികെ vv[i].total();
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        എങ്കിൽ( i < 0 )
            തിരികെ vv.size();

        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        തിരികെ vv[i].total();
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        സ്ഥിരം cuda::GpuMatND& _gpuMatND = *((സ്ഥിരം cuda::GpuMatND*)obj);
        എങ്കിൽ (_gpuMatND.dims > 0)
        {
            res = 1;
            വേണ്ടി(പൂർണ്ണസംഖ്യ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            തിരികെ res;
        }
    }

    എങ്കിൽ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            എങ്കിൽ (k == STD_VECTOR_VECTOR) { \
                സ്ഥിരം std::vector<std::vector<T > >* vv = (സ്ഥിരം std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                എങ്കിൽ (i < 0) \
                    തിരികെ n; \
                CV_Assert((size_t)i < n); \
                തിരികെ vv->at(i).size(); \
            } അല്ലെങ്കിൽ \
                തിരികെ ((സ്ഥിരം std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    തിരികെ size(i).area();
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::type_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
        തിരികെ ((സ്ഥിരം Mat*)obj)->type();

    എങ്കിൽ( k == UMAT )
        തിരികെ ((സ്ഥിരം UMat*)obj)->type();

    എങ്കിൽ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        തിരികെ CV_MAT_TYPE(flags);

    എങ്കിൽ( k == NONE )
        തിരികെ -1;

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        എങ്കിൽ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            തിരികെ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        തിരികെ vv[i >= 0 ? i : 0].type();
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        എങ്കിൽ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            തിരികെ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)vv.size() );
        തിരികെ vv[i >= 0 ? i : 0].type();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        എങ്കിൽ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            തിരികെ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        തിരികെ vv[i >= 0 ? i : 0].type();
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        സ്ഥിരം std::vector<cuda::GpuMat>& vv = *(സ്ഥിരം std::vector<cuda::GpuMat>*)obj;
        എങ്കിൽ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            തിരികെ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (പൂർണ്ണസംഖ്യ)vv.size());
        തിരികെ vv[i >= 0 ? i : 0].type();
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        എങ്കിൽ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            തിരികെ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (പൂർണ്ണസംഖ്യ)vv.size());
        തിരികെ vv[i >= 0 ? i : 0].type();
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
        തിരികെ ((സ്ഥിരം ogl::Buffer*)obj)->type();

    എങ്കിൽ( k == CUDA_GPU_MAT )
        തിരികെ ((സ്ഥിരം cuda::GpuMat*)obj)->type();

    എങ്കിൽ( k == CUDA_GPU_MATND )
        തിരികെ ((സ്ഥിരം cuda::GpuMatND*)obj)->type();

    എങ്കിൽ( k == CUDA_HOST_MEM )
        തിരികെ ((സ്ഥിരം cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

പൂർണ്ണസംഖ്യ നിവേശം_Array_2::depth_(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    തിരികെ CV_MAT_DEPTH(type(i));
}

തർക്കം നിവേശം_Array_2::empty_() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
        തിരികെ ((സ്ഥിരം Mat*)obj)->empty();

    എങ്കിൽ( k == UMAT )
        തിരികെ ((സ്ഥിരം UMat*)obj)->empty();

    എങ്കിൽ (k == MATX)
        തിരികെ അസത്യം;

    എങ്കിൽ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        തിരികെ n == 0;
    }

    എങ്കിൽ( k == NONE )
        തിരികെ സത്യം;

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        തിരികെ vv.empty();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        തിരികെ sz.height == 0;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        തിരികെ vv.empty();
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
        തിരികെ ((സ്ഥിരം ogl::Buffer*)obj)->empty();

    എങ്കിൽ( k == CUDA_GPU_MAT )
        തിരികെ ((സ്ഥിരം cuda::GpuMat*)obj)->empty();

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        സ്ഥിരം std::vector<cuda::GpuMat>& vv = *(സ്ഥിരം std::vector<cuda::GpuMat>*)obj;
        തിരികെ vv.empty();
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
        തിരികെ ((സ്ഥിരം cuda::GpuMatND*)obj)->empty();

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        തിരികെ vv.empty();
    }

    എങ്കിൽ( k == CUDA_HOST_MEM )
        തിരികെ ((സ്ഥിരം cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

തർക്കം നിവേശം_Array_2::isContinuous(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
        തിരികെ i < 0 ? ((സ്ഥിരം Mat*)obj)->isContinuous() : സത്യം;

    എങ്കിൽ( k == UMAT )
        തിരികെ i < 0 ? ((സ്ഥിരം UMat*)obj)->isContinuous() : സത്യം;

    എങ്കിൽ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        തിരികെ സത്യം;

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].isContinuous();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        തിരികെ vv[i].isContinuous();
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].isContinuous();
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
      തിരികെ i < 0 ? ((സ്ഥിരം cuda::GpuMat*)obj)->isContinuous() : സത്യം;

    എങ്കിൽ( k == CUDA_GPU_MATND )
        തിരികെ i < 0 ? ((സ്ഥിരം cuda::GpuMatND*)obj)->isContinuous() : സത്യം;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

തർക്കം നിവേശം_Array_2::isSubmatrix(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
        തിരികെ i < 0 ? ((സ്ഥിരം Mat*)obj)->isSubmatrix() : അസത്യം;

    എങ്കിൽ( k == UMAT )
        തിരികെ i < 0 ? ((സ്ഥിരം UMat*)obj)->isSubmatrix() : അസത്യം;

    എങ്കിൽ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        തിരികെ അസത്യം;

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].isSubmatrix();
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        തിരികെ vv[i].isSubmatrix();
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].isSubmatrix();
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
        തിരികെ ((സ്ഥിരം cuda::GpuMatND*)obj)->isSubmatrix();
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t നിവേശം_Array_2::offset(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം Mat * സ്ഥിരം m = ((സ്ഥിരം Mat*)obj);
        തിരികെ (size_t)(m->ptr() - m->datastart);
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം UMat*)obj)->offset;
    }

    എങ്കിൽ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        തിരികെ 0;

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (പൂർണ്ണസംഖ്യ)vv.size() );

        തിരികെ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        തിരികെ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].offset;
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        സ്ഥിരം cuda::GpuMat * സ്ഥിരം m = ((സ്ഥിരം cuda::GpuMat*)obj);
        തിരികെ (size_t)(m->data - m->datastart);
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        സ്ഥിരം std::vector<cuda::GpuMat>& vv = *(സ്ഥിരം std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t നിവേശം_Array_2::step(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം Mat*)obj)->step;
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം UMat*)obj)->step;
    }

    എങ്കിൽ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        തിരികെ 0;

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        സ്ഥിരം std::vector<Mat>& vv = *(സ്ഥിരം std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (പൂർണ്ണസംഖ്യ)vv.size() );
        തിരികെ vv[i].step;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        സ്ഥിരം Mat* vv = (സ്ഥിരം Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        തിരികെ vv[i].step;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        സ്ഥിരം std::vector<UMat>& vv = *(സ്ഥിരം std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].step;
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        തിരികെ ((സ്ഥിരം cuda::GpuMat*)obj)->step;
    }
    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        സ്ഥിരം std::vector<cuda::GpuMat>& vv = *(സ്ഥിരം std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        തിരികെ vv[i].step;
    }
    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
        സ്ഥിരം cuda::GpuMatND& _gpuMatND = *(സ്ഥിരം cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        തിരികെ _gpuMatND.step[i];
    }

    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        സ്ഥിരം std::vector<cuda::GpuMatND>& vv = *(സ്ഥിരം std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        സ്ഥിരം cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        തിരികെ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

ശൂന്യം നിവേശം_Array_2::copyTo(സ്ഥിരം _OutputArray& arr) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == NONE )
        arr.release();
    അല്ലെങ്കിൽ എങ്കിൽ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    അല്ലെങ്കിൽ എങ്കിൽ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "");
}

ശൂന്യം നിവേശം_Array_2::copyTo(സ്ഥിരം _OutputArray& arr, സ്ഥിരം നിവേശം_Array_2 & mask) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == NONE )
        arr.release();
    അല്ലെങ്കിൽ എങ്കിൽ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    അല്ലെങ്കിൽ എങ്കിൽ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "");
}

തർക്കം _OutputArray::fixedSize() സ്ഥിരം
{
    തിരികെ (flags & FIXED_SIZE) == FIXED_SIZE;
}

തർക്കം _OutputArray::fixedType() സ്ഥിരം
{
    തിരികെ (flags & FIXED_TYPE) == FIXED_TYPE;
}

ശൂന്യം _OutputArray::create_(Size _sz, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i, തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        തിരികെ;
    }
    എങ്കിൽ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        തിരികെ;
    }
    എങ്കിൽ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    എങ്കിൽ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    എങ്കിൽ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    എങ്കിൽ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    പൂർണ്ണസംഖ്യ sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ശൂന്യം _OutputArray::create_(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i, തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        തിരികെ;
    }
    എങ്കിൽ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        തിരികെ;
    }
    എങ്കിൽ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    എങ്കിൽ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    എങ്കിൽ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    എങ്കിൽ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    പൂർണ്ണസംഖ്യ sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ശൂന്യം _OutputArray::create(പൂർണ്ണസംഖ്യ d, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                          തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    നിവേശം_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    എങ്കിൽ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        എങ്കിൽ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            അല്ലെങ്കിൽ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        എങ്കിൽ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        എങ്കിൽ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            അല്ലെങ്കിൽ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        എങ്കിൽ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ( k == MATX )
    {
        CV_Assert( i < 0 );
        പൂർണ്ണസംഖ്യ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        എങ്കിൽ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            പൂർണ്ണസംഖ്യ total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            അല്ലെങ്കിൽ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(flags);

        എങ്കിൽ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            പൂർണ്ണസംഖ്യ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            എങ്കിൽ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                എങ്കിൽ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    തിരികെ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } അല്ലെങ്കിൽ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        നിർത്തുക

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        തിരികെ;
    }

    എങ്കിൽ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        എങ്കിൽ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            എങ്കിൽ( fixedType() )
            {
                പൂർണ്ണസംഖ്യ _type = CV_MAT_TYPE(flags);
                വേണ്ടി( size_t j = len0; j < len; j++ )
                {
                    എങ്കിൽ( v[j].type() == _type )
                        തുടരുക;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            തിരികെ;
        }

        CV_Assert( i < (പൂർണ്ണസംഖ്യ)v.size() );
        Mat& m = v[i];

        എങ്കിൽ( allowTransposed )
        {
            എങ്കിൽ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            എങ്കിൽ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            അല്ലെങ്കിൽ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        എങ്കിൽ(fixedSize())
        {
            CV_Assert(m.dims == d);
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        എങ്കിൽ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            എങ്കിൽ( fixedType() )
            {
                പൂർണ്ണസംഖ്യ _type = CV_MAT_TYPE(flags);
                വേണ്ടി( size_t j = len0; j < len; j++ )
                {
                    എങ്കിൽ( v[j].type() == _type )
                        തുടരുക;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            തിരികെ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        എങ്കിൽ( allowTransposed )
        {
            എങ്കിൽ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            എങ്കിൽ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            അല്ലെങ്കിൽ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        എങ്കിൽ(fixedSize())
        {
            CV_Assert(m.dims == d);
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        എങ്കിൽ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            എങ്കിൽ( fixedType() )
            {
                പൂർണ്ണസംഖ്യ _type = CV_MAT_TYPE(flags);
                വേണ്ടി( size_t j = len0; j < len; j++ )
                {
                    എങ്കിൽ( v[j].type() == _type )
                        തുടരുക;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            തിരികെ;
        }

        CV_Assert( i < (പൂർണ്ണസംഖ്യ)v.size() );
        UMat& m = v[i];

        എങ്കിൽ( allowTransposed )
        {
            എങ്കിൽ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            എങ്കിൽ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            അല്ലെങ്കിൽ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        എങ്കിൽ(fixedSize())
        {
            CV_Assert(m.dims == d);
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        തിരികെ;
    }

    എങ്കിൽ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ശൂന്യം _OutputArray::create(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                          തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    എങ്കിൽ (shape.dims < 0) {
        release();
    } അല്ലെങ്കിൽ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(പൂർണ്ണസംഖ്യ mtype) സ്ഥിരം
{
    mtype = CV_MAT_TYPE(mtype);
    തിരികെ getMat().reinterpret(mtype);
}

ശൂന്യം _OutputArray::createSameSize(സ്ഥിരം നിവേശം_Array_2& arr, പൂർണ്ണസംഖ്യ mtype) സ്ഥിരം
{
    create(arr.shape(), mtype);
}

ശൂന്യം _OutputArray::fit(Size _sz, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                       തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ശൂന്യം _OutputArray::fit(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                       തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ശൂന്യം _OutputArray::fit(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                       തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    എങ്കിൽ (shape.dims < 0) {
        release();
        തിരികെ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } അല്ലെങ്കിൽ എങ്കിൽ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

ശൂന്യം _OutputArray::fitSameSize(സ്ഥിരം നിവേശം_Array_2& arr, പൂർണ്ണസംഖ്യ mtype) സ്ഥിരം
{
    fit(arr.shape(), mtype);
}

ശൂന്യം _OutputArray::fit(പൂർണ്ണസംഖ്യ d, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ mtype, പൂർണ്ണസംഖ്യ i,
                       തർക്കം allowTransposed, _OutputArray::DepthMask fixedDepthMask) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    നിവേശം_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    എങ്കിൽ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        എങ്കിൽ (k == MAT)
            m = (Mat*)obj;
        അല്ലെങ്കിൽ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        എങ്കിൽ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            അല്ലെങ്കിൽ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        എങ്കിൽ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        തിരികെ;
    }

    എങ്കിൽ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        എങ്കിൽ (k == UMAT)
            m = (UMat*)obj;
        അല്ലെങ്കിൽ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        എങ്കിൽ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            തിരികെ;
        }

        എങ്കിൽ(fixedType())
        {
            എങ്കിൽ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            അല്ലെങ്കിൽ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        എങ്കിൽ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        തിരികെ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ശൂന്യം _OutputArray::release() സ്ഥിരം
{
    CV_Assert(!fixedSize());

    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        ((Mat*)obj)->release();
        തിരികെ;
    }

    എങ്കിൽ( k == UMAT )
    {
        ((UMat*)obj)->release();
        തിരികെ;
    }

    എങ്കിൽ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    എങ്കിൽ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    എങ്കിൽ( k == NONE )
        തിരികെ;

    എങ്കിൽ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        എങ്കിൽ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } അല്ലെങ്കിൽ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        നിർത്തുക

        പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        തിരികെ;
    }

    എങ്കിൽ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < sz.height; i++ )
            v[i].release();
        തിരികെ;
    }

    എങ്കിൽ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<തർക്കം>*)obj)->clear();
        തിരികെ;
    }

    എങ്കിൽ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        തിരികെ;
    }
    എങ്കിൽ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        തിരികെ;
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ശൂന്യം _OutputArray::clear() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        തിരികെ;
    }

    release();
}

തർക്കം _OutputArray::needed() സ്ഥിരം
{
    തിരികെ kind() != NONE;
}

Mat& _OutputArray::getMatRef(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ( i < 0 )
    {
        CV_Assert( k == MAT );
        തിരികെ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    എങ്കിൽ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)v.size() );
        തിരികെ v[i];
    }
    അല്ലെങ്കിൽ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        തിരികെ v[i];
    }
}

UMat& _OutputArray::getUMatRef(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ( i < 0 )
    {
        CV_Assert( k == UMAT );
        തിരികെ *(UMat*)obj;
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (പൂർണ്ണസംഖ്യ)v.size() );
        തിരികെ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    തിരികെ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    തിരികെ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    തിരികെ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    തിരികെ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    തിരികെ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    തിരികെ *(cuda::HostMem*)obj;
}

ശൂന്യം _OutputArray::setTo(സ്ഥിരം നിവേശം_Array_2& arr, സ്ഥിരം നിവേശം_Array_2 & mask) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == NONE )
        ;
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    അല്ലെങ്കിൽ എങ്കിൽ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), നിവേശം_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<ഇരട്ട, 4>(value.ptr<ഇരട്ട>())), mask);
#അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "");
}

ശൂന്യം _OutputArray::setZero() സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();

    എങ്കിൽ( k == NONE )
        ;
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    അല്ലെങ്കിൽ
    {
        setTo(Scalar::all(0), noArray());
    }
}

ശൂന്യം _OutputArray::assign(സ്ഥിരം UMat& u) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ശൂന്യം _OutputArray::assign(സ്ഥിരം Mat& m) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MATX)
    {
        m.copyTo(getMat());
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ശൂന്യം _OutputArray::move(UMat& u) സ്ഥിരം
{
    എങ്കിൽ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        തിരികെ;
    }
    പൂർണ്ണസംഖ്യ k = kind();
    എങ്കിൽ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ശൂന്യം _OutputArray::move(Mat& m) സ്ഥിരം
{
    എങ്കിൽ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        തിരികെ;
    }
    പൂർണ്ണസംഖ്യ k = kind();
    എങ്കിൽ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ശൂന്യം _OutputArray::assign(സ്ഥിരം std::vector<UMat>& v) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        വേണ്ടി (size_t i = 0; i < v.size(); i++)
        {
            സ്ഥിരം UMat& m = v[i];
            UMat& this_m = this_v[i];
            എങ്കിൽ (this_m.u != ശൂന്യ && this_m.u == m.u)
                തുടരുക; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        വേണ്ടി (size_t i = 0; i < v.size(); i++)
        {
            സ്ഥിരം UMat& m = v[i];
            Mat& this_m = this_v[i];
            എങ്കിൽ (this_m.u != ശൂന്യ && this_m.u == m.u)
                തുടരുക; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ശൂന്യം _OutputArray::assign(സ്ഥിരം std::vector<Mat>& v) സ്ഥിരം
{
    നിവേശം_Array_2::KindFlag k = kind();
    എങ്കിൽ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        വേണ്ടി (size_t i = 0; i < v.size(); i++)
        {
            സ്ഥിരം Mat& m = v[i];
            UMat& this_m = this_v[i];
            എങ്കിൽ (this_m.u != ശൂന്യ && this_m.u == m.u)
                തുടരുക; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        വേണ്ടി (size_t i = 0; i < v.size(); i++)
        {
            സ്ഥിരം Mat& m = v[i];
            Mat& this_m = this_v[i];
            എങ്കിൽ (this_m.u != ശൂന്യ && this_m.u == m.u)
                തുടരുക; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


സ്ഥിതം നിവേശം_Output_Array_2 _none;
നിവേശം_Output_Array noArray() { തിരികെ _none; }

} // cv::
