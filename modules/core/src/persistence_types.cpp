// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

namespace cv
{

void write( FileStorage& fs, const String& name, const Mat& m )
{
    char dt[22];

    якщо( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
        fs << "data" << "[:";
        для( ціле i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    інакше
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*sizeof(ціле) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
        fs << "data" << "[:";
        const Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        для( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

struct SparseNodeCmp
{
    SparseNodeCmp(ціле _dims) : dims(_dims) {}
    bool operator()(const SparseMat::Node* a, const SparseMat::Node* b)
    {
        для( ціле i = 0; i < dims; i++ )
        {
            ціле d = a->idx[i] - b->idx[i];
            якщо(d)
                повернути d < 0;
        }
        повернути false;
    }

    ціле dims;
};

void write( FileStorage& fs, const String& name, const SparseMat& m )
{
    char dt[22];

    fs.startWriteStruct(name, FileNode::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ціле dims = m.dims();
    якщо( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*sizeof(ціле) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<const SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    для( ; it != it_end; ++it )
    {
        CV_Assert(it.node() != 0);
        elems[i++] = it.node();
    }

    std::sort(elems.begin(), elems.end(), SparseNodeCmp(dims));
    const SparseMat::Node* prev_node = 0;
    size_t esz = m.elemSize();

    для( i = 0; i < n; i++ )
    {
        const SparseMat::Node* node = elems[i];
        ціле k = 0;

        якщо( prev_node )
        {
            для( ; k < dims; k++ )
                якщо( node->idx[k] != prev_node->idx[k] )
                    break;
            CV_Assert( k < dims );
            якщо( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        для( ; k < dims; k++ )
            writeScalar( fs, node->idx[k] );
        prev_node = node;

        const uchar* value = &m.value<uchar>(node);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

void read(const FileNode& node, Mat& m, const Mat& default_mat)
{
    якщо( node.empty() )
    {
        default_mat.copyTo(m);
        повернути;
    }

    std::string dt;
    ціле rows, cols, elem_type;

    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    read(node["rows"], rows, -1);
    якщо( rows >= 0 )
    {
        read(node["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    інакше
    {
        ціле sizes[CV_MAX_DIM] = {0}, dims;
        FileNode sizes_node = node["sizes"];
        CV_Assert( !sizes_node.empty() );

        dims = (ціле)sizes_node.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_node.readRaw("i", sizes, dims*sizeof(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    FileNode data_node = node["data"];
    CV_Assert(!data_node.empty());

    size_t nelems = data_node.size();
    якщо (nelems == 0)
    {
        m = Mat();
        повернути;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_node.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

void read( const FileNode& node, SparseMat& m, const SparseMat& default_mat )
{
    якщо( node.empty() )
    {
        default_mat.copyTo(m);
        повернути;
    }

    std::string dt;
    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ціле elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ціле sizes[CV_MAX_DIM] = {0};
    FileNode sizes_node = node["sizes"];
    CV_Assert( !sizes_node.empty() );

    ціле dims = (ціле)sizes_node.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_node.readRaw("i", sizes, dims*sizeof(sizes[0]));

    m.create(dims, sizes, elem_type);

    FileNode data = node["data"];
    CV_Assert( data.isSeq() );

    ціле cn = CV_MAT_CN(elem_type);
    ціле idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    FileNodeIterator it = data.begin();

    для( i = 0; i < sz; )
    {
        FileNode n = *it;
        ціле k = (ціле)n;
        якщо( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        інакше
        {
            якщо( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            інакше
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            для( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ціле idx_k = (ціле)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, true);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

void read(const FileNode& node, KeyPoint& value, const KeyPoint& default_value)
{
    якщо( node.empty() )
    {
        value = default_value;
        повернути;
    }
    node >> value;
}

void read(const FileNode& node, DMatch& value, const DMatch& default_value)
{
    якщо( node.empty() )
    {
        value = default_value;
        повернути;
    }
    node >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
void write( FileStorage& fs, const std::string& name, const std::vector<KeyPoint>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ);
    write(fs, vec);
}

void read(const FileNode& node, std::vector<KeyPoint>& keypoints)
{
    FileNode first_node = *(node.begin());
    якщо (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        для (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#інакше
        FileNodeIterator it = node.begin();
        it >> keypoints;
#endif
        повернути;
    }
    keypoints.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    для( ; it != it_end; )
    {
        KeyPoint kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

void write( FileStorage& fs, const std::string& name, const std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ);
    write(fs, vec);
}

void read(const FileNode& node, std::vector<DMatch>& matches)
{
    FileNode first_node = *(node.begin());
    якщо (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        для (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#інакше
        FileNodeIterator it = node.begin();
        it >> matches;
#endif
        повернути;
    }
    matches.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    для( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
