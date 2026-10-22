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
        fs.شروع_Write_Struct(name, File_گرہ::MAP, String("opencv-matrix"));
        fs << "rows" << m.صفاں;
        fs << "cols" << m.خانے_گنتی;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        لكل( صحيح i = 0; i < m.صفاں; i++ )
            fs.writeRaw(dt, m.ptr(i), m.خانے_گنتی*m.elem_ناپ());
        fs << "]";
        fs.سرا_Write_Struct();
    }
    وإلا
    {
        fs.شروع_Write_Struct(name, File_گرہ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t کل = it.size*m.elem_ناپ();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], کل );
        fs << "]";
        fs.سرا_Write_Struct();
    }
}

بنية Sparse_گرہ_Cmp
{
    Sparse_گرہ_Cmp(صحيح _dims) : dims(_dims) {}
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

    fs.شروع_Write_Struct(name, File_گرہ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    إذا( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_بفر<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_سرا = m.end();

    لكل( ; it != it_سرا; ++it )
    {
        CV_Assert(it.گرہ() != 0);
        elems[i++] = it.گرہ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_گرہ_Cmp(dims));
    ثابت SparseMat::Node* prev_گرہ = 0;
    size_t esz = m.elem_ناپ();

    لكل( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* گرہ = elems[i];
        صحيح k = 0;

        إذا( prev_گرہ )
        {
            لكل( ; k < dims; k++ )
                إذا( گرہ->idx[k] != prev_گرہ->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            إذا( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        لكل( ; k < dims; k++ )
            writeScalar( fs, گرہ->idx[k] );
        prev_گرہ = گرہ;

        ثابت uchar* value = &m.value<uchar>(گرہ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت File_گرہ& گرہ, Mat& m, ثابت Mat& default_mat)
{
    إذا( گرہ.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    صحيح صفاں, خانے_گنتی, elem_type;

    اقرأ(گرہ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(گرہ["rows"], صفاں, -1);
    إذا( صفاں >= 0 )
    {
        اقرأ(گرہ["cols"], خانے_گنتی, -1);
        m.create(صفاں, خانے_گنتی, elem_type);
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        File_گرہ sizes_گرہ = گرہ["sizes"];
        CV_Assert( !sizes_گرہ.empty() );

        dims = (صحيح)sizes_گرہ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_گرہ.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_گرہ انکڑے_گرہ = گرہ["data"];
    CV_Assert(!انکڑے_گرہ.empty());

    size_t nelems = انکڑے_گرہ.size();
    إذا (nelems == 0)
    {
        m = Mat();
        أرجع;
    }
    CV_Assert(nelems == m.کل()*m.channels());

    انکڑے_گرہ.readRaw(dt, (uchar*)m.ptr(), m.کل()*m.elem_ناپ());
}

فراغ اقرأ( ثابت File_گرہ& گرہ, SparseMat& m, ثابت SparseMat& default_mat )
{
    إذا( گرہ.empty() )
    {
        default_mat.copy_منزل(m);
        أرجع;
    }

    std::string dt;
    اقرأ(گرہ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    File_گرہ sizes_گرہ = گرہ["sizes"];
    CV_Assert( !sizes_گرہ.empty() );

    صحيح dims = (صحيح)sizes_گرہ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_گرہ.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_گرہ انکڑے = گرہ["data"];
    CV_Assert( انکڑے.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = انکڑے.size();
    size_t esz = m.elem_ناپ();
    File_گرہ_Iterator it = انکڑے.begin();

    لكل( i = 0; i < sz; )
    {
        File_گرہ n = *it;
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

فراغ اقرأ(ثابت File_گرہ& گرہ, کلید_نقطہ& value, ثابت کلید_نقطہ& default_قدر)
{
    إذا( گرہ.empty() )
    {
        value = default_قدر;
        أرجع;
    }
    گرہ >> value;
}

فراغ اقرأ(ثابت File_گرہ& گرہ, DMatch& value, ثابت DMatch& default_قدر)
{
    إذا( گرہ.empty() )
    {
        value = default_قدر;
        أرجع;
    }
    گرہ >> value;
}

#جے_تعریف_شدہ CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<کلید_نقطہ>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گرہ::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_گرہ& گرہ, std::vector<کلید_نقطہ>& keypoints)
{
    File_گرہ پہلا_گرہ = *(گرہ.begin());
    إذا (پہلا_گرہ.isSeq())
    {
        // modern scheme
#جے_تعریف_شدہ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گرہ_Iterator it = گرہ.begin();
        size_t کل = (size_t)it.بچیا;
        keypoints.resize(کل);
        لكل (size_t i = 0; i < کل; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وإلا
        File_گرہ_Iterator it = گرہ.begin();
        it >> keypoints;
#جے_انت
        أرجع;
    }
    keypoints.clear();
    File_گرہ_Iterator it = گرہ.begin(), it_سرا = گرہ.end();
    لكل( ; it != it_سرا; )
    {
        کلید_نقطہ kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گرہ::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_گرہ& گرہ, std::vector<DMatch>& matches)
{
    File_گرہ پہلا_گرہ = *(گرہ.begin());
    إذا (پہلا_گرہ.isSeq())
    {
        // modern scheme
#جے_تعریف_شدہ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گرہ_Iterator it = گرہ.begin();
        size_t کل = (size_t)it.بچیا;
        matches.resize(کل);
        لكل (size_t i = 0; i < کل; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وإلا
        File_گرہ_Iterator it = گرہ.begin();
        it >> matches;
#جے_انت
        أرجع;
    }
    matches.clear();
    File_گرہ_Iterator it = گرہ.begin(), it_سرا = گرہ.end();
    لكل( ; it != it_سرا; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#جے_انت


}
