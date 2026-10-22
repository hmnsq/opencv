// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#समावेश "precomp.hpp"

namespace cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
अटल व्योम reshapeTo(निवेश_Array _src, OutputArray _dst, स्थिर MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.योग() == src.योग());
    अगर (_dst.kind() == निवेश_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        अगर (dst.तथ्य && dst.तथ्य == src.तथ्य)
        {
            dst = src.reshape(0, shape);
            वापस;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    अगर (dst.तथ्य == src.तथ्य)
        वापस;
    अगर (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.तथ्य));
        वापस;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.तथ्य).copyTo(dst);
}

व्योम squeeze(निवेश_Array src, OutputArray dst, स्थिर std::vector<पूर्णांक>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

व्योम unsqueeze(निवेश_Array src, OutputArray dst, स्थिर std::vector<पूर्णांक>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

व्योम flatten(निवेश_Array src, OutputArray dst, पूर्णांक startAxis, पूर्णांक endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

व्योम reshape(निवेश_Array src, OutputArray dst, स्थिर std::vector<पूर्णांक>& newShape, bool allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
