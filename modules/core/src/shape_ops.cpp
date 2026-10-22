// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv {

// In place (dst shares the data of a continuous src) only the header changes; otherwise the
// elements are copied into dst.
സ്ഥിതം ശൂന്യം reshapeTo(നിവേശം_Array _src, OutputArray _dst, സ്ഥിരം MatShape& shape)
{
    Mat src = _src.getMat();
    CV_Assert(shape.total() == src.total());
    എങ്കിൽ (_dst.kind() == നിവേശം_Array_2::MAT && src.isContinuous())
    {
        Mat& dst = _dst.getMatRef();
        എങ്കിൽ (dst.data && dst.data == src.data)
        {
            dst = src.reshape(0, shape);
            തിരികെ;
        }
    }
    _dst.create(shape, src.type());
    Mat dst = _dst.getMat();
    എങ്കിൽ (dst.data == src.data)
        തിരികെ;
    എങ്കിൽ (dst.isContinuous())
    {
        src.copyTo(Mat(src.dims, src.size.p, src.type(), dst.data));
        തിരികെ;
    }
    // create() keeps a preallocated destination of the right shape even when it is strided
    Mat tmp;
    src.copyTo(tmp);
    Mat(shape.dims, shape.p, src.type(), tmp.data).copyTo(dst);
}

ശൂന്യം squeeze(നിവേശം_Array src, OutputArray dst, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().squeeze(axes));
}

ശൂന്യം unsqueeze(നിവേശം_Array src, OutputArray dst, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& axes)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().unsqueeze(axes));
}

ശൂന്യം flatten(നിവേശം_Array src, OutputArray dst, പൂർണ്ണസംഖ്യ startAxis, പൂർണ്ണസംഖ്യ endAxis)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().flatten(startAxis, endAxis));
}

ശൂന്യം reshape(നിവേശം_Array src, OutputArray dst, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& newShape, തർക്കം allowZero)
{
    CV_INSTRUMENT_REGION();
    reshapeTo(src, dst, src.shape().reshape(MatShape(newShape), allowZero));
}

} // namespace cv
