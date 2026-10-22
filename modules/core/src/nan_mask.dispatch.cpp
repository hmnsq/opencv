// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "nan_mask.simd.hpp"
#અન્તર્ભાવ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_patchNaNs( આગત_Output_Array _a, દ્વિગુણ value )
{
    પૂર્ણાંક ftype = _a.depth();

    અચલ ocl::Device d = ocl::Device::getDefault();
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    જો (!doubleSupport && ftype == CV_64F)
    {
        પાછા ખોટું;
    }

    પૂર્ણાંક rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    જો (k.empty())
        પાછા ખોટું;

    UMat a = _a.getUMat();
    પૂર્ણાંક cn = a.channels();

    // to pass float or double to args
    જો (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (દશાંશ)value);
    }
    નહીંતર // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

સ્થિર PatchNanFunc getPatchNanFunc(તાર્કિક isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય patchNaNs( આગત_Output_Array _a, દ્વિગુણ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    અચલ Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    માટે (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_finiteMask(અચલ UMat img, UMat mask)
{
    પૂર્ણાંક channels = img.channels();
    પૂર્ણાંક depth = img.depth();

    અચલ ocl::Device d = ocl::Device::getDefault();
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    જો (!doubleSupport && depth == CV_64F)
    {
        પાછા ખોટું;
    }

    પૂર્ણાંક rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    જો (k.empty())
        પાછા ખોટું;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

સ્થિર FiniteMaskFunc getFiniteMaskFunc(તાર્કિક isDouble, પૂર્ણાંક cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય finiteMask(આગત_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક channels = _src.channels();
    પૂર્ણાંક depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<પૂર્ણાંક> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    અચલ Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    માટે( i = 0; i < nplanes; i++, ++it )
    {
        અચલ uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv