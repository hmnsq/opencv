// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

فضاینام cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ایستا تهی reshapeTo(درون_داد_Array _src, OutputArray _dst, ثابت MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    اگر (_dst.kind() == درون_داد_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        اگر (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            بازگشت;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    اگر (dst.data == src.data)
        بازگشت;
    اگر (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        بازگشت;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

تهی squeeze(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحیح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

تهی unsqueeze(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحیح>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

تهی flatten(درون_داد_Array src, OutputArray dst, صحیح startAxis, صحیح endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

تهی reshape(درون_داد_Array src, OutputArray dst, ثابت std::vector<صحیح>& newShape, منطقی allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
