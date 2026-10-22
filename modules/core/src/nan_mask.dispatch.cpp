// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "nan_mask.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_patchNaNs( ନିବେଶ_Output_Array _a, ଦ୍ୱିଗୁଣ value )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ftype = _a.depth();

    ଅଚଳ ocl::Device d = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    ଯଦି (!doubleSupport && ftype == CV_64F)
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat a = _a.getUMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = a.channels();

    // to pass float or double to args
    ଯଦି (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (ଦଶମିକ)value);
    }
    ଅନ୍ୟଥା // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ସ୍ଥିର PatchNanFunc getPatchNanFunc(ତାର୍କିକ isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ patchNaNs( ନିବେଶ_Output_Array _a, ଦ୍ୱିଗୁଣ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ଅଚଳ Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_finiteMask(ଅଚଳ UMat img, UMat mask)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels = img.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = img.depth();

    ଅଚଳ ocl::Device d = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    ଯଦି (!doubleSupport && depth == CV_64F)
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ସ୍ଥିର FiniteMaskFunc getFiniteMaskFunc(ତାର୍କିକ isDouble, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ finiteMask(ନିବେଶ_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels = _src.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ଅଚଳ Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    ପାଇଁ( i = 0; i < nplanes; i++, ++it )
    {
        ଅଚଳ uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv