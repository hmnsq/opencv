// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ನಾಮಾಕಾಶ cv
{

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ಧ್ರುವ Mat& m )
{
    ವರ್ಣ dt[22];

    ಯದಿ( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_ಗಂಟು::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, ಪರಿಮಾಣ(dt) );
        fs << "data" << "[:";
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    ಅನ್ಯಥಾ
    {
        fs.startWriteStruct(name, File_ಗಂಟು::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, ಪರಿಮಾಣ(dt) );
        fs << "data" << "[:";
        ಧ್ರುವ Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

ಸಂರಚನಾ Sparse_ಗಂಟು_Cmp
{
    Sparse_ಗಂಟು_Cmp(ಪೂರ್ಣಾಙ್ಕ _dims) : dims(_dims) {}
    ತಾರ್ಕಿಕ operator()(ಧ್ರುವ SparseMat::Node* a, ಧ್ರುವ SparseMat::Node* b)
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
        {
            ಪೂರ್ಣಾಙ್ಕ d = a->idx[i] - b->idx[i];
            ಯದಿ(d)
                ಪ್ರತ್ಯಾಗಮ d < 0;
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ dims;
};

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ಧ್ರುವ SparseMat& m )
{
    ವರ್ಣ dt[22];

    fs.startWriteStruct(name, File_ಗಂಟು::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ಪೂರ್ಣಾಙ್ಕ dims = m.dims();
    ಯದಿ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, ಪರಿಮಾಣ(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ಧ್ರುವ SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    ಪ್ರತಿ( ; it != it_end; ++it )
    {
        CV_Assert(it.ಗಂಟು() != 0);
        elems[i++] = it.ಗಂಟು();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ಗಂಟು_Cmp(dims));
    ಧ್ರುವ SparseMat::Node* prev_ಗಂಟು = 0;
    size_t esz = m.elemSize();

    ಪ್ರತಿ( i = 0; i < n; i++ )
    {
        ಧ್ರುವ SparseMat::Node* ಗಂಟು = elems[i];
        ಪೂರ್ಣಾಙ್ಕ k = 0;

        ಯದಿ( prev_ಗಂಟು )
        {
            ಪ್ರತಿ( ; k < dims; k++ )
                ಯದಿ( ಗಂಟು->idx[k] != prev_ಗಂಟು->idx[k] )
                    ಭಙ್ಗ;
            CV_Assert( k < dims );
            ಯದಿ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        ಪ್ರತಿ( ; k < dims; k++ )
            writeScalar( fs, ಗಂಟು->idx[k] );
        prev_ಗಂಟು = ಗಂಟು;

        ಧ್ರುವ uchar* value = &m.value<uchar>(ಗಂಟು);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, Mat& m, ಧ್ರುವ Mat& default_mat)
{
    ಯದಿ( ಗಂಟು.empty() )
    {
        default_mat.copyTo(m);
        ಪ್ರತ್ಯಾಗಮ;
    }

    std::string dt;
    ಪೂರ್ಣಾಙ್ಕ rows, cols, elem_type;

    ಪಠ(ಗಂಟು["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ಪಠ(ಗಂಟು["rows"], rows, -1);
    ಯದಿ( rows >= 0 )
    {
        ಪಠ(ಗಂಟು["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ sizes[CV_MAX_DIM] = {0}, dims;
        File_ಗಂಟು sizes_ಗಂಟು = ಗಂಟು["sizes"];
        CV_Assert( !sizes_ಗಂಟು.empty() );

        dims = (ಪೂರ್ಣಾಙ್ಕ)sizes_ಗಂಟು.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ಗಂಟು.readRaw("i", sizes, dims*ಪರಿಮಾಣ(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ಗಂಟು data_ಗಂಟು = ಗಂಟು["data"];
    CV_Assert(!data_ಗಂಟು.empty());

    size_t nelems = data_ಗಂಟು.size();
    ಯದಿ (nelems == 0)
    {
        m = Mat();
        ಪ್ರತ್ಯಾಗಮ;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_ಗಂಟು.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

ಶೂನ್ಯ ಪಠ( ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, SparseMat& m, ಧ್ರುವ SparseMat& default_mat )
{
    ಯದಿ( ಗಂಟು.empty() )
    {
        default_mat.copyTo(m);
        ಪ್ರತ್ಯಾಗಮ;
    }

    std::string dt;
    ಪಠ(ಗಂಟು["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ಪೂರ್ಣಾಙ್ಕ elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ಪೂರ್ಣಾಙ್ಕ sizes[CV_MAX_DIM] = {0};
    File_ಗಂಟು sizes_ಗಂಟು = ಗಂಟು["sizes"];
    CV_Assert( !sizes_ಗಂಟು.empty() );

    ಪೂರ್ಣಾಙ್ಕ dims = (ಪೂರ್ಣಾಙ್ಕ)sizes_ಗಂಟು.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ಗಂಟು.readRaw("i", sizes, dims*ಪರಿಮಾಣ(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ಗಂಟು data = ಗಂಟು["data"];
    CV_Assert( data.isSeq() );

    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(elem_type);
    ಪೂರ್ಣಾಙ್ಕ idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_ಗಂಟು_Iterator it = data.begin();

    ಪ್ರತಿ( i = 0; i < sz; )
    {
        File_ಗಂಟು n = *it;
        ಪೂರ್ಣಾಙ್ಕ k = (ಪೂರ್ಣಾಙ್ಕ)n;
        ಯದಿ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ಅನ್ಯಥಾ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            ಪ್ರತಿ( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ಪೂರ್ಣಾಙ್ಕ idx_k = (ಪೂರ್ಣಾಙ್ಕ)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, ಸತ್ಯ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, ಕೀಲಿ_Point& value, ಧ್ರುವ ಕೀಲಿ_Point& default_ಮೌಲ್ಯ)
{
    ಯದಿ( ಗಂಟು.empty() )
    {
        value = default_ಮೌಲ್ಯ;
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಗಂಟು >> value;
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, DMatch& value, ಧ್ರುವ DMatch& default_ಮೌಲ್ಯ)
{
    ಯದಿ( ಗಂಟು.empty() )
    {
        value = default_ಮೌಲ್ಯ;
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಗಂಟು >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ std::string& name, ಧ್ರುವ std::vector<ಕೀಲಿ_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ಗಂಟು::SEQ);
    ಲಿಖ(fs, vec);
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, std::vector<ಕೀಲಿ_Point>& keypoints)
{
    File_ಗಂಟು first_ಗಂಟು = *(ಗಂಟು.begin());
    ಯದಿ (first_ಗಂಟು.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        ಪ್ರತಿ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ಅನ್ಯಥಾ
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        it >> keypoints;
#endif
        ಪ್ರತ್ಯಾಗಮ;
    }
    keypoints.clear();
    File_ಗಂಟು_Iterator it = ಗಂಟು.begin(), it_end = ಗಂಟು.end();
    ಪ್ರತಿ( ; it != it_end; )
    {
        ಕೀಲಿ_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ std::string& name, ಧ್ರುವ std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ಗಂಟು::SEQ);
    ಲಿಖ(fs, vec);
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, std::vector<DMatch>& matches)
{
    File_ಗಂಟು first_ಗಂಟು = *(ಗಂಟು.begin());
    ಯದಿ (first_ಗಂಟು.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        ಪ್ರತಿ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ಅನ್ಯಥಾ
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        it >> matches;
#endif
        ಪ್ರತ್ಯಾಗಮ;
    }
    matches.clear();
    File_ಗಂಟು_Iterator it = ಗಂಟು.begin(), it_end = ಗಂಟು.end();
    ಪ್ರತಿ( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
