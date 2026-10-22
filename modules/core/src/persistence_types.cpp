// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਅਚਲ Mat& m )
{
    ਅੱਖਰ dt[22];

    ਜੇ( m.dims == 2 || m.empty() )
    {
        fs.ਸ਼ੁਰੂ_Write_Struct(name, File_ਗੰਢ::MAP, String("opencv-matrix"));
        fs << "rows" << m.ਪੰਕਤੀਆਂ;
        fs << "cols" << m.ਖਾਨੇ_ਗਿਣਤੀ;
        fs << "dt" << fs::encodeFormat( m.type(), dt, ਆਕਾਰ(dt) );
        fs << "data" << "[:";
        ਲਈ( ਪੂਰਨ i = 0; i < m.ਪੰਕਤੀਆਂ; i++ )
            fs.writeRaw(dt, m.ptr(i), m.ਖਾਨੇ_ਗਿਣਤੀ*m.elem_ਮਾਪ());
        fs << "]";
        fs.ਸਿਰਾ_Write_Struct();
    }
    ਨਹੀਂਤਾਂ
    {
        fs.ਸ਼ੁਰੂ_Write_Struct(name, File_ਗੰਢ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*ਆਕਾਰ(ਪੂਰਨ) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, ਆਕਾਰ(dt) );
        fs << "data" << "[:";
        ਅਚਲ Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t ਕੁੱਲ = it.size*m.elem_ਮਾਪ();

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], ਕੁੱਲ );
        fs << "]";
        fs.ਸਿਰਾ_Write_Struct();
    }
}

ਬਣਤਰ Sparse_ਗੰਢ_Cmp
{
    Sparse_ਗੰਢ_Cmp(ਪੂਰਨ _dims) : dims(_dims) {}
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ()(ਅਚਲ SparseMat::Node* a, ਅਚਲ SparseMat::Node* b)
    {
        ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
        {
            ਪੂਰਨ d = a->idx[i] - b->idx[i];
            ਜੇ(d)
                ਵਾਪਸ d < 0;
        }
        ਵਾਪਸ ਝੂਠ;
    }

    ਪੂਰਨ dims;
};

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਅਚਲ SparseMat& m )
{
    ਅੱਖਰ dt[22];

    fs.ਸ਼ੁਰੂ_Write_Struct(name, File_ਗੰਢ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ਪੂਰਨ dims = m.dims();
    ਜੇ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*ਆਕਾਰ(ਪੂਰਨ) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, ਆਕਾਰ(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_ਬਫਰ<ਅਚਲ SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_ਸਿਰਾ = m.end();

    ਲਈ( ; it != it_ਸਿਰਾ; ++it )
    {
        CV_Assert(it.ਗੰਢ() != 0);
        elems[i++] = it.ਗੰਢ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ਗੰਢ_Cmp(dims));
    ਅਚਲ SparseMat::Node* prev_ਗੰਢ = 0;
    size_t esz = m.elem_ਮਾਪ();

    ਲਈ( i = 0; i < n; i++ )
    {
        ਅਚਲ SparseMat::Node* ਗੰਢ = elems[i];
        ਪੂਰਨ k = 0;

        ਜੇ( prev_ਗੰਢ )
        {
            ਲਈ( ; k < dims; k++ )
                ਜੇ( ਗੰਢ->idx[k] != prev_ਗੰਢ->idx[k] )
                    ਰੋਕੋ;
            CV_Assert( k < dims );
            ਜੇ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        ਲਈ( ; k < dims; k++ )
            writeScalar( fs, ਗੰਢ->idx[k] );
        prev_ਗੰਢ = ਗੰਢ;

        ਅਚਲ uchar* value = &m.value<uchar>(ਗੰਢ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, Mat& m, ਅਚਲ Mat& default_mat)
{
    ਜੇ( ਗੰਢ.empty() )
    {
        default_mat.copy_ਮੰਜ਼ਿਲ(m);
        ਵਾਪਸ;
    }

    std::string dt;
    ਪੂਰਨ ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, elem_type;

    ਪਠ(ਗੰਢ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ਪਠ(ਗੰਢ["rows"], ਪੰਕਤੀਆਂ, -1);
    ਜੇ( ਪੰਕਤੀਆਂ >= 0 )
    {
        ਪਠ(ਗੰਢ["cols"], ਖਾਨੇ_ਗਿਣਤੀ, -1);
        m.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, elem_type);
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ sizes[CV_MAX_DIM] = {0}, dims;
        File_ਗੰਢ sizes_ਗੰਢ = ਗੰਢ["sizes"];
        CV_Assert( !sizes_ਗੰਢ.empty() );

        dims = (ਪੂਰਨ)sizes_ਗੰਢ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ਗੰਢ.readRaw("i", sizes, dims*ਆਕਾਰ(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ਗੰਢ ਅੰਕੜੇ_ਗੰਢ = ਗੰਢ["data"];
    CV_Assert(!ਅੰਕੜੇ_ਗੰਢ.empty());

    size_t nelems = ਅੰਕੜੇ_ਗੰਢ.size();
    ਜੇ (nelems == 0)
    {
        m = Mat();
        ਵਾਪਸ;
    }
    CV_Assert(nelems == m.ਕੁੱਲ()*m.channels());

    ਅੰਕੜੇ_ਗੰਢ.readRaw(dt, (uchar*)m.ptr(), m.ਕੁੱਲ()*m.elem_ਮਾਪ());
}

ਖਾਲੀ ਪਠ( ਅਚਲ File_ਗੰਢ& ਗੰਢ, SparseMat& m, ਅਚਲ SparseMat& default_mat )
{
    ਜੇ( ਗੰਢ.empty() )
    {
        default_mat.copy_ਮੰਜ਼ਿਲ(m);
        ਵਾਪਸ;
    }

    std::string dt;
    ਪਠ(ਗੰਢ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ਪੂਰਨ elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ਪੂਰਨ sizes[CV_MAX_DIM] = {0};
    File_ਗੰਢ sizes_ਗੰਢ = ਗੰਢ["sizes"];
    CV_Assert( !sizes_ਗੰਢ.empty() );

    ਪੂਰਨ dims = (ਪੂਰਨ)sizes_ਗੰਢ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ਗੰਢ.readRaw("i", sizes, dims*ਆਕਾਰ(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ਗੰਢ ਅੰਕੜੇ = ਗੰਢ["data"];
    CV_Assert( ਅੰਕੜੇ.isSeq() );

    ਪੂਰਨ cn = CV_MAT_CN(elem_type);
    ਪੂਰਨ idx[CV_MAX_DIM] = {0};
    size_t i, sz = ਅੰਕੜੇ.size();
    size_t esz = m.elem_ਮਾਪ();
    File_ਗੰਢ_Iterator it = ਅੰਕੜੇ.begin();

    ਲਈ( i = 0; i < sz; )
    {
        File_ਗੰਢ n = *it;
        ਪੂਰਨ k = (ਪੂਰਨ)n;
        ਜੇ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ਨਹੀਂਤਾਂ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            ਲਈ( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ਪੂਰਨ idx_k = (ਪੂਰਨ)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, ਸੱਚ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, ਕੁੰਜੀ_ਬਿੰਦੂ& value, ਅਚਲ ਕੁੰਜੀ_ਬਿੰਦੂ& default_ਮੁੱਲ)
{
    ਜੇ( ਗੰਢ.empty() )
    {
        value = default_ਮੁੱਲ;
        ਵਾਪਸ;
    }
    ਗੰਢ >> value;
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, DMatch& value, ਅਚਲ DMatch& default_ਮੁੱਲ)
{
    ਜੇ( ਗੰਢ.empty() )
    {
        value = default_ਮੁੱਲ;
        ਵਾਪਸ;
    }
    ਗੰਢ >> value;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV__LEGACY_PERSISTENCE
ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ std::string& name, ਅਚਲ std::vector<ਕੁੰਜੀ_ਬਿੰਦੂ>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ਗੰਢ::SEQ);
    ਲਿਖ(fs, vec);
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, std::vector<ਕੁੰਜੀ_ਬਿੰਦੂ>& keypoints)
{
    File_ਗੰਢ ਪਹਿਲਾ_ਗੰਢ = *(ਗੰਢ.begin());
    ਜੇ (ਪਹਿਲਾ_ਗੰਢ.isSeq())
    {
        // modern scheme
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ਗੰਢ_Iterator it = ਗੰਢ.begin();
        size_t ਕੁੱਲ = (size_t)it.ਬਚਿਆ;
        keypoints.resize(ਕੁੱਲ);
        ਲਈ (size_t i = 0; i < ਕੁੱਲ; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ਨਹੀਂਤਾਂ
        File_ਗੰਢ_Iterator it = ਗੰਢ.begin();
        it >> keypoints;
#ਜੇ_ਅੰਤ
        ਵਾਪਸ;
    }
    keypoints.clear();
    File_ਗੰਢ_Iterator it = ਗੰਢ.begin(), it_ਸਿਰਾ = ਗੰਢ.end();
    ਲਈ( ; it != it_ਸਿਰਾ; )
    {
        ਕੁੰਜੀ_ਬਿੰਦੂ kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ std::string& name, ਅਚਲ std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ਗੰਢ::SEQ);
    ਲਿਖ(fs, vec);
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, std::vector<DMatch>& matches)
{
    File_ਗੰਢ ਪਹਿਲਾ_ਗੰਢ = *(ਗੰਢ.begin());
    ਜੇ (ਪਹਿਲਾ_ਗੰਢ.isSeq())
    {
        // modern scheme
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ਗੰਢ_Iterator it = ਗੰਢ.begin();
        size_t ਕੁੱਲ = (size_t)it.ਬਚਿਆ;
        matches.resize(ਕੁੱਲ);
        ਲਈ (size_t i = 0; i < ਕੁੱਲ; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ਨਹੀਂਤਾਂ
        File_ਗੰਢ_Iterator it = ਗੰਢ.begin();
        it >> matches;
#ਜੇ_ਅੰਤ
        ਵਾਪਸ;
    }
    matches.clear();
    File_ਗੰਢ_Iterator it = ਗੰਢ.begin(), it_ਸਿਰਾ = ਗੰਢ.end();
    ਲਈ( ; it != it_ਸਿਰਾ; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#ਜੇ_ਅੰਤ


}
