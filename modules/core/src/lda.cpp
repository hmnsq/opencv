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

#include "precomp.hpp"
#include <iostream>
#include <map>
#include <set>

namespace cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
anta_hstha std::vector<_Tp> remove_dups(const std::vector<_Tp>& src) {
    typedef typename std::set<_Tp>::const_iterator constSetIterator;
    typedef typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    for (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    for (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    ᨀᨘᨒ elems;
}

static Mat argsort(InputArray _src, bool ascending=true)
{
    Mat src = _src.getMat();
    ᨀᨘ (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    puur_naa_nka flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    ᨀᨘᨒ sorted_indices;
}

static Mat asRowMatrix(InputArrayOfArrays src, puur_naa_nka rtype, dviguu_na alpha=1, dviguu_na beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ᨀᨘ(src.kind() != _InputArray::STD_VECTOR_MAT && src.kind() != _InputArray::STD_ARRAY_MAT &&
        src.kind() != _InputArray::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    ᨀᨘ(n == 0)
        ᨀᨘᨒ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((puur_naa_nka)n, (puur_naa_nka)d, rtype);
    // now copy data
    for(puur_naa_nka i = 0; i < (puur_naa_nka)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ᨀᨘ(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (puur_naa_nka)d, (puur_naa_nka)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        ᨀᨘ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } ᨀᨘᨁ {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    ᨀᨘᨒ data;
}

static void sortMatrixColumnsByIndices(InputArray _src, InputArray _indices, OutputArray _dst) {
    ᨀᨘ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<puur_naa_nka> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    for(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((puur_naa_nka)idx);
        originalCol.copyTo(sortedCol);
    }
}

static Mat sortMatrixColumnsByIndices(InputArray src, InputArray indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    ᨀᨘᨒ dst;
}


template<typename _Tp> static bool
isSymmetric_(InputArray src) {
    Mat _src = src.getMat();
    ᨀᨘ(_src.cols != _src.rows)
        ᨀᨘᨒ false;
    for (puur_naa_nka i = 0; i < _src.rows; i++) {
        for (puur_naa_nka j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ᨀᨘ (a != b) {
                ᨀᨘᨒ false;
            }
        }
    }
    ᨀᨘᨒ true;
}

template<typename _Tp> static bool
isSymmetric_(InputArray src, dviguu_na eps) {
    Mat _src = src.getMat();
    ᨀᨘ(_src.cols != _src.rows)
        ᨀᨘᨒ false;
    for (puur_naa_nka i = 0; i < _src.rows; i++) {
        for (puur_naa_nka j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ᨀᨘ (std::abs(a - b) > eps) {
                ᨀᨘᨒ false;
            }
        }
    }
    ᨀᨘᨒ true;
}

static bool isSymmetric(InputArray src, dviguu_na eps=1e-16)
{
    Mat m = src.getMat();
    switch (m.type()) {
        pak_sa CV_8SC1: ᨀᨘᨒ isSymmetric_<var_na>(m); bha_nga;
        pak_sa CV_8UC1:
            ᨀᨘᨒ isSymmetric_<unsigned var_na>(m); bha_nga;
        pak_sa CV_16SC1:
            ᨀᨘᨒ isSymmetric_<short>(m); bha_nga;
        pak_sa CV_16UC1:
            ᨀᨘᨒ isSymmetric_<unsigned short>(m); bha_nga;
        pak_sa CV_32SC1:
            ᨀᨘᨒ isSymmetric_<puur_naa_nka>(m); bha_nga;
        pak_sa CV_32FC1:
            ᨀᨘᨒ isSymmetric_<float>(m, eps); bha_nga;
        pak_sa CV_64FC1:
            ᨀᨘᨒ isSymmetric_<dviguu_na>(m, eps); bha_nga;
        default:
            bha_nga;
    }
    ᨀᨘᨒ false;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(InputArray _W, InputArray _mean, InputArray _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    puur_naa_nka n = src.rows;
    puur_naa_nka d = src.cols;
    // make sure the data has the correct shape
    ᨀᨘ(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ᨀᨘ(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    ᨀᨘ(!mean.empty()) {
        for(puur_naa_nka i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ᨀᨘᨒ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(InputArray _W, InputArray _mean, InputArray _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    puur_naa_nka n = src.rows;
    puur_naa_nka d = src.cols;
    // make sure the data has the correct shape
    ᨀᨘ(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ᨀᨘ(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    ᨀᨘ(!mean.empty()) {
        for(puur_naa_nka i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    ᨀᨘᨒ X;
}


class EigenvalueDecomposition {
private:

    // Holds the data dimension.
    puur_naa_nka n;

    // Pointer to internal memory.
    dviguu_na *d, *e, *ort;
    dviguu_na **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(puur_naa_nka m) {
        ᨀᨘᨒ new _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(puur_naa_nka m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        for (puur_naa_nka i = 0; i < m; i++)
            arr[i] = val;
        ᨀᨘᨒ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(puur_naa_nka m, puur_naa_nka _n) {
        _Tp **arr = new _Tp*[m];
        for (puur_naa_nka i = 0; i < m; i++)
            arr[i] = new _Tp[_n];
        ᨀᨘᨒ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(puur_naa_nka m, puur_naa_nka _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        for (puur_naa_nka i = 0; i < m; i++) {
            for (puur_naa_nka j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ᨀᨘᨒ arr;
    }

    static void complex_div(dviguu_na xr, dviguu_na xi, dviguu_na yr, dviguu_na yi, dviguu_na& cdivr, dviguu_na& cdivi) {
        dviguu_na r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        ᨀᨘ (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ᨀᨘᨁ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    void hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        const puur_naa_nka max_iters_count = 1000 * this->n;

        const puur_naa_nka nn = this->n; CV_Assert(nn > 0);
        puur_naa_nka n1 = nn - 1;
        const puur_naa_nka low = 0;
        const puur_naa_nka high = nn - 1;
        const dviguu_na eps = std::numeric_limits<dviguu_na>::epsilon();
        dviguu_na exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        dviguu_na norm = 0.0;
        for (puur_naa_nka i = 0; i < nn; i++) {
#ᨀᨘ 0 // 'if' condition is always false
            ᨀᨘ (i < low || i > high) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            for (puur_naa_nka j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        puur_naa_nka iter = 0;
        ᨀᨛ (n1 >= low) {

            // Look for single small sub-diagonal element
            puur_naa_nka l = n1;
            ᨀᨛ (l > low) {
                ᨀᨘ (norm < FLT_EPSILON) {
                    bha_nga;
                }
                dviguu_na s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                ᨀᨘ (s == 0.0) {
                    s = norm;
                }
                ᨀᨘ (std::abs(H[l][l - 1]) < eps * s) {
                    bha_nga;
                }
                l--;
            }

            // Check for convergence
            ᨀᨘ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ᨀᨘᨁ ᨀᨘ (l == n1 - 1) {
                // Two roots found
                dviguu_na w = H[n1][n1 - 1] * H[n1 - 1][n1];
                dviguu_na p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                dviguu_na q = p * p + w;
                dviguu_na z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                dviguu_na x = H[n1][n1];

                ᨀᨘ (q >= 0) {
                    // Real pair
                    ᨀᨘ (p >= 0) {
                        z = p + z;
                    } ᨀᨘᨁ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ᨀᨘ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    dviguu_na s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    dviguu_na r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    for (puur_naa_nka j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    for (puur_naa_nka i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    for (puur_naa_nka i = low; i <= high; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ᨀᨘᨁ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ᨀᨘᨁ {
                // No convergence yet

                // Form shift
                dviguu_na x = H[n1][n1];
                dviguu_na y = 0.0;
                dviguu_na w = 0.0;
                ᨀᨘ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ᨀᨘ (iter == 10) {
                    exshift += x;
                    for (puur_naa_nka i = low; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    dviguu_na s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ᨀᨘ (iter == 30) {
                    dviguu_na s = (y - x) * 0.5;
                    s = s * s + w;
                    ᨀᨘ (s > 0) {
                        s = std::sqrt(s);
                        ᨀᨘ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        for (puur_naa_nka i = low; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ᨀᨘ (iter > max_iters_count)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                dviguu_na p = std::numeric_limits<dviguu_na>::quiet_NaN();
                dviguu_na q = std::numeric_limits<dviguu_na>::quiet_NaN();
                dviguu_na r = std::numeric_limits<dviguu_na>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                puur_naa_nka m = n1 - 2;
                ᨀᨛ (m >= l) {
                    dviguu_na z = H[m][m];
                    r = x - z;
                    dviguu_na s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ᨀᨘ (m == l) {
                        bha_nga;
                    }
                    ᨀᨘ (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        bha_nga;
                    }
                    m--;
                }

                for (puur_naa_nka i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ᨀᨘ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                for (puur_naa_nka k = m; k < n1; k++) {

                    bool notlast = (k != n1 - 1);
                    ᨀᨘ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        ᨀᨘ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ᨀᨘ (x == 0.0) {
                        bha_nga;
                    }
                    dviguu_na s = std::sqrt(p * p + q * q + r * r);
                    ᨀᨘ (p < 0) {
                        s = -s;
                    }
                    ᨀᨘ (s != 0) {
                        ᨀᨘ (k != m) {
                            H[k][k - 1] = -s * x;
                        } ᨀᨘᨁ ᨀᨘ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        dviguu_na z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        for (puur_naa_nka j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ᨀᨘ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        for (puur_naa_nka i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ᨀᨘ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        for (puur_naa_nka i = low; i <= high; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ᨀᨘ (notlast) {
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

        ᨀᨘ (norm < FLT_EPSILON) {
            ᨀᨘᨒ;
        }

        for (n1 = nn - 1; n1 >= 0; n1--) {
            dviguu_na p = d[n1];
            dviguu_na q = e[n1];

            ᨀᨘ (q == 0) {
                // Real vector
                dviguu_na z = std::numeric_limits<dviguu_na>::quiet_NaN();
                dviguu_na s = std::numeric_limits<dviguu_na>::quiet_NaN();

                puur_naa_nka l = n1;
                H[n1][n1] = 1.0;
                for (puur_naa_nka i = n1 - 1; i >= 0; i--) {
                    dviguu_na w = H[i][i] - p;
                    dviguu_na r = 0.0;
                    for (puur_naa_nka j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ᨀᨘ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ᨀᨘᨁ {
                        l = i;
                        ᨀᨘ (e[i] == 0.0) {
                            ᨀᨘ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ᨀᨘᨁ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ᨀᨘᨁ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            dviguu_na x = H[i][i + 1];
                            dviguu_na y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            dviguu_na t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ᨀᨘ (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ᨀᨘᨁ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        dviguu_na t = std::abs(H[i][n1]);
                        ᨀᨘ ((eps * t) * t > 1) {
                            dviguu_na inv_t = 1.0 / t;
                            for (puur_naa_nka j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ᨀᨘᨁ ᨀᨘ (q < 0) {
                // Complex vector
                dviguu_na z = std::numeric_limits<dviguu_na>::quiet_NaN();
                dviguu_na r = std::numeric_limits<dviguu_na>::quiet_NaN();
                dviguu_na s = std::numeric_limits<dviguu_na>::quiet_NaN();

                puur_naa_nka l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ᨀᨘ (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ᨀᨘᨁ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                for (puur_naa_nka i = n1 - 2; i >= 0; i--) {
                    dviguu_na ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    for (puur_naa_nka j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    dviguu_na w = H[i][i] - p;

                    ᨀᨘ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ᨀᨘᨁ {
                        l = i;
                        ᨀᨘ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ᨀᨘᨁ {
                            // Solve complex equations

                            dviguu_na x = H[i][i + 1];
                            dviguu_na y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ᨀᨘ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ᨀᨘ (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ᨀᨘᨁ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        dviguu_na t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        ᨀᨘ ((eps * t) * t > 1) {
                            for (puur_naa_nka j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ᨀᨘ 0 // 'if' condition is always false
        for (puur_naa_nka i = 0; i < nn; i++) {
            ᨀᨘ (i < low || i > high) {
                for (puur_naa_nka j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        for (puur_naa_nka j = nn - 1; j >= low; j--) {
            for (puur_naa_nka i = low; i <= high; i++) {
                dviguu_na z = 0.0;
                for (puur_naa_nka k = low; k <= std::min(j, high); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    void orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        puur_naa_nka low = 0;
        puur_naa_nka high = n - 1;

        for (puur_naa_nka m = low + 1; m < high; m++) {

            // Scale column.

            dviguu_na scale = 0.0;
            for (puur_naa_nka i = m; i <= high; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            ᨀᨘ (scale != 0.0) {

                // Compute Householder transformation.

                dviguu_na h = 0.0;
                for (puur_naa_nka i = high; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                dviguu_na g = std::sqrt(h);
                ᨀᨘ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                for (puur_naa_nka j = m; j < n; j++) {
                    dviguu_na f = 0.0;
                    for (puur_naa_nka i = high; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    for (puur_naa_nka i = m; i <= high; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                for (puur_naa_nka i = 0; i <= high; i++) {
                    dviguu_na f = 0.0;
                    for (puur_naa_nka j = high; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    for (puur_naa_nka j = m; j <= high; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        for (puur_naa_nka i = 0; i < n; i++) {
            for (puur_naa_nka j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        for (puur_naa_nka m = high - 1; m > low; m--) {
            ᨀᨘ (H[m][m - 1] != 0.0) {
                for (puur_naa_nka i = m + 1; i <= high; i++) {
                    ort[i] = H[i][m - 1];
                }
                for (puur_naa_nka j = m; j <= high; j++) {
                    dviguu_na g = 0.0;
                    for (puur_naa_nka i = m; i <= high; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    for (puur_naa_nka i = m; i <= high; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    void release() {
        // releases the working data
        delete[] d; d = NULL;
        delete[] e; e = NULL;
        delete[] ort; ort = NULL;
        for (puur_naa_nka i = 0; i < n; i++) {
            ᨀᨘ (H) delete[] H[i];
            ᨀᨘ (V) delete[] V[i];
        }
        delete[] H; H = NULL;
        delete[] V; V = NULL;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    void compute() {
        // Allocate memory for the working data.
        V = alloc_2d<dviguu_na> (n, n, 0.0);
        d = alloc_1d<dviguu_na> (n);
        e = alloc_1d<dviguu_na> (n);
        ort = alloc_1d<dviguu_na> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            for (puur_naa_nka i = 0; i < n; i++) {
                _eigenvalues.at<dviguu_na> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            for (puur_naa_nka i = 0; i < n; i++)
                for (puur_naa_nka j = 0; j < n; j++)
                    _eigenvectors.at<dviguu_na> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

public:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(NULL), e(NULL), ort(NULL),
        V(NULL), H(NULL)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    void compute(InputArray src, bool fallbackSymmetric = true)
    {
        CV_INSTRUMENT_REGION();

        ᨀᨘ(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ᨀᨘᨁ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            this->n = tmp.cols;
            // Allocate the matrix data to work on.
            this->H = alloc_2d<dviguu_na> (n, n);
            // Now safely copy the data.
            for (puur_naa_nka i = 0; i < tmp.rows; i++) {
                for (puur_naa_nka j = 0; j < tmp.cols; j++) {
                    this->H[i][j] = tmp.at<dviguu_na>(i, j);
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
    Mat eigenvalues() const { ᨀᨘᨒ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() const { ᨀᨘᨒ _eigenvectors; }
};

void eigenNonSymmetric(InputArray _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    puur_naa_nka type = src.type();
    puur_naa_nka n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ᨀᨘ (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    ᨀᨘᨁ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, false);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<dviguu_na> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<puur_naa_nka> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<dviguu_na> sorted_eigenvalues64f(n);
    for (puur_naa_nka i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    ᨀᨘ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<dviguu_na> sorted_eigenvectors64f(n, n, CV_64FC1);
        for (puur_naa_nka i = 0; i < n; i++)
        {
            dviguu_na* pDst = sorted_eigenvectors64f.ptr<dviguu_na>(i);
            dviguu_na* pSrc = eigenvectors64f.ptr<dviguu_na>(sort_indexes[i]);
            CV_Assert(pSrc != NULL);
            memcpy(pDst, pSrc, n * parimaa_na(dviguu_na));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(puur_naa_nka num_components) : _num_components(num_components) { }

LDA::LDA(InputArrayOfArrays src, InputArray labels, puur_naa_nka num_components) : _num_components(num_components)
{
    this->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

void LDA::save(const String& filename) const
{
    FileStorage fs(filename, FileStorage::WRITE);
    ᨀᨘ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    this->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
void LDA::load(const String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ᨀᨘ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    this->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
void LDA::save(FileStorage& fs) const {
    // write matrices
    fs << "num_components" << _num_components;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
void LDA::load(const FileStorage& fs) {
    //read matrices
    fs["num_components"] >> _num_components;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

void LDA::lda(InputArrayOfArrays _src, InputArray _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<puur_naa_nka> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        for(unsigned puur_naa_nka i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<puur_naa_nka>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<puur_naa_nka> mapped_labels(labels.size());
    std::vector<puur_naa_nka> num2label = remove_dups(labels);
    std::map<puur_naa_nka, puur_naa_nka> label2num;
    for (puur_naa_nka i = 0; i < (puur_naa_nka)num2label.size(); i++)
        label2num[num2label[i]] = i;
    for (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    puur_naa_nka N = data.rows;
    puur_naa_nka D = data.cols;
    // number of unique labels
    puur_naa_nka C = (puur_naa_nka)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ᨀᨘ(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    ᨀᨘ (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    ᨀᨘ (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    ᨀᨘ ((_num_components <= 0) || (_num_components >= C)) {
        _num_components = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<puur_naa_nka> numClass(C);
    // initialize
    for (puur_naa_nka i = 0; i < C; i++) {
        numClass[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    for (puur_naa_nka i = 0; i < N; i++) {
        Mat instance = data.row(i);
        puur_naa_nka classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        numClass[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<dviguu_na> (N));
    // calculate class means
    for (puur_naa_nka i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<dviguu_na> (numClass[i]));
    }
    // subtract class means
    for (puur_naa_nka i = 0; i < N; i++) {
        puur_naa_nka classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, true);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    for (puur_naa_nka i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, true);
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
    std::vector<puur_naa_nka> sorted_indices = argsort(_eigenvalues, false);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, _num_components));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, _num_components));
}

void LDA::compute(InputArrayOfArrays _src, InputArray _lbls) {
    switch(_src.kind()) {
    pak_sa _InputArray::STD_VECTOR_MAT:
    pak_sa _InputArray::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        bha_nga;
    pak_sa _InputArray::MAT:
        lda(_src.getMat(), _lbls);
        bha_nga;
    default:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        bha_nga;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(InputArray src) {
   ᨀᨘᨒ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(InputArray src) {
   ᨀᨘᨒ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
