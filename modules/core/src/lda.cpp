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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <iostream>
#અન્તર્ભાવ <map>
#અન્તર્ભાવ <set>

નામાકાશ cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
અંતઃરેખ std::vector<_Tp> remove_dups(અચલ std::vector<_Tp>& src) {
    પ્રકારવ્યાખ્યા typename std::set<_Tp>::const_iterator constSetIterator;
    પ્રકારવ્યાખ્યા typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    માટે (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    માટે (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    પાછા elems;
}

સ્થિર Mat argsort(આગત_Array _src, તાર્કિક ascending=સાચું)
{
    Mat src = _src.getMat();
    જો (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    પૂર્ણાંક flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    પાછા sorted_indices;
}

સ્થિર Mat asRowMatrix(આગત_Array_Of_Arrays src, પૂર્ણાંક rtype, દ્વિગુણ alpha=1, દ્વિગુણ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    જો(src.kind() != આગત_Array_2::STD_VECTOR_MAT && src.kind() != આગત_Array_2::STD_ARRAY_MAT &&
        src.kind() != આગત_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    જો(n == 0)
        પાછા Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((પૂર્ણાંક)n, (પૂર્ણાંક)d, rtype);
    // now copy data
    માટે(પૂર્ણાંક i = 0; i < (પૂર્ણાંક)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        જો(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (પૂર્ણાંક)d, (પૂર્ણાંક)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        જો(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } નહીંતર {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    પાછા data;
}

સ્થિર શૂન્ય sortMatrixColumnsByIndices(આગત_Array _src, આગત_Array _indices, OutputArray _dst) {
    જો(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<પૂર્ણાંક> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    માટે(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((પૂર્ણાંક)idx);
        originalCol.copyTo(sortedCol);
    }
}

સ્થિર Mat sortMatrixColumnsByIndices(આગત_Array src, આગત_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    પાછા dst;
}


template<typename _Tp> સ્થિર તાર્કિક
isSymmetric_(આગત_Array src) {
    Mat _src = src.getMat();
    જો(_src.cols != _src.rows)
        પાછા ખોટું;
    માટે (પૂર્ણાંક i = 0; i < _src.rows; i++) {
        માટે (પૂર્ણાંક j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            જો (a != b) {
                પાછા ખોટું;
            }
        }
    }
    પાછા સાચું;
}

template<typename _Tp> સ્થિર તાર્કિક
isSymmetric_(આગત_Array src, દ્વિગુણ eps) {
    Mat _src = src.getMat();
    જો(_src.cols != _src.rows)
        પાછા ખોટું;
    માટે (પૂર્ણાંક i = 0; i < _src.rows; i++) {
        માટે (પૂર્ણાંક j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            જો (std::abs(a - b) > eps) {
                પાછા ખોટું;
            }
        }
    }
    પાછા સાચું;
}

સ્થિર તાર્કિક isSymmetric(આગત_Array src, દ્વિગુણ eps=1e-16)
{
    Mat m = src.getMat();
    પસંદગી (m.type()) {
        સ્થિતિ CV_8SC1: પાછા isSymmetric_<અક્ષર>(m); તોડો;
        સ્થિતિ CV_8UC1:
            પાછા isSymmetric_<unsigned અક્ષર>(m); તોડો;
        સ્થિતિ CV_16SC1:
            પાછા isSymmetric_<short>(m); તોડો;
        સ્થિતિ CV_16UC1:
            પાછા isSymmetric_<unsigned short>(m); તોડો;
        સ્થિતિ CV_32SC1:
            પાછા isSymmetric_<પૂર્ણાંક>(m); તોડો;
        સ્થિતિ CV_32FC1:
            પાછા isSymmetric_<દશાંશ>(m, eps); તોડો;
        સ્થિતિ CV_64FC1:
            પાછા isSymmetric_<દ્વિગુણ>(m, eps); તોડો;
        મૂળભૂત:
            તોડો;
    }
    પાછા ખોટું;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(આગત_Array _W, આગત_Array _mean, આગત_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    પૂર્ણાંક n = src.rows;
    પૂર્ણાંક d = src.cols;
    // make sure the data has the correct shape
    જો(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    જો(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    જો(!mean.empty()) {
        માટે(પૂર્ણાંક i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    પાછા Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(આગત_Array _W, આગત_Array _mean, આગત_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    પૂર્ણાંક n = src.rows;
    પૂર્ણાંક d = src.cols;
    // make sure the data has the correct shape
    જો(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    જો(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    જો(!mean.empty()) {
        માટે(પૂર્ણાંક i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    પાછા X;
}


વર્ગ EigenvalueDecomposition {
ખાનગી:

    // Holds the data dimension.
    પૂર્ણાંક n;

    // Pointer to internal memory.
    દ્વિગુણ *d, *e, *ort;
    દ્વિગુણ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(પૂર્ણાંક m) {
        પાછા નવું _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(પૂર્ણાંક m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        માટે (પૂર્ણાંક i = 0; i < m; i++)
            arr[i] = val;
        પાછા arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(પૂર્ણાંક m, પૂર્ણાંક _n) {
        _Tp **arr = નવું _Tp*[m];
        માટે (પૂર્ણાંક i = 0; i < m; i++)
            arr[i] = નવું _Tp[_n];
        પાછા arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(પૂર્ણાંક m, પૂર્ણાંક _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        માટે (પૂર્ણાંક i = 0; i < m; i++) {
            માટે (પૂર્ણાંક j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        પાછા arr;
    }

    સ્થિર શૂન્ય complex_div(દ્વિગુણ xr, દ્વિગુણ xi, દ્વિગુણ yr, દ્વિગુણ yi, દ્વિગુણ& cdivr, દ્વિગુણ& cdivi) {
        દ્વિગુણ r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        જો (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } નહીંતર {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    શૂન્ય hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        અચલ પૂર્ણાંક max_iters_ગણતરી = 1000 * આ->n;

        અચલ પૂર્ણાંક nn = આ->n; CV_Assert(nn > 0);
        પૂર્ણાંક n1 = nn - 1;
        અચલ પૂર્ણાંક નીચલું = 0;
        અચલ પૂર્ણાંક ઉપલું = nn - 1;
        અચલ દ્વિગુણ eps = std::numeric_limits<દ્વિગુણ>::epsilon();
        દ્વિગુણ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        દ્વિગુણ norm = 0.0;
        માટે (પૂર્ણાંક i = 0; i < nn; i++) {
#જો 0 // 'if' condition is always false
            જો (i < નીચલું || i > ઉપલું) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            માટે (પૂર્ણાંક j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        પૂર્ણાંક iter = 0;
        જ્યારે (n1 >= નીચલું) {

            // Look for single small sub-diagonal element
            પૂર્ણાંક l = n1;
            જ્યારે (l > નીચલું) {
                જો (norm < FLT_EPSILON) {
                    તોડો;
                }
                દ્વિગુણ s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                જો (s == 0.0) {
                    s = norm;
                }
                જો (std::abs(H[l][l - 1]) < eps * s) {
                    તોડો;
                }
                l--;
            }

            // Check for convergence
            જો (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } નહીંતર જો (l == n1 - 1) {
                // Two roots found
                દ્વિગુણ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                દ્વિગુણ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                દ્વિગુણ q = p * p + w;
                દ્વિગુણ z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                દ્વિગુણ x = H[n1][n1];

                જો (q >= 0) {
                    // Real pair
                    જો (p >= 0) {
                        z = p + z;
                    } નહીંતર {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    જો (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    દ્વિગુણ s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    દ્વિગુણ r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    માટે (પૂર્ણાંક j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    માટે (પૂર્ણાંક i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    માટે (પૂર્ણાંક i = નીચલું; i <= ઉપલું; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } નહીંતર {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } નહીંતર {
                // No convergence yet

                // Form shift
                દ્વિગુણ x = H[n1][n1];
                દ્વિગુણ y = 0.0;
                દ્વિગુણ w = 0.0;
                જો (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                જો (iter == 10) {
                    exshift += x;
                    માટે (પૂર્ણાંક i = નીચલું; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    દ્વિગુણ s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                જો (iter == 30) {
                    દ્વિગુણ s = (y - x) * 0.5;
                    s = s * s + w;
                    જો (s > 0) {
                        s = std::sqrt(s);
                        જો (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        માટે (પૂર્ણાંક i = નીચલું; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                જો (iter > max_iters_ગણતરી)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                દ્વિગુણ p = std::numeric_limits<દ્વિગુણ>::quiet_NaN();
                દ્વિગુણ q = std::numeric_limits<દ્વિગુણ>::quiet_NaN();
                દ્વિગુણ r = std::numeric_limits<દ્વિગુણ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                પૂર્ણાંક m = n1 - 2;
                જ્યારે (m >= l) {
                    દ્વિગુણ z = H[m][m];
                    r = x - z;
                    દ્વિગુણ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    જો (m == l) {
                        તોડો;
                    }
                    જો (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        તોડો;
                    }
                    m--;
                }

                માટે (પૂર્ણાંક i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    જો (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                માટે (પૂર્ણાંક k = m; k < n1; k++) {

                    તાર્કિક notlast = (k != n1 - 1);
                    જો (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        જો (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    જો (x == 0.0) {
                        તોડો;
                    }
                    દ્વિગુણ s = std::sqrt(p * p + q * q + r * r);
                    જો (p < 0) {
                        s = -s;
                    }
                    જો (s != 0) {
                        જો (k != m) {
                            H[k][k - 1] = -s * x;
                        } નહીંતર જો (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        દ્વિગુણ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        માટે (પૂર્ણાંક j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            જો (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        માટે (પૂર્ણાંક i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            જો (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        માટે (પૂર્ણાંક i = નીચલું; i <= ઉપલું; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            જો (notlast) {
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

        જો (norm < FLT_EPSILON) {
            પાછા;
        }

        માટે (n1 = nn - 1; n1 >= 0; n1--) {
            દ્વિગુણ p = d[n1];
            દ્વિગુણ q = e[n1];

            જો (q == 0) {
                // Real vector
                દ્વિગુણ z = std::numeric_limits<દ્વિગુણ>::quiet_NaN();
                દ્વિગુણ s = std::numeric_limits<દ્વિગુણ>::quiet_NaN();

                પૂર્ણાંક l = n1;
                H[n1][n1] = 1.0;
                માટે (પૂર્ણાંક i = n1 - 1; i >= 0; i--) {
                    દ્વિગુણ w = H[i][i] - p;
                    દ્વિગુણ r = 0.0;
                    માટે (પૂર્ણાંક j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    જો (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } નહીંતર {
                        l = i;
                        જો (e[i] == 0.0) {
                            જો (w != 0.0) {
                                H[i][n1] = -r / w;
                            } નહીંતર {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } નહીંતર {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            દ્વિગુણ x = H[i][i + 1];
                            દ્વિગુણ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            દ્વિગુણ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            જો (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } નહીંતર {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        દ્વિગુણ t = std::abs(H[i][n1]);
                        જો ((eps * t) * t > 1) {
                            દ્વિગુણ inv_t = 1.0 / t;
                            માટે (પૂર્ણાંક j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } નહીંતર જો (q < 0) {
                // Complex vector
                દ્વિગુણ z = std::numeric_limits<દ્વિગુણ>::quiet_NaN();
                દ્વિગુણ r = std::numeric_limits<દ્વિગુણ>::quiet_NaN();
                દ્વિગુણ s = std::numeric_limits<દ્વિગુણ>::quiet_NaN();

                પૂર્ણાંક l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                જો (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } નહીંતર {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                માટે (પૂર્ણાંક i = n1 - 2; i >= 0; i--) {
                    દ્વિગુણ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    માટે (પૂર્ણાંક j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    દ્વિગુણ w = H[i][i] - p;

                    જો (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } નહીંતર {
                        l = i;
                        જો (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } નહીંતર {
                            // Solve complex equations

                            દ્વિગુણ x = H[i][i + 1];
                            દ્વિગુણ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            જો (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            જો (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } નહીંતર {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        દ્વિગુણ t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        જો ((eps * t) * t > 1) {
                            માટે (પૂર્ણાંક j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#જો 0 // 'if' condition is always false
        માટે (પૂર્ણાંક i = 0; i < nn; i++) {
            જો (i < નીચલું || i > ઉપલું) {
                માટે (પૂર્ણાંક j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        માટે (પૂર્ણાંક j = nn - 1; j >= નીચલું; j--) {
            માટે (પૂર્ણાંક i = નીચલું; i <= ઉપલું; i++) {
                દ્વિગુણ z = 0.0;
                માટે (પૂર્ણાંક k = નીચલું; k <= std::min(j, ઉપલું); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    શૂન્ય orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        પૂર્ણાંક નીચલું = 0;
        પૂર્ણાંક ઉપલું = n - 1;

        માટે (પૂર્ણાંક m = નીચલું + 1; m < ઉપલું; m++) {

            // Scale column.

            દ્વિગુણ scale = 0.0;
            માટે (પૂર્ણાંક i = m; i <= ઉપલું; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            જો (scale != 0.0) {

                // Compute Householder transformation.

                દ્વિગુણ h = 0.0;
                માટે (પૂર્ણાંક i = ઉપલું; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                દ્વિગુણ g = std::sqrt(h);
                જો (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                માટે (પૂર્ણાંક j = m; j < n; j++) {
                    દ્વિગુણ f = 0.0;
                    માટે (પૂર્ણાંક i = ઉપલું; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    માટે (પૂર્ણાંક i = m; i <= ઉપલું; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                માટે (પૂર્ણાંક i = 0; i <= ઉપલું; i++) {
                    દ્વિગુણ f = 0.0;
                    માટે (પૂર્ણાંક j = ઉપલું; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    માટે (પૂર્ણાંક j = m; j <= ઉપલું; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            માટે (પૂર્ણાંક j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        માટે (પૂર્ણાંક m = ઉપલું - 1; m > નીચલું; m--) {
            જો (H[m][m - 1] != 0.0) {
                માટે (પૂર્ણાંક i = m + 1; i <= ઉપલું; i++) {
                    ort[i] = H[i][m - 1];
                }
                માટે (પૂર્ણાંક j = m; j <= ઉપલું; j++) {
                    દ્વિગુણ g = 0.0;
                    માટે (પૂર્ણાંક i = m; i <= ઉપલું; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    માટે (પૂર્ણાંક i = m; i <= ઉપલું; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    શૂન્ય release() {
        // releases the working data
        delete[] d; d = ખાલી;
        delete[] e; e = ખાલી;
        delete[] ort; ort = ખાલી;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            જો (H) delete[] H[i];
            જો (V) delete[] V[i];
        }
        delete[] H; H = ખાલી;
        delete[] V; V = ખાલી;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    શૂન્ય compute() {
        // Allocate memory for the working data.
        V = alloc_2d<દ્વિગુણ> (n, n, 0.0);
        d = alloc_1d<દ્વિગુણ> (n);
        e = alloc_1d<દ્વિગુણ> (n);
        ort = alloc_1d<દ્વિગુણ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            માટે (પૂર્ણાંક i = 0; i < n; i++) {
                _eigenvalues.at<દ્વિગુણ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            માટે (પૂર્ણાંક i = 0; i < n; i++)
                માટે (પૂર્ણાંક j = 0; j < n; j++)
                    _eigenvectors.at<દ્વિગુણ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

જાહેર:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ખાલી), e(ખાલી), ort(ખાલી),
        V(ખાલી), H(ખાલી)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    શૂન્ય compute(આગત_Array src, તાર્કિક fallbackSymmetric = સાચું)
    {
        CV_INSTRUMENT_REGION();

        જો(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } નહીંતર {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            આ->n = tmp.cols;
            // Allocate the matrix data to work on.
            આ->H = alloc_2d<દ્વિગુણ> (n, n);
            // Now safely copy the data.
            માટે (પૂર્ણાંક i = 0; i < tmp.rows; i++) {
                માટે (પૂર્ણાંક j = 0; j < tmp.cols; j++) {
                    આ->H[i][j] = tmp.at<દ્વિગુણ>(i, j);
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
    Mat eigenvalues() અચલ { પાછા _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() અચલ { પાછા _eigenvectors; }
};

શૂન્ય eigenNonSymmetric(આગત_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    પૂર્ણાંક type = src.type();
    પૂર્ણાંક n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    જો (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    નહીંતર
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ખોટું);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<દ્વિગુણ> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<પૂર્ણાંક> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<દ્વિગુણ> sorted_eigenvalues64f(n);
    માટે (પૂર્ણાંક i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    જો( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<દ્વિગુણ> sorted_eigenvectors64f(n, n, CV_64FC1);
        માટે (પૂર્ણાંક i = 0; i < n; i++)
        {
            દ્વિગુણ* pDst = sorted_eigenvectors64f.ptr<દ્વિગુણ>(i);
            દ્વિગુણ* pSrc = eigenvectors64f.ptr<દ્વિગુણ>(sort_indexes[i]);
            CV_Assert(pSrc != ખાલી);
            memcpy(pDst, pSrc, n * કદ(દ્વિગુણ));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(પૂર્ણાંક અંક_components) : અંક_components_2(અંક_components) { }

LDA::LDA(આગત_Array_Of_Arrays src, આગત_Array labels, પૂર્ણાંક અંક_components) : અંક_components_2(અંક_components)
{
    આ->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

શૂન્ય LDA::save(અચલ String& filename) અચલ
{
    FileStorage fs(filename, FileStorage::WRITE);
    જો (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    આ->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
શૂન્ય LDA::load(અચલ String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    જો (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    આ->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
શૂન્ય LDA::save(FileStorage& fs) અચલ {
    // write matrices
    fs << "num_components" << અંક_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
શૂન્ય LDA::load(અચલ FileStorage& fs) {
    //read matrices
    fs["num_components"] >> અંક_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

શૂન્ય LDA::lda(આગત_Array_Of_Arrays _src, આગત_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<પૂર્ણાંક> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        માટે(unsigned પૂર્ણાંક i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<પૂર્ણાંક>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<પૂર્ણાંક> mapped_labels(labels.size());
    std::vector<પૂર્ણાંક> num2label = remove_dups(labels);
    std::map<પૂર્ણાંક, પૂર્ણાંક> label2num;
    માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)num2label.size(); i++)
        label2num[num2label[i]] = i;
    માટે (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    પૂર્ણાંક N = data.rows;
    પૂર્ણાંક D = data.cols;
    // number of unique labels
    પૂર્ણાંક C = (પૂર્ણાંક)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    જો(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    જો (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    જો (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    જો ((અંક_components_2 <= 0) || (અંક_components_2 >= C)) {
        અંક_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<પૂર્ણાંક> અંક_Class(C);
    // initialize
    માટે (પૂર્ણાંક i = 0; i < C; i++) {
        અંક_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    માટે (પૂર્ણાંક i = 0; i < N; i++) {
        Mat instance = data.row(i);
        પૂર્ણાંક classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        અંક_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<દ્વિગુણ> (N));
    // calculate class means
    માટે (પૂર્ણાંક i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<દ્વિગુણ> (અંક_Class[i]));
    }
    // subtract class means
    માટે (પૂર્ણાંક i = 0; i < N; i++) {
        પૂર્ણાંક classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, સાચું);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    માટે (પૂર્ણાંક i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, સાચું);
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
    std::vector<પૂર્ણાંક> sorted_indices = argsort(_eigenvalues, ખોટું);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, અંક_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, અંક_components_2));
}

શૂન્ય LDA::compute(આગત_Array_Of_Arrays _src, આગત_Array _lbls) {
    પસંદગી(_src.kind()) {
    સ્થિતિ આગત_Array_2::STD_VECTOR_MAT:
    સ્થિતિ આગત_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        તોડો;
    સ્થિતિ આગત_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        તોડો;
    મૂળભૂત:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        તોડો;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(આગત_Array src) {
   પાછા subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(આગત_Array src) {
   પાછા subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
