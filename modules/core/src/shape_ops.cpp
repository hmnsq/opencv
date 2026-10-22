// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

نامخلا cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ساکن خالی reshape_مقصد(داخلہ_Array _src, OutputArray _dst, ثابت MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.کل() == src.کل());
    اگر (_dst.قسم() == داخلہ_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        اگر (dst.ڈیٹا && dst.ڈیٹا == src.ڈیٹا)
        {
            dst = src.reshape(0, shape);
            واپس;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    اگر (dst.ڈیٹا == src.ڈیٹا)
        واپس;
    اگر (dst.isContinuous())
    {
        src.copy_مقصد(Mat(src.dims, src.size.p, src.type(), dst.ڈیٹا));
        واپس;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_مقصد(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.ڈیٹا).copy_مقصد(dst);
}

خالی squeeze(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحیح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().squeeze(axes));
}

خالی unsqueeze(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحیح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().unsqueeze(axes));
}

خالی flatten(داخلہ_Array src, OutputArray dst, صحیح آغاز_Axis, صحیح انتہا_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().flatten(آغاز_Axis, انتہا_Axis));
}

خالی reshape(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحیح>& newShape, منطقی allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
