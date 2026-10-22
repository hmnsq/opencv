// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

نطاق cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ساكن فراغ reshape_منزل(داخلہ_Array _src, OutputArray _dst, ثابت MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.کل() == src.کل());
    إذا (_dst.قسم() == داخلہ_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        إذا (dst.انکڑے && dst.انکڑے == src.انکڑے)
        {
            dst = src.reshape(0, shape);
            أرجع;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    إذا (dst.انکڑے == src.انکڑے)
        أرجع;
    إذا (dst.isContinuous())
    {
        src.copy_منزل(Mat(src.dims, src.size.p, src.type(), dst.انکڑے));
        أرجع;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_منزل(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.انکڑے).copy_منزل(dst);
}

فراغ squeeze(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_منزل(src, dst, src.shape().squeeze(axes));
}

فراغ unsqueeze(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحيح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_منزل(src, dst, src.shape().unsqueeze(axes));
}

فراغ flatten(داخلہ_Array src, OutputArray dst, صحيح شروع_Axis, صحيح سرا_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_منزل(src, dst, src.shape().flatten(شروع_Axis, سرا_Axis));
}

فراغ reshape(داخلہ_Array src, OutputArray dst, ثابت std::vector<صحيح>& newShape, منطقي allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_منزل(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
