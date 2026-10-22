// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence.hpp"

நாமாகாஶ cv
{

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, மாறிலி Mat& m )
{
    எழுத்து dt[22];

    எனில்( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_முனை::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, அளவு(dt) );
        fs << "data" << "[:";
        க்கு( முழு i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    இல்லையெனில்
    {
        fs.startWriteStruct(name, File_முனை::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*அளவு(முழு) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, அளவு(dt) );
        fs << "data" << "[:";
        மாறிலி Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

கட்டமைப்பு Sparse_முனை_Cmp
{
    Sparse_முனை_Cmp(முழு _dims) : dims(_dims) {}
    தருக்க operator()(மாறிலி SparseMat::Node* a, மாறிலி SparseMat::Node* b)
    {
        க்கு( முழு i = 0; i < dims; i++ )
        {
            முழு d = a->idx[i] - b->idx[i];
            எனில்(d)
                திரும்பு d < 0;
        }
        திரும்பு பொய்;
    }

    முழு dims;
};

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, மாறிலி SparseMat& m )
{
    எழுத்து dt[22];

    fs.startWriteStruct(name, File_முனை::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    முழு dims = m.dims();
    எனில்( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*அளவு(முழு) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, அளவு(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<மாறிலி SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    க்கு( ; it != it_end; ++it )
    {
        CV_Assert(it.முனை() != 0);
        elems[i++] = it.முனை();
    }

    std::sort(elems.begin(), elems.end(), Sparse_முனை_Cmp(dims));
    மாறிலி SparseMat::Node* prev_முனை = 0;
    size_t esz = m.elemSize();

    க்கு( i = 0; i < n; i++ )
    {
        மாறிலி SparseMat::Node* முனை = elems[i];
        முழு k = 0;

        எனில்( prev_முனை )
        {
            க்கு( ; k < dims; k++ )
                எனில்( முனை->idx[k] != prev_முனை->idx[k] )
                    நிறுத்து;
            CV_Assert( k < dims );
            எனில்( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        க்கு( ; k < dims; k++ )
            writeScalar( fs, முனை->idx[k] );
        prev_முனை = முனை;

        மாறிலி uchar* value = &m.value<uchar>(முனை);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

வெறுமை பட(மாறிலி File_முனை& முனை, Mat& m, மாறிலி Mat& default_mat)
{
    எனில்( முனை.empty() )
    {
        default_mat.copyTo(m);
        திரும்பு;
    }

    std::string dt;
    முழு rows, cols, elem_type;

    பட(முனை["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    பட(முனை["rows"], rows, -1);
    எனில்( rows >= 0 )
    {
        பட(முனை["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    இல்லையெனில்
    {
        முழு sizes[CV_MAX_DIM] = {0}, dims;
        File_முனை sizes_முனை = முனை["sizes"];
        CV_Assert( !sizes_முனை.empty() );

        dims = (முழு)sizes_முனை.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_முனை.readRaw("i", sizes, dims*அளவு(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_முனை data_முனை = முனை["data"];
    CV_Assert(!data_முனை.empty());

    size_t nelems = data_முனை.size();
    எனில் (nelems == 0)
    {
        m = Mat();
        திரும்பு;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_முனை.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

வெறுமை பட( மாறிலி File_முனை& முனை, SparseMat& m, மாறிலி SparseMat& default_mat )
{
    எனில்( முனை.empty() )
    {
        default_mat.copyTo(m);
        திரும்பு;
    }

    std::string dt;
    பட(முனை["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    முழு elem_type = fs::decodeSimpleFormat( dt.c_str() );

    முழு sizes[CV_MAX_DIM] = {0};
    File_முனை sizes_முனை = முனை["sizes"];
    CV_Assert( !sizes_முனை.empty() );

    முழு dims = (முழு)sizes_முனை.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_முனை.readRaw("i", sizes, dims*அளவு(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_முனை data = முனை["data"];
    CV_Assert( data.isSeq() );

    முழு cn = CV_MAT_CN(elem_type);
    முழு idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_முனை_Iterator it = data.begin();

    க்கு( i = 0; i < sz; )
    {
        File_முனை n = *it;
        முழு k = (முழு)n;
        எனில்( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        இல்லையெனில்
        {
            எனில்( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            இல்லையெனில்
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            க்கு( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                முழு idx_k = (முழு)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, உண்மை);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

வெறுமை பட(மாறிலி File_முனை& முனை, சாவி_Point& value, மாறிலி சாவி_Point& default_மதிப்பு)
{
    எனில்( முனை.empty() )
    {
        value = default_மதிப்பு;
        திரும்பு;
    }
    முனை >> value;
}

வெறுமை பட(மாறிலி File_முனை& முனை, DMatch& value, மாறிலி DMatch& default_மதிப்பு)
{
    எனில்( முனை.empty() )
    {
        value = default_மதிப்பு;
        திரும்பு;
    }
    முனை >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
வெறுமை லிக( FileStorage& fs, மாறிலி std::string& name, மாறிலி std::vector<சாவி_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_முனை::SEQ);
    லிக(fs, vec);
}

வெறுமை பட(மாறிலி File_முனை& முனை, std::vector<சாவி_Point>& keypoints)
{
    File_முனை first_முனை = *(முனை.begin());
    எனில் (first_முனை.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_முனை_Iterator it = முனை.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        க்கு (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#இல்லையெனில்
        File_முனை_Iterator it = முனை.begin();
        it >> keypoints;
#endif
        திரும்பு;
    }
    keypoints.clear();
    File_முனை_Iterator it = முனை.begin(), it_end = முனை.end();
    க்கு( ; it != it_end; )
    {
        சாவி_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

வெறுமை லிக( FileStorage& fs, மாறிலி std::string& name, மாறிலி std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_முனை::SEQ);
    லிக(fs, vec);
}

வெறுமை பட(மாறிலி File_முனை& முனை, std::vector<DMatch>& matches)
{
    File_முனை first_முனை = *(முனை.begin());
    எனில் (first_முனை.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_முனை_Iterator it = முனை.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        க்கு (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#இல்லையெனில்
        File_முனை_Iterator it = முனை.begin();
        it >> matches;
#endif
        திரும்பு;
    }
    matches.clear();
    File_முனை_Iterator it = முனை.begin(), it_end = முனை.end();
    க்கு( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
