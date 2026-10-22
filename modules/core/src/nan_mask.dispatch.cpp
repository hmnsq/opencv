// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "nan_mask.simd.hpp"
#समावेश "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_patchNaNs( निवेश_Output_Array _a, द्विगुन value )
{
    पूर्णांक ftype = _a.depth();

    स्थिर ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    अगर (!doubleSupport && ftype == CV_64F)
    {
        वापस false;
    }

    पूर्णांक rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    अगर (k.empty())
        वापस false;

    UMat a = _a.getUMat();
    पूर्णांक cn = a.channels();

    // to pass float or double to args
    अगर (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::Read_Only_No_आकार(a), ocl::KernelArg::WriteOnly(a, cn), (भग्न)value);
    }
    अन्यथा // CV_64F
    {
        k.args(ocl::KernelArg::Read_Only_No_आकार(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    माप_प्रकार globalsize[2] = { (माप_प्रकार)a.cols * cn, ((माप_प्रकार)a.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

अटल PatchNanFunc getPatchNanFunc(bool isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

व्योम patchNaNs( निवेश_Output_Array _a, द्विगुन _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    स्थिर Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    माप_प्रकार len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#अगर_घोषित HAVE_OPENCL

अटल bool ocl_finiteMask(स्थिर UMat img, UMat mask)
{
    पूर्णांक channels = img.channels();
    पूर्णांक depth = img.depth();

    स्थिर ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    अगर (!doubleSupport && depth == CV_64F)
    {
        वापस false;
    }

    पूर्णांक rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    अगर (k.empty())
        वापस false;

    k.args(ocl::KernelArg::Read_Only_No_आकार(img), ocl::KernelArg::WriteOnly(mask));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)img.cols, ((माप_प्रकार)img.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

अटल FiniteMaskFunc getFiniteMaskFunc(bool isDouble, पूर्णांक cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

व्योम finiteMask(निवेश_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक channels = _src.channels();
    पूर्णांक depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<पूर्णांक> vsz(_src.dims());
    _src.sizend(vsz.तथ्य());
    _mask.create(_src.dims(), vsz.तथ्य(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    स्थिर Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    माप_प्रकार योग = planes[0].योग();
    माप_प्रकार i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    क्रम( i = 0; i < nplanes; i++, ++it )
    {
        स्थिर uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, योग);
    }
}
} //namespace cv