// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/mat.hpp"

నామాకాశ cv {

రకనిర్వచనం Vec<పూర్ణాంకం, 5> Vec5i;
రకనిర్వచనం Vec<పూర్ణాంకం, 7> Vec7i;
రకనిర్వచనం Vec<పూర్ణాంకం, 9> Vec9i;
రకనిర్వచనం Vec<పూర్ణాంకం, 10> Vec10i;
రకనిర్వచనం Vec<పూర్ణాంకం, 11> Vec11i;
రకనిర్వచనం Vec<పూర్ణాంకం, 12> Vec12i;
రకనిర్వచనం Vec<పూర్ణాంకం, 13> Vec13i;
రకనిర్వచనం Vec<పూర్ణాంకం, 14> Vec14i;
రకనిర్వచనం Vec<పూర్ణాంకం, 15> Vec15i;
రకనిర్వచనం Vec<పూర్ణాంకం, 16> Vec16i;
రకనిర్వచనం Vec<పూర్ణాంకం, 32> Vec32i;
రకనిర్వచనం Vec<పూర్ణాంకం, 64> Vec64i;
రకనిర్వచనం Vec<పూర్ణాంకం, 128> Vec128i;

#నిర్వచనం_రద్దు STD_సదిశ_SWITCH
#నిర్వచించు STD_సదిశ_SWITCH(esz, func) \
    ఎంపిక( esz ) \
    { \
    స్థితి 1: func(uchar); \
    స్థితి 2: func(Vec2b); \
    స్థితి 3: func(Vec3b); \
    స్థితి 4: func(పూర్ణాంకం); \
    స్థితి 6: func(Vec3s); \
    స్థితి 8: func(Vec2i); \
    స్థితి 12: func(Vec3i); \
    స్థితి 16: func(Vec4i); \
    స్థితి 20: func(Vec5i); \
    స్థితి 24: func(Vec6i); \
    స్థితి 28: func(Vec7i); \
    స్థితి 32: func(Vec8i); \
    స్థితి 36: func(Vec9i); \
    స్థితి 40: func(Vec10i); \
    స్థితి 44: func(Vec11i); \
    స్థితి 48: func(Vec12i); \
    స్థితి 52: func(Vec13i); \
    స్థితి 56: func(Vec14i); \
    స్థితి 60: func(Vec15i); \
    స్థితి 64: func(Vec16i); \
    స్థితి 128: func(Vec32i); \
    స్థితి 256: func(Vec64i); \
    స్థితి 512: func(Vec128i); \
    అప్రమేయ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat ఆదానం_Array_2::getMat_(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    AccessFlag accessFlags = flags & ACCESS_ముసుగు;

    ఒకవేళ( k == MAT )
    {
        స్థిరరాశి Mat* m = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి *m;
        తిరిగి m->అడ్డువరుస(i);
    }

    ఒకవేళ( k == UMAT )
    {
        స్థిరరాశి UMat* m = (స్థిరరాశి UMat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి m->getMat(accessFlags);
        తిరిగి m->getMat(accessFlags).అడ్డువరుస(i);
    }

    ఒకవేళ (k == MATX)
    {
        CV_Assert( i < 0 );
        తిరిగి Mat(sz, flags, obj);
    }

    ఒకవేళ( k == STD_BOOL_సదిశ )
    {
        CV_Assert( i < 0 );
        పూర్ణాంకం t = CV_8U;
        స్థిరరాశి std::vector<తార్కిక>& v = *(స్థిరరాశి std::vector<తార్కిక>*)obj;
        పూర్ణాంకం j, n = (పూర్ణాంకం)v.size();
        ఒకవేళ( n == 0 )
            తిరిగి Mat();
        Mat m(1, &n, t);
        uchar* dst = m.దత్తాంశం;
        కొరకు( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        తిరిగి m;
    }

    ఒకవేళ( k == NONE )
        తిరిగి Mat();

    ఒకవేళ( k == STD_సదిశ || k == STD_సదిశ_సదిశ )
    {
        పూర్ణాంకం t = CV_MAT_TYPE(flags);
        పూర్ణాంకం esz = CV_ELEM_కొలత(t);

    #నిర్వచనం_రద్దు GET_MAT
    #నిర్వచించు GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ఒకవేళ (k == STD_సదిశ_సదిశ) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } లేకపోతే { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            పూర్ణాంకం v_sz = పూర్ణాంకం(v->size()); \
            తిరిగి v->empty() ? Mat() : Mat(1, &v_sz, t, v->దత్తాంశం()); \
        }

        STD_సదిశ_SWITCH(esz, GET_MAT)
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& v = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (పూర్ణాంకం)v.size() );

        తిరిగి v[i];
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* v = (స్థిరరాశి Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ఎత్తు );

        తిరిగి v[i];
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& v = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (పూర్ణాంకం)v.size() );

        తిరిగి v[i].getMat(accessFlags);
    }

    ఒకవేళ( k == OPENGL_బఫరు )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ఒకవేళ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        స్థిరరాశి cuda::HostMem* cuda_mem = (స్థిరరాశి cuda::HostMem*)obj;

        తిరిగి cuda_mem->create_Mat_శీర్షిక();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ఆదానం_Array_2::getUMat(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    AccessFlag accessFlags = flags & ACCESS_ముసుగు;

    ఒకవేళ( k == UMAT )
    {
        స్థిరరాశి UMat* m = (స్థిరరాశి UMat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి *m;
        తిరిగి m->అడ్డువరుస(i);
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& v = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (పూర్ణాంకం)v.size() );

        తిరిగి v[i];
    }

    ఒకవేళ( k == MAT )
    {
        స్థిరరాశి Mat* m = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి m->getUMat(accessFlags);
        తిరిగి m->అడ్డువరుస(i).getUMat(accessFlags);
    }

    తిరిగి getMat(i).getUMat(accessFlags);
}

శూన్య ఆదానం_Array_2::get_Mat_సదిశ(std::vector<Mat>& mv) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    AccessFlag accessFlags = flags & ACCESS_ముసుగు;

    ఒకవేళ( k == MAT )
    {
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)obj;
        పూర్ణాంకం n = (పూర్ణాంకం)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        కొరకు( పూర్ణాంకం i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.నిలువుల_సంఖ్య, m.type(), (శూన్య*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (శూన్య*)m.ptr(i), &m.అడుగు[1]);
        తిరిగి;
    }

    ఒకవేళ (k == MATX)
    {
        size_t n = sz.ఎత్తు, esz = CV_ELEM_కొలత(flags);
        mv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.వెడల్పు, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.వెడల్పు*i);
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ )
    {
        స్థిరరాశి std::vector<uchar>& v = *(స్థిరరాశి std::vector<uchar>*)obj;

        size_t n = మొత్తం(), esz = CV_ELEM_కొలత(flags);
        పూర్ణాంకం t = CV_MAT_లోతు(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (శూన్య*)(v.దత్తాంశం() + esz*i));
        తిరిగి;
    }

    ఒకవేళ( k == NONE )
    {
        mv.clear();
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_సదిశ )
    {
        పూర్ణాంకం typ = CV_MAT_TYPE(flags);
        పూర్ణాంకం esz = CV_ELEM_కొలత(typ);

        #నిర్వచనం_రద్దు GET_VEC_VEC_MAT_VEC
        #నిర్వచించు GET_VEC_VEC_MAT_VEC(T) \
        { \
            స్థిరరాశి std::vector<std::vector<T > >* vv = (స్థిరరాశి std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            కొరకు (size_t i = 0; i < n; i++) { \
                స్థిరరాశి std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)గరిష్ఠ_పూర్ణాంకం); \
                పూర్ణాంకం ni = (పూర్ణాంకం)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (శూన్య*)vi.దత్తాంశం()) : Mat(); \
            } \
        } \
        ఆపు

        STD_సదిశ_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& v = *(స్థిరరాశి std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        తిరిగి;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* v = (స్థిరరాశి Mat*)obj;
        size_t n = sz.ఎత్తు;
        mv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& v = *(స్థిరరాశి std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        తిరిగి;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

శూన్య ఆదానం_Array_2::get_UMat_సదిశ(std::vector<UMat>& umv) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    AccessFlag accessFlags = flags & ACCESS_ముసుగు;

    ఒకవేళ( k == NONE )
    {
        umv.clear();
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& v = *(స్థిరరాశి std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        తిరిగి;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* v = (స్థిరరాశి Mat*)obj;
        size_t n = sz.ఎత్తు;
        umv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& v = *(స్థిరరాశి std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        కొరకు( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        తిరిగి;
    }

    ఒకవేళ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        తిరిగి;
    }
    ఒకవేళ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        తిరిగి;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat ఆదానం_Array_2::getGpuMat() స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ (k == CUDA_GPU_MAT)
    {
        స్థిరరాశి cuda::GpuMat* d_mat = (స్థిరరాశి cuda::GpuMat*)obj;
        తిరిగి *d_mat;
    }

    ఒకవేళ (k == CUDA_HOST_MEM)
    {
        స్థిరరాశి cuda::HostMem* cuda_mem = (స్థిరరాశి cuda::HostMem*)obj;
        తిరిగి cuda_mem->create_Gpu_Mat_శీర్షిక();
    }

    ఒకవేళ (k == OPENGL_బఫరు)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ఒకవేళ (k == NONE)
        తిరిగి cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#లేకపోతే
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
}
శూన్య ఆదానం_Array_2::get_Gpu_Mat_సదిశ(std::vector<cuda::GpuMat>& gpumv) స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#లేకపోతే
    CV_వాడని(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
}
శూన్య ఆదానం_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#లేకపోతే
    CV_వాడని(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
}
cuda::GpuMatND ఆదానం_Array_2::getGpuMatND() స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ (k == CUDA_GPU_MATND)
    {
        స్థిరరాశి cuda::GpuMatND* d_mat = (స్థిరరాశి cuda::GpuMatND*)obj;
        తిరిగి *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#లేకపోతే
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
}
ogl::బఫరు_2 ఆదానం_Array_2::get_OGl_బఫరు() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    CV_Assert(k == OPENGL_బఫరు);

    స్థిరరాశి ogl::బఫరు_2* gl_buf = (స్థిరరాశి ogl::బఫరు_2*)obj;
    తిరిగి *gl_buf;
}

పూర్ణాంకం ఆదానం_Array_2::అడ్డువరుసలు_2(పూర్ణాంకం i) స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == CUDA_GPU_MATND)
    {
        స్థిరరాశి cuda::GpuMatND& _gpuMatND = *(స్థిరరాశి cuda::GpuMatND*)obj;
        తిరిగి (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#అయితే_ముగింపు

    తిరిగి కొలత_3(i).ఎత్తు;
}

పూర్ణాంకం ఆదానం_Array_2::నిలువుల_సంఖ్య_2(పూర్ణాంకం i) స్థిరరాశి
{
#నిర్వచితమైతే HAVE_CUDA
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == CUDA_GPU_MATND)
    {
        స్థిరరాశి cuda::GpuMatND& _gpuMatND = *(స్థిరరాశి cuda::GpuMatND*)obj;
        తిరిగి (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#అయితే_ముగింపు

    తిరిగి కొలత_3(i).వెడల్పు;
}

కొలత ఆదానం_Array_2::కొలత_3(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి Mat* m = (స్థిరరాశి Mat*)obj;
        CV_Assert(m->dims <= 2);
        తిరిగి కొలత(m->నిలువుల_సంఖ్య, m->అడ్డువరుసలు);
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి UMat* m = (స్థిరరాశి UMat*)obj;
        CV_Assert(m->dims <= 2);
        తిరిగి కొలత(m->నిలువుల_సంఖ్య, m->అడ్డువరుసలు);
    }

    ఒకవేళ (k == MATX)
    {
        CV_Assert( i < 0 );
        తిరిగి sz;
    }

    ఒకవేళ( k == NONE )
        తిరిగి కొలత();

    ఒకవేళ( k == STD_సదిశ || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
    {
        size_t n = మొత్తం(i);
        CV_Assert(n <= (size_t)గరిష్ఠ_పూర్ణాంకం);
        తిరిగి కొలత((పూర్ణాంకం)n, 1);
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి vv.empty() ? కొలత() : కొలత((పూర్ణాంకం)vv.size(), 1);
        CV_Assert( i < (పూర్ణాంకం)vv.size() );

        తిరిగి vv[i].size();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి sz.ఎత్తు==0 ? కొలత() : కొలత(sz.ఎత్తు, 1);
        CV_Assert( i < sz.ఎత్తు );

        తిరిగి vv[i].size();
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
#నిర్వచితమైతే HAVE_CUDA
        స్థిరరాశి std::vector<cuda::GpuMat>& vv = *(స్థిరరాశి std::vector<cuda::GpuMat>*)obj;
        ఒకవేళ (i < 0)
            తిరిగి vv.empty() ? కొలత() : కొలత((పూర్ణాంకం)vv.size(), 1);
        CV_Assert(i < (పూర్ణాంకం)vv.size());
        తిరిగి vv[i].size();
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
#నిర్వచితమైతే HAVE_CUDA
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        ఒకవేళ (i < 0)
            తిరిగి vv.empty() ? కొలత() : కొలత((పూర్ణాంకం)vv.size(), 1);
        CV_Assert(i < (పూర్ణాంకం)vv.size());
        స్థిరరాశి cuda::GpuMatND& m = vv[i];
        ఒకవేళ (m.dims == 0)
            తిరిగి కొలత();
        ఒకవేళ (m.dims == 1)
            తిరిగి కొలత(1, m.size[0]);
        తిరిగి కొలత(m.size[1], m.size[0]);
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి vv.empty() ? కొలత() : కొలత((పూర్ణాంకం)vv.size(), 1);
        CV_Assert( i < (పూర్ణాంకం)vv.size() );

        తిరిగి vv[i].size();
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
#నిర్వచితమైతే HAVE_CUDA
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        ఒకవేళ (i < 0)
            తిరిగి vv.empty() ? కొలత() : కొలత((పూర్ణాంకం)vv.size(), 1);
        CV_Assert(i < (పూర్ణాంకం)vv.size());
        స్థిరరాశి cuda::GpuMatND& m = vv[i];
        ఒకవేళ (m.dims == 0)
            తిరిగి కొలత();
        ఒకవేళ (m.dims == 1)
            తిరిగి కొలత(1, m.size[0]);
        తిరిగి కొలత(m.size[1], m.size[0]);
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == OPENGL_బఫరు )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి ogl::బఫరు_2* buf = (స్థిరరాశి ogl::బఫరు_2*)obj;
        తిరిగి buf->size();
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి cuda::GpuMat* d_mat = (స్థిరరాశి cuda::GpuMat*)obj;
        తిరిగి d_mat->size();
    }

    ఒకవేళ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి cuda::HostMem* cuda_mem = (స్థిరరాశి cuda::HostMem*)obj;
        తిరిగి cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

పూర్ణాంకం ఆదానం_Array_2::sizend(పూర్ణాంకం* arrsz, పూర్ణాంకం i) స్థిరరాశి
{
    పూర్ణాంకం j, d = 0;
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == NONE )
        ;
    లేకపోతే ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)obj;
        d = m.dims;
        ఒకవేళ(arrsz)
            కొరకు(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    లేకపోతే ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి UMat& m = *(స్థిరరాశి UMat*)obj;
        d = m.dims;
        ఒకవేళ(arrsz)
            కొరకు(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    లేకపోతే ఒకవేళ( k == STD_సదిశ_MAT && i >= 0 )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        స్థిరరాశి Mat& m = vv[i];
        d = m.dims;
        ఒకవేళ(arrsz)
            కొరకు(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    లేకపోతే ఒకవేళ( k == STD_ARRAY_MAT && i >= 0 )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert( i < sz.ఎత్తు );
        స్థిరరాశి Mat& m = vv[i];
        d = m.dims;
        ఒకవేళ(arrsz)
            కొరకు(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    లేకపోతే ఒకవేళ( k == STD_సదిశ_UMAT && i >= 0 )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        స్థిరరాశి UMat& m = vv[i];
        d = m.dims;
        ఒకవేళ(arrsz)
            కొరకు(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    లేకపోతే ఒకవేళ (((k == STD_సదిశ || k == STD_BOOL_సదిశ || k == STD_సదిశ_సదిశ) && i < 0) ||
             (k == STD_సదిశ_సదిశ && i >= 0))
    {
        size_t n = మొత్తం(i);
        d = 1;
        ఒకవేళ(arrsz)
        {
            arrsz[0] = పూర్ణాంకం(n);
        }
    }
    లేకపోతే
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        కొలత sz2d = size(i);
        d = 2;
        ఒకవేళ(arrsz)
        {
            arrsz[0] = sz2d.ఎత్తు;
            arrsz[1] = sz2d.వెడల్పు;
        }
    }

    తిరిగి d;
}

తార్కిక ఆదానం_Array_2::empty(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (i >= 0) {
        ఒకవేళ (k == STD_సదిశ_MAT) {
            స్వయంచాలక mv = reinterpret_cast<స్థిరరాశి std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            తిరిగి mv->at(i).empty();
        }
        లేకపోతే ఒకవేళ (k == STD_సదిశ_UMAT) {
            స్వయంచాలక umv = reinterpret_cast<స్థిరరాశి std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            తిరిగి umv->at(i).empty();
        }
        లేకపోతే ఒకవేళ (k == STD_సదిశ || k == STD_BOOL_సదిశ || k == STD_సదిశ_సదిశ) {
            size_t n = మొత్తం(i);
            తిరిగి n == 0;
        } లేకపోతే {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    తిరిగి empty();
}

MatShape ఆదానం_Array_2::shape(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    MatShape shape;

    ఒకవేళ( k == NONE )
        ;
    లేకపోతే ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)obj;
        shape = m.size;
    }
    లేకపోతే ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి UMat& m = *(స్థిరరాశి UMat*)obj;
        shape = m.size;
    }
    లేకపోతే ఒకవేళ( k == STD_సదిశ_MAT && i >= 0 )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        స్థిరరాశి Mat& m = vv[i];
        shape = m.size;
    }
    లేకపోతే ఒకవేళ( k == STD_ARRAY_MAT && i >= 0 )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert( i < sz.ఎత్తు );
        స్థిరరాశి Mat& m = vv[i];
        shape = m.size;
    }
    లేకపోతే ఒకవేళ( k == STD_సదిశ_UMAT && i >= 0 )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        స్థిరరాశి UMat& m = vv[i];
        shape = m.size;
    }
    లేకపోతే
    {
        పూర్ణాంకం sizes[CV_MAX_DIM];
        పూర్ణాంకం dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    తిరిగి shape;
}

తార్కిక ఆదానం_Array_2::same_కొలత(స్థిరరాశి ఆదానం_Array_2& arr) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k1 = రకం(), k2 = arr.రకం();
    కొలత sz1;

    ఒకవేళ( k1 == MAT )
    {
        స్థిరరాశి Mat* m = ((స్థిరరాశి Mat*)obj);
        ఒకవేళ( k2 == MAT )
            తిరిగి m->size == ((స్థిరరాశి Mat*)arr.obj)->size;
        ఒకవేళ( k2 == UMAT )
            తిరిగి m->size == ((స్థిరరాశి UMat*)arr.obj)->size;
        ఒకవేళ( m->dims > 2 )
            తిరిగి అబద్ధం;
        sz1 = m->size();
    }
    లేకపోతే ఒకవేళ( k1 == UMAT )
    {
        స్థిరరాశి UMat* m = ((స్థిరరాశి UMat*)obj);
        ఒకవేళ( k2 == MAT )
            తిరిగి m->size == ((స్థిరరాశి Mat*)arr.obj)->size;
        ఒకవేళ( k2 == UMAT )
            తిరిగి m->size == ((స్థిరరాశి UMat*)arr.obj)->size;
        ఒకవేళ( m->dims > 2 )
            తిరిగి అబద్ధం;
        sz1 = m->size();
    }
    లేకపోతే ఒకవేళ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        తిరిగి ((స్థిరరాశి cuda::GpuMatND*)obj)->size == ((స్థిరరాశి cuda::GpuMatND*)arr.obj)->size;
    }
    లేకపోతే
        sz1 = size();

    ఒకవేళ( arr.dims() > 2 )
        తిరిగి అబద్ధం;
    తిరిగి sz1 == arr.size();
}

పూర్ణాంకం ఆదానం_Array_2::dims_(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి Mat*)obj)->dims;
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి UMat*)obj)->dims;
    }

    ఒకవేళ (k == MATX)
    {
        CV_Assert( i < 0 );
        తిరిగి 2;
    }

    ఒకవేళ( k == STD_సదిశ || k == STD_BOOL_సదిశ )
    {
        CV_Assert( i < 0 );
        తిరిగి 1;
    }

    ఒకవేళ( k == NONE )
        తిరిగి 0;

    ఒకవేళ( k == STD_సదిశ_సదిశ )
    {
        ఒకవేళ( i < 0 )
            తిరిగి 1;
        CV_Assert( size_t(i) < మొత్తం() );
        తిరిగి 1;
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి 1;
        CV_Assert( i < (పూర్ణాంకం)vv.size() );

        తిరిగి vv[i].dims;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి 1;
        CV_Assert( i < sz.ఎత్తు );

        తిరిగి vv[i].dims;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి 1;
        CV_Assert( i < (పూర్ణాంకం)vv.size() );

        తిరిగి vv[i].dims;
    }

    ఒకవేళ( k == OPENGL_బఫరు )
    {
        CV_Assert( i < 0 );
        తిరిగి 2;
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        తిరిగి 2;
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి cuda::GpuMatND*)obj)->dims;
    }

    ఒకవేళ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        తిరిగి 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t ఆదానం_Array_2::మొత్తం(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి Mat*)obj)->మొత్తం();
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి UMat*)obj)->మొత్తం();
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి vv.size();

        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        తిరిగి vv[i].మొత్తం();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి sz.ఎత్తు;

        CV_Assert( i < sz.ఎత్తు );
        తిరిగి vv[i].మొత్తం();
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        ఒకవేళ( i < 0 )
            తిరిగి vv.size();

        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        తిరిగి vv[i].మొత్తం();
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        స్థిరరాశి cuda::GpuMatND& _gpuMatND = *((స్థిరరాశి cuda::GpuMatND*)obj);
        ఒకవేళ (_gpuMatND.dims > 0)
        {
            res = 1;
            కొరకు(పూర్ణాంకం d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            తిరిగి res;
        }
    }

    ఒకవేళ (k == STD_సదిశ || k == STD_సదిశ_సదిశ)
    {
        CV_Assert(i < 0 || k == STD_సదిశ_సదిశ);
        పూర్ణాంకం esz = CV_ELEM_కొలత(flags);

        #నిర్వచనం_రద్దు GET_VEC_కొలత
        #నిర్వచించు GET_VEC_కొలత(T) \
            ఒకవేళ (k == STD_సదిశ_సదిశ) { \
                స్థిరరాశి std::vector<std::vector<T > >* vv = (స్థిరరాశి std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ఒకవేళ (i < 0) \
                    తిరిగి n; \
                CV_Assert((size_t)i < n); \
                తిరిగి vv->at(i).size(); \
            } లేకపోతే \
                తిరిగి ((స్థిరరాశి std::vector<T >*)obj)->size()

        STD_సదిశ_SWITCH(esz, GET_VEC_కొలత)
    }

    తిరిగి size(i).area();
}

పూర్ణాంకం ఆదానం_Array_2::type_(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
        తిరిగి ((స్థిరరాశి Mat*)obj)->type();

    ఒకవేళ( k == UMAT )
        తిరిగి ((స్థిరరాశి UMat*)obj)->type();

    ఒకవేళ( k == MATX || k == STD_సదిశ || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
        తిరిగి CV_MAT_TYPE(flags);

    ఒకవేళ( k == NONE )
        తిరిగి -1;

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        ఒకవేళ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            తిరిగి CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        తిరిగి vv[i >= 0 ? i : 0].type();
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        ఒకవేళ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            తిరిగి CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (పూర్ణాంకం)vv.size() );
        తిరిగి vv[i >= 0 ? i : 0].type();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        ఒకవేళ( sz.ఎత్తు == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            తిరిగి CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.ఎత్తు );
        తిరిగి vv[i >= 0 ? i : 0].type();
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
#నిర్వచితమైతే HAVE_CUDA
        స్థిరరాశి std::vector<cuda::GpuMat>& vv = *(స్థిరరాశి std::vector<cuda::GpuMat>*)obj;
        ఒకవేళ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            తిరిగి CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (పూర్ణాంకం)vv.size());
        తిరిగి vv[i >= 0 ? i : 0].type();
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
#నిర్వచితమైతే HAVE_CUDA
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        ఒకవేళ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            తిరిగి CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (పూర్ణాంకం)vv.size());
        తిరిగి vv[i >= 0 ? i : 0].type();
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == OPENGL_బఫరు )
        తిరిగి ((స్థిరరాశి ogl::బఫరు_2*)obj)->type();

    ఒకవేళ( k == CUDA_GPU_MAT )
        తిరిగి ((స్థిరరాశి cuda::GpuMat*)obj)->type();

    ఒకవేళ( k == CUDA_GPU_MATND )
        తిరిగి ((స్థిరరాశి cuda::GpuMatND*)obj)->type();

    ఒకవేళ( k == CUDA_HOST_MEM )
        తిరిగి ((స్థిరరాశి cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

పూర్ణాంకం ఆదానం_Array_2::లోతు_4(పూర్ణాంకం i) స్థిరరాశి
{
    తిరిగి CV_MAT_లోతు(type(i));
}

తార్కిక ఆదానం_Array_2::ఖాళీలు() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
        తిరిగి ((స్థిరరాశి Mat*)obj)->empty();

    ఒకవేళ( k == UMAT )
        తిరిగి ((స్థిరరాశి UMat*)obj)->empty();

    ఒకవేళ (k == MATX)
        తిరిగి అబద్ధం;

    ఒకవేళ( k == STD_సదిశ || k == STD_BOOL_సదిశ || k == STD_సదిశ_సదిశ )
    {
        size_t n = మొత్తం(-1);
        తిరిగి n == 0;
    }

    ఒకవేళ( k == NONE )
        తిరిగి నిజం;

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        తిరిగి vv.empty();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        తిరిగి sz.ఎత్తు == 0;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        తిరిగి vv.empty();
    }

    ఒకవేళ( k == OPENGL_బఫరు )
        తిరిగి ((స్థిరరాశి ogl::బఫరు_2*)obj)->empty();

    ఒకవేళ( k == CUDA_GPU_MAT )
        తిరిగి ((స్థిరరాశి cuda::GpuMat*)obj)->empty();

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
        స్థిరరాశి std::vector<cuda::GpuMat>& vv = *(స్థిరరాశి std::vector<cuda::GpuMat>*)obj;
        తిరిగి vv.empty();
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
        తిరిగి ((స్థిరరాశి cuda::GpuMatND*)obj)->empty();

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        తిరిగి vv.empty();
    }

    ఒకవేళ( k == CUDA_HOST_MEM )
        తిరిగి ((స్థిరరాశి cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

తార్కిక ఆదానం_Array_2::isContinuous(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
        తిరిగి i < 0 ? ((స్థిరరాశి Mat*)obj)->isContinuous() : నిజం;

    ఒకవేళ( k == UMAT )
        తిరిగి i < 0 ? ((స్థిరరాశి UMat*)obj)->isContinuous() : నిజం;

    ఒకవేళ( k == MATX || k == STD_సదిశ ||
        k == NONE || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
        తిరిగి నిజం;

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].isContinuous();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ఎత్తు);
        తిరిగి vv[i].isContinuous();
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].isContinuous();
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
      తిరిగి i < 0 ? ((స్థిరరాశి cuda::GpuMat*)obj)->isContinuous() : నిజం;

    ఒకవేళ( k == CUDA_GPU_MATND )
        తిరిగి i < 0 ? ((స్థిరరాశి cuda::GpuMatND*)obj)->isContinuous() : నిజం;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

తార్కిక ఆదానం_Array_2::isSubmatrix(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
        తిరిగి i < 0 ? ((స్థిరరాశి Mat*)obj)->isSubmatrix() : అబద్ధం;

    ఒకవేళ( k == UMAT )
        తిరిగి i < 0 ? ((స్థిరరాశి UMat*)obj)->isSubmatrix() : అబద్ధం;

    ఒకవేళ( k == MATX || k == STD_సదిశ ||
        k == NONE || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
        తిరిగి అబద్ధం;

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].isSubmatrix();
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ఎత్తు);
        తిరిగి vv[i].isSubmatrix();
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].isSubmatrix();
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
    {
        తిరిగి ((స్థిరరాశి cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ఆదానం_Array_2::offset(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి Mat * స్థిరరాశి m = ((స్థిరరాశి Mat*)obj);
        తిరిగి (size_t)(m->ptr() - m->datastart);
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి UMat*)obj)->offset;
    }

    ఒకవేళ( k == MATX || k == STD_సదిశ ||
        k == NONE || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
        తిరిగి 0;

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (పూర్ణాంకం)vv.size() );

        తిరిగి (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ఎత్తు );
        తిరిగి (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].offset;
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        స్థిరరాశి cuda::GpuMat * స్థిరరాశి m = ((స్థిరరాశి cuda::GpuMat*)obj);
        తిరిగి (size_t)(m->దత్తాంశం - m->datastart);
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
        స్థిరరాశి std::vector<cuda::GpuMat>& vv = *(స్థిరరాశి std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి (size_t)(vv[i].దత్తాంశం - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ఆదానం_Array_2::అడుగు(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి Mat*)obj)->అడుగు;
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి UMat*)obj)->అడుగు;
    }

    ఒకవేళ( k == MATX || k == STD_సదిశ ||
        k == NONE || k == STD_సదిశ_సదిశ || k == STD_BOOL_సదిశ )
        తిరిగి 0;

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        స్థిరరాశి std::vector<Mat>& vv = *(స్థిరరాశి std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (పూర్ణాంకం)vv.size() );
        తిరిగి vv[i].అడుగు;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        స్థిరరాశి Mat* vv = (స్థిరరాశి Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ఎత్తు );
        తిరిగి vv[i].అడుగు;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        స్థిరరాశి std::vector<UMat>& vv = *(స్థిరరాశి std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].అడుగు;
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        తిరిగి ((స్థిరరాశి cuda::GpuMat*)obj)->అడుగు;
    }
    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
        స్థిరరాశి std::vector<cuda::GpuMat>& vv = *(స్థిరరాశి std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        తిరిగి vv[i].అడుగు;
    }
    ఒకవేళ( k == CUDA_GPU_MATND )
    {
        స్థిరరాశి cuda::GpuMatND& _gpuMatND = *(స్థిరరాశి cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        తిరిగి _gpuMatND.అడుగు[i];
    }

    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT_ND)
    {
        స్థిరరాశి std::vector<cuda::GpuMatND>& vv = *(స్థిరరాశి std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        స్థిరరాశి cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        తిరిగి _gpuMatND.అడుగు[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

శూన్య ఆదానం_Array_2::copy_గమ్యం(స్థిరరాశి _OutputArray& arr) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == NONE )
        arr.release();
    లేకపోతే ఒకవేళ( k == MAT || k == MATX || k == STD_సదిశ || k == STD_BOOL_సదిశ )
    {
        Mat m = getMat();
        m.copy_గమ్యం(arr);
    }
    లేకపోతే ఒకవేళ( k == UMAT )
        ((UMat*)obj)->copy_గమ్యం(arr);
#నిర్వచితమైతే HAVE_CUDA
    లేకపోతే ఒకవేళ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_గమ్యం(arr);
#అయితే_ముగింపు
    లేకపోతే
        CV_Error(Error::StsNotImplemented, "");
}

శూన్య ఆదానం_Array_2::copy_గమ్యం(స్థిరరాశి _OutputArray& arr, స్థిరరాశి ఆదానం_Array_2 & ముసుగు) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == NONE )
        arr.release();
    లేకపోతే ఒకవేళ( k == MAT || k == MATX || k == STD_సదిశ || k == STD_BOOL_సదిశ )
    {
        Mat m = getMat();
        m.copy_గమ్యం(arr, ముసుగు);
    }
    లేకపోతే ఒకవేళ( k == UMAT )
        ((UMat*)obj)->copy_గమ్యం(arr, ముసుగు);
#నిర్వచితమైతే HAVE_CUDA
    లేకపోతే ఒకవేళ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_గమ్యం(arr, ముసుగు);
#అయితే_ముగింపు
    లేకపోతే
        CV_Error(Error::StsNotImplemented, "");
}

తార్కిక _OutputArray::fixed_కొలత() స్థిరరాశి
{
    తిరిగి (flags & FIXED_కొలత) == FIXED_కొలత;
}

తార్కిక _OutputArray::fixedType() స్థిరరాశి
{
    తిరిగి (flags & FIXED_TYPE) == FIXED_TYPE;
}

శూన్య _OutputArray::create_(కొలత _sz, పూర్ణాంకం mtype, పూర్ణాంకం i, తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ( k == MAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((Mat*)obj)->size.క్రియాకారకం()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        తిరిగి;
    }
    ఒకవేళ( k == UMAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((UMat*)obj)->size.క్రియాకారకం()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        తిరిగి;
    }
    ఒకవేళ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.ఎత్తు) && (((cuda::GpuMatND*)obj)->size[1] == _sz.వెడల్పు)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        cuda::GpuMatND::కొలత_Array sizes({_sz.ఎత్తు, _sz.వెడల్పు});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == OPENGL_బఫరు && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((ogl::బఫరు_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::బఫరు_2*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_OPENGL
        ((ogl::బఫరు_2*)obj)->create(_sz, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    పూర్ణాంకం sizes[] = {_sz.ఎత్తు, _sz.వెడల్పు};
    create(2, sizes, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
}

శూన్య _OutputArray::create_(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం mtype, పూర్ణాంకం i, తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ( k == MAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((Mat*)obj)->size.క్రియాకారకం()() == కొలత(నిలువుల_సంఖ్య_3, అడ్డువరుసలు_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, mtype);
        తిరిగి;
    }
    ఒకవేళ( k == UMAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((UMat*)obj)->size.క్రియాకారకం()() == కొలత(నిలువుల_సంఖ్య_3, అడ్డువరుసలు_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, mtype);
        తిరిగి;
    }
    ఒకవేళ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((cuda::GpuMat*)obj)->size() == కొలత(నిలువుల_సంఖ్య_3, అడ్డువరుసలు_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == అడ్డువరుసలు_3) && (((cuda::GpuMatND*)obj)->size[1] == నిలువుల_సంఖ్య_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        cuda::GpuMatND::కొలత_Array sizes({అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == OPENGL_బఫరు && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((ogl::బఫరు_2*)obj)->size() == కొలత(నిలువుల_సంఖ్య_3, అడ్డువరుసలు_3));
        CV_Assert(!fixedType() || ((ogl::బఫరు_2*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_OPENGL
        ((ogl::బఫరు_2*)obj)->create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#అయితే_ముగింపు
    }
    ఒకవేళ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
        CV_Assert(!fixed_కొలత() || ((cuda::HostMem*)obj)->size() == కొలత(నిలువుల_సంఖ్య_3, అడ్డువరుసలు_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::HostMem*)obj)->create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    పూర్ణాంకం sizes[] = {అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
}

శూన్య _OutputArray::create(పూర్ణాంకం d, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం mtype, పూర్ణాంకం i,
                          తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    పూర్ణాంకం size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ఆదానం_Array_2::రకం_Flag k = రకం();
    mtype = CV_MAT_TYPE(mtype);

    ఒకవేళ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_కొలత()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ఒకవేళ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.అడ్డువరుసలు == size0 && m.నిలువుల_సంఖ్య == size1) ||
            (allowTransposed && m.అడ్డువరుసలు == size1 && m.నిలువుల_సంఖ్య == size0 && m.isContinuous())))
        {
            తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_లోతు(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m.type();
            లేకపోతే
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_కొలత()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ఒకవేళ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.అడ్డువరుసలు == size0 && m.నిలువుల_సంఖ్య == size1) ||
            (allowTransposed && m.అడ్డువరుసలు == size1 && m.నిలువుల_సంఖ్య == size0 && m.isContinuous())))
        {
            తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m.type();
            లేకపోతే
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ( k == MATX )
    {
        CV_Assert( i < 0 );
        పూర్ణాంకం type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_లోతు_ముసుగు) != 0) );
        CV_CheckLE(d, 2, "");
        కొలత requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ఒకవేళ (sz.వెడల్పు == 1 || sz.ఎత్తు == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            పూర్ణాంకం మొత్తం_1d = std::max(sz.వెడల్పు, sz.ఎత్తు);
            CV_Check(requested_size, std::max(requested_size.వెడల్పు, requested_size.ఎత్తు) == మొత్తం_1d, "");
        }
        లేకపోతే
        {
            ఒకవేళ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            లేకపోతే
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.ఎత్తు == sz.వెడల్పు && requested_size.వెడల్పు == sz.ఎత్తు)),
                        "");
            }
        }
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ || k == STD_సదిశ_సదిశ )
    {
        CV_Assert(!fixed_కొలత());
        CV_Assert(k == STD_సదిశ_సదిశ || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        పూర్ణాంకం esz = CV_ELEM_కొలత(flags);

        ఒకవేళ( k == STD_సదిశ || (k == STD_సదిశ_సదిశ && i >= 0) )
        {
            పూర్ణాంకం type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_లోతు_ముసుగు) != 0) );
        }

    #నిర్వచనం_రద్దు RESIZE_VEC
    #నిర్వచించు RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ఒకవేళ (k == STD_సదిశ_సదిశ) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ఒకవేళ (i < 0) { \
                    CV_Assert(!fixed_కొలత() || len == vv->size()); \
                    vv->resize(len); \
                    తిరిగి; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } లేకపోతే { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_కొలత() || len == v->size()); \
            v->resize(len); \
        } \
        ఆపు

        STD_సదిశ_SWITCH(esz, RESIZE_VEC)
        తిరిగి;
    }

    ఒకవేళ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ఒకవేళ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_కొలత() || len == len0);
            v.resize(len);
            ఒకవేళ( fixedType() )
            {
                పూర్ణాంకం _type = CV_MAT_TYPE(flags);
                కొరకు( size_t j = len0; j < len; j++ )
                {
                    ఒకవేళ( v[j].type() == _type )
                        కొనసాగు;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ముసుగు) | _type;
                }
            }
            తిరిగి;
        }

        CV_Assert( i < (పూర్ణాంకం)v.size() );
        Mat& m = v[i];

        ఒకవేళ( allowTransposed )
        {
            ఒకవేళ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_కొలత());
                m.release();
            }

            ఒకవేళ( d == 2 && m.dims == 2 && m.దత్తాంశం &&
                m.type() == mtype && m.అడ్డువరుసలు == sizes[1] && m.నిలువుల_సంఖ్య == sizes[0] )
                తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m.type();
            లేకపోతే
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_Assert(m.dims == d);
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ఒకవేళ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.ఎత్తు;

            CV_Assert(len == len0);
            ఒకవేళ( fixedType() )
            {
                పూర్ణాంకం _type = CV_MAT_TYPE(flags);
                కొరకు( size_t j = len0; j < len; j++ )
                {
                    ఒకవేళ( v[j].type() == _type )
                        కొనసాగు;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ముసుగు) | _type;
                }
            }
            తిరిగి;
        }

        CV_Assert( i < sz.ఎత్తు );
        Mat& m = v[i];

        ఒకవేళ( allowTransposed )
        {
            ఒకవేళ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_కొలత());
                m.release();
            }

            ఒకవేళ( d == 2 && m.dims == 2 && m.దత్తాంశం &&
                m.type() == mtype && m.అడ్డువరుసలు == sizes[1] && m.నిలువుల_సంఖ్య == sizes[0] )
                తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m.type();
            లేకపోతే
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ఒకవేళ(fixed_కొలత())
        {
            CV_Assert(m.dims == d);
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ఒకవేళ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_కొలత() || len == len0);
            v.resize(len);
            ఒకవేళ( fixedType() )
            {
                పూర్ణాంకం _type = CV_MAT_TYPE(flags);
                కొరకు( size_t j = len0; j < len; j++ )
                {
                    ఒకవేళ( v[j].type() == _type )
                        కొనసాగు;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ముసుగు) | _type;
                }
            }
            తిరిగి;
        }

        CV_Assert( i < (పూర్ణాంకం)v.size() );
        UMat& m = v[i];

        ఒకవేళ( allowTransposed )
        {
            ఒకవేళ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_కొలత());
                m.release();
            }

            ఒకవేళ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.అడ్డువరుసలు == sizes[1] && m.నిలువుల_సంఖ్య == sizes[0] )
                తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m.type();
            లేకపోతే
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_Assert(m.dims == d);
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
        తిరిగి;
    }

    ఒకవేళ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_లోతు_ముసుగు == 0 )
    {
#నిర్వచితమైతే HAVE_CUDA
        cuda::GpuMatND::కొలత_Array కొలత_Array_2 = cuda::GpuMatND::కొలత_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(కొలత_Array_2, mtype);
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

శూన్య _OutputArray::create(స్థిరరాశి MatShape& shape, పూర్ణాంకం mtype, పూర్ణాంకం i,
                          తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    ఒకవేళ (shape.dims < 0) {
        release();
    } లేకపోతే {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
    }
}

Mat _OutputArray::reinterpret(పూర్ణాంకం mtype) స్థిరరాశి
{
    mtype = CV_MAT_TYPE(mtype);
    తిరిగి getMat().reinterpret(mtype);
}

శూన్య _OutputArray::create_Same_కొలత(స్థిరరాశి ఆదానం_Array_2& arr, పూర్ణాంకం mtype) స్థిరరాశి
{
    create(arr.shape(), mtype);
}

శూన్య _OutputArray::fit(కొలత _sz, పూర్ణాంకం mtype, పూర్ణాంకం i,
                       తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    పూర్ణాంకం sizes[] = {_sz.ఎత్తు, _sz.వెడల్పు};
    fit(2, sizes, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
}

శూన్య _OutputArray::fit(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం mtype, పూర్ణాంకం i,
                       తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    పూర్ణాంకం sizes[] = {అడ్డువరుసలు, నిలువుల_సంఖ్య};
    fit(2, sizes, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
}

శూన్య _OutputArray::fit(స్థిరరాశి MatShape& shape, పూర్ణాంకం mtype, పూర్ణాంకం i,
                       తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    ఒకవేళ (shape.dims < 0) {
        release();
        తిరిగి;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ ((k == MAT && i < 0) || (k == STD_సదిశ_MAT && i >= 0)) {
        Mat_కొలత& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } లేకపోతే ఒకవేళ ((k == UMAT && i < 0) || (k == STD_సదిశ_UMAT && i >= 0)) {
        Mat_కొలత& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

శూన్య _OutputArray::fit_Same_కొలత(స్థిరరాశి ఆదానం_Array_2& arr, పూర్ణాంకం mtype) స్థిరరాశి
{
    fit(arr.shape(), mtype);
}

శూన్య _OutputArray::fit(పూర్ణాంకం d, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం mtype, పూర్ణాంకం i,
                       తార్కిక allowTransposed, _OutputArray::లోతు_ముసుగు fixed_లోతు_ముసుగు) స్థిరరాశి
{
    పూర్ణాంకం size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ఆదానం_Array_2::రకం_Flag k = రకం();
    mtype = CV_MAT_TYPE(mtype);

    ఒకవేళ( (k == MAT && i < 0) || (k == STD_సదిశ_MAT && i >= 0) )
    {
        Mat* m;
        ఒకవేళ (k == MAT)
            m = (Mat*)obj;
        లేకపోతే {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_కొలత()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ఒకవేళ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->అడ్డువరుసలు == size0 && m->నిలువుల_సంఖ్య == size1) ||
             (allowTransposed && m->అడ్డువరుసలు == size1 && m->నిలువుల_సంఖ్య == size0 && m->isContinuous())))
        {
            తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_లోతు(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m->type();
            లేకపోతే
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        తిరిగి;
    }

    ఒకవేళ( (k == UMAT && i < 0) || (k == STD_సదిశ_UMAT && i >= 0) )
    {
        UMat* m;
        ఒకవేళ (k == UMAT)
            m = (UMat*)obj;
        లేకపోతే {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_కొలత()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ఒకవేళ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->అడ్డువరుసలు == size0 && m->నిలువుల_సంఖ్య == size1) ||
             (allowTransposed && m->అడ్డువరుసలు == size1 && m->నిలువుల_సంఖ్య == size0 && m->isContinuous())))
        {
            తిరిగి;
        }

        ఒకవేళ(fixedType())
        {
            ఒకవేళ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_లోతు(flags)) & fixed_లోతు_ముసుగు) != 0 )
                mtype = m->type();
            లేకపోతే
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ఒకవేళ(fixed_కొలత())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            కొరకు(పూర్ణాంకం j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        తిరిగి;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_లోతు_ముసుగు);
}

శూన్య _OutputArray::release() స్థిరరాశి
{
    CV_Assert(!fixed_కొలత());

    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        ((Mat*)obj)->release();
        తిరిగి;
    }

    ఒకవేళ( k == UMAT )
    {
        ((UMat*)obj)->release();
        తిరిగి;
    }

    ఒకవేళ( k == CUDA_GPU_MAT )
    {
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == CUDA_GPU_MATND )
    {
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == CUDA_HOST_MEM )
    {
#నిర్వచితమైతే HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == OPENGL_బఫరు )
    {
#నిర్వచితమైతే HAVE_OPENGL
        ((ogl::బఫరు_2*)obj)->release();
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#అయితే_ముగింపు
    }

    ఒకవేళ( k == NONE )
        తిరిగి;

    ఒకవేళ( k == STD_సదిశ || k == STD_సదిశ_సదిశ )
    {
    #నిర్వచనం_రద్దు CLEAR_VEC
    #నిర్వచించు CLEAR_VEC(T) \
        ఒకవేళ (k == STD_సదిశ_సదిశ) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } లేకపోతే { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ఆపు

        పూర్ణాంకం esz = CV_ELEM_కొలత(flags);
        STD_సదిశ_SWITCH(esz, CLEAR_VEC);
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        తిరిగి;
    }

    ఒకవేళ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        కొరకు( పూర్ణాంకం i = 0; i < sz.ఎత్తు; i++ )
            v[i].release();
        తిరిగి;
    }

    ఒకవేళ( k == STD_BOOL_సదిశ )
    {
        ((std::vector<తార్కిక>*)obj)->clear();
        తిరిగి;
    }

    ఒకవేళ( k == STD_సదిశ_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        తిరిగి;
    }
    ఒకవేళ (k == STD_సదిశ_CUDA_GPU_MAT)
    {
#నిర్వచితమైతే HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        తిరిగి;
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

శూన్య _OutputArray::clear() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == MAT )
    {
        CV_Assert(!fixed_కొలత());
        ((Mat*)obj)->resize(0);
        తిరిగి;
    }

    release();
}

తార్కిక _OutputArray::needed() స్థిరరాశి
{
    తిరిగి రకం() != NONE;
}

Mat& _OutputArray::getMatRef(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ( i < 0 )
    {
        CV_Assert( k == MAT );
        తిరిగి *(Mat*)obj;
    }

    CV_Assert( k == STD_సదిశ_MAT || k == STD_ARRAY_MAT );

    ఒకవేళ( k == STD_సదిశ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (పూర్ణాంకం)v.size() );
        తిరిగి v[i];
    }
    లేకపోతే
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ఎత్తు );
        తిరిగి v[i];
    }
}

UMat& _OutputArray::getUMatRef(పూర్ణాంకం i) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ( i < 0 )
    {
        CV_Assert( k == UMAT );
        తిరిగి *(UMat*)obj;
    }
    లేకపోతే
    {
        CV_Assert( k == STD_సదిశ_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (పూర్ణాంకం)v.size() );
        తిరిగి v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert( k == CUDA_GPU_MAT );
    తిరిగి *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert(k == STD_సదిశ_CUDA_GPU_MAT);
    తిరిగి *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert( k == CUDA_GPU_MATND );
    తిరిగి *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert(k == STD_సదిశ_CUDA_GPU_MAT_ND);
    తిరిగి *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::బఫరు_2& _OutputArray::get_OGl_బఫరు_Ref() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert( k == OPENGL_బఫరు );
    తిరిగి *(ogl::బఫరు_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    CV_Assert( k == CUDA_HOST_MEM );
    తిరిగి *(cuda::HostMem*)obj;
}

శూన్య _OutputArray::సమితి_గమ్యం(స్థిరరాశి ఆదానం_Array_2& arr, స్థిరరాశి ఆదానం_Array_2 & ముసుగు) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == NONE )
        ;
    లేకపోతే ఒకవేళ (k == MAT || k == MATX || k == STD_సదిశ)
    {
        Mat m = getMat();
        m.సమితి_గమ్యం(arr, ముసుగు);
    }
    లేకపోతే ఒకవేళ( k == UMAT )
        ((UMat*)obj)->సమితి_గమ్యం(arr, ముసుగు);
    లేకపోతే ఒకవేళ( k == CUDA_GPU_MAT )
    {
#నిర్వచితమైతే HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.రకం(), ఆదానం_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->సమితి_గమ్యం(Scalar(Vec<ద్విగుణ, 4>(value.ptr<ద్విగుణ>())), ముసుగు);
#లేకపోతే
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#అయితే_ముగింపు
    }
    లేకపోతే
        CV_Error(Error::StsNotImplemented, "");
}

శూన్య _OutputArray::సమితి_Zero() స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();

    ఒకవేళ( k == NONE )
        ;
    లేకపోతే ఒకవేళ (k == MAT || k == MATX || k == STD_సదిశ)
    {
        Mat m = getMat();
        m.సమితి_Zero();
    }
    లేకపోతే
    {
        సమితి_గమ్యం(Scalar::all(0), noArray());
    }
}

శూన్య _OutputArray::assign(స్థిరరాశి UMat& u) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    లేకపోతే ఒకవేళ (k == MAT)
    {
        u.copy_గమ్యం(*(Mat*)obj); // TODO check u.getMat()
    }
    లేకపోతే ఒకవేళ (k == MATX)
    {
        u.copy_గమ్యం(getMat()); // TODO check u.getMat()
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


శూన్య _OutputArray::assign(స్థిరరాశి Mat& m) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == UMAT)
    {
        m.copy_గమ్యం(*(UMat*)obj); // TODO check m.getUMat()
    }
    లేకపోతే ఒకవేళ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    లేకపోతే ఒకవేళ (k == MATX)
    {
        m.copy_గమ్యం(getMat());
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


శూన్య _OutputArray::move(UMat& u) స్థిరరాశి
{
    ఒకవేళ (fixed_కొలత())
    {
        // TODO Performance warning
        assign(u);
        తిరిగి;
    }
    పూర్ణాంకం k = రకం();
    ఒకవేళ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    లేకపోతే ఒకవేళ (k == MAT)
    {
        u.copy_గమ్యం(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    లేకపోతే ఒకవేళ (k == MATX)
    {
        u.copy_గమ్యం(getMat()); // TODO check u.getMat()
        u.release();
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


శూన్య _OutputArray::move(Mat& m) స్థిరరాశి
{
    ఒకవేళ (fixed_కొలత())
    {
        // TODO Performance warning
        assign(m);
        తిరిగి;
    }
    పూర్ణాంకం k = రకం();
    ఒకవేళ (k == UMAT)
    {
        m.copy_గమ్యం(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    లేకపోతే ఒకవేళ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    లేకపోతే ఒకవేళ (k == MATX)
    {
        m.copy_గమ్యం(getMat());
        m.release();
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


శూన్య _OutputArray::assign(స్థిరరాశి std::vector<UMat>& v) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == STD_సదిశ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        కొరకు (size_t i = 0; i < v.size(); i++)
        {
            స్థిరరాశి UMat& m = v[i];
            UMat& this_m = this_v[i];
            ఒకవేళ (this_m.u != ఖాళీ && this_m.u == m.u)
                కొనసాగు; // same object (see dnn::Layer::forward_fallback)
            m.copy_గమ్యం(this_m);
        }
    }
    లేకపోతే ఒకవేళ (k == STD_సదిశ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        కొరకు (size_t i = 0; i < v.size(); i++)
        {
            స్థిరరాశి UMat& m = v[i];
            Mat& this_m = this_v[i];
            ఒకవేళ (this_m.u != ఖాళీ && this_m.u == m.u)
                కొనసాగు; // same object (see dnn::Layer::forward_fallback)
            m.copy_గమ్యం(this_m);
        }
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


శూన్య _OutputArray::assign(స్థిరరాశి std::vector<Mat>& v) స్థిరరాశి
{
    ఆదానం_Array_2::రకం_Flag k = రకం();
    ఒకవేళ (k == STD_సదిశ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        కొరకు (size_t i = 0; i < v.size(); i++)
        {
            స్థిరరాశి Mat& m = v[i];
            UMat& this_m = this_v[i];
            ఒకవేళ (this_m.u != ఖాళీ && this_m.u == m.u)
                కొనసాగు; // same object (see dnn::Layer::forward_fallback)
            m.copy_గమ్యం(this_m);
        }
    }
    లేకపోతే ఒకవేళ (k == STD_సదిశ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        కొరకు (size_t i = 0; i < v.size(); i++)
        {
            స్థిరరాశి Mat& m = v[i];
            Mat& this_m = this_v[i];
            ఒకవేళ (this_m.u != ఖాళీ && this_m.u == m.u)
                కొనసాగు; // same object (see dnn::Layer::forward_fallback)
            m.copy_గమ్యం(this_m);
        }
    }
    లేకపోతే
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


స్థిర ఆదానం_Output_Array_2 _none;
ఆదానం_Output_Array noArray() { తిరిగి _none; }

} // cv::
