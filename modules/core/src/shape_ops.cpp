// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
स्थिर शून्य reshape_गन्तव्यम्(निवेशः_Array _src, OutputArray _dst, ध्रुव MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.योग() == src.योग());
    यदि (_dst.विधा() == निवेशः_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        यदि (dst.दत्त && dst.दत्त == src.दत्त)
        {
            dst = src.reshape(0, shape);
            प्रत्यागम;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    यदि (dst.दत्त == src.दत्त)
        प्रत्यागम;
    यदि (dst.isContinuous())
    {
        src.copy_गन्तव्यम्(Mat(src.dims, src.size.p, src.type(), dst.दत्त));
        प्रत्यागम;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_गन्तव्यम्(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.दत्त).copy_गन्तव्यम्(dst);
}

शून्य squeeze(निवेशः_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_गन्तव्यम्(src, dst, src.shape().squeeze(axes));
}

शून्य unsqueeze(निवेशः_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_गन्तव्यम्(src, dst, src.shape().unsqueeze(axes));
}

शून्य flatten(निवेशः_Array src, OutputArray dst, पूर्णाङ्क startAxis, पूर्णाङ्क अन्तः_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_गन्तव्यम्(src, dst, src.shape().flatten(startAxis, अन्तः_Axis));
}

शून्य reshape(निवेशः_Array src, OutputArray dst, ध्रुव std::vector<पूर्णाङ्क>& newShape, तार्किक allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_गन्तव्यम्(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
