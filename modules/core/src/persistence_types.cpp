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
        fs.پیل_Write_Struct(name, File_غوټه::MAP, String("opencv-matrix"));
        fs << "rows" << m.لیکې;
        fs << "cols" << m.ستون_شمېر;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        لكل( صحيح i = 0; i < m.لیکې; i++ )
            fs.writeRaw(dt, m.ptr(i), m.ستون_شمېر*m.elem_اندازه());
        fs << "]";
        fs.پای_ټکی_Write_Struct();
    }
    وإلا
    {
        fs.پیل_Write_Struct(name, File_غوټه::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t ټول = it.size*m.elem_اندازه();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], ټول );
        fs << "]";
        fs.پای_ټکی_Write_Struct();
    }
}

بنية Sparse_غوټه_Cmp
{
    Sparse_غوټه_Cmp(صحيح _dims) : dims(_dims) {}
    منطقي چلوونکی()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
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

    fs.پیل_Write_Struct(name, File_غوټه::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    إذا( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_بفر<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_پای_ټکی = m.end();

    لكل( ; it != it_پای_ټکی; ++it )
    {
        CV_Assert(it.غوټه() != 0);
        elems[i++] = it.غوټه();
    }

    std::sort(elems.begin(), elems.end(), Sparse_غوټه_Cmp(dims));
    ثابت SparseMat::Node* prev_غوټه = 0;
    size_t esz = m.elem_اندازه();

    لكل( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* غوټه = elems[i];
        صحيح k = 0;

        إذا( prev_غوټه )
        {
            لكل( ; k < dims; k++ )
                إذا( غوټه->idx[k] != prev_غوټه->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            إذا( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        لكل( ; k < dims; k++ )
            writeScalar( fs, غوټه->idx[k] );
        prev_غوټه = غوټه;

        ثابت uchar* value = &m.value<uchar>(غوټه);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت File_غوټه& غوټه, Mat& m, ثابت Mat& default_mat)
{
    إذا( غوټه.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    صحيح لیکې, ستون_شمېر, elem_type;

    اقرأ(غوټه["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(غوټه["rows"], لیکې, -1);
    إذا( لیکې >= 0 )
    {
        اقرأ(غوټه["cols"], ستون_شمېر, -1);
        m.create(لیکې, ستون_شمېر, elem_type);
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        File_غوټه sizes_غوټه = غوټه["sizes"];
        CV_Assert( !sizes_غوټه.empty() );

        dims = (صحيح)sizes_غوټه.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_غوټه.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_غوټه معلومات_غوټه = غوټه["data"];
    CV_Assert(!معلومات_غوټه.empty());

    size_t nelems = معلومات_غوټه.size();
    إذا (nelems == 0)
    {
        m = Mat();
        أرجع;
    }
    CV_Assert(nelems == m.ټول()*m.channels());

    معلومات_غوټه.readRaw(dt, (uchar*)m.ptr(), m.ټول()*m.elem_اندازه());
}

فراغ اقرأ( ثابت File_غوټه& غوټه, SparseMat& m, ثابت SparseMat& default_mat )
{
    إذا( غوټه.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    اقرأ(غوټه["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    File_غوټه sizes_غوټه = غوټه["sizes"];
    CV_Assert( !sizes_غوټه.empty() );

    صحيح dims = (صحيح)sizes_غوټه.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_غوټه.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_غوټه معلومات = غوټه["data"];
    CV_Assert( معلومات.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = معلومات.size();
    size_t esz = m.elem_اندازه();
    File_غوټه_Iterator it = معلومات.begin();

    لكل( i = 0; i < sz; )
    {
        File_غوټه n = *it;
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
        uchar* valptr = m.ptr(idx, ریښتیا);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

فراغ اقرأ(ثابت File_غوټه& غوټه, کلی_ټکی& value, ثابت کلی_ټکی& default_ارزښت)
{
    إذا( غوټه.empty() )
    {
        value = default_ارزښت;
        أرجع;
    }
    غوټه >> value;
}

فراغ اقرأ(ثابت File_غوټه& غوټه, DMatch& value, ثابت DMatch& default_ارزښت)
{
    إذا( غوټه.empty() )
    {
        value = default_ارزښت;
        أرجع;
    }
    غوټه >> value;
}

#که_تعریف_شوی CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<کلی_ټکی>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_غوټه::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_غوټه& غوټه, std::vector<کلی_ټکی>& keypoints)
{
    File_غوټه لومړی_غوټه = *(غوټه.begin());
    إذا (لومړی_غوټه.isSeq())
    {
        // modern scheme
#که_تعریف_شوی OPENCV_TRAITS_ENABLE_DEPRECATED
        File_غوټه_Iterator it = غوټه.begin();
        size_t ټول = (size_t)it.پاتې_شوی;
        keypoints.resize(ټول);
        لكل (size_t i = 0; i < ټول; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وإلا
        File_غوټه_Iterator it = غوټه.begin();
        it >> keypoints;
#د_که_پای
        أرجع;
    }
    keypoints.clear();
    File_غوټه_Iterator it = غوټه.begin(), it_پای_ټکی = غوټه.end();
    لكل( ; it != it_پای_ټکی; )
    {
        کلی_ټکی kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_غوټه::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_غوټه& غوټه, std::vector<DMatch>& matches)
{
    File_غوټه لومړی_غوټه = *(غوټه.begin());
    إذا (لومړی_غوټه.isSeq())
    {
        // modern scheme
#که_تعریف_شوی OPENCV_TRAITS_ENABLE_DEPRECATED
        File_غوټه_Iterator it = غوټه.begin();
        size_t ټول = (size_t)it.پاتې_شوی;
        matches.resize(ټول);
        لكل (size_t i = 0; i < ټول; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وإلا
        File_غوټه_Iterator it = غوټه.begin();
        it >> matches;
#د_که_پای
        أرجع;
    }
    matches.clear();
    File_غوټه_Iterator it = غوټه.begin(), it_پای_ټکی = غوټه.end();
    لكل( ; it != it_پای_ټکی; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#د_که_پای


}
