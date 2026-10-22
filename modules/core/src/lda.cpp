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

نامخلا cv
{

// Removes duplicate elements in a given vector.
سانچہ<قسم_نام _Tp>
اندرونی std::vector<_Tp> remove_dups(ثابت std::vector<_Tp>& src) {
    قسمتعریف قسم_نام std::سیٹ<_Tp>::const_iterator const_سیٹ_Iterator;
    قسمتعریف قسم_نام std::vector<_Tp>::const_iterator constVecIterator;
    std::سیٹ<_Tp> سیٹ_elems;
    برائے (constVecIterator it = src.begin(); it != src.end(); ++it)
        سیٹ_elems.insert(*it);
    std::vector<_Tp> elems;
    برائے (const_سیٹ_Iterator it = سیٹ_elems.begin(); it != سیٹ_elems.end(); ++it)
        elems.push_back(*it);
    واپس elems;
}

ساکن Mat argsort(داخلہ_Array _src, منطقی ascending=سچ)
{
    Mat src = _src.getMat();
    اگر (src.صفیں != 1 && src.ستون_تعداد != 1) {
        String error_پیغام = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    صحیح flags = ترتیب_دو_EVERY_صف | (ascending ? ترتیب_دو_ASCENDING : ترتیب_دو_DESCENDING);
    Mat sorted_indices;
    ترتیب_دو_Idx(src.reshape(1,1),sorted_indices,flags);
    واپس sorted_indices;
}

ساکن Mat as_صف_میٹرکس(داخلہ_Array_کا_Arrays src, صحیح rtype, دوہرا alpha=1, دوہرا beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    اگر(src.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT && src.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
        src.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ) {
        String error_پیغام = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // number of samples
    size_t n = src.کل();
    // return empty matrix if no matrices given
    اگر(n == 0)
        واپس Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).کل();
    // create data matrix
    Mat ڈیٹا((صحیح)n, (صحیح)d, rtype);
    // now copy data
    برائے(صحیح i = 0; i < (صحیح)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        اگر(src.getMat(i).کل() != d) {
            String error_پیغام = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (صحیح)d, (صحیح)src.getMat(i).کل());
            CV_Error(Error::StsBadArg, error_پیغام);
        }
        // get a hold of the current row
        Mat xi = ڈیٹا.صف(i);
        // make reshape happy by cloning for non-continuous matrices
        اگر(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_مقصد(xi, rtype, alpha, beta);
        } ورنہ {
            src.getMat(i).clone().reshape(1, 1).convert_مقصد(xi, rtype, alpha, beta);
        }
    }
    واپس ڈیٹا;
}

ساکن خالی ترتیب_دو_میٹرکس_Columns_By_Indices(داخلہ_Array _src, داخلہ_Array _indices, OutputArray _dst) {
    اگر(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<صحیح> indices = _indices.getMat();
    _dst.create(src.صفیں, src.ستون_تعداد, src.type());
    Mat dst = _dst.getMat();
    برائے(size_t idx = 0; idx < indices.size(); idx++) {
        Mat original_ستون = src.ستون(indices[idx]);
        Mat sorted_ستون = dst.ستون((صحیح)idx);
        original_ستون.copy_مقصد(sorted_ستون);
    }
}

ساکن Mat ترتیب_دو_میٹرکس_Columns_By_Indices(داخلہ_Array src, داخلہ_Array indices) {
    Mat dst;
    ترتیب_دو_میٹرکس_Columns_By_Indices(src, indices, dst);
    واپس dst;
}


سانچہ<قسم_نام _Tp> ساکن منطقی
is_متشاکل(داخلہ_Array src) {
    Mat _src = src.getMat();
    اگر(_src.ستون_تعداد != _src.صفیں)
        واپس جھوٹ;
    برائے (صحیح i = 0; i < _src.صفیں; i++) {
        برائے (صحیح j = 0; j < _src.ستون_تعداد; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            اگر (a != b) {
                واپس جھوٹ;
            }
        }
    }
    واپس سچ;
}

سانچہ<قسم_نام _Tp> ساکن منطقی
is_متشاکل(داخلہ_Array src, دوہرا eps) {
    Mat _src = src.getMat();
    اگر(_src.ستون_تعداد != _src.صفیں)
        واپس جھوٹ;
    برائے (صحیح i = 0; i < _src.صفیں; i++) {
        برائے (صحیح j = 0; j < _src.ستون_تعداد; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            اگر (std::مطلق_قدر(a - b) > eps) {
                واپس جھوٹ;
            }
        }
    }
    واپس سچ;
}

ساکن منطقی is_متشاکل_2(داخلہ_Array src, دوہرا eps=1e-16)
{
    Mat m = src.getMat();
    انتخاب (m.type()) {
        حالت CV_8SC1: واپس is_متشاکل<حرف>(m); توڑو;
        حالت CV_8UC1:
            واپس is_متشاکل<غیرعلامتی حرف>(m); توڑو;
        حالت CV_16SC1:
            واپس is_متشاکل<مختصر>(m); توڑو;
        حالت CV_16UC1:
            واپس is_متشاکل<غیرعلامتی مختصر>(m); توڑو;
        حالت CV_32SC1:
            واپس is_متشاکل<صحیح>(m); توڑو;
        حالت CV_32FC1:
            واپس is_متشاکل<اعشاری>(m, eps); توڑو;
        حالت CV_64FC1:
            واپس is_متشاکل<دوہرا>(m, eps); توڑو;
        طےشدہ:
            توڑو;
    }
    واپس جھوٹ;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(داخلہ_Array _W, داخلہ_Array اوسط, داخلہ_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat اوسط_2 = اوسط.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحیح n = src.صفیں;
    صحیح d = src.ستون_تعداد;
    // make sure the data has the correct shape
    اگر(W.صفیں != d) {
        String error_پیغام = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.صفیں, src.ستون_تعداد, W.صفیں, W.ستون_تعداد);
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // make sure mean is correct if not empty
    اگر(!اوسط_2.empty() && (اوسط_2.کل() != (size_t) d)) {
        String error_پیغام = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, اوسط_2.کل());
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_مقصد(X, W.type());
    // safe to do, because of above assertion
    اگر(!اوسط_2.empty()) {
        برائے(صحیح i=0; i<n; i++) {
            Mat r_i = X.صف(i);
            تفریق(r_i, اوسط_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    واپس Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(داخلہ_Array _W, داخلہ_Array اوسط, داخلہ_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat اوسط_2 = اوسط.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحیح n = src.صفیں;
    صحیح d = src.ستون_تعداد;
    // make sure the data has the correct shape
    اگر(W.ستون_تعداد != d) {
        String error_پیغام = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.صفیں, src.ستون_تعداد, W.صفیں, W.ستون_تعداد);
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // make sure mean is correct if not empty
    اگر(!اوسط_2.empty() && (اوسط_2.کل() != (size_t) W.صفیں)) {
        String error_پیغام = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.ستون_تعداد, اوسط_2.کل());
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_مقصد(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    اگر(!اوسط_2.empty()) {
        برائے(صحیح i=0; i<n; i++) {
            Mat r_i = X.صف(i);
            جوڑو(r_i, اوسط_2.reshape(1,1), r_i);
        }
    }
    واپس X;
}


جماعت EigenvalueDecomposition {
نجی:

    // Holds the data dimension.
    صحیح n;

    // Pointer to internal memory.
    دوہرا *d, *e, *ort;
    دوہرا **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    سانچہ<قسم_نام _Tp>
    _Tp *alloc_1d(صحیح m) {
        واپس نیا _Tp[m];
    }

    // Allocates memory.
    سانچہ<قسم_نام _Tp>
    _Tp *alloc_1d(صحیح m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        برائے (صحیح i = 0; i < m; i++)
            arr[i] = val;
        واپس arr;
    }

    // Allocates memory.
    سانچہ<قسم_نام _Tp>
    _Tp **alloc_2d(صحیح m, صحیح _n) {
        _Tp **arr = نیا _Tp*[m];
        برائے (صحیح i = 0; i < m; i++)
            arr[i] = نیا _Tp[_n];
        واپس arr;
    }

    // Allocates memory.
    سانچہ<قسم_نام _Tp>
    _Tp **alloc_2d(صحیح m, صحیح _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        برائے (صحیح i = 0; i < m; i++) {
            برائے (صحیح j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        واپس arr;
    }

    ساکن خالی complex_div(دوہرا xr, دوہرا xi, دوہرا yr, دوہرا yi, دوہرا& cdivr, دوہرا& cdivi) {
        دوہرا r, dv;
        CV_DbgAssert(std::مطلق_قدر(yr) + std::مطلق_قدر(yi) > 0.0);
        اگر (std::مطلق_قدر(yr) > std::مطلق_قدر(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ورنہ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    خالی hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ثابت صحیح max_iters_گنتی = 1000 * یہ->n;

        ثابت صحیح nn = یہ->n; CV_Assert(nn > 0);
        صحیح n1 = nn - 1;
        ثابت صحیح کم = 0;
        ثابت صحیح بلند = nn - 1;
        ثابت دوہرا eps = std::numeric_limits<دوہرا>::epsilon();
        دوہرا exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        دوہرا norm = 0.0;
        برائے (صحیح i = 0; i < nn; i++) {
#اگر 0 // 'if' condition is always false
            اگر (i < کم || i > بلند) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#اگر_ختم
            برائے (صحیح j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::مطلق_قدر(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        صحیح iter = 0;
        جب (n1 >= کم) {

            // Look for single small sub-diagonal element
            صحیح l = n1;
            جب (l > کم) {
                اگر (norm < FLT_EPSILON) {
                    توڑو;
                }
                دوہرا s = std::مطلق_قدر(H[l - 1][l - 1]) + std::مطلق_قدر(H[l][l]);
                اگر (s == 0.0) {
                    s = norm;
                }
                اگر (std::مطلق_قدر(H[l][l - 1]) < eps * s) {
                    توڑو;
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

            } ورنہ اگر (l == n1 - 1) {
                // Two roots found
                دوہرا w = H[n1][n1 - 1] * H[n1 - 1][n1];
                دوہرا p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                دوہرا q = p * p + w;
                دوہرا z = std::جذر(std::مطلق_قدر(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                دوہرا x = H[n1][n1];

                اگر (q >= 0) {
                    // Real pair
                    اگر (p >= 0) {
                        z = p + z;
                    } ورنہ {
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
                    دوہرا s = std::مطلق_قدر(x) + std::مطلق_قدر(z);
                    p = x / s;
                    q = z / s;
                    دوہرا r = std::جذر(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    برائے (صحیح j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    برائے (صحیح i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    برائے (صحیح i = کم; i <= بلند; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ورنہ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ورنہ {
                // No convergence yet

                // Form shift
                دوہرا x = H[n1][n1];
                دوہرا y = 0.0;
                دوہرا w = 0.0;
                اگر (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                اگر (iter == 10) {
                    exshift += x;
                    برائے (صحیح i = کم; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    دوہرا s = std::مطلق_قدر(H[n1][n1 - 1]) + std::مطلق_قدر(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                اگر (iter == 30) {
                    دوہرا s = (y - x) * 0.5;
                    s = s * s + w;
                    اگر (s > 0) {
                        s = std::جذر(s);
                        اگر (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        برائے (صحیح i = کم; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                اگر (iter > max_iters_گنتی)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                دوہرا p = std::numeric_limits<دوہرا>::quiet_NaN();
                دوہرا q = std::numeric_limits<دوہرا>::quiet_NaN();
                دوہرا r = std::numeric_limits<دوہرا>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                صحیح m = n1 - 2;
                جب (m >= l) {
                    دوہرا z = H[m][m];
                    r = x - z;
                    دوہرا s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::مطلق_قدر(p) + std::مطلق_قدر(q) + std::مطلق_قدر(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    اگر (m == l) {
                        توڑو;
                    }
                    اگر (std::مطلق_قدر(H[m][m - 1]) * (std::مطلق_قدر(q) + std::مطلق_قدر(r)) < eps * (std::مطلق_قدر(p)
                                                                                     * (std::مطلق_قدر(H[m - 1][m - 1]) + std::مطلق_قدر(z) + std::مطلق_قدر(
                                                                                                                                           H[m + 1][m + 1])))) {
                        توڑو;
                    }
                    m--;
                }

                برائے (صحیح i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    اگر (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                برائے (صحیح k = m; k < n1; k++) {

                    منطقی notlast = (k != n1 - 1);
                    اگر (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::مطلق_قدر(p) + std::مطلق_قدر(q) + std::مطلق_قدر(r);
                        اگر (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    اگر (x == 0.0) {
                        توڑو;
                    }
                    دوہرا s = std::جذر(p * p + q * q + r * r);
                    اگر (p < 0) {
                        s = -s;
                    }
                    اگر (s != 0) {
                        اگر (k != m) {
                            H[k][k - 1] = -s * x;
                        } ورنہ اگر (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        دوہرا z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        برائے (صحیح j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            اگر (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        برائے (صحیح i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            اگر (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        برائے (صحیح i = کم; i <= بلند; i++) {
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
            واپس;
        }

        برائے (n1 = nn - 1; n1 >= 0; n1--) {
            دوہرا p = d[n1];
            دوہرا q = e[n1];

            اگر (q == 0) {
                // Real vector
                دوہرا z = std::numeric_limits<دوہرا>::quiet_NaN();
                دوہرا s = std::numeric_limits<دوہرا>::quiet_NaN();

                صحیح l = n1;
                H[n1][n1] = 1.0;
                برائے (صحیح i = n1 - 1; i >= 0; i--) {
                    دوہرا w = H[i][i] - p;
                    دوہرا r = 0.0;
                    برائے (صحیح j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    اگر (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ورنہ {
                        l = i;
                        اگر (e[i] == 0.0) {
                            اگر (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ورنہ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ورنہ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            دوہرا x = H[i][i + 1];
                            دوہرا y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            دوہرا t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            اگر (std::مطلق_قدر(x) > std::مطلق_قدر(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ورنہ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        دوہرا t = std::مطلق_قدر(H[i][n1]);
                        اگر ((eps * t) * t > 1) {
                            دوہرا inv_t = 1.0 / t;
                            برائے (صحیح j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ورنہ اگر (q < 0) {
                // Complex vector
                دوہرا z = std::numeric_limits<دوہرا>::quiet_NaN();
                دوہرا r = std::numeric_limits<دوہرا>::quiet_NaN();
                دوہرا s = std::numeric_limits<دوہرا>::quiet_NaN();

                صحیح l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                اگر (std::مطلق_قدر(H[n1][n1 - 1]) > std::مطلق_قدر(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ورنہ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                برائے (صحیح i = n1 - 2; i >= 0; i--) {
                    دوہرا ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    برائے (صحیح j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    دوہرا w = H[i][i] - p;

                    اگر (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ورنہ {
                        l = i;
                        اگر (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ورنہ {
                            // Solve complex equations

                            دوہرا x = H[i][i + 1];
                            دوہرا y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            اگر (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::مطلق_قدر(w) + std::مطلق_قدر(q) + std::مطلق_قدر(x)
                                                   + std::مطلق_قدر(y) + std::مطلق_قدر(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            اگر (std::مطلق_قدر(x) > (std::مطلق_قدر(z) + std::مطلق_قدر(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ورنہ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        دوہرا t = std::max(std::مطلق_قدر(H[i][n1 - 1]), std::مطلق_قدر(H[i][n1]));
                        اگر ((eps * t) * t > 1) {
                            برائے (صحیح j = i; j <= n1; j++) {
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
        برائے (صحیح i = 0; i < nn; i++) {
            اگر (i < کم || i > بلند) {
                برائے (صحیح j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#اگر_ختم

        // Back transformation to get eigenvectors of original matrix

        برائے (صحیح j = nn - 1; j >= کم; j--) {
            برائے (صحیح i = کم; i <= بلند; i++) {
                دوہرا z = 0.0;
                برائے (صحیح k = کم; k <= std::min(j, بلند); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    خالی orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        صحیح کم = 0;
        صحیح بلند = n - 1;

        برائے (صحیح m = کم + 1; m < بلند; m++) {

            // Scale column.

            دوہرا scale = 0.0;
            برائے (صحیح i = m; i <= بلند; i++) {
                scale = scale + std::مطلق_قدر(H[i][m - 1]);
            }
            اگر (scale != 0.0) {

                // Compute Householder transformation.

                دوہرا h = 0.0;
                برائے (صحیح i = بلند; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                دوہرا g = std::جذر(h);
                اگر (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                برائے (صحیح j = m; j < n; j++) {
                    دوہرا f = 0.0;
                    برائے (صحیح i = بلند; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    برائے (صحیح i = m; i <= بلند; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                برائے (صحیح i = 0; i <= بلند; i++) {
                    دوہرا f = 0.0;
                    برائے (صحیح j = بلند; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    برائے (صحیح j = m; j <= بلند; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        برائے (صحیح i = 0; i < n; i++) {
            برائے (صحیح j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        برائے (صحیح m = بلند - 1; m > کم; m--) {
            اگر (H[m][m - 1] != 0.0) {
                برائے (صحیح i = m + 1; i <= بلند; i++) {
                    ort[i] = H[i][m - 1];
                }
                برائے (صحیح j = m; j <= بلند; j++) {
                    دوہرا g = 0.0;
                    برائے (صحیح i = m; i <= بلند; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    برائے (صحیح i = m; i <= بلند; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    خالی release() {
        // releases the working data
        حذف[] d; d = تہی;
        حذف[] e; e = تہی;
        حذف[] ort; ort = تہی;
        برائے (صحیح i = 0; i < n; i++) {
            اگر (H) حذف[] H[i];
            اگر (V) حذف[] V[i];
        }
        حذف[] H; H = تہی;
        حذف[] V; V = تہی;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    خالی compute() {
        // Allocate memory for the working data.
        V = alloc_2d<دوہرا> (n, n, 0.0);
        d = alloc_1d<دوہرا> (n);
        e = alloc_1d<دوہرا> (n);
        ort = alloc_1d<دوہرا> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            برائے (صحیح i = 0; i < n; i++) {
                _eigenvalues.at<دوہرا> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            برائے (صحیح i = 0; i < n; i++)
                برائے (صحیح j = 0; j < n; j++)
                    _eigenvectors.at<دوہرا> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

عوامی:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(تہی), e(تہی), ort(تہی),
        V(تہی), H(تہی)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    خالی compute(داخلہ_Array src, منطقی fallback_متشاکل = سچ)
    {
        CV_INSTRUMENT_REGION();

        اگر(fallback_متشاکل && is_متشاکل_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ورنہ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_مقصد(tmp, CV_64FC1);
            // Get dimension of the matrix.
            یہ->n = tmp.ستون_تعداد;
            // Allocate the matrix data to work on.
            یہ->H = alloc_2d<دوہرا> (n, n);
            // Now safely copy the data.
            برائے (صحیح i = 0; i < tmp.صفیں; i++) {
                برائے (صحیح j = 0; j < tmp.ستون_تعداد; j++) {
                    یہ->H[i][j] = tmp.at<دوہرا>(i, j);
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
    Mat eigenvalues() ثابت { واپس _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ثابت { واپس _eigenvectors; }
};

خالی eigen_Non_متشاکل(داخلہ_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحیح type = src.type();
    صحیح n = src.صفیں;

    CV_Assert(src.صفیں == src.ستون_تعداد);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    اگر (type == CV_32F)
        src.convert_مقصد(src64f, CV_32FC1);
    ورنہ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, جھوٹ);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<دوہرا> eigenvalues64f;
    eigensystem.eigenvalues().copy_مقصد(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<صحیح> ترتیب_دو_indexes(n);
    cv::ترتیب_دو_Idx(eigenvalues64f, ترتیب_دو_indexes, ترتیب_دو_EVERY_صف | ترتیب_دو_DESCENDING);

    std::vector<دوہرا> sorted_eigenvalues64f(n);
    برائے (صحیح i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[ترتیب_دو_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_مقصد(_evals, type);

    اگر( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.صفیں == n);
        CV_Assert(eigenvectors64f.ستون_تعداد == n);
        Mat_<دوہرا> sorted_eigenvectors64f(n, n, CV_64FC1);
        برائے (صحیح i = 0; i < n; i++)
        {
            دوہرا* pDst = sorted_eigenvectors64f.ptr<دوہرا>(i);
            دوہرا* pSrc = eigenvectors64f.ptr<دوہرا>(ترتیب_دو_indexes[i]);
            CV_Assert(pSrc != تہی);
            حافظہ_نقل(pDst, pSrc, n * حجم(دوہرا));
        }
        sorted_eigenvectors64f.convert_مقصد(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(صحیح عدد_components) : عدد_components_2(عدد_components) { }

LDA::LDA(داخلہ_Array_کا_Arrays src, داخلہ_Array labels, صحیح عدد_components) : عدد_components_2(عدد_components)
{
    یہ->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

خالی LDA::save(ثابت String& filename) ثابت
{
    FileStorage fs(filename, FileStorage::WRITE);
    اگر (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    یہ->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
خالی LDA::load(ثابت String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    اگر (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    یہ->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
خالی LDA::save(FileStorage& fs) ثابت {
    // write matrices
    fs << "num_components" << عدد_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
خالی LDA::load(ثابت FileStorage& fs) {
    //read matrices
    fs["num_components"] >> عدد_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

خالی LDA::lda(داخلہ_Array_کا_Arrays _src, داخلہ_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<صحیح> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        برائے(غیرعلامتی صحیح i = 0; i < tmp.کل(); i++) {
            labels.push_back(tmp.at<صحیح>(i));
        }
    }
    // turn into row sampled matrix
    Mat ڈیٹا;
    // ensure working matrix is double precision
    src.convert_مقصد(ڈیٹا, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<صحیح> mapped_labels(labels.size());
    std::vector<صحیح> num2label = remove_dups(labels);
    std::map<صحیح, صحیح> label2num;
    برائے (صحیح i = 0; i < (صحیح)num2label.size(); i++)
        label2num[num2label[i]] = i;
    برائے (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    صحیح N = ڈیٹا.صفیں;
    صحیح D = ڈیٹا.ستون_تعداد;
    // number of unique labels
    صحیح C = (صحیح)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    اگر(C == 1) {
        String error_پیغام = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // throw error if less labels, than samples
    اگر (labels.size() != static_cast<size_t>(N)) {
        String error_پیغام = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_پیغام);
    }
    // warn if within-classes scatter matrix becomes singular
    اگر (N < D) {
        std::خارج << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::سطر_ختم;
    }
    // clip number of components to be a valid number
    اگر ((عدد_components_2 <= 0) || (عدد_components_2 >= C)) {
        عدد_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat اوسط_کل = Mat::zeros(1, D, ڈیٹا.type());
    // holds the mean for each class
    std::vector<Mat> اوسط_Class(C);
    std::vector<صحیح> عدد_Class(C);
    // initialize
    برائے (صحیح i = 0; i < C; i++) {
        عدد_Class[i] = 0;
        اوسط_Class[i] = Mat::zeros(1, D, ڈیٹا.type()); //! Dx1 image vector
    }
    // calculate sums
    برائے (صحیح i = 0; i < N; i++) {
        Mat instance = ڈیٹا.صف(i);
        صحیح classIdx = mapped_labels[i];
        جوڑو(اوسط_کل, instance, اوسط_کل);
        جوڑو(اوسط_Class[classIdx], instance, اوسط_Class[classIdx]);
        عدد_Class[classIdx]++;
    }
    // calculate total mean
    اوسط_کل.convert_مقصد(اوسط_کل, اوسط_کل.type(), 1.0 / static_cast<دوہرا> (N));
    // calculate class means
    برائے (صحیح i = 0; i < C; i++) {
        اوسط_Class[i].convert_مقصد(اوسط_Class[i], اوسط_Class[i].type(), 1.0 / static_cast<دوہرا> (عدد_Class[i]));
    }
    // subtract class means
    برائے (صحیح i = 0; i < N; i++) {
        صحیح classIdx = mapped_labels[i];
        Mat instance = ڈیٹا.صف(i);
        تفریق(instance, اوسط_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, ڈیٹا.type());
    mulTransposed(ڈیٹا, Sw, سچ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, ڈیٹا.type());
    برائے (صحیح i = 0; i < C; i++) {
        Mat tmp;
        تفریق(اوسط_Class[i], اوسط_کل, tmp);
        mulTransposed(tmp, tmp, سچ);
        جوڑو(Sb, tmp, Sb);
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
    std::vector<صحیح> sorted_indices = argsort(_eigenvalues, جھوٹ);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = ترتیب_دو_میٹرکس_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = ترتیب_دو_میٹرکس_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, عدد_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, عدد_components_2));
}

خالی LDA::compute(داخلہ_Array_کا_Arrays _src, داخلہ_Array _lbls) {
    انتخاب(_src.قسم()) {
    حالت داخلہ_Array_2::STD_سمتیہ_MAT:
    حالت داخلہ_Array_2::STD_ARRAY_MAT:
        lda(as_صف_میٹرکس(_src, CV_64FC1), _lbls);
        توڑو;
    حالت داخلہ_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        توڑو;
    طےشدہ:
        String error_پیغام= format("InputArray Datatype %d is not supported.", _src.قسم());
        CV_Error(Error::StsBadArg, error_پیغام);
        توڑو;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(داخلہ_Array src) {
   واپس subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(داخلہ_Array src) {
   واپس subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
