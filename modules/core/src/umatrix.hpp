// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_SRC_UMATRIX_HPP
#define OPENCV_CORE_SRC_UMATRIX_HPP

নামাকাশ cv {

কাঠামো CV_EXPORTS UMat_তথ্য_Auto_Lock
{
    explicit UMat_তথ্য_Auto_Lock(UMat_তথ্য* u);
    UMat_তথ্য_Auto_Lock(UMat_তথ্য* u1, UMat_তথ্য* u2);
    ~UMat_তথ্য_Auto_Lock();
    UMat_তথ্য* u1;
    UMat_তথ্য* u2;
};

}

#endif // OPENCV_CORE_SRC_UMATRIX_HPP
