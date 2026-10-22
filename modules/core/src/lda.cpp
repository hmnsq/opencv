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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <iostream>
#அந்தர்பாவ <map>
#அந்தர்பாவ <set>

நாமாகாஶ cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
உள்வரி std::vector<_Tp> remove_dups(மாறிலி std::vector<_Tp>& src) {
    வகைவரை typename std::set<_Tp>::const_iterator constSetIterator;
    வகைவரை typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    க்கு (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    க்கு (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    திரும்பு elems;
}

நிலையான Mat argsort(உள்ளீடு_Array _src, தருக்க ascending=உண்மை)
{
    Mat src = _src.getMat();
    எனில் (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    முழு flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    திரும்பு sorted_indices;
}

நிலையான Mat asRowMatrix(உள்ளீடு_Array_Of_Arrays src, முழு rtype, இரட்டை alpha=1, இரட்டை beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    எனில்(src.kind() != உள்ளீடு_Array_2::STD_VECTOR_MAT && src.kind() != உள்ளீடு_Array_2::STD_ARRAY_MAT &&
        src.kind() != உள்ளீடு_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    எனில்(n == 0)
        திரும்பு Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((முழு)n, (முழு)d, rtype);
    // now copy data
    க்கு(முழு i = 0; i < (முழு)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        எனில்(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (முழு)d, (முழு)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        எனில்(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } இல்லையெனில் {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    திரும்பு data;
}

நிலையான வெறுமை sortMatrixColumnsByIndices(உள்ளீடு_Array _src, உள்ளீடு_Array _indices, OutputArray _dst) {
    எனில்(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<முழு> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    க்கு(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((முழு)idx);
        originalCol.copyTo(sortedCol);
    }
}

நிலையான Mat sortMatrixColumnsByIndices(உள்ளீடு_Array src, உள்ளீடு_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    திரும்பு dst;
}


template<typename _Tp> நிலையான தருக்க
isSymmetric_(உள்ளீடு_Array src) {
    Mat _src = src.getMat();
    எனில்(_src.cols != _src.rows)
        திரும்பு பொய்;
    க்கு (முழு i = 0; i < _src.rows; i++) {
        க்கு (முழு j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            எனில் (a != b) {
                திரும்பு பொய்;
            }
        }
    }
    திரும்பு உண்மை;
}

template<typename _Tp> நிலையான தருக்க
isSymmetric_(உள்ளீடு_Array src, இரட்டை eps) {
    Mat _src = src.getMat();
    எனில்(_src.cols != _src.rows)
        திரும்பு பொய்;
    க்கு (முழு i = 0; i < _src.rows; i++) {
        க்கு (முழு j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            எனில் (std::abs(a - b) > eps) {
                திரும்பு பொய்;
            }
        }
    }
    திரும்பு உண்மை;
}

நிலையான தருக்க isSymmetric(உள்ளீடு_Array src, இரட்டை eps=1e-16)
{
    Mat m = src.getMat();
    தேர்வு (m.type()) {
        நிலை CV_8SC1: திரும்பு isSymmetric_<எழுத்து>(m); நிறுத்து;
        நிலை CV_8UC1:
            திரும்பு isSymmetric_<unsigned எழுத்து>(m); நிறுத்து;
        நிலை CV_16SC1:
            திரும்பு isSymmetric_<short>(m); நிறுத்து;
        நிலை CV_16UC1:
            திரும்பு isSymmetric_<unsigned short>(m); நிறுத்து;
        நிலை CV_32SC1:
            திரும்பு isSymmetric_<முழு>(m); நிறுத்து;
        நிலை CV_32FC1:
            திரும்பு isSymmetric_<தசம>(m, eps); நிறுத்து;
        நிலை CV_64FC1:
            திரும்பு isSymmetric_<இரட்டை>(m, eps); நிறுத்து;
        இயல்பு:
            நிறுத்து;
    }
    திரும்பு பொய்;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(உள்ளீடு_Array _W, உள்ளீடு_Array _mean, உள்ளீடு_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    முழு n = src.rows;
    முழு d = src.cols;
    // make sure the data has the correct shape
    எனில்(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    எனில்(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    எனில்(!mean.empty()) {
        க்கு(முழு i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    திரும்பு Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(உள்ளீடு_Array _W, உள்ளீடு_Array _mean, உள்ளீடு_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    முழு n = src.rows;
    முழு d = src.cols;
    // make sure the data has the correct shape
    எனில்(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    எனில்(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    எனில்(!mean.empty()) {
        க்கு(முழு i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    திரும்பு X;
}


வகுப்பு EigenvalueDecomposition {
தனிப்பட்ட:

    // Holds the data dimension.
    முழு n;

    // Pointer to internal memory.
    இரட்டை *d, *e, *ort;
    இரட்டை **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(முழு m) {
        திரும்பு புதிய _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(முழு m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        க்கு (முழு i = 0; i < m; i++)
            arr[i] = val;
        திரும்பு arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(முழு m, முழு _n) {
        _Tp **arr = புதிய _Tp*[m];
        க்கு (முழு i = 0; i < m; i++)
            arr[i] = புதிய _Tp[_n];
        திரும்பு arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(முழு m, முழு _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        க்கு (முழு i = 0; i < m; i++) {
            க்கு (முழு j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        திரும்பு arr;
    }

    நிலையான வெறுமை complex_div(இரட்டை xr, இரட்டை xi, இரட்டை yr, இரட்டை yi, இரட்டை& cdivr, இரட்டை& cdivi) {
        இரட்டை r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        எனில் (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } இல்லையெனில் {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    வெறுமை hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        மாறிலி முழு max_iters_மொத்தம் = 1000 * இது->n;

        மாறிலி முழு nn = இது->n; CV_Assert(nn > 0);
        முழு n1 = nn - 1;
        மாறிலி முழு கீழ் = 0;
        மாறிலி முழு உயர்வு = nn - 1;
        மாறிலி இரட்டை eps = std::numeric_limits<இரட்டை>::epsilon();
        இரட்டை exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        இரட்டை norm = 0.0;
        க்கு (முழு i = 0; i < nn; i++) {
#எனில் 0 // 'if' condition is always false
            எனில் (i < கீழ் || i > உயர்வு) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            க்கு (முழு j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        முழு iter = 0;
        போது (n1 >= கீழ்) {

            // Look for single small sub-diagonal element
            முழு l = n1;
            போது (l > கீழ்) {
                எனில் (norm < FLT_EPSILON) {
                    நிறுத்து;
                }
                இரட்டை s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                எனில் (s == 0.0) {
                    s = norm;
                }
                எனில் (std::abs(H[l][l - 1]) < eps * s) {
                    நிறுத்து;
                }
                l--;
            }

            // Check for convergence
            எனில் (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } இல்லையெனில் எனில் (l == n1 - 1) {
                // Two roots found
                இரட்டை w = H[n1][n1 - 1] * H[n1 - 1][n1];
                இரட்டை p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                இரட்டை q = p * p + w;
                இரட்டை z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                இரட்டை x = H[n1][n1];

                எனில் (q >= 0) {
                    // Real pair
                    எனில் (p >= 0) {
                        z = p + z;
                    } இல்லையெனில் {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    எனில் (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    இரட்டை s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    இரட்டை r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    க்கு (முழு j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    க்கு (முழு i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    க்கு (முழு i = கீழ்; i <= உயர்வு; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } இல்லையெனில் {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } இல்லையெனில் {
                // No convergence yet

                // Form shift
                இரட்டை x = H[n1][n1];
                இரட்டை y = 0.0;
                இரட்டை w = 0.0;
                எனில் (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                எனில் (iter == 10) {
                    exshift += x;
                    க்கு (முழு i = கீழ்; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    இரட்டை s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                எனில் (iter == 30) {
                    இரட்டை s = (y - x) * 0.5;
                    s = s * s + w;
                    எனில் (s > 0) {
                        s = std::sqrt(s);
                        எனில் (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        க்கு (முழு i = கீழ்; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                எனில் (iter > max_iters_மொத்தம்)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                இரட்டை p = std::numeric_limits<இரட்டை>::quiet_NaN();
                இரட்டை q = std::numeric_limits<இரட்டை>::quiet_NaN();
                இரட்டை r = std::numeric_limits<இரட்டை>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                முழு m = n1 - 2;
                போது (m >= l) {
                    இரட்டை z = H[m][m];
                    r = x - z;
                    இரட்டை s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    எனில் (m == l) {
                        நிறுத்து;
                    }
                    எனில் (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        நிறுத்து;
                    }
                    m--;
                }

                க்கு (முழு i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    எனில் (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                க்கு (முழு k = m; k < n1; k++) {

                    தருக்க notlast = (k != n1 - 1);
                    எனில் (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        எனில் (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    எனில் (x == 0.0) {
                        நிறுத்து;
                    }
                    இரட்டை s = std::sqrt(p * p + q * q + r * r);
                    எனில் (p < 0) {
                        s = -s;
                    }
                    எனில் (s != 0) {
                        எனில் (k != m) {
                            H[k][k - 1] = -s * x;
                        } இல்லையெனில் எனில் (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        இரட்டை z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        க்கு (முழு j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            எனில் (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        க்கு (முழு i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            எனில் (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        க்கு (முழு i = கீழ்; i <= உயர்வு; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            எனில் (notlast) {
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

        எனில் (norm < FLT_EPSILON) {
            திரும்பு;
        }

        க்கு (n1 = nn - 1; n1 >= 0; n1--) {
            இரட்டை p = d[n1];
            இரட்டை q = e[n1];

            எனில் (q == 0) {
                // Real vector
                இரட்டை z = std::numeric_limits<இரட்டை>::quiet_NaN();
                இரட்டை s = std::numeric_limits<இரட்டை>::quiet_NaN();

                முழு l = n1;
                H[n1][n1] = 1.0;
                க்கு (முழு i = n1 - 1; i >= 0; i--) {
                    இரட்டை w = H[i][i] - p;
                    இரட்டை r = 0.0;
                    க்கு (முழு j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    எனில் (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } இல்லையெனில் {
                        l = i;
                        எனில் (e[i] == 0.0) {
                            எனில் (w != 0.0) {
                                H[i][n1] = -r / w;
                            } இல்லையெனில் {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } இல்லையெனில் {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            இரட்டை x = H[i][i + 1];
                            இரட்டை y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            இரட்டை t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            எனில் (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } இல்லையெனில் {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        இரட்டை t = std::abs(H[i][n1]);
                        எனில் ((eps * t) * t > 1) {
                            இரட்டை inv_t = 1.0 / t;
                            க்கு (முழு j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } இல்லையெனில் எனில் (q < 0) {
                // Complex vector
                இரட்டை z = std::numeric_limits<இரட்டை>::quiet_NaN();
                இரட்டை r = std::numeric_limits<இரட்டை>::quiet_NaN();
                இரட்டை s = std::numeric_limits<இரட்டை>::quiet_NaN();

                முழு l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                எனில் (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } இல்லையெனில் {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                க்கு (முழு i = n1 - 2; i >= 0; i--) {
                    இரட்டை ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    க்கு (முழு j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    இரட்டை w = H[i][i] - p;

                    எனில் (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } இல்லையெனில் {
                        l = i;
                        எனில் (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } இல்லையெனில் {
                            // Solve complex equations

                            இரட்டை x = H[i][i + 1];
                            இரட்டை y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            எனில் (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            எனில் (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } இல்லையெனில் {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        இரட்டை t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        எனில் ((eps * t) * t > 1) {
                            க்கு (முழு j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#எனில் 0 // 'if' condition is always false
        க்கு (முழு i = 0; i < nn; i++) {
            எனில் (i < கீழ் || i > உயர்வு) {
                க்கு (முழு j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        க்கு (முழு j = nn - 1; j >= கீழ்; j--) {
            க்கு (முழு i = கீழ்; i <= உயர்வு; i++) {
                இரட்டை z = 0.0;
                க்கு (முழு k = கீழ்; k <= std::min(j, உயர்வு); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    வெறுமை orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        முழு கீழ் = 0;
        முழு உயர்வு = n - 1;

        க்கு (முழு m = கீழ் + 1; m < உயர்வு; m++) {

            // Scale column.

            இரட்டை scale = 0.0;
            க்கு (முழு i = m; i <= உயர்வு; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            எனில் (scale != 0.0) {

                // Compute Householder transformation.

                இரட்டை h = 0.0;
                க்கு (முழு i = உயர்வு; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                இரட்டை g = std::sqrt(h);
                எனில் (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                க்கு (முழு j = m; j < n; j++) {
                    இரட்டை f = 0.0;
                    க்கு (முழு i = உயர்வு; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    க்கு (முழு i = m; i <= உயர்வு; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                க்கு (முழு i = 0; i <= உயர்வு; i++) {
                    இரட்டை f = 0.0;
                    க்கு (முழு j = உயர்வு; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    க்கு (முழு j = m; j <= உயர்வு; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        க்கு (முழு i = 0; i < n; i++) {
            க்கு (முழு j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        க்கு (முழு m = உயர்வு - 1; m > கீழ்; m--) {
            எனில் (H[m][m - 1] != 0.0) {
                க்கு (முழு i = m + 1; i <= உயர்வு; i++) {
                    ort[i] = H[i][m - 1];
                }
                க்கு (முழு j = m; j <= உயர்வு; j++) {
                    இரட்டை g = 0.0;
                    க்கு (முழு i = m; i <= உயர்வு; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    க்கு (முழு i = m; i <= உயர்வு; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    வெறுமை release() {
        // releases the working data
        delete[] d; d = வெற்று;
        delete[] e; e = வெற்று;
        delete[] ort; ort = வெற்று;
        க்கு (முழு i = 0; i < n; i++) {
            எனில் (H) delete[] H[i];
            எனில் (V) delete[] V[i];
        }
        delete[] H; H = வெற்று;
        delete[] V; V = வெற்று;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    வெறுமை compute() {
        // Allocate memory for the working data.
        V = alloc_2d<இரட்டை> (n, n, 0.0);
        d = alloc_1d<இரட்டை> (n);
        e = alloc_1d<இரட்டை> (n);
        ort = alloc_1d<இரட்டை> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            க்கு (முழு i = 0; i < n; i++) {
                _eigenvalues.at<இரட்டை> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            க்கு (முழு i = 0; i < n; i++)
                க்கு (முழு j = 0; j < n; j++)
                    _eigenvectors.at<இரட்டை> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

பொது:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(வெற்று), e(வெற்று), ort(வெற்று),
        V(வெற்று), H(வெற்று)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    வெறுமை compute(உள்ளீடு_Array src, தருக்க fallbackSymmetric = உண்மை)
    {
        CV_INSTRUMENT_REGION();

        எனில்(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } இல்லையெனில் {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            இது->n = tmp.cols;
            // Allocate the matrix data to work on.
            இது->H = alloc_2d<இரட்டை> (n, n);
            // Now safely copy the data.
            க்கு (முழு i = 0; i < tmp.rows; i++) {
                க்கு (முழு j = 0; j < tmp.cols; j++) {
                    இது->H[i][j] = tmp.at<இரட்டை>(i, j);
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
    Mat eigenvalues() மாறிலி { திரும்பு _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() மாறிலி { திரும்பு _eigenvectors; }
};

வெறுமை eigenNonSymmetric(உள்ளீடு_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    முழு type = src.type();
    முழு n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    எனில் (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    இல்லையெனில்
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, பொய்);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<இரட்டை> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<முழு> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<இரட்டை> sorted_eigenvalues64f(n);
    க்கு (முழு i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    எனில்( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<இரட்டை> sorted_eigenvectors64f(n, n, CV_64FC1);
        க்கு (முழு i = 0; i < n; i++)
        {
            இரட்டை* pDst = sorted_eigenvectors64f.ptr<இரட்டை>(i);
            இரட்டை* pSrc = eigenvectors64f.ptr<இரட்டை>(sort_indexes[i]);
            CV_Assert(pSrc != வெற்று);
            memcpy(pDst, pSrc, n * அளவு(இரட்டை));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(முழு இலக்கம்_components) : இலக்கம்_components_2(இலக்கம்_components) { }

LDA::LDA(உள்ளீடு_Array_Of_Arrays src, உள்ளீடு_Array labels, முழு இலக்கம்_components) : இலக்கம்_components_2(இலக்கம்_components)
{
    இது->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

வெறுமை LDA::save(மாறிலி String& filename) மாறிலி
{
    FileStorage fs(filename, FileStorage::WRITE);
    எனில் (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    இது->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
வெறுமை LDA::load(மாறிலி String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    எனில் (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    இது->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
வெறுமை LDA::save(FileStorage& fs) மாறிலி {
    // write matrices
    fs << "num_components" << இலக்கம்_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
வெறுமை LDA::load(மாறிலி FileStorage& fs) {
    //read matrices
    fs["num_components"] >> இலக்கம்_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

வெறுமை LDA::lda(உள்ளீடு_Array_Of_Arrays _src, உள்ளீடு_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<முழு> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        க்கு(unsigned முழு i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<முழு>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<முழு> mapped_labels(labels.size());
    std::vector<முழு> num2label = remove_dups(labels);
    std::map<முழு, முழு> label2num;
    க்கு (முழு i = 0; i < (முழு)num2label.size(); i++)
        label2num[num2label[i]] = i;
    க்கு (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    முழு N = data.rows;
    முழு D = data.cols;
    // number of unique labels
    முழு C = (முழு)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    எனில்(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    எனில் (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    எனில் (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    எனில் ((இலக்கம்_components_2 <= 0) || (இலக்கம்_components_2 >= C)) {
        இலக்கம்_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<முழு> இலக்கம்_Class(C);
    // initialize
    க்கு (முழு i = 0; i < C; i++) {
        இலக்கம்_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    க்கு (முழு i = 0; i < N; i++) {
        Mat instance = data.row(i);
        முழு classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        இலக்கம்_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<இரட்டை> (N));
    // calculate class means
    க்கு (முழு i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<இரட்டை> (இலக்கம்_Class[i]));
    }
    // subtract class means
    க்கு (முழு i = 0; i < N; i++) {
        முழு classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, உண்மை);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    க்கு (முழு i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, உண்மை);
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
    std::vector<முழு> sorted_indices = argsort(_eigenvalues, பொய்);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, இலக்கம்_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, இலக்கம்_components_2));
}

வெறுமை LDA::compute(உள்ளீடு_Array_Of_Arrays _src, உள்ளீடு_Array _lbls) {
    தேர்வு(_src.kind()) {
    நிலை உள்ளீடு_Array_2::STD_VECTOR_MAT:
    நிலை உள்ளீடு_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        நிறுத்து;
    நிலை உள்ளீடு_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        நிறுத்து;
    இயல்பு:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        நிறுத்து;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(உள்ளீடு_Array src) {
   திரும்பு subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(உள்ளீடு_Array src) {
   திரும்பு subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
