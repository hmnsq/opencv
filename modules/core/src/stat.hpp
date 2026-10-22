// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#నిర్వచితం_కాకపోతే SRC_STAT_HPP
#నిర్వచించు SRC_STAT_HPP

#అన్తర్భావ "opencv2/core/mat.hpp"

నామాకాశ cv {

#నిర్వచితమైతే HAVE_OPENCL

గణన { OCL_OP_కూడిక = 0, OCL_OP_కూడిక_ABS =  1, OCL_OP_కూడిక_SQR = 2 };
తార్కిక ocl_కూడిక( ఆదానం_Array _src, Scalar & res, పూర్ణాంకం కూడిక_op, ఆదానం_Array ముసుగు_2 = noArray(),
                     ఆదానం_Array _src2 = noArray(), తార్కిక calc2 = అబద్ధం, స్థిరరాశి Scalar & res2 = Scalar() );
తార్కిక ocl_minMaxIdx( ఆదానం_Array _src, ద్విగుణ* minVal, ద్విగుణ* maxVal, పూర్ణాంకం* minLoc, పూర్ణాంకం* maxLoc, ఆదానం_Array ముసుగు_2,
                           పూర్ణాంకం ddepth = -1, తార్కిక abs_విలువలు = అబద్ధం, ఆదానం_Array _src2 = noArray(), ద్విగుణ * maxVal2 = ఖాళీ);

మూస <రకం_పేరు T> Scalar ocl_part_కూడిక(Mat m)
{
    CV_Assert(m.అడ్డువరుసలు == 1);

    Scalar s = Scalar::all(0);
    పూర్ణాంకం cn = m.channels();
    స్థిరరాశి T * స్థిరరాశి ptr = m.ptr<T>(0);

    కొరకు (పూర్ణాంకం x = 0, w = m.నిలువుల_సంఖ్య * cn; x < w; )
        కొరకు (పూర్ణాంకం c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    తిరిగి s;
}

#అయితే_ముగింపు

రకనిర్వచనం పూర్ణాంకం (*కూడిక_Func)(స్థిరరాశి uchar*, స్థిరరాశి uchar* ముసుగు, uchar*, పూర్ణాంకం, పూర్ణాంకం);
కూడిక_Func get_కూడిక_Func(పూర్ణాంకం లోతు);

}

#అయితే_ముగింపు // SRC_STAT_HPP
