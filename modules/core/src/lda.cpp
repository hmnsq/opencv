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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <iostream>
#అన్తర్భావ <map>
#అన్తర్భావ <set>

నామాకాశ cv
{

// Removes duplicate elements in a given vector.
మూస<రకం_పేరు _Tp>
అంతఃవరుస std::vector<_Tp> remove_dups(స్థిరరాశి std::vector<_Tp>& src) {
    రకనిర్వచనం రకం_పేరు std::సమితి<_Tp>::const_iterator const_సమితి_Iterator;
    రకనిర్వచనం రకం_పేరు std::vector<_Tp>::const_iterator constVecIterator;
    std::సమితి<_Tp> సమితి_elems;
    కొరకు (constVecIterator it = src.begin(); it != src.end(); ++it)
        సమితి_elems.insert(*it);
    std::vector<_Tp> elems;
    కొరకు (const_సమితి_Iterator it = సమితి_elems.begin(); it != సమితి_elems.end(); ++it)
        elems.push_back(*it);
    తిరిగి elems;
}

స్థిర Mat argsort(ఆదానం_Array _src, తార్కిక ascending=నిజం)
{
    Mat src = _src.getMat();
    ఒకవేళ (src.అడ్డువరుసలు != 1 && src.నిలువుల_సంఖ్య != 1) {
        String error_సందేశం = "Wrong shape of input matrix! Expected a matrix with one row or column.";
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    పూర్ణాంకం flags = క్రమీకరించు_EVERY_అడ్డువరుస | (ascending ? క్రమీకరించు_ASCENDING : క్రమీకరించు_DESCENDING);
    Mat sorted_indices;
    క్రమీకరించు_Idx(src.reshape(1,1),sorted_indices,flags);
    తిరిగి sorted_indices;
}

స్థిర Mat as_అడ్డువరుస_మాతృక(ఆదానం_Array_యొక్క_Arrays src, పూర్ణాంకం rtype, ద్విగుణ alpha=1, ద్విగుణ beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    ఒకవేళ(src.రకం() != ఆదానం_Array_2::STD_సదిశ_MAT && src.రకం() != ఆదానం_Array_2::STD_ARRAY_MAT &&
        src.రకం() != ఆదానం_Array_2::STD_సదిశ_సదిశ) {
        String error_సందేశం = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // number of samples
    size_t n = src.మొత్తం();
    // return empty matrix if no matrices given
    ఒకవేళ(n == 0)
        తిరిగి Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).మొత్తం();
    // create data matrix
    Mat దత్తాంశం((పూర్ణాంకం)n, (పూర్ణాంకం)d, rtype);
    // now copy data
    కొరకు(పూర్ణాంకం i = 0; i < (పూర్ణాంకం)n; i++) {
        // make sure data can be reshaped, throw exception if not!
        ఒకవేళ(src.getMat(i).మొత్తం() != d) {
            String error_సందేశం = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, (పూర్ణాంకం)d, (పూర్ణాంకం)src.getMat(i).మొత్తం());
            CV_Error(Error::StsBadArg, error_సందేశం);
        }
        // get a hold of the current row
        Mat xi = దత్తాంశం.అడ్డువరుస(i);
        // make reshape happy by cloning for non-continuous matrices
        ఒకవేళ(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convert_గమ్యం(xi, rtype, alpha, beta);
        } లేకపోతే {
            src.getMat(i).clone().reshape(1, 1).convert_గమ్యం(xi, rtype, alpha, beta);
        }
    }
    తిరిగి దత్తాంశం;
}

స్థిర శూన్య క్రమీకరించు_మాతృక_Columns_By_Indices(ఆదానం_Array _src, ఆదానం_Array _indices, OutputArray _dst) {
    ఒకవేళ(_indices.getMat().type() != CV_32SC1) {
        CV_Error(Error::StsUnsupportedFormat, "cv::sortColumnsByIndices only works on integer indices!");
    }
    Mat src = _src.getMat();
    std::vector<పూర్ణాంకం> indices = _indices.getMat();
    _dst.create(src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, src.type());
    Mat dst = _dst.getMat();
    కొరకు(size_t idx = 0; idx < indices.size(); idx++) {
        Mat original_నిలువు = src.నిలువు(indices[idx]);
        Mat sorted_నిలువు = dst.నిలువు((పూర్ణాంకం)idx);
        original_నిలువు.copy_గమ్యం(sorted_నిలువు);
    }
}

స్థిర Mat క్రమీకరించు_మాతృక_Columns_By_Indices(ఆదానం_Array src, ఆదానం_Array indices) {
    Mat dst;
    క్రమీకరించు_మాతృక_Columns_By_Indices(src, indices, dst);
    తిరిగి dst;
}


మూస<రకం_పేరు _Tp> స్థిర తార్కిక
is_సౌష్ఠవ(ఆదానం_Array src) {
    Mat _src = src.getMat();
    ఒకవేళ(_src.నిలువుల_సంఖ్య != _src.అడ్డువరుసలు)
        తిరిగి అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < _src.అడ్డువరుసలు; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < _src.నిలువుల_సంఖ్య; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ఒకవేళ (a != b) {
                తిరిగి అబద్ధం;
            }
        }
    }
    తిరిగి నిజం;
}

మూస<రకం_పేరు _Tp> స్థిర తార్కిక
is_సౌష్ఠవ(ఆదానం_Array src, ద్విగుణ eps) {
    Mat _src = src.getMat();
    ఒకవేళ(_src.నిలువుల_సంఖ్య != _src.అడ్డువరుసలు)
        తిరిగి అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < _src.అడ్డువరుసలు; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < _src.నిలువుల_సంఖ్య; j++) {
            _Tp a = _src.at<_Tp> (i, j);
            _Tp b = _src.at<_Tp> (j, i);
            ఒకవేళ (std::నిరపేక్ష(a - b) > eps) {
                తిరిగి అబద్ధం;
            }
        }
    }
    తిరిగి నిజం;
}

స్థిర తార్కిక is_సౌష్ఠవ_2(ఆదానం_Array src, ద్విగుణ eps=1e-16)
{
    Mat m = src.getMat();
    ఎంపిక (m.type()) {
        స్థితి CV_8SC1: తిరిగి is_సౌష్ఠవ<అక్షరం>(m); ఆపు;
        స్థితి CV_8UC1:
            తిరిగి is_సౌష్ఠవ<గుర్తులేని అక్షరం>(m); ఆపు;
        స్థితి CV_16SC1:
            తిరిగి is_సౌష్ఠవ<పొట్టి>(m); ఆపు;
        స్థితి CV_16UC1:
            తిరిగి is_సౌష్ఠవ<గుర్తులేని పొట్టి>(m); ఆపు;
        స్థితి CV_32SC1:
            తిరిగి is_సౌష్ఠవ<పూర్ణాంకం>(m); ఆపు;
        స్థితి CV_32FC1:
            తిరిగి is_సౌష్ఠవ<దశాంశం>(m, eps); ఆపు;
        స్థితి CV_64FC1:
            తిరిగి is_సౌష్ఠవ<ద్విగుణ>(m, eps); ఆపు;
        అప్రమేయ:
            ఆపు;
    }
    తిరిగి అబద్ధం;
}


//------------------------------------------------------------------------------
// cv::subspaceProject
//------------------------------------------------------------------------------
Mat LDA::subspaceProject(ఆదానం_Array _W, ఆదానం_Array సగటు, ఆదానం_Array _src) {
    // get data matrices
    Mat W = _W.getMat();
    Mat సగటు_2 = సగటు.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    పూర్ణాంకం n = src.అడ్డువరుసలు;
    పూర్ణాంకం d = src.నిలువుల_సంఖ్య;
    // make sure the data has the correct shape
    ఒకవేళ(W.అడ్డువరుసలు != d) {
        String error_సందేశం = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, W.అడ్డువరుసలు, W.నిలువుల_సంఖ్య);
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // make sure mean is correct if not empty
    ఒకవేళ(!సగటు_2.empty() && (సగటు_2.మొత్తం() != (size_t) d)) {
        String error_సందేశం = format("Wrong mean shape for the given data matrix. Expected %d, but was %zu.", d, సగటు_2.మొత్తం());
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // create temporary matrices
    Mat X, Y;
    // make sure you operate on correct type
    src.convert_గమ్యం(X, W.type());
    // safe to do, because of above assertion
    ఒకవేళ(!సగటు_2.empty()) {
        కొరకు(పూర్ణాంకం i=0; i<n; i++) {
            Mat r_i = X.అడ్డువరుస(i);
            వ్యవకలనం(r_i, సగటు_2.reshape(1,1), r_i);
        }
    }
    // finally calculate projection as Y = (X-mean)*W
    gemm(X, W, 1.0, Mat(), 0.0, Y);
    తిరిగి Y;
}

//------------------------------------------------------------------------------
// cv::subspaceReconstruct
//------------------------------------------------------------------------------
Mat LDA::subspaceReconstruct(ఆదానం_Array _W, ఆదానం_Array సగటు, ఆదానం_Array _src)
{
    // get data matrices
    Mat W = _W.getMat();
    Mat సగటు_2 = సగటు.getMat();
    Mat src = _src.getMat();
    // get number of samples and dimension
    పూర్ణాంకం n = src.అడ్డువరుసలు;
    పూర్ణాంకం d = src.నిలువుల_సంఖ్య;
    // make sure the data has the correct shape
    ఒకవేళ(W.నిలువుల_సంఖ్య != d) {
        String error_సందేశం = format("Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).", src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, W.అడ్డువరుసలు, W.నిలువుల_సంఖ్య);
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // make sure mean is correct if not empty
    ఒకవేళ(!సగటు_2.empty() && (సగటు_2.మొత్తం() != (size_t) W.అడ్డువరుసలు)) {
        String error_సందేశం = format("Wrong mean shape for the given eigenvector matrix. Expected %d, but was %zu.", W.నిలువుల_సంఖ్య, సగటు_2.మొత్తం());
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // initialize temporary matrices
    Mat X, Y;
    // copy data & make sure we are using the correct type
    src.convert_గమ్యం(Y, W.type());
    // calculate the reconstruction
    gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
    // safe to do because of above assertion
    ఒకవేళ(!సగటు_2.empty()) {
        కొరకు(పూర్ణాంకం i=0; i<n; i++) {
            Mat r_i = X.అడ్డువరుస(i);
            కూడు(r_i, సగటు_2.reshape(1,1), r_i);
        }
    }
    తిరిగి X;
}


తరగతి EigenvalueDecomposition {
అంతరంగ:

    // Holds the data dimension.
    పూర్ణాంకం n;

    // Pointer to internal memory.
    ద్విగుణ *d, *e, *ort;
    ద్విగుణ **V, **H;

    // Holds the computed eigenvalues.
    Mat _eigenvalues;

    // Holds the computed eigenvectors.
    Mat _eigenvectors;

    // Allocates memory.
    మూస<రకం_పేరు _Tp>
    _Tp *alloc_1d(పూర్ణాంకం m) {
        తిరిగి కొత్త _Tp[m];
    }

    // Allocates memory.
    మూస<రకం_పేరు _Tp>
    _Tp *alloc_1d(పూర్ణాంకం m, _Tp val) {
        _Tp *arr = alloc_1d<_Tp> (m);
        కొరకు (పూర్ణాంకం i = 0; i < m; i++)
            arr[i] = val;
        తిరిగి arr;
    }

    // Allocates memory.
    మూస<రకం_పేరు _Tp>
    _Tp **alloc_2d(పూర్ణాంకం m, పూర్ణాంకం _n) {
        _Tp **arr = కొత్త _Tp*[m];
        కొరకు (పూర్ణాంకం i = 0; i < m; i++)
            arr[i] = కొత్త _Tp[_n];
        తిరిగి arr;
    }

    // Allocates memory.
    మూస<రకం_పేరు _Tp>
    _Tp **alloc_2d(పూర్ణాంకం m, పూర్ణాంకం _n, _Tp val) {
        _Tp **arr = alloc_2d<_Tp> (m, _n);
        కొరకు (పూర్ణాంకం i = 0; i < m; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < _n; j++) {
                arr[i][j] = val;
            }
        }
        తిరిగి arr;
    }

    స్థిర శూన్య complex_div(ద్విగుణ xr, ద్విగుణ xi, ద్విగుణ yr, ద్విగుణ yi, ద్విగుణ& cdivr, ద్విగుణ& cdivi) {
        ద్విగుణ r, dv;
        CV_DbgAssert(std::నిరపేక్ష(yr) + std::నిరపేక్ష(yi) > 0.0);
        ఒకవేళ (std::నిరపేక్ష(yr) > std::నిరపేక్ష(yi)) {
            r = yi / yr;
            dv = yr + r * yi;
            cdivr = (xr + r * xi) / dv;
            cdivi = (xi - r * xr) / dv;
        } లేకపోతే {
            r = yr / yi;
            dv = yi + r * yr;
            cdivr = (r * xr + xi) / dv;
            cdivi = (r * xi - xr) / dv;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.

    శూన్య hqr2() {

        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize
        స్థిరరాశి పూర్ణాంకం max_iters_లెక్క = 1000 * ఇది->n;

        స్థిరరాశి పూర్ణాంకం nn = ఇది->n; CV_Assert(nn > 0);
        పూర్ణాంకం n1 = nn - 1;
        స్థిరరాశి పూర్ణాంకం దిగువ = 0;
        స్థిరరాశి పూర్ణాంకం ఎగువ = nn - 1;
        స్థిరరాశి ద్విగుణ eps = std::numeric_limits<ద్విగుణ>::epsilon();
        ద్విగుణ exshift = 0.0;

        // Store roots isolated by balanc and compute matrix norm

        ద్విగుణ norm = 0.0;
        కొరకు (పూర్ణాంకం i = 0; i < nn; i++) {
#ఒకవేళ 0 // 'if' condition is always false
            ఒకవేళ (i < దిగువ || i > ఎగువ) {
                d[i] = H[i][i];
                e[i] = 0.0;
            }
#అయితే_ముగింపు
            కొరకు (పూర్ణాంకం j = std::max(i - 1, 0); j < nn; j++) {
                norm += std::నిరపేక్ష(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index
        పూర్ణాంకం iter = 0;
        అయితే (n1 >= దిగువ) {

            // Look for single small sub-diagonal element
            పూర్ణాంకం l = n1;
            అయితే (l > దిగువ) {
                ఒకవేళ (norm < FLT_EPSILON) {
                    ఆపు;
                }
                ద్విగుణ s = std::నిరపేక్ష(H[l - 1][l - 1]) + std::నిరపేక్ష(H[l][l]);
                ఒకవేళ (s == 0.0) {
                    s = norm;
                }
                ఒకవేళ (std::నిరపేక్ష(H[l][l - 1]) < eps * s) {
                    ఆపు;
                }
                l--;
            }

            // Check for convergence
            ఒకవేళ (l == n1) {
                // One root found
                H[n1][n1] = H[n1][n1] + exshift;
                d[n1] = H[n1][n1];
                e[n1] = 0.0;
                n1--;
                iter = 0;

            } లేకపోతే ఒకవేళ (l == n1 - 1) {
                // Two roots found
                ద్విగుణ w = H[n1][n1 - 1] * H[n1 - 1][n1];
                ద్విగుణ p = (H[n1 - 1][n1 - 1] - H[n1][n1]) * 0.5;
                ద్విగుణ q = p * p + w;
                ద్విగుణ z = std::వర్గమూలం(std::నిరపేక్ష(q));
                H[n1][n1] = H[n1][n1] + exshift;
                H[n1 - 1][n1 - 1] = H[n1 - 1][n1 - 1] + exshift;
                ద్విగుణ x = H[n1][n1];

                ఒకవేళ (q >= 0) {
                    // Real pair
                    ఒకవేళ (p >= 0) {
                        z = p + z;
                    } లేకపోతే {
                        z = p - z;
                    }
                    d[n1 - 1] = x + z;
                    d[n1] = d[n1 - 1];
                    ఒకవేళ (z != 0.0) {
                        d[n1] = x - w / z;
                    }
                    e[n1 - 1] = 0.0;
                    e[n1] = 0.0;
                    x = H[n1][n1 - 1];
                    ద్విగుణ s = std::నిరపేక్ష(x) + std::నిరపేక్ష(z);
                    p = x / s;
                    q = z / s;
                    ద్విగుణ r = std::వర్గమూలం(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    కొరకు (పూర్ణాంకం j = n1 - 1; j < nn; j++) {
                        z = H[n1 - 1][j];
                        H[n1 - 1][j] = q * z + p * H[n1][j];
                        H[n1][j] = q * H[n1][j] - p * z;
                    }

                    // Column modification

                    కొరకు (పూర్ణాంకం i = 0; i <= n1; i++) {
                        z = H[i][n1 - 1];
                        H[i][n1 - 1] = q * z + p * H[i][n1];
                        H[i][n1] = q * H[i][n1] - p * z;
                    }

                    // Accumulate transformations

                    కొరకు (పూర్ణాంకం i = దిగువ; i <= ఎగువ; i++) {
                        z = V[i][n1 - 1];
                        V[i][n1 - 1] = q * z + p * V[i][n1];
                        V[i][n1] = q * V[i][n1] - p * z;
                    }

                } లేకపోతే {
                    // Complex pair
                    d[n1 - 1] = x + p;
                    d[n1] = x + p;
                    e[n1 - 1] = z;
                    e[n1] = -z;
                }
                n1 = n1 - 2;
                iter = 0;

            } లేకపోతే {
                // No convergence yet

                // Form shift
                ద్విగుణ x = H[n1][n1];
                ద్విగుణ y = 0.0;
                ద్విగుణ w = 0.0;
                ఒకవేళ (l < n1) {
                    y = H[n1 - 1][n1 - 1];
                    w = H[n1][n1 - 1] * H[n1 - 1][n1];
                }

                // Wilkinson's original ad hoc shift
                ఒకవేళ (iter == 10) {
                    exshift += x;
                    కొరకు (పూర్ణాంకం i = దిగువ; i <= n1; i++) {
                        H[i][i] -= x;
                    }
                    ద్విగుణ s = std::నిరపేక్ష(H[n1][n1 - 1]) + std::నిరపేక్ష(H[n1 - 1][n1 - 2]);
                    x = y = 0.75 * s;
                    w = -0.4375 * s * s;
                }

                // MATLAB's new ad hoc shift

                ఒకవేళ (iter == 30) {
                    ద్విగుణ s = (y - x) * 0.5;
                    s = s * s + w;
                    ఒకవేళ (s > 0) {
                        s = std::వర్గమూలం(s);
                        ఒకవేళ (y < x) {
                            s = -s;
                        }
                        s = x - w / ((y - x) * 0.5 + s);
                        కొరకు (పూర్ణాంకం i = దిగువ; i <= n1; i++) {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }

                iter = iter + 1;
                ఒకవేళ (iter > max_iters_లెక్క)
                    CV_Error(Error::StsNoConv, "Algorithm doesn't converge (complex eigen values?)");

                ద్విగుణ p = std::numeric_limits<ద్విగుణ>::quiet_NaN();
                ద్విగుణ q = std::numeric_limits<ద్విగుణ>::quiet_NaN();
                ద్విగుణ r = std::numeric_limits<ద్విగుణ>::quiet_NaN();

                // Look for two consecutive small sub-diagonal elements
                పూర్ణాంకం m = n1 - 2;
                అయితే (m >= l) {
                    ద్విగుణ z = H[m][m];
                    r = x - z;
                    ద్విగుణ s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = std::నిరపేక్ష(p) + std::నిరపేక్ష(q) + std::నిరపేక్ష(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    ఒకవేళ (m == l) {
                        ఆపు;
                    }
                    ఒకవేళ (std::నిరపేక్ష(H[m][m - 1]) * (std::నిరపేక్ష(q) + std::నిరపేక్ష(r)) < eps * (std::నిరపేక్ష(p)
                                                                                     * (std::నిరపేక్ష(H[m - 1][m - 1]) + std::నిరపేక్ష(z) + std::నిరపేక్ష(
                                                                                                                                           H[m + 1][m + 1])))) {
                        ఆపు;
                    }
                    m--;
                }

                కొరకు (పూర్ణాంకం i = m + 2; i <= n1; i++) {
                    H[i][i - 2] = 0.0;
                    ఒకవేళ (i > m + 2) {
                        H[i][i - 3] = 0.0;
                    }
                }

                // Double QR step involving rows l:n and columns m:n

                కొరకు (పూర్ణాంకం k = m; k < n1; k++) {

                    తార్కిక notlast = (k != n1 - 1);
                    ఒకవేళ (k != m) {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : 0.0);
                        x = std::నిరపేక్ష(p) + std::నిరపేక్ష(q) + std::నిరపేక్ష(r);
                        ఒకవేళ (x != 0.0) {
                            p = p / x;
                            q = q / x;
                            r = r / x;
                        }
                    }
                    ఒకవేళ (x == 0.0) {
                        ఆపు;
                    }
                    ద్విగుణ s = std::వర్గమూలం(p * p + q * q + r * r);
                    ఒకవేళ (p < 0) {
                        s = -s;
                    }
                    ఒకవేళ (s != 0) {
                        ఒకవేళ (k != m) {
                            H[k][k - 1] = -s * x;
                        } లేకపోతే ఒకవేళ (l != m) {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        ద్విగుణ z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        కొరకు (పూర్ణాంకం j = k; j < nn; j++) {
                            p = H[k][j] + q * H[k + 1][j];
                            ఒకవేళ (notlast) {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] -= p * x;
                            H[k + 1][j] -= p * y;
                        }

                        // Column modification

                        కొరకు (పూర్ణాంకం i = 0; i <= std::min(n1, k + 3); i++) {
                            p = x * H[i][k] + y * H[i][k + 1];
                            ఒకవేళ (notlast) {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] -= p;
                            H[i][k + 1] -= p * q;
                        }

                        // Accumulate transformations

                        కొరకు (పూర్ణాంకం i = దిగువ; i <= ఎగువ; i++) {
                            p = x * V[i][k] + y * V[i][k + 1];
                            ఒకవేళ (notlast) {
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

        ఒకవేళ (norm < FLT_EPSILON) {
            తిరిగి;
        }

        కొరకు (n1 = nn - 1; n1 >= 0; n1--) {
            ద్విగుణ p = d[n1];
            ద్విగుణ q = e[n1];

            ఒకవేళ (q == 0) {
                // Real vector
                ద్విగుణ z = std::numeric_limits<ద్విగుణ>::quiet_NaN();
                ద్విగుణ s = std::numeric_limits<ద్విగుణ>::quiet_NaN();

                పూర్ణాంకం l = n1;
                H[n1][n1] = 1.0;
                కొరకు (పూర్ణాంకం i = n1 - 1; i >= 0; i--) {
                    ద్విగుణ w = H[i][i] - p;
                    ద్విగుణ r = 0.0;
                    కొరకు (పూర్ణాంకం j = l; j <= n1; j++) {
                        r = r + H[i][j] * H[j][n1];
                    }
                    ఒకవేళ (e[i] < 0.0) {
                        z = w;
                        s = r;
                    } లేకపోతే {
                        l = i;
                        ఒకవేళ (e[i] == 0.0) {
                            ఒకవేళ (w != 0.0) {
                                H[i][n1] = -r / w;
                            } లేకపోతే {
                                H[i][n1] = -r / (eps * norm);
                            }
                        } లేకపోతే {
                            // Solve real equations
                            CV_DbgAssert(!cvIsNaN(z));
                            ద్విగుణ x = H[i][i + 1];
                            ద్విగుణ y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            ద్విగుణ t = (x * s - z * r) / q;
                            H[i][n1] = t;
                            ఒకవేళ (std::నిరపేక్ష(x) > std::నిరపేక్ష(z)) {
                                H[i + 1][n1] = (-r - w * t) / x;
                            } లేకపోతే {
                                CV_DbgAssert(z != 0.0);
                                H[i + 1][n1] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control
                        ద్విగుణ t = std::నిరపేక్ష(H[i][n1]);
                        ఒకవేళ ((eps * t) * t > 1) {
                            ద్విగుణ inv_t = 1.0 / t;
                            కొరకు (పూర్ణాంకం j = i; j <= n1; j++) {
                                H[j][n1] *= inv_t;
                            }
                        }
                    }
                }
            } లేకపోతే ఒకవేళ (q < 0) {
                // Complex vector
                ద్విగుణ z = std::numeric_limits<ద్విగుణ>::quiet_NaN();
                ద్విగుణ r = std::numeric_limits<ద్విగుణ>::quiet_NaN();
                ద్విగుణ s = std::numeric_limits<ద్విగుణ>::quiet_NaN();

                పూర్ణాంకం l = n1 - 1;

                // Last vector component imaginary so matrix is triangular

                ఒకవేళ (std::నిరపేక్ష(H[n1][n1 - 1]) > std::నిరపేక్ష(H[n1 - 1][n1])) {
                    H[n1 - 1][n1 - 1] = q / H[n1][n1 - 1];
                    H[n1 - 1][n1] = -(H[n1][n1] - p) / H[n1][n1 - 1];
                } లేకపోతే {
                    complex_div(
                            0.0, -H[n1 - 1][n1],
                            H[n1 - 1][n1 - 1] - p, q,
                            H[n1 - 1][n1 - 1], H[n1 - 1][n1]
                    );
                }
                H[n1][n1 - 1] = 0.0;
                H[n1][n1] = 1.0;
                కొరకు (పూర్ణాంకం i = n1 - 2; i >= 0; i--) {
                    ద్విగుణ ra, sa, vr, vi;
                    ra = 0.0;
                    sa = 0.0;
                    కొరకు (పూర్ణాంకం j = l; j <= n1; j++) {
                        ra = ra + H[i][j] * H[j][n1 - 1];
                        sa = sa + H[i][j] * H[j][n1];
                    }
                    ద్విగుణ w = H[i][i] - p;

                    ఒకవేళ (e[i] < 0.0) {
                        z = w;
                        r = ra;
                        s = sa;
                    } లేకపోతే {
                        l = i;
                        ఒకవేళ (e[i] == 0) {
                            complex_div(
                                    -ra, -sa,
                                    w, q,
                                    H[i][n1 - 1], H[i][n1]
                            );
                        } లేకపోతే {
                            // Solve complex equations

                            ద్విగుణ x = H[i][i + 1];
                            ద్విగుణ y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * 2.0 * q;
                            ఒకవేళ (vr == 0.0 && vi == 0.0) {
                                vr = eps * norm * (std::నిరపేక్ష(w) + std::నిరపేక్ష(q) + std::నిరపేక్ష(x)
                                                   + std::నిరపేక్ష(y) + std::నిరపేక్ష(z));
                            }
                            complex_div(
                                    x * r - z * ra + q * sa, x * s - z * sa - q * ra,
                                    vr, vi,
                                    H[i][n1 - 1], H[i][n1]);
                            ఒకవేళ (std::నిరపేక్ష(x) > (std::నిరపేక్ష(z) + std::నిరపేక్ష(q))) {
                                H[i + 1][n1 - 1] = (-ra - w * H[i][n1 - 1] + q
                                                   * H[i][n1]) / x;
                                H[i + 1][n1] = (-sa - w * H[i][n1] - q * H[i][n1
                                                                            - 1]) / x;
                            } లేకపోతే {
                                complex_div(
                                        -r - y * H[i][n1 - 1], -s - y * H[i][n1],
                                        z, q,
                                        H[i + 1][n1 - 1], H[i + 1][n1]);
                            }
                        }

                        // Overflow control

                        ద్విగుణ t = std::max(std::నిరపేక్ష(H[i][n1 - 1]), std::నిరపేక్ష(H[i][n1]));
                        ఒకవేళ ((eps * t) * t > 1) {
                            కొరకు (పూర్ణాంకం j = i; j <= n1; j++) {
                                H[j][n1 - 1] = H[j][n1 - 1] / t;
                                H[j][n1] = H[j][n1] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

#ఒకవేళ 0 // 'if' condition is always false
        కొరకు (పూర్ణాంకం i = 0; i < nn; i++) {
            ఒకవేళ (i < దిగువ || i > ఎగువ) {
                కొరకు (పూర్ణాంకం j = i; j < nn; j++) {
                    V[i][j] = H[i][j];
                }
            }
        }
#అయితే_ముగింపు

        // Back transformation to get eigenvectors of original matrix

        కొరకు (పూర్ణాంకం j = nn - 1; j >= దిగువ; j--) {
            కొరకు (పూర్ణాంకం i = దిగువ; i <= ఎగువ; i++) {
                ద్విగుణ z = 0.0;
                కొరకు (పూర్ణాంకం k = దిగువ; k <= std::min(j, ఎగువ); k++) {
                    z += V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.
    శూన్య orthes() {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        పూర్ణాంకం దిగువ = 0;
        పూర్ణాంకం ఎగువ = n - 1;

        కొరకు (పూర్ణాంకం m = దిగువ + 1; m < ఎగువ; m++) {

            // Scale column.

            ద్విగుణ scale = 0.0;
            కొరకు (పూర్ణాంకం i = m; i <= ఎగువ; i++) {
                scale = scale + std::నిరపేక్ష(H[i][m - 1]);
            }
            ఒకవేళ (scale != 0.0) {

                // Compute Householder transformation.

                ద్విగుణ h = 0.0;
                కొరకు (పూర్ణాంకం i = ఎగువ; i >= m; i--) {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ద్విగుణ g = std::వర్గమూలం(h);
                ఒకవేళ (ort[m] > 0) {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                కొరకు (పూర్ణాంకం j = m; j < n; j++) {
                    ద్విగుణ f = 0.0;
                    కొరకు (పూర్ణాంకం i = ఎగువ; i >= m; i--) {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    కొరకు (పూర్ణాంకం i = m; i <= ఎగువ; i++) {
                        H[i][j] -= f * ort[i];
                    }
                }

                కొరకు (పూర్ణాంకం i = 0; i <= ఎగువ; i++) {
                    ద్విగుణ f = 0.0;
                    కొరకు (పూర్ణాంకం j = ఎగువ; j >= m; j--) {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    కొరకు (పూర్ణాంకం j = m; j <= ఎగువ; j++) {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < n; j++) {
                V[i][j] = (i == j ? 1.0 : 0.0);
            }
        }

        కొరకు (పూర్ణాంకం m = ఎగువ - 1; m > దిగువ; m--) {
            ఒకవేళ (H[m][m - 1] != 0.0) {
                కొరకు (పూర్ణాంకం i = m + 1; i <= ఎగువ; i++) {
                    ort[i] = H[i][m - 1];
                }
                కొరకు (పూర్ణాంకం j = m; j <= ఎగువ; j++) {
                    ద్విగుణ g = 0.0;
                    కొరకు (పూర్ణాంకం i = m; i <= ఎగువ; i++) {
                        g += ort[i] * V[i][j];
                    }
                    // Double division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    కొరకు (పూర్ణాంకం i = m; i <= ఎగువ; i++) {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }

    // Releases all internal working memory.
    శూన్య release() {
        // releases the working data
        తొలగించు[] d; d = ఖాళీ;
        తొలగించు[] e; e = ఖాళీ;
        తొలగించు[] ort; ort = ఖాళీ;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            ఒకవేళ (H) తొలగించు[] H[i];
            ఒకవేళ (V) తొలగించు[] V[i];
        }
        తొలగించు[] H; H = ఖాళీ;
        తొలగించు[] V; V = ఖాళీ;
    }

    // Computes the Eigenvalue Decomposition for a matrix given in H.
    శూన్య compute() {
        // Allocate memory for the working data.
        V = alloc_2d<ద్విగుణ> (n, n, 0.0);
        d = alloc_1d<ద్విగుణ> (n);
        e = alloc_1d<ద్విగుణ> (n);
        ort = alloc_1d<ద్విగుణ> (n);
        {
            // Reduce to Hessenberg form.
            orthes();
            // Reduce Hessenberg to real Schur form.
            hqr2();
            // Copy eigenvalues to OpenCV Matrix.
            _eigenvalues.create(1, n, CV_64FC1);
            కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
                _eigenvalues.at<ద్విగుణ> (0, i) = d[i];
            }
            // Copy eigenvectors to OpenCV Matrix.
            _eigenvectors.create(n, n, CV_64FC1);
            కొరకు (పూర్ణాంకం i = 0; i < n; i++)
                కొరకు (పూర్ణాంకం j = 0; j < n; j++)
                    _eigenvectors.at<ద్విగుణ> (i, j) = V[i][j];
            // Deallocate the memory by releasing all internal working data.
            release();
        }
    }

బహిరంగ:
    // Initializes & computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    EigenvalueDecomposition() :
        n(0),
        d(ఖాళీ), e(ఖాళీ), ort(ఖాళీ),
        V(ఖాళీ), H(ఖాళీ)
    {
        // nothing
    }

    // This function computes the Eigenvalue Decomposition for a general matrix
    // given in src. This function is a port of the EigenvalueSolver in JAMA,
    // which has been released to public domain by The MathWorks and the
    // National Institute of Standards and Technology (NIST).
    శూన్య compute(ఆదానం_Array src, తార్కిక fallback_సౌష్ఠవ = నిజం)
    {
        CV_INSTRUMENT_REGION();

        ఒకవేళ(fallback_సౌష్ఠవ && is_సౌష్ఠవ_2(src)) {
            // Fall back to OpenCV for a symmetric matrix!
            cv::eigen(src, _eigenvalues, _eigenvectors);
        } లేకపోతే {
            Mat tmp;
            // Convert the given input matrix to double. Is there any way to
            // prevent allocating the temporary memory? Only used for copying
            // into working memory and deallocated after.
            src.getMat().convert_గమ్యం(tmp, CV_64FC1);
            // Get dimension of the matrix.
            ఇది->n = tmp.నిలువుల_సంఖ్య;
            // Allocate the matrix data to work on.
            ఇది->H = alloc_2d<ద్విగుణ> (n, n);
            // Now safely copy the data.
            కొరకు (పూర్ణాంకం i = 0; i < tmp.అడ్డువరుసలు; i++) {
                కొరకు (పూర్ణాంకం j = 0; j < tmp.నిలువుల_సంఖ్య; j++) {
                    ఇది->H[i][j] = tmp.at<ద్విగుణ>(i, j);
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
    Mat eigenvalues() స్థిరరాశి { తిరిగి _eigenvalues; }
    // Returns the eigenvectors of the Eigenvalue Decomposition.
    Mat eigenvectors() స్థిరరాశి { తిరిగి _eigenvectors; }
};

శూన్య eigen_Non_సౌష్ఠవ(ఆదానం_Array _src, OutputArray _evals, OutputArray _evects)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    పూర్ణాంకం type = src.type();
    పూర్ణాంకం n = src.అడ్డువరుసలు;

    CV_Assert(src.అడ్డువరుసలు == src.నిలువుల_సంఖ్య);
    CV_Assert(type == CV_32F || type == CV_64F);

    Mat src64f;
    ఒకవేళ (type == CV_32F)
        src.convert_గమ్యం(src64f, CV_32FC1);
    లేకపోతే
        src64f = src;

    EigenvalueDecomposition eigensystem;
    eigensystem.compute(src64f, అబద్ధం);

    // EigenvalueDecomposition returns transposed and non-sorted eigenvalues
    std::vector<ద్విగుణ> eigenvalues64f;
    eigensystem.eigenvalues().copy_గమ్యం(eigenvalues64f);
    CV_Assert(eigenvalues64f.size() == (size_t)n);

    std::vector<పూర్ణాంకం> క్రమీకరించు_indexes(n);
    cv::క్రమీకరించు_Idx(eigenvalues64f, క్రమీకరించు_indexes, క్రమీకరించు_EVERY_అడ్డువరుస | క్రమీకరించు_DESCENDING);

    std::vector<ద్విగుణ> sorted_eigenvalues64f(n);
    కొరకు (పూర్ణాంకం i = 0; i < n; i++) sorted_eigenvalues64f[i] = eigenvalues64f[క్రమీకరించు_indexes[i]];

    Mat(n, 1, CV_64F, &sorted_eigenvalues64f[0]).convert_గమ్యం(_evals, type);

    ఒకవేళ( _evects.needed() )
    {
        Mat eigenvectors64f = eigensystem.eigenvectors().t(); // transpose
        CV_Assert(eigenvectors64f.అడ్డువరుసలు == n);
        CV_Assert(eigenvectors64f.నిలువుల_సంఖ్య == n);
        Mat_<ద్విగుణ> sorted_eigenvectors64f(n, n, CV_64FC1);
        కొరకు (పూర్ణాంకం i = 0; i < n; i++)
        {
            ద్విగుణ* pDst = sorted_eigenvectors64f.ptr<ద్విగుణ>(i);
            ద్విగుణ* pSrc = eigenvectors64f.ptr<ద్విగుణ>(క్రమీకరించు_indexes[i]);
            CV_Assert(pSrc != ఖాళీ);
            జ్ఞాపకం_నకలు(pDst, pSrc, n * పరిమాణం(ద్విగుణ));
        }
        sorted_eigenvectors64f.convert_గమ్యం(_evects, type);
    }
}


//------------------------------------------------------------------------------
// Linear Discriminant Analysis implementation
//------------------------------------------------------------------------------

LDA::LDA(పూర్ణాంకం అంకె_components) : అంకె_components_2(అంకె_components) { }

LDA::LDA(ఆదానం_Array_యొక్క_Arrays src, ఆదానం_Array labels, పూర్ణాంకం అంకె_components) : అంకె_components_2(అంకె_components)
{
    ఇది->compute(src, labels); //! compute eigenvectors and eigenvalues
}

LDA::~LDA() {}

శూన్య LDA::save(స్థిరరాశి String& filename) స్థిరరాశి
{
    FileStorage fs(filename, FileStorage::WRITE);
    ఒకవేళ (!fs.isOpened()) {
        CV_Error(Error::StsError, "File can't be opened for writing!");
    }
    ఇది->save(fs);
    fs.release();
}

// Deserializes this object from a given filename.
శూన్య LDA::load(స్థిరరాశి String& filename) {
    FileStorage fs(filename, FileStorage::READ);
    ఒకవేళ (!fs.isOpened())
       CV_Error(Error::StsError, "File can't be opened for reading!");
    ఇది->load(fs);
    fs.release();
}

// Serializes this object to a given FileStorage.
శూన్య LDA::save(FileStorage& fs) స్థిరరాశి {
    // write matrices
    fs << "num_components" << అంకె_components_2;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
}

// Deserializes this object from a given FileStorage.
శూన్య LDA::load(స్థిరరాశి FileStorage& fs) {
    //read matrices
    fs["num_components"] >> అంకె_components_2;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
}

శూన్య LDA::lda(ఆదానం_Array_యొక్క_Arrays _src, ఆదానం_Array _lbls) {
    // get data
    Mat src = _src.getMat();
    std::vector<పూర్ణాంకం> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        కొరకు(గుర్తులేని పూర్ణాంకం i = 0; i < tmp.మొత్తం(); i++) {
            labels.push_back(tmp.at<పూర్ణాంకం>(i));
        }
    }
    // turn into row sampled matrix
    Mat దత్తాంశం;
    // ensure working matrix is double precision
    src.convert_గమ్యం(దత్తాంశం, CV_64FC1);
    // maps the labels, so they're ascending: [0,1,...,C]
    std::vector<పూర్ణాంకం> mapped_labels(labels.size());
    std::vector<పూర్ణాంకం> num2label = remove_dups(labels);
    std::map<పూర్ణాంకం, పూర్ణాంకం> label2num;
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)num2label.size(); i++)
        label2num[num2label[i]] = i;
    కొరకు (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];
    // get sample size, dimension
    పూర్ణాంకం N = దత్తాంశం.అడ్డువరుసలు;
    పూర్ణాంకం D = దత్తాంశం.నిలువుల_సంఖ్య;
    // number of unique labels
    పూర్ణాంకం C = (పూర్ణాంకం)num2label.size();
    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    ఒకవేళ(C == 1) {
        String error_సందేశం = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // throw error if less labels, than samples
    ఒకవేళ (labels.size() != static_cast<size_t>(N)) {
        String error_సందేశం = format("The number of samples must equal the number of labels. Given %zu labels, %d samples. ", labels.size(), N);
        CV_Error(Error::StsBadArg, error_సందేశం);
    }
    // warn if within-classes scatter matrix becomes singular
    ఒకవేళ (N < D) {
        std::బయటకు << "Warning: Less observations than feature dimension given!"
                  << "Computation will probably fail."
                  << std::పంక్తి_ముగింపు;
    }
    // clip number of components to be a valid number
    ఒకవేళ ((అంకె_components_2 <= 0) || (అంకె_components_2 >= C)) {
        అంకె_components_2 = (C - 1);
    }
    // holds the mean over all classes
    Mat సగటు_మొత్తం = Mat::zeros(1, D, దత్తాంశం.type());
    // holds the mean for each class
    std::vector<Mat> సగటు_Class(C);
    std::vector<పూర్ణాంకం> అంకె_Class(C);
    // initialize
    కొరకు (పూర్ణాంకం i = 0; i < C; i++) {
        అంకె_Class[i] = 0;
        సగటు_Class[i] = Mat::zeros(1, D, దత్తాంశం.type()); //! Dx1 image vector
    }
    // calculate sums
    కొరకు (పూర్ణాంకం i = 0; i < N; i++) {
        Mat instance = దత్తాంశం.అడ్డువరుస(i);
        పూర్ణాంకం classIdx = mapped_labels[i];
        కూడు(సగటు_మొత్తం, instance, సగటు_మొత్తం);
        కూడు(సగటు_Class[classIdx], instance, సగటు_Class[classIdx]);
        అంకె_Class[classIdx]++;
    }
    // calculate total mean
    సగటు_మొత్తం.convert_గమ్యం(సగటు_మొత్తం, సగటు_మొత్తం.type(), 1.0 / static_cast<ద్విగుణ> (N));
    // calculate class means
    కొరకు (పూర్ణాంకం i = 0; i < C; i++) {
        సగటు_Class[i].convert_గమ్యం(సగటు_Class[i], సగటు_Class[i].type(), 1.0 / static_cast<ద్విగుణ> (అంకె_Class[i]));
    }
    // subtract class means
    కొరకు (పూర్ణాంకం i = 0; i < N; i++) {
        పూర్ణాంకం classIdx = mapped_labels[i];
        Mat instance = దత్తాంశం.అడ్డువరుస(i);
        వ్యవకలనం(instance, సగటు_Class[classIdx], instance);
    }
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(D, D, దత్తాంశం.type());
    mulTransposed(దత్తాంశం, Sw, నిజం);
    // calculate between-classes scatter
    Mat Sb = Mat::zeros(D, D, దత్తాంశం.type());
    కొరకు (పూర్ణాంకం i = 0; i < C; i++) {
        Mat tmp;
        వ్యవకలనం(సగటు_Class[i], సగటు_మొత్తం, tmp);
        mulTransposed(tmp, tmp, నిజం);
        కూడు(Sb, tmp, Sb);
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
    std::vector<పూర్ణాంకం> sorted_indices = argsort(_eigenvalues, అబద్ధం);
    // now sort eigenvalues and eigenvectors accordingly
    _eigenvalues = క్రమీకరించు_మాతృక_Columns_By_Indices(_eigenvalues, sorted_indices);
    _eigenvectors = క్రమీకరించు_మాతృక_Columns_By_Indices(_eigenvectors, sorted_indices);
    // and now take only the num_components and we're out!
    _eigenvalues = Mat(_eigenvalues, Range::all(), Range(0, అంకె_components_2));
    _eigenvectors = Mat(_eigenvectors, Range::all(), Range(0, అంకె_components_2));
}

శూన్య LDA::compute(ఆదానం_Array_యొక్క_Arrays _src, ఆదానం_Array _lbls) {
    ఎంపిక(_src.రకం()) {
    స్థితి ఆదానం_Array_2::STD_సదిశ_MAT:
    స్థితి ఆదానం_Array_2::STD_ARRAY_MAT:
        lda(as_అడ్డువరుస_మాతృక(_src, CV_64FC1), _lbls);
        ఆపు;
    స్థితి ఆదానం_Array_2::MAT:
        lda(_src.getMat(), _lbls);
        ఆపు;
    అప్రమేయ:
        String error_సందేశం= format("InputArray Datatype %d is not supported.", _src.రకం());
        CV_Error(Error::StsBadArg, error_సందేశం);
        ఆపు;
    }
}

// Projects one or more row aligned samples into the LDA subspace.
Mat LDA::project(ఆదానం_Array src) {
   తిరిగి subspaceProject(_eigenvectors, Mat(), src);
}

// Reconstructs projections from the LDA subspace from one or more row aligned samples.
Mat LDA::reconstruct(ఆదానం_Array src) {
   తిరిగి subspaceReconstruct(_eigenvectors, Mat(), src);
}

}
