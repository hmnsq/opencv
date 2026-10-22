// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ସ୍ଥିର ଶୂନ୍ୟ reshapeTo(ନିବେଶ_Array _src, OutputArray _dst, ଅଚଳ MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    ଯଦି (_dst.kind() == ନିବେଶ_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ଯଦି (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            ଫେରନ୍ତୁ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ଯଦି (dst.data == src.data)
        ଫେରନ୍ତୁ;
    ଯଦି (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        ଫେରନ୍ତୁ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

ଶୂନ୍ୟ squeeze(ନିବେଶ_Array src, OutputArray dst, ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

ଶୂନ୍ୟ unsqueeze(ନିବେଶ_Array src, OutputArray dst, ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

ଶୂନ୍ୟ flatten(ନିବେଶ_Array src, OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା startAxis, ପୂର୍ଣ୍ଣସଂଖ୍ୟା endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

ଶୂନ୍ୟ reshape(ନିବେଶ_Array src, OutputArray dst, ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& newShape, ତାର୍କିକ allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
