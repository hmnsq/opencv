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
        fs.شروع_Write_Struct(name, File_گره::MAP, String("opencv-matrix"));
        fs << "rows" << m.ردیفها;
        fs << "cols" << m.تعداد_ستون;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        لكل( صحيح i = 0; i < m.ردیفها; i++ )
            fs.writeRaw(dt, m.ptr(i), m.تعداد_ستون*m.elem_اندازه());
        fs << "]";
        fs.انتها_Write_Struct();
    }
    وإلا
    {
        fs.شروع_Write_Struct(name, File_گره::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t مجموع = it.size*m.elem_اندازه();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], مجموع );
        fs << "]";
        fs.انتها_Write_Struct();
    }
}

بنية Sparse_گره_Cmp
{
    Sparse_گره_Cmp(صحيح _dims) : dims(_dims) {}
    منطقي عملگر()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
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

    fs.شروع_Write_Struct(name, File_گره::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    إذا( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_میانگیر<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_انتها = m.end();

    لكل( ; it != it_انتها; ++it )
    {
        CV_Assert(it.گره() != 0);
        elems[i++] = it.گره();
    }

    std::sort(elems.begin(), elems.end(), Sparse_گره_Cmp(dims));
    ثابت SparseMat::Node* prev_گره = 0;
    size_t esz = m.elem_اندازه();

    لكل( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* گره = elems[i];
        صحيح k = 0;

        إذا( prev_گره )
        {
            لكل( ; k < dims; k++ )
                إذا( گره->idx[k] != prev_گره->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            إذا( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        لكل( ; k < dims; k++ )
            writeScalar( fs, گره->idx[k] );
        prev_گره = گره;

        ثابت uchar* value = &m.value<uchar>(گره);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت File_گره& گره, Mat& m, ثابت Mat& default_mat)
{
    إذا( گره.empty() )
    {
        default_mat.copy_مقصد(m);
        أرجع;
    }

    std::string dt;
    صحيح ردیفها, تعداد_ستون, elem_type;

    اقرأ(گره["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(گره["rows"], ردیفها, -1);
    إذا( ردیفها >= 0 )
    {
        اقرأ(گره["cols"], تعداد_ستون, -1);
        m.create(ردیفها, تعداد_ستون, elem_type);
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        File_گره sizes_گره = گره["sizes"];
        CV_Assert( !sizes_گره.empty() );

        dims = (صحيح)sizes_گره.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_گره.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_گره داده_گره = گره["data"];
    CV_Assert(!داده_گره.empty());

    size_t nelems = داده_گره.size();
    إذا (nelems == 0)
    {
        m = Mat();
        أرجع;
    }
    CV_Assert(nelems == m.مجموع()*m.channels());

    داده_گره.readRaw(dt, (uchar*)m.ptr(), m.مجموع()*m.elem_اندازه());
}

فراغ اقرأ( ثابت File_گره& گره, SparseMat& m, ثابت SparseMat& default_mat )
{
    إذا( گره.empty() )
    {
        default_mat.copy_مقصد(m);
        أرجع;
    }

    std::string dt;
    اقرأ(گره["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    File_گره sizes_گره = گره["sizes"];
    CV_Assert( !sizes_گره.empty() );

    صحيح dims = (صحيح)sizes_گره.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_گره.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_گره داده = گره["data"];
    CV_Assert( داده.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = داده.size();
    size_t esz = m.elem_اندازه();
    File_گره_Iterator it = داده.begin();

    لكل( i = 0; i < sz; )
    {
        File_گره n = *it;
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
        uchar* valptr = m.ptr(idx, درست);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

فراغ اقرأ(ثابت File_گره& گره, کلید_نقطه& value, ثابت کلید_نقطه& default_مقدار)
{
    إذا( گره.empty() )
    {
        value = default_مقدار;
        أرجع;
    }
    گره >> value;
}

فراغ اقرأ(ثابت File_گره& گره, DMatch& value, ثابت DMatch& default_مقدار)
{
    إذا( گره.empty() )
    {
        value = default_مقدار;
        أرجع;
    }
    گره >> value;
}

#اگر_تعریف_شده CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<کلید_نقطه>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گره::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_گره& گره, std::vector<کلید_نقطه>& keypoints)
{
    File_گره اولی_گره = *(گره.begin());
    إذا (اولی_گره.isSeq())
    {
        // modern scheme
#اگر_تعریف_شده OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گره_Iterator it = گره.begin();
        size_t مجموع = (size_t)it.مانده;
        keypoints.resize(مجموع);
        لكل (size_t i = 0; i < مجموع; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#وإلا
        File_گره_Iterator it = گره.begin();
        it >> keypoints;
#پایان_اگر
        أرجع;
    }
    keypoints.clear();
    File_گره_Iterator it = گره.begin(), it_انتها = گره.end();
    لكل( ; it != it_انتها; )
    {
        کلید_نقطه kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گره::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت File_گره& گره, std::vector<DMatch>& matches)
{
    File_گره اولی_گره = *(گره.begin());
    إذا (اولی_گره.isSeq())
    {
        // modern scheme
#اگر_تعریف_شده OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گره_Iterator it = گره.begin();
        size_t مجموع = (size_t)it.مانده;
        matches.resize(مجموع);
        لكل (size_t i = 0; i < مجموع; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#وإلا
        File_گره_Iterator it = گره.begin();
        it >> matches;
#پایان_اگر
        أرجع;
    }
    matches.clear();
    File_گره_Iterator it = گره.begin(), it_انتها = گره.end();
    لكل( ; it != it_انتها; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#پایان_اگر


}
