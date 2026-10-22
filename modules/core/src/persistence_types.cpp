// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence.hpp"

નામાકાશ cv
{

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, અચલ Mat& m )
{
    અક્ષર dt[22];

    જો( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_ગાંઠ::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, કદ(dt) );
        fs << "data" << "[:";
        માટે( પૂર્ણાંક i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    નહીંતર
    {
        fs.startWriteStruct(name, File_ગાંઠ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*કદ(પૂર્ણાંક) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, કદ(dt) );
        fs << "data" << "[:";
        અચલ Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

સંરચના Sparse_ગાંઠ_Cmp
{
    Sparse_ગાંઠ_Cmp(પૂર્ણાંક _dims) : dims(_dims) {}
    તાર્કિક operator()(અચલ SparseMat::Node* a, અચલ SparseMat::Node* b)
    {
        માટે( પૂર્ણાંક i = 0; i < dims; i++ )
        {
            પૂર્ણાંક d = a->idx[i] - b->idx[i];
            જો(d)
                પાછા d < 0;
        }
        પાછા ખોટું;
    }

    પૂર્ણાંક dims;
};

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, અચલ SparseMat& m )
{
    અક્ષર dt[22];

    fs.startWriteStruct(name, File_ગાંઠ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    પૂર્ણાંક dims = m.dims();
    જો( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*કદ(પૂર્ણાંક) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, કદ(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<અચલ SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    માટે( ; it != it_end; ++it )
    {
        CV_Assert(it.ગાંઠ() != 0);
        elems[i++] = it.ગાંઠ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ગાંઠ_Cmp(dims));
    અચલ SparseMat::Node* prev_ગાંઠ = 0;
    size_t esz = m.elemSize();

    માટે( i = 0; i < n; i++ )
    {
        અચલ SparseMat::Node* ગાંઠ = elems[i];
        પૂર્ણાંક k = 0;

        જો( prev_ગાંઠ )
        {
            માટે( ; k < dims; k++ )
                જો( ગાંઠ->idx[k] != prev_ગાંઠ->idx[k] )
                    તોડો;
            CV_Assert( k < dims );
            જો( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        માટે( ; k < dims; k++ )
            writeScalar( fs, ગાંઠ->idx[k] );
        prev_ગાંઠ = ગાંઠ;

        અચલ uchar* value = &m.value<uchar>(ગાંઠ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, Mat& m, અચલ Mat& default_mat)
{
    જો( ગાંઠ.empty() )
    {
        default_mat.copyTo(m);
        પાછા;
    }

    std::string dt;
    પૂર્ણાંક rows, cols, elem_type;

    પઠ(ગાંઠ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    પઠ(ગાંઠ["rows"], rows, -1);
    જો( rows >= 0 )
    {
        પઠ(ગાંઠ["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    નહીંતર
    {
        પૂર્ણાંક sizes[CV_MAX_DIM] = {0}, dims;
        File_ગાંઠ sizes_ગાંઠ = ગાંઠ["sizes"];
        CV_Assert( !sizes_ગાંઠ.empty() );

        dims = (પૂર્ણાંક)sizes_ગાંઠ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ગાંઠ.readRaw("i", sizes, dims*કદ(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ગાંઠ data_ગાંઠ = ગાંઠ["data"];
    CV_Assert(!data_ગાંઠ.empty());

    size_t nelems = data_ગાંઠ.size();
    જો (nelems == 0)
    {
        m = Mat();
        પાછા;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_ગાંઠ.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

શૂન્ય પઠ( અચલ File_ગાંઠ& ગાંઠ, SparseMat& m, અચલ SparseMat& default_mat )
{
    જો( ગાંઠ.empty() )
    {
        default_mat.copyTo(m);
        પાછા;
    }

    std::string dt;
    પઠ(ગાંઠ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    પૂર્ણાંક elem_type = fs::decodeSimpleFormat( dt.c_str() );

    પૂર્ણાંક sizes[CV_MAX_DIM] = {0};
    File_ગાંઠ sizes_ગાંઠ = ગાંઠ["sizes"];
    CV_Assert( !sizes_ગાંઠ.empty() );

    પૂર્ણાંક dims = (પૂર્ણાંક)sizes_ગાંઠ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ગાંઠ.readRaw("i", sizes, dims*કદ(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ગાંઠ data = ગાંઠ["data"];
    CV_Assert( data.isSeq() );

    પૂર્ણાંક cn = CV_MAT_CN(elem_type);
    પૂર્ણાંક idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_ગાંઠ_Iterator it = data.begin();

    માટે( i = 0; i < sz; )
    {
        File_ગાંઠ n = *it;
        પૂર્ણાંક k = (પૂર્ણાંક)n;
        જો( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        નહીંતર
        {
            જો( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            નહીંતર
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            માટે( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                પૂર્ણાંક idx_k = (પૂર્ણાંક)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, સાચું);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, ચાવી_Point& value, અચલ ચાવી_Point& default_કિંમત)
{
    જો( ગાંઠ.empty() )
    {
        value = default_કિંમત;
        પાછા;
    }
    ગાંઠ >> value;
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, DMatch& value, અચલ DMatch& default_કિંમત)
{
    જો( ગાંઠ.empty() )
    {
        value = default_કિંમત;
        પાછા;
    }
    ગાંઠ >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
શૂન્ય લિખ( FileStorage& fs, અચલ std::string& name, અચલ std::vector<ચાવી_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ગાંઠ::SEQ);
    લિખ(fs, vec);
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, std::vector<ચાવી_Point>& keypoints)
{
    File_ગાંઠ first_ગાંઠ = *(ગાંઠ.begin());
    જો (first_ગાંઠ.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ગાંઠ_Iterator it = ગાંઠ.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        માટે (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#નહીંતર
        File_ગાંઠ_Iterator it = ગાંઠ.begin();
        it >> keypoints;
#endif
        પાછા;
    }
    keypoints.clear();
    File_ગાંઠ_Iterator it = ગાંઠ.begin(), it_end = ગાંઠ.end();
    માટે( ; it != it_end; )
    {
        ચાવી_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

શૂન્ય લિખ( FileStorage& fs, અચલ std::string& name, અચલ std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ગાંઠ::SEQ);
    લિખ(fs, vec);
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, std::vector<DMatch>& matches)
{
    File_ગાંઠ first_ગાંઠ = *(ગાંઠ.begin());
    જો (first_ગાંઠ.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ગાંઠ_Iterator it = ગાંઠ.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        માટે (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#નહીંતર
        File_ગાંઠ_Iterator it = ગાંઠ.begin();
        it >> matches;
#endif
        પાછા;
    }
    matches.clear();
    File_ગાંઠ_Iterator it = ગાંઠ.begin(), it_end = ગાંઠ.end();
    માટે( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
