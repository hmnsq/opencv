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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <map>
#ਅਨ੍ਤਰ੍ਭਾਵ <set>

ਨਾਮਾਕਾਸ਼ cv
{

// Removes duplicate elements in a given vector.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp>
ਸਤਰਅੰਦਰ std::vector<_Tp> remove_dups(ਅਚਲ std::vector<_Tp>& src) {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ std::ਸੈੱਟ<_Tp>::const_iterator const_ਸੈੱਟ_Iterator;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ std::vector<_Tp>::const_iterator constVecIterator;
    std::ਸੈੱਟ<_Tp> ਸੈੱਟ_elems;
    ਲਈ (constVecIterator it = src.begin(); it != src.end(); ++it)
        ਸੈੱਟ_elems.insert(*it);
    std::vector<_Tp> elems;
    ਲਈ (const_ਸੈੱਟ_Iterator it = ਸੈੱਟ_elems.begin(); it != ਸੈੱਟ_elems.end(); ++it)
        elems.push_back(*it);
    ਵਾਪਸ elems;
}

ਸਥਿਰ Mat argsort(ਨਿਵੇਸ਼_Array _src, ਤਾਰਕਿਕ ascending=ਸੱਚ)
{
    Mat src = _src.getMat();
    ਜੇ (src.ਪੰਕਤੀਆਂ != 1 && src.ਖਾਨੇ_ਗਿਣਤੀ != 1) {
        String error_ਸੁਨੇਹਾ = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    ਪੂਰਨ flags = ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ | (ascending ? ਛਾਂਟੋ_ASCENDING : ਛਾਂਟੋ_DESCENDING);
    Mat sorted_indices;
    ਛਾਂਟੋ_Idx(src.reshape(1,1),sorted_indices,flags);
    ਵਾਪਸ sorted_indices;
}

ਸਥਿਰ Mat as_ਪੰਕਤੀ_ਮੈਟ੍ਰਿਕਸ(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays src, ਪੂਰਨ rtype, ਦੁੱਗਣਾ alpha=1, ਦੁੱਗਣਾ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ਜੇ(src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT && src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT &&
        src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_ਸਦਿਸ਼) {
        String error_ਸੁਨੇਹਾ = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // number of samples
    size_t n = src.ਕੁੱਲ();
    // return empty matrix if no matrices given
    ਜੇ(n == 0)
        ਵਾਪਸ Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).ਕੁੱਲ();
    // create data matrix
    Mat ਅੰਕੜੇ((ਪੂਰਨ)n, (ਪੂਰਨ)d, rtype);
    // now copy data
    ਲਈ(ਪੂਰਨ i = 0; i < (ਪੂਰਨ)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ਜੇ(src.getMat(i).ਕੁੱਲ() != d) {
            String error_ਸੁਨੇਹਾ = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (ਪੂਰਨ)d, (ਪੂਰਨ)src.getMat(i).ਕੁੱਲ());
            CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
        }
        // get a hold of the current row
        Mat xi = ਅੰਕੜੇ.ਪੰਕਤੀ(i);
        // make reshape happy by cloning for non-continuous matrices
        ਜੇ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_ਮੰਜ਼ਿਲ(xi, rtype, alpha, beta);
        } ਨਹੀਂਤਾਂ {
            src.getMat(i).clone().reshape(1, 1).convert_ਮੰਜ਼ਿਲ(xi, rtype, alpha, beta);
        }
    }
    ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ_ਮੈਟ੍ਰਿਕਸ_Columns_By_Indices(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _indices, OutputArray _dst) {
    ਜੇ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<ਪੂਰਨ> indices = _indices.getMat();
    _dst.create(src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.type());
    Mat dst = _dst.getMat();
    ਲਈ(size_t idx = 0; idx < indices.size(); idx++) {
        Mat original_ਖਾਨਾ = src.ਖਾਨਾ(indices[idx]);
        Mat sorted_ਖਾਨਾ = dst.ਖਾਨਾ((ਪੂਰਨ)idx);
        original_ਖਾਨਾ.copy_ਮੰਜ਼ਿਲ(sorted_ਖਾਨਾ);
    }
}

ਸਥਿਰ Mat ਛਾਂਟੋ_ਮੈਟ੍ਰਿਕਸ_Columns_By_Indices(ਨਿਵੇਸ਼_Array src, ਨਿਵੇਸ਼_Array indices) {
    Mat dst;
    ਛਾਂਟੋ_ਮੈਟ੍ਰਿਕਸ_Columns_By_Indices(src, indices, dst);
    ਵਾਪਸ dst;
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਤਾਰਕਿਕ
is_ਸਮਮਿਤ(ਨਿਵੇਸ਼_Array src) {
    Mat _src = src.getMat();
    ਜੇ(_src.ਖਾਨੇ_ਗਿਣਤੀ != _src.ਪੰਕਤੀਆਂ)
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < _src.ਪੰਕਤੀਆਂ; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < _src.ਖਾਨੇ_ਗਿਣਤੀ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ਜੇ (a != b) {
                ਵਾਪਸ ਝੂਠ;
            }
        }
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਤਾਰਕਿਕ
is_ਸਮਮਿਤ(ਨਿਵੇਸ਼_Array src, ਦੁੱਗਣਾ eps) {
    Mat _src = src.getMat();
    ਜੇ(_src.ਖਾਨੇ_ਗਿਣਤੀ != _src.ਪੰਕਤੀਆਂ)
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < _src.ਪੰਕਤੀਆਂ; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < _src.ਖਾਨੇ_ਗਿਣਤੀ; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ਜੇ (std::ਨਿਰਪੇਖ(a - b) > eps) {
                ਵਾਪਸ ਝੂਠ;
            }
        }
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ is_ਸਮਮਿਤ_2(ਨਿਵੇਸ਼_Array src, ਦੁੱਗਣਾ eps=1e-16)
{
    Mat m = src.getMat();
    ਚੋਣ (m.type()) {
        ਹਾਲਤ CV_8SC1: ਵਾਪਸ is_ਸਮਮਿਤ<ਅੱਖਰ>(m); ਰੋਕੋ;
        ਹਾਲਤ CV_8UC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(m); ਰੋਕੋ;
        ਹਾਲਤ CV_16SC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਛੋਟਾ>(m); ਰੋਕੋ;
        ਹਾਲਤ CV_16UC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ>(m); ਰੋਕੋ;
        ਹਾਲਤ CV_32SC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਪੂਰਨ>(m); ਰੋਕੋ;
        ਹਾਲਤ CV_32FC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਦਸ਼ਮਲਵ>(m, eps); ਰੋਕੋ;
        ਹਾਲਤ CV_64FC1:
            ਵਾਪਸ is_ਸਮਮਿਤ<ਦੁੱਗਣਾ>(m, eps); ਰੋਕੋ;
        ਮੂਲ:
            ਰੋਕੋ;
    }
    ਵਾਪਸ ਝੂਠ;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(ਨਿਵੇਸ਼_Array _W, ਨਿਵੇਸ਼_Array ਔਸਤ, ਨਿਵੇਸ਼_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat ਔਸਤ_2 = ਔਸਤ.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ਪੂਰਨ n = src.ਪੰਕਤੀਆਂ;
    ਪੂਰਨ d = src.ਖਾਨੇ_ਗਿਣਤੀ;
    // make sure the data has the correct shape
    ਜੇ(W.ਪੰਕਤੀਆਂ != d) {
        String error_ਸੁਨੇਹਾ = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, W.ਪੰਕਤੀਆਂ, W.ਖਾਨੇ_ਗਿਣਤੀ);
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // make sure mean is correct if not empty
    ਜੇ(!ਔਸਤ_2.empty() && (ਔਸਤ_2.ਕੁੱਲ() != (size_t) d)) {
        String error_ਸੁਨੇਹਾ = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, ਔਸਤ_2.ਕੁੱਲ());
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_ਮੰਜ਼ਿਲ(X, W.type());
    // safe to do, because of above assertion
    ਜੇ(!ਔਸਤ_2.empty()) {
        ਲਈ(ਪੂਰਨ i=0; i<n; i++) {
            Mat r_i = X.ਪੰਕਤੀ(i);
            ਘਟਾਓ(r_i, ਔਸਤ_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    ਵਾਪਸ Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(ਨਿਵੇਸ਼_Array _W, ਨਿਵੇਸ਼_Array ਔਸਤ, ਨਿਵੇਸ਼_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat ਔਸਤ_2 = ਔਸਤ.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    ਪੂਰਨ n = src.ਪੰਕਤੀਆਂ;
    ਪੂਰਨ d = src.ਖਾਨੇ_ਗਿਣਤੀ;
    // make sure the data has the correct shape
    ਜੇ(W.ਖਾਨੇ_ਗਿਣਤੀ != d) {
        String error_ਸੁਨੇਹਾ = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, W.ਪੰਕਤੀਆਂ, W.ਖਾਨੇ_ਗਿਣਤੀ);
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // make sure mean is correct if not empty
    ਜੇ(!ਔਸਤ_2.empty() && (ਔਸਤ_2.ਕੁੱਲ() != (size_t) W.ਪੰਕਤੀਆਂ)) {
        String error_ਸੁਨੇਹਾ = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.ਖਾਨੇ_ਗਿਣਤੀ, ਔਸਤ_2.ਕੁੱਲ());
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_ਮੰਜ਼ਿਲ(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    ਜੇ(!ਔਸਤ_2.empty()) {
        ਲਈ(ਪੂਰਨ i=0; i<n; i++) {
            Mat r_i = X.ਪੰਕਤੀ(i);
            ਜੋੜੋ(r_i, ਔਸਤ_2.reshape(1,1), r_i);
        }
    }
    ਵਾਪਸ X;
}


ਵਰਗ EigenvalueDecomposition {
ਨਿਜੀ:

    // Holds the data dimension.
    ਪੂਰਨ n;

    // Pointer to internal memory.
    ਦੁੱਗਣਾ *d, *e, *ort;
    ਦੁੱਗਣਾ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp>
    _Tp *alloc_1d(ਪੂਰਨ m) {
        ਵਾਪਸ ਨਵਾਂ _Tp[m];
    }

    // Allocates memory.
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp>
    _Tp *alloc_1d(ਪੂਰਨ m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        ਲਈ (ਪੂਰਨ i = 0; i < m; i++)
            arr[i] = val;
        ਵਾਪਸ arr;
    }

    // Allocates memory.
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp>
    _Tp **alloc_2d(ਪੂਰਨ m, ਪੂਰਨ _n) {
        _Tp **arr = ਨਵਾਂ _Tp*[m];
        ਲਈ (ਪੂਰਨ i = 0; i < m; i++)
            arr[i] = ਨਵਾਂ _Tp[_n];
        ਵਾਪਸ arr;
    }

    // Allocates memory.
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp>
    _Tp **alloc_2d(ਪੂਰਨ m, ਪੂਰਨ _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        ਲਈ (ਪੂਰਨ i = 0; i < m; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        ਵਾਪਸ arr;
    }

    ਸਥਿਰ ਖਾਲੀ complex_div(ਦੁੱਗਣਾ xr, ਦੁੱਗਣਾ xi, ਦੁੱਗਣਾ yr, ਦੁੱਗਣਾ yi, ਦੁੱਗਣਾ& cdivr, ਦੁੱਗਣਾ& cdivi) {
        ਦੁੱਗਣਾ r, dv;
        CV_DbgAssert(std::ਨਿਰਪੇਖ(yr) + std::ਨਿਰਪੇਖ(yi) > 0.0);
        ਜੇ (std::ਨਿਰਪੇਖ(yr) > std::ਨਿਰਪੇਖ(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } ਨਹੀਂਤਾਂ {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    ਖਾਲੀ hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        ਅਚਲ ਪੂਰਨ max_iters_ਗਿਣਤੀ = 1000 * ਇਹ->n;

        ਅਚਲ ਪੂਰਨ nn = ਇਹ->n; CV_Assert(nn > 0);
        ਪੂਰਨ n1 = nn - 1;
        ਅਚਲ ਪੂਰਨ ਨੀਵਾਂ = 0;
        ਅਚਲ ਪੂਰਨ ਉੱਚਾ = nn - 1;
        ਅਚਲ ਦੁੱਗਣਾ eps = std::numeric_limits<ਦੁੱਗਣਾ>::epsilon();
        ਦੁੱਗਣਾ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ਦੁੱਗਣਾ norm = 0.0;
        ਲਈ (ਪੂਰਨ i = 0; i < nn; i++) {
#ਜੇ 0 // 'if' condition is always false
            ਜੇ (i < ਨੀਵਾਂ || i > ਉੱਚਾ) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#ਜੇ_ਅੰਤ
            ਲਈ (ਪੂਰਨ j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::ਨਿਰਪੇਖ(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        ਪੂਰਨ iter = 0;
        ਜਦੋਂ (n1 >= ਨੀਵਾਂ) {

            // Look for single small sub-diagonal element
            ਪੂਰਨ l = n1;
            ਜਦੋਂ (l > ਨੀਵਾਂ) {
                ਜੇ (norm < FLT_EPSILON) {
                    ਰੋਕੋ;
                }
                ਦੁੱਗਣਾ s = std::ਨਿਰਪੇਖ(H[l - 1][l - 1]) + std::ਨਿਰਪੇਖ(H[l][l]);
                ਜੇ (s == 0.0) {
                    s = norm;
                }
                ਜੇ (std::ਨਿਰਪੇਖ(H[l][l - 1]) < eps * s) {
                    ਰੋਕੋ;
                }
                l--;
            }

            // Check for convergence
            ਜੇ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } ਨਹੀਂਤਾਂ ਜੇ (l == n1 - 1) {
                // Two roots found
                ਦੁੱਗਣਾ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ਦੁੱਗਣਾ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ਦੁੱਗਣਾ q = p * p + w;
                ਦੁੱਗਣਾ z = std::ਵਰਗਮੂਲ(std::ਨਿਰਪੇਖ(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ਦੁੱਗਣਾ x = H[n1][n1];

                ਜੇ (q >= 0) {
                    // Real pair
                    ਜੇ (p >= 0) {
                        z = p + z;
                    } ਨਹੀਂਤਾਂ {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ਜੇ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ਦੁੱਗਣਾ s = std::ਨਿਰਪੇਖ(x) + std::ਨਿਰਪੇਖ(z);
                    p = x / s;
                    q = z / s;
                    ਦੁੱਗਣਾ r = std::ਵਰਗਮੂਲ(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    ਲਈ (ਪੂਰਨ j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    ਲਈ (ਪੂਰਨ i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    ਲਈ (ਪੂਰਨ i = ਨੀਵਾਂ; i <= ਉੱਚਾ; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } ਨਹੀਂਤਾਂ {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } ਨਹੀਂਤਾਂ {
                // No convergence yet

                // Form shift
                ਦੁੱਗਣਾ x = H[n1][n1];
                ਦੁੱਗਣਾ y = 0.0;
                ਦੁੱਗਣਾ w = 0.0;
                ਜੇ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ਜੇ (iter == 10) {
                    exshift += x;
                    ਲਈ (ਪੂਰਨ i = ਨੀਵਾਂ; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ਦੁੱਗਣਾ s = std::ਨਿਰਪੇਖ(H[n1][n1 - 1]) + std::ਨਿਰਪੇਖ(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ਜੇ (iter == 30) {
                    ਦੁੱਗਣਾ s = (y - x) * 0.5;
                    s = s * s + w;
                    ਜੇ (s > 0) {
                        s = std::ਵਰਗਮੂਲ(s);
                        ਜੇ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        ਲਈ (ਪੂਰਨ i = ਨੀਵਾਂ; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ਜੇ (iter > max_iters_ਗਿਣਤੀ)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ਦੁੱਗਣਾ p = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();
                ਦੁੱਗਣਾ q = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();
                ਦੁੱਗਣਾ r = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                ਪੂਰਨ m = n1 - 2;
                ਜਦੋਂ (m >= l) {
                    ਦੁੱਗਣਾ z = H[m][m];
                    r = x - z;
                    ਦੁੱਗਣਾ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::ਨਿਰਪੇਖ(p) + std::ਨਿਰਪੇਖ(q) + std::ਨਿਰਪੇਖ(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ਜੇ (m == l) {
                        ਰੋਕੋ;
                    }
                    ਜੇ (std::ਨਿਰਪੇਖ(H[m][m - 1]) * (std::ਨਿਰਪੇਖ(q) + std::ਨਿਰਪੇਖ(r)) < eps * (std::ਨਿਰਪੇਖ(p)
                                                                                     * (std::ਨਿਰਪੇਖ(H[m - 1][m - 1]) + std::ਨਿਰਪੇਖ(z) + std::ਨਿਰਪੇਖ(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ਰੋਕੋ;
                    }
                    m--;
                }

                ਲਈ (ਪੂਰਨ i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ਜੇ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                ਲਈ (ਪੂਰਨ k = m; k < n1; k++) {

                    ਤਾਰਕਿਕ notlast = (k != n1 - 1);
                    ਜੇ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::ਨਿਰਪੇਖ(p) + std::ਨਿਰਪੇਖ(q) + std::ਨਿਰਪੇਖ(r);
                        ਜੇ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ਜੇ (x == 0.0) {
                        ਰੋਕੋ;
                    }
                    ਦੁੱਗਣਾ s = std::ਵਰਗਮੂਲ(p * p + q * q + r * r);
                    ਜੇ (p < 0) {
                        s = -s;
                    }
                    ਜੇ (s != 0) {
                        ਜੇ (k != m) {
                            H[k][k - 1] = -s * x;
                        } ਨਹੀਂਤਾਂ ਜੇ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ਦੁੱਗਣਾ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        ਲਈ (ਪੂਰਨ j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ਜੇ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        ਲਈ (ਪੂਰਨ i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ਜੇ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        ਲਈ (ਪੂਰਨ i = ਨੀਵਾਂ; i <= ਉੱਚਾ; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ਜੇ (notlast) {
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

        ਜੇ (norm < FLT_EPSILON) {
            ਵਾਪਸ;
        }

        ਲਈ (n1 = nn - 1; n1 >= 0; n1--) {
            ਦੁੱਗਣਾ p = d[n1];
            ਦੁੱਗਣਾ q = e[n1];

            ਜੇ (q == 0) {
                // Real vector
                ਦੁੱਗਣਾ z = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();
                ਦੁੱਗਣਾ s = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();

                ਪੂਰਨ l = n1;
                H[n1][n1] = 1.0;
                ਲਈ (ਪੂਰਨ i = n1 - 1; i >= 0; i--) {
                    ਦੁੱਗਣਾ w = H[i][i] - p;
                    ਦੁੱਗਣਾ r = 0.0;
                    ਲਈ (ਪੂਰਨ j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ਜੇ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } ਨਹੀਂਤਾਂ {
                        l = i;
                        ਜੇ (e[i] == 0.0) {
                            ਜੇ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } ਨਹੀਂਤਾਂ {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } ਨਹੀਂਤਾਂ {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ਦੁੱਗਣਾ x = H[i][i + 1];
                            ਦੁੱਗਣਾ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ਦੁੱਗਣਾ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ਜੇ (std::ਨਿਰਪੇਖ(x) > std::ਨਿਰਪੇਖ(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } ਨਹੀਂਤਾਂ {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ਦੁੱਗਣਾ t = std::ਨਿਰਪੇਖ(H[i][n1]);
                        ਜੇ ((eps * t) * t > 1) {
                            ਦੁੱਗਣਾ inv_t = 1.0 / t;
                            ਲਈ (ਪੂਰਨ j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } ਨਹੀਂਤਾਂ ਜੇ (q < 0) {
                // Complex vector
                ਦੁੱਗਣਾ z = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();
                ਦੁੱਗਣਾ r = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();
                ਦੁੱਗਣਾ s = std::numeric_limits<ਦੁੱਗਣਾ>::quiet_NaN();

                ਪੂਰਨ l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ਜੇ (std::ਨਿਰਪੇਖ(H[n1][n1 - 1]) > std::ਨਿਰਪੇਖ(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } ਨਹੀਂਤਾਂ {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                ਲਈ (ਪੂਰਨ i = n1 - 2; i >= 0; i--) {
                    ਦੁੱਗਣਾ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    ਲਈ (ਪੂਰਨ j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ਦੁੱਗਣਾ w = H[i][i] - p;

                    ਜੇ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } ਨਹੀਂਤਾਂ {
                        l = i;
                        ਜੇ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } ਨਹੀਂਤਾਂ {
                            // Solve complex equations

                            ਦੁੱਗਣਾ x = H[i][i + 1];
                            ਦੁੱਗਣਾ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ਜੇ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::ਨਿਰਪੇਖ(w) + std::ਨਿਰਪੇਖ(q) + std::ਨਿਰਪੇਖ(x)
                                                   + std::ਨਿਰਪੇਖ(y) + std::ਨਿਰਪੇਖ(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ਜੇ (std::ਨਿਰਪੇਖ(x) > (std::ਨਿਰਪੇਖ(z) + std::ਨਿਰਪੇਖ(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } ਨਹੀਂਤਾਂ {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ਦੁੱਗਣਾ t = std::max(std::ਨਿਰਪੇਖ(H[i][n1 - 1]), std::ਨਿਰਪੇਖ(H[i][n1]));
                        ਜੇ ((eps * t) * t > 1) {
                            ਲਈ (ਪੂਰਨ j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ਜੇ 0 // 'if' condition is always false
        ਲਈ (ਪੂਰਨ i = 0; i < nn; i++) {
            ਜੇ (i < ਨੀਵਾਂ || i > ਉੱਚਾ) {
                ਲਈ (ਪੂਰਨ j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#ਜੇ_ਅੰਤ

        // Back transformation to get eigenvectors of original matrix

        ਲਈ (ਪੂਰਨ j = nn - 1; j >= ਨੀਵਾਂ; j--) {
            ਲਈ (ਪੂਰਨ i = ਨੀਵਾਂ; i <= ਉੱਚਾ; i++) {
                ਦੁੱਗਣਾ z = 0.0;
                ਲਈ (ਪੂਰਨ k = ਨੀਵਾਂ; k <= std::min(j, ਉੱਚਾ); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    ਖਾਲੀ orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        ਪੂਰਨ ਨੀਵਾਂ = 0;
        ਪੂਰਨ ਉੱਚਾ = n - 1;

        ਲਈ (ਪੂਰਨ m = ਨੀਵਾਂ + 1; m < ਉੱਚਾ; m++) {

            // Scale column.

            ਦੁੱਗਣਾ scale = 0.0;
            ਲਈ (ਪੂਰਨ i = m; i <= ਉੱਚਾ; i++) {
                scale = scale + std::ਨਿਰਪੇਖ(H[i][m - 1]);
            }
            ਜੇ (scale != 0.0) {

                // Compute Householder transformation.

                ਦੁੱਗਣਾ h = 0.0;
                ਲਈ (ਪੂਰਨ i = ਉੱਚਾ; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ਦੁੱਗਣਾ g = std::ਵਰਗਮੂਲ(h);
                ਜੇ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                ਲਈ (ਪੂਰਨ j = m; j < n; j++) {
                    ਦੁੱਗਣਾ f = 0.0;
                    ਲਈ (ਪੂਰਨ i = ਉੱਚਾ; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    ਲਈ (ਪੂਰਨ i = m; i <= ਉੱਚਾ; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                ਲਈ (ਪੂਰਨ i = 0; i <= ਉੱਚਾ; i++) {
                    ਦੁੱਗਣਾ f = 0.0;
                    ਲਈ (ਪੂਰਨ j = ਉੱਚਾ; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    ਲਈ (ਪੂਰਨ j = m; j <= ਉੱਚਾ; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        ਲਈ (ਪੂਰਨ m = ਉੱਚਾ - 1; m > ਨੀਵਾਂ; m--) {
            ਜੇ (H[m][m - 1] != 0.0) {
                ਲਈ (ਪੂਰਨ i = m + 1; i <= ਉੱਚਾ; i++) {
                    ort[i] = H[i][m - 1];
                }
                ਲਈ (ਪੂਰਨ j = m; j <= ਉੱਚਾ; j++) {
                    ਦੁੱਗਣਾ g = 0.0;
                    ਲਈ (ਪੂਰਨ i = m; i <= ਉੱਚਾ; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    ਲਈ (ਪੂਰਨ i = m; i <= ਉੱਚਾ; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    ਖਾਲੀ release() {
        // releases the working data
        ਮਿਟਾਓ[] d; d = ਨਿਰਾ;
        ਮਿਟਾਓ[] e; e = ਨਿਰਾ;
        ਮਿਟਾਓ[] ort; ort = ਨਿਰਾ;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ਜੇ (H) ਮਿਟਾਓ[] H[i];
            ਜੇ (V) ਮਿਟਾਓ[] V[i];
        }
        ਮਿਟਾਓ[] H; H = ਨਿਰਾ;
        ਮਿਟਾਓ[] V; V = ਨਿਰਾ;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    ਖਾਲੀ compute() {
        // Allocate memory for the working data.
        V = alloc_2d<ਦੁੱਗਣਾ> (n, n, 0.0);
        d = alloc_1d<ਦੁੱਗਣਾ> (n);
        e = alloc_1d<ਦੁੱਗਣਾ> (n);
        ort = alloc_1d<ਦੁੱਗਣਾ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
                _eigenvalues.at<ਦੁੱਗਣਾ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
                ਲਈ (ਪੂਰਨ j = 0; j < n; j++)
                    _eigenvectors.at<ਦੁੱਗਣਾ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

ਜਨਤਕ:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ਨਿਰਾ), e(ਨਿਰਾ), ort(ਨਿਰਾ),
        V(ਨਿਰਾ), H(ਨਿਰਾ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    ਖਾਲੀ compute(ਨਿਵੇਸ਼_Array src, ਤਾਰਕਿਕ fallback_ਸਮਮਿਤ = ਸੱਚ)
    {
        CV_INSTRUMENT_REGION();

        ਜੇ(fallback_ਸਮਮਿਤ && is_ਸਮਮਿਤ_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } ਨਹੀਂਤਾਂ {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_ਮੰਜ਼ਿਲ(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ਇਹ->n = tmp.ਖਾਨੇ_ਗਿਣਤੀ;
            // Allocate the matrix data to work on.
            ਇਹ->H = alloc_2d<ਦੁੱਗਣਾ> (n, n);
            // Now safely copy the data.
            ਲਈ (ਪੂਰਨ i = 0; i < tmp.ਪੰਕਤੀਆਂ; i++) {
                ਲਈ (ਪੂਰਨ j = 0; j < tmp.ਖਾਨੇ_ਗਿਣਤੀ; j++) {
                    ਇਹ->H[i][j] = tmp.at<ਦੁੱਗਣਾ>(i, j);
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
    Mat eigenvalues() ਅਚਲ { ਵਾਪਸ _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ਅਚਲ { ਵਾਪਸ _eigenvectors; }
};

ਖਾਲੀ eigen_Non_ਸਮਮਿਤ(ਨਿਵੇਸ਼_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ਪੂਰਨ type = src.type();
    ਪੂਰਨ n = src.ਪੰਕਤੀਆਂ;

    CV_Assert(src.ਪੰਕਤੀਆਂ == src.ਖਾਨੇ_ਗਿਣਤੀ);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ਜੇ (type == CV_32F)
        src.convert_ਮੰਜ਼ਿਲ(src64f, CV_32FC1);
    ਨਹੀਂਤਾਂ
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, ਝੂਠ);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ਦੁੱਗਣਾ> eigenvalues64f;
    eigensystem.eigenvalues().copy_ਮੰਜ਼ਿਲ(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<ਪੂਰਨ> ਛਾਂਟੋ_indexes(n);
    cv::ਛਾਂਟੋ_Idx(eigenvalues64f, ਛਾਂਟੋ_indexes, ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ | ਛਾਂਟੋ_DESCENDING);

    std::vector<ਦੁੱਗਣਾ> sorted_eigenvalues64f(n);
    ਲਈ (ਪੂਰਨ i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[ਛਾਂਟੋ_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_ਮੰਜ਼ਿਲ(_evals, type);

    ਜੇ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.ਪੰਕਤੀਆਂ == n);
        CV_Assert(eigenvectors64f.ਖਾਨੇ_ਗਿਣਤੀ == n);
        Mat_<ਦੁੱਗਣਾ> sorted_eigenvectors64f(n, n, CV_64FC1);
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
        {
            ਦੁੱਗਣਾ* pDst = sorted_eigenvectors64f.ptr<ਦੁੱਗਣਾ>(i);
            ਦੁੱਗਣਾ* pSrc = eigenvectors64f.ptr<ਦੁੱਗਣਾ>(ਛਾਂਟੋ_indexes[i]);
            CV_Assert(pSrc != ਨਿਰਾ);
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(pDst, pSrc, n * ਆਕਾਰ(ਦੁੱਗਣਾ));
        }
        sorted_eigenvectors64f.convert_ਮੰਜ਼ਿਲ(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(ਪੂਰਨ ਅੰਕ_components) : ਅੰਕ_components_2(ਅੰਕ_components) { }

LDA::LDA(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays src, ਨਿਵੇਸ਼_Array labels, ਪੂਰਨ ਅੰਕ_components) : ਅੰਕ_components_2(ਅੰਕ_components)
{
    ਇਹ->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

ਖਾਲੀ LDA::save(ਅਚਲ String& filename) ਅਚਲ
{
    FileStorage fs(filename, FileStorage::WRITE);
    ਜੇ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ਇਹ->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
ਖਾਲੀ LDA::load(ਅਚਲ String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ਜੇ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ਇਹ->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
ਖਾਲੀ LDA::save(FileStorage& fs) ਅਚਲ {
    // write matrices
    fs << "num_components" << ਅੰਕ_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
ਖਾਲੀ LDA::load(ਅਚਲ FileStorage& fs) {
    //read matrices
    fs["num_components"] >> ਅੰਕ_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

ਖਾਲੀ LDA::lda(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _src, ਨਿਵੇਸ਼_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<ਪੂਰਨ> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        ਲਈ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < tmp.ਕੁੱਲ(); i++) {
            labels.push_back(tmp.at<ਪੂਰਨ>(i));
        }
    }
    // turn into row sampled matrix
    Mat ਅੰਕੜੇ;
    // ensure working matrix is double precision
    src.convert_ਮੰਜ਼ਿਲ(ਅੰਕੜੇ, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<ਪੂਰਨ> mapped_labels(labels.size());
    std::vector<ਪੂਰਨ> num2label = remove_dups(labels);
    std::map<ਪੂਰਨ, ਪੂਰਨ> label2num;
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)num2label.size(); i++)
        label2num[num2label[i]] = i;
    ਲਈ (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    ਪੂਰਨ N = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
    ਪੂਰਨ D = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
    // number of unique labels
    ਪੂਰਨ C = (ਪੂਰਨ)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ਜੇ(C == 1) {
        String error_ਸੁਨੇਹਾ = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // throw error if less labels, than samples
    ਜੇ (labels.size() != static_cast<size_t>(N)) {
        String error_ਸੁਨੇਹਾ = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
    }
    // warn if within-classes scatter matrix becomes singular
    ਜੇ (N < D) {
        std::ਬਾਹਰ << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::ਸਤਰ_ਅੰਤ;
    }
    // clip number of components to be a valid number
    ਜੇ ((ਅੰਕ_components_2 <= 0) || (ਅੰਕ_components_2 >= C)) {
        ਅੰਕ_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat ਔਸਤ_ਕੁੱਲ = Mat::zeros(1, D, ਅੰਕੜੇ.type());
    // holds the mean for each class
    std::vector<Mat> ਔਸਤ_Class(C);
    std::vector<ਪੂਰਨ> ਅੰਕ_Class(C);
    // initialize
    ਲਈ (ਪੂਰਨ i = 0; i < C; i++) {
        ਅੰਕ_Class[i] = 0;
        ਔਸਤ_Class[i] = Mat::zeros(1, D, ਅੰਕੜੇ.type()); //! Dx1 image vector
    }
    // calculate sums
    ਲਈ (ਪੂਰਨ i = 0; i < N; i++) {
        Mat instance = ਅੰਕੜੇ.ਪੰਕਤੀ(i);
        ਪੂਰਨ classIdx = mapped_labels[i];
        ਜੋੜੋ(ਔਸਤ_ਕੁੱਲ, instance, ਔਸਤ_ਕੁੱਲ);
        ਜੋੜੋ(ਔਸਤ_Class[classIdx], instance, ਔਸਤ_Class[classIdx]);
        ਅੰਕ_Class[classIdx]++;
    }
    // calculate total mean
    ਔਸਤ_ਕੁੱਲ.convert_ਮੰਜ਼ਿਲ(ਔਸਤ_ਕੁੱਲ, ਔਸਤ_ਕੁੱਲ.type(), 1.0 / static_cast<ਦੁੱਗਣਾ> (N));
    // calculate class means
    ਲਈ (ਪੂਰਨ i = 0; i < C; i++) {
        ਔਸਤ_Class[i].convert_ਮੰਜ਼ਿਲ(ਔਸਤ_Class[i], ਔਸਤ_Class[i].type(), 1.0 / static_cast<ਦੁੱਗਣਾ> (ਅੰਕ_Class[i]));
    }
    // subtract class means
    ਲਈ (ਪੂਰਨ i = 0; i < N; i++) {
        ਪੂਰਨ classIdx = mapped_labels[i];
        Mat instance = ਅੰਕੜੇ.ਪੰਕਤੀ(i);
        ਘਟਾਓ(instance, ਔਸਤ_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, ਅੰਕੜੇ.type());
    mulTransposed(ਅੰਕੜੇ, Sw, ਸੱਚ);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, ਅੰਕੜੇ.type());
    ਲਈ (ਪੂਰਨ i = 0; i < C; i++) {
        Mat tmp;
        ਘਟਾਓ(ਔਸਤ_Class[i], ਔਸਤ_ਕੁੱਲ, tmp);
        mulTransposed(tmp, tmp, ਸੱਚ);
        ਜੋੜੋ(Sb, tmp, Sb);
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
    std::vector<ਪੂਰਨ> sorted_indices = argsort(_eigenvalues, ਝੂਠ);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = ਛਾਂਟੋ_ਮੈਟ੍ਰਿਕਸ_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = ਛਾਂਟੋ_ਮੈਟ੍ਰਿਕਸ_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, ਅੰਕ_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, ਅੰਕ_components_2));
}

ਖਾਲੀ LDA::compute(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _src, ਨਿਵੇਸ਼_Array _lbls) {
    ਚੋਣ(_src.ਕਿਸਮ()) {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT:
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT:
        lda(as_ਪੰਕਤੀ_ਮੈਟ੍ਰਿਕਸ(_src, CV_64FC1), _lbls);
        ਰੋਕੋ;
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ਰੋਕੋ;
    ਮੂਲ:
        String error_ਸੁਨੇਹਾ= format("InputArray Datatype %d is not supported.", _src.ਕਿਸਮ());
        CV_Error(Error::StsBadArg, error_ਸੁਨੇਹਾ);
        ਰੋਕੋ;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(ਨਿਵੇਸ਼_Array src) {
   ਵਾਪਸ subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(ਨਿਵੇਸ਼_Array src) {
   ਵਾਪਸ subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
