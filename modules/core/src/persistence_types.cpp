// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence.hpp"

ନାମାକାଶ cv
{

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ଅଚଳ Mat& m )
{
    ଅକ୍ଷର dt[22];

    ଯଦି( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_ଗଣ୍ଠି::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, ଆକାର(dt) );
        fs << "data" << "[:";
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    ଅନ୍ୟଥା
    {
        fs.startWriteStruct(name, File_ଗଣ୍ଠି::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, ଆକାର(dt) );
        fs << "data" << "[:";
        ଅଚଳ Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

ସଂରଚନା Sparse_ଗଣ୍ଠି_Cmp
{
    Sparse_ଗଣ୍ଠି_Cmp(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims) : dims(_dims) {}
    ତାର୍କିକ operator()(ଅଚଳ SparseMat::Node* a, ଅଚଳ SparseMat::Node* b)
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = a->idx[i] - b->idx[i];
            ଯଦି(d)
                ଫେରନ୍ତୁ d < 0;
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims;
};

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ଅଚଳ SparseMat& m )
{
    ଅକ୍ଷର dt[22];

    fs.startWriteStruct(name, File_ଗଣ୍ଠି::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = m.dims();
    ଯଦି( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, ଆକାର(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ଅଚଳ SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    ପାଇଁ( ; it != it_end; ++it )
    {
        CV_Assert(it.ଗଣ୍ଠି() != 0);
        elems[i++] = it.ଗଣ୍ଠି();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ଗଣ୍ଠି_Cmp(dims));
    ଅଚଳ SparseMat::Node* prev_ଗଣ୍ଠି = 0;
    size_t esz = m.elemSize();

    ପାଇଁ( i = 0; i < n; i++ )
    {
        ଅଚଳ SparseMat::Node* ଗଣ୍ଠି = elems[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;

        ଯଦି( prev_ଗଣ୍ଠି )
        {
            ପାଇଁ( ; k < dims; k++ )
                ଯଦି( ଗଣ୍ଠି->idx[k] != prev_ଗଣ୍ଠି->idx[k] )
                    ଭାଙ୍ଗ;
            CV_Assert( k < dims );
            ଯଦି( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        ପାଇଁ( ; k < dims; k++ )
            writeScalar( fs, ଗଣ୍ଠି->idx[k] );
        prev_ଗଣ୍ଠି = ଗଣ୍ଠି;

        ଅଚଳ uchar* value = &m.value<uchar>(ଗଣ୍ଠି);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, Mat& m, ଅଚଳ Mat& default_mat)
{
    ଯଦି( ଗଣ୍ଠି.empty() )
    {
        default_mat.copyTo(m);
        ଫେରନ୍ତୁ;
    }

    std::string dt;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, cols, elem_type;

    ପଠ(ଗଣ୍ଠି["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ପଠ(ଗଣ୍ଠି["rows"], rows, -1);
    ଯଦି( rows >= 0 )
    {
        ପଠ(ଗଣ୍ଠି["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[CV_MAX_DIM] = {0}, dims;
        File_ଗଣ୍ଠି sizes_ଗଣ୍ଠି = ଗଣ୍ଠି["sizes"];
        CV_Assert( !sizes_ଗଣ୍ଠି.empty() );

        dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sizes_ଗଣ୍ଠି.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ଗଣ୍ଠି.readRaw("i", sizes, dims*ଆକାର(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ଗଣ୍ଠି data_ଗଣ୍ଠି = ଗଣ୍ଠି["data"];
    CV_Assert(!data_ଗଣ୍ଠି.empty());

    size_t nelems = data_ଗଣ୍ଠି.size();
    ଯଦି (nelems == 0)
    {
        m = Mat();
        ଫେରନ୍ତୁ;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_ଗଣ୍ଠି.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

ଶୂନ୍ୟ ପଠ( ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, SparseMat& m, ଅଚଳ SparseMat& default_mat )
{
    ଯଦି( ଗଣ୍ଠି.empty() )
    {
        default_mat.copyTo(m);
        ଫେରନ୍ତୁ;
    }

    std::string dt;
    ପଠ(ଗଣ୍ଠି["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[CV_MAX_DIM] = {0};
    File_ଗଣ୍ଠି sizes_ଗଣ୍ଠି = ଗଣ୍ଠି["sizes"];
    CV_Assert( !sizes_ଗଣ୍ଠି.empty() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sizes_ଗଣ୍ଠି.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ଗଣ୍ଠି.readRaw("i", sizes, dims*ଆକାର(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ଗଣ୍ଠି data = ଗଣ୍ଠି["data"];
    CV_Assert( data.isSeq() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(elem_type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_ଗଣ୍ଠି_Iterator it = data.begin();

    ପାଇଁ( i = 0; i < sz; )
    {
        File_ଗଣ୍ଠି n = *it;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n;
        ଯଦି( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ଅନ୍ୟଥା
        {
            ଯଦି( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ଅନ୍ୟଥା
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            ପାଇଁ( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx_k = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, ସତ୍ୟ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, ଚାବି_Point& value, ଅଚଳ ଚାବି_Point& default_ମୂଲ୍ୟ)
{
    ଯଦି( ଗଣ୍ଠି.empty() )
    {
        value = default_ମୂଲ୍ୟ;
        ଫେରନ୍ତୁ;
    }
    ଗଣ୍ଠି >> value;
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, DMatch& value, ଅଚଳ DMatch& default_ମୂଲ୍ୟ)
{
    ଯଦି( ଗଣ୍ଠି.empty() )
    {
        value = default_ମୂଲ୍ୟ;
        ଫେରନ୍ତୁ;
    }
    ଗଣ୍ଠି >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ std::string& name, ଅଚଳ std::vector<ଚାବି_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ଗଣ୍ଠି::SEQ);
    ଲିଖ(fs, vec);
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, std::vector<ଚାବି_Point>& keypoints)
{
    File_ଗଣ୍ଠି first_ଗଣ୍ଠି = *(ଗଣ୍ଠି.begin());
    ଯଦି (first_ଗଣ୍ଠି.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        ପାଇଁ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ଅନ୍ୟଥା
        File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin();
        it >> keypoints;
#endif
        ଫେରନ୍ତୁ;
    }
    keypoints.clear();
    File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin(), it_end = ଗଣ୍ଠି.end();
    ପାଇଁ( ; it != it_end; )
    {
        ଚାବି_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ std::string& name, ଅଚଳ std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ଗଣ୍ଠି::SEQ);
    ଲିଖ(fs, vec);
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, std::vector<DMatch>& matches)
{
    File_ଗଣ୍ଠି first_ଗଣ୍ଠି = *(ଗଣ୍ଠି.begin());
    ଯଦି (first_ଗଣ୍ଠି.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        ପାଇଁ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ଅନ୍ୟଥା
        File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin();
        it >> matches;
#endif
        ଫେରନ୍ତୁ;
    }
    matches.clear();
    File_ଗଣ୍ଠି_Iterator it = ଗଣ୍ଠି.begin(), it_end = ଗଣ୍ଠି.end();
    ପାଇଁ( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
