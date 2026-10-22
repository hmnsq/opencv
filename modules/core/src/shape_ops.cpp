// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

نطاق cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ساكن فراغ reshape_مقصد(درون_داد_Array _src, OutputArray _dst, ثابت MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.مجموع() == src.مجموع());
    إذا (_dst.نوع() == درون_داد_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        إذا (dst.داده && dst.داده == src.داده)
        {
            dst = src.reshape(0, shape);
            أرجع;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    إذا (dst.داده == src.داده)
        أرجع;
    إذا (dst.isContinuous())
    {
        src.copy_مقصد(Mat(src.dims, src.size.p, src.type(), dst.داده));
        أرجع;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_مقصد(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.داده).copy_مقصد(dst);
}

فراغ squeeze(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().squeeze(axes));
}

فراغ unsqueeze(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().unsqueeze(axes));
}

فراغ flatten(درون_داد_Array src, OutputArray dst, صحيح شروع_Axis, صحيح انتها_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().flatten(شروع_Axis, انتها_Axis));
}

فراغ reshape(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحيح>& newShape, منطقي allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_مقصد(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
