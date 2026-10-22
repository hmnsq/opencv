// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "persistence.hpp"

namespace cv
{

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, स्थिर Mat& m )
{
    अक्षर dt[22];

    अगर( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_गाँठ::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, माप(dt) );
        fs << "data" << "[:";
        क्रम( पूर्णांक i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elem_आकार());
        fs << "]";
        fs.endWriteStruct();
    }
    अन्यथा
    {
        fs.startWriteStruct(name, File_गाँठ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*माप(पूर्णांक) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, माप(dt) );
        fs << "data" << "[:";
        स्थिर Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        माप_प्रकार योग = it.size*m.elem_आकार();

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], योग );
        fs << "]";
        fs.endWriteStruct();
    }
}

काष्ठा Sparse_गाँठ_Cmp
{
    Sparse_गाँठ_Cmp(पूर्णांक _dims) : dims(_dims) {}
    bool चालक()(स्थिर SparseMat::Node* a, स्थिर SparseMat::Node* b)
    {
        क्रम( पूर्णांक i = 0; i < dims; i++ )
        {
            पूर्णांक d = a->idx[i] - b->idx[i];
            अगर(d)
                वापस d < 0;
        }
        वापस false;
    }

    पूर्णांक dims;
};

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, स्थिर SparseMat& m )
{
    अक्षर dt[22];

    fs.startWriteStruct(name, File_गाँठ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    पूर्णांक dims = m.dims();
    अगर( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*माप(पूर्णांक) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, माप(dt) );
    fs << "data" << "[:";

    माप_प्रकार i = 0, n = m.nzcount();
    Auto_बफर<स्थिर SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    क्रम( ; it != it_end; ++it )
    {
        CV_Assert(it.गाँठ() != 0);
        elems[i++] = it.गाँठ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_गाँठ_Cmp(dims));
    स्थिर SparseMat::Node* पूर्व_गाँठ = 0;
    माप_प्रकार esz = m.elem_आकार();

    क्रम( i = 0; i < n; i++ )
    {
        स्थिर SparseMat::Node* गाँठ = elems[i];
        पूर्णांक k = 0;

        अगर( पूर्व_गाँठ )
        {
            क्रम( ; k < dims; k++ )
                अगर( गाँठ->idx[k] != पूर्व_गाँठ->idx[k] )
                    अवरोध;
            CV_Assert( k < dims );
            अगर( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        क्रम( ; k < dims; k++ )
            writeScalar( fs, गाँठ->idx[k] );
        पूर्व_गाँठ = गाँठ;

        स्थिर uchar* value = &m.value<uchar>(गाँठ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, Mat& m, स्थिर Mat& default_mat)
{
    अगर( गाँठ.empty() )
    {
        default_mat.copyTo(m);
        वापस;
    }

    std::string dt;
    पूर्णांक rows, cols, elem_type;

    पढ़ो(गाँठ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पढ़ो(गाँठ["rows"], rows, -1);
    अगर( rows >= 0 )
    {
        पढ़ो(गाँठ["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    अन्यथा
    {
        पूर्णांक sizes[CV_MAX_DIM] = {0}, dims;
        File_गाँठ sizes_गाँठ = गाँठ["sizes"];
        CV_Assert( !sizes_गाँठ.empty() );

        dims = (पूर्णांक)sizes_गाँठ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_गाँठ.readRaw("i", sizes, dims*माप(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_गाँठ तथ्य_गाँठ = गाँठ["data"];
    CV_Assert(!तथ्य_गाँठ.empty());

    माप_प्रकार nelems = तथ्य_गाँठ.size();
    अगर (nelems == 0)
    {
        m = Mat();
        वापस;
    }
    CV_Assert(nelems == m.योग()*m.channels());

    तथ्य_गाँठ.readRaw(dt, (uchar*)m.ptr(), m.योग()*m.elem_आकार());
}

व्योम पढ़ो( स्थिर File_गाँठ& गाँठ, SparseMat& m, स्थिर SparseMat& default_mat )
{
    अगर( गाँठ.empty() )
    {
        default_mat.copyTo(m);
        वापस;
    }

    std::string dt;
    पढ़ो(गाँठ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    पूर्णांक elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पूर्णांक sizes[CV_MAX_DIM] = {0};
    File_गाँठ sizes_गाँठ = गाँठ["sizes"];
    CV_Assert( !sizes_गाँठ.empty() );

    पूर्णांक dims = (पूर्णांक)sizes_गाँठ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_गाँठ.readRaw("i", sizes, dims*माप(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_गाँठ तथ्य = गाँठ["data"];
    CV_Assert( तथ्य.isSeq() );

    पूर्णांक cn = CV_MAT_CN(elem_type);
    पूर्णांक idx[CV_MAX_DIM] = {0};
    माप_प्रकार i, sz = तथ्य.size();
    माप_प्रकार esz = m.elem_आकार();
    File_गाँठ_Iterator it = तथ्य.begin();

    क्रम( i = 0; i < sz; )
    {
        File_गाँठ n = *it;
        पूर्णांक k = (पूर्णांक)n;
        अगर( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        अन्यथा
        {
            अगर( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            अन्यथा
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            क्रम( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                पूर्णांक idx_k = (पूर्णांक)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, true);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, कुंजी_Point& value, स्थिर कुंजी_Point& default_मूल्य)
{
    अगर( गाँठ.empty() )
    {
        value = default_मूल्य;
        वापस;
    }
    गाँठ >> value;
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, DMatch& value, स्थिर DMatch& default_मूल्य)
{
    अगर( गाँठ.empty() )
    {
        value = default_मूल्य;
        वापस;
    }
    गाँठ >> value;
}

#अगर_घोषित CV__LEGACY_PERSISTENCE
व्योम ग_लिखो( FileStorage& fs, स्थिर std::string& name, स्थिर std::vector<कुंजी_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext खाली(fs, name, File_गाँठ::SEQ);
    ग_लिखो(fs, vec);
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, std::vector<कुंजी_Point>& keypoints)
{
    File_गाँठ first_गाँठ = *(गाँठ.begin());
    अगर (first_गाँठ.isSeq())
    {
        // modern scheme
#अगर_घोषित OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाँठ_Iterator it = गाँठ.begin();
        माप_प्रकार योग = (माप_प्रकार)it.remaining;
        keypoints.resize(योग);
        क्रम (माप_प्रकार i = 0; i < योग; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#अन्यथा
        File_गाँठ_Iterator it = गाँठ.begin();
        it >> keypoints;
#पूर्ण_अगर
        वापस;
    }
    keypoints.सफा();
    File_गाँठ_Iterator it = गाँठ.begin(), it_end = गाँठ.end();
    क्रम( ; it != it_end; )
    {
        कुंजी_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

व्योम ग_लिखो( FileStorage& fs, स्थिर std::string& name, स्थिर std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext खाली(fs, name, File_गाँठ::SEQ);
    ग_लिखो(fs, vec);
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, std::vector<DMatch>& matches)
{
    File_गाँठ first_गाँठ = *(गाँठ.begin());
    अगर (first_गाँठ.isSeq())
    {
        // modern scheme
#अगर_घोषित OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाँठ_Iterator it = गाँठ.begin();
        माप_प्रकार योग = (माप_प्रकार)it.remaining;
        matches.resize(योग);
        क्रम (माप_प्रकार i = 0; i < योग; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#अन्यथा
        File_गाँठ_Iterator it = गाँठ.begin();
        it >> matches;
#पूर्ण_अगर
        वापस;
    }
    matches.सफा();
    File_गाँठ_Iterator it = गाँठ.begin(), it_end = गाँठ.end();
    क्रम( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#पूर्ण_अगर


}
