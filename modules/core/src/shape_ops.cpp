// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
ಸ್ಥಿರ ಶೂನ್ಯ reshape_ಗಮ್ಯ(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಸ್ಥಿರಾಂಕ MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.ಒಟ್ಟು() == src.ಒಟ್ಟು());
    ಆದರೆ (_dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        ಆದರೆ (dst.ದತ್ತಾಂಶ && dst.ದತ್ತಾಂಶ == src.ದತ್ತಾಂಶ)
        {
            dst = src.reshape(0, shape);
            ಹಿಂತಿರುಗಿ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    ಆದರೆ (dst.ದತ್ತಾಂಶ == src.ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ;
    ಆದರೆ (dst.isContinuous())
    {
        src.copy_ಗಮ್ಯ(Mat(src.dims, src.size.p, src.type(), dst.ದತ್ತಾಂಶ));
        ಹಿಂತಿರುಗಿ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copy_ಗಮ್ಯ(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.ದತ್ತಾಂಶ).copy_ಗಮ್ಯ(dst);
}

ಶೂನ್ಯ squeeze(ಒಳಹರಿವು_Array src, OutputArray dst, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_ಗಮ್ಯ(src, dst, src.shape().squeeze(axes));
}

ಶೂನ್ಯ unsqueeze(ಒಳಹರಿವು_Array src, OutputArray dst, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshape_ಗಮ್ಯ(src, dst, src.shape().unsqueeze(axes));
}

ಶೂನ್ಯ flatten(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಂಕ ಆರಂಭ_Axis, ಪೂರ್ಣಾಂಕ ಕೊನೆ_Axis)
{
    CV_INSTRUMENT_REGION();
    reshape_ಗಮ್ಯ(src, dst, src.shape().flatten(ಆರಂಭ_Axis, ಕೊನೆ_Axis));
}

ಶೂನ್ಯ reshape(ಒಳಹರಿವು_Array src, OutputArray dst, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& newShape, ತಾರ್ಕಿಕ allowZero)
{
    CV_INSTRUMENT_REGION();
    reshape_ಗಮ್ಯ(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
