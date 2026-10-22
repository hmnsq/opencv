// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ವ್ಯಾಖ್ಯಾನಿಸು IPP_DISABLE_REDUCE 1
#ವ್ಯಾಖ್ಯಾನಿಸು IPP_DISABLE_ವಿಂಗಡಿಸು   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

ಶೂನ್ಯ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.ಅಡ್ಡಸಾಲುಗಳು, b.ಅಡ್ಡಸಾಲುಗಳು);
    std::swap(a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, b.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    std::swap(a.ದತ್ತಾಂಶ, b.ದತ್ತಾಂಶ);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.ಹೆಜ್ಜೆ, b.ಹೆಜ್ಜೆ);
}


ಶೂನ್ಯ cv::hconcat(ಸ್ಥಿರಾಂಕ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( nsrc == 0 || !src )
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    ಗಾಗಿ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].ಅಡ್ಡಸಾಲುಗಳು == src[0].ಅಡ್ಡಸಾಲುಗಳು &&
                   src[i].type() == src[0].type());
        ಒಟ್ಟು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ += src[i].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
    _dst.create( src[0].ಅಡ್ಡಸಾಲುಗಳು, ಒಟ್ಟು_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src[0].type());
    Mat dst = _dst.getMat();
    ಗಾಗಿ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, 0, src[i].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src[i].ಅಡ್ಡಸಾಲುಗಳು));
        src[i].copy_ಗಮ್ಯ(dpart);
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ += src[i].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
}

ಶೂನ್ಯ cv::hconcat(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

ಶೂನ್ಯ cv::hconcat(ಒಳಹರಿವು_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ಸದಿಶ(src);
    hconcat(!src.empty() ? &src[0] : ಶೂನ್ಯಸೂಚಕ, src.size(), dst);
}

ಶೂನ್ಯ cv::vconcat(ಸ್ಥಿರಾಂಕ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    ಆದರೆ( nsrc == 0 || !src )
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಅಡ್ಡಸಾಲುಗಳು = 0, ಅಡ್ಡಸಾಲುಗಳು = 0;
    ಗಾಗಿ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == src[0].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ &&
                  src[i].type() == src[0].type());
        ಒಟ್ಟು_ಅಡ್ಡಸಾಲುಗಳು += src[i].ಅಡ್ಡಸಾಲುಗಳು;
    }
    _dst.create( ಒಟ್ಟು_ಅಡ್ಡಸಾಲುಗಳು, src[0].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src[0].type());
    Mat dst = _dst.getMat();
    ಗಾಗಿ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, ಅಡ್ಡಸಾಲುಗಳು, src[i].ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src[i].ಅಡ್ಡಸಾಲುಗಳು));
        src[i].copy_ಗಮ್ಯ(dpart);
        ಅಡ್ಡಸಾಲುಗಳು += src[i].ಅಡ್ಡಸಾಲುಗಳು;
    }
}

ಶೂನ್ಯ cv::vconcat(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

ಶೂನ್ಯ cv::vconcat(ಒಳಹರಿವು_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ಸದಿಶ(src);
    vconcat(!src.empty() ? &src[0] : ಶೂನ್ಯಸೂಚಕ, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಗಣ_ತಟಸ್ಥ( ಒಳಹರಿವು_Output_Array _m, ಸ್ಥಿರಾಂಕ Scalar& s )
{
    ಪೂರ್ಣಾಂಕ type = _m.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type), kercn = cn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = 1;
    ಪೂರ್ಣಾಂಕ sctype = CV_MAKE_TYPE(ಆಳ, cn == 3 ? 4 : cn);
    ಆದರೆ (ocl::Device::getDefault().isIntel())
    {
        ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = 4;
        ಆದರೆ (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_m), 4);
            ಆದರೆ (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::ಗಣ_ತಟಸ್ಥ_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                         ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ), cn,
                         ocl::memop_Type_ಗಮ್ಯ_Str(sctype),
                         kercn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat m = _m.getUMat();
    k.ವಾದಗಳು(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)m.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಶೂನ್ಯಸೂಚಕ, ಸುಳ್ಳು);
}

}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::ಗಣ_ತಟಸ್ಥ( ಒಳಹರಿವು_Output_Array _m, ಸ್ಥಿರಾಂಕ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_ಸತತ(_m.isUMat(),
               ocl_ಗಣ_ತಟಸ್ಥ(_m, s))

    Mat m = _m.getMat();
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type = m.type();

    ಆದರೆ( type == CV_32FC1 )
    {
        ದಶಮಾಂಶ* ದತ್ತಾಂಶ = m.ptr<ದಶಮಾಂಶ>();
        ದಶಮಾಂಶ val = (ದಶಮಾಂಶ)s[0];
        size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ[0]);

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++, ದತ್ತಾಂಶ += ಹೆಜ್ಜೆ )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ದತ್ತಾಂಶ[j] = 0;
            ಆದರೆ( i < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ )
                ದತ್ತಾಂಶ[i] = val;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64FC1 )
    {
        ದ್ವಿಗುಣ* ದತ್ತಾಂಶ = m.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ val = s[0];
        size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ[0]);

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++, ದತ್ತಾಂಶ += ಹೆಜ್ಜೆ )
        {
            std::fill(ದತ್ತಾಂಶ, ದತ್ತಾಂಶ + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, 0.0);
            ಆದರೆ (i < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
                ದತ್ತಾಂಶ[i] = val;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


ನಾಮಾಕಾಶ cv {

UMat UMat::eye(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat::eye(ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು), type, usageFlags);
}

UMat UMat::eye(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    ಗಣ_ತಟಸ್ಥ(m);
    ಹಿಂತಿರುಗಿ m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ಒಳಹರಿವು_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    ಪೂರ್ಣಾಂಕ type = m.type();
    ಪೂರ್ಣಾಂಕ nm = std::min(m.ಅಡ್ಡಸಾಲುಗಳು, m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

    ಆದರೆ( type == CV_32FC1 )
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ptr = m.ptr<ದಶಮಾಂಶ>();
        size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ptr[0]) + 1;
        ದ್ವಿಗುಣ _s = 0;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nm; i++ )
            _s += ptr[i*ಹೆಜ್ಜೆ];
        ಹಿಂತಿರುಗಿ _s;
    }

    ಆದರೆ( type == CV_64FC1 )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr = m.ptr<ದ್ವಿಗುಣ>();
        size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ptr[0]) + 1;
        ದ್ವಿಗುಣ _s = 0;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nm; i++ )
            _s += ptr[i*ಹೆಜ್ಜೆ];
        ಹಿಂತಿರುಗಿ _s;
    }

    ಹಿಂತಿರುಗಿ cv::ಮೊತ್ತ(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

ಶೂನ್ಯ cv::completeSymm( ಒಳಹರಿವು_Output_Array _m, ತಾರ್ಕಿಕ LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ, esz = m.elem_ಗಾತ್ರ();
    CV_Assert( m.dims <= 2 && m.ಅಡ್ಡಸಾಲುಗಳು == m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );

    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ j0 = 0, j1 = ಅಡ್ಡಸಾಲುಗಳು;

    uchar* ದತ್ತಾಂಶ = m.ptr();
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
    {
        ಆದರೆ( !LtoR ) j1 = i; ಇಲ್ಲದಿದ್ದರೆ j0 = i+1;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = j0; j < j1; j++ )
            ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ + (i*ಹೆಜ್ಜೆ + j*esz), ದತ್ತಾಂಶ + (j*ಹೆಜ್ಜೆ + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ಒಳಹರಿವು_Array _m) ಸ್ಥಿರಾಂಕ
{
    Mat m = _m.getMat();
    ಪೂರ್ಣಾಂಕ tp = type(), d = CV_MAT_ಆಳ(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((ಅಡ್ಡಸಾಲುಗಳು == 3 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) || (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*channels() == 3 && ಅಡ್ಡಸಾಲುಗಳು == 1)));
    Mat ಫಲಿತಾಂಶ(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, tp);

    ಆದರೆ( d == CV_32F )
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *a = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ದತ್ತಾಂಶ, *b = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)m.ದತ್ತಾಂಶ;
        ದಶಮಾಂಶ* c = (ದಶಮಾಂಶ*)ಫಲಿತಾಂಶ.ದತ್ತಾಂಶ;
        size_t lda = ಅಡ್ಡಸಾಲುಗಳು > 1 ? ಹೆಜ್ಜೆ/ಗಾತ್ರ(a[0]) : 1;
        size_t ldb = ಅಡ್ಡಸಾಲುಗಳು > 1 ? m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == CV_64F )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *a = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ದತ್ತಾಂಶ, *b = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)m.ದತ್ತಾಂಶ;
        ದ್ವಿಗುಣ* c = (ದ್ವಿಗುಣ*)ಫಲಿತಾಂಶ.ದತ್ತಾಂಶ;
        size_t lda = ಅಡ್ಡಸಾಲುಗಳು > 1 ? ಹೆಜ್ಜೆ/ಗಾತ್ರ(a[0]) : 1;
        size_t ldb = ಅಡ್ಡಸಾಲುಗಳು > 1 ? m.ಹೆಜ್ಜೆ/ಗಾತ್ರ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

ನಾಮಾಕಾಶ cv
{

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*Reduce_ಮೊತ್ತ_Func)(ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst);
Reduce_ಮೊತ್ತ_Func getReduceCSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
Reduce_ಮೊತ್ತ_Func getReduceRSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
Reduce_ಮೊತ್ತ_Func getReduceRSum2Func(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಬಗೆ_ಹೆಸರು Op>
ರಚನೆ ReduceR_SIMD
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ T*, ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಪೂರ್ಣಾಂಕ, WT*, ಸ್ಥಿರಾಂಕ Op&) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ ಆರಂಭ;
    }
};


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Op, ವರ್ಗ OpInit>
ವರ್ಗ ReduceR_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
  ReduceR_Invoker(ಸ್ಥಿರಾಂಕ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),ಬಫರ್(srcmat.size().ಅಗಲ*srcmat.channels())
  {
  }
  ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
  {
    ಸ್ಥಿರಾಂಕ T* src = srcmat.ptr<T>();
    ಸ್ಥಿರಾಂಕ size_t srcstep = srcmat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(src[0]);
    WT* buf = ಬಫರ್.ದತ್ತಾಂಶ();
    ST* dst = dstmat.ptr<ST>();
    ಪೂರ್ಣಾಂಕ i = 0;

    ಗಾಗಿ( i = range.ಆರಂಭ ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.size().ಎತ್ತರ;
    ಗಾಗಿ( ; --ಎತ್ತರ; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.ಆರಂಭ, range.end, buf, op);
        #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    ಗಾಗಿ( i = range.ಆರಂಭ ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ಖಾಸಗಿ:
  ಸ್ಥಿರಾಂಕ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  ಬದಲಾಯಿಸಬಹುದಾದ Auto_ಬಫರ್<WT> ಬಫರ್;
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ವರ್ಗ Op, ವರ್ಗ OpInit = OpNop<ST> > ಸ್ಥಿರ ಶೂನ್ಯ
reduceR_( ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().ಅಗಲ*srcmat.channels()), body, srcmat.size().ಅಗಲ*CV_ELEM_ಗಾತ್ರ(srcmat.ಆಳ())/64);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Op, ವರ್ಗ OpInit>
ವರ್ಗ ReduceC_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
  ReduceC_Invoker(ಸ್ಥಿರಾಂಕ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
  {
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = srcmat.size().ಅಗಲ*cn;
    Auto_ಬಫರ್<WT> cumul(cn);
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++ )
    {
        ಸ್ಥಿರಾಂಕ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        ಆದರೆ( ಅಗಲ == cn )
        {
          ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ k = cn ; k < ಅಗಲ ; k += cn )
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ಖಾಸಗಿ:
  ಸ್ಥಿರಾಂಕ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ವರ್ಗ Op, ವರ್ಗ OpInit = OpNop<ST> > ಸ್ಥಿರ ಶೂನ್ಯ
reduceC_( ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().ಎತ್ತರ), body);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ReduceFunc)( ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst );

}

#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R8u32s  reduceR_<uchar, ಪೂರ್ಣಾಂಕ,   Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R8u32f  reduceR_<uchar, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R8u64f  reduceR_<uchar, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R16u32f reduceR_<ushort,ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R16u64f reduceR_<ushort,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R16s32f reduceR_<ಗಿಡ್ಡ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R16s64f reduceR_<ಗಿಡ್ಡ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R32f32f reduceR_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R32f64f reduceR_<ದಶಮಾಂಶ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_R64f64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >

#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R8u32s  reduceR_<uchar, ಪೂರ್ಣಾಂಕ,   Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R8u32f  reduceR_<uchar, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R8u64f  reduceR_<uchar, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R16u32f reduceR_<ushort,ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R16u64f reduceR_<ushort,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R16s32f reduceR_<ಗಿಡ್ಡ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R16s64f reduceR_<ಗಿಡ್ಡ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R32f32f reduceR_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R32f64f reduceR_<ದಶಮಾಂಶ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2R64f64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxR16s reduceR_<ಗಿಡ್ಡ, ಗಿಡ್ಡ, OpMax<ಗಿಡ್ಡ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxR32f reduceR_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, OpMax<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxR64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMax<ದ್ವಿಗುಣ> >

#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinR16s reduceR_<ಗಿಡ್ಡ, ಗಿಡ್ಡ, OpMin<ಗಿಡ್ಡ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinR32f reduceR_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, OpMin<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinR64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMin<ದ್ವಿಗುಣ> >

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ ipp_reduce_ಮೊತ್ತ_C_8u16u16s32f_64f(ಸ್ಥಿರಾಂಕ cv::Mat& srcmat, cv::Mat& dstmat)
{
    ಪೂರ್ಣಾಂಕ sstep = (ಪೂರ್ಣಾಂಕ)srcmat.ಹೆಜ್ಜೆ, stype = srcmat.type(),
            ddepth = dstmat.ಆಳ();

    Ippi_ಗಾತ್ರ roisize = { srcmat.size().ಅಗಲ, 1 };

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * Ippi_ಮೊತ್ತ)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pSrc, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_3, Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, Ipp64f* p_ಮೊತ್ತ);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * Ippi_ಮೊತ್ತ_Hint)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pSrc, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_3, Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, Ipp64f* p_ಮೊತ್ತ, IppHintAlgorithm hint);
    Ippi_ಮೊತ್ತ ippi_ಮೊತ್ತ = 0;
    Ippi_ಮೊತ್ತ_Hint ippi_ಮೊತ್ತ_Hint = 0;

    ಆದರೆ(ddepth == CV_64F)
    {
        ippi_ಮೊತ್ತ =
            stype == CV_8UC1 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_ಮೊತ್ತ)ippi_ಮೊತ್ತ_16s_C4_R : 0;
        ippi_ಮೊತ್ತ_Hint =
            stype == CV_32FC1 ? (Ippi_ಮೊತ್ತ_Hint)ippi_ಮೊತ್ತ_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_ಮೊತ್ತ_Hint)ippi_ಮೊತ್ತ_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_ಮೊತ್ತ_Hint)ippi_ಮೊತ್ತ_32f_C4_R : 0;
    }

    ಆದರೆ(ippi_ಮೊತ್ತ)
    {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ y = 0; y < srcmat.size().ಎತ್ತರ; y++)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ippi_ಮೊತ್ತ, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ippi_ಮೊತ್ತ_Hint)
    {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ y = 0; y < srcmat.size().ಎತ್ತರ; y++)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ippi_ಮೊತ್ತ_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ reduce_ಮೊತ್ತ_C_8u16u16s32f_64f(ಸ್ಥಿರಾಂಕ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_ಸತತ_FAST(ipp_reduce_ಮೊತ್ತ_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    ಆದರೆ(dstmat.ಆಳ() == CV_64F)
    {
        ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ದ್ವಿಗುಣ,   cv::Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ದ್ವಿಗುಣ,   cv::Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<ಗಿಡ್ಡ, ದ್ವಿಗುಣ,   cv::Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ದಶಮಾಂಶ, ದ್ವಿಗುಣ,   cv::Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C8u32s  reduceC_<uchar, ಪೂರ್ಣಾಂಕ,   Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C8u32f  reduceC_<uchar, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16u32f reduceC_<ushort,ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16s32f reduceC_<ಗಿಡ್ಡ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C32f32f reduceC_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C64f64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >

#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C8u32s  reduceC_<uchar, ಪೂರ್ಣಾಂಕ,   Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C8u32f  reduceC_<uchar, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C16u32f reduceC_<ushort,ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C16s32f reduceC_<ಗಿಡ್ಡ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C32f32f reduceC_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, Op_ಕೂಡಿಸು_Sqr<ದಶಮಾಂಶ>, OpSqr<ದಶಮಾಂಶ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C64f64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C8u64f  reduce_ಮೊತ್ತ_C_8u16u16s32f_64f
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16u64f reduce_ಮೊತ್ತ_C_8u16u16s32f_64f
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16s64f reduce_ಮೊತ್ತ_C_8u16u16s32f_64f
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C32f64f reduce_ಮೊತ್ತ_C_8u16u16s32f_64f
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C8u64f  reduceC_<uchar, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ಪೂರ್ಣಾಂಕ>, OpNop<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16u64f reduceC_<ushort,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C16s64f reduceC_<ಗಿಡ್ಡ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduce_ಮೊತ್ತ_C32f64f reduceC_<ದಶಮಾಂಶ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು<ದ್ವಿಗುಣ> >
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C8u64f  reduceC_<uchar, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ಪೂರ್ಣಾಂಕ>,   OpSqr<ಪೂರ್ಣಾಂಕ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C16u64f reduceC_<ushort,ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C16s64f reduceC_<ಗಿಡ್ಡ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceSum2C32f64f reduceC_<ದಶಮಾಂಶ, ದ್ವಿಗುಣ,Op_ಕೂಡಿಸು_Sqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#ವ್ಯಾಖ್ಯಾನಿಸು REDUCE_OP(favor, optype, type1, type2) \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ ipp_reduce##optype##C##favor(ಸ್ಥಿರಾಂಕ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    ಆದರೆ((srcmat.channels() == 1)) \
    { \
        ಪೂರ್ಣಾಂಕ sstep = (ಪೂರ್ಣಾಂಕ)srcmat.ಹೆಜ್ಜೆ; \
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp##favor IppType; \
        Ippi_ಗಾತ್ರ roisize = ippi_ಗಾತ್ರ(srcmat.size().ಅಗಲ, 1);\
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ y = 0; y < srcmat.size().ಎತ್ತರ; y++)\
        {\
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;\
        }\
        ಹಿಂತಿರುಗಿ ನಿಜ;\
    }\
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ reduce##optype##C##favor(ಸ್ಥಿರಾಂಕ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_ಸತತ_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, ಗಿಡ್ಡ, ಗಿಡ್ಡ)
REDUCE_OP(32f, Max, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxC16s reduceC_<ಗಿಡ್ಡ, ಗಿಡ್ಡ, OpMax<ಗಿಡ್ಡ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxC32f reduceC_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, OpMax<ದಶಮಾಂಶ> >
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMaxC64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMax<ದ್ವಿಗುಣ> >

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, ಗಿಡ್ಡ, ಗಿಡ್ಡ)
REDUCE_OP(32f, Min, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinC16s reduceC_<ಗಿಡ್ಡ, ಗಿಡ್ಡ, OpMin<ಗಿಡ್ಡ> >
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinC32f reduceC_<ದಶಮಾಂಶ, ದಶಮಾಂಶ, OpMin<ದಶಮಾಂಶ> >
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು reduceMinC64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMin<ದ್ವಿಗುಣ> >

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst,
                       ಪೂರ್ಣಾಂಕ dim, ಪೂರ್ಣಾಂಕ op, ಪೂರ್ಣಾಂಕ op0, ಪೂರ್ಣಾಂಕ stype, ಪೂರ್ಣಾಂಕ dtype)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ min_opt_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 128, buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 32;
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_ಆಳ(dtype), ddepth0 = ddepth;
    ಸ್ಥಿರಾಂಕ ocl::Device &defDev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_ಗುಂಪು_ಗಾತ್ರ();
    ತಾರ್ಕಿಕ useOptimized = 1 == dim && _src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ() > min_opt_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && (wgs >= buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

    ಆದರೆ (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (op == REDUCE_AVG)
    {
        ಆದರೆ (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    ಪೂರ್ಣಾಂಕ wdepth = std::max(ddepth, CV_32F);
    ಆದರೆ (useOptimized)
    {
        size_t tile_ಎತ್ತರ = (size_t)(wgs / buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಆದರೆ (defDev.isIntel())
        {
            ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t max_ಅಂಶ_ಒಳ_ಗುಂಪು_ಎಣಿಕೆ = 16;
            tile_ಎತ್ತರ = min(tile_ಎತ್ತರ, defDev.local_Mem_ಗಾತ್ರ() / buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ / CV_ELEM_ಗಾತ್ರ(CV_MAKETYPE(wdepth, cn)) / max_ಅಂಶ_ಒಳ_ಗುಂಪು_ಎಣಿಕೆ);
        }
        ಅಕ್ಷರ cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, tile_ಎತ್ತರ, ops[op], cn, ddepth,
                                            ocl::type_ಗಮ್ಯ_Str(sdepth),
                                            ocl::type_ಗಮ್ಯ_Str(ddepth),
                                            ocl::type_ಗಮ್ಯ_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ಗಾತ್ರ(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], ಗಾತ್ರ(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], ಗಾತ್ರ(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        ಆದರೆ (k.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        UMat src = _src.getUMat();
        ಗಾತ್ರ_2 dsize(1, src.ಅಡ್ಡಸಾಲುಗಳು);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ಆದರೆ (op0 == REDUCE_AVG)
            k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst), 1.0f / src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst));

        size_t local_ಗಾತ್ರ[2] = { (size_t)buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)tile_ಎತ್ತರ};
        size_t global_ಗಾತ್ರ[2] = { (size_t)buf_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)src.ಅಡ್ಡಸಾಲುಗಳು };
        ಹಿಂತಿರುಗಿ k.ಸತತ(2, global_ಗಾತ್ರ, local_ಗಾತ್ರ, ಸುಳ್ಳು);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಅಕ್ಷರ cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_ಗಮ್ಯ_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_ಗಮ್ಯ_Str(ddepth), ocl::type_ಗಮ್ಯ_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ಗಾತ್ರ(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], ಗಾತ್ರ(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], ಗಾತ್ರ(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        ಆದರೆ (k.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        UMat src = _src.getUMat();
        ಗಾತ್ರ_2 dsize(dim == 0 ? src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : 1, dim == 0 ? 1 : src.ಅಡ್ಡಸಾಲುಗಳು);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst);

        ಆದರೆ (op0 == REDUCE_AVG)
            k.ವಾದಗಳು(srcarg, temparg, 1.0f / (dim == 0 ? src.ಅಡ್ಡಸಾಲುಗಳು : src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ));
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(srcarg, temparg);

        size_t globalsize = std::max(dsize.ಅಗಲ, dsize.ಎತ್ತರ);
        ಹಿಂತಿರುಗಿ k.ಸತತ(1, &globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
    }
}

}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ dim, ಪೂರ್ಣಾಂಕ op, ಪೂರ್ಣಾಂಕ dtype)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, ನಿಜ, dtype < 0 && _dst.fixedType() ? _dst.ಆಳ() : dtype));
        ಹಿಂತಿರುಗಿ;
    }
    ಪೂರ್ಣಾಂಕ op0 = op;
    ಪೂರ್ಣಾಂಕ stype = _src.type(), sdepth = CV_MAT_ಆಳ(stype), cn = CV_MAT_CN(stype);
    ಆದರೆ( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    ಪೂರ್ಣಾಂಕ ddepth = CV_MAT_ಆಳ(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_ಮೊತ್ತ || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_ಸತತ(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    ಆದರೆ (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    ಆದರೆ (src.empty()) {
        src.convert_ಗಮ್ಯ(_dst, dtype);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.ಅಡ್ಡಸಾಲುಗಳು, dim == 0 ? src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : 1, dtype);
    Mat dst = _dst.getMat(), ತಾತ್ಕಾಲಿಕ_2 = dst;

    ಆದರೆ( op == REDUCE_AVG )
    {
        op = REDUCE_ಮೊತ್ತ;
        ಆದರೆ( sdepth < CV_32S && ddepth < CV_32S )
        {
            ತಾತ್ಕಾಲಿಕ_2.create(dst.ಅಡ್ಡಸಾಲುಗಳು, dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    ಆದರೆ( dim == 0 )
    {
        ಆದರೆ( op == REDUCE_ಮೊತ್ತ )
        {
            Reduce_ಮೊತ್ತ_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            ಆದರೆ(simd_func)
                func = (ReduceFunc)simd_func;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_ಮೊತ್ತ_R8u32s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_R8u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_R8u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_R16u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_R16u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_R16s32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_R16s64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_R32f32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_R32f64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_R64f64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(op == REDUCE_MAX)
        {
            ಆದರೆ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(op == REDUCE_MIN)
        {
            ಆದರೆ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( op == REDUCE_SUM2 )
        {
            Reduce_ಮೊತ್ತ_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            ಆದರೆ(simd_func)
                func = simd_func;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ(op == REDUCE_ಮೊತ್ತ)
        {
            Reduce_ಮೊತ್ತ_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            ಆದರೆ(simd_func)
                func = (ReduceFunc)simd_func;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_ಮೊತ್ತ_C8u32s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_C8u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_C8u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_C16u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_C16u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_C16s32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_C16s64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_ಮೊತ್ತ_C32f32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_C32f64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_ಮೊತ್ತ_C64f64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(op == REDUCE_MAX)
        {
            ಆದರೆ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(op == REDUCE_MIN)
        {
            ಆದರೆ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(op == REDUCE_SUM2)
        {
            ಆದರೆ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    ಆದರೆ( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, ತಾತ್ಕಾಲಿಕ_2 );

    ಆದರೆ( op0 == REDUCE_AVG )
        ತಾತ್ಕಾಲಿಕ_2.convert_ಗಮ್ಯ(dst, dst.type(), 1./(dim == 0 ? src.ಅಡ್ಡಸಾಲುಗಳು : src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು( ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst, ಪೂರ್ಣಾಂಕ flags )
{
    ಪೂರ್ಣಾಂಕ n, len;
    ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು = (flags & 1) == ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು;
    ತಾರ್ಕಿಕ inplace = src.ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ;
    ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_Descending = (flags & ವಿಂಗಡಿಸು_DESCENDING) != 0;

    ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
        n = src.ಅಡ್ಡಸಾಲುಗಳು, len = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಇಲ್ಲದಿದ್ದರೆ
        n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, len = src.ಅಡ್ಡಸಾಲುಗಳು;
    parallel_for_(Range(0, n), [&](ಸ್ಥಿರಾಂಕ Range& range)
    {
        Auto_ಬಫರ್<T> buf;
        ಆದರೆ( !ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            buf.allocate(len);
        T* bptr = buf.ದತ್ತಾಂಶ();

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; i++ )
        {
            T* ptr = bptr;
            ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            {
                T* dptr = dst.ptr<T>(i);
                ಆದರೆ( !inplace )
                {
                    ಸ್ಥಿರಾಂಕ T* sptr = src.ptr<T>(i);
                    ಸ್ಮರಣೆ_ನಕಲು(dptr, sptr, ಗಾತ್ರ(T) * len);
                }
                ptr = dptr;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            ಆದರೆ( ವಿಂಗಡಿಸು_Descending )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            ಆದರೆ( !ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_ವಿಂಗಡಿಸು
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL *Ipp_ವಿಂಗಡಿಸು_Func)(ಶೂನ್ಯ  *pSrcDst, ಪೂರ್ಣಾಂಕ    len, Ipp8u *p_ಬಫರ್);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(T*, ಪೂರ್ಣಾಂಕ, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ipp_ವಿಂಗಡಿಸು_Wrap(ಶೂನ್ಯ* pSrcDst, ಪೂರ್ಣಾಂಕ len, Ipp8u* p_ಬಫರ್)
{
    ಹಿಂತಿರುಗಿ fn((T*)pSrcDst, len, p_ಬಫರ್);
}

ಸ್ಥಿರ Ipp_ವಿಂಗಡಿಸು_Func get_ವಿಂಗಡಿಸು_Func(ಪೂರ್ಣಾಂಕ ಆಳ, ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_Descending)
{
    ಆದರೆ (!ವಿಂಗಡಿಸು_Descending)
        ಹಿಂತಿರುಗಿ ಆಳ == CV_8U ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp8u, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_8u_I> :
            ಆಳ == CV_16U ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp16u, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_16u_I> :
            ಆಳ == CV_16S ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp16s, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_16s_I> :
            ಆಳ == CV_32S ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp32s, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_32s_I> :
            ಆಳ == CV_32F ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp32f, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_32f_I> :
            ಆಳ == CV_64F ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp64f, ipps_ವಿಂಗಡಿಸು_Radix_Ascend_64f_I> :
            0;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ ಆಳ == CV_8U ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp8u, ipps_ವಿಂಗಡಿಸು_Radix_Descend_8u_I> :
            ಆಳ == CV_16U ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp16u, ipps_ವಿಂಗಡಿಸು_Radix_Descend_16u_I> :
            ಆಳ == CV_16S ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp16s, ipps_ವಿಂಗಡಿಸು_Radix_Descend_16s_I> :
            ಆಳ == CV_32S ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp32s, ipps_ವಿಂಗಡಿಸು_Radix_Descend_32s_I> :
            ಆಳ == CV_32F ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp32f, ipps_ವಿಂಗಡಿಸು_Radix_Descend_32f_I> :
            ಆಳ == CV_64F ? ipp_ವಿಂಗಡಿಸು_Wrap<Ipp64f, ipps_ವಿಂಗಡಿಸು_Radix_Descend_64f_I> :
            0;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ವಿಂಗಡಿಸು(ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION_IPP();

    ತಾರ್ಕಿಕ        ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು        = (flags & 1) == ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು;
    ತಾರ್ಕಿಕ        ವಿಂಗಡಿಸು_Descending  = (flags & ವಿಂಗಡಿಸು_DESCENDING) != 0;
    ತಾರ್ಕಿಕ        inplace         = (src.ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ);
    ಪೂರ್ಣಾಂಕ         ಆಳ           = src.ಆಳ();
    Ipp_ದತ್ತಾಂಶ_Type type            = ippi_Get_ದತ್ತಾಂಶ_Type(ಆಳ);

    Ipp_ವಿಂಗಡಿಸು_Func ipps_ವಿಂಗಡಿಸು_Radix_I = get_ವಿಂಗಡಿಸು_Func(ಆಳ, ವಿಂಗಡಿಸು_Descending);
    ಆದರೆ(!ipps_ವಿಂಗಡಿಸು_Radix_I)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ n, len;
    ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
        n = src.ಅಡ್ಡಸಾಲುಗಳು, len = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಇಲ್ಲದಿದ್ದರೆ
        n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, len = src.ಅಡ್ಡಸಾಲುಗಳು;

    ಪೂರ್ಣಾಂಕ ಬಫರ್_ಗಾತ್ರ;
    ಆದರೆ(ipps_ವಿಂಗಡಿಸು_Radix_Get_ಬಫರ್_ಗಾತ್ರ(len, type, &ಬಫರ್_ಗಾತ್ರ) < 0)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    ಅಸ್ಥಿರ ತಾರ್ಕಿಕ ok = ನಿಜ;
    parallel_for_(Range(0, n), [&](ಸ್ಥಿರಾಂಕ Range& range)
    {
        Auto_ಬಫರ್<Ipp8u> ಬಫರ್(ಬಫರ್_ಗಾತ್ರ);
        Mat  ಅಡ್ಡಸಾಲು;
        Rect subRect(0,0,1,src.ಅಡ್ಡಸಾಲುಗಳು);
        ಆದರೆ( !ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            ಅಡ್ಡಸಾಲು.create(1, src.ಅಡ್ಡಸಾಲುಗಳು, src.type());

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; i++ )
        {
            ಆದರೆ( !ok )
                ನಿಲ್ಲಿಸು;
            ಶೂನ್ಯ* ptr;
            ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            {
                uchar* dptr = dst.ptr(i);
                ಆದರೆ( !inplace )
                    ಸ್ಮರಣೆ_ನಕಲು(dptr, src.ptr(i), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * src.elem_ಗಾತ್ರ());
                ptr = dptr;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                subRect.x = i;
                Mat(src, subRect).copy_ಗಮ್ಯ(ಅಡ್ಡಸಾಲು);
                ptr = ಅಡ್ಡಸಾಲು.ptr();
            }

            ಆದರೆ( CV_INSTRUMENT_FUN_IPP(ipps_ವಿಂಗಡಿಸು_Radix_I, ptr, len, ಬಫರ್.ದತ್ತಾಂಶ()) < 0 )
            {
                ok = ಸುಳ್ಳು;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ( !ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            {
                Mat dstSub(dst, subRect);
                ಅಡ್ಡಸಾಲು = ಅಡ್ಡಸಾಲು.reshape(1, dstSub.ಅಡ್ಡಸಾಲುಗಳು);
                ಅಡ್ಡಸಾಲು.copy_ಗಮ್ಯ(dstSub);
                ಅಡ್ಡಸಾಲು = ಅಡ್ಡಸಾಲು.reshape(1, 1);
            }
        }
    });

    ಹಿಂತಿರುಗಿ ok;
}
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ವರ್ಗ LessThanIdx
{
ಸಾರ್ವಜನಿಕ:
    LessThanIdx( ಸ್ಥಿರಾಂಕ _Tp* _arr ) : arr(_arr) {}
    ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ()(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ arr[a] < arr[b]; }
    ಸ್ಥಿರಾಂಕ _Tp* arr;
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು_Idx_2( ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst, ಪೂರ್ಣಾಂಕ flags )
{
    Auto_ಬಫರ್<T> buf;
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> ibuf;
    ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು = (flags & 1) == ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು;
    ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_Descending = (flags & ವಿಂಗಡಿಸು_DESCENDING) != 0;

    CV_Assert( src.ದತ್ತಾಂಶ != dst.ದತ್ತಾಂಶ );

    ಪೂರ್ಣಾಂಕ n, len;
    ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
        n = src.ಅಡ್ಡಸಾಲುಗಳು, len = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, len = src.ಅಡ್ಡಸಾಲುಗಳು;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.ದತ್ತಾಂಶ();
    ಪೂರ್ಣಾಂಕ* _iptr = ibuf.ದತ್ತಾಂಶ();

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        ಪೂರ್ಣಾಂಕ* iptr = _iptr;

        ಆದರೆ( ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
        {
            ptr = (T*)(src.ದತ್ತಾಂಶ + src.ಹೆಜ್ಜೆ*i);
            iptr = dst.ptr<ಪೂರ್ಣಾಂಕ>(i);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        ಆದರೆ( ವಿಂಗಡಿಸು_Descending )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        ಆದರೆ( !ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು )
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < len; j++ )
                dst.ptr<ಪೂರ್ಣಾಂಕ>(j)[i] = iptr[j];
    }
}

#ಆದರೆ defined(HAVE_IPP) && !IPP_DISABLE_ವಿಂಗಡಿಸು
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL *Ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Func)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  pSrc, Ipp32s src_Stride_ಬೈಟುಗಳು, Ipp32s *pDstIndx, ಪೂರ್ಣಾಂಕ len, Ipp8u *p_ಬಫರ್);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(ಸ್ಥಿರಾಂಕ T*, Ipp32s, Ipp32s*, ಪೂರ್ಣಾಂಕ, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* pSrc, Ipp32s src_Stride_ಬೈಟುಗಳು, Ipp32s *pDstIndx, ಪೂರ್ಣಾಂಕ len, Ipp8u *p_ಬಫರ್)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ T*)pSrc, src_Stride_ಬೈಟುಗಳು, pDstIndx, len, p_ಬಫರ್);
}

ಸ್ಥಿರ Ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Func get_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Func(ಪೂರ್ಣಾಂಕ ಆಳ, ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_Descending)
{
    ಆದರೆ (!ವಿಂಗಡಿಸು_Descending)
        ಹಿಂತಿರುಗಿ ಆಳ == CV_8U ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp8u, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Ascend_8u> :
            ಆಳ == CV_16U ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp16u, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Ascend_16u> :
            ಆಳ == CV_16S ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp16s, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Ascend_16s> :
            ಆಳ == CV_32S ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp32s, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Ascend_32s> :
            ಆಳ == CV_32F ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp32f, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Ascend_32f> :
            0;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ ಆಳ == CV_8U ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp8u, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Descend_8u> :
            ಆಳ == CV_16U ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp16u, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Descend_16u> :
            ಆಳ == CV_16S ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp16s, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Descend_16s> :
            ಆಳ == CV_32S ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp32s, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Descend_32s> :
            ಆಳ == CV_32F ? ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Wrap<Ipp32f, ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Descend_32f> :
            0;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ವಿಂಗಡಿಸು_Idx( ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION_IPP();

    ತಾರ್ಕಿಕ        ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು        = (flags & 1) == ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು;
    ತಾರ್ಕಿಕ        ವಿಂಗಡಿಸು_Descending  = (flags & ವಿಂಗಡಿಸು_DESCENDING) != 0;
    ಪೂರ್ಣಾಂಕ         ಆಳ           = src.ಆಳ();
    Ipp_ದತ್ತಾಂಶ_Type type            = ippi_Get_ದತ್ತಾಂಶ_Type(ಆಳ);

    Ipp_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Func ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ = get_ವಿಂಗಡಿಸು_ಸೂಚ್ಯಂಕ_Func(ಆಳ, ವಿಂಗಡಿಸು_Descending);
    ಆದರೆ(!ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(ವಿಂಗಡಿಸು_ಅಡ್ಡಸಾಲುಗಳು)
    {
        Auto_ಬಫರ್<Ipp8u> ಬಫರ್;
        ಪೂರ್ಣಾಂಕ               ಬಫರ್_ಗಾತ್ರ;
        ಆದರೆ(ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Get_ಬಫರ್_ಗಾತ್ರ(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, &ಬಫರ್_ಗಾತ್ರ) < 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಬಫರ್.allocate(ಬಫರ್_ಗಾತ್ರ);

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < src.ಅಡ್ಡಸಾಲುಗಳು; i++)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)src.ptr(i), (Ipp32s)src.ಹೆಜ್ಜೆ[1], (Ipp32s*)dst.ptr(i), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಬಫರ್.ದತ್ತಾಂಶ()) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat  dst_ಅಡ್ಡಸಾಲು(1, dst.ಅಡ್ಡಸಾಲುಗಳು, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.ಅಡ್ಡಸಾಲುಗಳು);

        Auto_ಬಫರ್<Ipp8u> ಬಫರ್;
        ಪೂರ್ಣಾಂಕ               ಬಫರ್_ಗಾತ್ರ;
        ಆದರೆ(ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ_Get_ಬಫರ್_ಗಾತ್ರ(src.ಅಡ್ಡಸಾಲುಗಳು, type, &ಬಫರ್_ಗಾತ್ರ) < 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಬಫರ್.allocate(ಬಫರ್_ಗಾತ್ರ);

        Ipp32s src_ಹೆಜ್ಜೆ_3 = (Ipp32s)src.ಹೆಜ್ಜೆ[0];
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ipps_ವಿಂಗಡಿಸು_Radix_ಸೂಚ್ಯಂಕ, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)src.ptr(0, i), src_ಹೆಜ್ಜೆ_3, (Ipp32s*)dst_ಅಡ್ಡಸಾಲು.ptr(), src.ಅಡ್ಡಸಾಲುಗಳು, ಬಫರ್.ದತ್ತಾಂಶ()) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

            dst_ಅಡ್ಡಸಾಲು = dst_ಅಡ್ಡಸಾಲು.reshape(1, dstSub.ಅಡ್ಡಸಾಲುಗಳು);
            dst_ಅಡ್ಡಸಾಲು.copy_ಗಮ್ಯ(dstSub);
        }
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ವಿಂಗಡಿಸು_Func)(ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst, ಪೂರ್ಣಾಂಕ flags);
}

ಶೂನ್ಯ cv::sort( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_ಗಾತ್ರ( src, src.type() );
    Mat dst = _dst.getMat();
#ಆದರೆ !IPP_DISABLE_ವಿಂಗಡಿಸು
    CV_IPP_ಸತತ_FAST(ipp_ವಿಂಗಡಿಸು(src, dst, flags));
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರ ವಿಂಗಡಿಸು_Func tab[CV_ಆಳ_MAX] =
    {
        ವಿಂಗಡಿಸು<uchar>, ವಿಂಗಡಿಸು<schar>, ವಿಂಗಡಿಸು<ushort>, ವಿಂಗಡಿಸು<ಗಿಡ್ಡ>,
        ವಿಂಗಡಿಸು<ಪೂರ್ಣಾಂಕ>, ವಿಂಗಡಿಸು<ದಶಮಾಂಶ>, ವಿಂಗಡಿಸು<ದ್ವಿಗುಣ>, 0
    };
    ವಿಂಗಡಿಸು_Func func = tab[src.ಆಳ()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

ಶೂನ್ಯ cv::ವಿಂಗಡಿಸು_Idx( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    ಆದರೆ( dst.ದತ್ತಾಂಶ == src.ದತ್ತಾಂಶ )
        _dst.release();
    _dst.create_Same_ಗಾತ್ರ( src, CV_32S );
    dst = _dst.getMat();

#ಆದರೆ !IPP_DISABLE_ವಿಂಗಡಿಸು
    CV_IPP_ಸತತ_FAST(ipp_ವಿಂಗಡಿಸು_Idx(src, dst, flags));
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರ ವಿಂಗಡಿಸು_Func tab[CV_ಆಳ_MAX] =
    {
        ವಿಂಗಡಿಸು_Idx_2<uchar>, ವಿಂಗಡಿಸು_Idx_2<schar>, ವಿಂಗಡಿಸು_Idx_2<ushort>, ವಿಂಗಡಿಸು_Idx_2<ಗಿಡ್ಡ>,
        ವಿಂಗಡಿಸು_Idx_2<ಪೂರ್ಣಾಂಕ>, ವಿಂಗಡಿಸು_Idx_2<ದಶಮಾಂಶ>, ವಿಂಗಡಿಸು_Idx_2<ದ್ವಿಗುಣ>, 0
    };
    ವಿಂಗಡಿಸು_Func func = tab[src.ಆಳ()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
