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
        fs.شروعات_Write_Struct(name, File_ڳنڍ::MAP, String("opencv-matrix"));
        fs << "rows" << m.صفون;
        fs << "cols" << m.کانن_ڳڻپ;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        لكل( صحيح i = 0; i < m.صفون; i++ )
            fs.writeRaw(dt, m.ptr(i), m.کانن_ڳڻپ*m.elem_سائيز());
        fs << "]";
        fs.پڇاڙي_Write_Struct();
    }
    وإلا
    {
        fs.شروعات_Write_Struct(name, File_ڳنڍ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t ڪل = it.size*m.elem_سائيز();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], ڪل );
        fs << "]";
        fs.پڇاڙي_Write_Struct();
    }
}

بنية Sparse_ڳنڍ_Cmp
{
    Sparse_ڳنڍ_Cmp(صحيح _dims) : dims(_dims) {}
    منطقي عامل()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
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

    fs.شروعات_Write_Struct(name, File_ڳنڍ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    إذا( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_بفر<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_پڇاڙي = m.end();

    لكل( ; it != it_پڇاڙي; ++it )
    {
        CV_Assert(it.ڳنڍ() != 0);
        elems[i++] = it.ڳنڍ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_ڳنڍ_Cmp(dims));
    ثابت SparseMat::Node* prev_ڳنڍ = 0;
    size_t esz = m.elem_سائيز();

    لكل( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* ڳنڍ = elems[i];
        صحيح k = 0;

        إذا( prev_ڳنڍ )
        {
            لكل( ; k < dims; k++ )
                إذا( ڳنڍ->idx[k] != prev_ڳنڍ->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            إذا( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        لكل( ; k < dims; k++ )
            writeScalar( fs, ڳنڍ->idx[k] );
        prev_ڳنڍ = ڳنڍ;

        ثابت uchar* value = &m.value<uchar>(ڳنڍ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت File_ڳنڍ& ڳنڍ, Mat& m, ثابت Mat& default_mat)
{
    إذا( ڳنڍ.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    صحيح صفون, کانن_ڳڻپ, elem_type;

    اقرأ(ڳنڍ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(ڳنڍ["rows"], صفون, -1);
    إذا( صفون >= 0 )
    {
        اقرأ(ڳنڍ["cols"], کانن_ڳڻپ, -1);
        m.create(صفون, کانن_ڳڻپ, elem_type);
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        File_ڳنڍ sizes_ڳنڍ = ڳنڍ["sizes"];
        CV_Assert( !sizes_ڳنڍ.empty() );

        dims = (صحيح)sizes_ڳنڍ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_ڳنڍ.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_ڳنڍ معلومات_ڳنڍ = ڳنڍ["data"];
    CV_Assert(!معلومات_ڳنڍ.empty());

    size_t nelems = معلومات_ڳنڍ.size();
    إذا (nelems == 0)
    {
        m = Mat();
        أرجع;
    }
    CV_Assert(nelems == m.ڪل()*m.channels());

    معلومات_ڳنڍ.readRaw(dt, (uchar*)m.ptr(), m.ڪل()*m.elem_سائيز());
}

فراغ اقرأ( ثابت File_ڳنڍ& ڳنڍ, SparseMat& m, ثابت SparseMat& default_mat )
{
    إذا( ڳنڍ.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    اقرأ(ڳنڍ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    File_ڳنڍ sizes_ڳنڍ = ڳنڍ["sizes"];
    CV_Assert( !sizes_ڳنڍ.empty() );

    صحيح dims = (صحيح)sizes_ڳنڍ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_ڳنڍ.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_ڳنڍ معلومات = ڳنڍ["data"];
    CV_Assert( معلومات.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = معلومات.size();
    size_t esz = m.elem_سائيز();
    File_ڳنڍ_Iterator it = معلومات.begin();

    لكل( i = 0; i < sz; )
    {
        File_ڳنڍ n = *it;
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
        uchar* valptr = m.ptr(idx, سچ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

فراغ اقرأ(ثابت File_ڳنڍ& ڳنڍ, ڪنجي_نقطو& value, ثابت ڪنجي_نقطو& default_قدر)
{
    إذا( ڳنڍ.empty() )
    {
        value = default_قدر;
        أرجع;
    }
    ڳنڍ >> value;
}

فراغ اقرأ(ثابت File_ڳنڍ& ڳنڍ, DMatch& value, ثابت DMatch& default_قدر)
{
    إذا( ڳنڍ.empty() )
    {
        value = default_قدر;
        أرجع;
    }
    ڳنڍ >> value;
}

#جيڪڏهن_وصف_ٿيل CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<ڪنجي_نقطو>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ڳنڍ::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_ڳنڍ& ڳنڍ, std::vector<ڪنجي_نقطو>& keypoints)
{
    File_ڳنڍ پهريون_ڳنڍ = *(ڳنڍ.begin());
    إذا (پهريون_ڳنڍ.isSeq())
    {
        // modern scheme
#جيڪڏهن_وصف_ٿيل OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ڳنڍ_Iterator it = ڳنڍ.begin();
        size_t ڪل = (size_t)it.بچيل;
        keypoints.resize(ڪل);
        لكل (size_t i = 0; i < ڪل; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وإلا
        File_ڳنڍ_Iterator it = ڳنڍ.begin();
        it >> keypoints;
#جيڪڏهن_پورو
        أرجع;
    }
    keypoints.clear();
    File_ڳنڍ_Iterator it = ڳنڍ.begin(), it_پڇاڙي = ڳنڍ.end();
    لكل( ; it != it_پڇاڙي; )
    {
        ڪنجي_نقطو kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_ڳنڍ::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_ڳنڍ& ڳنڍ, std::vector<DMatch>& matches)
{
    File_ڳنڍ پهريون_ڳنڍ = *(ڳنڍ.begin());
    إذا (پهريون_ڳنڍ.isSeq())
    {
        // modern scheme
#جيڪڏهن_وصف_ٿيل OPENCV_TRAITS_ENABLE_DEPRECATED
        File_ڳنڍ_Iterator it = ڳنڍ.begin();
        size_t ڪل = (size_t)it.بچيل;
        matches.resize(ڪل);
        لكل (size_t i = 0; i < ڪل; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وإلا
        File_ڳنڍ_Iterator it = ڳنڍ.begin();
        it >> matches;
#جيڪڏهن_پورو
        أرجع;
    }
    matches.clear();
    File_ڳنڍ_Iterator it = ڳنڍ.begin(), it_پڇاڙي = ڳنڍ.end();
    لكل( ; it != it_پڇاڙي; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#جيڪڏهن_پورو


}
