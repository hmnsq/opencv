// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

नामाकाश cv
{

शून्य लिख( FileStorage& fs, ध्रुव String& name, ध्रुव Mat& m )
{
    वर्ण dt[22];

    जइ( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_ग्रन्थिः::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
        fs << "data" << "[:";
        प्रति( पूर्णाङ्क i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    अण्णहा
    {
        fs.startWriteStruct(name, File_ग्रन्थिः::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*परिमाण(पूर्णाङ्क) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
        fs << "data" << "[:";
        ध्रुव Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

संरचना Sparse_ग्रन्थिः_Cmp
{
    Sparse_ग्रन्थिः_Cmp(पूर्णाङ्क _dims) : dims(_dims) {}
    तार्किक operator()(ध्रुव SparseMat::Node* a, ध्रुव SparseMat::Node* b)
    {
        प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
        {
            पूर्णाङ्क d = a->idx[i] - b->idx[i];
            जइ(d)
                प्रत्यागम d < 0;
        }
        प्रत्यागम मिच्छ;
    }

    पूर्णाङ्क dims;
};

शून्य लिख( FileStorage& fs, ध्रुव String& name, ध्रुव SparseMat& m )
{
    वर्ण dt[22];

    fs.startWriteStruct(name, File_ग्रन्थिः::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    पूर्णाङ्क dims = m.dims();
    जइ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*परिमाण(पूर्णाङ्क) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ध्रुव SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    प्रति( ; it != it_end; ++it )
    {
        CV_Assert(it.ग्रन्थिः() != 0);
        elems[i++] = it.ग्रन्थिः();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ग्रन्थिः_Cmp(dims));
    ध्रुव SparseMat::Node* prev_ग्रन्थिः = 0;
    size_t esz = m.elemSize();

    प्रति( i = 0; i < n; i++ )
    {
        ध्रुव SparseMat::Node* ग्रन्थिः = elems[i];
        पूर्णाङ्क k = 0;

        जइ( prev_ग्रन्थिः )
        {
            प्रति( ; k < dims; k++ )
                जइ( ग्रन्थिः->idx[k] != prev_ग्रन्थिः->idx[k] )
                    भङ्ग;
            CV_Assert( k < dims );
            जइ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        प्रति( ; k < dims; k++ )
            writeScalar( fs, ग्रन्थिः->idx[k] );
        prev_ग्रन्थिः = ग्रन्थिः;

        ध्रुव uchar* value = &m.value<uchar>(ग्रन्थिः);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, Mat& m, ध्रुव Mat& default_mat)
{
    जइ( ग्रन्थिः.empty() )
    {
        default_mat.copyTo(m);
        प्रत्यागम;
    }

    std::string dt;
    पूर्णाङ्क rows, cols, elem_type;

    पठ(ग्रन्थिः["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पठ(ग्रन्थिः["rows"], rows, -1);
    जइ( rows >= 0 )
    {
        पठ(ग्रन्थिः["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    अण्णहा
    {
        पूर्णाङ्क sizes[CV_MAX_DIM] = {0}, dims;
        File_ग्रन्थिः sizes_ग्रन्थिः = ग्रन्थिः["sizes"];
        CV_Assert( !sizes_ग्रन्थिः.empty() );

        dims = (पूर्णाङ्क)sizes_ग्रन्थिः.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ग्रन्थिः.readRaw("i", sizes, dims*परिमाण(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ग्रन्थिः data_ग्रन्थिः = ग्रन्थिः["data"];
    CV_Assert(!data_ग्रन्थिः.empty());

    size_t nelems = data_ग्रन्थिः.size();
    जइ (nelems == 0)
    {
        m = Mat();
        प्रत्यागम;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_ग्रन्थिः.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

शून्य पठ( ध्रुव File_ग्रन्थिः& ग्रन्थिः, SparseMat& m, ध्रुव SparseMat& default_mat )
{
    जइ( ग्रन्थिः.empty() )
    {
        default_mat.copyTo(m);
        प्रत्यागम;
    }

    std::string dt;
    पठ(ग्रन्थिः["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    पूर्णाङ्क elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पूर्णाङ्क sizes[CV_MAX_DIM] = {0};
    File_ग्रन्थिः sizes_ग्रन्थिः = ग्रन्थिः["sizes"];
    CV_Assert( !sizes_ग्रन्थिः.empty() );

    पूर्णाङ्क dims = (पूर्णाङ्क)sizes_ग्रन्थिः.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ग्रन्थिः.readRaw("i", sizes, dims*परिमाण(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ग्रन्थिः data = ग्रन्थिः["data"];
    CV_Assert( data.isSeq() );

    पूर्णाङ्क cn = CV_MAT_CN(elem_type);
    पूर्णाङ्क idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_ग्रन्थिः_Iterator it = data.begin();

    प्रति( i = 0; i < sz; )
    {
        File_ग्रन्थिः n = *it;
        पूर्णाङ्क k = (पूर्णाङ्क)n;
        जइ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        अण्णहा
        {
            जइ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            अण्णहा
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            प्रति( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                पूर्णाङ्क idx_k = (पूर्णाङ्क)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, सच्च);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, कुञ्चिका_Point& value, ध्रुव कुञ्चिका_Point& default_मानम्)
{
    जइ( ग्रन्थिः.empty() )
    {
        value = default_मानम्;
        प्रत्यागम;
    }
    ग्रन्थिः >> value;
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, DMatch& value, ध्रुव DMatch& default_मानम्)
{
    जइ( ग्रन्थिः.empty() )
    {
        value = default_मानम्;
        प्रत्यागम;
    }
    ग्रन्थिः >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
शून्य लिख( FileStorage& fs, ध्रुव std::string& name, ध्रुव std::vector<कुञ्चिका_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ग्रन्थिः::SEQ);
    लिख(fs, vec);
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, std::vector<कुञ्चिका_Point>& keypoints)
{
    File_ग्रन्थिः first_ग्रन्थिः = *(ग्रन्थिः.begin());
    जइ (first_ग्रन्थिः.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        प्रति (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#अण्णहा
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        it >> keypoints;
#endif
        प्रत्यागम;
    }
    keypoints.clear();
    File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin(), it_end = ग्रन्थिः.end();
    प्रति( ; it != it_end; )
    {
        कुञ्चिका_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

शून्य लिख( FileStorage& fs, ध्रुव std::string& name, ध्रुव std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ग्रन्थिः::SEQ);
    लिख(fs, vec);
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, std::vector<DMatch>& matches)
{
    File_ग्रन्थिः first_ग्रन्थिः = *(ग्रन्थिः.begin());
    जइ (first_ग्रन्थिः.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        प्रति (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#अण्णहा
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        it >> matches;
#endif
        प्रत्यागम;
    }
    matches.clear();
    File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin(), it_end = ग्रन्थिः.end();
    प्रति( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
