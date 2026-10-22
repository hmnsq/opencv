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

    אם( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        עבור( صحيح i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    אחרת
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*حجم(صحيح) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
        fs << "data" << "[:";
        ثابت Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        עבור( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

بنية SparseNodeCmp
{
    SparseNodeCmp(صحيح _dims) : dims(_dims) {}
    منطقي operator()(ثابت SparseMat::Node* a, ثابت SparseMat::Node* b)
    {
        עבור( صحيح i = 0; i < dims; i++ )
        {
            صحيح d = a->idx[i] - b->idx[i];
            אם(d)
                החזר d < 0;
        }
        החזר שקר;
    }

    صحيح dims;
};

فراغ اكتب( FileStorage& fs, ثابت String& name, ثابت SparseMat& m )
{
    حرف dt[22];

    fs.startWriteStruct(name, FileNode::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    صحيح dims = m.dims();
    אם( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*حجم(صحيح) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, حجم(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<ثابت SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    עבור( ; it != it_end; ++it )
    {
        CV_Assert(it.node() != 0);
        elems[i++] = it.node();
    }

    std::sort(elems.begin(), elems.end(), SparseNodeCmp(dims));
    ثابت SparseMat::Node* prev_node = 0;
    size_t esz = m.elemSize();

    עבור( i = 0; i < n; i++ )
    {
        ثابت SparseMat::Node* node = elems[i];
        صحيح k = 0;

        אם( prev_node )
        {
            עבור( ; k < dims; k++ )
                אם( node->idx[k] != prev_node->idx[k] )
                    اكسر;
            CV_Assert( k < dims );
            אם( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        עבור( ; k < dims; k++ )
            writeScalar( fs, node->idx[k] );
        prev_node = node;

        ثابت uchar* value = &m.value<uchar>(node);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

فراغ اقرأ(ثابت FileNode& node, Mat& m, ثابت Mat& default_mat)
{
    אם( node.empty() )
    {
        default_mat.copyTo(m);
        החזר;
    }

    std::string dt;
    صحيح rows, cols, elem_type;

    اقرأ(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    اقرأ(node["rows"], rows, -1);
    אם( rows >= 0 )
    {
        اقرأ(node["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    אחרת
    {
        صحيح sizes[CV_MAX_DIM] = {0}, dims;
        FileNode sizes_node = node["sizes"];
        CV_Assert( !sizes_node.empty() );

        dims = (صحيح)sizes_node.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_node.readRaw("i", sizes, dims*حجم(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    FileNode data_node = node["data"];
    CV_Assert(!data_node.empty());

    size_t nelems = data_node.size();
    אם (nelems == 0)
    {
        m = Mat();
        החזר;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_node.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

فراغ اقرأ( ثابت FileNode& node, SparseMat& m, ثابت SparseMat& default_mat )
{
    אם( node.empty() )
    {
        default_mat.copyTo(m);
        החזר;
    }

    std::string dt;
    اقرأ(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    صحيح elem_type = fs::decodeSimpleFormat( dt.c_str() );

    صحيح sizes[CV_MAX_DIM] = {0};
    FileNode sizes_node = node["sizes"];
    CV_Assert( !sizes_node.empty() );

    صحيح dims = (صحيح)sizes_node.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_node.readRaw("i", sizes, dims*حجم(sizes[0]));

    m.create(dims, sizes, elem_type);

    FileNode data = node["data"];
    CV_Assert( data.isSeq() );

    صحيح cn = CV_MAT_CN(elem_type);
    صحيح idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    FileNodeIterator it = data.begin();

    עבור( i = 0; i < sz; )
    {
        FileNode n = *it;
        صحيح k = (صحيح)n;
        אם( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        אחרת
        {
            אם( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            אחרת
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            עבור( ; k < dims; k++ )
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
        uchar* valptr = m.ptr(idx, אמת);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

فراغ اقرأ(ثابت FileNode& node, KeyPoint& value, ثابت KeyPoint& default_value)
{
    אם( node.empty() )
    {
        value = default_value;
        החזר;
    }
    node >> value;
}

فراغ اقرأ(ثابت FileNode& node, DMatch& value, ثابت DMatch& default_value)
{
    אם( node.empty() )
    {
        value = default_value;
        החזר;
    }
    node >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<KeyPoint>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت FileNode& node, std::vector<KeyPoint>& keypoints)
{
    FileNode first_node = *(node.begin());
    אם (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        עבור (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#אחרת
        FileNodeIterator it = node.begin();
        it >> keypoints;
#endif
        החזר;
    }
    keypoints.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    עבור( ; it != it_end; )
    {
        KeyPoint kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

فراغ اكتب( FileStorage& fs, ثابت std::string& name, ثابت std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ);
    اكتب(fs, vec);
}

فراغ اقرأ(ثابت FileNode& node, std::vector<DMatch>& matches)
{
    FileNode first_node = *(node.begin());
    אם (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        עבור (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#אחרת
        FileNodeIterator it = node.begin();
        it >> matches;
#endif
        החזר;
    }
    matches.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    עבור( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
