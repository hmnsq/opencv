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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <iostream>
#അന്തര്ഭാവ <map>
#അന്തര്ഭാവ <set>

നാമാകാശ cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
വരിയിൽ std::vector<_Tp> remove_dups(സ്ഥിരം std::vector<_Tp>& src) {
    തരനിർവചനം typename std::set<_Tp>::const_iterator constSetIterator;
    തരനിർവചനം typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    വേണ്ടി (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    വേണ്ടി (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    തിരികെ elems;
}

സ്ഥിതം Mat argsort(നിവേശം_Array _src, തർക്കം ascending=സത്യം)
{
    Mat src = _src.getMat();
    എങ്കിൽ (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    പൂർണ്ണസംഖ്യ flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    തിരികെ sorted_indices;
}

സ്ഥിതം Mat asRowMatrix(നിവേശം_Array_Of_Arrays src, പൂർണ്ണസംഖ്യ rtype, ഇരട്ട alpha=1, ഇരട്ട beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    എങ്കിൽ(src.kind() != നിവേശം_Array_2::STD_VECTOR_MAT && src.kind() != നിവേശം_Array_2::STD_ARRAY_MAT &&
        src.kind() != നിവേശം_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    എങ്കിൽ(n == 0)
        തിരികെ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((പൂർണ്ണസംഖ്യ)n, (പൂർണ്ണസംഖ്യ)d, rtype);
    // now copy data
    വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        എങ്കിൽ(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (പൂർണ്ണസംഖ്യ)d, (പൂർണ്ണസംഖ്യ)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        എങ്കിൽ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } അല്ലെങ്കിൽ {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    തിരികെ data;
}

സ്ഥിതം ശൂന്യം sortMatrixColumnsByIndices(നിവേശം_Array _src, നിവേശം_Array _indices, OutputArray _dst) {
    എങ്കിൽ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<പൂർണ്ണസംഖ്യ> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    വേണ്ടി(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((പൂർണ്ണസംഖ്യ)idx);
        originalCol.copyTo(sortedCol);
    }
}

സ്ഥിതം Mat sortMatrixColumnsByIndices(നിവേശം_Array src, നിവേശം_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    തിരികെ dst;
}


template<typename _Tp> സ്ഥിതം തർക്കം
isSymmetric_(നിവേശം_Array src) {
    Mat _src = src.getMat();
    എങ്കിൽ(_src.cols != _src.rows)
        തിരികെ അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _src.rows; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            എങ്കിൽ (a != b) {
                തിരികെ അസത്യം;
            }
        }
    }
    തിരികെ സത്യം;
}

template<typename _Tp> സ്ഥിതം തർക്കം
isSymmetric_(നിവേശം_Array src, ഇരട്ട eps) {
    Mat _src = src.getMat();
    എങ്കിൽ(_src.cols != _src.rows)
        തിരികെ അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _src.rows; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            എങ്കിൽ (std::abs(a - b) > eps) {
                തിരികെ അസത്യം;
            }
        }
    }
    തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം isSymmetric(നിവേശം_Array src, ഇരട്ട eps=1e-16)
{
    Mat m = src.getMat();
    തിരഞ്ഞെടുപ്പ് (m.type()) {
        അവസ്ഥ CV_8SC1: തിരികെ isSymmetric_<അക്ഷരം>(m); നിർത്തുക;
        അവസ്ഥ CV_8UC1:
            തിരികെ isSymmetric_<unsigned അക്ഷരം>(m); നിർത്തുക;
        അവസ്ഥ CV_16SC1:
            തിരികെ isSymmetric_<short>(m); നിർത്തുക;
        അവസ്ഥ CV_16UC1:
            തിരികെ isSymmetric_<unsigned short>(m); നിർത്തുക;
        അവസ്ഥ CV_32SC1:
            തിരികെ isSymmetric_<പൂർണ്ണസംഖ്യ>(m); നിർത്തുക;
        അവസ്ഥ CV_32FC1:
            തിരികെ isSymmetric_<ദശാംശം>(m, eps); നിർത്തുക;
        അവസ്ഥ CV_64FC1:
            തിരികെ isSymmetric_<ഇരട്ട>(m, eps); നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
    }
    തിരികെ അസത്യം;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(നിവേശം_Array _W, നിവേശം_Array _mean, നിവേശം_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    പൂർണ്ണസംഖ്യ n = src.rows;
    പൂർണ്ണസംഖ്യ d = src.cols;
    // make sure the data has the correct shape
    എങ്കിൽ(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    എങ്കിൽ(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    എങ്കിൽ(!mean.empty()) {
        വേണ്ടി(പൂർണ്ണസംഖ്യ i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    തിരികെ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(നിവേശം_Array _W, നിവേശം_Array _mean, നിവേശം_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    പൂർണ്ണസംഖ്യ n = src.rows;
    പൂർണ്ണസംഖ്യ d = src.cols;
    // make sure the data has the correct shape
    എങ്കിൽ(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    എങ്കിൽ(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    എങ്കിൽ(!mean.empty()) {
        വേണ്ടി(പൂർണ്ണസംഖ്യ i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    തിരികെ X;
}


വർഗ്ഗം EigenvalueDecomposition {
സ്വകാര്യ:

    // Holds the data dimension.
    പൂർണ്ണസംഖ്യ n;

    // Pointer to internal memory.
    ഇരട്ട *d, *e, *ort;
    ഇരട്ട **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(പൂർണ്ണസംഖ്യ m) {
        തിരികെ പുതിയ _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(പൂർണ്ണസംഖ്യ m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m; i++)
            arr[i] = val;
        തിരികെ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ _n) {
        _Tp **arr = പുതിയ _Tp*[m];
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m; i++)
            arr[i] = പുതിയ _Tp[_n];
        തിരികെ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m; i++) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        തിരികെ arr;
    }

    സ്ഥിതം ശൂന്യം complex_div(ഇരട്ട xr, ഇരട്ട xi, ഇരട്ട yr, ഇരട്ട yi, ഇരട്ട& cdivr, ഇരട്ട& cdivi) {
        ഇരട്ട r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        എങ്കിൽ (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } അല്ലെങ്കിൽ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    ശൂന്യം hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        സ്ഥിരം പൂർണ്ണസംഖ്യ max_iters_എണ്ണം = 1000 * ഇത്->n;

        സ്ഥിരം പൂർണ്ണസംഖ്യ nn = ഇത്->n; CV_Assert(nn > 0);
        പൂർണ്ണസംഖ്യ n1 = nn - 1;
        സ്ഥിരം പൂർണ്ണസംഖ്യ താഴെയുള്ള = 0;
        സ്ഥിരം പൂർണ്ണസംഖ്യ മുകളിലെ = nn - 1;
        സ്ഥിരം ഇരട്ട eps = std::numeric_limits<ഇരട്ട>::epsilon();
        ഇരട്ട exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ഇരട്ട norm = 0.0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nn; i++) {
#എങ്കിൽ 0 // 'if' condition is always false
            എങ്കിൽ (i < താഴെയുള്ള || i > മുകളിലെ) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        പൂർണ്ണസംഖ്യ iter = 0;
        ആകുമ്പോൾ (n1 >= താഴെയുള്ള) {

            // Look for single small sub-diagonal element
            പൂർണ്ണസംഖ്യ l = n1;
            ആകുമ്പോൾ (l > താഴെയുള്ള) {
                എങ്കിൽ (norm < FLT_EPSILON) {
                    നിർത്തുക;
                }
                ഇരട്ട s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                എങ്കിൽ (s == 0.0) {
                    s = norm;
                }
                എങ്കിൽ (std::abs(H[l][l - 1]) < eps * s) {
                    നിർത്തുക;
                }
                l--;
            }

            // Check for convergence
            എങ്കിൽ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } അല്ലെങ്കിൽ എങ്കിൽ (l == n1 - 1) {
                // Two roots found
                ഇരട്ട w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ഇരട്ട p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ഇരട്ട q = p * p + w;
                ഇരട്ട z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ഇരട്ട x = H[n1][n1];

                എങ്കിൽ (q >= 0) {
                    // Real pair
                    എങ്കിൽ (p >= 0) {
                        z = p + z;
                    } അല്ലെങ്കിൽ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    എങ്കിൽ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ഇരട്ട s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    ഇരട്ട r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = താഴെയുള്ള; i <= മുകളിലെ; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } അല്ലെങ്കിൽ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } അല്ലെങ്കിൽ {
                // No convergence yet

                // Form shift
                ഇരട്ട x = H[n1][n1];
                ഇരട്ട y = 0.0;
                ഇരട്ട w = 0.0;
                എങ്കിൽ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                എങ്കിൽ (iter == 10) {
                    exshift += x;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = താഴെയുള്ള; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ഇരട്ട s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                എങ്കിൽ (iter == 30) {
                    ഇരട്ട s = (y - x) * 0.5;
                    s = s * s + w;
                    എങ്കിൽ (s > 0) {
                        s = std::sqrt(s);
                        എങ്കിൽ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        വേണ്ടി (പൂർണ്ണസംഖ്യ i = താഴെയുള്ള; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                എങ്കിൽ (iter > max_iters_എണ്ണം)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ഇരട്ട p = std::numeric_limits<ഇരട്ട>::quiet_NaN();
                ഇരട്ട q = std::numeric_limits<ഇരട്ട>::quiet_NaN();
                ഇരട്ട r = std::numeric_limits<ഇരട്ട>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                പൂർണ്ണസംഖ്യ m = n1 - 2;
                ആകുമ്പോൾ (m >= l) {
                    ഇരട്ട z = H[m][m];
                    r = x - z;
                    ഇരട്ട s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    എങ്കിൽ (m == l) {
                        നിർത്തുക;
                    }
                    എങ്കിൽ (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        നിർത്തുക;
                    }
                    m--;
                }

                വേണ്ടി (പൂർണ്ണസംഖ്യ i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    എങ്കിൽ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                വേണ്ടി (പൂർണ്ണസംഖ്യ k = m; k < n1; k++) {

                    തർക്കം notlast = (k != n1 - 1);
                    എങ്കിൽ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        എങ്കിൽ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    എങ്കിൽ (x == 0.0) {
                        നിർത്തുക;
                    }
                    ഇരട്ട s = std::sqrt(p * p + q * q + r * r);
                    എങ്കിൽ (p < 0) {
                        s = -s;
                    }
                    എങ്കിൽ (s != 0) {
                        എങ്കിൽ (k != m) {
                            H[k][k - 1] = -s * x;
                        } അല്ലെങ്കിൽ എങ്കിൽ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ഇരട്ട z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        വേണ്ടി (പൂർണ്ണസംഖ്യ j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            എങ്കിൽ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            എങ്കിൽ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        വേണ്ടി (പൂർണ്ണസംഖ്യ i = താഴെയുള്ള; i <= മുകളിലെ; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            എങ്കിൽ (notlast) {
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

        എങ്കിൽ (norm < FLT_EPSILON) {
            തിരികെ;
        }

        വേണ്ടി (n1 = nn - 1; n1 >= 0; n1--) {
            ഇരട്ട p = d[n1];
            ഇരട്ട q = e[n1];

            എങ്കിൽ (q == 0) {
                // Real vector
                ഇരട്ട z = std::numeric_limits<ഇരട്ട>::quiet_NaN();
                ഇരട്ട s = std::numeric_limits<ഇരട്ട>::quiet_NaN();

                പൂർണ്ണസംഖ്യ l = n1;
                H[n1][n1] = 1.0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = n1 - 1; i >= 0; i--) {
                    ഇരട്ട w = H[i][i] - p;
                    ഇരട്ട r = 0.0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    എങ്കിൽ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } അല്ലെങ്കിൽ {
                        l = i;
                        എങ്കിൽ (e[i] == 0.0) {
                            എങ്കിൽ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } അല്ലെങ്കിൽ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } അല്ലെങ്കിൽ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ഇരട്ട x = H[i][i + 1];
                            ഇരട്ട y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ഇരട്ട t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            എങ്കിൽ (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } അല്ലെങ്കിൽ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ഇരട്ട t = std::abs(H[i][n1]);
                        എങ്കിൽ ((eps * t) * t > 1) {
                            ഇരട്ട inv_t = 1.0 / t;
                            വേണ്ടി (പൂർണ്ണസംഖ്യ j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (q < 0) {
                // Complex vector
                ഇരട്ട z = std::numeric_limits<ഇരട്ട>::quiet_NaN();
                ഇരട്ട r = std::numeric_limits<ഇരട്ട>::quiet_NaN();
                ഇരട്ട s = std::numeric_limits<ഇരട്ട>::quiet_NaN();

                പൂർണ്ണസംഖ്യ l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                എങ്കിൽ (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } അല്ലെങ്കിൽ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = n1 - 2; i >= 0; i--) {
                    ഇരട്ട ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ഇരട്ട w = H[i][i] - p;

                    എങ്കിൽ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } അല്ലെങ്കിൽ {
                        l = i;
                        എങ്കിൽ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } അല്ലെങ്കിൽ {
                            // Solve complex equations

                            ഇരട്ട x = H[i][i + 1];
                            ഇരട്ട y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            എങ്കിൽ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            എങ്കിൽ (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } അല്ലെങ്കിൽ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ഇരട്ട t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        എങ്കിൽ ((eps * t) * t > 1) {
                            വേണ്ടി (പൂർണ്ണസംഖ്യ j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#എങ്കിൽ 0 // 'if' condition is always false
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nn; i++) {
            എങ്കിൽ (i < താഴെയുള്ള || i > മുകളിലെ) {
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        വേണ്ടി (പൂർണ്ണസംഖ്യ j = nn - 1; j >= താഴെയുള്ള; j--) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = താഴെയുള്ള; i <= മുകളിലെ; i++) {
                ഇരട്ട z = 0.0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = താഴെയുള്ള; k <= std::min(j, മുകളിലെ); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    ശൂന്യം orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        പൂർണ്ണസംഖ്യ താഴെയുള്ള = 0;
        പൂർണ്ണസംഖ്യ മുകളിലെ = n - 1;

        വേണ്ടി (പൂർണ്ണസംഖ്യ m = താഴെയുള്ള + 1; m < മുകളിലെ; m++) {

            // Scale column.

            ഇരട്ട scale = 0.0;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = m; i <= മുകളിലെ; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            എങ്കിൽ (scale != 0.0) {

                // Compute Householder transformation.

                ഇരട്ട h = 0.0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = മുകളിലെ; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ഇരട്ട g = std::sqrt(h);
                എങ്കിൽ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                വേണ്ടി (പൂർണ്ണസംഖ്യ j = m; j < n; j++) {
                    ഇരട്ട f = 0.0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = മുകളിലെ; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = m; i <= മുകളിലെ; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <= മുകളിലെ; i++) {
                    ഇരട്ട f = 0.0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = മുകളിലെ; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = m; j <= മുകളിലെ; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        വേണ്ടി (പൂർണ്ണസംഖ്യ m = മുകളിലെ - 1; m > താഴെയുള്ള; m--) {
            എങ്കിൽ (H[m][m - 1] != 0.0) {
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = m + 1; i <= മുകളിലെ; i++) {
                    ort[i] = H[i][m - 1];
                }
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = m; j <= മുകളിലെ; j++) {
                    ഇരട്ട g = 0.0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = m; i <= മുകളിലെ; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = m; i <= മുകളിലെ; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    ശൂന്യം release() {
        // releases the working data
        delete[] d; d = ശൂന്യ;
        delete[] e; e = ശൂന്യ;
        delete[] ort; ort = ശൂന്യ;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            എങ്കിൽ (H) delete[] H[i];
            എങ്കിൽ (V) delete[] V[i];
        }
        delete[] H; H = ശൂന്യ;
        delete[] V; V = ശൂന്യ;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    ശൂന്യം compute() {
        // Allocate memory for the working data.
        V = alloc_2d<ഇരട്ട> (n, n, 0.0);
        d = alloc_1d<ഇരട്ട> (n);
        e = alloc_1d<ഇരട്ട> (n);
        ort = alloc_1d<ഇരട്ട> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
                _eigenvalues.at<ഇരട്ട> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < n; j++)
                    _eigenvectors.at<ഇരട്ട> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

പൊതു:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ശൂന്യ), e(ശൂന്യ), ort(ശൂന്യ),
        V(ശൂന്യ), H(ശൂന്യ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    ശൂന്യം compute(നിവേശം_Array src, തർക്കം fallbackSymmetric = സത്യം)
    {
        CV_INSTRUMENT_REGION();

        എങ്കിൽ(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } അല്ലെങ്കിൽ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ഇത്->n = tmp.cols;
            // Allocate the matrix data to work on.
            ഇത്->H = alloc_2d<ഇരട്ട> (n, n);
            // Now safely copy the data.
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < tmp.rows; i++) {
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < tmp.cols; j++) {
                    ഇത്->H[i][j] = tmp.at<ഇരട്ട>(i, j);
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
    Mat eigenvalues() സ്ഥിരം { തിരികെ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() സ്ഥിരം { തിരികെ _eigenvectors; }
};

ശൂന്യം eigenNonSymmetric(നിവേശം_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    പൂർണ്ണസംഖ്യ type = src.type();
    പൂർണ്ണസംഖ്യ n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    എങ്കിൽ (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    അല്ലെങ്കിൽ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, അസത്യം);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ഇരട്ട> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<പൂർണ്ണസംഖ്യ> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<ഇരട്ട> sorted_eigenvalues64f(n);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    എങ്കിൽ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<ഇരട്ട> sorted_eigenvectors64f(n, n, CV_64FC1);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
        {
            ഇരട്ട* pDst = sorted_eigenvectors64f.ptr<ഇരട്ട>(i);
            ഇരട്ട* pSrc = eigenvectors64f.ptr<ഇരട്ട>(sort_indexes[i]);
            CV_Assert(pSrc != ശൂന്യ);
            memcpy(pDst, pSrc, n * വലുപ്പം(ഇരട്ട));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(പൂർണ്ണസംഖ്യ അക്കം_components) : അക്കം_components_2(അക്കം_components) { }

LDA::LDA(നിവേശം_Array_Of_Arrays src, നിവേശം_Array labels, പൂർണ്ണസംഖ്യ അക്കം_components) : അക്കം_components_2(അക്കം_components)
{
    ഇത്->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

ശൂന്യം LDA::save(സ്ഥിരം String& filename) സ്ഥിരം
{
    FileStorage fs(filename, FileStorage::WRITE);
    എങ്കിൽ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ഇത്->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
ശൂന്യം LDA::load(സ്ഥിരം String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    എങ്കിൽ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ഇത്->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
ശൂന്യം LDA::save(FileStorage& fs) സ്ഥിരം {
    // write matrices
    fs << "num_components" << അക്കം_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
ശൂന്യം LDA::load(സ്ഥിരം FileStorage& fs) {
    //read matrices
    fs["num_components"] >> അക്കം_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

ശൂന്യം LDA::lda(നിവേശം_Array_Of_Arrays _src, നിവേശം_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<പൂർണ്ണസംഖ്യ> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        വേണ്ടി(unsigned പൂർണ്ണസംഖ്യ i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<പൂർണ്ണസംഖ്യ>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<പൂർണ്ണസംഖ്യ> mapped_labels(labels.size());
    std::vector<പൂർണ്ണസംഖ്യ> num2label = remove_dups(labels);
    std::map<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ> label2num;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)num2label.size(); i++)
        label2num[num2label[i]] = i;
    വേണ്ടി (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    പൂർണ്ണസംഖ്യ N = data.rows;
    പൂർണ്ണസംഖ്യ D = data.cols;
    // number of unique labels
    പൂർണ്ണസംഖ്യ C = (പൂർണ്ണസംഖ്യ)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    എങ്കിൽ(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    എങ്കിൽ (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    എങ്കിൽ (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    എങ്കിൽ ((അക്കം_components_2 <= 0) || (അക്കം_components_2 >= C)) {
        അക്കം_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<പൂർണ്ണസംഖ്യ> അക്കം_Class(C);
    // initialize
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < C; i++) {
        അക്കം_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++) {
        Mat instance = data.row(i);
        പൂർണ്ണസംഖ്യ classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        അക്കം_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<ഇരട്ട> (N));
    // calculate class means
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<ഇരട്ട> (അക്കം_Class[i]));
    }
    // subtract class means
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++) {
        പൂർണ്ണസംഖ്യ classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, സത്യം);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, സത്യം);
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
    std::vector<പൂർണ്ണസംഖ്യ> sorted_indices = argsort(_eigenvalues, അസത്യം);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, അക്കം_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, അക്കം_components_2));
}

ശൂന്യം LDA::compute(നിവേശം_Array_Of_Arrays _src, നിവേശം_Array _lbls) {
    തിരഞ്ഞെടുപ്പ്(_src.kind()) {
    അവസ്ഥ നിവേശം_Array_2::STD_VECTOR_MAT:
    അവസ്ഥ നിവേശം_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        നിർത്തുക;
    അവസ്ഥ നിവേശം_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        നിർത്തുക;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(നിവേശം_Array src) {
   തിരികെ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(നിവേശം_Array src) {
   തിരികെ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
