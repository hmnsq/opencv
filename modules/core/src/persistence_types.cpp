// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

नामाकाश cv
{

शून्य लिख( FileStorage& fs, स्थिरांक String& name, स्थिरांक Mat& m )
{
    अक्षर dt[22];

    जर( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_गाठ::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, आकार(dt) );
        fs << "data" << "[:";
        साठी( पूर्णांक i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    अन्यथा
    {
        fs.startWriteStruct(name, File_गाठ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*आकार(पूर्णांक) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, आकार(dt) );
        fs << "data" << "[:";
        स्थिरांक Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        माप_प्रकार total = it.size*m.elemSize();

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

संरचना Sparse_गाठ_Cmp
{
    Sparse_गाठ_Cmp(पूर्णांक _dims) : dims(_dims) {}
    तार्किक चालक()(स्थिरांक SparseMat::Node* a, स्थिरांक SparseMat::Node* b)
    {
        साठी( पूर्णांक i = 0; i < dims; i++ )
        {
            पूर्णांक d = a->idx[i] - b->idx[i];
            जर(d)
                परत d < 0;
        }
        परत असत्य;
    }

    पूर्णांक dims;
};

शून्य लिख( FileStorage& fs, स्थिरांक String& name, स्थिरांक SparseMat& m )
{
    अक्षर dt[22];

    fs.startWriteStruct(name, File_गाठ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    पूर्णांक dims = m.dims();
    जर( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*आकार(पूर्णांक) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, आकार(dt) );
    fs << "data" << "[:";

    माप_प्रकार i = 0, n = m.nzcount();
    AutoBuffer<स्थिरांक SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    साठी( ; it != it_end; ++it )
    {
        CV_Assert(it.गाठ() != 0);
        elems[i++] = it.गाठ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_गाठ_Cmp(dims));
    स्थिरांक SparseMat::Node* prev_गाठ = 0;
    माप_प्रकार esz = m.elemSize();

    साठी( i = 0; i < n; i++ )
    {
        स्थिरांक SparseMat::Node* गाठ = elems[i];
        पूर्णांक k = 0;

        जर( prev_गाठ )
        {
            साठी( ; k < dims; k++ )
                जर( गाठ->idx[k] != prev_गाठ->idx[k] )
                    थांबा;
            CV_Assert( k < dims );
            जर( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        साठी( ; k < dims; k++ )
            writeScalar( fs, गाठ->idx[k] );
        prev_गाठ = गाठ;

        स्थिरांक uchar* value = &m.value<uchar>(गाठ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, Mat& m, स्थिरांक Mat& default_mat)
{
    जर( गाठ.empty() )
    {
        default_mat.copyTo(m);
        परत;
    }

    std::string dt;
    पूर्णांक rows, cols, elem_type;

    पठ(गाठ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पठ(गाठ["rows"], rows, -1);
    जर( rows >= 0 )
    {
        पठ(गाठ["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    अन्यथा
    {
        पूर्णांक sizes[CV_MAX_DIM] = {0}, dims;
        File_गाठ sizes_गाठ = गाठ["sizes"];
        CV_Assert( !sizes_गाठ.empty() );

        dims = (पूर्णांक)sizes_गाठ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_गाठ.readRaw("i", sizes, dims*आकार(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_गाठ data_गाठ = गाठ["data"];
    CV_Assert(!data_गाठ.empty());

    माप_प्रकार nelems = data_गाठ.size();
    जर (nelems == 0)
    {
        m = Mat();
        परत;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_गाठ.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

शून्य पठ( स्थिरांक File_गाठ& गाठ, SparseMat& m, स्थिरांक SparseMat& default_mat )
{
    जर( गाठ.empty() )
    {
        default_mat.copyTo(m);
        परत;
    }

    std::string dt;
    पठ(गाठ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    पूर्णांक elem_type = fs::decodeSimpleFormat( dt.c_str() );

    पूर्णांक sizes[CV_MAX_DIM] = {0};
    File_गाठ sizes_गाठ = गाठ["sizes"];
    CV_Assert( !sizes_गाठ.empty() );

    पूर्णांक dims = (पूर्णांक)sizes_गाठ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_गाठ.readRaw("i", sizes, dims*आकार(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_गाठ data = गाठ["data"];
    CV_Assert( data.isSeq() );

    पूर्णांक cn = CV_MAT_CN(elem_type);
    पूर्णांक idx[CV_MAX_DIM] = {0};
    माप_प्रकार i, sz = data.size();
    माप_प्रकार esz = m.elemSize();
    File_गाठ_Iterator it = data.begin();

    साठी( i = 0; i < sz; )
    {
        File_गाठ n = *it;
        पूर्णांक k = (पूर्णांक)n;
        जर( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        अन्यथा
        {
            जर( i > 0 )
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
            साठी( ; k < dims; k++ )
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
        uchar* valptr = m.ptr(idx, सत्य);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, किल्ली_Point& value, स्थिरांक किल्ली_Point& default_मूल्य)
{
    जर( गाठ.empty() )
    {
        value = default_मूल्य;
        परत;
    }
    गाठ >> value;
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, DMatch& value, स्थिरांक DMatch& default_मूल्य)
{
    जर( गाठ.empty() )
    {
        value = default_मूल्य;
        परत;
    }
    गाठ >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
शून्य लिख( FileStorage& fs, स्थिरांक std::string& name, स्थिरांक std::vector<किल्ली_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_गाठ::SEQ);
    लिख(fs, vec);
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, std::vector<किल्ली_Point>& keypoints)
{
    File_गाठ first_गाठ = *(गाठ.begin());
    जर (first_गाठ.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाठ_Iterator it = गाठ.begin();
        माप_प्रकार total = (माप_प्रकार)it.remaining;
        keypoints.resize(total);
        साठी (माप_प्रकार i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#अन्यथा
        File_गाठ_Iterator it = गाठ.begin();
        it >> keypoints;
#endif
        परत;
    }
    keypoints.clear();
    File_गाठ_Iterator it = गाठ.begin(), it_end = गाठ.end();
    साठी( ; it != it_end; )
    {
        किल्ली_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

शून्य लिख( FileStorage& fs, स्थिरांक std::string& name, स्थिरांक std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_गाठ::SEQ);
    लिख(fs, vec);
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, std::vector<DMatch>& matches)
{
    File_गाठ first_गाठ = *(गाठ.begin());
    जर (first_गाठ.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_गाठ_Iterator it = गाठ.begin();
        माप_प्रकार total = (माप_प्रकार)it.remaining;
        matches.resize(total);
        साठी (माप_प्रकार i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#अन्यथा
        File_गाठ_Iterator it = गाठ.begin();
        it >> matches;
#endif
        परत;
    }
    matches.clear();
    File_गाठ_Iterator it = गाठ.begin(), it_end = गाठ.end();
    साठी( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
