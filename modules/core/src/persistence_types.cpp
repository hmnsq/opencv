// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ನಾಮಾಕಾಶ cv
{

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ Mat& m )
{
    ಅಕ್ಷರ dt[22];

    ಆದರೆ( m.dims == 2 || m.empty() )
    {
        fs.ಆರಂಭ_Write_Struct(name, File_ಗಂಟು::MAP, String("opencv-matrix"));
        fs << "rows" << m.ಅಡ್ಡಸಾಲುಗಳು;
        fs << "cols" << m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        fs << "dt" << fs::encodeFormat( m.type(), dt, ಗಾತ್ರ(dt) );
        fs << "data" << "[:";
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < m.ಅಡ್ಡಸಾಲುಗಳು; i++ )
            fs.writeRaw(dt, m.ptr(i), m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*m.elem_ಗಾತ್ರ());
        fs << "]";
        fs.ಕೊನೆ_Write_Struct();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        fs.ಆರಂಭ_Write_Struct(name, File_ಗಂಟು::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, ಗಾತ್ರ(dt) );
        fs << "data" << "[:";
        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t ಒಟ್ಟು = it.size*m.elem_ಗಾತ್ರ();

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], ಒಟ್ಟು );
        fs << "]";
        fs.ಕೊನೆ_Write_Struct();
    }
}

ರಚನೆ Sparse_ಗಂಟು_Cmp
{
    Sparse_ಗಂಟು_Cmp(ಪೂರ್ಣಾಂಕ _dims) : dims(_dims) {}
    ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ SparseMat::Node* a, ಸ್ಥಿರಾಂಕ SparseMat::Node* b)
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
        {
            ಪೂರ್ಣಾಂಕ d = a->idx[i] - b->idx[i];
            ಆದರೆ(d)
                ಹಿಂತಿರುಗಿ d < 0;
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಪೂರ್ಣಾಂಕ dims;
};

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ SparseMat& m )
{
    ಅಕ್ಷರ dt[22];

    fs.ಆರಂಭ_Write_Struct(name, File_ಗಂಟು::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ಪೂರ್ಣಾಂಕ dims = m.dims();
    ಆದರೆ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, ಗಾತ್ರ(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_ಕೊನೆ = m.end();

    ಗಾಗಿ( ; it != it_ಕೊನೆ; ++it )
    {
        CV_Assert(it.ಗಂಟು() != 0);
        elems[i++] = it.ಗಂಟು();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ಗಂಟು_Cmp(dims));
    ಸ್ಥಿರಾಂಕ SparseMat::Node* prev_ಗಂಟು = 0;
    size_t esz = m.elem_ಗಾತ್ರ();

    ಗಾಗಿ( i = 0; i < n; i++ )
    {
        ಸ್ಥಿರಾಂಕ SparseMat::Node* ಗಂಟು = elems[i];
        ಪೂರ್ಣಾಂಕ k = 0;

        ಆದರೆ( prev_ಗಂಟು )
        {
            ಗಾಗಿ( ; k < dims; k++ )
                ಆದರೆ( ಗಂಟು->idx[k] != prev_ಗಂಟು->idx[k] )
                    ನಿಲ್ಲಿಸು;
            CV_Assert( k < dims );
            ಆದರೆ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        ಗಾಗಿ( ; k < dims; k++ )
            writeScalar( fs, ಗಂಟು->idx[k] );
        prev_ಗಂಟು = ಗಂಟು;

        ಸ್ಥಿರಾಂಕ uchar* value = &m.value<uchar>(ಗಂಟು);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, Mat& m, ಸ್ಥಿರಾಂಕ Mat& default_mat)
{
    ಆದರೆ( ಗಂಟು.empty() )
    {
        default_mat.copy_ಗಮ್ಯ(m);
        ಹಿಂತಿರುಗಿ;
    }

    std::string dt;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, elem_type;

    ಪಠ(ಗಂಟು["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ಪಠ(ಗಂಟು["rows"], ಅಡ್ಡಸಾಲುಗಳು, -1);
    ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು >= 0 )
    {
        ಪಠ(ಗಂಟು["cols"], ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, -1);
        m.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, elem_type);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ sizes[CV_MAX_DIM] = {0}, dims;
        File_ಗಂಟು sizes_ಗಂಟು = ಗಂಟು["sizes"];
        CV_Assert( !sizes_ಗಂಟು.empty() );

        dims = (ಪೂರ್ಣಾಂಕ)sizes_ಗಂಟು.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ಗಂಟು.readRaw("i", sizes, dims*ಗಾತ್ರ(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ಗಂಟು ದತ್ತಾಂಶ_ಗಂಟು = ಗಂಟು["data"];
    CV_Assert(!ದತ್ತಾಂಶ_ಗಂಟು.empty());

    size_t nelems = ದತ್ತಾಂಶ_ಗಂಟು.size();
    ಆದರೆ (nelems == 0)
    {
        m = Mat();
        ಹಿಂತಿರುಗಿ;
    }
    CV_Assert(nelems == m.ಒಟ್ಟು()*m.channels());

    ದತ್ತಾಂಶ_ಗಂಟು.readRaw(dt, (uchar*)m.ptr(), m.ಒಟ್ಟು()*m.elem_ಗಾತ್ರ());
}

ಶೂನ್ಯ ಪಠ( ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, SparseMat& m, ಸ್ಥಿರಾಂಕ SparseMat& default_mat )
{
    ಆದರೆ( ಗಂಟು.empty() )
    {
        default_mat.copy_ಗಮ್ಯ(m);
        ಹಿಂತಿರುಗಿ;
    }

    std::string dt;
    ಪಠ(ಗಂಟು["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ಪೂರ್ಣಾಂಕ elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ಪೂರ್ಣಾಂಕ sizes[CV_MAX_DIM] = {0};
    File_ಗಂಟು sizes_ಗಂಟು = ಗಂಟು["sizes"];
    CV_Assert( !sizes_ಗಂಟು.empty() );

    ಪೂರ್ಣಾಂಕ dims = (ಪೂರ್ಣಾಂಕ)sizes_ಗಂಟು.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ಗಂಟು.readRaw("i", sizes, dims*ಗಾತ್ರ(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ಗಂಟು ದತ್ತಾಂಶ = ಗಂಟು["data"];
    CV_Assert( ದತ್ತಾಂಶ.isSeq() );

    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(elem_type);
    ಪೂರ್ಣಾಂಕ idx[CV_MAX_DIM] = {0};
    size_t i, sz = ದತ್ತಾಂಶ.size();
    size_t esz = m.elem_ಗಾತ್ರ();
    File_ಗಂಟು_Iterator it = ದತ್ತಾಂಶ.begin();

    ಗಾಗಿ( i = 0; i < sz; )
    {
        File_ಗಂಟು n = *it;
        ಪೂರ್ಣಾಂಕ k = (ಪೂರ್ಣಾಂಕ)n;
        ಆದರೆ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            ಗಾಗಿ( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ಪೂರ್ಣಾಂಕ idx_k = (ಪೂರ್ಣಾಂಕ)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, ನಿಜ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, ಕೀಲಿ_ಬಿಂದು& value, ಸ್ಥಿರಾಂಕ ಕೀಲಿ_ಬಿಂದು& default_ಮೌಲ್ಯ)
{
    ಆದರೆ( ಗಂಟು.empty() )
    {
        value = default_ಮೌಲ್ಯ;
        ಹಿಂತಿರುಗಿ;
    }
    ಗಂಟು >> value;
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, DMatch& value, ಸ್ಥಿರಾಂಕ DMatch& default_ಮೌಲ್ಯ)
{
    ಆದರೆ( ಗಂಟು.empty() )
    {
        value = default_ಮೌಲ್ಯ;
        ಹಿಂತಿರುಗಿ;
    }
    ಗಂಟು >> value;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV__LEGACY_PERSISTENCE
ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ std::string& name, ಸ್ಥಿರಾಂಕ std::vector<ಕೀಲಿ_ಬಿಂದು>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ಗಂಟು::SEQ);
    ಲಿಖ(fs, vec);
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, std::vector<ಕೀಲಿ_ಬಿಂದು>& keypoints)
{
    File_ಗಂಟು ಮೊದಲ_ಗಂಟು = *(ಗಂಟು.begin());
    ಆದರೆ (ಮೊದಲ_ಗಂಟು.isSeq())
    {
        // modern scheme
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        size_t ಒಟ್ಟು = (size_t)it.ಉಳಿದ;
        keypoints.resize(ಒಟ್ಟು);
        ಗಾಗಿ (size_t i = 0; i < ಒಟ್ಟು; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ಇಲ್ಲದಿದ್ದರೆ
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        it >> keypoints;
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ;
    }
    keypoints.clear();
    File_ಗಂಟು_Iterator it = ಗಂಟು.begin(), it_ಕೊನೆ = ಗಂಟು.end();
    ಗಾಗಿ( ; it != it_ಕೊನೆ; )
    {
        ಕೀಲಿ_ಬಿಂದು kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ std::string& name, ಸ್ಥಿರಾಂಕ std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ಗಂಟು::SEQ);
    ಲಿಖ(fs, vec);
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, std::vector<DMatch>& matches)
{
    File_ಗಂಟು ಮೊದಲ_ಗಂಟು = *(ಗಂಟು.begin());
    ಆದರೆ (ಮೊದಲ_ಗಂಟು.isSeq())
    {
        // modern scheme
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        size_t ಒಟ್ಟು = (size_t)it.ಉಳಿದ;
        matches.resize(ಒಟ್ಟು);
        ಗಾಗಿ (size_t i = 0; i < ಒಟ್ಟು; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ಇಲ್ಲದಿದ್ದರೆ
        File_ಗಂಟು_Iterator it = ಗಂಟು.begin();
        it >> matches;
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ;
    }
    matches.clear();
    File_ಗಂಟು_Iterator it = ಗಂಟು.begin(), it_ಕೊನೆ = ಗಂಟು.end();
    ಗಾಗಿ( ; it != it_ಕೊನೆ; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#ಆದರೆ_ಅಂತ್ಯ


}
