// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
સ્થિર શૂન્ય reshapeTo(આગત_Array _src, OutputArray _dst, અચલ MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    જો (_dst.kind() == આગત_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        જો (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            પાછા;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    જો (dst.data == src.data)
        પાછા;
    જો (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        પાછા;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

શૂન્ય squeeze(આગત_Array src, OutputArray dst, અચલ std::vector<પૂર્ણાંક>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

શૂન્ય unsqueeze(આગત_Array src, OutputArray dst, અચલ std::vector<પૂર્ણાંક>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

શૂન્ય flatten(આગત_Array src, OutputArray dst, પૂર્ણાંક startAxis, પૂર્ણાંક endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

શૂન્ય reshape(આગત_Array src, OutputArray dst, અચલ std::vector<પૂર્ણાંક>& newShape, તાર્કિક allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
