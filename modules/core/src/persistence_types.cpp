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

    если( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
        fs << "data" << "[:";
        для( целое i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    иначе
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*sizeof(целое) );
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
    SparseNodeCmp(целое _dims) : dims(_dims) {}
    bool operator()(const SparseMat::Node* a, const SparseMat::Node* b)
    {
        для( целое i = 0; i < dims; i++ )
        {
            целое d = a->idx[i] - b->idx[i];
            если(d)
                вернуть d < 0;
        }
        вернуть false;
    }

    целое dims;
};

void write( FileStorage& fs, const String& name, const SparseMat& m )
{
    char dt[22];

    fs.startWriteStruct(name, FileNode::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    целое dims = m.dims();
    если( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*sizeof(целое) );
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
        целое k = 0;

        если( prev_node )
        {
            для( ; k < dims; k++ )
                если( node->idx[k] != prev_node->idx[k] )
                    break;
            CV_Assert( k < dims );
            если( k < dims - 1 )
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
    если( node.empty() )
    {
        default_mat.copyTo(m);
        вернуть;
    }

    std::string dt;
    целое rows, cols, elem_type;

    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    read(node["rows"], rows, -1);
    если( rows >= 0 )
    {
        read(node["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    иначе
    {
        целое sizes[CV_MAX_DIM] = {0}, dims;
        FileNode sizes_node = node["sizes"];
        CV_Assert( !sizes_node.empty() );

        dims = (целое)sizes_node.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_node.readRaw("i", sizes, dims*sizeof(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    FileNode data_node = node["data"];
    CV_Assert(!data_node.empty());

    size_t nelems = data_node.size();
    если (nelems == 0)
    {
        m = Mat();
        вернуть;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_node.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

void read( const FileNode& node, SparseMat& m, const SparseMat& default_mat )
{
    если( node.empty() )
    {
        default_mat.copyTo(m);
        вернуть;
    }

    std::string dt;
    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    целое elem_type = fs::decodeSimpleFormat( dt.c_str() );

    целое sizes[CV_MAX_DIM] = {0};
    FileNode sizes_node = node["sizes"];
    CV_Assert( !sizes_node.empty() );

    целое dims = (целое)sizes_node.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_node.readRaw("i", sizes, dims*sizeof(sizes[0]));

    m.create(dims, sizes, elem_type);

    FileNode data = node["data"];
    CV_Assert( data.isSeq() );

    целое cn = CV_MAT_CN(elem_type);
    целое idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    FileNodeIterator it = data.begin();

    для( i = 0; i < sz; )
    {
        FileNode n = *it;
        целое k = (целое)n;
        если( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        иначе
        {
            если( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            иначе
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
                целое idx_k = (целое)n;
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
    если( node.empty() )
    {
        value = default_value;
        вернуть;
    }
    node >> value;
}

void read(const FileNode& node, DMatch& value, const DMatch& default_value)
{
    если( node.empty() )
    {
        value = default_value;
        вернуть;
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
    если (first_node.isSeq())
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
#иначе
        FileNodeIterator it = node.begin();
        it >> keypoints;
#endif
        вернуть;
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
    если (first_node.isSeq())
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
#иначе
        FileNodeIterator it = node.begin();
        it >> matches;
#endif
        вернуть;
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
