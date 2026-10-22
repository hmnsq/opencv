// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

नामाकाश cv
{

शून्य लिख( FileStorage& fs, अचर String& name, अचर Mat& m )
{
    वर्ण dt[22];

    यदि( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_गाँठो::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
        fs << "data" << "[:";
        लागि( पूर्णाङ्क i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    अन्यथा
    {
        fs.startWriteStruct(name, File_गाँठो::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*परिमाण(पूर्णाङ्क) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
        fs << "data" << "[:";
        अचर Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        माप_प्रकार total = it.size*m.elemSize();

        लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

संरचना Sparse_गाँठो_Cmp
{
    Sparse_गाँठो_Cmp(पूर्णाङ्क _dims) : dims(_dims) {}
    तार्किक चालक()(अचर SparseMat::Node* a, अचर SparseMat::Node* b)
    {
        लागि( पूर्णाङ्क i = 0; i < dims; i++ )
        {
            पूर्णाङ्क d = a->idx[i] - b->idx[i];
            यदि(d)
                फर्काऊ d < 0;
        }
        फर्काऊ असत्य;
    }

    पूर्णाङ्क dims;
};

शून्य लिख( FileStorage& fs, अचर String& name, अचर SparseMat& m )
{
    वर्ण dt[22];

    fs.startWriteStruct(name, File_गाँठो::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    पूर्णाङ्क dims = m.dims();
    यदि( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*परिमाण(पूर्णाङ्क) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
    fs << "data" << "[:";

    माप_प्रकार i = 0, n = m.nzcount();
    AutoBuffer<अचर SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    लागि( ; it != it_end; ++it )
    {
        CV_Assert(it.गाँठो() != 0);
        elems[i++] = it.गाँठो();
    }

    std::sort(elems.begin(), elems.end(), Sparse_गाँठो_Cmp(dims));
    अचर SparseMat::Node* prev_गाँठो = 0;
    माप_प्रकार esz = m.elemSize();

    लागि( i = 0; i < n; i++ )
    {
        अचर SparseMat::Node* गाँठो = elems[i];
        पूर्णाङ्क k = 0;

        यदि( prev_गाँठो )
        {
            लागि( ; k < dims; k++ )
                यदि( गाँठो->idx[k] != prev_गाँठो->idx[k] )
                    रोक;
            CV_Assert( k < dims );
            यदि( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        लागि( ; k < dims; k++ )
            writeScalar( fs, गाँठो->idx[k] );
        prev_गाँठो = गाँठो;

        अचर uchar* value = &m.value<uchar>(गाँठो);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

शून्य पठ(अचर File_गाँठो& गाँठो, Mat& m, अचर Mat& default_mat)
{
    यदि( गाँठो.empty() )
    {
        default_mat.copyTo(m);
        फर्काऊ;
    }

    std::string dt;
    पूर्णाङ्क rows, cols, elem_type;

    पठ(गाँठो["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पठ(गाँठो["rows"], rows, -1);
    यदि( rows >= 0 )
    {
        पठ(गाँठो["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    अन्यथा
    {
        पूर्णाङ्क sizes[CV_MAX_DIM] = {0}, dims;
        File_गाँठो sizes_गाँठो = गाँठो["sizes"];
        CV_Assert( !sizes_गाँठो.empty() );

        dims = (पूर्णाङ्क)sizes_गाँठो.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_गाँठो.readRaw("i", sizes, dims*परिमाण(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_गाँठो data_गाँठो = गाँठो["data"];
    CV_Assert(!data_गाँठो.empty());

    माप_प्रकार nelems = data_गाँठो.size();
    यदि (nelems == 0)
    {
        m = Mat();
        फर्काऊ;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_गाँठो.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

शून्य पठ( अचर File_गाँठो& गाँठो, SparseMat& m, अचर SparseMat& default_mat )
{
    यदि( गाँठो.empty() )
    {
        default_mat.copyTo(m);
        फर्काऊ;
    }

    std::string dt;
    पठ(गाँठो["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    पूर्णाङ्क elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पूर्णाङ्क sizes[CV_MAX_DIM] = {0};
    File_गाँठो sizes_गाँठो = गाँठो["sizes"];
    CV_Assert( !sizes_गाँठो.empty() );

    पूर्णाङ्क dims = (पूर्णाङ्क)sizes_गाँठो.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_गाँठो.readRaw("i", sizes, dims*परिमाण(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_गाँठो data = गाँठो["data"];
    CV_Assert( data.isSeq() );

    पूर्णाङ्क cn = CV_MAT_CN(elem_type);
    पूर्णाङ्क idx[CV_MAX_DIM] = {0};
    माप_प्रकार i, sz = data.size();
    माप_प्रकार esz = m.elemSize();
    File_गाँठो_Iterator it = data.begin();

    लागि( i = 0; i < sz; )
    {
        File_गाँठो n = *it;
        पूर्णाङ्क k = (पूर्णाङ्क)n;
        यदि( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        अन्यथा
        {
            यदि( i > 0 )
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
            लागि( ; k < dims; k++ )
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
        uchar* valptr = m.ptr(idx, सत्य);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

शून्य पठ(अचर File_गाँठो& गाँठो, साँचो_Point& value, अचर साँचो_Point& default_मान)
{
    यदि( गाँठो.empty() )
    {
        value = default_मान;
        फर्काऊ;
    }
    गाँठो >> value;
}

शून्य पठ(अचर File_गाँठो& गाँठो, DMatch& value, अचर DMatch& default_मान)
{
    यदि( गाँठो.empty() )
    {
        value = default_मान;
        फर्काऊ;
    }
    गाँठो >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
शून्य लिख( FileStorage& fs, अचर std::string& name, अचर std::vector<साँचो_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_गाँठो::SEQ);
    लिख(fs, vec);
}

शून्य पठ(अचर File_गाँठो& गाँठो, std::vector<साँचो_Point>& keypoints)
{
    File_गाँठो first_गाँठो = *(गाँठो.begin());
    यदि (first_गाँठो.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाँठो_Iterator it = गाँठो.begin();
        माप_प्रकार total = (माप_प्रकार)it.remaining;
        keypoints.resize(total);
        लागि (माप_प्रकार i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#अन्यथा
        File_गाँठो_Iterator it = गाँठो.begin();
        it >> keypoints;
#endif
        फर्काऊ;
    }
    keypoints.clear();
    File_गाँठो_Iterator it = गाँठो.begin(), it_end = गाँठो.end();
    लागि( ; it != it_end; )
    {
        साँचो_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

शून्य लिख( FileStorage& fs, अचर std::string& name, अचर std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_गाँठो::SEQ);
    लिख(fs, vec);
}

शून्य पठ(अचर File_गाँठो& गाँठो, std::vector<DMatch>& matches)
{
    File_गाँठो first_गाँठो = *(गाँठो.begin());
    यदि (first_गाँठो.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाँठो_Iterator it = गाँठो.begin();
        माप_प्रकार total = (माप_प्रकार)it.remaining;
        matches.resize(total);
        लागि (माप_प्रकार i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#अन्यथा
        File_गाँठो_Iterator it = गाँठो.begin();
        it >> matches;
#endif
        फर्काऊ;
    }
    matches.clear();
    File_गाँठो_Iterator it = गाँठो.begin(), it_end = गाँठो.end();
    लागि( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
