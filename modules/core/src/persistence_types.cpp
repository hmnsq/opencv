// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

namespace cv
{

void write( FileStorage& fs, const String& name, const Mat& m )
{
    var_na dt[22];

    ຖ້າ( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
        fs << "data" << "[:";
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*parimaa_na(ຈຳນວນເຕັມ) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
        fs << "data" << "[:";
        const Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

sa_nracanaa SparseNodeCmp
{
    SparseNodeCmp(ຈຳນວນເຕັມ _dims) : dims(_dims) {}
    bool operator()(const SparseMat::Node* a, const SparseMat::Node* b)
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < dims; i++ )
        {
            ຈຳນວນເຕັມ d = a->idx[i] - b->idx[i];
            ຖ້າ(d)
                ສົ່ງຄືນ d < 0;
        }
        ສົ່ງຄືນ false;
    }

    ຈຳນວນເຕັມ dims;
};

void write( FileStorage& fs, const String& name, const SparseMat& m )
{
    var_na dt[22];

    fs.startWriteStruct(name, FileNode::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ຈຳນວນເຕັມ dims = m.dims();
    ຖ້າ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*parimaa_na(ຈຳນວນເຕັມ) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<const SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    ສຳລັບ( ; it != it_end; ++it )
    {
        CV_Assert(it.node() != 0);
        elems[i++] = it.node();
    }

    std::sort(elems.begin(), elems.end(), SparseNodeCmp(dims));
    const SparseMat::Node* prev_node = 0;
    size_t esz = m.elemSize();

    ສຳລັບ( i = 0; i < n; i++ )
    {
        const SparseMat::Node* node = elems[i];
        ຈຳນວນເຕັມ k = 0;

        ຖ້າ( prev_node )
        {
            ສຳລັບ( ; k < dims; k++ )
                ຖ້າ( node->idx[k] != prev_node->idx[k] )
                    bha_nga;
            CV_Assert( k < dims );
            ຖ້າ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        ສຳລັບ( ; k < dims; k++ )
            writeScalar( fs, node->idx[k] );
        prev_node = node;

        const uchar* value = &m.value<uchar>(node);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

void read(const FileNode& node, Mat& m, const Mat& default_mat)
{
    ຖ້າ( node.empty() )
    {
        default_mat.copyTo(m);
        ສົ່ງຄືນ;
    }

    std::string dt;
    ຈຳນວນເຕັມ rows, cols, elem_type;

    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    read(node["rows"], rows, -1);
    ຖ້າ( rows >= 0 )
    {
        read(node["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຈຳນວນເຕັມ sizes[CV_MAX_DIM] = {0}, dims;
        FileNode sizes_node = node["sizes"];
        CV_Assert( !sizes_node.empty() );

        dims = (ຈຳນວນເຕັມ)sizes_node.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_node.readRaw("i", sizes, dims*parimaa_na(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    FileNode data_node = node["data"];
    CV_Assert(!data_node.empty());

    size_t nelems = data_node.size();
    ຖ້າ (nelems == 0)
    {
        m = Mat();
        ສົ່ງຄືນ;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_node.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

void read( const FileNode& node, SparseMat& m, const SparseMat& default_mat )
{
    ຖ້າ( node.empty() )
    {
        default_mat.copyTo(m);
        ສົ່ງຄືນ;
    }

    std::string dt;
    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ຈຳນວນເຕັມ elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ຈຳນວນເຕັມ sizes[CV_MAX_DIM] = {0};
    FileNode sizes_node = node["sizes"];
    CV_Assert( !sizes_node.empty() );

    ຈຳນວນເຕັມ dims = (ຈຳນວນເຕັມ)sizes_node.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_node.readRaw("i", sizes, dims*parimaa_na(sizes[0]));

    m.create(dims, sizes, elem_type);

    FileNode data = node["data"];
    CV_Assert( data.isSeq() );

    ຈຳນວນເຕັມ cn = CV_MAT_CN(elem_type);
    ຈຳນວນເຕັມ idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    FileNodeIterator it = data.begin();

    ສຳລັບ( i = 0; i < sz; )
    {
        FileNode n = *it;
        ຈຳນວນເຕັມ k = (ຈຳນວນເຕັມ)n;
        ຖ້າ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ຖ້າ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            ສຳລັບ( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ຈຳນວນເຕັມ idx_k = (ຈຳນວນເຕັມ)n;
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
    ຖ້າ( node.empty() )
    {
        value = default_value;
        ສົ່ງຄືນ;
    }
    node >> value;
}

void read(const FileNode& node, DMatch& value, const DMatch& default_value)
{
    ຖ້າ( node.empty() )
    {
        value = default_value;
        ສົ່ງຄືນ;
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
    ຖ້າ (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        ສຳລັບ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#ບໍ່ດັ່ງນັ້ນ
        FileNodeIterator it = node.begin();
        it >> keypoints;
#endif
        ສົ່ງຄືນ;
    }
    keypoints.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    ສຳລັບ( ; it != it_end; )
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
    ຖ້າ (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        ສຳລັບ (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#ບໍ່ດັ່ງນັ້ນ
        FileNodeIterator it = node.begin();
        it >> matches;
#endif
        ສົ່ງຄືນ;
    }
    matches.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    ສຳລັບ( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
