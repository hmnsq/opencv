// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#ଅନ୍ତର୍ଭାଵ "opencv2/core/mat.hpp"

ନାମାକାଶ cv {

#ifdef HAVE_OPENCL

ଗଣନା { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
ତାର୍କିକ ocl_sum( ନିବେଶ_Array _src, Scalar & res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum_op, ନିବେଶ_Array _mask = noArray(),
                     ନିବେଶ_Array _src2 = noArray(), ତାର୍କିକ calc2 = ମିଥ୍ୟା, ଅଚଳ Scalar & res2 = Scalar() );
ତାର୍କିକ ocl_minMaxIdx( ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ* minVal, ଦ୍ୱିଗୁଣ* maxVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* minLoc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* maxLoc, ନିବେଶ_Array _mask,
                           ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = -1, ତାର୍କିକ abs_ମୂଲ୍ୟଗୁଡ଼ିକ = ମିଥ୍ୟା, ନିବେଶ_Array _src2 = noArray(), ଦ୍ୱିଗୁଣ * maxVal2 = ଖାଲି);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = m.channels();
    ଅଚଳ T * ଅଚଳ ptr = m.ptr<T>(0);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0, w = m.cols * cn; x < w; )
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ଫେରନ୍ତୁ s;
}

#endif

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*SumFunc)(ଅଚଳ uchar*, ଅଚଳ uchar* mask, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
SumFunc getSumFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

}

#endif // SRC_STAT_HPP
