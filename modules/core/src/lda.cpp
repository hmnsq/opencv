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

#ضمن "precomp.hpp"
#ضمن <iostream>
#ضمن <map>
#ضمن <set>

نطاق cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
مضمن std::vector<_Tp> remove_dups(ثابت std::vector<_Tp>& src) {
    تعريفنوع typename std::set<_Tp>::const_iterator constSetIterator;
    تعريفنوع typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    لكل (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    لكل (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    ܦܢܐ elems;
}

ساكن Mat argsort(InputArray _src, منطقي ascending=ܫܪܪܐ)
{
    Mat src = _src.getMat();
    ܐܢ (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    صحيح flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    ܦܢܐ sorted_indices;
}

ساكن Mat asRowMatrix(InputArrayOfArrays src, صحيح rtype, مزدوج alpha=1, مزدوج beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ܐܢ(src.kind() != _InputArray::STD_VECTOR_MAT && src.kind() != _InputArray::STD_ARRAY_MAT &&
        src.kind() != _InputArray::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    ܐܢ(n == 0)
        ܦܢܐ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((صحيح)n, (صحيح)d, rtype);
    // now copy data
    لكل(صحيح i = 0; i < (صحيح)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ܐܢ(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (صحيح)d, (صحيح)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        ܐܢ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } ܐܚܪܢܐ {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    ܦܢܐ data;
}

ساكن فراغ sortMatrixColumnsByIndices(InputArray _src, InputArray _indices, OutputArray _dst) {
    ܐܢ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<صحيح> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    لكل(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((صحيح)idx);
        originalCol.copyTo(sortedCol);
    }
}

ساكن Mat sortMatrixColumnsByIndices(InputArray src, InputArray indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    ܦܢܐ dst;
}


template<typename _Tp> ساكن منطقي
isSymmetric_(InputArray src) {
    Mat _src = src.getMat();
    ܐܢ(_src.cols != _src.rows)
        ܦܢܐ ܕܓܠܐ;
    لكل (صحيح i = 0; i < _src.rows; i++) {
        لكل (صحيح j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ܐܢ (a != b) {
                ܦܢܐ ܕܓܠܐ;
            }
        }
    }
    ܦܢܐ ܫܪܪܐ;
}

template<typename _Tp> ساكن منطقي
isSymmetric_(InputArray src, مزدوج eps) {
    Mat _src = src.getMat();
    ܐܢ(_src.cols != _src.rows)
        ܦܢܐ ܕܓܠܐ;
    لكل (صحيح i = 0; i < _src.rows; i++) {
        لكل (صحيح j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ܐܢ (std::abs(a - b) > eps) {
                ܦܢܐ ܕܓܠܐ;
            }
        }
    }
    ܦܢܐ ܫܪܪܐ;
}

ساكن منطقي isSymmetric(InputArray src, مزدوج eps=1e-16)
{
    Mat m = src.getMat();
    بدل (m.type()) {
        حالة CV_8SC1: ܦܢܐ isSymmetric_<حرف>(m); اكسر;
        حالة CV_8UC1:
            ܦܢܐ isSymmetric_<unsigned حرف>(m); اكسر;
        حالة CV_16SC1:
            ܦܢܐ isSymmetric_<short>(m); اكسر;
        حالة CV_16UC1:
            ܦܢܐ isSymmetric_<unsigned short>(m); اكسر;
        حالة CV_32SC1:
            ܦܢܐ isSymmetric_<صحيح>(m); اكسر;
        حالة CV_32FC1:
            ܦܢܐ isSymmetric_<عائم>(m, eps); اكسر;
        حالة CV_64FC1:
            ܦܢܐ isSymmetric_<مزدوج>(m, eps); اكسر;
        افتراضي:
            اكسر;
    }
    ܦܢܐ ܕܓܠܐ;
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
    صحيح n = src.rows;
    صحيح d = src.cols;
    // make sure the data has the correct shape
    ܐܢ(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ܐܢ(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    ܐܢ(!mean.empty()) {
        لكل(صحيح i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ܦܢܐ Y;
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
    صحيح n = src.rows;
    صحيح d = src.cols;
    // make sure the data has the correct shape
    ܐܢ(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ܐܢ(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    ܐܢ(!mean.empty()) {
        لكل(صحيح i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    ܦܢܐ X;
}


صنف EigenvalueDecomposition {
خاص:

    // Holds the data dimension.
    صحيح n;

    // Pointer to internal memory.
    مزدوج *d, *e, *ort;
    مزدوج **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(صحيح m) {
        ܦܢܐ جديد _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(صحيح m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        لكل (صحيح i = 0; i < m; i++)
            arr[i] = val;
        ܦܢܐ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(صحيح m, صحيح _n) {
        _Tp **arr = جديد _Tp*[m];
        لكل (صحيح i = 0; i < m; i++)
            arr[i] = جديد _Tp[_n];
        ܦܢܐ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(صحيح m, صحيح _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        لكل (صحيح i = 0; i < m; i++) {
            لكل (صحيح j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ܦܢܐ arr;
    }

    ساكن فراغ complex_div(مزدوج xr, مزدوج xi, مزدوج yr, مزدوج yi, مزدوج& cdivr, مزدوج& cdivi) {
        مزدوج r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        ܐܢ (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ܐܚܪܢܐ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    فراغ hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ثابت صحيح max_iters_count = 1000 * هذا->n;

        ثابت صحيح nn = هذا->n; CV_Assert(nn > 0);
        صحيح n1 = nn - 1;
        ثابت صحيح low = 0;
        ثابت صحيح high = nn - 1;
        ثابت مزدوج eps = std::numeric_limits<مزدوج>::epsilon();
        مزدوج exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        مزدوج norm = 0.0;
        لكل (صحيح i = 0; i < nn; i++) {
#ܐܢ 0 // 'if' condition is always false
            ܐܢ (i < low || i > high) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            لكل (صحيح j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        صحيح iter = 0;
        ܟܡܐ (n1 >= low) {

            // Look for single small sub-diagonal element
            صحيح l = n1;
            ܟܡܐ (l > low) {
                ܐܢ (norm < FLT_EPSILON) {
                    اكسر;
                }
                مزدوج s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                ܐܢ (s == 0.0) {
                    s = norm;
                }
                ܐܢ (std::abs(H[l][l - 1]) < eps * s) {
                    اكسر;
                }
                l--;
            }

            // Check for convergence
            ܐܢ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ܐܚܪܢܐ ܐܢ (l == n1 - 1) {
                // Two roots found
                مزدوج w = H[n1][n1 - 1] * H[n1 - 1][n1];
                مزدوج p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                مزدوج q = p * p + w;
                مزدوج z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                مزدوج x = H[n1][n1];

                ܐܢ (q >= 0) {
                    // Real pair
                    ܐܢ (p >= 0) {
                        z = p + z;
                    } ܐܚܪܢܐ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ܐܢ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    مزدوج s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    مزدوج r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    لكل (صحيح j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    لكل (صحيح i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    لكل (صحيح i = low; i <= high; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ܐܚܪܢܐ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ܐܚܪܢܐ {
                // No convergence yet

                // Form shift
                مزدوج x = H[n1][n1];
                مزدوج y = 0.0;
                مزدوج w = 0.0;
                ܐܢ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ܐܢ (iter == 10) {
                    exshift += x;
                    لكل (صحيح i = low; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    مزدوج s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ܐܢ (iter == 30) {
                    مزدوج s = (y - x) * 0.5;
                    s = s * s + w;
                    ܐܢ (s > 0) {
                        s = std::sqrt(s);
                        ܐܢ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        لكل (صحيح i = low; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ܐܢ (iter > max_iters_count)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                مزدوج p = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج q = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج r = std::numeric_limits<مزدوج>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                صحيح m = n1 - 2;
                ܟܡܐ (m >= l) {
                    مزدوج z = H[m][m];
                    r = x - z;
                    مزدوج s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ܐܢ (m == l) {
                        اكسر;
                    }
                    ܐܢ (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        اكسر;
                    }
                    m--;
                }

                لكل (صحيح i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ܐܢ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                لكل (صحيح k = m; k < n1; k++) {

                    منطقي notlast = (k != n1 - 1);
                    ܐܢ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        ܐܢ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ܐܢ (x == 0.0) {
                        اكسر;
                    }
                    مزدوج s = std::sqrt(p * p + q * q + r * r);
                    ܐܢ (p < 0) {
                        s = -s;
                    }
                    ܐܢ (s != 0) {
                        ܐܢ (k != m) {
                            H[k][k - 1] = -s * x;
                        } ܐܚܪܢܐ ܐܢ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        مزدوج z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        لكل (صحيح j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ܐܢ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        لكل (صحيح i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ܐܢ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        لكل (صحيح i = low; i <= high; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ܐܢ (notlast) {
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

        ܐܢ (norm < FLT_EPSILON) {
            ܦܢܐ;
        }

        لكل (n1 = nn - 1; n1 >= 0; n1--) {
            مزدوج p = d[n1];
            مزدوج q = e[n1];

            ܐܢ (q == 0) {
                // Real vector
                مزدوج z = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج s = std::numeric_limits<مزدوج>::quiet_NaN();

                صحيح l = n1;
                H[n1][n1] = 1.0;
                لكل (صحيح i = n1 - 1; i >= 0; i--) {
                    مزدوج w = H[i][i] - p;
                    مزدوج r = 0.0;
                    لكل (صحيح j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ܐܢ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ܐܚܪܢܐ {
                        l = i;
                        ܐܢ (e[i] == 0.0) {
                            ܐܢ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ܐܚܪܢܐ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ܐܚܪܢܐ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            مزدوج x = H[i][i + 1];
                            مزدوج y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            مزدوج t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ܐܢ (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ܐܚܪܢܐ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        مزدوج t = std::abs(H[i][n1]);
                        ܐܢ ((eps * t) * t > 1) {
                            مزدوج inv_t = 1.0 / t;
                            لكل (صحيح j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ܐܚܪܢܐ ܐܢ (q < 0) {
                // Complex vector
                مزدوج z = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج r = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج s = std::numeric_limits<مزدوج>::quiet_NaN();

                صحيح l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ܐܢ (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ܐܚܪܢܐ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                لكل (صحيح i = n1 - 2; i >= 0; i--) {
                    مزدوج ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    لكل (صحيح j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    مزدوج w = H[i][i] - p;

                    ܐܢ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ܐܚܪܢܐ {
                        l = i;
                        ܐܢ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ܐܚܪܢܐ {
                            // Solve complex equations

                            مزدوج x = H[i][i + 1];
                            مزدوج y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ܐܢ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ܐܢ (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ܐܚܪܢܐ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        مزدوج t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        ܐܢ ((eps * t) * t > 1) {
                            لكل (صحيح j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ܐܢ 0 // 'if' condition is always false
        لكل (صحيح i = 0; i < nn; i++) {
            ܐܢ (i < low || i > high) {
                لكل (صحيح j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        لكل (صحيح j = nn - 1; j >= low; j--) {
            لكل (صحيح i = low; i <= high; i++) {
                مزدوج z = 0.0;
                لكل (صحيح k = low; k <= std::min(j, high); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    فراغ orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        صحيح low = 0;
        صحيح high = n - 1;

        لكل (صحيح m = low + 1; m < high; m++) {

            // Scale column.

            مزدوج scale = 0.0;
            لكل (صحيح i = m; i <= high; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            ܐܢ (scale != 0.0) {

                // Compute Householder transformation.

                مزدوج h = 0.0;
                لكل (صحيح i = high; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                مزدوج g = std::sqrt(h);
                ܐܢ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                لكل (صحيح j = m; j < n; j++) {
                    مزدوج f = 0.0;
                    لكل (صحيح i = high; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    لكل (صحيح i = m; i <= high; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                لكل (صحيح i = 0; i <= high; i++) {
                    مزدوج f = 0.0;
                    لكل (صحيح j = high; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    لكل (صحيح j = m; j <= high; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        لكل (صحيح i = 0; i < n; i++) {
            لكل (صحيح j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        لكل (صحيح m = high - 1; m > low; m--) {
            ܐܢ (H[m][m - 1] != 0.0) {
                لكل (صحيح i = m + 1; i <= high; i++) {
                    ort[i] = H[i][m - 1];
                }
                لكل (صحيح j = m; j <= high; j++) {
                    مزدوج g = 0.0;
                    لكل (صحيح i = m; i <= high; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    لكل (صحيح i = m; i <= high; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    فراغ release() {
        // releases the working data
        delete[] d; d = عدم;
        delete[] e; e = عدم;
        delete[] ort; ort = عدم;
        لكل (صحيح i = 0; i < n; i++) {
            ܐܢ (H) delete[] H[i];
            ܐܢ (V) delete[] V[i];
        }
        delete[] H; H = عدم;
        delete[] V; V = عدم;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    فراغ compute() {
        // Allocate memory for the working data.
        V = alloc_2d<مزدوج> (n, n, 0.0);
        d = alloc_1d<مزدوج> (n);
        e = alloc_1d<مزدوج> (n);
        ort = alloc_1d<مزدوج> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            لكل (صحيح i = 0; i < n; i++) {
                _eigenvalues.at<مزدوج> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح j = 0; j < n; j++)
                    _eigenvectors.at<مزدوج> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

عام:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(عدم), e(عدم), ort(عدم),
        V(عدم), H(عدم)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    فراغ compute(InputArray src, منطقي fallbackSymmetric = ܫܪܪܐ)
    {
        CV_INSTRUMENT_REGION();

        ܐܢ(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ܐܚܪܢܐ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            هذا->n = tmp.cols;
            // Allocate the matrix data to work on.
            هذا->H = alloc_2d<مزدوج> (n, n);
            // Now safely copy the data.
            لكل (صحيح i = 0; i < tmp.rows; i++) {
                لكل (صحيح j = 0; j < tmp.cols; j++) {
                    هذا->H[i][j] = tmp.at<مزدوج>(i, j);
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
    Mat eigenvalues() ثابت { ܦܢܐ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ثابت { ܦܢܐ _eigenvectors; }
};

فراغ eigenNonSymmetric(InputArray _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحيح type = src.type();
    صحيح n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ܐܢ (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    ܐܚܪܢܐ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ܕܓܠܐ);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<مزدوج> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<صحيح> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<مزدوج> sorted_eigenvalues64f(n);
    لكل (صحيح i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    ܐܢ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<مزدوج> sorted_eigenvectors64f(n, n, CV_64FC1);
        لكل (صحيح i = 0; i < n; i++)
        {
            مزدوج* pDst = sorted_eigenvectors64f.ptr<مزدوج>(i);
            مزدوج* pSrc = eigenvectors64f.ptr<مزدوج>(sort_indexes[i]);
            CV_Assert(pSrc != عدم);
            memcpy(pDst, pSrc, n * حجم(مزدوج));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(صحيح num_components) : _num_components(num_components) { }

LDA::LDA(InputArrayOfArrays src, InputArray labels, صحيح num_components) : _num_components(num_components)
{
    هذا->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

فراغ LDA::save(ثابت String& filename) ثابت
{
    FileStorage fs(filename, FileStorage::WRITE);
    ܐܢ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    هذا->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
فراغ LDA::load(ثابت String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ܐܢ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    هذا->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
فراغ LDA::save(FileStorage& fs) ثابت {
    // write matrices
    fs << "num_components" << _num_components;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
فراغ LDA::load(ثابت FileStorage& fs) {
    //read matrices
    fs["num_components"] >> _num_components;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

فراغ LDA::lda(InputArrayOfArrays _src, InputArray _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<صحيح> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        لكل(unsigned صحيح i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<صحيح>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<صحيح> mapped_labels(labels.size());
    std::vector<صحيح> num2label = remove_dups(labels);
    std::map<صحيح, صحيح> label2num;
    لكل (صحيح i = 0; i < (صحيح)num2label.size(); i++)
        label2num[num2label[i]] = i;
    لكل (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    صحيح N = data.rows;
    صحيح D = data.cols;
    // number of unique labels
    صحيح C = (صحيح)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ܐܢ(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    ܐܢ (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    ܐܢ (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    ܐܢ ((_num_components <= 0) || (_num_components >= C)) {
        _num_components = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<صحيح> numClass(C);
    // initialize
    لكل (صحيح i = 0; i < C; i++) {
        numClass[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    لكل (صحيح i = 0; i < N; i++) {
        Mat instance = data.row(i);
        صحيح classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        numClass[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<مزدوج> (N));
    // calculate class means
    لكل (صحيح i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<مزدوج> (numClass[i]));
    }
    // subtract class means
    لكل (صحيح i = 0; i < N; i++) {
        صحيح classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, ܫܪܪܐ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    لكل (صحيح i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, ܫܪܪܐ);
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
    std::vector<صحيح> sorted_indices = argsort(_eigenvalues, ܕܓܠܐ);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, _num_components));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, _num_components));
}

فراغ LDA::compute(InputArrayOfArrays _src, InputArray _lbls) {
    بدل(_src.kind()) {
    حالة _InputArray::STD_VECTOR_MAT:
    حالة _InputArray::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        اكسر;
    حالة _InputArray::MAT:
        lda(_src.getMat(), _lbls);
        اكسر;
    افتراضي:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        اكسر;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(InputArray src) {
   ܦܢܐ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(InputArray src) {
   ܦܢܐ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
