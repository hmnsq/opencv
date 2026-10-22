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

    အကယ်၍( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
        fs << "data" << "[:";
        အတွက်( ကိန်းပြည့် i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    မဟုတ်ပါက
    {
        fs.startWriteStruct(name, FileNode::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*parimaa_na(ကိန်းပြည့်) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
        fs << "data" << "[:";
        const Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

sa_nracanaa SparseNodeCmp
{
    SparseNodeCmp(ကိန်းပြည့် _dims) : dims(_dims) {}
    bool operator()(const SparseMat::Node* a, const SparseMat::Node* b)
    {
        အတွက်( ကိန်းပြည့် i = 0; i < dims; i++ )
        {
            ကိန်းပြည့် d = a->idx[i] - b->idx[i];
            အကယ်၍(d)
                ပြန်ပေး d < 0;
        }
        ပြန်ပေး false;
    }

    ကိန်းပြည့် dims;
};

void write( FileStorage& fs, const String& name, const SparseMat& m )
{
    var_na dt[22];

    fs.startWriteStruct(name, FileNode::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    ကိန်းပြည့် dims = m.dims();
    အကယ်၍( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*parimaa_na(ကိန်းပြည့်) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, parimaa_na(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<const SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    အတွက်( ; it != it_end; ++it )
    {
        CV_Assert(it.node() != 0);
        elems[i++] = it.node();
    }

    std::sort(elems.begin(), elems.end(), SparseNodeCmp(dims));
    const SparseMat::Node* prev_node = 0;
    size_t esz = m.elemSize();

    အတွက်( i = 0; i < n; i++ )
    {
        const SparseMat::Node* node = elems[i];
        ကိန်းပြည့် k = 0;

        အကယ်၍( prev_node )
        {
            အတွက်( ; k < dims; k++ )
                အကယ်၍( node->idx[k] != prev_node->idx[k] )
                    bha_nga;
            CV_Assert( k < dims );
            အကယ်၍( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        အတွက်( ; k < dims; k++ )
            writeScalar( fs, node->idx[k] );
        prev_node = node;

        const uchar* value = &m.value<uchar>(node);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

void read(const FileNode& node, Mat& m, const Mat& default_mat)
{
    အကယ်၍( node.empty() )
    {
        default_mat.copyTo(m);
        ပြန်ပေး;
    }

    std::string dt;
    ကိန်းပြည့် rows, cols, elem_type;

    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    read(node["rows"], rows, -1);
    အကယ်၍( rows >= 0 )
    {
        read(node["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    မဟုတ်ပါက
    {
        ကိန်းပြည့် sizes[CV_MAX_DIM] = {0}, dims;
        FileNode sizes_node = node["sizes"];
        CV_Assert( !sizes_node.empty() );

        dims = (ကိန်းပြည့်)sizes_node.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_node.readRaw("i", sizes, dims*parimaa_na(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    FileNode data_node = node["data"];
    CV_Assert(!data_node.empty());

    size_t nelems = data_node.size();
    အကယ်၍ (nelems == 0)
    {
        m = Mat();
        ပြန်ပေး;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_node.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

void read( const FileNode& node, SparseMat& m, const SparseMat& default_mat )
{
    အကယ်၍( node.empty() )
    {
        default_mat.copyTo(m);
        ပြန်ပေး;
    }

    std::string dt;
    read(node["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    ကိန်းပြည့် elem_type = fs::decodeSimpleFormat( dt.c_str() );

    ကိန်းပြည့် sizes[CV_MAX_DIM] = {0};
    FileNode sizes_node = node["sizes"];
    CV_Assert( !sizes_node.empty() );

    ကိန်းပြည့် dims = (ကိန်းပြည့်)sizes_node.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_node.readRaw("i", sizes, dims*parimaa_na(sizes[0]));

    m.create(dims, sizes, elem_type);

    FileNode data = node["data"];
    CV_Assert( data.isSeq() );

    ကိန်းပြည့် cn = CV_MAT_CN(elem_type);
    ကိန်းပြည့် idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    FileNodeIterator it = data.begin();

    အတွက်( i = 0; i < sz; )
    {
        FileNode n = *it;
        ကိန်းပြည့် k = (ကိန်းပြည့်)n;
        အကယ်၍( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        မဟုတ်ပါက
        {
            အကယ်၍( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            မဟုတ်ပါက
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            အတွက်( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                ကိန်းပြည့် idx_k = (ကိန်းပြည့်)n;
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
    အကယ်၍( node.empty() )
    {
        value = default_value;
        ပြန်ပေး;
    }
    node >> value;
}

void read(const FileNode& node, DMatch& value, const DMatch& default_value)
{
    အကယ်၍( node.empty() )
    {
        value = default_value;
        ပြန်ပေး;
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
    အကယ်၍ (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        အတွက် (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#မဟုတ်ပါက
        FileNodeIterator it = node.begin();
        it >> keypoints;
#endif
        ပြန်ပေး;
    }
    keypoints.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    အတွက်( ; it != it_end; )
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
    အကယ်၍ (first_node.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        FileNodeIterator it = node.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        အတွက် (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#မဟုတ်ပါက
        FileNodeIterator it = node.begin();
        it >> matches;
#endif
        ပြန်ပေး;
    }
    matches.clear();
    FileNodeIterator it = node.begin(), it_end = node.end();
    အတွက်( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
