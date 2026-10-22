// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#അന്തര്ഭാവ "opencv2/core/mat.hpp"

നാമാകാശ cv {

#ifdef HAVE_OPENCL

എണ്ണൽ { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
തർക്കം ocl_sum( നിവേശം_Array _src, Scalar & res, പൂർണ്ണസംഖ്യ sum_op, നിവേശം_Array _mask = noArray(),
                     നിവേശം_Array _src2 = noArray(), തർക്കം calc2 = അസത്യം, സ്ഥിരം Scalar & res2 = Scalar() );
തർക്കം ocl_minMaxIdx( നിവേശം_Array _src, ഇരട്ട* minVal, ഇരട്ട* maxVal, പൂർണ്ണസംഖ്യ* minLoc, പൂർണ്ണസംഖ്യ* maxLoc, നിവേശം_Array _mask,
                           പൂർണ്ണസംഖ്യ ddepth = -1, തർക്കം abs_മൂല്യങ്ങൾ = അസത്യം, നിവേശം_Array _src2 = noArray(), ഇരട്ട * maxVal2 = ശൂന്യ);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    പൂർണ്ണസംഖ്യ cn = m.channels();
    സ്ഥിരം T * സ്ഥിരം ptr = m.ptr<T>(0);

    വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0, w = m.cols * cn; x < w; )
        വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    തിരികെ s;
}

#endif

തരനിർവചനം പൂർണ്ണസംഖ്യ (*SumFunc)(സ്ഥിരം uchar*, സ്ഥിരം uchar* mask, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);
SumFunc getSumFunc(പൂർണ്ണസംഖ്യ depth);

}

#endif // SRC_STAT_HPP
