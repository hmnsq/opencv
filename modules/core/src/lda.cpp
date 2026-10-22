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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ <map>
#ଅନ୍ତର୍ଭାଵ <set>

ନାମାକାଶ cv
{

// Removes duplicate elements in a given vector.
template<typename _Tp>
ଧାଡ଼ିଭିତରେ std::vector<_Tp> remove_dups(ଅଚଳ std::vector<_Tp>& src) {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename std::set<_Tp>::const_iterator constSetIterator;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename std::vector<_Tp>::const_iterator constVecIterator;
    std::set<_Tp> set_elems;
    ପାଇଁ (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    std::vector<_Tp> elems;
    ପାଇଁ (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    ଫେରନ୍ତୁ elems;
}

ସ୍ଥିର Mat argsort(ନିବେଶ_Array _src, ତାର୍କିକ ascending=ସତ୍ୟ)
{
    Mat src = _src.getMat();
    ଯଦି (src.rows != 1 && src.cols != 1) {
        String error_message = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_message);
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = SORT_EVERY_ROW | (ascending ? SORT_ASCENDING : SORT_DESCENDING);
    Mat sorted_indices;
    sortIdx(src.reshape(1,1),sorted_indices,flags);
    ଫେରନ୍ତୁ sorted_indices;
}

ସ୍ଥିର Mat asRowMatrix(ନିବେଶ_Array_Of_Arrays src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rtype, ଦ୍ୱିଗୁଣ alpha=1, ଦ୍ୱିଗୁଣ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ଯଦି(src.kind() != ନିବେଶ_Array_2::STD_VECTOR_MAT && src.kind() != ନିବେଶ_Array_2::STD_ARRAY_MAT &&
        src.kind() != ନିବେଶ_Array_2::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    ଯଦି(n == 0)
        ଫେରନ୍ତୁ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)d, rtype);
    // now copy data
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ଯଦି(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)d, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        ଯଦି(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } ଅନ୍ୟଥା {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ sortMatrixColumnsByIndices(ନିବେଶ_Array _src, ନିବେଶ_Array _indices, OutputArray _dst) {
    ଯଦି(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> indices = _indices.getMat();
    _dst.create(src.rows, src.cols, src.type());
    Mat dst = _dst.getMat();
    ପାଇଁ(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)idx);
        originalCol.copyTo(sortedCol);
    }
}

ସ୍ଥିର Mat sortMatrixColumnsByIndices(ନିବେଶ_Array src, ନିବେଶ_Array indices) {
    Mat dst;
    sortMatrixColumnsByIndices(src, indices, dst);
    ଫେରନ୍ତୁ dst;
}


template<typename _Tp> ସ୍ଥିର ତାର୍କିକ
isSymmetric_(ନିବେଶ_Array src) {
    Mat _src = src.getMat();
    ଯଦି(_src.cols != _src.rows)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _src.rows; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ଯଦି (a != b) {
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

template<typename _Tp> ସ୍ଥିର ତାର୍କିକ
isSymmetric_(ନିବେଶ_Array src, ଦ୍ୱିଗୁଣ eps) {
    Mat _src = src.getMat();
    ଯଦି(_src.cols != _src.rows)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _src.rows; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < _src.cols; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ଯଦି (std::abs(a - b) > eps) {
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ isSymmetric(ନିବେଶ_Array src, ଦ୍ୱିଗୁଣ eps=1e-16)
{
    Mat m = src.getMat();
    ବାଛ (m.type()) {
        ସ୍ଥିତି CV_8SC1: ଫେରନ୍ତୁ isSymmetric_<ଅକ୍ଷର>(m); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_8UC1:
            ଫେରନ୍ତୁ isSymmetric_<unsigned ଅକ୍ଷର>(m); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_16SC1:
            ଫେରନ୍ତୁ isSymmetric_<short>(m); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_16UC1:
            ଫେରନ୍ତୁ isSymmetric_<unsigned short>(m); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_32SC1:
            ଫେରନ୍ତୁ isSymmetric_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_32FC1:
            ଫେରନ୍ତୁ isSymmetric_<ଦଶମିକ>(m, eps); ଭାଙ୍ଗ;
        ସ୍ଥିତି CV_64FC1:
            ଫେରନ୍ତୁ isSymmetric_<ଦ୍ୱିଗୁଣ>(m, eps); ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(ନିବେଶ_Array _W, ନିବେଶ_Array _mean, ନିବେଶ_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = src.rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = src.cols;
    // make sure the data has the correct shape
    ଯଦି(W.rows != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ଯଦି(!mean.empty() && (mean.total() != (size_t) d)) {
        String error_message = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, mean.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convertTo(X, W.type());
    // safe to do, because of above assertion
    ଯଦି(!mean.empty()) {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0; i<n; i++) {
            Mat r_i = X.row(i);
            subtract(r_i, mean.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ଫେରନ୍ତୁ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(ନିବେଶ_Array _W, ନିବେଶ_Array _mean, ନିବେଶ_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat mean = _mean.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = src.rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = src.cols;
    // make sure the data has the correct shape
    ଯଦି(W.cols != d) {
        String error_message = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.rows, src.cols, W.rows, W.cols);
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure mean is correct if not empty
    ଯଦି(!mean.empty() && (mean.total() != (size_t) W.rows)) {
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
    ଯଦି(!mean.empty()) {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0; i<n; i++) {
            Mat r_i = X.row(i);
            add(r_i, mean.reshape(1,1), r_i);
        }
    }
    ଫେରନ୍ତୁ X;
}


ଶ୍ରେଣୀ EigenvalueDecomposition {
ବ୍ୟକ୍ତିଗତ:

    // Holds the data dimension.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

    // Pointer to internal memory.
    ଦ୍ୱିଗୁଣ *d, *e, *ort;
    ଦ୍ୱିଗୁଣ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(ପୂର୍ଣ୍ଣସଂଖ୍ୟା m) {
        ଫେରନ୍ତୁ ନୂତନ _Tp[m];
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp *alloc_1d(ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++)
            arr[i] = val;
        ଫେରନ୍ତୁ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _n) {
        _Tp **arr = ନୂତନ _Tp*[m];
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++)
            arr[i] = ନୂତନ _Tp[_n];
        ଫେରନ୍ତୁ arr;
    }

    // Allocates memory.
    template<typename _Tp>
    _Tp **alloc_2d(ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ଫେରନ୍ତୁ arr;
    }

    ସ୍ଥିର ଶୂନ୍ୟ complex_div(ଦ୍ୱିଗୁଣ xr, ଦ୍ୱିଗୁଣ xi, ଦ୍ୱିଗୁଣ yr, ଦ୍ୱିଗୁଣ yi, ଦ୍ୱିଗୁଣ& cdivr, ଦ୍ୱିଗୁଣ& cdivi) {
        ଦ୍ୱିଗୁଣ r, dv;
        CV_DbgAssert(std::abs(yr) + std::abs(yi) > 0.0);
        ଯଦି (std::abs(yr) > std::abs(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ଅନ୍ୟଥା {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    ଶୂନ୍ୟ hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_iters_ଗଣନା = 1000 * ଏହି->n;

        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nn = ଏହି->n; CV_Assert(nn > 0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1 = nn - 1;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର = nn - 1;
        ଅଚଳ ଦ୍ୱିଗୁଣ eps = std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon();
        ଦ୍ୱିଗୁଣ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ଦ୍ୱିଗୁଣ norm = 0.0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nn; i++) {
#ଯଦି 0 // 'if' condition is always false
            ଯଦି (i < ତଳ || i > ଉପର) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#endif
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା iter = 0;
        ଯେତେବେଳେ (n1 >= ତଳ) {

            // Look for single small sub-diagonal element
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = n1;
            ଯେତେବେଳେ (l > ତଳ) {
                ଯଦି (norm < FLT_EPSILON) {
                    ଭାଙ୍ଗ;
                }
                ଦ୍ୱିଗୁଣ s = std::abs(H[l - 1][l - 1]) + std::abs(H[l][l]);
                ଯଦି (s == 0.0) {
                    s = norm;
                }
                ଯଦି (std::abs(H[l][l - 1]) < eps * s) {
                    ଭାଙ୍ଗ;
                }
                l--;
            }

            // Check for convergence
            ଯଦି (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ଅନ୍ୟଥା ଯଦି (l == n1 - 1) {
                // Two roots found
                ଦ୍ୱିଗୁଣ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ଦ୍ୱିଗୁଣ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ଦ୍ୱିଗୁଣ q = p * p + w;
                ଦ୍ୱିଗୁଣ z = std::sqrt(std::abs(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ଦ୍ୱିଗୁଣ x = H[n1][n1];

                ଯଦି (q >= 0) {
                    // Real pair
                    ଯଦି (p >= 0) {
                        z = p + z;
                    } ଅନ୍ୟଥା {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ଯଦି (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ଦ୍ୱିଗୁଣ s = std::abs(x) + std::abs(z);
                    p = x / s;
                    q = z / s;
                    ଦ୍ୱିଗୁଣ r = std::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ତଳ; i <= ଉପର; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ଅନ୍ୟଥା {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ଅନ୍ୟଥା {
                // No convergence yet

                // Form shift
                ଦ୍ୱିଗୁଣ x = H[n1][n1];
                ଦ୍ୱିଗୁଣ y = 0.0;
                ଦ୍ୱିଗୁଣ w = 0.0;
                ଯଦି (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ଯଦି (iter == 10) {
                    exshift += x;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ତଳ; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ଦ୍ୱିଗୁଣ s = std::abs(H[n1][n1 - 1]) + std::abs(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ଯଦି (iter == 30) {
                    ଦ୍ୱିଗୁଣ s = (y - x) * 0.5;
                    s = s * s + w;
                    ଯଦି (s > 0) {
                        s = std::sqrt(s);
                        ଯଦି (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ତଳ; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ଯଦି (iter > max_iters_ଗଣନା)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ଦ୍ୱିଗୁଣ p = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();
                ଦ୍ୱିଗୁଣ q = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();
                ଦ୍ୱିଗୁଣ r = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = n1 - 2;
                ଯେତେବେଳେ (m >= l) {
                    ଦ୍ୱିଗୁଣ z = H[m][m];
                    r = x - z;
                    ଦ୍ୱିଗୁଣ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::abs(p) + std::abs(q) + std::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ଯଦି (m == l) {
                        ଭାଙ୍ଗ;
                    }
                    ଯଦି (std::abs(H[m][m - 1]) * (std::abs(q) + std::abs(r)) < eps * (std::abs(p)
                                                                                     * (std::abs(H[m - 1][m - 1]) + std::abs(z) + std::abs(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ଭାଙ୍ଗ;
                    }
                    m--;
                }

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ଯଦି (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = m; k < n1; k++) {

                    ତାର୍କିକ notlast = (k != n1 - 1);
                    ଯଦି (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::abs(p) + std::abs(q) + std::abs(r);
                        ଯଦି (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ଯଦି (x == 0.0) {
                        ଭାଙ୍ଗ;
                    }
                    ଦ୍ୱିଗୁଣ s = std::sqrt(p * p + q * q + r * r);
                    ଯଦି (p < 0) {
                        s = -s;
                    }
                    ଯଦି (s != 0) {
                        ଯଦି (k != m) {
                            H[k][k - 1] = -s * x;
                        } ଅନ୍ୟଥା ଯଦି (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ଦ୍ୱିଗୁଣ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ଯଦି (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ଯଦି (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ତଳ; i <= ଉପର; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ଯଦି (notlast) {
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

        ଯଦି (norm < FLT_EPSILON) {
            ଫେରନ୍ତୁ;
        }

        ପାଇଁ (n1 = nn - 1; n1 >= 0; n1--) {
            ଦ୍ୱିଗୁଣ p = d[n1];
            ଦ୍ୱିଗୁଣ q = e[n1];

            ଯଦି (q == 0) {
                // Real vector
                ଦ୍ୱିଗୁଣ z = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();
                ଦ୍ୱିଗୁଣ s = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = n1;
                H[n1][n1] = 1.0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = n1 - 1; i >= 0; i--) {
                    ଦ୍ୱିଗୁଣ w = H[i][i] - p;
                    ଦ୍ୱିଗୁଣ r = 0.0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ଯଦି (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ଅନ୍ୟଥା {
                        l = i;
                        ଯଦି (e[i] == 0.0) {
                            ଯଦି (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ଅନ୍ୟଥା {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ଅନ୍ୟଥା {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ଦ୍ୱିଗୁଣ x = H[i][i + 1];
                            ଦ୍ୱିଗୁଣ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ଦ୍ୱିଗୁଣ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ଯଦି (std::abs(x) > std::abs(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ଅନ୍ୟଥା {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ଦ୍ୱିଗୁଣ t = std::abs(H[i][n1]);
                        ଯଦି ((eps * t) * t > 1) {
                            ଦ୍ୱିଗୁଣ inv_t = 1.0 / t;
                            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ଅନ୍ୟଥା ଯଦି (q < 0) {
                // Complex vector
                ଦ୍ୱିଗୁଣ z = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();
                ଦ୍ୱିଗୁଣ r = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();
                ଦ୍ୱିଗୁଣ s = std::numeric_limits<ଦ୍ୱିଗୁଣ>::quiet_NaN();

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ଯଦି (std::abs(H[n1][n1 - 1]) > std::abs(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ଅନ୍ୟଥା {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = n1 - 2; i >= 0; i--) {
                    ଦ୍ୱିଗୁଣ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ଦ୍ୱିଗୁଣ w = H[i][i] - p;

                    ଯଦି (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ଅନ୍ୟଥା {
                        l = i;
                        ଯଦି (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ଅନ୍ୟଥା {
                            // Solve complex equations

                            ଦ୍ୱିଗୁଣ x = H[i][i + 1];
                            ଦ୍ୱିଗୁଣ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ଯଦି (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::abs(w) + std::abs(q) + std::abs(x)
                                                   + std::abs(y) + std::abs(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ଯଦି (std::abs(x) > (std::abs(z) + std::abs(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ଅନ୍ୟଥା {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ଦ୍ୱିଗୁଣ t = std::max(std::abs(H[i][n1 - 1]), std::abs(H[i][n1]));
                        ଯଦି ((eps * t) * t > 1) {
                            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ଯଦି 0 // 'if' condition is always false
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nn; i++) {
            ଯଦି (i < ତଳ || i > ଉପର) {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#endif

        // Back transformation to get eigenvectors of original matrix

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = nn - 1; j >= ତଳ; j--) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ତଳ; i <= ଉପର; i++) {
                ଦ୍ୱିଗୁଣ z = 0.0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = ତଳ; k <= std::min(j, ଉପର); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    ଶୂନ୍ୟ orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର = n - 1;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = ତଳ + 1; m < ଉପର; m++) {

            // Scale column.

            ଦ୍ୱିଗୁଣ scale = 0.0;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m; i <= ଉପର; i++) {
                scale = scale + std::abs(H[i][m - 1]);
            }
            ଯଦି (scale != 0.0) {

                // Compute Householder transformation.

                ଦ୍ୱିଗୁଣ h = 0.0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଉପର; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ଦ୍ୱିଗୁଣ g = std::sqrt(h);
                ଯଦି (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = m; j < n; j++) {
                    ଦ୍ୱିଗୁଣ f = 0.0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଉପର; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m; i <= ଉପର; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <= ଉପର; i++) {
                    ଦ୍ୱିଗୁଣ f = 0.0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = ଉପର; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = m; j <= ଉପର; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = ଉପର - 1; m > ତଳ; m--) {
            ଯଦି (H[m][m - 1] != 0.0) {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m + 1; i <= ଉପର; i++) {
                    ort[i] = H[i][m - 1];
                }
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = m; j <= ଉପର; j++) {
                    ଦ୍ୱିଗୁଣ g = 0.0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m; i <= ଉପର; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m; i <= ଉପର; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    ଶୂନ୍ୟ release() {
        // releases the working data
        delete[] d; d = ଖାଲି;
        delete[] e; e = ଖାଲି;
        delete[] ort; ort = ଖାଲି;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ଯଦି (H) delete[] H[i];
            ଯଦି (V) delete[] V[i];
        }
        delete[] H; H = ଖାଲି;
        delete[] V; V = ଖାଲି;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    ଶୂନ୍ୟ compute() {
        // Allocate memory for the working data.
        V = alloc_2d<ଦ୍ୱିଗୁଣ> (n, n, 0.0);
        d = alloc_1d<ଦ୍ୱିଗୁଣ> (n);
        e = alloc_1d<ଦ୍ୱିଗୁଣ> (n);
        ort = alloc_1d<ଦ୍ୱିଗୁଣ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
                _eigenvalues.at<ଦ୍ୱିଗୁଣ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < n; j++)
                    _eigenvectors.at<ଦ୍ୱିଗୁଣ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

ସର୍ବସାଧାରଣ:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ଖାଲି), e(ଖାଲି), ort(ଖାଲି),
        V(ଖାଲି), H(ଖାଲି)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    ଶୂନ୍ୟ compute(ନିବେଶ_Array src, ତାର୍କିକ fallbackSymmetric = ସତ୍ୟ)
    {
        CV_INSTRUMENT_REGION();

        ଯଦି(fallbackSymmetric && isSymmetric(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ଅନ୍ୟଥା {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convertTo(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ଏହି->n = tmp.cols;
            // Allocate the matrix data to work on.
            ଏହି->H = alloc_2d<ଦ୍ୱିଗୁଣ> (n, n);
            // Now safely copy the data.
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < tmp.rows; i++) {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < tmp.cols; j++) {
                    ଏହି->H[i][j] = tmp.at<ଦ୍ୱିଗୁଣ>(i, j);
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
    Mat eigenvalues() ଅଚଳ { ଫେରନ୍ତୁ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ଅଚଳ { ଫେରନ୍ତୁ _eigenvectors; }
};

ଶୂନ୍ୟ eigenNonSymmetric(ନିବେଶ_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = src.rows;

    CV_Assert(src.rows == src.cols);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ଯଦି (type == CV_32F)
        src.convertTo(src64f, CV_32FC1);
    ଅନ୍ୟଥା
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ମିଥ୍ୟା);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ଦ୍ୱିଗୁଣ> eigenvalues64f;
    eigensystem.eigenvalues().copyTo(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> sort_indexes(n);
    cv::sortIdx(eigenvalues64f, sort_indexes, SORT_EVERY_ROW | SORT_DESCENDING);

    std::vector<ଦ୍ୱିଗୁଣ> sorted_eigenvalues64f(n);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[sort_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convertTo(_evals, type);

    ଯଦି( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.rows == n);
        CV_Assert(eigenvectors64f.cols == n);
        Mat_<ଦ୍ୱିଗୁଣ> sorted_eigenvectors64f(n, n, CV_64FC1);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
        {
            ଦ୍ୱିଗୁଣ* pDst = sorted_eigenvectors64f.ptr<ଦ୍ୱିଗୁଣ>(i);
            ଦ୍ୱିଗୁଣ* pSrc = eigenvectors64f.ptr<ଦ୍ୱିଗୁଣ>(sort_indexes[i]);
            CV_Assert(pSrc != ଖାଲି);
            memcpy(pDst, pSrc, n * ଆକାର(ଦ୍ୱିଗୁଣ));
        }
        sorted_eigenvectors64f.convertTo(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_components) : ଅଙ୍କ_components_2(ଅଙ୍କ_components) { }

LDA::LDA(ନିବେଶ_Array_Of_Arrays src, ନିବେଶ_Array labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_components) : ଅଙ୍କ_components_2(ଅଙ୍କ_components)
{
    ଏହି->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

ଶୂନ୍ୟ LDA::save(ଅଚଳ String& filename) ଅଚଳ
{
    FileStorage fs(filename, FileStorage::WRITE);
    ଯଦି (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ଏହି->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
ଶୂନ୍ୟ LDA::load(ଅଚଳ String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ଯଦି (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ଏହି->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
ଶୂନ୍ୟ LDA::save(FileStorage& fs) ଅଚଳ {
    // write matrices
    fs << "num_components" << ଅଙ୍କ_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
ଶୂନ୍ୟ LDA::load(ଅଚଳ FileStorage& fs) {
    //read matrices
    fs["num_components"] >> ଅଙ୍କ_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

ଶୂନ୍ୟ LDA::lda(ନିବେଶ_Array_Of_Arrays _src, ନିବେଶ_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        ପାଇଁ(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < tmp.total(); i++) {
            labels.push_back(tmp.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i));
        }
    }
    // turn into row sampled matrix
    Mat data;
    // ensure working matrix is double precision
    src.convertTo(data, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> mapped_labels(labels.size());
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> num2label = remove_dups(labels);
    std::map<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> label2num;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)num2label.size(); i++)
        label2num[num2label[i]] = i;
    ପାଇଁ (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = data.rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା D = data.cols;
    // number of unique labels
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା C = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ଯଦି(C == 1) {
        String error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_message);
    }
    // throw error if less labels, than samples
    ଯଦି (labels.size() != static_cast<size_t>(N)) {
        String error_message = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_message);
    }
    // warn if within-classes scatter matrix becomes singular
    ଯଦି (N < D) {
        std::cout << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::endl;
    }
    // clip number of components to be a valid number
    ଯଦି ((ଅଙ୍କ_components_2 <= 0) || (ଅଙ୍କ_components_2 >= C)) {
        ଅଙ୍କ_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat meanTotal = Mat::zeros(1, D, data.type());
    // holds the mean for each class
    std::vector<Mat> meanClass(C);
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ଅଙ୍କ_Class(C);
    // initialize
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < C; i++) {
        ଅଙ୍କ_Class[i] = 0;
        meanClass[i] = Mat::zeros(1, D, data.type()); //! Dx1 image vector
    }
    // calculate sums
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++) {
        Mat instance = data.row(i);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        ଅଙ୍କ_Class[classIdx]++;
    }
    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<ଦ୍ୱିଗୁଣ> (N));
    // calculate class means
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < C; i++) {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0 / static_cast<ଦ୍ୱିଗୁଣ> (ଅଙ୍କ_Class[i]));
    }
    // subtract class means
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା classIdx = mapped_labels[i];
        Mat instance = data.row(i);
        subtract(instance, meanClass[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, data.type());
    mulTransposed(data, Sw, ସତ୍ୟ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, data.type());
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < C; i++) {
        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, ସତ୍ୟ);
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
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> sorted_indices = argsort(_eigenvalues, ମିଥ୍ୟା);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = sortMatrixColumnsByIndices(_eigenvalues, sorted_indices);
    _eigenvectors = sortMatrixColumnsByIndices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, ଅଙ୍କ_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, ଅଙ୍କ_components_2));
}

ଶୂନ୍ୟ LDA::compute(ନିବେଶ_Array_Of_Arrays _src, ନିବେଶ_Array _lbls) {
    ବାଛ(_src.kind()) {
    ସ୍ଥିତି ନିବେଶ_Array_2::STD_VECTOR_MAT:
    ସ୍ଥିତି ନିବେଶ_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି ନିବେଶ_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        String error_message= format("InputArray Datatype %d is not supported.", _src.kind());
        CV_Error(Error::StsBadArg, error_message);
        ଭାଙ୍ଗ;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(ନିବେଶ_Array src) {
   ଫେରନ୍ତୁ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(ନିବେଶ_Array src) {
   ଫେରନ୍ତୁ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
