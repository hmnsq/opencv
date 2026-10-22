// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ಸ್ಥಿರ ಶೂನ್ಯ reshapeTo(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಧ್ರುವ MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    ಯದಿ (_dst.kind() == ಒಳಹರಿವು_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ಯದಿ (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ಯದಿ (dst.data == src.data)
        ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        ಪ್ರತ್ಯಾಗಮ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

ಶೂನ್ಯ squeeze(ಒಳಹರಿವು_Array src, OutputArray dst, ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

ಶೂನ್ಯ unsqueeze(ಒಳಹರಿವು_Array src, OutputArray dst, ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

ಶೂನ್ಯ flatten(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಙ್ಕ startAxis, ಪೂರ್ಣಾಙ್ಕ endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

ಶೂನ್ಯ reshape(ಒಳಹರಿವು_Array src, OutputArray dst, ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& newShape, ತಾರ್ಕಿಕ allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
