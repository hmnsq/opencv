// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"

namespace cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
static void reshapeTo(InputArray _src, OutputArray _dst, const MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    ако (_dst.kind() == _InputArray::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ако (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            върни;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ако (dst.data == src.data)
        върни;
    ако (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        върни;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

void squeeze(InputArray src, OutputArray dst, const std::vector<цяло>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

void unsqueeze(InputArray src, OutputArray dst, const std::vector<цяло>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

void flatten(InputArray src, OutputArray dst, цяло startAxis, цяло endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

void reshape(InputArray src, OutputArray dst, const std::vector<цяло>& newShape, bool allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
