// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

فضاینام cv
{

تهی اكتب( FileStorage& fs, ثابت String& name, ثابت Mat& m )
{
    نویسه dt[22];

    اگر( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_گره::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, اندازه(dt) );
        fs << "data" << "[:";
        برای( صحیح i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    وگرنه
    {
        fs.startWriteStruct(name, File_گره::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*اندازه(صحیح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, اندازه(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

ساختار Sparse_گره_Cmp
{
    Sparse_گره_Cmp(صحیح _dims) : dims(_dims) {}
    منطقی operator()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
    {
        برای( صحیح i = 0; i < dims; i++ )
        {
            صحیح d = a->idx[i] - b->idx[i];
            اگر(d)
                بازگشت d < 0;
        }
        بازگشت نادرست;
    }

    صحیح dims;
};

تهی اكتب( FileStorage& fs, ثابت String& name, ثابت SparseMat& m )
{
    نویسه dt[22];

    fs.startWriteStruct(name, File_گره::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحیح dims = m.dims();
    اگر( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*اندازه(صحیح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, اندازه(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    برای( ; it != it_end; ++it )
    {
        CV_Assert(it.گره() != 0);
        elems[i++] = it.گره();
    }

    std::sort(elems.begin(), elems.end(), Sparse_گره_Cmp(dims));
    ثابت SparseMat::Node* prev_گره = 0;
    size_t esz = m.elemSize();

    برای( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* گره = elems[i];
        صحیح k = 0;

        اگر( prev_گره )
        {
            برای( ; k < dims; k++ )
                اگر( گره->idx[k] != prev_گره->idx[k] )
                    بشکن;
            CV_Assert( k < dims );
            اگر( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        برای( ; k < dims; k++ )
            writeScalar( fs, گره->idx[k] );
        prev_گره = گره;

        ثابت uchar* value = &m.value<uchar>(گره);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

تهی اقرأ(ثابت File_گره& گره, Mat& m, ثابت Mat& default_mat)
{
    اگر( گره.empty() )
    {
        default_mat.copyTo(m);
        بازگشت;
    }

    std::string dt;
    صحیح rows, cols, elem_type;

    اقرأ(گره["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(گره["rows"], rows, -1);
    اگر( rows >= 0 )
    {
        اقرأ(گره["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    وگرنه
    {
        صحیح sizes[CV_MAX_DIM] = {0}, dims;
        File_گره sizes_گره = گره["sizes"];
        CV_Assert( !sizes_گره.empty() );

        dims = (صحیح)sizes_گره.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_گره.readRaw("i", sizes, dims*اندازه(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_گره data_گره = گره["data"];
    CV_Assert(!data_گره.empty());

    size_t nelems = data_گره.size();
    اگر (nelems == 0)
    {
        m = Mat();
        بازگشت;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_گره.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

تهی اقرأ( ثابت File_گره& گره, SparseMat& m, ثابت SparseMat& default_mat )
{
    اگر( گره.empty() )
    {
        default_mat.copyTo(m);
        بازگشت;
    }

    std::string dt;
    اقرأ(گره["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحیح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحیح sizes[CV_MAX_DIM] = {0};
    File_گره sizes_گره = گره["sizes"];
    CV_Assert( !sizes_گره.empty() );

    صحیح dims = (صحیح)sizes_گره.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_گره.readRaw("i", sizes, dims*اندازه(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_گره data = گره["data"];
    CV_Assert( data.isSeq() );

    صحیح cn = CV_MAT_CN(elem_type);
    صحیح idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_گره_Iterator it = data.begin();

    برای( i = 0; i < sz; )
    {
        File_گره n = *it;
        صحیح k = (صحیح)n;
        اگر( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        وگرنه
        {
            اگر( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            وگرنه
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            برای( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                صحیح idx_k = (صحیح)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, درست);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

تهی اقرأ(ثابت File_گره& گره, کلید_Point& value, ثابت کلید_Point& default_مقدار)
{
    اگر( گره.empty() )
    {
        value = default_مقدار;
        بازگشت;
    }
    گره >> value;
}

تهی اقرأ(ثابت File_گره& گره, DMatch& value, ثابت DMatch& default_مقدار)
{
    اگر( گره.empty() )
    {
        value = default_مقدار;
        بازگشت;
    }
    گره >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
تهی اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<کلید_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گره::SEQ);
    اكتب(fs, vec);
}

تهی اقرأ(ثابت File_گره& گره, std::vector<کلید_Point>& keypoints)
{
    File_گره first_گره = *(گره.begin());
    اگر (first_گره.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گره_Iterator it = گره.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        برای (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وگرنه
        File_گره_Iterator it = گره.begin();
        it >> keypoints;
#endif
        بازگشت;
    }
    keypoints.clear();
    File_گره_Iterator it = گره.begin(), it_end = گره.end();
    برای( ; it != it_end; )
    {
        کلید_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

تهی اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گره::SEQ);
    اكتب(fs, vec);
}

تهی اقرأ(ثابت File_گره& گره, std::vector<DMatch>& matches)
{
    File_گره first_گره = *(گره.begin());
    اگر (first_گره.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گره_Iterator it = گره.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        برای (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وگرنه
        File_گره_Iterator it = گره.begin();
        it >> matches;
#endif
        بازگشت;
    }
    matches.clear();
    File_گره_Iterator it = گره.begin(), it_end = گره.end();
    برای( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
