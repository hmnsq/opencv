// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
स्थिर शून्य reshapeTo(आदान_Array _src, OutputArray _dst, ध्रुव MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    यदि (_dst.kind() == आदान_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        यदि (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            प्रत्यागम;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    यदि (dst.data == src.data)
        प्रत्यागम;
    यदि (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        प्रत्यागम;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

शून्य squeeze(आदान_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

शून्य unsqueeze(आदान_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

शून्य flatten(आदान_Array src, OutputArray dst, पूर्णाङ्क startAxis, पूर्णाङ्क endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

शून्य reshape(आदान_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& newShape, तार्किक allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
