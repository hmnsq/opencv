// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#અન્તર્ભાવ "opencv2/core/mat.hpp"

નામાકાશ cv {

#ifdef HAVE_OPENCL

ગણના { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
તાર્કિક ocl_sum( આગત_Array _src, Scalar & res, પૂર્ણાંક sum_op, આગત_Array _mask = noArray(),
                     આગત_Array _src2 = noArray(), તાર્કિક calc2 = ખોટું, અચલ Scalar & res2 = Scalar() );
તાર્કિક ocl_minMaxIdx( આગત_Array _src, દ્વિગુણ* minVal, દ્વિગુણ* maxVal, પૂર્ણાંક* minLoc, પૂર્ણાંક* maxLoc, આગત_Array _mask,
                           પૂર્ણાંક ddepth = -1, તાર્કિક abs_કિંમતો = ખોટું, આગત_Array _src2 = noArray(), દ્વિગુણ * maxVal2 = ખાલી);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    પૂર્ણાંક cn = m.channels();
    અચલ T * અચલ ptr = m.ptr<T>(0);

    માટે (પૂર્ણાંક x = 0, w = m.cols * cn; x < w; )
        માટે (પૂર્ણાંક c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    પાછા s;
}

#endif

પ્રકારવ્યાખ્યા પૂર્ણાંક (*SumFunc)(અચલ uchar*, અચલ uchar* mask, uchar*, પૂર્ણાંક, પૂર્ણાંક);
SumFunc getSumFunc(પૂર્ણાંક depth);

}

#endif // SRC_STAT_HPP
