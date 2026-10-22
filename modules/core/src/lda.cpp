/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ <map>
#ಅನ್ತರ್ಭಾವ <set>

ನಾಮಾಕಾಶ cv
{

// Removes duplicate elements in a given vector.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp>
ಸಾಲೊಳಗೆ std::vector<_Tp> remove_dups(ಸ್ಥಿರಾಂಕ std::vector<_Tp>& src) {
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು std::ಗಣ<_Tp>::const_iterator const_ಗಣ_Iterator;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು std::vector<_Tp>::const_iterator constVecIterator;
    std::ಗಣ<_Tp> ಗಣ_elems;
    ಗಾಗಿ (constVecIterator it = src.begin(); it != src.end(); ++it)
        ಗಣ_elems.insert(*it);
    std::vector<_Tp> elems;
    ಗಾಗಿ (const_ಗಣ_Iterator it = ಗಣ_elems.begin(); it != ಗಣ_elems.end(); ++it)
        elems.push_back(*it);
    ಹಿಂತಿರುಗಿ elems;
}

ಸ್ಥಿರ Mat argsort(ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ ascending=ನಿಜ)
{
    Mat src = _src.getMat();
    ಆದರೆ (src.ಅಡ್ಡಸಾಲುಗಳು != 1 && src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != 1) {
        String error_ಸಂದೇಶ = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    ಪೂರ್ಣಾಂಕ flags = ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು | (ascending ? ವಿಂಗಡಿಸು_ASCENDING : ವಿಂಗಡಿಸು_DESCENDING);
    Mat sorted_indices;
    ವಿಂಗಡಿಸು_Idx(src.reshape(1,1),sorted_indices,flags);
    ಹಿಂತಿರುಗಿ sorted_indices;
}

ಸ್ಥಿರ Mat as_ಅಡ್ಡಸಾಲು_ಮಾತೃಕೆ(ಒಳಹರಿವು_Array_ದ_Arrays src, ಪೂರ್ಣಾಂಕ rtype, ದ್ವಿಗುಣ alpha=1, ದ್ವಿಗುಣ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ಆದರೆ(src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT && src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
        src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_ಸದಿಶ) {
        String error_ಸಂದೇಶ = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // number of samples
    size_t n = src.ಒಟ್ಟು();
    // return empty matrix if no matrices given
    ಆದರೆ(n == 0)
        ಹಿಂತಿರುಗಿ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).ಒಟ್ಟು();
    // create data matrix
    Mat ದತ್ತಾಂಶ((ಪೂರ್ಣಾಂಕ)n, (ಪೂರ್ಣಾಂಕ)d, rtype);
    // now copy data
    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ಆದರೆ(src.getMat(i).ಒಟ್ಟು() != d) {
            String error_ಸಂದೇಶ = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (ಪೂರ್ಣಾಂಕ)d, (ಪೂರ್ಣಾಂಕ)src.getMat(i).ಒಟ್ಟು());
            CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
        }
        // get a hold of the current row
        Mat xi = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲು(i);
        // make reshape happy by cloning for non-continuous matrices
        ಆದರೆ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_ಗಮ್ಯ(xi, rtype, alpha, beta);
        } ಇಲ್ಲದಿದ್ದರೆ {
            src.getMat(i).clone().reshape(1, 1).convert_ಗಮ್ಯ(xi, rtype, alpha, beta);
        }
    }
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು_ಮಾತೃಕೆ_Columns_By_Indices(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _indices, OutputArray _dst) {
    ಆದರೆ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<ಪೂರ್ಣಾಂಕ> indices = _indices.getMat();
    _dst.create(src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.type());
    Mat dst = _dst.getMat();
    ಗಾಗಿ(size_t idx = 0; idx < indices.size(); idx++) {
        Mat original_ಲಂಬಸಾಲು = src.ಲಂಬಸಾಲು(indices[idx]);
        Mat sorted_ಲಂಬಸಾಲು = dst.ಲಂಬಸಾಲು((ಪೂರ್ಣಾಂಕ)idx);
        original_ಲಂಬಸಾಲು.copy_ಗಮ್ಯ(sorted_ಲಂಬಸಾಲು);
    }
}

ಸ್ಥಿರ Mat ವಿಂಗಡಿಸು_ಮಾತೃಕೆ_Columns_By_Indices(ಒಳಹರಿವು_Array src, ಒಳಹರಿವು_Array indices) {
    Mat dst;
    ವಿಂಗಡಿಸು_ಮಾತೃಕೆ_Columns_By_Indices(src, indices, dst);
    ಹಿಂತಿರುಗಿ dst;
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ತಾರ್ಕಿಕ
is_ಸಮಮಿತ(ಒಳಹರಿವು_Array src) {
    Mat _src = src.getMat();
    ಆದರೆ(_src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != _src.ಅಡ್ಡಸಾಲುಗಳು)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _src.ಅಡ್ಡಸಾಲುಗಳು; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < _src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ಆದರೆ (a != b) {
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ತಾರ್ಕಿಕ
is_ಸಮಮಿತ(ಒಳಹರಿವು_Array src, ದ್ವಿಗುಣ eps) {
    Mat _src = src.getMat();
    ಆದರೆ(_src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != _src.ಅಡ್ಡಸಾಲುಗಳು)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _src.ಅಡ್ಡಸಾಲುಗಳು; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < _src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ಆದರೆ (std::ನಿರಪೇಕ್ಷ(a - b) > eps) {
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ is_ಸಮಮಿತ_2(ಒಳಹರಿವು_Array src, ದ್ವಿಗುಣ eps=1e-16)
{
    Mat m = src.getMat();
    ಆಯ್ಕೆ (m.type()) {
        ಸ್ಥಿತಿ CV_8SC1: ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ಅಕ್ಷರ>(m); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_8UC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(m); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_16SC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ಗಿಡ್ಡ>(m); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_16UC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ>(m); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_32SC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ಪೂರ್ಣಾಂಕ>(m); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_32FC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ದಶಮಾಂಶ>(m, eps); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CV_64FC1:
            ಹಿಂತಿರುಗಿ is_ಸಮಮಿತ<ದ್ವಿಗುಣ>(m, eps); ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(ಒಳಹರಿವು_Array _W, ಒಳಹರಿವು_Array ಸರಾಸರಿ, ಒಳಹರಿವು_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat ಸರಾಸರಿ_2 = ಸರಾಸರಿ.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ಪೂರ್ಣಾಂಕ n = src.ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ d = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    // make sure the data has the correct shape
    ಆದರೆ(W.ಅಡ್ಡಸಾಲುಗಳು != d) {
        String error_ಸಂದೇಶ = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, W.ಅಡ್ಡಸಾಲುಗಳು, W.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // make sure mean is correct if not empty
    ಆದರೆ(!ಸರಾಸರಿ_2.empty() && (ಸರಾಸರಿ_2.ಒಟ್ಟು() != (size_t) d)) {
        String error_ಸಂದೇಶ = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, ಸರಾಸರಿ_2.ಒಟ್ಟು());
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_ಗಮ್ಯ(X, W.type());
    // safe to do, because of above assertion
    ಆದರೆ(!ಸರಾಸರಿ_2.empty()) {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i=0; i<n; i++) {
            Mat r_i = X.ಅಡ್ಡಸಾಲು(i);
            ಕಳೆ(r_i, ಸರಾಸರಿ_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ಹಿಂತಿರುಗಿ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(ಒಳಹರಿವು_Array _W, ಒಳಹರಿವು_Array ಸರಾಸರಿ, ಒಳಹರಿವು_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat ಸರಾಸರಿ_2 = ಸರಾಸರಿ.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ಪೂರ್ಣಾಂಕ n = src.ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ d = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    // make sure the data has the correct shape
    ಆದರೆ(W.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != d) {
        String error_ಸಂದೇಶ = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, W.ಅಡ್ಡಸಾಲುಗಳು, W.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // make sure mean is correct if not empty
    ಆದರೆ(!ಸರಾಸರಿ_2.empty() && (ಸರಾಸರಿ_2.ಒಟ್ಟು() != (size_t) W.ಅಡ್ಡಸಾಲುಗಳು)) {
        String error_ಸಂದೇಶ = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಸರಾಸರಿ_2.ಒಟ್ಟು());
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_ಗಮ್ಯ(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    ಆದರೆ(!ಸರಾಸರಿ_2.empty()) {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i=0; i<n; i++) {
            Mat r_i = X.ಅಡ್ಡಸಾಲು(i);
            ಕೂಡಿಸು(r_i, ಸರಾಸರಿ_2.reshape(1,1), r_i);
        }
    }
    ಹಿಂತಿರುಗಿ X;
}


ವರ್ಗ EigenvalueDecomposition {
ಖಾಸಗಿ:

    // Holds the data dimension.
    ಪೂರ್ಣಾಂಕ n;

    // Pointer to internal memory.
    ದ್ವಿಗುಣ *d, *e, *ort;
    ದ್ವಿಗುಣ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp>
    _Tp *alloc_1d(ಪೂರ್ಣಾಂಕ m) {
        ಹಿಂತಿರುಗಿ ಹೊಸ _Tp[m];
    }

    // Allocates memory.
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp>
    _Tp *alloc_1d(ಪೂರ್ಣಾಂಕ m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m; i++)
            arr[i] = val;
        ಹಿಂತಿರುಗಿ arr;
    }

    // Allocates memory.
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp>
    _Tp **alloc_2d(ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ _n) {
        _Tp **arr = ಹೊಸ _Tp*[m];
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m; i++)
            arr[i] = ಹೊಸ _Tp[_n];
        ಹಿಂತಿರುಗಿ arr;
    }

    // Allocates memory.
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp>
    _Tp **alloc_2d(ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m; i++) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ಹಿಂತಿರುಗಿ arr;
    }

    ಸ್ಥಿರ ಶೂನ್ಯ complex_div(ದ್ವಿಗುಣ xr, ದ್ವಿಗುಣ xi, ದ್ವಿಗುಣ yr, ದ್ವಿಗುಣ yi, ದ್ವಿಗುಣ& cdivr, ದ್ವಿಗುಣ& cdivi) {
        ದ್ವಿಗುಣ r, dv;
        CV_DbgAssert(std::ನಿರಪೇಕ್ಷ(yr) + std::ನಿರಪೇಕ್ಷ(yi) > 0.0);
        ಆದರೆ (std::ನಿರಪೇಕ್ಷ(yr) > std::ನಿರಪೇಕ್ಷ(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ಇಲ್ಲದಿದ್ದರೆ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    ಶೂನ್ಯ hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ max_iters_ಎಣಿಕೆ = 1000 * ಇದು->n;

        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nn = ಇದು->n; CV_Assert(nn > 0);
        ಪೂರ್ಣಾಂಕ n1 = nn - 1;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಮೇಲಿನ = nn - 1;
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ eps = std::numeric_limits<ದ್ವಿಗುಣ>::epsilon();
        ದ್ವಿಗುಣ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ದ್ವಿಗುಣ norm = 0.0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nn; i++) {
#ಆದರೆ 0 // 'if' condition is always false
            ಆದರೆ (i < ಕೆಳಗಿನ || i > ಮೇಲಿನ) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::ನಿರಪೇಕ್ಷ(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        ಪೂರ್ಣಾಂಕ iter = 0;
        ಆಗ (n1 >= ಕೆಳಗಿನ) {

            // Look for single small sub-diagonal element
            ಪೂರ್ಣಾಂಕ l = n1;
            ಆಗ (l > ಕೆಳಗಿನ) {
                ಆದರೆ (norm < FLT_EPSILON) {
                    ನಿಲ್ಲಿಸು;
                }
                ದ್ವಿಗುಣ s = std::ನಿರಪೇಕ್ಷ(H[l - 1][l - 1]) + std::ನಿರಪೇಕ್ಷ(H[l][l]);
                ಆದರೆ (s == 0.0) {
                    s = norm;
                }
                ಆದರೆ (std::ನಿರಪೇಕ್ಷ(H[l][l - 1]) < eps * s) {
                    ನಿಲ್ಲಿಸು;
                }
                l--;
            }

            // Check for convergence
            ಆದರೆ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l == n1 - 1) {
                // Two roots found
                ದ್ವಿಗುಣ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ದ್ವಿಗುಣ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ದ್ವಿಗುಣ q = p * p + w;
                ದ್ವಿಗುಣ z = std::ವರ್ಗಮೂಲ(std::ನಿರಪೇಕ್ಷ(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ದ್ವಿಗುಣ x = H[n1][n1];

                ಆದರೆ (q >= 0) {
                    // Real pair
                    ಆದರೆ (p >= 0) {
                        z = p + z;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ಆದರೆ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ದ್ವಿಗುಣ s = std::ನಿರಪೇಕ್ಷ(x) + std::ನಿರಪೇಕ್ಷ(z);
                    p = x / s;
                    q = z / s;
                    ದ್ವಿಗುಣ r = std::ವರ್ಗಮೂಲ(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ಇಲ್ಲದಿದ್ದರೆ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ಇಲ್ಲದಿದ್ದರೆ {
                // No convergence yet

                // Form shift
                ದ್ವಿಗುಣ x = H[n1][n1];
                ದ್ವಿಗುಣ y = 0.0;
                ದ್ವಿಗುಣ w = 0.0;
                ಆದರೆ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ಆದರೆ (iter == 10) {
                    exshift += x;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಕೆಳಗಿನ; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ದ್ವಿಗುಣ s = std::ನಿರಪೇಕ್ಷ(H[n1][n1 - 1]) + std::ನಿರಪೇಕ್ಷ(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ಆದರೆ (iter == 30) {
                    ದ್ವಿಗುಣ s = (y - x) * 0.5;
                    s = s * s + w;
                    ಆದರೆ (s > 0) {
                        s = std::ವರ್ಗಮೂಲ(s);
                        ಆದರೆ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಕೆಳಗಿನ; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ಆದರೆ (iter > max_iters_ಎಣಿಕೆ)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ದ್ವಿಗುಣ p = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ q = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ r = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                ಪೂರ್ಣಾಂಕ m = n1 - 2;
                ಆಗ (m >= l) {
                    ದ್ವಿಗುಣ z = H[m][m];
                    r = x - z;
                    ದ್ವಿಗುಣ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::ನಿರಪೇಕ್ಷ(p) + std::ನಿರಪೇಕ್ಷ(q) + std::ನಿರಪೇಕ್ಷ(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ಆದರೆ (m == l) {
                        ನಿಲ್ಲಿಸು;
                    }
                    ಆದರೆ (std::ನಿರಪೇಕ್ಷ(H[m][m - 1]) * (std::ನಿರಪೇಕ್ಷ(q) + std::ನಿರಪೇಕ್ಷ(r)) < eps * (std::ನಿರಪೇಕ್ಷ(p)
                                                                                     * (std::ನಿರಪೇಕ್ಷ(H[m - 1][m - 1]) + std::ನಿರಪೇಕ್ಷ(z) + std::ನಿರಪೇಕ್ಷ(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ನಿಲ್ಲಿಸು;
                    }
                    m--;
                }

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ಆದರೆ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = m; k < n1; k++) {

                    ತಾರ್ಕಿಕ notlast = (k != n1 - 1);
                    ಆದರೆ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::ನಿರಪೇಕ್ಷ(p) + std::ನಿರಪೇಕ್ಷ(q) + std::ನಿರಪೇಕ್ಷ(r);
                        ಆದರೆ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ಆದರೆ (x == 0.0) {
                        ನಿಲ್ಲಿಸು;
                    }
                    ದ್ವಿಗುಣ s = std::ವರ್ಗಮೂಲ(p * p + q * q + r * r);
                    ಆದರೆ (p < 0) {
                        s = -s;
                    }
                    ಆದರೆ (s != 0) {
                        ಆದರೆ (k != m) {
                            H[k][k - 1] = -s * x;
                        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ದ್ವಿಗುಣ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ಆದರೆ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ಆದರೆ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ಆದರೆ (notlast) {
                                p = p + z * V[i][k + 2];
                                V[i][k + 2] = V[i][k + 2] - p * r;
                            }
                            V[i][k] = V[i][k] - p;
                            V[i][k + 1] = V[i][k + 1] - p * q;
                        }
                    } // (s != 0)
                } // k loop
            } // check convergence
        } // while (n1 >= low)

        // Backsubstitute to find vectors of upper triangular form

        ಆದರೆ (norm < FLT_EPSILON) {
            ಹಿಂತಿರುಗಿ;
        }

        ಗಾಗಿ (n1 = nn - 1; n1 >= 0; n1--) {
            ದ್ವಿಗುಣ p = d[n1];
            ದ್ವಿಗುಣ q = e[n1];

            ಆದರೆ (q == 0) {
                // Real vector
                ದ್ವಿಗುಣ z = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ s = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                ಪೂರ್ಣಾಂಕ l = n1;
                H[n1][n1] = 1.0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = n1 - 1; i >= 0; i--) {
                    ದ್ವಿಗುಣ w = H[i][i] - p;
                    ದ್ವಿಗುಣ r = 0.0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ಆದರೆ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        l = i;
                        ಆದರೆ (e[i] == 0.0) {
                            ಆದರೆ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ಇಲ್ಲದಿದ್ದರೆ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ಇಲ್ಲದಿದ್ದರೆ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ದ್ವಿಗುಣ x = H[i][i + 1];
                            ದ್ವಿಗುಣ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ದ್ವಿಗುಣ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ಆದರೆ (std::ನಿರಪೇಕ್ಷ(x) > std::ನಿರಪೇಕ್ಷ(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ಇಲ್ಲದಿದ್ದರೆ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ದ್ವಿಗುಣ t = std::ನಿರಪೇಕ್ಷ(H[i][n1]);
                        ಆದರೆ ((eps * t) * t > 1) {
                            ದ್ವಿಗುಣ inv_t = 1.0 / t;
                            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (q < 0) {
                // Complex vector
                ದ್ವಿಗುಣ z = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ r = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ s = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                ಪೂರ್ಣಾಂಕ l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ಆದರೆ (std::ನಿರಪೇಕ್ಷ(H[n1][n1 - 1]) > std::ನಿರಪೇಕ್ಷ(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ಇಲ್ಲದಿದ್ದರೆ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = n1 - 2; i >= 0; i--) {
                    ದ್ವಿಗುಣ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ದ್ವಿಗುಣ w = H[i][i] - p;

                    ಆದರೆ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        l = i;
                        ಆದರೆ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ಇಲ್ಲದಿದ್ದರೆ {
                            // Solve complex equations

                            ದ್ವಿಗುಣ x = H[i][i + 1];
                            ದ್ವಿಗುಣ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ಆದರೆ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::ನಿರಪೇಕ್ಷ(w) + std::ನಿರಪೇಕ್ಷ(q) + std::ನಿರಪೇಕ್ಷ(x)
                                                   + std::ನಿರಪೇಕ್ಷ(y) + std::ನಿರಪೇಕ್ಷ(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ಆದರೆ (std::ನಿರಪೇಕ್ಷ(x) > (std::ನಿರಪೇಕ್ಷ(z) + std::ನಿರಪೇಕ್ಷ(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ಇಲ್ಲದಿದ್ದರೆ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ದ್ವಿಗುಣ t = std::max(std::ನಿರಪೇಕ್ಷ(H[i][n1 - 1]), std::ನಿರಪೇಕ್ಷ(H[i][n1]));
                        ಆದರೆ ((eps * t) * t > 1) {
                            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ಆದರೆ 0 // 'if' condition is always false
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nn; i++) {
            ಆದರೆ (i < ಕೆಳಗಿನ || i > ಮೇಲಿನ) {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

        // Back transformation to get eigenvectors of original matrix

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = nn - 1; j >= ಕೆಳಗಿನ; j--) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                ದ್ವಿಗುಣ z = 0.0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = ಕೆಳಗಿನ; k <= std::min(j, ಮೇಲಿನ); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    ಶೂನ್ಯ orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ = 0;
        ಪೂರ್ಣಾಂಕ ಮೇಲಿನ = n - 1;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ m = ಕೆಳಗಿನ + 1; m < ಮೇಲಿನ; m++) {

            // Scale column.

            ದ್ವಿಗುಣ scale = 0.0;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m; i <= ಮೇಲಿನ; i++) {
                scale = scale + std::ನಿರಪೇಕ್ಷ(H[i][m - 1]);
            }
            ಆದರೆ (scale != 0.0) {

                // Compute Householder transformation.

                ದ್ವಿಗುಣ h = 0.0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಮೇಲಿನ; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ದ್ವಿಗುಣ g = std::ವರ್ಗಮೂಲ(h);
                ಆದರೆ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = m; j < n; j++) {
                    ದ್ವಿಗುಣ f = 0.0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಮೇಲಿನ; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m; i <= ಮೇಲಿನ; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <= ಮೇಲಿನ; i++) {
                    ದ್ವಿಗುಣ f = 0.0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = ಮೇಲಿನ; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = m; j <= ಮೇಲಿನ; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ m = ಮೇಲಿನ - 1; m > ಕೆಳಗಿನ; m--) {
            ಆದರೆ (H[m][m - 1] != 0.0) {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m + 1; i <= ಮೇಲಿನ; i++) {
                    ort[i] = H[i][m - 1];
                }
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = m; j <= ಮೇಲಿನ; j++) {
                    ದ್ವಿಗುಣ g = 0.0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m; i <= ಮೇಲಿನ; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = m; i <= ಮೇಲಿನ; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    ಶೂನ್ಯ release() {
        // releases the working data
        ಅಳಿಸು[] d; d = ಖಾಲಿ;
        ಅಳಿಸು[] e; e = ಖಾಲಿ;
        ಅಳಿಸು[] ort; ort = ಖಾಲಿ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ಆದರೆ (H) ಅಳಿಸು[] H[i];
            ಆದರೆ (V) ಅಳಿಸು[] V[i];
        }
        ಅಳಿಸು[] H; H = ಖಾಲಿ;
        ಅಳಿಸು[] V; V = ಖಾಲಿ;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    ಶೂನ್ಯ compute() {
        // Allocate memory for the working data.
        V = alloc_2d<ದ್ವಿಗುಣ> (n, n, 0.0);
        d = alloc_1d<ದ್ವಿಗುಣ> (n);
        e = alloc_1d<ದ್ವಿಗುಣ> (n);
        ort = alloc_1d<ದ್ವಿಗುಣ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
                _eigenvalues.at<ದ್ವಿಗುಣ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n; j++)
                    _eigenvectors.at<ದ್ವಿಗುಣ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

ಸಾರ್ವಜನಿಕ:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ಖಾಲಿ), e(ಖಾಲಿ), ort(ಖಾಲಿ),
        V(ಖಾಲಿ), H(ಖಾಲಿ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    ಶೂನ್ಯ compute(ಒಳಹರಿವು_Array src, ತಾರ್ಕಿಕ fallback_ಸಮಮಿತ = ನಿಜ)
    {
        CV_INSTRUMENT_REGION();

        ಆದರೆ(fallback_ಸಮಮಿತ && is_ಸಮಮಿತ_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ಇಲ್ಲದಿದ್ದರೆ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_ಗಮ್ಯ(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ಇದು->n = tmp.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            // Allocate the matrix data to work on.
            ಇದು->H = alloc_2d<ದ್ವಿಗುಣ> (n, n);
            // Now safely copy the data.
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < tmp.ಅಡ್ಡಸಾಲುಗಳು; i++) {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < tmp.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++) {
                    ಇದು->H[i][j] = tmp.at<ದ್ವಿಗುಣ>(i, j);
                }
            }
            // Deallocates the temporary matrix before computing.
            tmp.release();
            // Performs the eigenvalue decomposition of H.
            compute();
        }
    }

    ~EigenvalueDecomposition() { release(); }

    // Returns the eigenvalues of the Eigenvalue Decomposition.
    Mat eigenvalues() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ _eigenvectors; }
};

ಶೂನ್ಯ eigen_Non_ಸಮಮಿತ(ಒಳಹರಿವು_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();
    ಪೂರ್ಣಾಂಕ n = src.ಅಡ್ಡಸಾಲುಗಳು;

    CV_Assert(src.ಅಡ್ಡಸಾಲುಗಳು == src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ಆದರೆ (type == CV_32F)
        src.convert_ಗಮ್ಯ(src64f, CV_32FC1);
    ಇಲ್ಲದಿದ್ದರೆ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ಸುಳ್ಳು);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ದ್ವಿಗುಣ> eigenvalues64f;
    eigensystem.eigenvalues().copy_ಗಮ್ಯ(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<ಪೂರ್ಣಾಂಕ> ವಿಂಗಡಿಸು_indexes(n);
    cv::ವಿಂಗಡಿಸು_Idx(eigenvalues64f, ವಿಂಗಡಿಸು_indexes, ವಿಂಗಡಿಸು_EVERY_ಅಡ್ಡಸಾಲು | ವಿಂಗಡಿಸು_DESCENDING);

    std::vector<ದ್ವಿಗುಣ> sorted_eigenvalues64f(n);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[ವಿಂಗಡಿಸು_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_ಗಮ್ಯ(_evals, type);

    ಆದರೆ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.ಅಡ್ಡಸಾಲುಗಳು == n);
        CV_Assert(eigenvectors64f.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == n);
        Mat_<ದ್ವಿಗುಣ> sorted_eigenvectors64f(n, n, CV_64FC1);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
        {
            ದ್ವಿಗುಣ* pDst = sorted_eigenvectors64f.ptr<ದ್ವಿಗುಣ>(i);
            ದ್ವಿಗುಣ* pSrc = eigenvectors64f.ptr<ದ್ವಿಗುಣ>(ವಿಂಗಡಿಸು_indexes[i]);
            CV_Assert(pSrc != ಖಾಲಿ);
            ಸ್ಮರಣೆ_ನಕಲು(pDst, pSrc, n * ಗಾತ್ರ(ದ್ವಿಗುಣ));
        }
        sorted_eigenvectors64f.convert_ಗಮ್ಯ(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(ಪೂರ್ಣಾಂಕ ಅಂಕೆ_components) : ಅಂಕೆ_components_2(ಅಂಕೆ_components) { }

LDA::LDA(ಒಳಹರಿವು_Array_ದ_Arrays src, ಒಳಹರಿವು_Array labels, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_components) : ಅಂಕೆ_components_2(ಅಂಕೆ_components)
{
    ಇದು->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

ಶೂನ್ಯ LDA::save(ಸ್ಥಿರಾಂಕ String& filename) ಸ್ಥಿರಾಂಕ
{
    FileStorage fs(filename, FileStorage::WRITE);
    ಆದರೆ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ಇದು->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
ಶೂನ್ಯ LDA::load(ಸ್ಥಿರಾಂಕ String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ಆದರೆ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ಇದು->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
ಶೂನ್ಯ LDA::save(FileStorage& fs) ಸ್ಥಿರಾಂಕ {
    // write matrices
    fs << "num_components" << ಅಂಕೆ_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
ಶೂನ್ಯ LDA::load(ಸ್ಥಿರಾಂಕ FileStorage& fs) {
    //read matrices
    fs["num_components"] >> ಅಂಕೆ_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

ಶೂನ್ಯ LDA::lda(ಒಳಹರಿವು_Array_ದ_Arrays _src, ಒಳಹರಿವು_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<ಪೂರ್ಣಾಂಕ> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        ಗಾಗಿ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < tmp.ಒಟ್ಟು(); i++) {
            labels.push_back(tmp.at<ಪೂರ್ಣಾಂಕ>(i));
        }
    }
    // turn into row sampled matrix
    Mat ದತ್ತಾಂಶ;
    // ensure working matrix is double precision
    src.convert_ಗಮ್ಯ(ದತ್ತಾಂಶ, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<ಪೂರ್ಣಾಂಕ> mapped_labels(labels.size());
    std::vector<ಪೂರ್ಣಾಂಕ> num2label = remove_dups(labels);
    std::map<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ> label2num;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)num2label.size(); i++)
        label2num[num2label[i]] = i;
    ಗಾಗಿ (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    ಪೂರ್ಣಾಂಕ N = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ D = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    // number of unique labels
    ಪೂರ್ಣಾಂಕ C = (ಪೂರ್ಣಾಂಕ)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ಆದರೆ(C == 1) {
        String error_ಸಂದೇಶ = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // throw error if less labels, than samples
    ಆದರೆ (labels.size() != static_cast<size_t>(N)) {
        String error_ಸಂದೇಶ = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
    }
    // warn if within-classes scatter matrix becomes singular
    ಆದರೆ (N < D) {
        std::ಹೊರಗೆ << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    // clip number of components to be a valid number
    ಆದರೆ ((ಅಂಕೆ_components_2 <= 0) || (ಅಂಕೆ_components_2 >= C)) {
        ಅಂಕೆ_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat ಸರಾಸರಿ_ಒಟ್ಟು = Mat::zeros(1, D, ದತ್ತಾಂಶ.type());
    // holds the mean for each class
    std::vector<Mat> ಸರಾಸರಿ_Class(C);
    std::vector<ಪೂರ್ಣಾಂಕ> ಅಂಕೆ_Class(C);
    // initialize
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < C; i++) {
        ಅಂಕೆ_Class[i] = 0;
        ಸರಾಸರಿ_Class[i] = Mat::zeros(1, D, ದತ್ತಾಂಶ.type()); //! Dx1 image vector
    }
    // calculate sums
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++) {
        Mat instance = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲು(i);
        ಪೂರ್ಣಾಂಕ classIdx = mapped_labels[i];
        ಕೂಡಿಸು(ಸರಾಸರಿ_ಒಟ್ಟು, instance, ಸರಾಸರಿ_ಒಟ್ಟು);
        ಕೂಡಿಸು(ಸರಾಸರಿ_Class[classIdx], instance, ಸರಾಸರಿ_Class[classIdx]);
        ಅಂಕೆ_Class[classIdx]++;
    }
    // calculate total mean
    ಸರಾಸರಿ_ಒಟ್ಟು.convert_ಗಮ್ಯ(ಸರಾಸರಿ_ಒಟ್ಟು, ಸರಾಸರಿ_ಒಟ್ಟು.type(), 1.0 / static_cast<ದ್ವಿಗುಣ> (N));
    // calculate class means
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < C; i++) {
        ಸರಾಸರಿ_Class[i].convert_ಗಮ್ಯ(ಸರಾಸರಿ_Class[i], ಸರಾಸರಿ_Class[i].type(), 1.0 / static_cast<ದ್ವಿಗುಣ> (ಅಂಕೆ_Class[i]));
    }
    // subtract class means
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++) {
        ಪೂರ್ಣಾಂಕ classIdx = mapped_labels[i];
        Mat instance = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲು(i);
        ಕಳೆ(instance, ಸರಾಸರಿ_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, ದತ್ತಾಂಶ.type());
    mulTransposed(ದತ್ತಾಂಶ, Sw, ನಿಜ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, ದತ್ತಾಂಶ.type());
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < C; i++) {
        Mat tmp;
        ಕಳೆ(ಸರಾಸರಿ_Class[i], ಸರಾಸರಿ_ಒಟ್ಟು, tmp);
        mulTransposed(tmp, tmp, ನಿಜ);
        ಕೂಡಿಸು(Sb, tmp, Sb);
    }
    // invert Sw
    Mat Swi = Sw.inv();
    // M = inv(Sw)*Sb
    Mat M;
    gemm(Swi, Sb, 1.0, Mat(), 0.0, M);
    EigenvalueDecomposition es;
    es.compute(M);
    _eigenvalues = es.eigenvalues();
    _eigenvectors = es.eigenvectors();
    // reshape eigenvalues, so they are stored by column
    _eigenvalues = _eigenvalues.reshape(1, 1);
    // get sorted indices descending by their eigenvalue
    std::vector<ಪೂರ್ಣಾಂಕ> sorted_indices = argsort(_eigenvalues, ಸುಳ್ಳು);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = ವಿಂಗಡಿಸು_ಮಾತೃಕೆ_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = ವಿಂಗಡಿಸು_ಮಾತೃಕೆ_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, ಅಂಕೆ_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, ಅಂಕೆ_components_2));
}

ಶೂನ್ಯ LDA::compute(ಒಳಹರಿವು_Array_ದ_Arrays _src, ಒಳಹರಿವು_Array _lbls) {
    ಆಯ್ಕೆ(_src.ಬಗೆ()) {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT:
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT:
        lda(as_ಅಡ್ಡಸಾಲು_ಮಾತೃಕೆ(_src, CV_64FC1), _lbls);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        String error_ಸಂದೇಶ= format("InputArray Datatype %d is not supported.", _src.ಬಗೆ());
        CV_Error(Error::StsBadArg, error_ಸಂದೇಶ);
        ನಿಲ್ಲಿಸು;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(ಒಳಹರಿವು_Array src) {
   ಹಿಂತಿರುಗಿ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(ಒಳಹರಿವು_Array src) {
   ಹಿಂತಿರುಗಿ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
