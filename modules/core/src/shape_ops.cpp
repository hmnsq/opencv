// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
স্থির শূন্য reshapeTo(প্রবেশ_Array _src, OutputArray _dst, ধ্রুব MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    যদি (_dst.kind() == প্রবেশ_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        যদি (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            প্রত্যাগম;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    যদি (dst.data == src.data)
        প্রত্যাগম;
    যদি (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        প্রত্যাগম;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

শূন্য squeeze(প্রবেশ_Array src, OutputArray dst, ধ্রুব std::vector<পূর্ণাঙ্ক>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

শূন্য unsqueeze(প্রবেশ_Array src, OutputArray dst, ধ্রুব std::vector<পূর্ণাঙ্ক>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

শূন্য flatten(প্রবেশ_Array src, OutputArray dst, পূর্ণাঙ্ক startAxis, পূর্ণাঙ্ক endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

শূন্য reshape(প্রবেশ_Array src, OutputArray dst, ধ্রুব std::vector<পূর্ণাঙ্ক>& newShape, তার্কিক allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
