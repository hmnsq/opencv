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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <iostream>
#अन्तर्भाव <map>
#अन्तर्भाव <set>

नामाकाश cv
{

// Removes duplicate elements in a given vector.
ढाँचा<typename _Tp>
ओळीत std::vector<_Tp> remove_dups(स्थिरांक std::vector<_Tp>& src) {
    प्रकारव्याख्या typename std::set<_Tp>::const_iterator constSetIterator;
    प्रकारव्याख्या typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    साठी (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    साठी (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    परत elems;
}

स्थिर Mat argsort(आदान_Array _src, तार्किक ascending=सत्य)
{
    Mat src = _src.getMat();
    जर (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    पूर्णांक flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    परत sorted_indices;
}

स्थिर Mat asRowMatrix(आदान_Array_Of_Arrays src, पूर्णांक rtype, द्विगुण alpha=1, द्विगुण beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    जर(src.kind() != आदान_Array_2::STD_VECTOR_MAT && src.kind() != आदान_Array_2::STD_ARRAY_MAT &&
        src.kind() != आदान_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    माप_प्रकार n = src.total();
    // return empty matrix if no matrices given
    जर(n == 0)
        परत Mat();
    // dimensionality of (reshaped) samples
    माप_प्रकार d = src.getMat(0).total();
    // create data matrix
    Mat data((पूर्णांक)n, (पूर्णांक)d, rtype);
    // now copy data
    साठी(पूर्णांक i = 0; i < (पूर्णांक)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        जर(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (पूर्णांक)d, (पूर्णांक)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        जर(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } अन्यथा {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    परत data;
}

स्थिर शून्य sortMatrixColumnsByIndices(आदान_Array _src, आदान_Array _indices, OutputArray _dst) {
    जर(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<पूर्णांक> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    साठी(माप_प्रकार idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((पूर्णांक)idx);
        originalCol.copyTo(sortedCol);
    }
}

स्थिर Mat sortMatrixColumnsByIndices(आदान_Array src, आदान_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    परत dst;
}


ढाँचा<typename _Tp> स्थिर तार्किक
isSymmetric_(आदान_Array src) {
    Mat _src = src.getMat();
    जर(_src.cols != _src.rows)
        परत असत्य;
    साठी (पूर्णांक i = 0; i < _src.rows; i++) {
        साठी (पूर्णांक j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            जर (a != b) {
                परत असत्य;
            }
        }
    }
    परत सत्य;
}

ढाँचा<typename _Tp> स्थिर तार्किक
isSymmetric_(आदान_Array src, द्विगुण eps) {
    Mat _src = src.getMat();
    जर(_src.cols != _src.rows)
        परत असत्य;
    साठी (पूर्णांक i = 0; i < _src.rows; i++) {
        साठी (पूर्णांक j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            जर (std::असल(a - b) > eps) {
                परत असत्य;
            }
        }
    }
    परत सत्य;
}

स्थिर तार्किक isSymmetric(आदान_Array src, द्विगुण eps=1e-16)
{
    Mat m = src.getMat();
    निवड (m.type()) {
        स्थिती CV_8SC1: परत isSymmetric_<अक्षर>(m); थांबा;
        स्थिती CV_8UC1:
            परत isSymmetric_<अचिन्हित अक्षर>(m); थांबा;
        स्थिती CV_16SC1:
            परत isSymmetric_<लघु>(m); थांबा;
        स्थिती CV_16UC1:
            परत isSymmetric_<अचिन्हित लघु>(m); थांबा;
        स्थिती CV_32SC1:
            परत isSymmetric_<पूर्णांक>(m); थांबा;
        स्थिती CV_32FC1:
            परत isSymmetric_<दशांश>(m, eps); थांबा;
        स्थिती CV_64FC1:
            परत isSymmetric_<द्विगुण>(m, eps); थांबा;
        पूर्वनिर्धारित:
            थांबा;
    }
    परत असत्य;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(आदान_Array _W, आदान_Array _mean, आदान_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णांक n = src.rows;
    पूर्णांक d = src.cols;
    // make sure the data has the correct shape
    जर(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    जर(!mean.empty() && (mean.total() != (माप_प्रकार) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    जर(!mean.empty()) {
        साठी(पूर्णांक i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    परत Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(आदान_Array _W, आदान_Array _mean, आदान_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णांक n = src.rows;
    पूर्णांक d = src.cols;
    // make sure the data has the correct shape
    जर(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    जर(!mean.empty() && (mean.total() != (माप_प्रकार) W.rows)) {
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
    जर(!mean.empty()) {
        साठी(पूर्णांक i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    परत X;
}


वर्ग EigenvalueDecomposition {
खाजगी:

    // Holds the data dimension.
    पूर्णांक n;

    // Pointer to internal memory.
    द्विगुण *d, *e, *ort;
    द्विगुण **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp *alloc_1d(पूर्णांक m) {
        परत नवीन _Tp[m];
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp *alloc_1d(पूर्णांक m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        साठी (पूर्णांक i = 0; i < m; i++)
            arr[i] = val;
        परत arr;
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp **alloc_2d(पूर्णांक m, पूर्णांक _n) {
        _Tp **arr = नवीन _Tp*[m];
        साठी (पूर्णांक i = 0; i < m; i++)
            arr[i] = नवीन _Tp[_n];
        परत arr;
    }

    // Allocates memory.
    ढाँचा<typename _Tp>
    _Tp **alloc_2d(पूर्णांक m, पूर्णांक _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        साठी (पूर्णांक i = 0; i < m; i++) {
            साठी (पूर्णांक j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        परत arr;
    }

    स्थिर शून्य complex_div(द्विगुण xr, द्विगुण xi, द्विगुण yr, द्विगुण yi, द्विगुण& cdivr, द्विगुण& cdivi) {
        द्विगुण r, dv;
        CV_DbgAssert(std::असल(yr) + std::असल(yi) > 0.0);
        जर (std::असल(yr) > std::असल(yi)) {
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

    शून्य hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        स्थिरांक पूर्णांक max_iters_मोजणी = 1000 * हे->n;

        स्थिरांक पूर्णांक nn = हे->n; CV_Assert(nn > 0);
        पूर्णांक n1 = nn - 1;
        स्थिरांक पूर्णांक खालचा = 0;
        स्थिरांक पूर्णांक वरचा = nn - 1;
        स्थिरांक द्विगुण eps = std::numeric_limits<द्विगुण>::epsilon();
        द्विगुण exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        द्विगुण norm = 0.0;
        साठी (पूर्णांक i = 0; i < nn; i++) {
#जर 0 // 'if' condition is always false
            जर (i < खालचा || i > वरचा) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            साठी (पूर्णांक j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::असल(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        पूर्णांक iter = 0;
        पर्यंत (n1 >= खालचा) {

            // Look for single small sub-diagonal element
            पूर्णांक l = n1;
            पर्यंत (l > खालचा) {
                जर (norm < भग्न_अंतर) {
                    थांबा;
                }
                द्विगुण s = std::असल(H[l - 1][l - 1]) + std::असल(H[l][l]);
                जर (s == 0.0) {
                    s = norm;
                }
                जर (std::असल(H[l][l - 1]) < eps * s) {
                    थांबा;
                }
                l--;
            }

            // Check for convergence
            जर (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } अन्यथा जर (l == n1 - 1) {
                // Two roots found
                द्विगुण w = H[n1][n1 - 1] * H[n1 - 1][n1];
                द्विगुण p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                द्विगुण q = p * p + w;
                द्विगुण z = std::वर्ग_मूल(std::असल(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                द्विगुण x = H[n1][n1];

                जर (q >= 0) {
                    // Real pair
                    जर (p >= 0) {
                        z = p + z;
                    } अन्यथा {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    जर (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    द्विगुण s = std::असल(x) + std::असल(z);
                    p = x / s;
                    q = z / s;
                    द्विगुण r = std::वर्ग_मूल(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    साठी (पूर्णांक j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    साठी (पूर्णांक i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    साठी (पूर्णांक i = खालचा; i <= वरचा; i++) {
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
                द्विगुण x = H[n1][n1];
                द्विगुण y = 0.0;
                द्विगुण w = 0.0;
                जर (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                जर (iter == 10) {
                    exshift += x;
                    साठी (पूर्णांक i = खालचा; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    द्विगुण s = std::असल(H[n1][n1 - 1]) + std::असल(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                जर (iter == 30) {
                    द्विगुण s = (y - x) * 0.5;
                    s = s * s + w;
                    जर (s > 0) {
                        s = std::वर्ग_मूल(s);
                        जर (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        साठी (पूर्णांक i = खालचा; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                जर (iter > max_iters_मोजणी)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                द्विगुण p = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण q = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण r = std::numeric_limits<द्विगुण>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                पूर्णांक m = n1 - 2;
                पर्यंत (m >= l) {
                    द्विगुण z = H[m][m];
                    r = x - z;
                    द्विगुण s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::असल(p) + std::असल(q) + std::असल(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    जर (m == l) {
                        थांबा;
                    }
                    जर (std::असल(H[m][m - 1]) * (std::असल(q) + std::असल(r)) < eps * (std::असल(p)
                                                                                     * (std::असल(H[m - 1][m - 1]) + std::असल(z) + std::असल(
                                                                                                                                           H[m + 1][m + 1])))) {
                        थांबा;
                    }
                    m--;
                }

                साठी (पूर्णांक i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    जर (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                साठी (पूर्णांक k = m; k < n1; k++) {

                    तार्किक notlast = (k != n1 - 1);
                    जर (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::असल(p) + std::असल(q) + std::असल(r);
                        जर (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    जर (x == 0.0) {
                        थांबा;
                    }
                    द्विगुण s = std::वर्ग_मूल(p * p + q * q + r * r);
                    जर (p < 0) {
                        s = -s;
                    }
                    जर (s != 0) {
                        जर (k != m) {
                            H[k][k - 1] = -s * x;
                        } अन्यथा जर (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        द्विगुण z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        साठी (पूर्णांक j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            जर (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        साठी (पूर्णांक i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            जर (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        साठी (पूर्णांक i = खालचा; i <= वरचा; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            जर (notlast) {
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

        जर (norm < भग्न_अंतर) {
            परत;
        }

        साठी (n1 = nn - 1; n1 >= 0; n1--) {
            द्विगुण p = d[n1];
            द्विगुण q = e[n1];

            जर (q == 0) {
                // Real vector
                द्विगुण z = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण s = std::numeric_limits<द्विगुण>::quiet_NaN();

                पूर्णांक l = n1;
                H[n1][n1] = 1.0;
                साठी (पूर्णांक i = n1 - 1; i >= 0; i--) {
                    द्विगुण w = H[i][i] - p;
                    द्विगुण r = 0.0;
                    साठी (पूर्णांक j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    जर (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } अन्यथा {
                        l = i;
                        जर (e[i] == 0.0) {
                            जर (w != 0.0) {
                                H[i][n1] = -r / w;
                            } अन्यथा {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } अन्यथा {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            द्विगुण x = H[i][i + 1];
                            द्विगुण y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            द्विगुण t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            जर (std::असल(x) > std::असल(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } अन्यथा {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        द्विगुण t = std::असल(H[i][n1]);
                        जर ((eps * t) * t > 1) {
                            द्विगुण inv_t = 1.0 / t;
                            साठी (पूर्णांक j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } अन्यथा जर (q < 0) {
                // Complex vector
                द्विगुण z = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण r = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण s = std::numeric_limits<द्विगुण>::quiet_NaN();

                पूर्णांक l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                जर (std::असल(H[n1][n1 - 1]) > std::असल(H[n1 - 1][n1])) {
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
                साठी (पूर्णांक i = n1 - 2; i >= 0; i--) {
                    द्विगुण ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    साठी (पूर्णांक j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    द्विगुण w = H[i][i] - p;

                    जर (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } अन्यथा {
                        l = i;
                        जर (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } अन्यथा {
                            // Solve complex equations

                            द्विगुण x = H[i][i + 1];
                            द्विगुण y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            जर (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::असल(w) + std::असल(q) + std::असल(x)
                                                   + std::असल(y) + std::असल(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            जर (std::असल(x) > (std::असल(z) + std::असल(q))) {
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

                        द्विगुण t = std::max(std::असल(H[i][n1 - 1]), std::असल(H[i][n1]));
                        जर ((eps * t) * t > 1) {
                            साठी (पूर्णांक j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#जर 0 // 'if' condition is always false
        साठी (पूर्णांक i = 0; i < nn; i++) {
            जर (i < खालचा || i > वरचा) {
                साठी (पूर्णांक j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        साठी (पूर्णांक j = nn - 1; j >= खालचा; j--) {
            साठी (पूर्णांक i = खालचा; i <= वरचा; i++) {
                द्विगुण z = 0.0;
                साठी (पूर्णांक k = खालचा; k <= std::min(j, वरचा); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    शून्य orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        पूर्णांक खालचा = 0;
        पूर्णांक वरचा = n - 1;

        साठी (पूर्णांक m = खालचा + 1; m < वरचा; m++) {

            // Scale column.

            द्विगुण scale = 0.0;
            साठी (पूर्णांक i = m; i <= वरचा; i++) {
                scale = scale + std::असल(H[i][m - 1]);
            }
            जर (scale != 0.0) {

                // Compute Householder transformation.

                द्विगुण h = 0.0;
                साठी (पूर्णांक i = वरचा; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                द्विगुण g = std::वर्ग_मूल(h);
                जर (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                साठी (पूर्णांक j = m; j < n; j++) {
                    द्विगुण f = 0.0;
                    साठी (पूर्णांक i = वरचा; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    साठी (पूर्णांक i = m; i <= वरचा; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                साठी (पूर्णांक i = 0; i <= वरचा; i++) {
                    द्विगुण f = 0.0;
                    साठी (पूर्णांक j = वरचा; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    साठी (पूर्णांक j = m; j <= वरचा; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        साठी (पूर्णांक i = 0; i < n; i++) {
            साठी (पूर्णांक j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        साठी (पूर्णांक m = वरचा - 1; m > खालचा; m--) {
            जर (H[m][m - 1] != 0.0) {
                साठी (पूर्णांक i = m + 1; i <= वरचा; i++) {
                    ort[i] = H[i][m - 1];
                }
                साठी (पूर्णांक j = m; j <= वरचा; j++) {
                    द्विगुण g = 0.0;
                    साठी (पूर्णांक i = m; i <= वरचा; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    साठी (पूर्णांक i = m; i <= वरचा; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    शून्य release() {
        // releases the working data
        delete[] d; d = रिक्त;
        delete[] e; e = रिक्त;
        delete[] ort; ort = रिक्त;
        साठी (पूर्णांक i = 0; i < n; i++) {
            जर (H) delete[] H[i];
            जर (V) delete[] V[i];
        }
        delete[] H; H = रिक्त;
        delete[] V; V = रिक्त;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    शून्य compute() {
        // Allocate memory for the working data.
        V = alloc_2d<द्विगुण> (n, n, 0.0);
        d = alloc_1d<द्विगुण> (n);
        e = alloc_1d<द्विगुण> (n);
        ort = alloc_1d<द्विगुण> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            साठी (पूर्णांक i = 0; i < n; i++) {
                _eigenvalues.at<द्विगुण> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            साठी (पूर्णांक i = 0; i < n; i++)
                साठी (पूर्णांक j = 0; j < n; j++)
                    _eigenvectors.at<द्विगुण> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

सार्वजनिक:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(रिक्त), e(रिक्त), ort(रिक्त),
        V(रिक्त), H(रिक्त)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    शून्य compute(आदान_Array src, तार्किक fallbackSymmetric = सत्य)
    {
        CV_INSTRUMENT_REGION();

        जर(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } अन्यथा {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            हे->n = tmp.cols;
            // Allocate the matrix data to work on.
            हे->H = alloc_2d<द्विगुण> (n, n);
            // Now safely copy the data.
            साठी (पूर्णांक i = 0; i < tmp.rows; i++) {
                साठी (पूर्णांक j = 0; j < tmp.cols; j++) {
                    हे->H[i][j] = tmp.at<द्विगुण>(i, j);
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
    Mat eigenvalues() स्थिरांक { परत _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() स्थिरांक { परत _eigenvectors; }
};

शून्य eigenNonSymmetric(आदान_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    पूर्णांक n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    जर (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    अन्यथा
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, असत्य);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<द्विगुण> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (माप_प्रकार)n);

    std::vector<पूर्णांक> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<द्विगुण> sorted_eigenvalues64f(n);
    साठी (पूर्णांक i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    जर( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<द्विगुण> sorted_eigenvectors64f(n, n, CV_64FC1);
        साठी (पूर्णांक i = 0; i < n; i++)
        {
            द्विगुण* pDst = sorted_eigenvectors64f.ptr<द्विगुण>(i);
            द्विगुण* pSrc = eigenvectors64f.ptr<द्विगुण>(sort_indexes[i]);
            CV_Assert(pSrc != रिक्त);
            स_नकल(pDst, pSrc, n * आकार(द्विगुण));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(पूर्णांक अंक_components) : अंक_components_2(अंक_components) { }

LDA::LDA(आदान_Array_Of_Arrays src, आदान_Array labels, पूर्णांक अंक_components) : अंक_components_2(अंक_components)
{
    हे->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

शून्य LDA::save(स्थिरांक String& filename) स्थिरांक
{
    FileStorage fs(filename, FileStorage::WRITE);
    जर (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    हे->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
शून्य LDA::load(स्थिरांक String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    जर (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    हे->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
शून्य LDA::save(FileStorage& fs) स्थिरांक {
    // write matrices
    fs << "num_components" << अंक_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
शून्य LDA::load(स्थिरांक FileStorage& fs) {
    //read matrices
    fs["num_components"] >> अंक_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

शून्य LDA::lda(आदान_Array_Of_Arrays _src, आदान_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<पूर्णांक> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        साठी(अचिन्हित पूर्णांक i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<पूर्णांक>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<पूर्णांक> mapped_labels(labels.size());
    std::vector<पूर्णांक> num2label = remove_dups(labels);
    std::map<पूर्णांक, पूर्णांक> label2num;
    साठी (पूर्णांक i = 0; i < (पूर्णांक)num2label.size(); i++)
        label2num[num2label[i]] = i;
    साठी (माप_प्रकार i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    पूर्णांक N = data.rows;
    पूर्णांक D = data.cols;
    // number of unique labels
    पूर्णांक C = (पूर्णांक)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    जर(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    जर (labels.size() != static_cast<माप_प्रकार>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    जर (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    जर ((अंक_components_2 <= 0) || (अंक_components_2 >= C)) {
        अंक_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<पूर्णांक> अंक_Class(C);
    // initialize
    साठी (पूर्णांक i = 0; i < C; i++) {
        अंक_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    साठी (पूर्णांक i = 0; i < N; i++) {
        Mat instance = data.row(i);
        पूर्णांक classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        अंक_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<द्विगुण> (N));
    // calculate class means
    साठी (पूर्णांक i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<द्विगुण> (अंक_Class[i]));
    }
    // subtract class means
    साठी (पूर्णांक i = 0; i < N; i++) {
        पूर्णांक classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, सत्य);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    साठी (पूर्णांक i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, सत्य);
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
    std::vector<पूर्णांक> sorted_indices = argsort(_eigenvalues, असत्य);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, अंक_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, अंक_components_2));
}

शून्य LDA::compute(आदान_Array_Of_Arrays _src, आदान_Array _lbls) {
    निवड(_src.kind()) {
    स्थिती आदान_Array_2::STD_VECTOR_MAT:
    स्थिती आदान_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        थांबा;
    स्थिती आदान_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        थांबा;
    पूर्वनिर्धारित:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        थांबा;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(आदान_Array src) {
   परत subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(आदान_Array src) {
   परत subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
