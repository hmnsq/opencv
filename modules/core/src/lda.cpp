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
سانچو<قسم_نالو _Tp>
مضمن std::vector<_Tp> remove_dups(ثابت std::vector<_Tp>& src) {
    تعريفنوع قسم_نالو std::سيٽ<_Tp>::const_iterator const_سيٽ_Iterator;
    تعريفنوع قسم_نالو std::vector<_Tp>::const_iterator constVecIterator;
    std::سيٽ<_Tp> سيٽ_elems;
    لكل (constVecIterator it = src.begin(); it != src.end(); ++it)
        سيٽ_elems.insert(*it);
    std::vector<_Tp> elems;
    لكل (const_سيٽ_Iterator it = سيٽ_elems.begin(); it != سيٽ_elems.end(); ++it)
        elems.push_back(*it);
    أرجع elems;
}

ساكن Mat argsort(داخلا_Array _src, منطقي ascending=سچ)
{
    Mat src = _src.getMat();
    إذا (src.صفون != 1 && src.کانن_ڳڻپ != 1) {
        String error_پيغام = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    صحيح flags = ترتيب_ڏيو_EVERY_صف | (ascending ? ترتيب_ڏيو_ASCENDING : ترتيب_ڏيو_DESCENDING);
    Mat sorted_indices;
    ترتيب_ڏيو_Idx(src.reshape(1,1),sorted_indices,flags);
    أرجع sorted_indices;
}

ساكن Mat as_صف_ميٽرڪس(داخلا_Array_جو_Arrays src, صحيح rtype, مزدوج alpha=1, مزدوج beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    إذا(src.قسم() != داخلا_Array_2::STD_ويڪٽر_MAT && src.قسم() != داخلا_Array_2::STD_ARRAY_MAT &&
        src.قسم() != داخلا_Array_2::STD_ويڪٽر_ويڪٽر) {
        String error_پيغام = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // number of samples
    size_t n = src.ڪل();
    // return empty matrix if no matrices given
    إذا(n == 0)
        أرجع Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).ڪل();
    // create data matrix
    Mat معلومات((صحيح)n, (صحيح)d, rtype);
    // now copy data
    لكل(صحيح i = 0; i < (صحيح)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        إذا(src.getMat(i).ڪل() != d) {
            String error_پيغام = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (صحيح)d, (صحيح)src.getMat(i).ڪل());
            CV_Error(Error::StsBadArg, error_پيغام);
        }
        // get a hold of the current row
        Mat xi = معلومات.صف(i);
        // make reshape happy by cloning for non-continuous matrices
        إذا(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_منزل(xi, rtype, alpha, beta);
        } وإلا {
            src.getMat(i).clone().reshape(1, 1).convert_منزل(xi, rtype, alpha, beta);
        }
    }
    أرجع معلومات;
}

ساكن فراغ ترتيب_ڏيو_ميٽرڪس_Columns_By_Indices(داخلا_Array _src, داخلا_Array _indices, OutputArray _dst) {
    إذا(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<صحيح> indices = _indices.getMat();
    _dst.create(src.صفون, src.کانن_ڳڻپ, src.type());
    Mat dst = _dst.getMat();
    لكل(size_t idx = 0; idx < indices.size(); idx++) {
        Mat original_کانو = src.کانو(indices[idx]);
        Mat sorted_کانو = dst.کانو((صحيح)idx);
        original_کانو.copy_منزل(sorted_کانو);
    }
}

ساكن Mat ترتيب_ڏيو_ميٽرڪس_Columns_By_Indices(داخلا_Array src, داخلا_Array indices) {
    Mat dst;
    ترتيب_ڏيو_ميٽرڪس_Columns_By_Indices(src, indices, dst);
    أرجع dst;
}


سانچو<قسم_نالو _Tp> ساكن منطقي
is_متناظر(داخلا_Array src) {
    Mat _src = src.getMat();
    إذا(_src.کانن_ڳڻپ != _src.صفون)
        أرجع خطأ;
    لكل (صحيح i = 0; i < _src.صفون; i++) {
        لكل (صحيح j = 0; j < _src.کانن_ڳڻپ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            إذا (a != b) {
                أرجع خطأ;
            }
        }
    }
    أرجع سچ;
}

سانچو<قسم_نالو _Tp> ساكن منطقي
is_متناظر(داخلا_Array src, مزدوج eps) {
    Mat _src = src.getMat();
    إذا(_src.کانن_ڳڻپ != _src.صفون)
        أرجع خطأ;
    لكل (صحيح i = 0; i < _src.صفون; i++) {
        لكل (صحيح j = 0; j < _src.کانن_ڳڻپ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            إذا (std::مطلق_قدر(a - b) > eps) {
                أرجع خطأ;
            }
        }
    }
    أرجع سچ;
}

ساكن منطقي is_متناظر_2(داخلا_Array src, مزدوج eps=1e-16)
{
    Mat m = src.getMat();
    بدل (m.type()) {
        حالة CV_8SC1: أرجع is_متناظر<حرف>(m); اكسر;
        حالة CV_8UC1:
            أرجع is_متناظر<بنانشان حرف>(m); اكسر;
        حالة CV_16SC1:
            أرجع is_متناظر<ننڍو>(m); اكسر;
        حالة CV_16UC1:
            أرجع is_متناظر<بنانشان ننڍو>(m); اكسر;
        حالة CV_32SC1:
            أرجع is_متناظر<صحيح>(m); اكسر;
        حالة CV_32FC1:
            أرجع is_متناظر<عائم>(m, eps); اكسر;
        حالة CV_64FC1:
            أرجع is_متناظر<مزدوج>(m, eps); اكسر;
        افتراضي:
            اكسر;
    }
    أرجع خطأ;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(داخلا_Array _W, داخلا_Array سراسري, داخلا_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat سراسري_2 = سراسري.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحيح n = src.صفون;
    صحيح d = src.کانن_ڳڻپ;
    // make sure the data has the correct shape
    إذا(W.صفون != d) {
        String error_پيغام = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.صفون, src.کانن_ڳڻپ, W.صفون, W.کانن_ڳڻپ);
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // make sure mean is correct if not empty
    إذا(!سراسري_2.empty() && (سراسري_2.ڪل() != (size_t) d)) {
        String error_پيغام = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, سراسري_2.ڪل());
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_منزل(X, W.type());
    // safe to do, because of above assertion
    إذا(!سراسري_2.empty()) {
        لكل(صحيح i=0; i<n; i++) {
            Mat r_i = X.صف(i);
            ڪٽيو(r_i, سراسري_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    أرجع Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(داخلا_Array _W, داخلا_Array سراسري, داخلا_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat سراسري_2 = سراسري.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    صحيح n = src.صفون;
    صحيح d = src.کانن_ڳڻپ;
    // make sure the data has the correct shape
    إذا(W.کانن_ڳڻپ != d) {
        String error_پيغام = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.صفون, src.کانن_ڳڻپ, W.صفون, W.کانن_ڳڻپ);
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // make sure mean is correct if not empty
    إذا(!سراسري_2.empty() && (سراسري_2.ڪل() != (size_t) W.صفون)) {
        String error_پيغام = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.کانن_ڳڻپ, سراسري_2.ڪل());
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_منزل(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    إذا(!سراسري_2.empty()) {
        لكل(صحيح i=0; i<n; i++) {
            Mat r_i = X.صف(i);
            جوڙيو(r_i, سراسري_2.reshape(1,1), r_i);
        }
    }
    أرجع X;
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
    سانچو<قسم_نالو _Tp>
    _Tp *alloc_1d(صحيح m) {
        أرجع جديد _Tp[m];
    }

    // Allocates memory.
    سانچو<قسم_نالو _Tp>
    _Tp *alloc_1d(صحيح m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        لكل (صحيح i = 0; i < m; i++)
            arr[i] = val;
        أرجع arr;
    }

    // Allocates memory.
    سانچو<قسم_نالو _Tp>
    _Tp **alloc_2d(صحيح m, صحيح _n) {
        _Tp **arr = جديد _Tp*[m];
        لكل (صحيح i = 0; i < m; i++)
            arr[i] = جديد _Tp[_n];
        أرجع arr;
    }

    // Allocates memory.
    سانچو<قسم_نالو _Tp>
    _Tp **alloc_2d(صحيح m, صحيح _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        لكل (صحيح i = 0; i < m; i++) {
            لكل (صحيح j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        أرجع arr;
    }

    ساكن فراغ complex_div(مزدوج xr, مزدوج xi, مزدوج yr, مزدوج yi, مزدوج& cdivr, مزدوج& cdivi) {
        مزدوج r, dv;
        CV_DbgAssert(std::مطلق_قدر(yr) + std::مطلق_قدر(yi) > 0.0);
        إذا (std::مطلق_قدر(yr) > std::مطلق_قدر(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } وإلا {
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
        ثابت صحيح max_iters_ڳڻپ = 1000 * هذا->n;

        ثابت صحيح nn = هذا->n; CV_Assert(nn > 0);
        صحيح n1 = nn - 1;
        ثابت صحيح هيٺيون = 0;
        ثابت صحيح مٿاهون = nn - 1;
        ثابت مزدوج eps = std::numeric_limits<مزدوج>::epsilon();
        مزدوج exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        مزدوج norm = 0.0;
        لكل (صحيح i = 0; i < nn; i++) {
#إذا 0 // 'if' condition is always false
            إذا (i < هيٺيون || i > مٿاهون) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#جيڪڏهن_پورو
            لكل (صحيح j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::مطلق_قدر(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        صحيح iter = 0;
        طالما (n1 >= هيٺيون) {

            // Look for single small sub-diagonal element
            صحيح l = n1;
            طالما (l > هيٺيون) {
                إذا (norm < FLT_EPSILON) {
                    اكسر;
                }
                مزدوج s = std::مطلق_قدر(H[l - 1][l - 1]) + std::مطلق_قدر(H[l][l]);
                إذا (s == 0.0) {
                    s = norm;
                }
                إذا (std::مطلق_قدر(H[l][l - 1]) < eps * s) {
                    اكسر;
                }
                l--;
            }

            // Check for convergence
            إذا (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } وإلا إذا (l == n1 - 1) {
                // Two roots found
                مزدوج w = H[n1][n1 - 1] * H[n1 - 1][n1];
                مزدوج p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                مزدوج q = p * p + w;
                مزدوج z = std::جذر(std::مطلق_قدر(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                مزدوج x = H[n1][n1];

                إذا (q >= 0) {
                    // Real pair
                    إذا (p >= 0) {
                        z = p + z;
                    } وإلا {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    إذا (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    مزدوج s = std::مطلق_قدر(x) + std::مطلق_قدر(z);
                    p = x / s;
                    q = z / s;
                    مزدوج r = std::جذر(p * p + q * q);
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

                    لكل (صحيح i = هيٺيون; i <= مٿاهون; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } وإلا {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } وإلا {
                // No convergence yet

                // Form shift
                مزدوج x = H[n1][n1];
                مزدوج y = 0.0;
                مزدوج w = 0.0;
                إذا (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                إذا (iter == 10) {
                    exshift += x;
                    لكل (صحيح i = هيٺيون; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    مزدوج s = std::مطلق_قدر(H[n1][n1 - 1]) + std::مطلق_قدر(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                إذا (iter == 30) {
                    مزدوج s = (y - x) * 0.5;
                    s = s * s + w;
                    إذا (s > 0) {
                        s = std::جذر(s);
                        إذا (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        لكل (صحيح i = هيٺيون; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                إذا (iter > max_iters_ڳڻپ)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                مزدوج p = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج q = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج r = std::numeric_limits<مزدوج>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                صحيح m = n1 - 2;
                طالما (m >= l) {
                    مزدوج z = H[m][m];
                    r = x - z;
                    مزدوج s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::مطلق_قدر(p) + std::مطلق_قدر(q) + std::مطلق_قدر(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    إذا (m == l) {
                        اكسر;
                    }
                    إذا (std::مطلق_قدر(H[m][m - 1]) * (std::مطلق_قدر(q) + std::مطلق_قدر(r)) < eps * (std::مطلق_قدر(p)
                                                                                     * (std::مطلق_قدر(H[m - 1][m - 1]) + std::مطلق_قدر(z) + std::مطلق_قدر(
                                                                                                                                           H[m + 1][m + 1])))) {
                        اكسر;
                    }
                    m--;
                }

                لكل (صحيح i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    إذا (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                لكل (صحيح k = m; k < n1; k++) {

                    منطقي notlast = (k != n1 - 1);
                    إذا (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::مطلق_قدر(p) + std::مطلق_قدر(q) + std::مطلق_قدر(r);
                        إذا (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    إذا (x == 0.0) {
                        اكسر;
                    }
                    مزدوج s = std::جذر(p * p + q * q + r * r);
                    إذا (p < 0) {
                        s = -s;
                    }
                    إذا (s != 0) {
                        إذا (k != m) {
                            H[k][k - 1] = -s * x;
                        } وإلا إذا (l != m) {
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
                            إذا (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        لكل (صحيح i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            إذا (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        لكل (صحيح i = هيٺيون; i <= مٿاهون; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            إذا (notlast) {
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

        إذا (norm < FLT_EPSILON) {
            أرجع;
        }

        لكل (n1 = nn - 1; n1 >= 0; n1--) {
            مزدوج p = d[n1];
            مزدوج q = e[n1];

            إذا (q == 0) {
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
                    إذا (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } وإلا {
                        l = i;
                        إذا (e[i] == 0.0) {
                            إذا (w != 0.0) {
                                H[i][n1] = -r / w;
                            } وإلا {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } وإلا {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            مزدوج x = H[i][i + 1];
                            مزدوج y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            مزدوج t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            إذا (std::مطلق_قدر(x) > std::مطلق_قدر(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } وإلا {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        مزدوج t = std::مطلق_قدر(H[i][n1]);
                        إذا ((eps * t) * t > 1) {
                            مزدوج inv_t = 1.0 / t;
                            لكل (صحيح j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } وإلا إذا (q < 0) {
                // Complex vector
                مزدوج z = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج r = std::numeric_limits<مزدوج>::quiet_NaN();
                مزدوج s = std::numeric_limits<مزدوج>::quiet_NaN();

                صحيح l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                إذا (std::مطلق_قدر(H[n1][n1 - 1]) > std::مطلق_قدر(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } وإلا {
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

                    إذا (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } وإلا {
                        l = i;
                        إذا (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } وإلا {
                            // Solve complex equations

                            مزدوج x = H[i][i + 1];
                            مزدوج y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            إذا (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::مطلق_قدر(w) + std::مطلق_قدر(q) + std::مطلق_قدر(x)
                                                   + std::مطلق_قدر(y) + std::مطلق_قدر(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            إذا (std::مطلق_قدر(x) > (std::مطلق_قدر(z) + std::مطلق_قدر(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } وإلا {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        مزدوج t = std::max(std::مطلق_قدر(H[i][n1 - 1]), std::مطلق_قدر(H[i][n1]));
                        إذا ((eps * t) * t > 1) {
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

#إذا 0 // 'if' condition is always false
        لكل (صحيح i = 0; i < nn; i++) {
            إذا (i < هيٺيون || i > مٿاهون) {
                لكل (صحيح j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#جيڪڏهن_پورو

        // Back transformation to get eigenvectors of original matrix

        لكل (صحيح j = nn - 1; j >= هيٺيون; j--) {
            لكل (صحيح i = هيٺيون; i <= مٿاهون; i++) {
                مزدوج z = 0.0;
                لكل (صحيح k = هيٺيون; k <= std::min(j, مٿاهون); k++) {
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
        صحيح هيٺيون = 0;
        صحيح مٿاهون = n - 1;

        لكل (صحيح m = هيٺيون + 1; m < مٿاهون; m++) {

            // Scale column.

            مزدوج scale = 0.0;
            لكل (صحيح i = m; i <= مٿاهون; i++) {
                scale = scale + std::مطلق_قدر(H[i][m - 1]);
            }
            إذا (scale != 0.0) {

                // Compute Householder transformation.

                مزدوج h = 0.0;
                لكل (صحيح i = مٿاهون; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                مزدوج g = std::جذر(h);
                إذا (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                لكل (صحيح j = m; j < n; j++) {
                    مزدوج f = 0.0;
                    لكل (صحيح i = مٿاهون; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    لكل (صحيح i = m; i <= مٿاهون; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                لكل (صحيح i = 0; i <= مٿاهون; i++) {
                    مزدوج f = 0.0;
                    لكل (صحيح j = مٿاهون; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    لكل (صحيح j = m; j <= مٿاهون; j++) {
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

        لكل (صحيح m = مٿاهون - 1; m > هيٺيون; m--) {
            إذا (H[m][m - 1] != 0.0) {
                لكل (صحيح i = m + 1; i <= مٿاهون; i++) {
                    ort[i] = H[i][m - 1];
                }
                لكل (صحيح j = m; j <= مٿاهون; j++) {
                    مزدوج g = 0.0;
                    لكل (صحيح i = m; i <= مٿاهون; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    لكل (صحيح i = m; i <= مٿاهون; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    فراغ release() {
        // releases the working data
        ميٽيو[] d; d = عدم;
        ميٽيو[] e; e = عدم;
        ميٽيو[] ort; ort = عدم;
        لكل (صحيح i = 0; i < n; i++) {
            إذا (H) ميٽيو[] H[i];
            إذا (V) ميٽيو[] V[i];
        }
        ميٽيو[] H; H = عدم;
        ميٽيو[] V; V = عدم;
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
    فراغ compute(داخلا_Array src, منطقي fallback_متناظر = سچ)
    {
        CV_INSTRUMENT_REGION();

        إذا(fallback_متناظر && is_متناظر_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } وإلا {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_منزل(tmp, CV_64FC1);
            // Get dimension of the matrix.
            هذا->n = tmp.کانن_ڳڻپ;
            // Allocate the matrix data to work on.
            هذا->H = alloc_2d<مزدوج> (n, n);
            // Now safely copy the data.
            لكل (صحيح i = 0; i < tmp.صفون; i++) {
                لكل (صحيح j = 0; j < tmp.کانن_ڳڻپ; j++) {
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
    Mat eigenvalues() ثابت { أرجع _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ثابت { أرجع _eigenvectors; }
};

فراغ eigen_Non_متناظر(داخلا_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحيح type = src.type();
    صحيح n = src.صفون;

    CV_Assert(src.صفون == src.کانن_ڳڻپ);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    إذا (type == CV_32F)
        src.convert_منزل(src64f, CV_32FC1);
    وإلا
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, خطأ);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<مزدوج> eigenvalues64f;
    eigensystem.eigenvalues().copy_منزل(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<صحيح> ترتيب_ڏيو_indexes(n);
    cv::ترتيب_ڏيو_Idx(eigenvalues64f, ترتيب_ڏيو_indexes, ترتيب_ڏيو_EVERY_صف | ترتيب_ڏيو_DESCENDING);

    std::vector<مزدوج> sorted_eigenvalues64f(n);
    لكل (صحيح i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[ترتيب_ڏيو_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_منزل(_evals, type);

    إذا( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.صفون == n);
        CV_Assert(eigenvectors64f.کانن_ڳڻپ == n);
        Mat_<مزدوج> sorted_eigenvectors64f(n, n, CV_64FC1);
        لكل (صحيح i = 0; i < n; i++)
        {
            مزدوج* pDst = sorted_eigenvectors64f.ptr<مزدوج>(i);
            مزدوج* pSrc = eigenvectors64f.ptr<مزدوج>(ترتيب_ڏيو_indexes[i]);
            CV_Assert(pSrc != عدم);
            يادگيري_نقل(pDst, pSrc, n * حجم(مزدوج));
        }
        sorted_eigenvectors64f.convert_منزل(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(صحيح انگ_components) : انگ_components_2(انگ_components) { }

LDA::LDA(داخلا_Array_جو_Arrays src, داخلا_Array labels, صحيح انگ_components) : انگ_components_2(انگ_components)
{
    هذا->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

فراغ LDA::save(ثابت String& filename) ثابت
{
    FileStorage fs(filename, FileStorage::WRITE);
    إذا (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    هذا->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
فراغ LDA::load(ثابت String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    إذا (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    هذا->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
فراغ LDA::save(FileStorage& fs) ثابت {
    // write matrices
    fs << "num_components" << انگ_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
فراغ LDA::load(ثابت FileStorage& fs) {
    //read matrices
    fs["num_components"] >> انگ_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

فراغ LDA::lda(داخلا_Array_جو_Arrays _src, داخلا_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<صحيح> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        لكل(بنانشان صحيح i = 0; i < tmp.ڪل(); i++) {
            labels.push_back(tmp.at<صحيح>(i));
        }
    }
    // turn into row sampled matrix
    Mat معلومات;
    // ensure working matrix is double precision
    src.convert_منزل(معلومات, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<صحيح> mapped_labels(labels.size());
    std::vector<صحيح> num2label = remove_dups(labels);
    std::map<صحيح, صحيح> label2num;
    لكل (صحيح i = 0; i < (صحيح)num2label.size(); i++)
        label2num[num2label[i]] = i;
    لكل (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    صحيح N = معلومات.صفون;
    صحيح D = معلومات.کانن_ڳڻپ;
    // number of unique labels
    صحيح C = (صحيح)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    إذا(C == 1) {
        String error_پيغام = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // throw error if less labels, than samples
    إذا (labels.size() != static_cast<size_t>(N)) {
        String error_پيغام = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_پيغام);
    }
    // warn if within-classes scatter matrix becomes singular
    إذا (N < D) {
        std::ٻاهر << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::سٽ_پڄاڻي;
    }
    // clip number of components to be a valid number
    إذا ((انگ_components_2 <= 0) || (انگ_components_2 >= C)) {
        انگ_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat سراسري_ڪل = Mat::zeros(1, D, معلومات.type());
    // holds the mean for each class
    std::vector<Mat> سراسري_Class(C);
    std::vector<صحيح> انگ_Class(C);
    // initialize
    لكل (صحيح i = 0; i < C; i++) {
        انگ_Class[i] = 0;
        سراسري_Class[i] = Mat::zeros(1, D, معلومات.type()); //! Dx1 image vector
    }
    // calculate sums
    لكل (صحيح i = 0; i < N; i++) {
        Mat instance = معلومات.صف(i);
        صحيح classIdx = mapped_labels[i];
        جوڙيو(سراسري_ڪل, instance, سراسري_ڪل);
        جوڙيو(سراسري_Class[classIdx], instance, سراسري_Class[classIdx]);
        انگ_Class[classIdx]++;
    }
    // calculate total mean
    سراسري_ڪل.convert_منزل(سراسري_ڪل, سراسري_ڪل.type(), 1.0 / static_cast<مزدوج> (N));
    // calculate class means
    لكل (صحيح i = 0; i < C; i++) {
        سراسري_Class[i].convert_منزل(سراسري_Class[i], سراسري_Class[i].type(), 1.0 / static_cast<مزدوج> (انگ_Class[i]));
    }
    // subtract class means
    لكل (صحيح i = 0; i < N; i++) {
        صحيح classIdx = mapped_labels[i];
        Mat instance = معلومات.صف(i);
        ڪٽيو(instance, سراسري_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, معلومات.type());
    mulTransposed(معلومات, Sw, سچ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, معلومات.type());
    لكل (صحيح i = 0; i < C; i++) {
        Mat tmp;
        ڪٽيو(سراسري_Class[i], سراسري_ڪل, tmp);
        mulTransposed(tmp, tmp, سچ);
        جوڙيو(Sb, tmp, Sb);
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
    std::vector<صحيح> sorted_indices = argsort(_eigenvalues, خطأ);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = ترتيب_ڏيو_ميٽرڪس_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = ترتيب_ڏيو_ميٽرڪس_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, انگ_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, انگ_components_2));
}

فراغ LDA::compute(داخلا_Array_جو_Arrays _src, داخلا_Array _lbls) {
    بدل(_src.قسم()) {
    حالة داخلا_Array_2::STD_ويڪٽر_MAT:
    حالة داخلا_Array_2::STD_ARRAY_MAT:
        lda(as_صف_ميٽرڪس(_src, CV_64FC1), _lbls);
        اكسر;
    حالة داخلا_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        اكسر;
    افتراضي:
        String error_پيغام= format("InputArray Datatype %d is not supported.", _src.قسم());
        CV_Error(Error::StsBadArg, error_پيغام);
        اكسر;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(داخلا_Array src) {
   أرجع subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(داخلا_Array src) {
   أرجع subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
