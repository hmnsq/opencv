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
template<typename _Tp>
ಅನ್ತಃಸ್ಥ std::vector<_Tp> remove_dups(ಧ್ರುವ std::vector<_Tp>& src) {
    ಪ್ರಕಾರನಿರ್ದೇಶ typename std::set<_Tp>::const_iterator constSetIterator;
    ಪ್ರಕಾರನಿರ್ದೇಶ typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    ಪ್ರತಿ (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    ಪ್ರತಿ (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    ಪ್ರತ್ಯಾಗಮ elems;
}

ಸ್ಥಿರ Mat argsort(ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ ascending=ಸತ್ಯ)
{
    Mat src = _src.getMat();
    ಯದಿ (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    ಪೂರ್ಣಾಙ್ಕ flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    ಪ್ರತ್ಯಾಗಮ sorted_indices;
}

ಸ್ಥಿರ Mat asRowMatrix(ಒಳಹರಿವು_Array_Of_Arrays src, ಪೂರ್ಣಾಙ್ಕ rtype, ದ್ವಿಗುಣ alpha=1, ದ್ವಿಗುಣ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ಯದಿ(src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT && src.kind() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
        src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    ಯದಿ(n == 0)
        ಪ್ರತ್ಯಾಗಮ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((ಪೂರ್ಣಾಙ್ಕ)n, (ಪೂರ್ಣಾಙ್ಕ)d, rtype);
    // now copy data
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ಯದಿ(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (ಪೂರ್ಣಾಙ್ಕ)d, (ಪೂರ್ಣಾಙ್ಕ)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        ಯದಿ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } ಅನ್ಯಥಾ {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಶೂನ್ಯ sortMatrixColumnsByIndices(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _indices, OutputArray _dst) {
    ಯದಿ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<ಪೂರ್ಣಾಙ್ಕ> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    ಪ್ರತಿ(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((ಪೂರ್ಣಾಙ್ಕ)idx);
        originalCol.copyTo(sortedCol);
    }
}

ಸ್ಥಿರ Mat sortMatrixColumnsByIndices(ಒಳಹರಿವು_Array src, ಒಳಹರಿವು_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    ಪ್ರತ್ಯಾಗಮ dst;
}


template<typename _Tp> ಸ್ಥಿರ ತಾರ್ಕಿಕ
isSymmetric_(ಒಳಹರಿವು_Array src) {
    Mat _src = src.getMat();
    ಯದಿ(_src.cols != _src.rows)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _src.rows; i++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ಯದಿ (a != b) {
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template<typename _Tp> ಸ್ಥಿರ ತಾರ್ಕಿಕ
isSymmetric_(ಒಳಹರಿವು_Array src, ದ್ವಿಗುಣ eps) {
    Mat _src = src.getMat();
    ಯದಿ(_src.cols != _src.rows)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _src.rows; i++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ಯದಿ (std::abs(a - b) > eps) {
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ isSymmetric(ಒಳಹರಿವು_Array src, ದ್ವಿಗುಣ eps=1e-16)
{
    Mat m = src.getMat();
    ವಿಕಲ್ಪ (m.type()) {
        ಪಕ್ಷ CV_8SC1: ಪ್ರತ್ಯಾಗಮ isSymmetric_<ವರ್ಣ>(m); ಭಙ್ಗ;
        ಪಕ್ಷ CV_8UC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<unsigned ವರ್ಣ>(m); ಭಙ್ಗ;
        ಪಕ್ಷ CV_16SC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<short>(m); ಭಙ್ಗ;
        ಪಕ್ಷ CV_16UC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<unsigned short>(m); ಭಙ್ಗ;
        ಪಕ್ಷ CV_32SC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<ಪೂರ್ಣಾಙ್ಕ>(m); ಭಙ್ಗ;
        ಪಕ್ಷ CV_32FC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<ಭಿನ್ನ>(m, eps); ಭಙ್ಗ;
        ಪಕ್ಷ CV_64FC1:
            ಪ್ರತ್ಯಾಗಮ isSymmetric_<ದ್ವಿಗುಣ>(m, eps); ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(ಒಳಹರಿವು_Array _W, ಒಳಹರಿವು_Array _mean, ಒಳಹರಿವು_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ಪೂರ್ಣಾಙ್ಕ n = src.rows;
    ಪೂರ್ಣಾಙ್ಕ d = src.cols;
    // make sure the data has the correct shape
    ಯದಿ(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ಯದಿ(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    ಯದಿ(!mean.empty()) {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ಪ್ರತ್ಯಾಗಮ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(ಒಳಹರಿವು_Array _W, ಒಳಹರಿವು_Array _mean, ಒಳಹರಿವು_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ಪೂರ್ಣಾಙ್ಕ n = src.rows;
    ಪೂರ್ಣಾಙ್ಕ d = src.cols;
    // make sure the data has the correct shape
    ಯದಿ(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ಯದಿ(!mean.empty() && (mean.total() != (size_t) W.rows)) {
        String error_message = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.cols, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convertTo(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    ಯದಿ(!mean.empty()) {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    ಪ್ರತ್ಯಾಗಮ X;
}


ವರ್ಗ EigenvalueDecomposition {
ಗುಪ್ತ:

    // Holds the data dimension.
    ಪೂರ್ಣಾಙ್ಕ n;

    // Pointer to internal memory.
    ದ್ವಿಗುಣ *d, *e, *ort;
    ದ್ವಿಗುಣ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(ಪೂರ್ಣಾಙ್ಕ m) {
        ಪ್ರತ್ಯಾಗಮ ನೂತನ _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(ಪೂರ್ಣಾಙ್ಕ m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++)
            arr[i] = val;
        ಪ್ರತ್ಯಾಗಮ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ _n) {
        _Tp **arr = ನೂತನ _Tp*[m];
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++)
            arr[i] = ನೂತನ _Tp[_n];
        ಪ್ರತ್ಯಾಗಮ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ಪ್ರತ್ಯಾಗಮ arr;
    }

    ಸ್ಥಿರ ಶೂನ್ಯ complex_div(ದ್ವಿಗುಣ xr, ದ್ವಿಗುಣ xi, ದ್ವಿಗುಣ yr, ದ್ವಿಗುಣ yi, ದ್ವಿಗುಣ& cdivr, ದ್ವಿಗುಣ& cdivi) {
        ದ್ವಿಗುಣ r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        ಯದಿ (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ಅನ್ಯಥಾ {
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
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ max_iters_ಎಣಿಕೆ = 1000 * ಏತತ್->n;

        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nn = ಏತತ್->n; CV_Assert(nn > 0);
        ಪೂರ್ಣಾಙ್ಕ n1 = nn - 1;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ = 0;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ = nn - 1;
        ಧ್ರುವ ದ್ವಿಗುಣ eps = std::numeric_limits<ದ್ವಿಗುಣ>::epsilon();
        ದ್ವಿಗುಣ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ದ್ವಿಗುಣ norm = 0.0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nn; i++) {
#ಯದಿ 0 // 'if' condition is always false
            ಯದಿ (i < ಕೆಳಗಿನ || i > ಮೇಲಿನ) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        ಪೂರ್ಣಾಙ್ಕ iter = 0;
        ಯಾವತ್ (n1 >= ಕೆಳಗಿನ) {

            // Look for single small sub-diagonal element
            ಪೂರ್ಣಾಙ್ಕ l = n1;
            ಯಾವತ್ (l > ಕೆಳಗಿನ) {
                ಯದಿ (norm < FLT_EPSILON) {
                    ಭಙ್ಗ;
                }
                ದ್ವಿಗುಣ s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                ಯದಿ (s == 0.0) {
                    s = norm;
                }
                ಯದಿ (std::abs(H[l][l - 1]) < eps * s) {
                    ಭಙ್ಗ;
                }
                l--;
            }

            // Check for convergence
            ಯದಿ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ಅನ್ಯಥಾ ಯದಿ (l == n1 - 1) {
                // Two roots found
                ದ್ವಿಗುಣ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ದ್ವಿಗುಣ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ದ್ವಿಗುಣ q = p * p + w;
                ದ್ವಿಗುಣ z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ದ್ವಿಗುಣ x = H[n1][n1];

                ಯದಿ (q >= 0) {
                    // Real pair
                    ಯದಿ (p >= 0) {
                        z = p + z;
                    } ಅನ್ಯಥಾ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ಯದಿ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ದ್ವಿಗುಣ s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    ದ್ವಿಗುಣ r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ಅನ್ಯಥಾ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ಅನ್ಯಥಾ {
                // No convergence yet

                // Form shift
                ದ್ವಿಗುಣ x = H[n1][n1];
                ದ್ವಿಗುಣ y = 0.0;
                ದ್ವಿಗುಣ w = 0.0;
                ಯದಿ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ಯದಿ (iter == 10) {
                    exshift += x;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಕೆಳಗಿನ; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ದ್ವಿಗುಣ s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ಯದಿ (iter == 30) {
                    ದ್ವಿಗುಣ s = (y - x) * 0.5;
                    s = s * s + w;
                    ಯದಿ (s > 0) {
                        s = std::sqrt(s);
                        ಯದಿ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಕೆಳಗಿನ; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ಯದಿ (iter > max_iters_ಎಣಿಕೆ)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ದ್ವಿಗುಣ p = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ q = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ r = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                ಪೂರ್ಣಾಙ್ಕ m = n1 - 2;
                ಯಾವತ್ (m >= l) {
                    ದ್ವಿಗುಣ z = H[m][m];
                    r = x - z;
                    ದ್ವಿಗುಣ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ಯದಿ (m == l) {
                        ಭಙ್ಗ;
                    }
                    ಯದಿ (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ಭಙ್ಗ;
                    }
                    m--;
                }

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ಯದಿ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = m; k < n1; k++) {

                    ತಾರ್ಕಿಕ notlast = (k != n1 - 1);
                    ಯದಿ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        ಯದಿ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ಯದಿ (x == 0.0) {
                        ಭಙ್ಗ;
                    }
                    ದ್ವಿಗುಣ s = std::sqrt(p * p + q * q + r * r);
                    ಯದಿ (p < 0) {
                        s = -s;
                    }
                    ಯದಿ (s != 0) {
                        ಯದಿ (k != m) {
                            H[k][k - 1] = -s * x;
                        } ಅನ್ಯಥಾ ಯದಿ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ದ್ವಿಗುಣ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ಯದಿ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ಯದಿ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ಯದಿ (notlast) {
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

        ಯದಿ (norm < FLT_EPSILON) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಪ್ರತಿ (n1 = nn - 1; n1 >= 0; n1--) {
            ದ್ವಿಗುಣ p = d[n1];
            ದ್ವಿಗುಣ q = e[n1];

            ಯದಿ (q == 0) {
                // Real vector
                ದ್ವಿಗುಣ z = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ s = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                ಪೂರ್ಣಾಙ್ಕ l = n1;
                H[n1][n1] = 1.0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = n1 - 1; i >= 0; i--) {
                    ದ್ವಿಗುಣ w = H[i][i] - p;
                    ದ್ವಿಗುಣ r = 0.0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ಯದಿ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ಅನ್ಯಥಾ {
                        l = i;
                        ಯದಿ (e[i] == 0.0) {
                            ಯದಿ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ಅನ್ಯಥಾ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ಅನ್ಯಥಾ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ದ್ವಿಗುಣ x = H[i][i + 1];
                            ದ್ವಿಗುಣ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ದ್ವಿಗುಣ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ಯದಿ (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ಅನ್ಯಥಾ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ದ್ವಿಗುಣ t = std::abs(H[i][n1]);
                        ಯದಿ ((eps * t) * t > 1) {
                            ದ್ವಿಗುಣ inv_t = 1.0 / t;
                            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ಅನ್ಯಥಾ ಯದಿ (q < 0) {
                // Complex vector
                ದ್ವಿಗುಣ z = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ r = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();
                ದ್ವಿಗುಣ s = std::numeric_limits<ದ್ವಿಗುಣ>::quiet_NaN();

                ಪೂರ್ಣಾಙ್ಕ l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ಯದಿ (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ಅನ್ಯಥಾ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = n1 - 2; i >= 0; i--) {
                    ದ್ವಿಗುಣ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ದ್ವಿಗುಣ w = H[i][i] - p;

                    ಯದಿ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ಅನ್ಯಥಾ {
                        l = i;
                        ಯದಿ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ಅನ್ಯಥಾ {
                            // Solve complex equations

                            ದ್ವಿಗುಣ x = H[i][i + 1];
                            ದ್ವಿಗುಣ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ಯದಿ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ಯದಿ (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ಅನ್ಯಥಾ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ದ್ವಿಗುಣ t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        ಯದಿ ((eps * t) * t > 1) {
                            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ಯದಿ 0 // 'if' condition is always false
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nn; i++) {
            ಯದಿ (i < ಕೆಳಗಿನ || i > ಮೇಲಿನ) {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = nn - 1; j >= ಕೆಳಗಿನ; j--) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಕೆಳಗಿನ; i <= ಮೇಲಿನ; i++) {
                ದ್ವಿಗುಣ z = 0.0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = ಕೆಳಗಿನ; k <= std::min(j, ಮೇಲಿನ); k++) {
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
        ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ = 0;
        ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ = n - 1;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ m = ಕೆಳಗಿನ + 1; m < ಮೇಲಿನ; m++) {

            // Scale column.

            ದ್ವಿಗುಣ scale = 0.0;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m; i <= ಮೇಲಿನ; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            ಯದಿ (scale != 0.0) {

                // Compute Householder transformation.

                ದ್ವಿಗುಣ h = 0.0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಮೇಲಿನ; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ದ್ವಿಗುಣ g = std::sqrt(h);
                ಯದಿ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = m; j < n; j++) {
                    ದ್ವಿಗುಣ f = 0.0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ಮೇಲಿನ; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m; i <= ಮೇಲಿನ; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <= ಮೇಲಿನ; i++) {
                    ದ್ವಿಗುಣ f = 0.0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = ಮೇಲಿನ; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = m; j <= ಮೇಲಿನ; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ m = ಮೇಲಿನ - 1; m > ಕೆಳಗಿನ; m--) {
            ಯದಿ (H[m][m - 1] != 0.0) {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m + 1; i <= ಮೇಲಿನ; i++) {
                    ort[i] = H[i][m - 1];
                }
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = m; j <= ಮೇಲಿನ; j++) {
                    ದ್ವಿಗುಣ g = 0.0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m; i <= ಮೇಲಿನ; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = m; i <= ಮೇಲಿನ; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    ಶೂನ್ಯ release() {
        // releases the working data
        delete[] d; d = ರಿಕ್ತ;
        delete[] e; e = ರಿಕ್ತ;
        delete[] ort; ort = ರಿಕ್ತ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ಯದಿ (H) delete[] H[i];
            ಯದಿ (V) delete[] V[i];
        }
        delete[] H; H = ರಿಕ್ತ;
        delete[] V; V = ರಿಕ್ತ;
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
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
                _eigenvalues.at<ದ್ವಿಗುಣ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < n; j++)
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
        d(ರಿಕ್ತ), e(ರಿಕ್ತ), ort(ರಿಕ್ತ),
        V(ರಿಕ್ತ), H(ರಿಕ್ತ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    ಶೂನ್ಯ compute(ಒಳಹರಿವು_Array src, ತಾರ್ಕಿಕ fallbackSymmetric = ಸತ್ಯ)
    {
        CV_INSTRUMENT_REGION();

        ಯದಿ(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ಅನ್ಯಥಾ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ಏತತ್->n = tmp.cols;
            // Allocate the matrix data to work on.
            ಏತತ್->H = alloc_2d<ದ್ವಿಗುಣ> (n, n);
            // Now safely copy the data.
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < tmp.rows; i++) {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < tmp.cols; j++) {
                    ಏತತ್->H[i][j] = tmp.at<ದ್ವಿಗುಣ>(i, j);
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
    Mat eigenvalues() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _eigenvectors; }
};

ಶೂನ್ಯ eigenNonSymmetric(ಒಳಹರಿವು_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ಪೂರ್ಣಾಙ್ಕ n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ಯದಿ (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    ಅನ್ಯಥಾ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ಅಸತ್ಯ);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ದ್ವಿಗುಣ> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<ಪೂರ್ಣಾಙ್ಕ> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<ದ್ವಿಗುಣ> sorted_eigenvalues64f(n);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    ಯದಿ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<ದ್ವಿಗುಣ> sorted_eigenvectors64f(n, n, CV_64FC1);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
        {
            ದ್ವಿಗುಣ* pDst = sorted_eigenvectors64f.ptr<ದ್ವಿಗುಣ>(i);
            ದ್ವಿಗುಣ* pSrc = eigenvectors64f.ptr<ದ್ವಿಗುಣ>(sort_indexes[i]);
            CV_Assert(pSrc != ರಿಕ್ತ);
            memcpy(pDst, pSrc, n * ಪರಿಮಾಣ(ದ್ವಿಗುಣ));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_components) : ಅಂಕೆ_components_2(ಅಂಕೆ_components) { }

LDA::LDA(ಒಳಹರಿವು_Array_Of_Arrays src, ಒಳಹರಿವು_Array labels, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_components) : ಅಂಕೆ_components_2(ಅಂಕೆ_components)
{
    ಏತತ್->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

ಶೂನ್ಯ LDA::save(ಧ್ರುವ String& filename) ಧ್ರುವ
{
    FileStorage fs(filename, FileStorage::WRITE);
    ಯದಿ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ಏತತ್->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
ಶೂನ್ಯ LDA::load(ಧ್ರುವ String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ಯದಿ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ಏತತ್->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
ಶೂನ್ಯ LDA::save(FileStorage& fs) ಧ್ರುವ {
    // write matrices
    fs << "num_components" << ಅಂಕೆ_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
ಶೂನ್ಯ LDA::load(ಧ್ರುವ FileStorage& fs) {
    //read matrices
    fs["num_components"] >> ಅಂಕೆ_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

ಶೂನ್ಯ LDA::lda(ಒಳಹರಿವು_Array_Of_Arrays _src, ಒಳಹರಿವು_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<ಪೂರ್ಣಾಙ್ಕ> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        ಪ್ರತಿ(unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<ಪೂರ್ಣಾಙ್ಕ>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<ಪೂರ್ಣಾಙ್ಕ> mapped_labels(labels.size());
    std::vector<ಪೂರ್ಣಾಙ್ಕ> num2label = remove_dups(labels);
    std::map<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ> label2num;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)num2label.size(); i++)
        label2num[num2label[i]] = i;
    ಪ್ರತಿ (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    ಪೂರ್ಣಾಙ್ಕ N = data.rows;
    ಪೂರ್ಣಾಙ್ಕ D = data.cols;
    // number of unique labels
    ಪೂರ್ಣಾಙ್ಕ C = (ಪೂರ್ಣಾಙ್ಕ)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ಯದಿ(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    ಯದಿ (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    ಯದಿ (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    ಯದಿ ((ಅಂಕೆ_components_2 <= 0) || (ಅಂಕೆ_components_2 >= C)) {
        ಅಂಕೆ_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<ಪೂರ್ಣಾಙ್ಕ> ಅಂಕೆ_Class(C);
    // initialize
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < C; i++) {
        ಅಂಕೆ_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++) {
        Mat instance = data.row(i);
        ಪೂರ್ಣಾಙ್ಕ classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        ಅಂಕೆ_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<ದ್ವಿಗುಣ> (N));
    // calculate class means
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<ದ್ವಿಗುಣ> (ಅಂಕೆ_Class[i]));
    }
    // subtract class means
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++) {
        ಪೂರ್ಣಾಙ್ಕ classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, ಸತ್ಯ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, ಸತ್ಯ);
        add(Sb, tmp, Sb);
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
    std::vector<ಪೂರ್ಣಾಙ್ಕ> sorted_indices = argsort(_eigenvalues, ಅಸತ್ಯ);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, ಅಂಕೆ_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, ಅಂಕೆ_components_2));
}

ಶೂನ್ಯ LDA::compute(ಒಳಹರಿವು_Array_Of_Arrays _src, ಒಳಹರಿವು_Array _lbls) {
    ವಿಕಲ್ಪ(_src.kind()) {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT:
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        ಭಙ್ಗ;
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        ಭಙ್ಗ;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(ಒಳಹರಿವು_Array src) {
   ಪ್ರತ್ಯಾಗಮ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(ಒಳಹರಿವು_Array src) {
   ಪ್ರತ್ಯಾಗಮ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
