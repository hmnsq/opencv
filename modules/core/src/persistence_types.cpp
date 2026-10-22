// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

نطاق cv
{

فراغ اكتب( FileStorage& fs, ثابت String& name, ثابت Mat& m )
{
    حرف dt[22];

    إذا( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_تۈگۈن::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        لكل( صحيح i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    وإلا
    {
        fs.startWriteStruct(name, File_تۈگۈن::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

بنية Sparse_تۈگۈن_Cmp
{
    Sparse_تۈگۈن_Cmp(صحيح _dims) : dims(_dims) {}
    منطقي operator()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
    {
        لكل( صحيح i = 0; i < dims; i++ )
        {
            صحيح d = a->idx[i] - b->idx[i];
            إذا(d)
                أرجع d < 0;
        }
        أرجع خطأ;
    }

    صحيح dims;
};

فراغ اكتب( FileStorage& fs, ثابت String& name, ثابت SparseMat& m )
{
    حرف dt[22];

    fs.startWriteStruct(name, File_تۈگۈن::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    إذا( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    لكل( ; it != it_end; ++it )
    {
        CV_Assert(it.تۈگۈن() != 0);
        elems[i++] = it.تۈگۈن();
    }

    std::sort(elems.begin(), elems.end(), Sparse_تۈگۈن_Cmp(dims));
    ثابت SparseMat::Node* prev_تۈگۈن = 0;
    size_t esz = m.elemSize();

    لكل( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* تۈگۈن = elems[i];
        صحيح k = 0;

        إذا( prev_تۈگۈن )
        {
            لكل( ; k < dims; k++ )
                إذا( تۈگۈن->idx[k] != prev_تۈگۈن->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            إذا( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        لكل( ; k < dims; k++ )
            writeScalar( fs, تۈگۈن->idx[k] );
        prev_تۈگۈن = تۈگۈن;

        ثابت uchar* value = &m.value<uchar>(تۈگۈن);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت File_تۈگۈن& تۈگۈن, Mat& m, ثابت Mat& default_mat)
{
    إذا( تۈگۈن.empty() )
    {
        default_mat.copyTo(m);
        أرجع;
    }

    std::string dt;
    صحيح rows, cols, elem_type;

    اقرأ(تۈگۈن["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(تۈگۈن["rows"], rows, -1);
    إذا( rows >= 0 )
    {
        اقرأ(تۈگۈن["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        File_تۈگۈن sizes_تۈگۈن = تۈگۈن["sizes"];
        CV_Assert( !sizes_تۈگۈن.empty() );

        dims = (صحيح)sizes_تۈگۈن.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_تۈگۈن.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_تۈگۈن data_تۈگۈن = تۈگۈن["data"];
    CV_Assert(!data_تۈگۈن.empty());

    size_t nelems = data_تۈگۈن.size();
    إذا (nelems == 0)
    {
        m = Mat();
        أرجع;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_تۈگۈن.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

فراغ اقرأ( ثابت File_تۈگۈن& تۈگۈن, SparseMat& m, ثابت SparseMat& default_mat )
{
    إذا( تۈگۈن.empty() )
    {
        default_mat.copyTo(m);
        أرجع;
    }

    std::string dt;
    اقرأ(تۈگۈن["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    File_تۈگۈن sizes_تۈگۈن = تۈگۈن["sizes"];
    CV_Assert( !sizes_تۈگۈن.empty() );

    صحيح dims = (صحيح)sizes_تۈگۈن.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_تۈگۈن.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_تۈگۈن data = تۈگۈن["data"];
    CV_Assert( data.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_تۈگۈن_Iterator it = data.begin();

    لكل( i = 0; i < sz; )
    {
        File_تۈگۈن n = *it;
        صحيح k = (صحيح)n;
        إذا( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        وإلا
        {
            إذا( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            وإلا
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            لكل( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                صحيح idx_k = (صحيح)n;
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

فراغ اقرأ(ثابت File_تۈگۈن& تۈگۈن, ئاچقۇچ_Point& value, ثابت ئاچقۇچ_Point& default_قىممەت)
{
    إذا( تۈگۈن.empty() )
    {
        value = default_قىممەت;
        أرجع;
    }
    تۈگۈن >> value;
}

فراغ اقرأ(ثابت File_تۈگۈن& تۈگۈن, DMatch& value, ثابت DMatch& default_قىممەت)
{
    إذا( تۈگۈن.empty() )
    {
        value = default_قىممەت;
        أرجع;
    }
    تۈگۈن >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<ئاچقۇچ_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_تۈگۈن::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_تۈگۈن& تۈگۈن, std::vector<ئاچقۇچ_Point>& keypoints)
{
    File_تۈگۈن first_تۈگۈن = *(تۈگۈن.begin());
    إذا (first_تۈگۈن.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_تۈگۈن_Iterator it = تۈگۈن.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        لكل (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وإلا
        File_تۈگۈن_Iterator it = تۈگۈن.begin();
        it >> keypoints;
#endif
        أرجع;
    }
    keypoints.clear();
    File_تۈگۈن_Iterator it = تۈگۈن.begin(), it_end = تۈگۈن.end();
    لكل( ; it != it_end; )
    {
        ئاچقۇچ_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_تۈگۈن::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_تۈگۈن& تۈگۈن, std::vector<DMatch>& matches)
{
    File_تۈگۈن first_تۈگۈن = *(تۈگۈن.begin());
    إذا (first_تۈگۈن.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_تۈگۈن_Iterator it = تۈگۈن.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        لكل (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وإلا
        File_تۈگۈن_Iterator it = تۈگۈن.begin();
        it >> matches;
#endif
        أرجع;
    }
    matches.clear();
    File_تۈگۈن_Iterator it = تۈگۈن.begin(), it_end = تۈگۈن.end();
    لكل( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
