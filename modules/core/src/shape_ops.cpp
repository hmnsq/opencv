// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
స్థిర శూన్య reshape_గమ్యం(ఆదానం_Array _src, OutputArray _dst, స్థిరరాశి MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.మొత్తం() == src.మొత్తం());
    ఒకవేళ (_dst.రకం() == ఆదానం_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ఒకవేళ (dst.దత్తాంశం && dst.దత్తాంశం == src.దత్తాంశం)
        {
            dst = src.reshape(0, shape);
            తిరిగి;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ఒకవేళ (dst.దత్తాంశం == src.దత్తాంశం)
        తిరిగి;
    ఒకవేళ (dst.isContinuous())
    {
        src.copy_గమ్యం(Mat(src.dims, src.size.p, src.type(), dst.దత్తాంశం));
        తిరిగి;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_గమ్యం(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.దత్తాంశం).copy_గమ్యం(dst);
}

శూన్య squeeze(ఆదానం_Array src, OutputArray dst, స్థిరరాశి std::vector<పూర్ణాంకం>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_గమ్యం(src, dst, src.shape().squeeze(axes));
}

శూన్య unsqueeze(ఆదానం_Array src, OutputArray dst, స్థిరరాశి std::vector<పూర్ణాంకం>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_గమ్యం(src, dst, src.shape().unsqueeze(axes));
}

శూన్య flatten(ఆదానం_Array src, OutputArray dst, పూర్ణాంకం ప్రారంభం_Axis, పూర్ణాంకం చివర_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_గమ్యం(src, dst, src.shape().flatten(ప్రారంభం_Axis, చివర_Axis));
}

శూన్య reshape(ఆదానం_Array src, OutputArray dst, స్థిరరాశి std::vector<పూర్ణాంకం>& newShape, తార్కిక allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_గమ్యం(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
