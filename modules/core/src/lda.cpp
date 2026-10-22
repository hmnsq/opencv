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

#شامل "precomp.hpp"
#شامل <iostream>
#شامل <map>
#شامل <set>

فضاینام cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
درونخطی std::vector<_Tp> remove_dups(ثابت std::vector<_Tp>& src) {
    تعریفنوع typename std::set<_Tp>::const_iterator constSetIterator;
    تعریفنوع typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    برای (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    برای (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    بازگشت elems;
}

ایستا Mat argsort(درون_داد_Array _src, منطقی ascending=درست)
{
    Mat src = _src.getMat();
    اگر (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    صحیح flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    بازگشت sorted_indices;
}

ایستا Mat asRowMatrix(درون_داد_Array_Of_Arrays src, صحیح rtype, دوگانه alpha=1, دوگانه beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    اگر(src.kind() != درون_داد_Array_2::STD_VECTOR_MAT && src.kind() != درون_داد_Array_2::STD_ARRAY_MAT &&
        src.kind() != درون_داد_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    اگر(n == 0)
        بازگشت Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((صحیح)n, (صحیح)d, rtype);
    // now copy data
    برای(صحیح i = 0; i < (صحیح)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        اگر(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (صحیح)d, (صحیح)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        اگر(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } وگرنه {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    بازگشت data;
}

ایستا تهی sortMatrixColumnsByIndices(درون_داد_Array _src, درون_داد_Array _indices, OutputArray _dst) {
    اگر(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<صحیح> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    برای(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((صحیح)idx);
        originalCol.copyTo(sortedCol);
    }
}

ایستا Mat sortMatrixColumnsByIndices(درون_داد_Array src, درون_داد_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    بازگشت dst;
}


template<typename _Tp> ایستا منطقی
isSymmetric_(درون_داد_Array src) {
    Mat _src = src.getMat();
    اگر(_src.cols != _src.rows)
        بازگشت نادرست;
    برای (صحیح i = 0; i < _src.rows; i++) {
        برای (صحیح j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            اگر (a != b) {
                بازگشت نادرست;
            }
        }
    }
    بازگشت درست;
}

template<typename _Tp> ایستا منطقی
isSymmetric_(درون_داد_Array src, دوگانه eps) {
    Mat _src = src.getMat();
    اگر(_src.cols != _src.rows)
        بازگشت نادرست;
    برای (صحیح i = 0; i < _src.rows; i++) {
        برای (صحیح j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            اگر (std::abs(a - b) > eps) {
                بازگشت نادرست;
            }
        }
    }
    بازگشت درست;
}

ایستا منطقی isSymmetric(درون_داد_Array src, دوگانه eps=1e-16)
{
    Mat m = src.getMat();
    انتخاب (m.type()) {
        حالت CV_8SC1: بازگشت isSymmetric_<نویسه>(m); بشکن;
        حالت CV_8UC1:
            بازگشت isSymmetric_<unsigned نویسه>(m); بشکن;
        حالت CV_16SC1:
            بازگشت isSymmetric_<short>(m); بشکن;
        حالت CV_16UC1:
            بازگشت isSymmetric_<unsigned short>(m); بشکن;
        حالت CV_32SC1:
            بازگشت isSymmetric_<صحیح>(m); بشکن;
        حالت CV_32FC1:
            بازگشت isSymmetric_<اعشاری>(m, eps); بشکن;
        حالت CV_64FC1:
            بازگشت isSymmetric_<دوگانه>(m, eps); بشکن;
        پیشفرض:
            بشکن;
    }
    بازگشت نادرست;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(درون_داد_Array _W, درون_داد_Array _mean, درون_داد_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحیح n = src.rows;
    صحیح d = src.cols;
    // make sure the data has the correct shape
    اگر(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    اگر(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    اگر(!mean.empty()) {
        برای(صحیح i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    بازگشت Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(درون_داد_Array _W, درون_داد_Array _mean, درون_داد_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحیح n = src.rows;
    صحیح d = src.cols;
    // make sure the data has the correct shape
    اگر(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    اگر(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    اگر(!mean.empty()) {
        برای(صحیح i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    بازگشت X;
}


رده EigenvalueDecomposition {
خصوصی:

    // Holds the data dimension.
    صحیح n;

    // Pointer to internal memory.
    دوگانه *d, *e, *ort;
    دوگانه **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(صحیح m) {
        بازگشت جدید _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(صحیح m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        برای (صحیح i = 0; i < m; i++)
            arr[i] = val;
        بازگشت arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(صحیح m, صحیح _n) {
        _Tp **arr = جدید _Tp*[m];
        برای (صحیح i = 0; i < m; i++)
            arr[i] = جدید _Tp[_n];
        بازگشت arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(صحیح m, صحیح _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        برای (صحیح i = 0; i < m; i++) {
            برای (صحیح j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        بازگشت arr;
    }

    ایستا تهی complex_div(دوگانه xr, دوگانه xi, دوگانه yr, دوگانه yi, دوگانه& cdivr, دوگانه& cdivi) {
        دوگانه r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        اگر (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } وگرنه {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    تهی hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ثابت صحیح max_iters_شمار = 1000 * این->n;

        ثابت صحیح nn = این->n; CV_Assert(nn > 0);
        صحیح n1 = nn - 1;
        ثابت صحیح کم = 0;
        ثابت صحیح بالا = nn - 1;
        ثابت دوگانه eps = std::numeric_limits<دوگانه>::epsilon();
        دوگانه exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        دوگانه norm = 0.0;
        برای (صحیح i = 0; i < nn; i++) {
#اگر 0 // 'if' condition is always false
            اگر (i < کم || i > بالا) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            برای (صحیح j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        صحیح iter = 0;
        وقتی (n1 >= کم) {

            // Look for single small sub-diagonal element
            صحیح l = n1;
            وقتی (l > کم) {
                اگر (norm < FLT_EPSILON) {
                    بشکن;
                }
                دوگانه s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                اگر (s == 0.0) {
                    s = norm;
                }
                اگر (std::abs(H[l][l - 1]) < eps * s) {
                    بشکن;
                }
                l--;
            }

            // Check for convergence
            اگر (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } وگرنه اگر (l == n1 - 1) {
                // Two roots found
                دوگانه w = H[n1][n1 - 1] * H[n1 - 1][n1];
                دوگانه p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                دوگانه q = p * p + w;
                دوگانه z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                دوگانه x = H[n1][n1];

                اگر (q >= 0) {
                    // Real pair
                    اگر (p >= 0) {
                        z = p + z;
                    } وگرنه {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    اگر (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    دوگانه s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    دوگانه r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    برای (صحیح j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    برای (صحیح i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    برای (صحیح i = کم; i <= بالا; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } وگرنه {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } وگرنه {
                // No convergence yet

                // Form shift
                دوگانه x = H[n1][n1];
                دوگانه y = 0.0;
                دوگانه w = 0.0;
                اگر (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                اگر (iter == 10) {
                    exshift += x;
                    برای (صحیح i = کم; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    دوگانه s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                اگر (iter == 30) {
                    دوگانه s = (y - x) * 0.5;
                    s = s * s + w;
                    اگر (s > 0) {
                        s = std::sqrt(s);
                        اگر (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        برای (صحیح i = کم; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                اگر (iter > max_iters_شمار)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                دوگانه p = std::numeric_limits<دوگانه>::quiet_NaN();
                دوگانه q = std::numeric_limits<دوگانه>::quiet_NaN();
                دوگانه r = std::numeric_limits<دوگانه>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                صحیح m = n1 - 2;
                وقتی (m >= l) {
                    دوگانه z = H[m][m];
                    r = x - z;
                    دوگانه s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    اگر (m == l) {
                        بشکن;
                    }
                    اگر (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        بشکن;
                    }
                    m--;
                }

                برای (صحیح i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    اگر (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                برای (صحیح k = m; k < n1; k++) {

                    منطقی notlast = (k != n1 - 1);
                    اگر (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        اگر (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    اگر (x == 0.0) {
                        بشکن;
                    }
                    دوگانه s = std::sqrt(p * p + q * q + r * r);
                    اگر (p < 0) {
                        s = -s;
                    }
                    اگر (s != 0) {
                        اگر (k != m) {
                            H[k][k - 1] = -s * x;
                        } وگرنه اگر (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        دوگانه z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        برای (صحیح j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            اگر (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        برای (صحیح i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            اگر (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        برای (صحیح i = کم; i <= بالا; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            اگر (notlast) {
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

        اگر (norm < FLT_EPSILON) {
            بازگشت;
        }

        برای (n1 = nn - 1; n1 >= 0; n1--) {
            دوگانه p = d[n1];
            دوگانه q = e[n1];

            اگر (q == 0) {
                // Real vector
                دوگانه z = std::numeric_limits<دوگانه>::quiet_NaN();
                دوگانه s = std::numeric_limits<دوگانه>::quiet_NaN();

                صحیح l = n1;
                H[n1][n1] = 1.0;
                برای (صحیح i = n1 - 1; i >= 0; i--) {
                    دوگانه w = H[i][i] - p;
                    دوگانه r = 0.0;
                    برای (صحیح j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    اگر (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } وگرنه {
                        l = i;
                        اگر (e[i] == 0.0) {
                            اگر (w != 0.0) {
                                H[i][n1] = -r / w;
                            } وگرنه {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } وگرنه {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            دوگانه x = H[i][i + 1];
                            دوگانه y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            دوگانه t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            اگر (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } وگرنه {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        دوگانه t = std::abs(H[i][n1]);
                        اگر ((eps * t) * t > 1) {
                            دوگانه inv_t = 1.0 / t;
                            برای (صحیح j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } وگرنه اگر (q < 0) {
                // Complex vector
                دوگانه z = std::numeric_limits<دوگانه>::quiet_NaN();
                دوگانه r = std::numeric_limits<دوگانه>::quiet_NaN();
                دوگانه s = std::numeric_limits<دوگانه>::quiet_NaN();

                صحیح l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                اگر (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } وگرنه {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                برای (صحیح i = n1 - 2; i >= 0; i--) {
                    دوگانه ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    برای (صحیح j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    دوگانه w = H[i][i] - p;

                    اگر (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } وگرنه {
                        l = i;
                        اگر (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } وگرنه {
                            // Solve complex equations

                            دوگانه x = H[i][i + 1];
                            دوگانه y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            اگر (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            اگر (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } وگرنه {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        دوگانه t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        اگر ((eps * t) * t > 1) {
                            برای (صحیح j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#اگر 0 // 'if' condition is always false
        برای (صحیح i = 0; i < nn; i++) {
            اگر (i < کم || i > بالا) {
                برای (صحیح j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        برای (صحیح j = nn - 1; j >= کم; j--) {
            برای (صحیح i = کم; i <= بالا; i++) {
                دوگانه z = 0.0;
                برای (صحیح k = کم; k <= std::min(j, بالا); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    تهی orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        صحیح کم = 0;
        صحیح بالا = n - 1;

        برای (صحیح m = کم + 1; m < بالا; m++) {

            // Scale column.

            دوگانه scale = 0.0;
            برای (صحیح i = m; i <= بالا; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            اگر (scale != 0.0) {

                // Compute Householder transformation.

                دوگانه h = 0.0;
                برای (صحیح i = بالا; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                دوگانه g = std::sqrt(h);
                اگر (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                برای (صحیح j = m; j < n; j++) {
                    دوگانه f = 0.0;
                    برای (صحیح i = بالا; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    برای (صحیح i = m; i <= بالا; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                برای (صحیح i = 0; i <= بالا; i++) {
                    دوگانه f = 0.0;
                    برای (صحیح j = بالا; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    برای (صحیح j = m; j <= بالا; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        برای (صحیح i = 0; i < n; i++) {
            برای (صحیح j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        برای (صحیح m = بالا - 1; m > کم; m--) {
            اگر (H[m][m - 1] != 0.0) {
                برای (صحیح i = m + 1; i <= بالا; i++) {
                    ort[i] = H[i][m - 1];
                }
                برای (صحیح j = m; j <= بالا; j++) {
                    دوگانه g = 0.0;
                    برای (صحیح i = m; i <= بالا; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    برای (صحیح i = m; i <= بالا; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    تهی release() {
        // releases the working data
        delete[] d; d = پوچ;
        delete[] e; e = پوچ;
        delete[] ort; ort = پوچ;
        برای (صحیح i = 0; i < n; i++) {
            اگر (H) delete[] H[i];
            اگر (V) delete[] V[i];
        }
        delete[] H; H = پوچ;
        delete[] V; V = پوچ;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    تهی compute() {
        // Allocate memory for the working data.
        V = alloc_2d<دوگانه> (n, n, 0.0);
        d = alloc_1d<دوگانه> (n);
        e = alloc_1d<دوگانه> (n);
        ort = alloc_1d<دوگانه> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            برای (صحیح i = 0; i < n; i++) {
                _eigenvalues.at<دوگانه> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            برای (صحیح i = 0; i < n; i++)
                برای (صحیح j = 0; j < n; j++)
                    _eigenvectors.at<دوگانه> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

عمومی:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(پوچ), e(پوچ), ort(پوچ),
        V(پوچ), H(پوچ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    تهی compute(درون_داد_Array src, منطقی fallbackSymmetric = درست)
    {
        CV_INSTRUMENT_REGION();

        اگر(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } وگرنه {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            این->n = tmp.cols;
            // Allocate the matrix data to work on.
            این->H = alloc_2d<دوگانه> (n, n);
            // Now safely copy the data.
            برای (صحیح i = 0; i < tmp.rows; i++) {
                برای (صحیح j = 0; j < tmp.cols; j++) {
                    این->H[i][j] = tmp.at<دوگانه>(i, j);
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
    Mat eigenvalues() ثابت { بازگشت _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ثابت { بازگشت _eigenvectors; }
};

تهی eigenNonSymmetric(درون_داد_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحیح type = src.type();
    صحیح n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    اگر (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    وگرنه
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, نادرست);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<دوگانه> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<صحیح> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<دوگانه> sorted_eigenvalues64f(n);
    برای (صحیح i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    اگر( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<دوگانه> sorted_eigenvectors64f(n, n, CV_64FC1);
        برای (صحیح i = 0; i < n; i++)
        {
            دوگانه* pDst = sorted_eigenvectors64f.ptr<دوگانه>(i);
            دوگانه* pSrc = eigenvectors64f.ptr<دوگانه>(sort_indexes[i]);
            CV_Assert(pSrc != پوچ);
            memcpy(pDst, pSrc, n * اندازه(دوگانه));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(صحیح رقم_components) : رقم_components_2(رقم_components) { }

LDA::LDA(درون_داد_Array_Of_Arrays src, درون_داد_Array labels, صحیح رقم_components) : رقم_components_2(رقم_components)
{
    این->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

تهی LDA::save(ثابت String& filename) ثابت
{
    FileStorage fs(filename, FileStorage::WRITE);
    اگر (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    این->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
تهی LDA::load(ثابت String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    اگر (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    این->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
تهی LDA::save(FileStorage& fs) ثابت {
    // write matrices
    fs << "num_components" << رقم_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
تهی LDA::load(ثابت FileStorage& fs) {
    //read matrices
    fs["num_components"] >> رقم_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

تهی LDA::lda(درون_داد_Array_Of_Arrays _src, درون_داد_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<صحیح> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        برای(unsigned صحیح i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<صحیح>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<صحیح> mapped_labels(labels.size());
    std::vector<صحیح> num2label = remove_dups(labels);
    std::map<صحیح, صحیح> label2num;
    برای (صحیح i = 0; i < (صحیح)num2label.size(); i++)
        label2num[num2label[i]] = i;
    برای (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    صحیح N = data.rows;
    صحیح D = data.cols;
    // number of unique labels
    صحیح C = (صحیح)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    اگر(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    اگر (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    اگر (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    اگر ((رقم_components_2 <= 0) || (رقم_components_2 >= C)) {
        رقم_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<صحیح> رقم_Class(C);
    // initialize
    برای (صحیح i = 0; i < C; i++) {
        رقم_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    برای (صحیح i = 0; i < N; i++) {
        Mat instance = data.row(i);
        صحیح classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        رقم_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<دوگانه> (N));
    // calculate class means
    برای (صحیح i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<دوگانه> (رقم_Class[i]));
    }
    // subtract class means
    برای (صحیح i = 0; i < N; i++) {
        صحیح classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, درست);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    برای (صحیح i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, درست);
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
    std::vector<صحیح> sorted_indices = argsort(_eigenvalues, نادرست);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, رقم_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, رقم_components_2));
}

تهی LDA::compute(درون_داد_Array_Of_Arrays _src, درون_داد_Array _lbls) {
    انتخاب(_src.kind()) {
    حالت درون_داد_Array_2::STD_VECTOR_MAT:
    حالت درون_داد_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        بشکن;
    حالت درون_داد_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        بشکن;
    پیشفرض:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        بشکن;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(درون_داد_Array src) {
   بازگشت subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(درون_داد_Array src) {
   بازگشت subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
