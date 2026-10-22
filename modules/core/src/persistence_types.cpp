// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence.hpp"

നാമാകാശ cv
{

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, സ്ഥിരം Mat& m )
{
    അക്ഷരം dt[22];

    എങ്കിൽ( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_കണ്ണി::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, വലുപ്പം(dt) );
        fs << "data" << "[:";
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    അല്ലെങ്കിൽ
    {
        fs.startWriteStruct(name, File_കണ്ണി::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*വലുപ്പം(പൂർണ്ണസംഖ്യ) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, വലുപ്പം(dt) );
        fs << "data" << "[:";
        സ്ഥിരം Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

ഘടന Sparse_കണ്ണി_Cmp
{
    Sparse_കണ്ണി_Cmp(പൂർണ്ണസംഖ്യ _dims) : dims(_dims) {}
    തർക്കം operator()(സ്ഥിരം SparseMat::Node* a, സ്ഥിരം SparseMat::Node* b)
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
        {
            പൂർണ്ണസംഖ്യ d = a->idx[i] - b->idx[i];
            എങ്കിൽ(d)
                തിരികെ d < 0;
        }
        തിരികെ അസത്യം;
    }

    പൂർണ്ണസംഖ്യ dims;
};

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, സ്ഥിരം SparseMat& m )
{
    അക്ഷരം dt[22];

    fs.startWriteStruct(name, File_കണ്ണി::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    പൂർണ്ണസംഖ്യ dims = m.dims();
    എങ്കിൽ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*വലുപ്പം(പൂർണ്ണസംഖ്യ) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, വലുപ്പം(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<സ്ഥിരം SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    വേണ്ടി( ; it != it_end; ++it )
    {
        CV_Assert(it.കണ്ണി() != 0);
        elems[i++] = it.കണ്ണി();
    }

    std::sort(elems.begin(), elems.end(), Sparse_കണ്ണി_Cmp(dims));
    സ്ഥിരം SparseMat::Node* prev_കണ്ണി = 0;
    size_t esz = m.elemSize();

    വേണ്ടി( i = 0; i < n; i++ )
    {
        സ്ഥിരം SparseMat::Node* കണ്ണി = elems[i];
        പൂർണ്ണസംഖ്യ k = 0;

        എങ്കിൽ( prev_കണ്ണി )
        {
            വേണ്ടി( ; k < dims; k++ )
                എങ്കിൽ( കണ്ണി->idx[k] != prev_കണ്ണി->idx[k] )
                    നിർത്തുക;
            CV_Assert( k < dims );
            എങ്കിൽ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        വേണ്ടി( ; k < dims; k++ )
            writeScalar( fs, കണ്ണി->idx[k] );
        prev_കണ്ണി = കണ്ണി;

        സ്ഥിരം uchar* value = &m.value<uchar>(കണ്ണി);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, Mat& m, സ്ഥിരം Mat& default_mat)
{
    എങ്കിൽ( കണ്ണി.empty() )
    {
        default_mat.copyTo(m);
        തിരികെ;
    }

    std::string dt;
    പൂർണ്ണസംഖ്യ rows, cols, elem_type;

    പഠ(കണ്ണി["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    പഠ(കണ്ണി["rows"], rows, -1);
    എങ്കിൽ( rows >= 0 )
    {
        പഠ(കണ്ണി["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ sizes[CV_MAX_DIM] = {0}, dims;
        File_കണ്ണി sizes_കണ്ണി = കണ്ണി["sizes"];
        CV_Assert( !sizes_കണ്ണി.empty() );

        dims = (പൂർണ്ണസംഖ്യ)sizes_കണ്ണി.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_കണ്ണി.readRaw("i", sizes, dims*വലുപ്പം(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_കണ്ണി data_കണ്ണി = കണ്ണി["data"];
    CV_Assert(!data_കണ്ണി.empty());

    size_t nelems = data_കണ്ണി.size();
    എങ്കിൽ (nelems == 0)
    {
        m = Mat();
        തിരികെ;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_കണ്ണി.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

ശൂന്യം പഠ( സ്ഥിരം File_കണ്ണി& കണ്ണി, SparseMat& m, സ്ഥിരം SparseMat& default_mat )
{
    എങ്കിൽ( കണ്ണി.empty() )
    {
        default_mat.copyTo(m);
        തിരികെ;
    }

    std::string dt;
    പഠ(കണ്ണി["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    പൂർണ്ണസംഖ്യ elem_type = fs::decodeSimpleFormat( dt.c_str() );

    പൂർണ്ണസംഖ്യ sizes[CV_MAX_DIM] = {0};
    File_കണ്ണി sizes_കണ്ണി = കണ്ണി["sizes"];
    CV_Assert( !sizes_കണ്ണി.empty() );

    പൂർണ്ണസംഖ്യ dims = (പൂർണ്ണസംഖ്യ)sizes_കണ്ണി.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_കണ്ണി.readRaw("i", sizes, dims*വലുപ്പം(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_കണ്ണി data = കണ്ണി["data"];
    CV_Assert( data.isSeq() );

    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(elem_type);
    പൂർണ്ണസംഖ്യ idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_കണ്ണി_Iterator it = data.begin();

    വേണ്ടി( i = 0; i < sz; )
    {
        File_കണ്ണി n = *it;
        പൂർണ്ണസംഖ്യ k = (പൂർണ്ണസംഖ്യ)n;
        എങ്കിൽ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            അല്ലെങ്കിൽ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            വേണ്ടി( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                പൂർണ്ണസംഖ്യ idx_k = (പൂർണ്ണസംഖ്യ)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, സത്യം);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, താക്കോൽ_Point& value, സ്ഥിരം താക്കോൽ_Point& default_മൂല്യം)
{
    എങ്കിൽ( കണ്ണി.empty() )
    {
        value = default_മൂല്യം;
        തിരികെ;
    }
    കണ്ണി >> value;
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, DMatch& value, സ്ഥിരം DMatch& default_മൂല്യം)
{
    എങ്കിൽ( കണ്ണി.empty() )
    {
        value = default_മൂല്യം;
        തിരികെ;
    }
    കണ്ണി >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം std::string& name, സ്ഥിരം std::vector<താക്കോൽ_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_കണ്ണി::SEQ);
    ലിഖ(fs, vec);
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, std::vector<താക്കോൽ_Point>& keypoints)
{
    File_കണ്ണി first_കണ്ണി = *(കണ്ണി.begin());
    എങ്കിൽ (first_കണ്ണി.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_കണ്ണി_Iterator it = കണ്ണി.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        വേണ്ടി (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#അല്ലെങ്കിൽ
        File_കണ്ണി_Iterator it = കണ്ണി.begin();
        it >> keypoints;
#endif
        തിരികെ;
    }
    keypoints.clear();
    File_കണ്ണി_Iterator it = കണ്ണി.begin(), it_end = കണ്ണി.end();
    വേണ്ടി( ; it != it_end; )
    {
        താക്കോൽ_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം std::string& name, സ്ഥിരം std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_കണ്ണി::SEQ);
    ലിഖ(fs, vec);
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, std::vector<DMatch>& matches)
{
    File_കണ്ണി first_കണ്ണി = *(കണ്ണി.begin());
    എങ്കിൽ (first_കണ്ണി.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_കണ്ണി_Iterator it = കണ്ണി.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        വേണ്ടി (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#അല്ലെങ്കിൽ
        File_കണ്ണി_Iterator it = കണ്ണി.begin();
        it >> matches;
#endif
        തിരികെ;
    }
    matches.clear();
    File_കണ്ണി_Iterator it = കണ്ണി.begin(), it_end = കണ്ണി.end();
    വേണ്ടി( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
