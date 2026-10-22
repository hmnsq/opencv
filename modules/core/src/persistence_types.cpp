// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

نامخلا cv
{

خالی اكتب( FileStorage& fs, ثابت String& name, ثابت Mat& m )
{
    حرف dt[22];

    اگر( m.dims == 2 || m.empty() )
    {
        fs.آغاز_Write_Struct(name, File_گرہ::MAP, String("opencv-matrix"));
        fs << "rows" << m.صفیں;
        fs << "cols" << m.ستون_تعداد;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        برائے( صحیح i = 0; i < m.صفیں; i++ )
            fs.writeRaw(dt, m.ptr(i), m.ستون_تعداد*m.elem_سائز());
        fs << "]";
        fs.انتہا_Write_Struct();
    }
    ورنہ
    {
        fs.آغاز_Write_Struct(name, File_گرہ::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحیح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t کل = it.size*m.elem_سائز();

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], کل );
        fs << "]";
        fs.انتہا_Write_Struct();
    }
}

ساخت Sparse_گرہ_Cmp
{
    Sparse_گرہ_Cmp(صحیح _dims) : dims(_dims) {}
    منطقی عامل()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
    {
        برائے( صحیح i = 0; i < dims; i++ )
        {
            صحیح d = a->idx[i] - b->idx[i];
            اگر(d)
                واپس d < 0;
        }
        واپس جھوٹ;
    }

    صحیح dims;
};

خالی اكتب( FileStorage& fs, ثابت String& name, ثابت SparseMat& m )
{
    حرف dt[22];

    fs.آغاز_Write_Struct(name, File_گرہ::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحیح dims = m.dims();
    اگر( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحیح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_بفر<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_انتہا = m.end();

    برائے( ; it != it_انتہا; ++it )
    {
        CV_Assert(it.گرہ() != 0);
        elems[i++] = it.گرہ();
    }

    std::sort(elems.begin(), elems.end(), Sparse_گرہ_Cmp(dims));
    ثابت SparseMat::Node* پچھلا_گرہ = 0;
    size_t esz = m.elem_سائز();

    برائے( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* گرہ = elems[i];
        صحیح k = 0;

        اگر( پچھلا_گرہ )
        {
            برائے( ; k < dims; k++ )
                اگر( گرہ->idx[k] != پچھلا_گرہ->idx[k] )
                    توڑو;
            CV_Assert( k < dims );
            اگر( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        برائے( ; k < dims; k++ )
            writeScalar( fs, گرہ->idx[k] );
        پچھلا_گرہ = گرہ;

        ثابت uchar* value = &m.value<uchar>(گرہ);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

خالی اقرأ(ثابت File_گرہ& گرہ, Mat& m, ثابت Mat& default_mat)
{
    اگر( گرہ.empty() )
    {
        default_mat.copy_مقصد(m);
        واپس;
    }

    std::string dt;
    صحیح صفیں, ستون_تعداد, elem_type;

    اقرأ(گرہ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(گرہ["rows"], صفیں, -1);
    اگر( صفیں >= 0 )
    {
        اقرأ(گرہ["cols"], ستون_تعداد, -1);
        m.create(صفیں, ستون_تعداد, elem_type);
    }
    ورنہ
    {
        صحیح sizes[CV_MAX_DIM] = {0}, dims;
        File_گرہ sizes_گرہ = گرہ["sizes"];
        CV_Assert( !sizes_گرہ.empty() );

        dims = (صحیح)sizes_گرہ.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_گرہ.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_گرہ ڈیٹا_گرہ = گرہ["data"];
    CV_Assert(!ڈیٹا_گرہ.empty());

    size_t nelems = ڈیٹا_گرہ.size();
    اگر (nelems == 0)
    {
        m = Mat();
        واپس;
    }
    CV_Assert(nelems == m.کل()*m.channels());

    ڈیٹا_گرہ.readRaw(dt, (uchar*)m.ptr(), m.کل()*m.elem_سائز());
}

خالی اقرأ( ثابت File_گرہ& گرہ, SparseMat& m, ثابت SparseMat& default_mat )
{
    اگر( گرہ.empty() )
    {
        default_mat.copy_مقصد(m);
        واپس;
    }

    std::string dt;
    اقرأ(گرہ["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحیح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحیح sizes[CV_MAX_DIM] = {0};
    File_گرہ sizes_گرہ = گرہ["sizes"];
    CV_Assert( !sizes_گرہ.empty() );

    صحیح dims = (صحیح)sizes_گرہ.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_گرہ.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_گرہ ڈیٹا = گرہ["data"];
    CV_Assert( ڈیٹا.isSeq() );

    صحیح cn = CV_MAT_CN(elem_type);
    صحیح idx[CV_MAX_DIM] = {0};
    size_t i, sz = ڈیٹا.size();
    size_t esz = m.elem_سائز();
    File_گرہ_Iterator it = ڈیٹا.begin();

    برائے( i = 0; i < sz; )
    {
        File_گرہ n = *it;
        صحیح k = (صحیح)n;
        اگر( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ورنہ
        {
            اگر( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ورنہ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            برائے( ; k < dims; k++ )
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
        uchar* valptr = m.ptr(idx, سچ);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

خالی اقرأ(ثابت File_گرہ& گرہ, کلید_نقطہ& value, ثابت کلید_نقطہ& default_قدر)
{
    اگر( گرہ.empty() )
    {
        value = default_قدر;
        واپس;
    }
    گرہ >> value;
}

خالی اقرأ(ثابت File_گرہ& گرہ, DMatch& value, ثابت DMatch& default_قدر)
{
    اگر( گرہ.empty() )
    {
        value = default_قدر;
        واپس;
    }
    گرہ >> value;
}

#اگر_تعریف_شدہ CV__LEGACY_PERSISTENCE
خالی اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<کلید_نقطہ>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گرہ::SEQ);
    اكتب(fs, vec);
}

خالی اقرأ(ثابت File_گرہ& گرہ, std::vector<کلید_نقطہ>& keypoints)
{
    File_گرہ پہلا_گرہ = *(گرہ.begin());
    اگر (پہلا_گرہ.isSeq())
    {
        // modern scheme
#اگر_تعریف_شدہ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گرہ_Iterator it = گرہ.begin();
        size_t کل = (size_t)it.بقایا;
        keypoints.resize(کل);
        برائے (size_t i = 0; i < کل; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ورنہ
        File_گرہ_Iterator it = گرہ.begin();
        it >> keypoints;
#اگر_ختم
        واپس;
    }
    keypoints.clear();
    File_گرہ_Iterator it = گرہ.begin(), it_انتہا = گرہ.end();
    برائے( ; it != it_انتہا; )
    {
        کلید_نقطہ kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

خالی اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_گرہ::SEQ);
    اكتب(fs, vec);
}

خالی اقرأ(ثابت File_گرہ& گرہ, std::vector<DMatch>& matches)
{
    File_گرہ پہلا_گرہ = *(گرہ.begin());
    اگر (پہلا_گرہ.isSeq())
    {
        // modern scheme
#اگر_تعریف_شدہ OPENCV_TRAITS_ENABLE_DEPRECATED
        File_گرہ_Iterator it = گرہ.begin();
        size_t کل = (size_t)it.بقایا;
        matches.resize(کل);
        برائے (size_t i = 0; i < کل; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ورنہ
        File_گرہ_Iterator it = گرہ.begin();
        it >> matches;
#اگر_ختم
        واپس;
    }
    matches.clear();
    File_گرہ_Iterator it = گرہ.begin(), it_انتہا = گرہ.end();
    برائے( ; it != it_انتہا; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#اگر_ختم


}
