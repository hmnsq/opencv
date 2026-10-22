// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

نطاق cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ساكن فراغ reshapeTo(InputArray _src, OutputArray _dst, ثابت MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    ܐܢ (_dst.kind() == _InputArray::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ܐܢ (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            ܦܢܐ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ܐܢ (dst.data == src.data)
        ܦܢܐ;
    ܐܢ (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        ܦܢܐ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

فراغ squeeze(InputArray src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

فراغ unsqueeze(InputArray src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

فراغ flatten(InputArray src, OutputArray dst, صحيح startAxis, صحيح endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

فراغ reshape(InputArray src, OutputArray dst, ثابت std::vector<صحيح>& newShape, منطقي allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
