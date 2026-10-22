// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ਸਥਿਰ ਖਾਲੀ reshape_ਮੰਜ਼ਿਲ(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਅਚਲ MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.ਕੁੱਲ() == src.ਕੁੱਲ());
    ਜੇ (_dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ਜੇ (dst.ਅੰਕੜੇ && dst.ਅੰਕੜੇ == src.ਅੰਕੜੇ)
        {
            dst = src.reshape(0, shape);
            ਵਾਪਸ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ਜੇ (dst.ਅੰਕੜੇ == src.ਅੰਕੜੇ)
        ਵਾਪਸ;
    ਜੇ (dst.isContinuous())
    {
        src.copy_ਮੰਜ਼ਿਲ(Mat(src.dims, src.size.p, src.type(), dst.ਅੰਕੜੇ));
        ਵਾਪਸ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_ਮੰਜ਼ਿਲ(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.ਅੰਕੜੇ).copy_ਮੰਜ਼ਿਲ(dst);
}

ਖਾਲੀ squeeze(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਅਚਲ std::vector<ਪੂਰਨ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_ਮੰਜ਼ਿਲ(src, dst, src.shape().squeeze(axes));
}

ਖਾਲੀ unsqueeze(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਅਚਲ std::vector<ਪੂਰਨ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_ਮੰਜ਼ਿਲ(src, dst, src.shape().unsqueeze(axes));
}

ਖਾਲੀ flatten(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਪੂਰਨ ਸ਼ੁਰੂ_Axis, ਪੂਰਨ ਸਿਰਾ_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_ਮੰਜ਼ਿਲ(src, dst, src.shape().flatten(ਸ਼ੁਰੂ_Axis, ਸਿਰਾ_Axis));
}

ਖਾਲੀ reshape(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਅਚਲ std::vector<ਪੂਰਨ>& newShape, ਤਾਰਕਿਕ allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_ਮੰਜ਼ਿਲ(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
