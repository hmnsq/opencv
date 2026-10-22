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

#अपरिभाषय STD_सदिशम्_SWITCH
#परिभाषय STD_सदिशम्_SWITCH(esz, func) \
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

Mat निवेशः_Array_2::getMat_(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    AccessFlag accessFlags = flags & ACCESS_आवरणम्;

    यदि( k == MAT )
    {
        ध्रुव Mat* m = (ध्रुव Mat*)obj;
        यदि( i < 0 )
            प्रत्यागम *m;
        प्रत्यागम m->row(i);
    }

    यदि( k == UMAT )
    {
        ध्रुव UMat* m = (ध्रुव UMat*)obj;
        यदि( i < 0 )
            प्रत्यागम m->getMat(accessFlags);
        प्रत्यागम m->getMat(accessFlags).row(i);
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        प्रत्यागम Mat(sz, flags, obj);
    }

    यदि( k == STD_BOOL_सदिशम् )
    {
        CV_Assert( i < 0 );
        पूर्णाङ्क t = CV_8U;
        ध्रुव std::vector<तार्किक>& v = *(ध्रुव std::vector<तार्किक>*)obj;
        पूर्णाङ्क j, n = (पूर्णाङ्क)v.size();
        यदि( n == 0 )
            प्रत्यागम Mat();
        Mat m(1, &n, t);
        uchar* dst = m.दत्त;
        प्रति( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        प्रत्यागम m;
    }

    यदि( k == NONE )
        प्रत्यागम Mat();

    यदि( k == STD_सदिशम् || k == STD_सदिशम्_सदिशम् )
    {
        पूर्णाङ्क t = CV_MAT_TYPE(flags);
        पूर्णाङ्क esz = CV_ELEM_आकार(t);

    #अपरिभाषय GET_MAT
    #परिभाषय GET_MAT(T) \
        { \
            std::vector<T>* v; \
            यदि (k == STD_सदिशम्_सदिशम्) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            पूर्णाङ्क v_sz = पूर्णाङ्क(v->size()); \
            प्रत्यागम v->empty() ? Mat() : Mat(1, &v_sz, t, v->दत्त()); \
        }

        STD_सदिशम्_SWITCH(esz, GET_MAT)
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& v = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        प्रत्यागम v[i];
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* v = (ध्रुव Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        प्रत्यागम v[i];
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& v = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        प्रत्यागम v[i].getMat(accessFlags);
    }

    यदि( k == OPENGL_बफर )
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

        ध्रुव cuda::HostMem* cuda_mem = (ध्रुव cuda::HostMem*)obj;

        प्रत्यागम cuda_mem->create_Mat_शीर्षकम्();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat निवेशः_Array_2::getUMat(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    AccessFlag accessFlags = flags & ACCESS_आवरणम्;

    यदि( k == UMAT )
    {
        ध्रुव UMat* m = (ध्रुव UMat*)obj;
        यदि( i < 0 )
            प्रत्यागम *m;
        प्रत्यागम m->row(i);
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& v = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        प्रत्यागम v[i];
    }

    यदि( k == MAT )
    {
        ध्रुव Mat* m = (ध्रुव Mat*)obj;
        यदि( i < 0 )
            प्रत्यागम m->getUMat(accessFlags);
        प्रत्यागम m->row(i).getUMat(accessFlags);
    }

    प्रत्यागम getMat(i).getUMat(accessFlags);
}

शून्य निवेशः_Array_2::get_Mat_सदिशम्(std::vector<Mat>& mv) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    AccessFlag accessFlags = flags & ACCESS_आवरणम्;

    यदि( k == MAT )
    {
        ध्रुव Mat& m = *(ध्रुव Mat*)obj;
        पूर्णाङ्क n = (पूर्णाङ्क)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        प्रति( पूर्णाङ्क i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.स्तम्भसङ्ख्या, m.type(), (शून्य*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (शून्य*)m.ptr(i), &m.चरणः[1]);
        प्रत्यागम;
    }

    यदि (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_आकार(flags);
        mv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.विस्तारः, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.विस्तारः*i);
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम् )
    {
        ध्रुव std::vector<uchar>& v = *(ध्रुव std::vector<uchar>*)obj;

        size_t n = योग(), esz = CV_ELEM_आकार(flags);
        पूर्णाङ्क t = CV_MAT_गभीरता(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (शून्य*)(v.दत्त() + esz*i));
        प्रत्यागम;
    }

    यदि( k == NONE )
    {
        mv.clear();
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_सदिशम् )
    {
        पूर्णाङ्क typ = CV_MAT_TYPE(flags);
        पूर्णाङ्क esz = CV_ELEM_आकार(typ);

        #अपरिभाषय GET_VEC_VEC_MAT_VEC
        #परिभाषय GET_VEC_VEC_MAT_VEC(T) \
        { \
            ध्रुव std::vector<std::vector<T > >* vv = (ध्रुव std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            प्रति (size_t i = 0; i < n; i++) { \
                ध्रुव std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)अधिकतम_पूर्णाङ्कः); \
                पूर्णाङ्क ni = (पूर्णाङ्क)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (शून्य*)vi.दत्त()) : Mat(); \
            } \
        } \
        भङ्ग

        STD_सदिशम्_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& v = *(ध्रुव std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        प्रत्यागम;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* v = (ध्रुव Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& v = *(ध्रुव std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        प्रत्यागम;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य निवेशः_Array_2::get_UMat_सदिशम्(std::vector<UMat>& umv) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    AccessFlag accessFlags = flags & ACCESS_आवरणम्;

    यदि( k == NONE )
    {
        umv.clear();
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& v = *(ध्रुव std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        प्रत्यागम;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* v = (ध्रुव Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& v = *(ध्रुव std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        प्रति( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        प्रत्यागम;
    }

    यदि( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        प्रत्यागम;
    }
    यदि( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        प्रत्यागम;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat निवेशः_Array_2::getGpuMat() ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि (k == CUDA_GPU_MAT)
    {
        ध्रुव cuda::GpuMat* d_mat = (ध्रुव cuda::GpuMat*)obj;
        प्रत्यागम *d_mat;
    }

    यदि (k == CUDA_HOST_MEM)
    {
        ध्रुव cuda::HostMem* cuda_mem = (ध्रुव cuda::HostMem*)obj;
        प्रत्यागम cuda_mem->create_Gpu_Mat_शीर्षकम्();
    }

    यदि (k == OPENGL_बफर)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    यदि (k == NONE)
        प्रत्यागम cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
}
शून्य निवेशः_Array_2::get_Gpu_Mat_सदिशम्(std::vector<cuda::GpuMat>& gpumv) ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#अन्यथा
    CV_अप्रयुक्तम्(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
}
शून्य निवेशः_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#अन्यथा
    CV_अप्रयुक्तम्(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
}
cuda::GpuMatND निवेशः_Array_2::getGpuMatND() ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि (k == CUDA_GPU_MATND)
    {
        ध्रुव cuda::GpuMatND* d_mat = (ध्रुव cuda::GpuMatND*)obj;
        प्रत्यागम *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
}
ogl::बफर_2 निवेशः_Array_2::get_OGl_बफर() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    CV_Assert(k == OPENGL_बफर);

    ध्रुव ogl::बफर_2* gl_buf = (ध्रुव ogl::बफर_2*)obj;
    प्रत्यागम *gl_buf;
}

पूर्णाङ्क निवेशः_Array_2::पङ्क्तिकाः_2(पूर्णाङ्क i) ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == CUDA_GPU_MATND)
    {
        ध्रुव cuda::GpuMatND& _gpuMatND = *(ध्रुव cuda::GpuMatND*)obj;
        प्रत्यागम (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#यदि_अन्तः

    प्रत्यागम आकार_3(i).height;
}

पूर्णाङ्क निवेशः_Array_2::स्तम्भसङ्ख्या_2(पूर्णाङ्क i) ध्रुव
{
#यदि_परिभाषितम् HAVE_CUDA
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == CUDA_GPU_MATND)
    {
        ध्रुव cuda::GpuMatND& _gpuMatND = *(ध्रुव cuda::GpuMatND*)obj;
        प्रत्यागम (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#यदि_अन्तः

    प्रत्यागम आकार_3(i).विस्तारः;
}

आकार निवेशः_Array_2::आकार_3(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव Mat* m = (ध्रुव Mat*)obj;
        CV_Assert(m->dims <= 2);
        प्रत्यागम आकार(m->स्तम्भसङ्ख्या, m->पङ्क्तिकाः);
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        ध्रुव UMat* m = (ध्रुव UMat*)obj;
        CV_Assert(m->dims <= 2);
        प्रत्यागम आकार(m->स्तम्भसङ्ख्या, m->पङ्क्तिकाः);
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        प्रत्यागम sz;
    }

    यदि( k == NONE )
        प्रत्यागम आकार();

    यदि( k == STD_सदिशम् || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
    {
        size_t n = योग(i);
        CV_Assert(n <= (size_t)अधिकतम_पूर्णाङ्कः);
        प्रत्यागम आकार((पूर्णाङ्क)n, 1);
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        यदि( i < 0 )
            प्रत्यागम vv.empty() ? आकार() : आकार((पूर्णाङ्क)vv.size(), 1);
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        प्रत्यागम vv[i].size();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        यदि( i < 0 )
            प्रत्यागम sz.height==0 ? आकार() : आकार(sz.height, 1);
        CV_Assert( i < sz.height );

        प्रत्यागम vv[i].size();
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ध्रुव std::vector<cuda::GpuMat>& vv = *(ध्रुव std::vector<cuda::GpuMat>*)obj;
        यदि (i < 0)
            प्रत्यागम vv.empty() ? आकार() : आकार((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        प्रत्यागम vv[i].size();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        यदि (i < 0)
            प्रत्यागम vv.empty() ? आकार() : आकार((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        ध्रुव cuda::GpuMatND& m = vv[i];
        यदि (m.dims == 0)
            प्रत्यागम आकार();
        यदि (m.dims == 1)
            प्रत्यागम आकार(1, m.size[0]);
        प्रत्यागम आकार(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        यदि( i < 0 )
            प्रत्यागम vv.empty() ? आकार() : आकार((पूर्णाङ्क)vv.size(), 1);
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        प्रत्यागम vv[i].size();
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        यदि (i < 0)
            प्रत्यागम vv.empty() ? आकार() : आकार((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        ध्रुव cuda::GpuMatND& m = vv[i];
        यदि (m.dims == 0)
            प्रत्यागम आकार();
        यदि (m.dims == 1)
            प्रत्यागम आकार(1, m.size[0]);
        प्रत्यागम आकार(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == OPENGL_बफर )
    {
        CV_Assert( i < 0 );
        ध्रुव ogl::बफर_2* buf = (ध्रुव ogl::बफर_2*)obj;
        प्रत्यागम buf->size();
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव cuda::GpuMat* d_mat = (ध्रुव cuda::GpuMat*)obj;
        प्रत्यागम d_mat->size();
    }

    यदि( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ध्रुव cuda::HostMem* cuda_mem = (ध्रुव cuda::HostMem*)obj;
        प्रत्यागम cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णाङ्क निवेशः_Array_2::sizend(पूर्णाङ्क* arrsz, पूर्णाङ्क i) ध्रुव
{
    पूर्णाङ्क j, d = 0;
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == NONE )
        ;
    अन्यथा यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव Mat& m = *(ध्रुव Mat*)obj;
        d = m.dims;
        यदि(arrsz)
            प्रति(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        ध्रुव UMat& m = *(ध्रुव UMat*)obj;
        d = m.dims;
        यदि(arrsz)
            प्रति(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_सदिशम्_MAT && i >= 0 )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        ध्रुव Mat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            प्रति(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_ARRAY_MAT && i >= 0 )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert( i < sz.height );
        ध्रुव Mat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            प्रति(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_सदिशम्_UMAT && i >= 0 )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        ध्रुव UMat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            प्रति(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि (((k == STD_सदिशम् || k == STD_BOOL_सदिशम् || k == STD_सदिशम्_सदिशम्) && i < 0) ||
             (k == STD_सदिशम्_सदिशम् && i >= 0))
    {
        size_t n = योग(i);
        d = 1;
        यदि(arrsz)
        {
            arrsz[0] = पूर्णाङ्क(n);
        }
    }
    अन्यथा
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        आकार sz2d = size(i);
        d = 2;
        यदि(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.विस्तारः;
        }
    }

    प्रत्यागम d;
}

तार्किक निवेशः_Array_2::empty(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (i >= 0) {
        यदि (k == STD_सदिशम्_MAT) {
            स्वचालित mv = reinterpret_cast<ध्रुव std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            प्रत्यागम mv->at(i).empty();
        }
        अन्यथा यदि (k == STD_सदिशम्_UMAT) {
            स्वचालित umv = reinterpret_cast<ध्रुव std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            प्रत्यागम umv->at(i).empty();
        }
        अन्यथा यदि (k == STD_सदिशम् || k == STD_BOOL_सदिशम् || k == STD_सदिशम्_सदिशम्) {
            size_t n = योग(i);
            प्रत्यागम n == 0;
        } अन्यथा {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    प्रत्यागम empty();
}

MatShape निवेशः_Array_2::shape(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    MatShape shape;

    यदि( k == NONE )
        ;
    अन्यथा यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव Mat& m = *(ध्रुव Mat*)obj;
        shape = m.size;
    }
    अन्यथा यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        ध्रुव UMat& m = *(ध्रुव UMat*)obj;
        shape = m.size;
    }
    अन्यथा यदि( k == STD_सदिशम्_MAT && i >= 0 )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ध्रुव Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा यदि( k == STD_ARRAY_MAT && i >= 0 )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert( i < sz.height );
        ध्रुव Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा यदि( k == STD_सदिशम्_UMAT && i >= 0 )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ध्रुव UMat& m = vv[i];
        shape = m.size;
    }
    अन्यथा
    {
        पूर्णाङ्क sizes[CV_MAX_DIM];
        पूर्णाङ्क dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    प्रत्यागम shape;
}

तार्किक निवेशः_Array_2::same_आकार(ध्रुव निवेशः_Array_2& arr) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k1 = विधा(), k2 = arr.विधा();
    आकार sz1;

    यदि( k1 == MAT )
    {
        ध्रुव Mat* m = ((ध्रुव Mat*)obj);
        यदि( k2 == MAT )
            प्रत्यागम m->size == ((ध्रुव Mat*)arr.obj)->size;
        यदि( k2 == UMAT )
            प्रत्यागम m->size == ((ध्रुव UMat*)arr.obj)->size;
        यदि( m->dims > 2 )
            प्रत्यागम असत्य;
        sz1 = m->size();
    }
    अन्यथा यदि( k1 == UMAT )
    {
        ध्रुव UMat* m = ((ध्रुव UMat*)obj);
        यदि( k2 == MAT )
            प्रत्यागम m->size == ((ध्रुव Mat*)arr.obj)->size;
        यदि( k2 == UMAT )
            प्रत्यागम m->size == ((ध्रुव UMat*)arr.obj)->size;
        यदि( m->dims > 2 )
            प्रत्यागम असत्य;
        sz1 = m->size();
    }
    अन्यथा यदि ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        प्रत्यागम ((ध्रुव cuda::GpuMatND*)obj)->size == ((ध्रुव cuda::GpuMatND*)arr.obj)->size;
    }
    अन्यथा
        sz1 = size();

    यदि( arr.dims() > 2 )
        प्रत्यागम असत्य;
    प्रत्यागम sz1 == arr.size();
}

पूर्णाङ्क निवेशः_Array_2::dims_(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव Mat*)obj)->dims;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव UMat*)obj)->dims;
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        प्रत्यागम 2;
    }

    यदि( k == STD_सदिशम् || k == STD_BOOL_सदिशम् )
    {
        CV_Assert( i < 0 );
        प्रत्यागम 1;
    }

    यदि( k == NONE )
        प्रत्यागम 0;

    यदि( k == STD_सदिशम्_सदिशम् )
    {
        यदि( i < 0 )
            प्रत्यागम 1;
        CV_Assert( size_t(i) < योग() );
        प्रत्यागम 1;
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        यदि( i < 0 )
            प्रत्यागम 1;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        प्रत्यागम vv[i].dims;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        यदि( i < 0 )
            प्रत्यागम 1;
        CV_Assert( i < sz.height );

        प्रत्यागम vv[i].dims;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        यदि( i < 0 )
            प्रत्यागम 1;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        प्रत्यागम vv[i].dims;
    }

    यदि( k == OPENGL_बफर )
    {
        CV_Assert( i < 0 );
        प्रत्यागम 2;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम 2;
    }

    यदि( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव cuda::GpuMatND*)obj)->dims;
    }

    यदि( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        प्रत्यागम 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t निवेशः_Array_2::योग(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव Mat*)obj)->योग();
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव UMat*)obj)->योग();
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        यदि( i < 0 )
            प्रत्यागम vv.size();

        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        प्रत्यागम vv[i].योग();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        यदि( i < 0 )
            प्रत्यागम sz.height;

        CV_Assert( i < sz.height );
        प्रत्यागम vv[i].योग();
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        यदि( i < 0 )
            प्रत्यागम vv.size();

        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        प्रत्यागम vv[i].योग();
    }

    यदि( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ध्रुव cuda::GpuMatND& _gpuMatND = *((ध्रुव cuda::GpuMatND*)obj);
        यदि (_gpuMatND.dims > 0)
        {
            res = 1;
            प्रति(पूर्णाङ्क d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            प्रत्यागम res;
        }
    }

    यदि (k == STD_सदिशम् || k == STD_सदिशम्_सदिशम्)
    {
        CV_Assert(i < 0 || k == STD_सदिशम्_सदिशम्);
        पूर्णाङ्क esz = CV_ELEM_आकार(flags);

        #अपरिभाषय GET_VEC_आकार
        #परिभाषय GET_VEC_आकार(T) \
            यदि (k == STD_सदिशम्_सदिशम्) { \
                ध्रुव std::vector<std::vector<T > >* vv = (ध्रुव std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                यदि (i < 0) \
                    प्रत्यागम n; \
                CV_Assert((size_t)i < n); \
                प्रत्यागम vv->at(i).size(); \
            } अन्यथा \
                प्रत्यागम ((ध्रुव std::vector<T >*)obj)->size()

        STD_सदिशम्_SWITCH(esz, GET_VEC_आकार)
    }

    प्रत्यागम size(i).area();
}

पूर्णाङ्क निवेशः_Array_2::type_(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
        प्रत्यागम ((ध्रुव Mat*)obj)->type();

    यदि( k == UMAT )
        प्रत्यागम ((ध्रुव UMat*)obj)->type();

    यदि( k == MATX || k == STD_सदिशम् || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
        प्रत्यागम CV_MAT_TYPE(flags);

    यदि( k == NONE )
        प्रत्यागम -1;

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        यदि( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            प्रत्यागम CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        प्रत्यागम vv[i >= 0 ? i : 0].type();
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        यदि( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            प्रत्यागम CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        प्रत्यागम vv[i >= 0 ? i : 0].type();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        यदि( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            प्रत्यागम CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        प्रत्यागम vv[i >= 0 ? i : 0].type();
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ध्रुव std::vector<cuda::GpuMat>& vv = *(ध्रुव std::vector<cuda::GpuMat>*)obj;
        यदि (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            प्रत्यागम CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        प्रत्यागम vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        यदि (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            प्रत्यागम CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        प्रत्यागम vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == OPENGL_बफर )
        प्रत्यागम ((ध्रुव ogl::बफर_2*)obj)->type();

    यदि( k == CUDA_GPU_MAT )
        प्रत्यागम ((ध्रुव cuda::GpuMat*)obj)->type();

    यदि( k == CUDA_GPU_MATND )
        प्रत्यागम ((ध्रुव cuda::GpuMatND*)obj)->type();

    यदि( k == CUDA_HOST_MEM )
        प्रत्यागम ((ध्रुव cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णाङ्क निवेशः_Array_2::गभीरता_4(पूर्णाङ्क i) ध्रुव
{
    प्रत्यागम CV_MAT_गभीरता(type(i));
}

तार्किक निवेशः_Array_2::रिक्तानि() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
        प्रत्यागम ((ध्रुव Mat*)obj)->empty();

    यदि( k == UMAT )
        प्रत्यागम ((ध्रुव UMat*)obj)->empty();

    यदि (k == MATX)
        प्रत्यागम असत्य;

    यदि( k == STD_सदिशम् || k == STD_BOOL_सदिशम् || k == STD_सदिशम्_सदिशम् )
    {
        size_t n = योग(-1);
        प्रत्यागम n == 0;
    }

    यदि( k == NONE )
        प्रत्यागम सत्य;

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        प्रत्यागम vv.empty();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        प्रत्यागम sz.height == 0;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        प्रत्यागम vv.empty();
    }

    यदि( k == OPENGL_बफर )
        प्रत्यागम ((ध्रुव ogl::बफर_2*)obj)->empty();

    यदि( k == CUDA_GPU_MAT )
        प्रत्यागम ((ध्रुव cuda::GpuMat*)obj)->empty();

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
        ध्रुव std::vector<cuda::GpuMat>& vv = *(ध्रुव std::vector<cuda::GpuMat>*)obj;
        प्रत्यागम vv.empty();
    }

    यदि( k == CUDA_GPU_MATND )
        प्रत्यागम ((ध्रुव cuda::GpuMatND*)obj)->empty();

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        प्रत्यागम vv.empty();
    }

    यदि( k == CUDA_HOST_MEM )
        प्रत्यागम ((ध्रुव cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक निवेशः_Array_2::isContinuous(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
        प्रत्यागम i < 0 ? ((ध्रुव Mat*)obj)->isContinuous() : सत्य;

    यदि( k == UMAT )
        प्रत्यागम i < 0 ? ((ध्रुव UMat*)obj)->isContinuous() : सत्य;

    यदि( k == MATX || k == STD_सदिशम् ||
        k == NONE || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
        प्रत्यागम सत्य;

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].isContinuous();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        प्रत्यागम vv[i].isContinuous();
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].isContinuous();
    }

    यदि( k == CUDA_GPU_MAT )
      प्रत्यागम i < 0 ? ((ध्रुव cuda::GpuMat*)obj)->isContinuous() : सत्य;

    यदि( k == CUDA_GPU_MATND )
        प्रत्यागम i < 0 ? ((ध्रुव cuda::GpuMatND*)obj)->isContinuous() : सत्य;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक निवेशः_Array_2::isSubmatrix(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
        प्रत्यागम i < 0 ? ((ध्रुव Mat*)obj)->isSubmatrix() : असत्य;

    यदि( k == UMAT )
        प्रत्यागम i < 0 ? ((ध्रुव UMat*)obj)->isSubmatrix() : असत्य;

    यदि( k == MATX || k == STD_सदिशम् ||
        k == NONE || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
        प्रत्यागम असत्य;

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].isSubmatrix();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        प्रत्यागम vv[i].isSubmatrix();
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].isSubmatrix();
    }

    यदि( k == CUDA_GPU_MATND )
    {
        प्रत्यागम ((ध्रुव cuda::GpuMatND*)obj)->isSubmatrix();
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t निवेशः_Array_2::offset(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव Mat * ध्रुव m = ((ध्रुव Mat*)obj);
        प्रत्यागम (size_t)(m->ptr() - m->datastart);
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव UMat*)obj)->offset;
    }

    यदि( k == MATX || k == STD_सदिशम् ||
        k == NONE || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
        प्रत्यागम 0;

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णाङ्क)vv.size() );

        प्रत्यागम (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        प्रत्यागम (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].offset;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ध्रुव cuda::GpuMat * ध्रुव m = ((ध्रुव cuda::GpuMat*)obj);
        प्रत्यागम (size_t)(m->दत्त - m->datastart);
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
        ध्रुव std::vector<cuda::GpuMat>& vv = *(ध्रुव std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम (size_t)(vv[i].दत्त - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t निवेशः_Array_2::चरणः(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव Mat*)obj)->चरणः;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव UMat*)obj)->चरणः;
    }

    यदि( k == MATX || k == STD_सदिशम् ||
        k == NONE || k == STD_सदिशम्_सदिशम् || k == STD_BOOL_सदिशम् )
        प्रत्यागम 0;

    यदि( k == STD_सदिशम्_MAT )
    {
        ध्रुव std::vector<Mat>& vv = *(ध्रुव std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णाङ्क)vv.size() );
        प्रत्यागम vv[i].चरणः;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        ध्रुव Mat* vv = (ध्रुव Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        प्रत्यागम vv[i].चरणः;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ध्रुव std::vector<UMat>& vv = *(ध्रुव std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].चरणः;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        प्रत्यागम ((ध्रुव cuda::GpuMat*)obj)->चरणः;
    }
    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
        ध्रुव std::vector<cuda::GpuMat>& vv = *(ध्रुव std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        प्रत्यागम vv[i].चरणः;
    }
    यदि( k == CUDA_GPU_MATND )
    {
        ध्रुव cuda::GpuMatND& _gpuMatND = *(ध्रुव cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        प्रत्यागम _gpuMatND.चरणः[i];
    }

    यदि (k == STD_सदिशम्_CUDA_GPU_MAT_ND)
    {
        ध्रुव std::vector<cuda::GpuMatND>& vv = *(ध्रुव std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ध्रुव cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        प्रत्यागम _gpuMatND.चरणः[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

शून्य निवेशः_Array_2::copy_गन्तव्यम्(ध्रुव _OutputArray& arr) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == NONE )
        arr.release();
    अन्यथा यदि( k == MAT || k == MATX || k == STD_सदिशम् || k == STD_BOOL_सदिशम् )
    {
        Mat m = getMat();
        m.copy_गन्तव्यम्(arr);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->copy_गन्तव्यम्(arr);
#यदि_परिभाषितम् HAVE_CUDA
    अन्यथा यदि (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_गन्तव्यम्(arr);
#यदि_अन्तः
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य निवेशः_Array_2::copy_गन्तव्यम्(ध्रुव _OutputArray& arr, ध्रुव निवेशः_Array_2 & आवरणम्) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == NONE )
        arr.release();
    अन्यथा यदि( k == MAT || k == MATX || k == STD_सदिशम् || k == STD_BOOL_सदिशम् )
    {
        Mat m = getMat();
        m.copy_गन्तव्यम्(arr, आवरणम्);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->copy_गन्तव्यम्(arr, आवरणम्);
#यदि_परिभाषितम् HAVE_CUDA
    अन्यथा यदि (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_गन्तव्यम्(arr, आवरणम्);
#यदि_अन्तः
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

तार्किक _OutputArray::fixed_आकार() ध्रुव
{
    प्रत्यागम (flags & FIXED_आकार) == FIXED_आकार;
}

तार्किक _OutputArray::fixedType() ध्रुव
{
    प्रत्यागम (flags & FIXED_TYPE) == FIXED_TYPE;
}

शून्य _OutputArray::create_(आकार _sz, पूर्णाङ्क mtype, पूर्णाङ्क i, तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि( k == MAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((Mat*)obj)->size.संक्रियकः()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        प्रत्यागम;
    }
    यदि( k == UMAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((UMat*)obj)->size.संक्रियकः()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        प्रत्यागम;
    }
    यदि( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    यदि( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.विस्तारः)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        cuda::GpuMatND::आकार_Array sizes({_sz.height, _sz.विस्तारः});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    यदि( k == OPENGL_बफर && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((ogl::बफर_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::बफर_2*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_OPENGL
        ((ogl::बफर_2*)obj)->create(_sz, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#यदि_अन्तः
    }
    यदि( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    पूर्णाङ्क sizes[] = {_sz.height, _sz.विस्तारः};
    create(2, sizes, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
}

शून्य _OutputArray::create_(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क mtype, पूर्णाङ्क i, तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि( k == MAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((Mat*)obj)->size.संक्रियकः()() == आकार(स्तम्भसङ्ख्या_3, पङ्क्तिकाः_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, mtype);
        प्रत्यागम;
    }
    यदि( k == UMAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((UMat*)obj)->size.संक्रियकः()() == आकार(स्तम्भसङ्ख्या_3, पङ्क्तिकाः_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, mtype);
        प्रत्यागम;
    }
    यदि( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::GpuMat*)obj)->size() == आकार(स्तम्भसङ्ख्या_3, पङ्क्तिकाः_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    यदि( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == पङ्क्तिकाः_3) && (((cuda::GpuMatND*)obj)->size[1] == स्तम्भसङ्ख्या_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        cuda::GpuMatND::आकार_Array sizes({पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    यदि( k == OPENGL_बफर && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((ogl::बफर_2*)obj)->size() == आकार(स्तम्भसङ्ख्या_3, पङ्क्तिकाः_3));
        CV_Assert(!fixedType() || ((ogl::बफर_2*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_OPENGL
        ((ogl::बफर_2*)obj)->create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#यदि_अन्तः
    }
    यदि( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::HostMem*)obj)->size() == आकार(स्तम्भसङ्ख्या_3, पङ्क्तिकाः_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::HostMem*)obj)->create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    पूर्णाङ्क sizes[] = {पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
}

शून्य _OutputArray::create(पूर्णाङ्क d, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क mtype, पूर्णाङ्क i,
                          तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    पूर्णाङ्क size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    निवेशः_Array_2::विधा_Flag k = विधा();
    mtype = CV_MAT_TYPE(mtype);

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.पङ्क्तिकाः == size0 && m.स्तम्भसङ्ख्या == size1) ||
            (allowTransposed && m.पङ्क्तिकाः == size1 && m.स्तम्भसङ्ख्या == size0 && m.isContinuous())))
        {
            प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_गभीरता(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixed_आकार())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.पङ्क्तिकाः == size0 && m.स्तम्भसङ्ख्या == size1) ||
            (allowTransposed && m.पङ्क्तिकाः == size1 && m.स्तम्भसङ्ख्या == size0 && m.isContinuous())))
        {
            प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixed_आकार())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि( k == MATX )
    {
        CV_Assert( i < 0 );
        पूर्णाङ्क type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_गभीरता_आवरणम्) != 0) );
        CV_CheckLE(d, 2, "");
        आकार requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        यदि (sz.विस्तारः == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            पूर्णाङ्क योग_1d = std::max(sz.विस्तारः, sz.height);
            CV_Check(requested_size, std::max(requested_size.विस्तारः, requested_size.height) == योग_1d, "");
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
                        (requested_size == sz || (requested_size.height == sz.विस्तारः && requested_size.विस्तारः == sz.height)),
                        "");
            }
        }
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम् || k == STD_सदिशम्_सदिशम् )
    {
        CV_Assert(!fixed_आकार());
        CV_Assert(k == STD_सदिशम्_सदिशम् || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        पूर्णाङ्क esz = CV_ELEM_आकार(flags);

        यदि( k == STD_सदिशम् || (k == STD_सदिशम्_सदिशम् && i >= 0) )
        {
            पूर्णाङ्क type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_गभीरता_आवरणम्) != 0) );
        }

    #अपरिभाषय RESIZE_VEC
    #परिभाषय RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            यदि (k == STD_सदिशम्_सदिशम्) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                यदि (i < 0) { \
                    CV_Assert(!fixed_आकार() || len == vv->size()); \
                    vv->resize(len); \
                    प्रत्यागम; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_आकार() || len == v->size()); \
            v->resize(len); \
        } \
        भङ्ग

        STD_सदिशम्_SWITCH(esz, RESIZE_VEC)
        प्रत्यागम;
    }

    यदि( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_आकार() || len == len0);
            v.resize(len);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                प्रति( size_t j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        अनुवर्त;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_आवरणम्) | _type;
                }
            }
            प्रत्यागम;
        }

        CV_Assert( i < (पूर्णाङ्क)v.size() );
        Mat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.दत्त &&
                m.type() == mtype && m.पङ्क्तिकाः == sizes[1] && m.स्तम्भसङ्ख्या == sizes[0] )
                प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        यदि(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                प्रति( size_t j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        अनुवर्त;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_आवरणम्) | _type;
                }
            }
            प्रत्यागम;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.दत्त &&
                m.type() == mtype && m.पङ्क्तिकाः == sizes[1] && m.स्तम्भसङ्ख्या == sizes[0] )
                प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        यदि(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_आकार() || len == len0);
            v.resize(len);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                प्रति( size_t j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        अनुवर्त;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_आवरणम्) | _type;
                }
            }
            प्रत्यागम;
        }

        CV_Assert( i < (पूर्णाङ्क)v.size() );
        UMat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.पङ्क्तिकाः == sizes[1] && m.स्तम्भसङ्ख्या == sizes[0] )
                प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        यदि(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
        प्रत्यागम;
    }

    यदि( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_गभीरता_आवरणम् == 0 )
    {
#यदि_परिभाषितम् HAVE_CUDA
        cuda::GpuMatND::आकार_Array आकार_Array_2 = cuda::GpuMatND::आकार_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(आकार_Array_2, mtype);
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::create(ध्रुव MatShape& shape, पूर्णाङ्क mtype, पूर्णाङ्क i,
                          तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    यदि (shape.dims < 0) {
        release();
    } अन्यथा {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
    }
}

Mat _OutputArray::reinterpret(पूर्णाङ्क mtype) ध्रुव
{
    mtype = CV_MAT_TYPE(mtype);
    प्रत्यागम getMat().reinterpret(mtype);
}

शून्य _OutputArray::create_Same_आकार(ध्रुव निवेशः_Array_2& arr, पूर्णाङ्क mtype) ध्रुव
{
    create(arr.shape(), mtype);
}

शून्य _OutputArray::fit(आकार _sz, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    पूर्णाङ्क sizes[] = {_sz.height, _sz.विस्तारः};
    fit(2, sizes, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
}

शून्य _OutputArray::fit(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    पूर्णाङ्क sizes[] = {पङ्क्तिकाः, स्तम्भसङ्ख्या};
    fit(2, sizes, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
}

शून्य _OutputArray::fit(ध्रुव MatShape& shape, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    यदि (shape.dims < 0) {
        release();
        प्रत्यागम;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि ((k == MAT && i < 0) || (k == STD_सदिशम्_MAT && i >= 0)) {
        Mat_आकार& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } अन्यथा यदि ((k == UMAT && i < 0) || (k == STD_सदिशम्_UMAT && i >= 0)) {
        Mat_आकार& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

शून्य _OutputArray::fit_Same_आकार(ध्रुव निवेशः_Array_2& arr, पूर्णाङ्क mtype) ध्रुव
{
    fit(arr.shape(), mtype);
}

शून्य _OutputArray::fit(पूर्णाङ्क d, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::गभीरता_आवरणम् fixed_गभीरता_आवरणम्) ध्रुव
{
    पूर्णाङ्क size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    निवेशः_Array_2::विधा_Flag k = विधा();
    mtype = CV_MAT_TYPE(mtype);

    यदि( (k == MAT && i < 0) || (k == STD_सदिशम्_MAT && i >= 0) )
    {
        Mat* m;
        यदि (k == MAT)
            m = (Mat*)obj;
        अन्यथा {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->पङ्क्तिकाः == size0 && m->स्तम्भसङ्ख्या == size1) ||
             (allowTransposed && m->पङ्क्तिकाः == size1 && m->स्तम्भसङ्ख्या == size0 && m->isContinuous())))
        {
            प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_गभीरता(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixed_आकार())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        प्रत्यागम;
    }

    यदि( (k == UMAT && i < 0) || (k == STD_सदिशम्_UMAT && i >= 0) )
    {
        UMat* m;
        यदि (k == UMAT)
            m = (UMat*)obj;
        अन्यथा {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->पङ्क्तिकाः == size0 && m->स्तम्भसङ्ख्या == size1) ||
             (allowTransposed && m->पङ्क्तिकाः == size1 && m->स्तम्भसङ्ख्या == size0 && m->isContinuous())))
        {
            प्रत्यागम;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_गभीरता(flags)) & fixed_गभीरता_आवरणम्) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixed_आकार())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            प्रति(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        प्रत्यागम;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_गभीरता_आवरणम्);
}

शून्य _OutputArray::release() ध्रुव
{
    CV_Assert(!fixed_आकार());

    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        ((Mat*)obj)->release();
        प्रत्यागम;
    }

    यदि( k == UMAT )
    {
        ((UMat*)obj)->release();
        प्रत्यागम;
    }

    यदि( k == CUDA_GPU_MAT )
    {
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == CUDA_GPU_MATND )
    {
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == CUDA_HOST_MEM )
    {
#यदि_परिभाषितम् HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }

    यदि( k == OPENGL_बफर )
    {
#यदि_परिभाषितम् HAVE_OPENGL
        ((ogl::बफर_2*)obj)->release();
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#यदि_अन्तः
    }

    यदि( k == NONE )
        प्रत्यागम;

    यदि( k == STD_सदिशम् || k == STD_सदिशम्_सदिशम् )
    {
    #अपरिभाषय CLEAR_VEC
    #परिभाषय CLEAR_VEC(T) \
        यदि (k == STD_सदिशम्_सदिशम्) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } अन्यथा { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        भङ्ग

        पूर्णाङ्क esz = CV_ELEM_आकार(flags);
        STD_सदिशम्_SWITCH(esz, CLEAR_VEC);
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        प्रत्यागम;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        प्रति( पूर्णाङ्क i = 0; i < sz.height; i++ )
            v[i].release();
        प्रत्यागम;
    }

    यदि( k == STD_BOOL_सदिशम् )
    {
        ((std::vector<तार्किक>*)obj)->clear();
        प्रत्यागम;
    }

    यदि( k == STD_सदिशम्_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        प्रत्यागम;
    }
    यदि (k == STD_सदिशम्_CUDA_GPU_MAT)
    {
#यदि_परिभाषितम् HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        प्रत्यागम;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::clear() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == MAT )
    {
        CV_Assert(!fixed_आकार());
        ((Mat*)obj)->resize(0);
        प्रत्यागम;
    }

    release();
}

तार्किक _OutputArray::needed() ध्रुव
{
    प्रत्यागम विधा() != NONE;
}

Mat& _OutputArray::getMatRef(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि( i < 0 )
    {
        CV_Assert( k == MAT );
        प्रत्यागम *(Mat*)obj;
    }

    CV_Assert( k == STD_सदिशम्_MAT || k == STD_ARRAY_MAT );

    यदि( k == STD_सदिशम्_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)v.size() );
        प्रत्यागम v[i];
    }
    अन्यथा
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        प्रत्यागम v[i];
    }
}

UMat& _OutputArray::getUMatRef(पूर्णाङ्क i) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि( i < 0 )
    {
        CV_Assert( k == UMAT );
        प्रत्यागम *(UMat*)obj;
    }
    अन्यथा
    {
        CV_Assert( k == STD_सदिशम्_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)v.size() );
        प्रत्यागम v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert( k == CUDA_GPU_MAT );
    प्रत्यागम *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert(k == STD_सदिशम्_CUDA_GPU_MAT);
    प्रत्यागम *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert( k == CUDA_GPU_MATND );
    प्रत्यागम *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert(k == STD_सदिशम्_CUDA_GPU_MAT_ND);
    प्रत्यागम *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::बफर_2& _OutputArray::get_OGl_बफर_Ref() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert( k == OPENGL_बफर );
    प्रत्यागम *(ogl::बफर_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    CV_Assert( k == CUDA_HOST_MEM );
    प्रत्यागम *(cuda::HostMem*)obj;
}

शून्य _OutputArray::समुच्चयः_गन्तव्यम्(ध्रुव निवेशः_Array_2& arr, ध्रुव निवेशः_Array_2 & आवरणम्) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == NONE )
        ;
    अन्यथा यदि (k == MAT || k == MATX || k == STD_सदिशम्)
    {
        Mat m = getMat();
        m.समुच्चयः_गन्तव्यम्(arr, आवरणम्);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->समुच्चयः_गन्तव्यम्(arr, आवरणम्);
    अन्यथा यदि( k == CUDA_GPU_MAT )
    {
#यदि_परिभाषितम् HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.विधा(), निवेशः_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->समुच्चयः_गन्तव्यम्(Scalar(Vec<द्विगुण, 4>(value.ptr<द्विगुण>())), आवरणम्);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#यदि_अन्तः
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य _OutputArray::समुच्चयः_Zero() ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();

    यदि( k == NONE )
        ;
    अन्यथा यदि (k == MAT || k == MATX || k == STD_सदिशम्)
    {
        Mat m = getMat();
        m.समुच्चयः_Zero();
    }
    अन्यथा
    {
        समुच्चयः_गन्तव्यम्(Scalar::all(0), noArray());
    }
}

शून्य _OutputArray::assign(ध्रुव UMat& u) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    अन्यथा यदि (k == MAT)
    {
        u.copy_गन्तव्यम्(*(Mat*)obj); // TODO check u.getMat()
    }
    अन्यथा यदि (k == MATX)
    {
        u.copy_गन्तव्यम्(getMat()); // TODO check u.getMat()
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(ध्रुव Mat& m) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == UMAT)
    {
        m.copy_गन्तव्यम्(*(UMat*)obj); // TODO check m.getUMat()
    }
    अन्यथा यदि (k == MAT)
    {
        *(Mat*)obj = m;
    }
    अन्यथा यदि (k == MATX)
    {
        m.copy_गन्तव्यम्(getMat());
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(UMat& u) ध्रुव
{
    यदि (fixed_आकार())
    {
        // TODO Performance warning
        assign(u);
        प्रत्यागम;
    }
    पूर्णाङ्क k = विधा();
    यदि (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    अन्यथा यदि (k == MAT)
    {
        u.copy_गन्तव्यम्(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    अन्यथा यदि (k == MATX)
    {
        u.copy_गन्तव्यम्(getMat()); // TODO check u.getMat()
        u.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(Mat& m) ध्रुव
{
    यदि (fixed_आकार())
    {
        // TODO Performance warning
        assign(m);
        प्रत्यागम;
    }
    पूर्णाङ्क k = विधा();
    यदि (k == UMAT)
    {
        m.copy_गन्तव्यम्(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    अन्यथा यदि (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    अन्यथा यदि (k == MATX)
    {
        m.copy_गन्तव्यम्(getMat());
        m.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(ध्रुव std::vector<UMat>& v) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == STD_सदिशम्_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        प्रति (size_t i = 0; i < v.size(); i++)
        {
            ध्रुव UMat& m = v[i];
            UMat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                अनुवर्त; // same object (see dnn::Layer::forward_fallback)
            m.copy_गन्तव्यम्(this_m);
        }
    }
    अन्यथा यदि (k == STD_सदिशम्_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        प्रति (size_t i = 0; i < v.size(); i++)
        {
            ध्रुव UMat& m = v[i];
            Mat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                अनुवर्त; // same object (see dnn::Layer::forward_fallback)
            m.copy_गन्तव्यम्(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(ध्रुव std::vector<Mat>& v) ध्रुव
{
    निवेशः_Array_2::विधा_Flag k = विधा();
    यदि (k == STD_सदिशम्_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        प्रति (size_t i = 0; i < v.size(); i++)
        {
            ध्रुव Mat& m = v[i];
            UMat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                अनुवर्त; // same object (see dnn::Layer::forward_fallback)
            m.copy_गन्तव्यम्(this_m);
        }
    }
    अन्यथा यदि (k == STD_सदिशम्_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        प्रति (size_t i = 0; i < v.size(); i++)
        {
            ध्रुव Mat& m = v[i];
            Mat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                अनुवर्त; // same object (see dnn::Layer::forward_fallback)
            m.copy_गन्तव्यम्(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


स्थिर निवेशः_Output_Array_2 _none;
निवेशः_Output_Array noArray() { प्रत्यागम _none; }

} // cv::
