// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अगर_अघोषित OPENCV_CORE_SRC_UMATRIX_HPP
#घोषणा OPENCV_CORE_SRC_UMATRIX_HPP

namespace cv {

काष्ठा CV_EXPORTS UMat_तथ्य_Auto_Lock
{
    explicit UMat_तथ्य_Auto_Lock(UMat_तथ्य* u);
    UMat_तथ्य_Auto_Lock(UMat_तथ्य* u1, UMat_तथ्य* u2);
    ~UMat_तथ्य_Auto_Lock();
    UMat_तथ्य* u1;
    UMat_तथ्य* u2;
};

}

#पूर्ण_अगर // OPENCV_CORE_SRC_UMATRIX_HPP
