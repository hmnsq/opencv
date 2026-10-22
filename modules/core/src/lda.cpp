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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <iostream>
#অন্তর্ভাব <map>
#অন্তর্ভাব <set>

নামাকাশ cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
অন্তঃস্থ std::vector<_Tp> remove_dups(ধ্রুব std::vector<_Tp>& src) {
    প্রকারনির্দেশ typename std::set<_Tp>::const_iterator constSetIterator;
    প্রকারনির্দেশ typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    প্রতি (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    প্রতি (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    প্রত্যাগম elems;
}

স্থির Mat argsort(প্রবেশ_Array _src, তার্কিক ascending=সত্য)
{
    Mat src = _src.getMat();
    যদি (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    পূর্ণাঙ্ক flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    প্রত্যাগম sorted_indices;
}

স্থির Mat asRowMatrix(প্রবেশ_Array_Of_Arrays src, পূর্ণাঙ্ক rtype, দ্বিগুণ alpha=1, দ্বিগুণ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    যদি(src.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT && src.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
        src.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    যদি(n == 0)
        প্রত্যাগম Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((পূর্ণাঙ্ক)n, (পূর্ণাঙ্ক)d, rtype);
    // now copy data
    প্রতি(পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        যদি(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (পূর্ণাঙ্ক)d, (পূর্ণাঙ্ক)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        যদি(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } অন্যথা {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    প্রত্যাগম data;
}

স্থির শূন্য sortMatrixColumnsByIndices(প্রবেশ_Array _src, প্রবেশ_Array _indices, OutputArray _dst) {
    যদি(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<পূর্ণাঙ্ক> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    প্রতি(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((পূর্ণাঙ্ক)idx);
        originalCol.copyTo(sortedCol);
    }
}

স্থির Mat sortMatrixColumnsByIndices(প্রবেশ_Array src, প্রবেশ_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    প্রত্যাগম dst;
}


template<typename _Tp> স্থির তার্কিক
isSymmetric_(প্রবেশ_Array src) {
    Mat _src = src.getMat();
    যদি(_src.cols != _src.rows)
        প্রত্যাগম অসত্য;
    প্রতি (পূর্ণাঙ্ক i = 0; i < _src.rows; i++) {
        প্রতি (পূর্ণাঙ্ক j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            যদি (a != b) {
                প্রত্যাগম অসত্য;
            }
        }
    }
    প্রত্যাগম সত্য;
}

template<typename _Tp> স্থির তার্কিক
isSymmetric_(প্রবেশ_Array src, দ্বিগুণ eps) {
    Mat _src = src.getMat();
    যদি(_src.cols != _src.rows)
        প্রত্যাগম অসত্য;
    প্রতি (পূর্ণাঙ্ক i = 0; i < _src.rows; i++) {
        প্রতি (পূর্ণাঙ্ক j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            যদি (std::abs(a - b) > eps) {
                প্রত্যাগম অসত্য;
            }
        }
    }
    প্রত্যাগম সত্য;
}

স্থির তার্কিক isSymmetric(প্রবেশ_Array src, দ্বিগুণ eps=1e-16)
{
    Mat m = src.getMat();
    বিকল্প (m.type()) {
        পক্ষ CV_8SC1: প্রত্যাগম isSymmetric_<বর্ণ>(m); ভঙ্গ;
        পক্ষ CV_8UC1:
            প্রত্যাগম isSymmetric_<unsigned বর্ণ>(m); ভঙ্গ;
        পক্ষ CV_16SC1:
            প্রত্যাগম isSymmetric_<short>(m); ভঙ্গ;
        পক্ষ CV_16UC1:
            প্রত্যাগম isSymmetric_<unsigned short>(m); ভঙ্গ;
        পক্ষ CV_32SC1:
            প্রত্যাগম isSymmetric_<পূর্ণাঙ্ক>(m); ভঙ্গ;
        পক্ষ CV_32FC1:
            প্রত্যাগম isSymmetric_<ভিন্ন>(m, eps); ভঙ্গ;
        পক্ষ CV_64FC1:
            প্রত্যাগম isSymmetric_<দ্বিগুণ>(m, eps); ভঙ্গ;
        স্বভাব:
            ভঙ্গ;
    }
    প্রত্যাগম অসত্য;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(প্রবেশ_Array _W, প্রবেশ_Array _mean, প্রবেশ_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    পূর্ণাঙ্ক n = src.rows;
    পূর্ণাঙ্ক d = src.cols;
    // make sure the data has the correct shape
    যদি(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    যদি(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    যদি(!mean.empty()) {
        প্রতি(পূর্ণাঙ্ক i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    প্রত্যাগম Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(প্রবেশ_Array _W, প্রবেশ_Array _mean, প্রবেশ_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    পূর্ণাঙ্ক n = src.rows;
    পূর্ণাঙ্ক d = src.cols;
    // make sure the data has the correct shape
    যদি(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    যদি(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    যদি(!mean.empty()) {
        প্রতি(পূর্ণাঙ্ক i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    প্রত্যাগম X;
}


বর্গ EigenvalueDecomposition {
গুপ্ত:

    // Holds the data dimension.
    পূর্ণাঙ্ক n;

    // Pointer to internal memory.
    দ্বিগুণ *d, *e, *ort;
    দ্বিগুণ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(পূর্ণাঙ্ক m) {
        প্রত্যাগম নূতন _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(পূর্ণাঙ্ক m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        প্রতি (পূর্ণাঙ্ক i = 0; i < m; i++)
            arr[i] = val;
        প্রত্যাগম arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(পূর্ণাঙ্ক m, পূর্ণাঙ্ক _n) {
        _Tp **arr = নূতন _Tp*[m];
        প্রতি (পূর্ণাঙ্ক i = 0; i < m; i++)
            arr[i] = নূতন _Tp[_n];
        প্রত্যাগম arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(পূর্ণাঙ্ক m, পূর্ণাঙ্ক _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        প্রতি (পূর্ণাঙ্ক i = 0; i < m; i++) {
            প্রতি (পূর্ণাঙ্ক j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        প্রত্যাগম arr;
    }

    স্থির শূন্য complex_div(দ্বিগুণ xr, দ্বিগুণ xi, দ্বিগুণ yr, দ্বিগুণ yi, দ্বিগুণ& cdivr, দ্বিগুণ& cdivi) {
        দ্বিগুণ r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        যদি (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } অন্যথা {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    শূন্য hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ধ্রুব পূর্ণাঙ্ক max_iters_গুনতি = 1000 * এতত্->n;

        ধ্রুব পূর্ণাঙ্ক nn = এতত্->n; CV_Assert(nn > 0);
        পূর্ণাঙ্ক n1 = nn - 1;
        ধ্রুব পূর্ণাঙ্ক নিচের = 0;
        ধ্রুব পূর্ণাঙ্ক উপরের = nn - 1;
        ধ্রুব দ্বিগুণ eps = std::numeric_limits<দ্বিগুণ>::epsilon();
        দ্বিগুণ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        দ্বিগুণ norm = 0.0;
        প্রতি (পূর্ণাঙ্ক i = 0; i < nn; i++) {
#যদি 0 // 'if' condition is always false
            যদি (i < নিচের || i > উপরের) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            প্রতি (পূর্ণাঙ্ক j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        পূর্ণাঙ্ক iter = 0;
        যাবত্ (n1 >= নিচের) {

            // Look for single small sub-diagonal element
            পূর্ণাঙ্ক l = n1;
            যাবত্ (l > নিচের) {
                যদি (norm < FLT_EPSILON) {
                    ভঙ্গ;
                }
                দ্বিগুণ s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                যদি (s == 0.0) {
                    s = norm;
                }
                যদি (std::abs(H[l][l - 1]) < eps * s) {
                    ভঙ্গ;
                }
                l--;
            }

            // Check for convergence
            যদি (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } অন্যথা যদি (l == n1 - 1) {
                // Two roots found
                দ্বিগুণ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                দ্বিগুণ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                দ্বিগুণ q = p * p + w;
                দ্বিগুণ z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                দ্বিগুণ x = H[n1][n1];

                যদি (q >= 0) {
                    // Real pair
                    যদি (p >= 0) {
                        z = p + z;
                    } অন্যথা {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    যদি (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    দ্বিগুণ s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    দ্বিগুণ r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    প্রতি (পূর্ণাঙ্ক j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    প্রতি (পূর্ণাঙ্ক i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    প্রতি (পূর্ণাঙ্ক i = নিচের; i <= উপরের; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } অন্যথা {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } অন্যথা {
                // No convergence yet

                // Form shift
                দ্বিগুণ x = H[n1][n1];
                দ্বিগুণ y = 0.0;
                দ্বিগুণ w = 0.0;
                যদি (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                যদি (iter == 10) {
                    exshift += x;
                    প্রতি (পূর্ণাঙ্ক i = নিচের; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    দ্বিগুণ s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                যদি (iter == 30) {
                    দ্বিগুণ s = (y - x) * 0.5;
                    s = s * s + w;
                    যদি (s > 0) {
                        s = std::sqrt(s);
                        যদি (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        প্রতি (পূর্ণাঙ্ক i = নিচের; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                যদি (iter > max_iters_গুনতি)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                দ্বিগুণ p = std::numeric_limits<দ্বিগুণ>::quiet_NaN();
                দ্বিগুণ q = std::numeric_limits<দ্বিগুণ>::quiet_NaN();
                দ্বিগুণ r = std::numeric_limits<দ্বিগুণ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                পূর্ণাঙ্ক m = n1 - 2;
                যাবত্ (m >= l) {
                    দ্বিগুণ z = H[m][m];
                    r = x - z;
                    দ্বিগুণ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    যদি (m == l) {
                        ভঙ্গ;
                    }
                    যদি (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ভঙ্গ;
                    }
                    m--;
                }

                প্রতি (পূর্ণাঙ্ক i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    যদি (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                প্রতি (পূর্ণাঙ্ক k = m; k < n1; k++) {

                    তার্কিক notlast = (k != n1 - 1);
                    যদি (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        যদি (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    যদি (x == 0.0) {
                        ভঙ্গ;
                    }
                    দ্বিগুণ s = std::sqrt(p * p + q * q + r * r);
                    যদি (p < 0) {
                        s = -s;
                    }
                    যদি (s != 0) {
                        যদি (k != m) {
                            H[k][k - 1] = -s * x;
                        } অন্যথা যদি (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        দ্বিগুণ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        প্রতি (পূর্ণাঙ্ক j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            যদি (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        প্রতি (পূর্ণাঙ্ক i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            যদি (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        প্রতি (পূর্ণাঙ্ক i = নিচের; i <= উপরের; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            যদি (notlast) {
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

        যদি (norm < FLT_EPSILON) {
            প্রত্যাগম;
        }

        প্রতি (n1 = nn - 1; n1 >= 0; n1--) {
            দ্বিগুণ p = d[n1];
            দ্বিগুণ q = e[n1];

            যদি (q == 0) {
                // Real vector
                দ্বিগুণ z = std::numeric_limits<দ্বিগুণ>::quiet_NaN();
                দ্বিগুণ s = std::numeric_limits<দ্বিগুণ>::quiet_NaN();

                পূর্ণাঙ্ক l = n1;
                H[n1][n1] = 1.0;
                প্রতি (পূর্ণাঙ্ক i = n1 - 1; i >= 0; i--) {
                    দ্বিগুণ w = H[i][i] - p;
                    দ্বিগুণ r = 0.0;
                    প্রতি (পূর্ণাঙ্ক j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    যদি (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } অন্যথা {
                        l = i;
                        যদি (e[i] == 0.0) {
                            যদি (w != 0.0) {
                                H[i][n1] = -r / w;
                            } অন্যথা {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } অন্যথা {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            দ্বিগুণ x = H[i][i + 1];
                            দ্বিগুণ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            দ্বিগুণ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            যদি (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } অন্যথা {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        দ্বিগুণ t = std::abs(H[i][n1]);
                        যদি ((eps * t) * t > 1) {
                            দ্বিগুণ inv_t = 1.0 / t;
                            প্রতি (পূর্ণাঙ্ক j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } অন্যথা যদি (q < 0) {
                // Complex vector
                দ্বিগুণ z = std::numeric_limits<দ্বিগুণ>::quiet_NaN();
                দ্বিগুণ r = std::numeric_limits<দ্বিগুণ>::quiet_NaN();
                দ্বিগুণ s = std::numeric_limits<দ্বিগুণ>::quiet_NaN();

                পূর্ণাঙ্ক l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                যদি (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } অন্যথা {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                প্রতি (পূর্ণাঙ্ক i = n1 - 2; i >= 0; i--) {
                    দ্বিগুণ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    প্রতি (পূর্ণাঙ্ক j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    দ্বিগুণ w = H[i][i] - p;

                    যদি (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } অন্যথা {
                        l = i;
                        যদি (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } অন্যথা {
                            // Solve complex equations

                            দ্বিগুণ x = H[i][i + 1];
                            দ্বিগুণ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            যদি (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            যদি (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } অন্যথা {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        দ্বিগুণ t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        যদি ((eps * t) * t > 1) {
                            প্রতি (পূর্ণাঙ্ক j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#যদি 0 // 'if' condition is always false
        প্রতি (পূর্ণাঙ্ক i = 0; i < nn; i++) {
            যদি (i < নিচের || i > উপরের) {
                প্রতি (পূর্ণাঙ্ক j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        প্রতি (পূর্ণাঙ্ক j = nn - 1; j >= নিচের; j--) {
            প্রতি (পূর্ণাঙ্ক i = নিচের; i <= উপরের; i++) {
                দ্বিগুণ z = 0.0;
                প্রতি (পূর্ণাঙ্ক k = নিচের; k <= std::min(j, উপরের); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    শূন্য orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        পূর্ণাঙ্ক নিচের = 0;
        পূর্ণাঙ্ক উপরের = n - 1;

        প্রতি (পূর্ণাঙ্ক m = নিচের + 1; m < উপরের; m++) {

            // Scale column.

            দ্বিগুণ scale = 0.0;
            প্রতি (পূর্ণাঙ্ক i = m; i <= উপরের; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            যদি (scale != 0.0) {

                // Compute Householder transformation.

                দ্বিগুণ h = 0.0;
                প্রতি (পূর্ণাঙ্ক i = উপরের; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                দ্বিগুণ g = std::sqrt(h);
                যদি (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                প্রতি (পূর্ণাঙ্ক j = m; j < n; j++) {
                    দ্বিগুণ f = 0.0;
                    প্রতি (পূর্ণাঙ্ক i = উপরের; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    প্রতি (পূর্ণাঙ্ক i = m; i <= উপরের; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                প্রতি (পূর্ণাঙ্ক i = 0; i <= উপরের; i++) {
                    দ্বিগুণ f = 0.0;
                    প্রতি (পূর্ণাঙ্ক j = উপরের; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    প্রতি (পূর্ণাঙ্ক j = m; j <= উপরের; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++) {
            প্রতি (পূর্ণাঙ্ক j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        প্রতি (পূর্ণাঙ্ক m = উপরের - 1; m > নিচের; m--) {
            যদি (H[m][m - 1] != 0.0) {
                প্রতি (পূর্ণাঙ্ক i = m + 1; i <= উপরের; i++) {
                    ort[i] = H[i][m - 1];
                }
                প্রতি (পূর্ণাঙ্ক j = m; j <= উপরের; j++) {
                    দ্বিগুণ g = 0.0;
                    প্রতি (পূর্ণাঙ্ক i = m; i <= উপরের; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    প্রতি (পূর্ণাঙ্ক i = m; i <= উপরের; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    শূন্য release() {
        // releases the working data
        delete[] d; d = রিক্ত;
        delete[] e; e = রিক্ত;
        delete[] ort; ort = রিক্ত;
        প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++) {
            যদি (H) delete[] H[i];
            যদি (V) delete[] V[i];
        }
        delete[] H; H = রিক্ত;
        delete[] V; V = রিক্ত;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    শূন্য compute() {
        // Allocate memory for the working data.
        V = alloc_2d<দ্বিগুণ> (n, n, 0.0);
        d = alloc_1d<দ্বিগুণ> (n);
        e = alloc_1d<দ্বিগুণ> (n);
        ort = alloc_1d<দ্বিগুণ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++) {
                _eigenvalues.at<দ্বিগুণ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++)
                প্রতি (পূর্ণাঙ্ক j = 0; j < n; j++)
                    _eigenvectors.at<দ্বিগুণ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

সার্বজনিক:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(রিক্ত), e(রিক্ত), ort(রিক্ত),
        V(রিক্ত), H(রিক্ত)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    শূন্য compute(প্রবেশ_Array src, তার্কিক fallbackSymmetric = সত্য)
    {
        CV_INSTRUMENT_REGION();

        যদি(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } অন্যথা {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            এতত্->n = tmp.cols;
            // Allocate the matrix data to work on.
            এতত্->H = alloc_2d<দ্বিগুণ> (n, n);
            // Now safely copy the data.
            প্রতি (পূর্ণাঙ্ক i = 0; i < tmp.rows; i++) {
                প্রতি (পূর্ণাঙ্ক j = 0; j < tmp.cols; j++) {
                    এতত্->H[i][j] = tmp.at<দ্বিগুণ>(i, j);
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
    Mat eigenvalues() ধ্রুব { প্রত্যাগম _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ধ্রুব { প্রত্যাগম _eigenvectors; }
};

শূন্য eigenNonSymmetric(প্রবেশ_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    পূর্ণাঙ্ক type = src.type();
    পূর্ণাঙ্ক n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    যদি (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    অন্যথা
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, অসত্য);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<দ্বিগুণ> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<পূর্ণাঙ্ক> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<দ্বিগুণ> sorted_eigenvalues64f(n);
    প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    যদি( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<দ্বিগুণ> sorted_eigenvectors64f(n, n, CV_64FC1);
        প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++)
        {
            দ্বিগুণ* pDst = sorted_eigenvectors64f.ptr<দ্বিগুণ>(i);
            দ্বিগুণ* pSrc = eigenvectors64f.ptr<দ্বিগুণ>(sort_indexes[i]);
            CV_Assert(pSrc != রিক্ত);
            memcpy(pDst, pSrc, n * পরিমাণ(দ্বিগুণ));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(পূর্ণাঙ্ক অঙ্ক_components) : অঙ্ক_components_2(অঙ্ক_components) { }

LDA::LDA(প্রবেশ_Array_Of_Arrays src, প্রবেশ_Array labels, পূর্ণাঙ্ক অঙ্ক_components) : অঙ্ক_components_2(অঙ্ক_components)
{
    এতত্->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

শূন্য LDA::save(ধ্রুব String& filename) ধ্রুব
{
    FileStorage fs(filename, FileStorage::WRITE);
    যদি (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    এতত্->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
শূন্য LDA::load(ধ্রুব String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    যদি (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    এতত্->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
শূন্য LDA::save(FileStorage& fs) ধ্রুব {
    // write matrices
    fs << "num_components" << অঙ্ক_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
শূন্য LDA::load(ধ্রুব FileStorage& fs) {
    //read matrices
    fs["num_components"] >> অঙ্ক_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

শূন্য LDA::lda(প্রবেশ_Array_Of_Arrays _src, প্রবেশ_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<পূর্ণাঙ্ক> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        প্রতি(unsigned পূর্ণাঙ্ক i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<পূর্ণাঙ্ক>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<পূর্ণাঙ্ক> mapped_labels(labels.size());
    std::vector<পূর্ণাঙ্ক> num2label = remove_dups(labels);
    std::map<পূর্ণাঙ্ক, পূর্ণাঙ্ক> label2num;
    প্রতি (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)num2label.size(); i++)
        label2num[num2label[i]] = i;
    প্রতি (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    পূর্ণাঙ্ক N = data.rows;
    পূর্ণাঙ্ক D = data.cols;
    // number of unique labels
    পূর্ণাঙ্ক C = (পূর্ণাঙ্ক)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    যদি(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    যদি (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    যদি (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    যদি ((অঙ্ক_components_2 <= 0) || (অঙ্ক_components_2 >= C)) {
        অঙ্ক_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<পূর্ণাঙ্ক> অঙ্ক_Class(C);
    // initialize
    প্রতি (পূর্ণাঙ্ক i = 0; i < C; i++) {
        অঙ্ক_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    প্রতি (পূর্ণাঙ্ক i = 0; i < N; i++) {
        Mat instance = data.row(i);
        পূর্ণাঙ্ক classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        অঙ্ক_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<দ্বিগুণ> (N));
    // calculate class means
    প্রতি (পূর্ণাঙ্ক i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<দ্বিগুণ> (অঙ্ক_Class[i]));
    }
    // subtract class means
    প্রতি (পূর্ণাঙ্ক i = 0; i < N; i++) {
        পূর্ণাঙ্ক classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, সত্য);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    প্রতি (পূর্ণাঙ্ক i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, সত্য);
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
    std::vector<পূর্ণাঙ্ক> sorted_indices = argsort(_eigenvalues, অসত্য);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, অঙ্ক_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, অঙ্ক_components_2));
}

শূন্য LDA::compute(প্রবেশ_Array_Of_Arrays _src, প্রবেশ_Array _lbls) {
    বিকল্প(_src.kind()) {
    পক্ষ প্রবেশ_Array_2::STD_VECTOR_MAT:
    পক্ষ প্রবেশ_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        ভঙ্গ;
    পক্ষ প্রবেশ_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ভঙ্গ;
    স্বভাব:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        ভঙ্গ;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(প্রবেশ_Array src) {
   প্রত্যাগম subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(প্রবেশ_Array src) {
   প্রত্যাগম subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
