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

    यदि( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_ग्रन्थिः::MAP, String("opencv-matrix"));
        fs << "rows" << m.पङ्क्तिकाः;
        fs << "cols" << m.स्तम्भसङ्ख्या;
        fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
        fs << "data" << "[:";
        प्रति( पूर्णाङ्क i = 0; i < m.पङ्क्तिकाः; i++ )
            fs.writeRaw(dt, m.ptr(i), m.स्तम्भसङ्ख्या*m.elem_आकार());
        fs << "]";
        fs.अन्तः_Write_Struct();
    }
    अन्यथा
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
        size_t योग = it.size*m.elem_आकार();

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], योग );
        fs << "]";
        fs.अन्तः_Write_Struct();
    }
}

संरचना Sparse_ग्रन्थिः_Cmp
{
    Sparse_ग्रन्थिः_Cmp(पूर्णाङ्क _dims) : dims(_dims) {}
    तार्किक संक्रियकः()(ध्रुव SparseMat::Node* a, ध्रुव SparseMat::Node* b)
    {
        प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
        {
            पूर्णाङ्क d = a->idx[i] - b->idx[i];
            यदि(d)
                प्रत्यागम d < 0;
        }
        प्रत्यागम असत्य;
    }

    पूर्णाङ्क dims;
};

शून्य लिख( FileStorage& fs, ध्रुव String& name, ध्रुव SparseMat& m )
{
    वर्ण dt[22];

    fs.startWriteStruct(name, File_ग्रन्थिः::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    पूर्णाङ्क dims = m.dims();
    यदि( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*परिमाण(पूर्णाङ्क) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, परिमाण(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_बफर<ध्रुव SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_अन्तः = m.end();

    प्रति( ; it != it_अन्तः; ++it )
    {
        CV_Assert(it.ग्रन्थिः() != 0);
        elems[i++] = it.ग्रन्थिः();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ग्रन्थिः_Cmp(dims));
    ध्रुव SparseMat::Node* पूर्व_ग्रन्थिः = 0;
    size_t esz = m.elem_आकार();

    प्रति( i = 0; i < n; i++ )
    {
        ध्रुव SparseMat::Node* ग्रन्थिः = elems[i];
        पूर्णाङ्क k = 0;

        यदि( पूर्व_ग्रन्थिः )
        {
            प्रति( ; k < dims; k++ )
                यदि( ग्रन्थिः->idx[k] != पूर्व_ग्रन्थिः->idx[k] )
                    भङ्ग;
            CV_Assert( k < dims );
            यदि( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        प्रति( ; k < dims; k++ )
            writeScalar( fs, ग्रन्थिः->idx[k] );
        पूर्व_ग्रन्थिः = ग्रन्थिः;

        ध्रुव uchar* value = &m.value<uchar>(ग्रन्थिः);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, Mat& m, ध्रुव Mat& default_mat)
{
    यदि( ग्रन्थिः.empty() )
    {
        default_mat.copy_गन्तव्यम्(m);
        प्रत्यागम;
    }

    std::string dt;
    पूर्णाङ्क पङ्क्तिकाः, स्तम्भसङ्ख्या, elem_type;

    पठ(ग्रन्थिः["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पठ(ग्रन्थिः["rows"], पङ्क्तिकाः, -1);
    यदि( पङ्क्तिकाः >= 0 )
    {
        पठ(ग्रन्थिः["cols"], स्तम्भसङ्ख्या, -1);
        m.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, elem_type);
    }
    अन्यथा
    {
        पूर्णाङ्क sizes[CV_MAX_DIM] = {0}, dims;
        File_ग्रन्थिः sizes_ग्रन्थिः = ग्रन्थिः["sizes"];
        CV_Assert( !sizes_ग्रन्थिः.empty() );

        dims = (पूर्णाङ्क)sizes_ग्रन्थिः.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ग्रन्थिः.readRaw("i", sizes, dims*परिमाण(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ग्रन्थिः दत्त_ग्रन्थिः = ग्रन्थिः["data"];
    CV_Assert(!दत्त_ग्रन्थिः.empty());

    size_t nelems = दत्त_ग्रन्थिः.size();
    यदि (nelems == 0)
    {
        m = Mat();
        प्रत्यागम;
    }
    CV_Assert(nelems == m.योग()*m.channels());

    दत्त_ग्रन्थिः.readRaw(dt, (uchar*)m.ptr(), m.योग()*m.elem_आकार());
}

शून्य पठ( ध्रुव File_ग्रन्थिः& ग्रन्थिः, SparseMat& m, ध्रुव SparseMat& default_mat )
{
    यदि( ग्रन्थिः.empty() )
    {
        default_mat.copy_गन्तव्यम्(m);
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

    File_ग्रन्थिः दत्त = ग्रन्थिः["data"];
    CV_Assert( दत्त.isSeq() );

    पूर्णाङ्क cn = CV_MAT_CN(elem_type);
    पूर्णाङ्क idx[CV_MAX_DIM] = {0};
    size_t i, sz = दत्त.size();
    size_t esz = m.elem_आकार();
    File_ग्रन्थिः_Iterator it = दत्त.begin();

    प्रति( i = 0; i < sz; )
    {
        File_ग्रन्थिः n = *it;
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
        uchar* valptr = m.ptr(idx, सत्य);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, कुञ्चिका_Point& value, ध्रुव कुञ्चिका_Point& default_मानम्)
{
    यदि( ग्रन्थिः.empty() )
    {
        value = default_मानम्;
        प्रत्यागम;
    }
    ग्रन्थिः >> value;
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, DMatch& value, ध्रुव DMatch& default_मानम्)
{
    यदि( ग्रन्थिः.empty() )
    {
        value = default_मानम्;
        प्रत्यागम;
    }
    ग्रन्थिः >> value;
}

#यदि_परिभाषितम् CV__LEGACY_PERSISTENCE
शून्य लिख( FileStorage& fs, ध्रुव std::string& name, ध्रुव std::vector<कुञ्चिका_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ग्रन्थिः::SEQ);
    लिख(fs, vec);
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, std::vector<कुञ्चिका_Point>& keypoints)
{
    File_ग्रन्थिः first_ग्रन्थिः = *(ग्रन्थिः.begin());
    यदि (first_ग्रन्थिः.isSeq())
    {
        // modern scheme
#यदि_परिभाषितम् OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        size_t योग = (size_t)it.remaining;
        keypoints.resize(योग);
        प्रति (size_t i = 0; i < योग; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#अन्यथा
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        it >> keypoints;
#यदि_अन्तः
        प्रत्यागम;
    }
    keypoints.clear();
    File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin(), it_अन्तः = ग्रन्थिः.end();
    प्रति( ; it != it_अन्तः; )
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
    यदि (first_ग्रन्थिः.isSeq())
    {
        // modern scheme
#यदि_परिभाषितम् OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        size_t योग = (size_t)it.remaining;
        matches.resize(योग);
        प्रति (size_t i = 0; i < योग; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#अन्यथा
        File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin();
        it >> matches;
#यदि_अन्तः
        प्रत्यागम;
    }
    matches.clear();
    File_ग्रन्थिः_Iterator it = ग्रन्थिः.begin(), it_अन्तः = ग्रन्थिः.end();
    प्रति( ; it != it_अन्तः; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#यदि_अन्तः


}
