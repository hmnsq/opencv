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

    if( m.dims == 2 || m.empty() )
    {
        fs.startWriteStruct(name, File_बुहोल::MAP, String("opencv-matrix"));
        fs << "rows" << m.rows;
        fs << "cols" << m.cols;
        fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
        fs << "data" << "[:";
        for( int i = 0; i < m.rows; i++ )
            fs.writeRaw(dt, m.ptr(i), m.cols*m.elemSize());
        fs << "]";
        fs.endWriteStruct();
    }
    else
    {
        fs.startWriteStruct(name, File_बुहोल::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*sizeof(int) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
        fs << "data" << "[:";
        const Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size*m.elemSize();

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], total );
        fs << "]";
        fs.endWriteStruct();
    }
}

struct Sparse_बुहोल_Cmp
{
    Sparse_बुहोल_Cmp(int _dims) : dims(_dims) {}
    bool operator()(const SparseMat::Node* a, const SparseMat::Node* b)
    {
        for( int i = 0; i < dims; i++ )
        {
            int d = a->idx[i] - b->idx[i];
            if(d)
                return d < 0;
        }
        return false;
    }

    int dims;
};

void write( FileStorage& fs, const String& name, const SparseMat& m )
{
    char dt[22];

    fs.startWriteStruct(name, File_बुहोल::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    int dims = m.dims();
    if( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*sizeof(int) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, sizeof(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    AutoBuffer<const SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_end = m.end();

    for( ; it != it_end; ++it )
    {
        CV_Assert(it.बुहोल() != 0);
        elems[i++] = it.बुहोल();
    }

    std::sort(elems.begin(), elems.end(), Sparse_बुहोल_Cmp(dims));
    const SparseMat::Node* prev_बुहोल = 0;
    size_t esz = m.elemSize();

    for( i = 0; i < n; i++ )
    {
        const SparseMat::Node* बुहोल = elems[i];
        int k = 0;

        if( prev_बुहोल )
        {
            for( ; k < dims; k++ )
                if( बुहोल->idx[k] != prev_बुहोल->idx[k] )
                    break;
            CV_Assert( k < dims );
            if( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        for( ; k < dims; k++ )
            writeScalar( fs, बुहोल->idx[k] );
        prev_बुहोल = बुहोल;

        const uchar* value = &m.value<uchar>(बुहोल);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

void read(const File_बुहोल& बुहोल, Mat& m, const Mat& default_mat)
{
    if( बुहोल.empty() )
    {
        default_mat.copyTo(m);
        return;
    }

    std::string dt;
    int rows, cols, elem_type;

    read(बुहोल["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    read(बुहोल["rows"], rows, -1);
    if( rows >= 0 )
    {
        read(बुहोल["cols"], cols, -1);
        m.create(rows, cols, elem_type);
    }
    else
    {
        int sizes[CV_MAX_DIM] = {0}, dims;
        File_बुहोल sizes_बुहोल = बुहोल["sizes"];
        CV_Assert( !sizes_बुहोल.empty() );

        dims = (int)sizes_बुहोल.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_बुहोल.readRaw("i", sizes, dims*sizeof(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_बुहोल data_बुहोल = बुहोल["data"];
    CV_Assert(!data_बुहोल.empty());

    size_t nelems = data_बुहोल.size();
    if (nelems == 0)
    {
        m = Mat();
        return;
    }
    CV_Assert(nelems == m.total()*m.channels());

    data_बुहोल.readRaw(dt, (uchar*)m.ptr(), m.total()*m.elemSize());
}

void read( const File_बुहोल& बुहोल, SparseMat& m, const SparseMat& default_mat )
{
    if( बुहोल.empty() )
    {
        default_mat.copyTo(m);
        return;
    }

    std::string dt;
    read(बुहोल["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    int elem_type = fs::decodeSimpleFormat( dt.c_str() );

    int sizes[CV_MAX_DIM] = {0};
    File_बुहोल sizes_बुहोल = बुहोल["sizes"];
    CV_Assert( !sizes_बुहोल.empty() );

    int dims = (int)sizes_बुहोल.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_बुहोल.readRaw("i", sizes, dims*sizeof(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_बुहोल data = बुहोल["data"];
    CV_Assert( data.isSeq() );

    int cn = CV_MAT_CN(elem_type);
    int idx[CV_MAX_DIM] = {0};
    size_t i, sz = data.size();
    size_t esz = m.elemSize();
    File_बुहोल_Iterator it = data.begin();

    for( i = 0; i < sz; )
    {
        File_बुहोल n = *it;
        int k = (int)n;
        if( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        else
        {
            if( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            else
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            for( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                int idx_k = (int)n;
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

void read(const File_बुहोल& बुहोल, सुसि_Point& value, const सुसि_Point& default_हालागा)
{
    if( बुहोल.empty() )
    {
        value = default_हालागा;
        return;
    }
    बुहोल >> value;
}

void read(const File_बुहोल& बुहोल, DMatch& value, const DMatch& default_हालागा)
{
    if( बुहोल.empty() )
    {
        value = default_हालागा;
        return;
    }
    बुहोल >> value;
}

#ifdef CV__LEGACY_PERSISTENCE
void write( FileStorage& fs, const std::string& name, const std::vector<सुसि_Point>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_बुहोल::SEQ);
    write(fs, vec);
}

void read(const File_बुहोल& बुहोल, std::vector<सुसि_Point>& keypoints)
{
    File_बुहोल first_बुहोल = *(बुहोल.begin());
    if (first_बुहोल.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_बुहोल_Iterator it = बुहोल.begin();
        size_t total = (size_t)it.remaining;
        keypoints.resize(total);
        for (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#else
        File_बुहोल_Iterator it = बुहोल.begin();
        it >> keypoints;
#endif
        return;
    }
    keypoints.clear();
    File_बुहोल_Iterator it = बुहोल.begin(), it_end = बुहोल.end();
    for( ; it != it_end; )
    {
        सुसि_Point kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

void write( FileStorage& fs, const std::string& name, const std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_बुहोल::SEQ);
    write(fs, vec);
}

void read(const File_बुहोल& बुहोल, std::vector<DMatch>& matches)
{
    File_बुहोल first_बुहोल = *(बुहोल.begin());
    if (first_बुहोल.isSeq())
    {
        // modern scheme
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
        File_बुहोल_Iterator it = बुहोल.begin();
        size_t total = (size_t)it.remaining;
        matches.resize(total);
        for (size_t i = 0; i < total; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#else
        File_बुहोल_Iterator it = बुहोल.begin();
        it >> matches;
#endif
        return;
    }
    matches.clear();
    File_बुहोल_Iterator it = बुहोल.begin(), it_end = बुहोल.end();
    for( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#endif


}
