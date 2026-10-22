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
प्रतिरूपम्<प्रकारनाम _Tp>
अन्तःस्थ std::vector<_Tp> remove_dups(ध्रुव std::vector<_Tp>& src) {
    प्रकारनिर्देश प्रकारनाम std::समुच्चयः<_Tp>::const_iterator const_समुच्चयः_Iterator;
    प्रकारनिर्देश प्रकारनाम std::vector<_Tp>::const_iterator constVecIterator;
    std::समुच्चयः<_Tp> समुच्चयः_elems;
    प्रति (constVecIterator it = src.begin(); it != src.end(); ++it)
        समुच्चयः_elems.insert(*it);
    std::vector<_Tp> elems;
    प्रति (const_समुच्चयः_Iterator it = समुच्चयः_elems.begin(); it != समुच्चयः_elems.end(); ++it)
        elems.push_back(*it);
    प्रत्यागम elems;
}

स्थिर Mat argsort(निवेशः_Array _src, तार्किक ascending=सत्य)
{
    Mat src = _src.getMat();
    यदि (src.पङ्क्तिकाः != 1 && src.स्तम्भसङ्ख्या != 1) {
        String error_सन्देशः = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    पूर्णाङ्क flags = क्रमय_EVERY_ROW | (ascending ? क्रमय_ASCENDING : क्रमय_DESCENDING);
    Mat sorted_indices;
    क्रमय_Idx(src.reshape(1,1),sorted_indices,flags);
    प्रत्यागम sorted_indices;
}

स्थिर Mat asRowMatrix(निवेशः_Array_सम्बन्धि_Arrays src, पूर्णाङ्क rtype, द्विगुण alpha=1, द्विगुण beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    यदि(src.विधा() != निवेशः_Array_2::STD_सदिशम्_MAT && src.विधा() != निवेशः_Array_2::STD_ARRAY_MAT &&
        src.विधा() != निवेशः_Array_2::STD_सदिशम्_सदिशम्) {
        String error_सन्देशः = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // number of samples
    size_t n = src.योग();
    // return empty matrix if no matrices given
    यदि(n == 0)
        प्रत्यागम Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).योग();
    // create data matrix
    Mat दत्त((पूर्णाङ्क)n, (पूर्णाङ्क)d, rtype);
    // now copy data
    प्रति(पूर्णाङ्क i = 0; i < (पूर्णाङ्क)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        यदि(src.getMat(i).योग() != d) {
            String error_सन्देशः = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (पूर्णाङ्क)d, (पूर्णाङ्क)src.getMat(i).योग());
            CV_Error(Error::StsBadArg, error_सन्देशः);
        }
        // get a hold of the current row
        Mat xi = दत्त.row(i);
        // make reshape happy by cloning for non-continuous matrices
        यदि(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_गन्तव्यम्(xi, rtype, alpha, beta);
        } अन्यथा {
            src.getMat(i).clone().reshape(1, 1).convert_गन्तव्यम्(xi, rtype, alpha, beta);
        }
    }
    प्रत्यागम दत्त;
}

स्थिर शून्य क्रमय_Matrix_Columns_By_Indices(निवेशः_Array _src, निवेशः_Array _indices, OutputArray _dst) {
    यदि(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<पूर्णाङ्क> indices = _indices.getMat();
    _dst.create(src.पङ्क्तिकाः, src.स्तम्भसङ्ख्या, src.type());
    Mat dst = _dst.getMat();
    प्रति(size_t idx = 0; idx < indices.size(); idx++) {
        Mat originalCol = src.col(indices[idx]);
        Mat sortedCol = dst.col((पूर्णाङ्क)idx);
        originalCol.copy_गन्तव्यम्(sortedCol);
    }
}

स्थिर Mat क्रमय_Matrix_Columns_By_Indices(निवेशः_Array src, निवेशः_Array indices) {
    Mat dst;
    क्रमय_Matrix_Columns_By_Indices(src, indices, dst);
    प्रत्यागम dst;
}


प्रतिरूपम्<प्रकारनाम _Tp> स्थिर तार्किक
is_सममितम्(निवेशः_Array src) {
    Mat _src = src.getMat();
    यदि(_src.स्तम्भसङ्ख्या != _src.पङ्क्तिकाः)
        प्रत्यागम असत्य;
    प्रति (पूर्णाङ्क i = 0; i < _src.पङ्क्तिकाः; i++) {
        प्रति (पूर्णाङ्क j = 0; j < _src.स्तम्भसङ्ख्या; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            यदि (a != b) {
                प्रत्यागम असत्य;
            }
        }
    }
    प्रत्यागम सत्य;
}

प्रतिरूपम्<प्रकारनाम _Tp> स्थिर तार्किक
is_सममितम्(निवेशः_Array src, द्विगुण eps) {
    Mat _src = src.getMat();
    यदि(_src.स्तम्भसङ्ख्या != _src.पङ्क्तिकाः)
        प्रत्यागम असत्य;
    प्रति (पूर्णाङ्क i = 0; i < _src.पङ्क्तिकाः; i++) {
        प्रति (पूर्णाङ्क j = 0; j < _src.स्तम्भसङ्ख्या; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            यदि (std::निरपेक्षम्(a - b) > eps) {
                प्रत्यागम असत्य;
            }
        }
    }
    प्रत्यागम सत्य;
}

स्थिर तार्किक is_सममितम्_2(निवेशः_Array src, द्विगुण eps=1e-16)
{
    Mat m = src.getMat();
    विकल्प (m.type()) {
        पक्ष CV_8SC1: प्रत्यागम is_सममितम्<वर्ण>(m); भङ्ग;
        पक्ष CV_8UC1:
            प्रत्यागम is_सममितम्<अचिह्नित वर्ण>(m); भङ्ग;
        पक्ष CV_16SC1:
            प्रत्यागम is_सममितम्<ह्रस्व>(m); भङ्ग;
        पक्ष CV_16UC1:
            प्रत्यागम is_सममितम्<अचिह्नित ह्रस्व>(m); भङ्ग;
        पक्ष CV_32SC1:
            प्रत्यागम is_सममितम्<पूर्णाङ्क>(m); भङ्ग;
        पक्ष CV_32FC1:
            प्रत्यागम is_सममितम्<भिन्न>(m, eps); भङ्ग;
        पक्ष CV_64FC1:
            प्रत्यागम is_सममितम्<द्विगुण>(m, eps); भङ्ग;
        स्वभाव:
            भङ्ग;
    }
    प्रत्यागम असत्य;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(निवेशः_Array _W, निवेशः_Array माध्यम्, निवेशः_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat माध्यम्_2 = माध्यम्.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णाङ्क n = src.पङ्क्तिकाः;
    पूर्णाङ्क d = src.स्तम्भसङ्ख्या;
    // make sure the data has the correct shape
    यदि(W.पङ्क्तिकाः != d) {
        String error_सन्देशः = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.पङ्क्तिकाः, src.स्तम्भसङ्ख्या, W.पङ्क्तिकाः, W.स्तम्भसङ्ख्या);
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // make sure mean is correct if not empty
    यदि(!माध्यम्_2.empty() && (माध्यम्_2.योग() != (size_t) d)) {
        String error_सन्देशः = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, माध्यम्_2.योग());
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_गन्तव्यम्(X, W.type());
    // safe to do, because of above assertion
    यदि(!माध्यम्_2.empty()) {
        प्रति(पूर्णाङ्क i=0; i<n; i++) {
            Mat r_i = X.row(i);
            व्यवकलय(r_i, माध्यम्_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    प्रत्यागम Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(निवेशः_Array _W, निवेशः_Array माध्यम्, निवेशः_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat माध्यम्_2 = माध्यम्.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    पूर्णाङ्क n = src.पङ्क्तिकाः;
    पूर्णाङ्क d = src.स्तम्भसङ्ख्या;
    // make sure the data has the correct shape
    यदि(W.स्तम्भसङ्ख्या != d) {
        String error_सन्देशः = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.पङ्क्तिकाः, src.स्तम्भसङ्ख्या, W.पङ्क्तिकाः, W.स्तम्भसङ्ख्या);
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // make sure mean is correct if not empty
    यदि(!माध्यम्_2.empty() && (माध्यम्_2.योग() != (size_t) W.पङ्क्तिकाः)) {
        String error_सन्देशः = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.स्तम्भसङ्ख्या, माध्यम्_2.योग());
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_गन्तव्यम्(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    यदि(!माध्यम्_2.empty()) {
        प्रति(पूर्णाङ्क i=0; i<n; i++) {
            Mat r_i = X.row(i);
            सङ्कलय(r_i, माध्यम्_2.reshape(1,1), r_i);
        }
    }
    प्रत्यागम X;
}


वर्ग EigenvalueDecomposition {
गुप्त:

    // Holds the data dimension.
    पूर्णाङ्क n;

    // Pointer to internal memory.
    द्विगुण *d, *e, *ort;
    द्विगुण **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    प्रतिरूपम्<प्रकारनाम _Tp>
    _Tp *alloc_1d(पूर्णाङ्क m) {
        प्रत्यागम नूतन _Tp[m];
    }

    // Allocates memory.
    प्रतिरूपम्<प्रकारनाम _Tp>
    _Tp *alloc_1d(पूर्णाङ्क m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        प्रति (पूर्णाङ्क i = 0; i < m; i++)
            arr[i] = val;
        प्रत्यागम arr;
    }

    // Allocates memory.
    प्रतिरूपम्<प्रकारनाम _Tp>
    _Tp **alloc_2d(पूर्णाङ्क m, पूर्णाङ्क _n) {
        _Tp **arr = नूतन _Tp*[m];
        प्रति (पूर्णाङ्क i = 0; i < m; i++)
            arr[i] = नूतन _Tp[_n];
        प्रत्यागम arr;
    }

    // Allocates memory.
    प्रतिरूपम्<प्रकारनाम _Tp>
    _Tp **alloc_2d(पूर्णाङ्क m, पूर्णाङ्क _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        प्रति (पूर्णाङ्क i = 0; i < m; i++) {
            प्रति (पूर्णाङ्क j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        प्रत्यागम arr;
    }

    स्थिर शून्य complex_div(द्विगुण xr, द्विगुण xi, द्विगुण yr, द्विगुण yi, द्विगुण& cdivr, द्विगुण& cdivi) {
        द्विगुण r, dv;
        CV_DbgAssert(std::निरपेक्षम्(yr) + std::निरपेक्षम्(yi) > 0.0);
        यदि (std::निरपेक्षम्(yr) > std::निरपेक्षम्(yi)) {
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
        ध्रुव पूर्णाङ्क max_iters_गणनम् = 1000 * एतत्->n;

        ध्रुव पूर्णाङ्क nn = एतत्->n; CV_Assert(nn > 0);
        पूर्णाङ्क n1 = nn - 1;
        ध्रुव पूर्णाङ्क नीचः = 0;
        ध्रुव पूर्णाङ्क उच्चः = nn - 1;
        ध्रुव द्विगुण eps = std::numeric_limits<द्विगुण>::epsilon();
        द्विगुण exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        द्विगुण norm = 0.0;
        प्रति (पूर्णाङ्क i = 0; i < nn; i++) {
#यदि 0 // 'if' condition is always false
            यदि (i < नीचः || i > उच्चः) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#यदि_अन्तः
            प्रति (पूर्णाङ्क j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::निरपेक्षम्(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        पूर्णाङ्क iter = 0;
        यावत् (n1 >= नीचः) {

            // Look for single small sub-diagonal element
            पूर्णाङ्क l = n1;
            यावत् (l > नीचः) {
                यदि (norm < FLT_EPSILON) {
                    भङ्ग;
                }
                द्विगुण s = std::निरपेक्षम्(H[l - 1][l - 1]) + std::निरपेक्षम्(H[l][l]);
                यदि (s == 0.0) {
                    s = norm;
                }
                यदि (std::निरपेक्षम्(H[l][l - 1]) < eps * s) {
                    भङ्ग;
                }
                l--;
            }

            // Check for convergence
            यदि (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } अन्यथा यदि (l == n1 - 1) {
                // Two roots found
                द्विगुण w = H[n1][n1 - 1] * H[n1 - 1][n1];
                द्विगुण p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                द्विगुण q = p * p + w;
                द्विगुण z = std::वर्गमूलम्(std::निरपेक्षम्(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                द्विगुण x = H[n1][n1];

                यदि (q >= 0) {
                    // Real pair
                    यदि (p >= 0) {
                        z = p + z;
                    } अन्यथा {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    यदि (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    द्विगुण s = std::निरपेक्षम्(x) + std::निरपेक्षम्(z);
                    p = x / s;
                    q = z / s;
                    द्विगुण r = std::वर्गमूलम्(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    प्रति (पूर्णाङ्क j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    प्रति (पूर्णाङ्क i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    प्रति (पूर्णाङ्क i = नीचः; i <= उच्चः; i++) {
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
                यदि (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                यदि (iter == 10) {
                    exshift += x;
                    प्रति (पूर्णाङ्क i = नीचः; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    द्विगुण s = std::निरपेक्षम्(H[n1][n1 - 1]) + std::निरपेक्षम्(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                यदि (iter == 30) {
                    द्विगुण s = (y - x) * 0.5;
                    s = s * s + w;
                    यदि (s > 0) {
                        s = std::वर्गमूलम्(s);
                        यदि (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        प्रति (पूर्णाङ्क i = नीचः; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                यदि (iter > max_iters_गणनम्)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                द्विगुण p = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण q = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण r = std::numeric_limits<द्विगुण>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                पूर्णाङ्क m = n1 - 2;
                यावत् (m >= l) {
                    द्विगुण z = H[m][m];
                    r = x - z;
                    द्विगुण s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::निरपेक्षम्(p) + std::निरपेक्षम्(q) + std::निरपेक्षम्(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    यदि (m == l) {
                        भङ्ग;
                    }
                    यदि (std::निरपेक्षम्(H[m][m - 1]) * (std::निरपेक्षम्(q) + std::निरपेक्षम्(r)) < eps * (std::निरपेक्षम्(p)
                                                                                     * (std::निरपेक्षम्(H[m - 1][m - 1]) + std::निरपेक्षम्(z) + std::निरपेक्षम्(
                                                                                                                                           H[m + 1][m + 1])))) {
                        भङ्ग;
                    }
                    m--;
                }

                प्रति (पूर्णाङ्क i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    यदि (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                प्रति (पूर्णाङ्क k = m; k < n1; k++) {

                    तार्किक notlast = (k != n1 - 1);
                    यदि (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::निरपेक्षम्(p) + std::निरपेक्षम्(q) + std::निरपेक्षम्(r);
                        यदि (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    यदि (x == 0.0) {
                        भङ्ग;
                    }
                    द्विगुण s = std::वर्गमूलम्(p * p + q * q + r * r);
                    यदि (p < 0) {
                        s = -s;
                    }
                    यदि (s != 0) {
                        यदि (k != m) {
                            H[k][k - 1] = -s * x;
                        } अन्यथा यदि (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        द्विगुण z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        प्रति (पूर्णाङ्क j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            यदि (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        प्रति (पूर्णाङ्क i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            यदि (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        प्रति (पूर्णाङ्क i = नीचः; i <= उच्चः; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            यदि (notlast) {
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

        यदि (norm < FLT_EPSILON) {
            प्रत्यागम;
        }

        प्रति (n1 = nn - 1; n1 >= 0; n1--) {
            द्विगुण p = d[n1];
            द्विगुण q = e[n1];

            यदि (q == 0) {
                // Real vector
                द्विगुण z = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण s = std::numeric_limits<द्विगुण>::quiet_NaN();

                पूर्णाङ्क l = n1;
                H[n1][n1] = 1.0;
                प्रति (पूर्णाङ्क i = n1 - 1; i >= 0; i--) {
                    द्विगुण w = H[i][i] - p;
                    द्विगुण r = 0.0;
                    प्रति (पूर्णाङ्क j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    यदि (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } अन्यथा {
                        l = i;
                        यदि (e[i] == 0.0) {
                            यदि (w != 0.0) {
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
                            यदि (std::निरपेक्षम्(x) > std::निरपेक्षम्(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } अन्यथा {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        द्विगुण t = std::निरपेक्षम्(H[i][n1]);
                        यदि ((eps * t) * t > 1) {
                            द्विगुण inv_t = 1.0 / t;
                            प्रति (पूर्णाङ्क j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } अन्यथा यदि (q < 0) {
                // Complex vector
                द्विगुण z = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण r = std::numeric_limits<द्विगुण>::quiet_NaN();
                द्विगुण s = std::numeric_limits<द्विगुण>::quiet_NaN();

                पूर्णाङ्क l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                यदि (std::निरपेक्षम्(H[n1][n1 - 1]) > std::निरपेक्षम्(H[n1 - 1][n1])) {
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
                प्रति (पूर्णाङ्क i = n1 - 2; i >= 0; i--) {
                    द्विगुण ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    प्रति (पूर्णाङ्क j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    द्विगुण w = H[i][i] - p;

                    यदि (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } अन्यथा {
                        l = i;
                        यदि (e[i] == 0) {
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
                            यदि (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::निरपेक्षम्(w) + std::निरपेक्षम्(q) + std::निरपेक्षम्(x)
                                                   + std::निरपेक्षम्(y) + std::निरपेक्षम्(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            यदि (std::निरपेक्षम्(x) > (std::निरपेक्षम्(z) + std::निरपेक्षम्(q))) {
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

                        द्विगुण t = std::max(std::निरपेक्षम्(H[i][n1 - 1]), std::निरपेक्षम्(H[i][n1]));
                        यदि ((eps * t) * t > 1) {
                            प्रति (पूर्णाङ्क j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#यदि 0 // 'if' condition is always false
        प्रति (पूर्णाङ्क i = 0; i < nn; i++) {
            यदि (i < नीचः || i > उच्चः) {
                प्रति (पूर्णाङ्क j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#यदि_अन्तः

        // Back transformation to get eigenvectors of original matrix

        प्रति (पूर्णाङ्क j = nn - 1; j >= नीचः; j--) {
            प्रति (पूर्णाङ्क i = नीचः; i <= उच्चः; i++) {
                द्विगुण z = 0.0;
                प्रति (पूर्णाङ्क k = नीचः; k <= std::min(j, उच्चः); k++) {
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
        पूर्णाङ्क नीचः = 0;
        पूर्णाङ्क उच्चः = n - 1;

        प्रति (पूर्णाङ्क m = नीचः + 1; m < उच्चः; m++) {

            // Scale column.

            द्विगुण scale = 0.0;
            प्रति (पूर्णाङ्क i = m; i <= उच्चः; i++) {
                scale = scale + std::निरपेक्षम्(H[i][m - 1]);
            }
            यदि (scale != 0.0) {

                // Compute Householder transformation.

                द्विगुण h = 0.0;
                प्रति (पूर्णाङ्क i = उच्चः; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                द्विगुण g = std::वर्गमूलम्(h);
                यदि (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                प्रति (पूर्णाङ्क j = m; j < n; j++) {
                    द्विगुण f = 0.0;
                    प्रति (पूर्णाङ्क i = उच्चः; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    प्रति (पूर्णाङ्क i = m; i <= उच्चः; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                प्रति (पूर्णाङ्क i = 0; i <= उच्चः; i++) {
                    द्विगुण f = 0.0;
                    प्रति (पूर्णाङ्क j = उच्चः; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    प्रति (पूर्णाङ्क j = m; j <= उच्चः; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            प्रति (पूर्णाङ्क j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        प्रति (पूर्णाङ्क m = उच्चः - 1; m > नीचः; m--) {
            यदि (H[m][m - 1] != 0.0) {
                प्रति (पूर्णाङ्क i = m + 1; i <= उच्चः; i++) {
                    ort[i] = H[i][m - 1];
                }
                प्रति (पूर्णाङ्क j = m; j <= उच्चः; j++) {
                    द्विगुण g = 0.0;
                    प्रति (पूर्णाङ्क i = m; i <= उच्चः; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    प्रति (पूर्णाङ्क i = m; i <= उच्चः; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    शून्य release() {
        // releases the working data
        विलोपय[] d; d = रिक्त;
        विलोपय[] e; e = रिक्त;
        विलोपय[] ort; ort = रिक्त;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            यदि (H) विलोपय[] H[i];
            यदि (V) विलोपय[] V[i];
        }
        विलोपय[] H; H = रिक्त;
        विलोपय[] V; V = रिक्त;
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
            प्रति (पूर्णाङ्क i = 0; i < n; i++) {
                _eigenvalues.at<द्विगुण> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            प्रति (पूर्णाङ्क i = 0; i < n; i++)
                प्रति (पूर्णाङ्क j = 0; j < n; j++)
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
    शून्य compute(निवेशः_Array src, तार्किक fallback_सममितम् = सत्य)
    {
        CV_INSTRUMENT_REGION();

        यदि(fallback_सममितम् && is_सममितम्_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } अन्यथा {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_गन्तव्यम्(tmp, CV_64FC1);
            // Get dimension of the matrix.
            एतत्->n = tmp.स्तम्भसङ्ख्या;
            // Allocate the matrix data to work on.
            एतत्->H = alloc_2d<द्विगुण> (n, n);
            // Now safely copy the data.
            प्रति (पूर्णाङ्क i = 0; i < tmp.पङ्क्तिकाः; i++) {
                प्रति (पूर्णाङ्क j = 0; j < tmp.स्तम्भसङ्ख्या; j++) {
                    एतत्->H[i][j] = tmp.at<द्विगुण>(i, j);
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
    Mat eigenvalues() ध्रुव { प्रत्यागम _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() ध्रुव { प्रत्यागम _eigenvectors; }
};

शून्य eigen_Non_सममितम्(निवेशः_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णाङ्क type = src.type();
    पूर्णाङ्क n = src.पङ्क्तिकाः;

    CV_Assert(src.पङ्क्तिकाः == src.स्तम्भसङ्ख्या);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    यदि (type == CV_32F)
        src.convert_गन्तव्यम्(src64f, CV_32FC1);
    अन्यथा
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, असत्य);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<द्विगुण> eigenvalues64f;
    eigensystem.eigenvalues().copy_गन्तव्यम्(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<पूर्णाङ्क> क्रमय_indexes(n);
    cv::क्रमय_Idx(eigenvalues64f, क्रमय_indexes, क्रमय_EVERY_ROW | क्रमय_DESCENDING);

    std::vector<द्विगुण> sorted_eigenvalues64f(n);
    प्रति (पूर्णाङ्क i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[क्रमय_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_गन्तव्यम्(_evals, type);

    यदि( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.पङ्क्तिकाः == n);
        CV_Assert(eigenvectors64f.स्तम्भसङ्ख्या == n);
        Mat_<द्विगुण> sorted_eigenvectors64f(n, n, CV_64FC1);
        प्रति (पूर्णाङ्क i = 0; i < n; i++)
        {
            द्विगुण* pDst = sorted_eigenvectors64f.ptr<द्विगुण>(i);
            द्विगुण* pSrc = eigenvectors64f.ptr<द्विगुण>(क्रमय_indexes[i]);
            CV_Assert(pSrc != रिक्त);
            स्मृतिं_प्रतिलिख(pDst, pSrc, n * परिमाण(द्विगुण));
        }
        sorted_eigenvectors64f.convert_गन्तव्यम्(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(पूर्णाङ्क अङ्कः_components) : अङ्कः_components_2(अङ्कः_components) { }

LDA::LDA(निवेशः_Array_सम्बन्धि_Arrays src, निवेशः_Array labels, पूर्णाङ्क अङ्कः_components) : अङ्कः_components_2(अङ्कः_components)
{
    एतत्->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

शून्य LDA::save(ध्रुव String& filename) ध्रुव
{
    FileStorage fs(filename, FileStorage::WRITE);
    यदि (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    एतत्->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
शून्य LDA::load(ध्रुव String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    यदि (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    एतत्->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
शून्य LDA::save(FileStorage& fs) ध्रुव {
    // write matrices
    fs << "num_components" << अङ्कः_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
शून्य LDA::load(ध्रुव FileStorage& fs) {
    //read matrices
    fs["num_components"] >> अङ्कः_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

शून्य LDA::lda(निवेशः_Array_सम्बन्धि_Arrays _src, निवेशः_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<पूर्णाङ्क> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        प्रति(अचिह्नित पूर्णाङ्क i = 0; i < tmp.योग(); i++) {
            labels.push_back(tmp.at<पूर्णाङ्क>(i));
        }
    }
    // turn into row sampled matrix
    Mat दत्त;
    // ensure working matrix is double precision
    src.convert_गन्तव्यम्(दत्त, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<पूर्णाङ्क> mapped_labels(labels.size());
    std::vector<पूर्णाङ्क> num2label = remove_dups(labels);
    std::map<पूर्णाङ्क, पूर्णाङ्क> label2num;
    प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)num2label.size(); i++)
        label2num[num2label[i]] = i;
    प्रति (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    पूर्णाङ्क N = दत्त.पङ्क्तिकाः;
    पूर्णाङ्क D = दत्त.स्तम्भसङ्ख्या;
    // number of unique labels
    पूर्णाङ्क C = (पूर्णाङ्क)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    यदि(C == 1) {
        String error_सन्देशः = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // throw error if less labels, than samples
    यदि (labels.size() != static_cast<size_t>(N)) {
        String error_सन्देशः = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_सन्देशः);
    }
    // warn if within-classes scatter matrix becomes singular
    यदि (N < D) {
        std::निर्गमः << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::पङ्क्त्यन्तः;
    }
    // clip number of components to be a valid number
    यदि ((अङ्कः_components_2 <= 0) || (अङ्कः_components_2 >= C)) {
        अङ्कः_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat माध्यम्_योग = Mat::zeros(1, D, दत्त.type());
    // holds the mean for each class
    std::vector<Mat> माध्यम्_Class(C);
    std::vector<पूर्णाङ्क> अङ्कः_Class(C);
    // initialize
    प्रति (पूर्णाङ्क i = 0; i < C; i++) {
        अङ्कः_Class[i] = 0;
        माध्यम्_Class[i] = Mat::zeros(1, D, दत्त.type()); //! Dx1 image vector
    }
    // calculate sums
    प्रति (पूर्णाङ्क i = 0; i < N; i++) {
        Mat instance = दत्त.row(i);
        पूर्णाङ्क classIdx = mapped_labels[i];
        सङ्कलय(माध्यम्_योग, instance, माध्यम्_योग);
        सङ्कलय(माध्यम्_Class[classIdx], instance, माध्यम्_Class[classIdx]);
        अङ्कः_Class[classIdx]++;
    }
    // calculate total mean
    माध्यम्_योग.convert_गन्तव्यम्(माध्यम्_योग, माध्यम्_योग.type(), 1.0 / static_cast<द्विगुण> (N));
    // calculate class means
    प्रति (पूर्णाङ्क i = 0; i < C; i++) {
        माध्यम्_Class[i].convert_गन्तव्यम्(माध्यम्_Class[i], माध्यम्_Class[i].type(), 1.0 / static_cast<द्विगुण> (अङ्कः_Class[i]));
    }
    // subtract class means
    प्रति (पूर्णाङ्क i = 0; i < N; i++) {
        पूर्णाङ्क classIdx = mapped_labels[i];
        Mat instance = दत्त.row(i);
        व्यवकलय(instance, माध्यम्_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, दत्त.type());
    mulTransposed(दत्त, Sw, सत्य);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, दत्त.type());
    प्रति (पूर्णाङ्क i = 0; i < C; i++) {
        Mat tmp;
        व्यवकलय(माध्यम्_Class[i], माध्यम्_योग, tmp);
        mulTransposed(tmp, tmp, सत्य);
        सङ्कलय(Sb, tmp, Sb);
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
    std::vector<पूर्णाङ्क> sorted_indices = argsort(_eigenvalues, असत्य);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = क्रमय_Matrix_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = क्रमय_Matrix_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, अङ्कः_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, अङ्कः_components_2));
}

शून्य LDA::compute(निवेशः_Array_सम्बन्धि_Arrays _src, निवेशः_Array _lbls) {
    विकल्प(_src.विधा()) {
    पक्ष निवेशः_Array_2::STD_सदिशम्_MAT:
    पक्ष निवेशः_Array_2::STD_ARRAY_MAT:
        lda(asRowMatrix(_src, CV_64FC1), _lbls);
        भङ्ग;
    पक्ष निवेशः_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        भङ्ग;
    स्वभाव:
        String error_सन्देशः= format("InputArray Datatype %d is not supported.", _src.विधा());
        CV_Error(Error::StsBadArg, error_सन्देशः);
        भङ्ग;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(निवेशः_Array src) {
   प्रत्यागम subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(निवेशः_Array src) {
   प्रत्यागम subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
