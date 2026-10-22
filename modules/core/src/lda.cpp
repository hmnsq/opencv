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

#समावेश "precomp.hpp"
#समावेश <iostream>
#समावेश <map>
#समावेश <set>

namespace cv
{

// Removes duplicate elements in a given vector.
ढाँचा<typename _Tp>
अंतरभूत std::vector<_Tp> remove_dups(स्थिर std::vector<_Tp>& src) {
    प्रकार typename std::set<_Tp>::const_iterator constSetIterator;
    प्रकार typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    क्रम (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    क्रम (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    वापस elems;
}

अटल Mat argsort(निवेश_Array _src, bool ascending=true)
{
    Mat src = _src.getMat();
    अगर (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    पूर्णांक flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    वापस sorted_indices;
}

अटल Mat asRowMatrix(निवेश_Array_Of_Arrays src, पूर्णांक rtype, द्विगुन alpha=1, द्विगुन beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    अगर(src.kind() != निवेश_Array_2::STD_VECTOR_MAT && src.kind() != निवेश_Array_2::STD_ARRAY_MAT &&
        src.kind() != निवेश_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    माप_प्रकार n = src.योग();
    // return empty matrix if no matrices given
    अगर(n == 0)
        वापस Mat();
    // dimensionality of (reshaped) samples
    माप_प्रकार d = src.getMat(0).योग();
    // create data matrix
    Mat तथ्य((पूर्णांक)n, (पूर्णांक)d, rtype);
    // now copy data
    क्रम(पूर्णांक i = 0; i < (पूर्णांक)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        अगर(src.getMat(i).योग() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (पूर्णांक)d, (पूर्णांक)src.getMat(i).योग());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = तथ्य.row(i);
        // make reshape happy by cloning for non-continuous matrices
        अगर(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } अन्यथा {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    वापस तथ्य;
}

अटल व्योम sortMatrixColumnsByIndices(निवेश_Array _src, निवेश_Array _indices, OutputArray _dst) {
    अगर(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<पूर्णांक> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    क्रम(माप_प्रकार idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((पूर्णांक)idx);
        originalCol.copyTo(sortedCol);
    }
}

अटल Mat sortMatrixColumnsByIndices(निवेश_Array src, निवेश_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    वापस dst;
}


ढाँचा<typename _Tp> अटल bool
isSymmetric_(निवेश_Array src) {
    Mat _src = src.getMat();
    अगर(_src.cols != _src.rows)
        वापस false;
    क्रम (पूर्णांक i = 0; i < _src.rows; i++) {
        क्रम (पूर्णांक j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            अगर (a != b) {
                वापस false;
            }
        }
    }
    वापस true;
}

ढाँचा<typename _Tp> अटल bool
isSymmetric_(निवेश_Array src, द्विगुन eps) {
    Mat _src = src.getMat();
    अगर(_src.cols != _src.rows)
        वापस false;
    क्रम (पूर्णांक i = 0; i < _src.rows; i++) {
        क्रम (पूर्णांक j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            अगर (std::असल(a - b) > eps) {
                वापस false;
            }
        }
    }
    वापस true;
}

अटल bool isSymmetric(निवेश_Array src, द्विगुन eps=1e-16)
{
    Mat m = src.getMat();
    चयन (m.type()) {
        हाल CV_8SC1: वापस isSymmetric_<अक्षर>(m); अवरोध;
        हाल CV_8UC1:
            वापस isSymmetric_<अचिन्हित अक्षर>(m); अवरोध;
        हाल CV_16SC1:
            वापस isSymmetric_<लघु>(m); अवरोध;
        हाल CV_16UC1:
            वापस isSymmetric_<अचिन्हित लघु>(m); अवरोध;
        हाल CV_32SC1:
            वापस isSymmetric_<पूर्णांक>(m); अवरोध;
        हाल CV_32FC1:
            वापस isSymmetric_<भग्न>(m, eps); अवरोध;
        हाल CV_64FC1:
            वापस isSymmetric_<द्विगुन>(m, eps); अवरोध;
        शेष:
            अवरोध;
    }
    वापस false;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(निवेश_Array _W, निवेश_Array _mean, निवेश_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णांक n = src.rows;
    पूर्णांक d = src.cols;
    // make sure the data has the correct shape
    अगर(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    अगर(!mean.empty() && (mean.योग() != (माप_प्रकार) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.योग());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    अगर(!mean.empty()) {
        क्रम(पूर्णांक i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    वापस Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(निवेश_Array _W, निवेश_Array _mean, निवेश_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णांक n = src.rows;
    पूर्णांक d = src.cols;
    // make sure the data has the correct shape
    अगर(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    अगर(!mean.empty() && (mean.योग() != (माप_प्रकार) W.rows)) {
        String error_message = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.cols, mean.योग());
        CV_Error(Error::StsBadArg, error_message);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convertTo(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    अगर(!mean.empty()) {
        क्रम(पूर्णांक i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    वापस X;
}


श्रेणी EigenvalueDecomposition {
निजी:

    // Holds the data dimension.
    पूर्णांक n;

    // Pointer to internal memory.
    द्विगुन *d, *e, *ort;
    द्विगुन **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp *alloc_1d(पूर्णांक m) {
        वापस नया _Tp[m];
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp *alloc_1d(पूर्णांक m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        क्रम (पूर्णांक i = 0; i < m; i++)
            arr[i] = val;
        वापस arr;
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp **alloc_2d(पूर्णांक m, पूर्णांक _n) {
        _Tp **arr = नया _Tp*[m];
        क्रम (पूर्णांक i = 0; i < m; i++)
            arr[i] = नया _Tp[_n];
        वापस arr;
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp **alloc_2d(पूर्णांक m, पूर्णांक _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        क्रम (पूर्णांक i = 0; i < m; i++) {
            क्रम (पूर्णांक j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        वापस arr;
    }

    अटल व्योम complex_div(द्विगुन xr, द्विगुन xi, द्विगुन yr, द्विगुन yi, द्विगुन& cdivr, द्विगुन& cdivi) {
        द्विगुन r, dv;
        CV_DbgAssert(std::असल(yr) + std::असल(yi) > 0.0);
        अगर (std::असल(yr) > std::असल(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } अन्यथा {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    व्योम hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        स्थिर पूर्णांक max_iters_गिनती = 1000 * यह->n;

        स्थिर पूर्णांक nn = यह->n; CV_Assert(nn > 0);
        पूर्णांक n1 = nn - 1;
        स्थिर पूर्णांक निचला = 0;
        स्थिर पूर्णांक ऊपरी = nn - 1;
        स्थिर द्विगुन eps = std::numeric_limits<द्विगुन>::epsilon();
        द्विगुन exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        द्विगुन norm = 0.0;
        क्रम (पूर्णांक i = 0; i < nn; i++) {
#अगर 0 // 'if' condition is always false
            अगर (i < निचला || i > ऊपरी) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#पूर्ण_अगर
            क्रम (पूर्णांक j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::असल(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        पूर्णांक iter = 0;
        जबतक (n1 >= निचला) {

            // Look for single small sub-diagonal element
            पूर्णांक l = n1;
            जबतक (l > निचला) {
                अगर (norm < भग्न_अंतर) {
                    अवरोध;
                }
                द्विगुन s = std::असल(H[l - 1][l - 1]) + std::असल(H[l][l]);
                अगर (s == 0.0) {
                    s = norm;
                }
                अगर (std::असल(H[l][l - 1]) < eps * s) {
                    अवरोध;
                }
                l--;
            }

            // Check for convergence
            अगर (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } अन्यथा अगर (l == n1 - 1) {
                // Two roots found
                द्विगुन w = H[n1][n1 - 1] * H[n1 - 1][n1];
                द्विगुन p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                द्विगुन q = p * p + w;
                द्विगुन z = std::वर्ग_मूल(std::असल(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                द्विगुन x = H[n1][n1];

                अगर (q >= 0) {
                    // Real pair
                    अगर (p >= 0) {
                        z = p + z;
                    } अन्यथा {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    अगर (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    द्विगुन s = std::असल(x) + std::असल(z);
                    p = x / s;
                    q = z / s;
                    द्विगुन r = std::वर्ग_मूल(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    क्रम (पूर्णांक j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    क्रम (पूर्णांक i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    क्रम (पूर्णांक i = निचला; i <= ऊपरी; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } अन्यथा {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } अन्यथा {
                // No convergence yet

                // Form shift
                द्विगुन x = H[n1][n1];
                द्विगुन y = 0.0;
                द्विगुन w = 0.0;
                अगर (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                अगर (iter == 10) {
                    exshift += x;
                    क्रम (पूर्णांक i = निचला; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    द्विगुन s = std::असल(H[n1][n1 - 1]) + std::असल(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                अगर (iter == 30) {
                    द्विगुन s = (y - x) * 0.5;
                    s = s * s + w;
                    अगर (s > 0) {
                        s = std::वर्ग_मूल(s);
                        अगर (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        क्रम (पूर्णांक i = निचला; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                अगर (iter > max_iters_गिनती)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                द्विगुन p = std::numeric_limits<द्विगुन>::quiet_NaN();
                द्विगुन q = std::numeric_limits<द्विगुन>::quiet_NaN();
                द्विगुन r = std::numeric_limits<द्विगुन>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                पूर्णांक m = n1 - 2;
                जबतक (m >= l) {
                    द्विगुन z = H[m][m];
                    r = x - z;
                    द्विगुन s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::असल(p) + std::असल(q) + std::असल(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    अगर (m == l) {
                        अवरोध;
                    }
                    अगर (std::असल(H[m][m - 1]) * (std::असल(q) + std::असल(r)) < eps * (std::असल(p)
                                                                                     * (std::असल(H[m - 1][m - 1]) + std::असल(z) + std::असल(
                                                                                                                                           H[m + 1][m + 1])))) {
                        अवरोध;
                    }
                    m--;
                }

                क्रम (पूर्णांक i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    अगर (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                क्रम (पूर्णांक k = m; k < n1; k++) {

                    bool notlast = (k != n1 - 1);
                    अगर (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::असल(p) + std::असल(q) + std::असल(r);
                        अगर (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    अगर (x == 0.0) {
                        अवरोध;
                    }
                    द्विगुन s = std::वर्ग_मूल(p * p + q * q + r * r);
                    अगर (p < 0) {
                        s = -s;
                    }
                    अगर (s != 0) {
                        अगर (k != m) {
                            H[k][k - 1] = -s * x;
                        } अन्यथा अगर (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        द्विगुन z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        क्रम (पूर्णांक j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            अगर (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        क्रम (पूर्णांक i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            अगर (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        क्रम (पूर्णांक i = निचला; i <= ऊपरी; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            अगर (notlast) {
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

        अगर (norm < भग्न_अंतर) {
            वापस;
        }

        क्रम (n1 = nn - 1; n1 >= 0; n1--) {
            द्विगुन p = d[n1];
            द्विगुन q = e[n1];

            अगर (q == 0) {
                // Real vector
                द्विगुन z = std::numeric_limits<द्विगुन>::quiet_NaN();
                द्विगुन s = std::numeric_limits<द्विगुन>::quiet_NaN();

                पूर्णांक l = n1;
                H[n1][n1] = 1.0;
                क्रम (पूर्णांक i = n1 - 1; i >= 0; i--) {
                    द्विगुन w = H[i][i] - p;
                    द्विगुन r = 0.0;
                    क्रम (पूर्णांक j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    अगर (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } अन्यथा {
                        l = i;
                        अगर (e[i] == 0.0) {
                            अगर (w != 0.0) {
                                H[i][n1] = -r / w;
                            } अन्यथा {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } अन्यथा {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            द्विगुन x = H[i][i + 1];
                            द्विगुन y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            द्विगुन t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            अगर (std::असल(x) > std::असल(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } अन्यथा {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        द्विगुन t = std::असल(H[i][n1]);
                        अगर ((eps * t) * t > 1) {
                            द्विगुन inv_t = 1.0 / t;
                            क्रम (पूर्णांक j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } अन्यथा अगर (q < 0) {
                // Complex vector
                द्विगुन z = std::numeric_limits<द्विगुन>::quiet_NaN();
                द्विगुन r = std::numeric_limits<द्विगुन>::quiet_NaN();
                द्विगुन s = std::numeric_limits<द्विगुन>::quiet_NaN();

                पूर्णांक l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                अगर (std::असल(H[n1][n1 - 1]) > std::असल(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } अन्यथा {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                क्रम (पूर्णांक i = n1 - 2; i >= 0; i--) {
                    द्विगुन ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    क्रम (पूर्णांक j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    द्विगुन w = H[i][i] - p;

                    अगर (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } अन्यथा {
                        l = i;
                        अगर (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } अन्यथा {
                            // Solve complex equations

                            द्विगुन x = H[i][i + 1];
                            द्विगुन y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            अगर (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::असल(w) + std::असल(q) + std::असल(x)
                                                   + std::असल(y) + std::असल(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            अगर (std::असल(x) > (std::असल(z) + std::असल(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } अन्यथा {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        द्विगुन t = std::max(std::असल(H[i][n1 - 1]), std::असल(H[i][n1]));
                        अगर ((eps * t) * t > 1) {
                            क्रम (पूर्णांक j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#अगर 0 // 'if' condition is always false
        क्रम (पूर्णांक i = 0; i < nn; i++) {
            अगर (i < निचला || i > ऊपरी) {
                क्रम (पूर्णांक j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#पूर्ण_अगर

        // Back transformation to get eigenvectors of original matrix

        क्रम (पूर्णांक j = nn - 1; j >= निचला; j--) {
            क्रम (पूर्णांक i = निचला; i <= ऊपरी; i++) {
                द्विगुन z = 0.0;
                क्रम (पूर्णांक k = निचला; k <= std::min(j, ऊपरी); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    व्योम orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        पूर्णांक निचला = 0;
        पूर्णांक ऊपरी = n - 1;

        क्रम (पूर्णांक m = निचला + 1; m < ऊपरी; m++) {

            // Scale column.

            द्विगुन scale = 0.0;
            क्रम (पूर्णांक i = m; i <= ऊपरी; i++) {
                scale = scale + std::असल(H[i][m - 1]);
            }
            अगर (scale != 0.0) {

                // Compute Householder transformation.

                द्विगुन h = 0.0;
                क्रम (पूर्णांक i = ऊपरी; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                द्विगुन g = std::वर्ग_मूल(h);
                अगर (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                क्रम (पूर्णांक j = m; j < n; j++) {
                    द्विगुन f = 0.0;
                    क्रम (पूर्णांक i = ऊपरी; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    क्रम (पूर्णांक i = m; i <= ऊपरी; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                क्रम (पूर्णांक i = 0; i <= ऊपरी; i++) {
                    द्विगुन f = 0.0;
                    क्रम (पूर्णांक j = ऊपरी; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    क्रम (पूर्णांक j = m; j <= ऊपरी; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        क्रम (पूर्णांक i = 0; i < n; i++) {
            क्रम (पूर्णांक j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        क्रम (पूर्णांक m = ऊपरी - 1; m > निचला; m--) {
            अगर (H[m][m - 1] != 0.0) {
                क्रम (पूर्णांक i = m + 1; i <= ऊपरी; i++) {
                    ort[i] = H[i][m - 1];
                }
                क्रम (पूर्णांक j = m; j <= ऊपरी; j++) {
                    द्विगुन g = 0.0;
                    क्रम (पूर्णांक i = m; i <= ऊपरी; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    क्रम (पूर्णांक i = m; i <= ऊपरी; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    व्योम release() {
        // releases the working data
        मिटाओ[] d; d = शून्य;
        मिटाओ[] e; e = शून्य;
        मिटाओ[] ort; ort = शून्य;
        क्रम (पूर्णांक i = 0; i < n; i++) {
            अगर (H) मिटाओ[] H[i];
            अगर (V) मिटाओ[] V[i];
        }
        मिटाओ[] H; H = शून्य;
        मिटाओ[] V; V = शून्य;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    व्योम compute() {
        // Allocate memory for the working data.
        V = alloc_2d<द्विगुन> (n, n, 0.0);
        d = alloc_1d<द्विगुन> (n);
        e = alloc_1d<द्विगुन> (n);
        ort = alloc_1d<द्विगुन> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            क्रम (पूर्णांक i = 0; i < n; i++) {
                _eigenvalues.at<द्विगुन> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            क्रम (पूर्णांक i = 0; i < n; i++)
                क्रम (पूर्णांक j = 0; j < n; j++)
                    _eigenvectors.at<द्विगुन> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

खुला:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(शून्य), e(शून्य), ort(शून्य),
        V(शून्य), H(शून्य)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    व्योम compute(निवेश_Array src, bool fallbackSymmetric = true)
    {
        CV_INSTRUMENT_REGION();

        अगर(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } अन्यथा {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            यह->n = tmp.cols;
            // Allocate the matrix data to work on.
            यह->H = alloc_2d<द्विगुन> (n, n);
            // Now safely copy the data.
            क्रम (पूर्णांक i = 0; i < tmp.rows; i++) {
                क्रम (पूर्णांक j = 0; j < tmp.cols; j++) {
                    यह->H[i][j] = tmp.at<द्विगुन>(i, j);
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
    Mat eigenvalues() स्थिर { वापस _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() स्थिर { वापस _eigenvectors; }
};

व्योम eigenNonSymmetric(निवेश_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    पूर्णांक n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    अगर (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    अन्यथा
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, false);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<द्विगुन> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (माप_प्रकार)n);

    std::vector<पूर्णांक> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<द्विगुन> sorted_eigenvalues64f(n);
    क्रम (पूर्णांक i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    अगर( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<द्विगुन> sorted_eigenvectors64f(n, n, CV_64FC1);
        क्रम (पूर्णांक i = 0; i < n; i++)
        {
            द्विगुन* pDst = sorted_eigenvectors64f.ptr<द्विगुन>(i);
            द्विगुन* pSrc = eigenvectors64f.ptr<द्विगुन>(sort_indexes[i]);
            CV_Assert(pSrc != शून्य);
            स_नकल(pDst, pSrc, n * माप(द्विगुन));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(पूर्णांक अंक_components) : अंक_components_2(अंक_components) { }

LDA::LDA(निवेश_Array_Of_Arrays src, निवेश_Array labels, पूर्णांक अंक_components) : अंक_components_2(अंक_components)
{
    यह->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

व्योम LDA::save(स्थिर String& filename) स्थिर
{
    FileStorage fs(filename, FileStorage::WRITE);
    अगर (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    यह->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
व्योम LDA::load(स्थिर String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    अगर (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    यह->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
व्योम LDA::save(FileStorage& fs) स्थिर {
    // write matrices
    fs << "num_components" << अंक_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
व्योम LDA::load(स्थिर FileStorage& fs) {
    //read matrices
    fs["num_components"] >> अंक_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

व्योम LDA::lda(निवेश_Array_Of_Arrays _src, निवेश_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<पूर्णांक> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        क्रम(अचिन्हित पूर्णांक i = 0; i < tmp.योग(); i++) {
            labels.push_back(tmp.at<पूर्णांक>(i));
        }
    }
    // turn into row sampled matrix
    Mat तथ्य;
    // ensure working matrix is double precision
    src.convertTo(तथ्य, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<पूर्णांक> mapped_labels(labels.size());
    std::vector<पूर्णांक> num2label = remove_dups(labels);
    std::map<पूर्णांक, पूर्णांक> label2num;
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)num2label.size(); i++)
        label2num[num2label[i]] = i;
    क्रम (माप_प्रकार i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    पूर्णांक N = तथ्य.rows;
    पूर्णांक D = तथ्य.cols;
    // number of unique labels
    पूर्णांक C = (पूर्णांक)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    अगर(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    अगर (labels.size() != static_cast<माप_प्रकार>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    अगर (N < D) {
        std::अ_बाहर << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::प_पूर्ण;
    }
    // clip number of components to be a valid number
    अगर ((अंक_components_2 <= 0) || (अंक_components_2 >= C)) {
        अंक_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat mean_योग = Mat::zeros(1, D, तथ्य.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<पूर्णांक> अंक_Class(C);
    // initialize
    क्रम (पूर्णांक i = 0; i < C; i++) {
        अंक_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, तथ्य.type()); //! Dx1 image vector
    }
    // calculate sums
    क्रम (पूर्णांक i = 0; i < N; i++) {
        Mat instance = तथ्य.row(i);
        पूर्णांक classIdx = mapped_labels[i];
        add(mean_योग, instance, mean_योग);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        अंक_Class[classIdx]++;
    }
    // calculate total mean
    mean_योग.convertTo(mean_योग, mean_योग.type(), 1.0 / static_cast<द्विगुन> (N));
    // calculate class means
    क्रम (पूर्णांक i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<द्विगुन> (अंक_Class[i]));
    }
    // subtract class means
    क्रम (पूर्णांक i = 0; i < N; i++) {
        पूर्णांक classIdx = mapped_labels[i];
        Mat instance = तथ्य.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, तथ्य.type());
    mulTransposed(तथ्य, Sw, true);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, तथ्य.type());
    क्रम (पूर्णांक i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], mean_योग, tmp);
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
    std::vector<पूर्णांक> sorted_indices = argsort(_eigenvalues, false);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, अंक_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, अंक_components_2));
}

व्योम LDA::compute(निवेश_Array_Of_Arrays _src, निवेश_Array _lbls) {
    चयन(_src.kind()) {
    हाल निवेश_Array_2::STD_VECTOR_MAT:
    हाल निवेश_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        अवरोध;
    हाल निवेश_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        अवरोध;
    शेष:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        अवरोध;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(निवेश_Array src) {
   वापस subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(निवेश_Array src) {
   वापस subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
