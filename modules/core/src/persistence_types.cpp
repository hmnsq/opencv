// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence.hpp"

నామాకాశ cv
{

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, స్థిరరాశి Mat& m )
{
    అక్షరం dt[22];

    ఒకవేళ( m.dims == 2 || m.empty() )
    {
        fs.ప్రారంభం_Write_Struct(name, File_కణుపు::MAP, String("opencv-matrix"));
        fs << "rows" << m.అడ్డువరుసలు;
        fs << "cols" << m.నిలువుల_సంఖ్య;
        fs << "dt" << fs::encodeFormat( m.type(), dt, పరిమాణం(dt) );
        fs << "data" << "[:";
        కొరకు( పూర్ణాంకం i = 0; i < m.అడ్డువరుసలు; i++ )
            fs.writeRaw(dt, m.ptr(i), m.నిలువుల_సంఖ్య*m.elem_కొలత());
        fs << "]";
        fs.చివర_Write_Struct();
    }
    లేకపోతే
    {
        fs.ప్రారంభం_Write_Struct(name, File_కణుపు::MAP, String("opencv-nd-matrix"));
        fs << "sizes" << "[:";
        fs.writeRaw( "i", m.size.p, m.dims*పరిమాణం(పూర్ణాంకం) );
        fs << "]";
        fs << "dt" << fs::encodeFormat( m.type(), dt, పరిమాణం(dt) );
        fs << "data" << "[:";
        స్థిరరాశి Mat* arrays[] = {&m, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        size_t మొత్తం = it.size*m.elem_కొలత();

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            fs.writeRaw( dt, ptrs[0], మొత్తం );
        fs << "]";
        fs.చివర_Write_Struct();
    }
}

నిర్మాణం Sparse_కణుపు_Cmp
{
    Sparse_కణుపు_Cmp(పూర్ణాంకం _dims) : dims(_dims) {}
    తార్కిక క్రియాకారకం()(స్థిరరాశి SparseMat::Node* a, స్థిరరాశి SparseMat::Node* b)
    {
        కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
        {
            పూర్ణాంకం d = a->idx[i] - b->idx[i];
            ఒకవేళ(d)
                తిరిగి d < 0;
        }
        తిరిగి అబద్ధం;
    }

    పూర్ణాంకం dims;
};

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, స్థిరరాశి SparseMat& m )
{
    అక్షరం dt[22];

    fs.ప్రారంభం_Write_Struct(name, File_కణుపు::MAP, String("opencv-sparse-matrix"));
    fs << "sizes" << "[:";
    పూర్ణాంకం dims = m.dims();
    ఒకవేళ( dims > 0 )
        fs.writeRaw("i", m.hdr->size, dims*పరిమాణం(పూర్ణాంకం) );
    fs << "]";
    fs << "dt" << fs::encodeFormat( m.type(), dt, పరిమాణం(dt) );
    fs << "data" << "[:";

    size_t i = 0, n = m.nzcount();
    Auto_బఫరు<స్థిరరాశి SparseMat::Node*> elems(n);
    SparseMatConstIterator it = m.begin(), it_చివర = m.end();

    కొరకు( ; it != it_చివర; ++it )
    {
        CV_Assert(it.కణుపు() != 0);
        elems[i++] = it.కణుపు();
    }

    std::sort(elems.begin(), elems.end(), Sparse_కణుపు_Cmp(dims));
    స్థిరరాశి SparseMat::Node* prev_కణుపు = 0;
    size_t esz = m.elem_కొలత();

    కొరకు( i = 0; i < n; i++ )
    {
        స్థిరరాశి SparseMat::Node* కణుపు = elems[i];
        పూర్ణాంకం k = 0;

        ఒకవేళ( prev_కణుపు )
        {
            కొరకు( ; k < dims; k++ )
                ఒకవేళ( కణుపు->idx[k] != prev_కణుపు->idx[k] )
                    ఆపు;
            CV_Assert( k < dims );
            ఒకవేళ( k < dims - 1 )
                writeScalar( fs, k - dims + 1 );
        }
        కొరకు( ; k < dims; k++ )
            writeScalar( fs, కణుపు->idx[k] );
        prev_కణుపు = కణుపు;

        స్థిరరాశి uchar* value = &m.value<uchar>(కణుపు);
        fs.writeRaw(dt, value, esz);
    }

    fs << "]" << "}";
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, Mat& m, స్థిరరాశి Mat& default_mat)
{
    ఒకవేళ( కణుపు.empty() )
    {
        default_mat.copy_గమ్యం(m);
        తిరిగి;
    }

    std::string dt;
    పూర్ణాంకం అడ్డువరుసలు, నిలువుల_సంఖ్య, elem_type;

    పఠ(కణుపు["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    elem_type = fs::decodeSimpleFormat( dt.c_str() );

    పఠ(కణుపు["rows"], అడ్డువరుసలు, -1);
    ఒకవేళ( అడ్డువరుసలు >= 0 )
    {
        పఠ(కణుపు["cols"], నిలువుల_సంఖ్య, -1);
        m.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, elem_type);
    }
    లేకపోతే
    {
        పూర్ణాంకం sizes[CV_MAX_DIM] = {0}, dims;
        File_కణుపు sizes_కణుపు = కణుపు["sizes"];
        CV_Assert( !sizes_కణుపు.empty() );

        dims = (పూర్ణాంకం)sizes_కణుపు.size();
        CV_Assert( dims >= 0 && dims <= CV_MAX_DIM );
        sizes_కణుపు.readRaw("i", sizes, dims*పరిమాణం(sizes[0]));

        m.create(dims, sizes, elem_type);
    }

    File_కణుపు దత్తాంశం_కణుపు = కణుపు["data"];
    CV_Assert(!దత్తాంశం_కణుపు.empty());

    size_t nelems = దత్తాంశం_కణుపు.size();
    ఒకవేళ (nelems == 0)
    {
        m = Mat();
        తిరిగి;
    }
    CV_Assert(nelems == m.మొత్తం()*m.channels());

    దత్తాంశం_కణుపు.readRaw(dt, (uchar*)m.ptr(), m.మొత్తం()*m.elem_కొలత());
}

శూన్య పఠ( స్థిరరాశి File_కణుపు& కణుపు, SparseMat& m, స్థిరరాశి SparseMat& default_mat )
{
    ఒకవేళ( కణుపు.empty() )
    {
        default_mat.copy_గమ్యం(m);
        తిరిగి;
    }

    std::string dt;
    పఠ(కణుపు["dt"], dt, std::string());
    CV_Assert( !dt.empty() );

    పూర్ణాంకం elem_type = fs::decodeSimpleFormat( dt.c_str() );

    పూర్ణాంకం sizes[CV_MAX_DIM] = {0};
    File_కణుపు sizes_కణుపు = కణుపు["sizes"];
    CV_Assert( !sizes_కణుపు.empty() );

    పూర్ణాంకం dims = (పూర్ణాంకం)sizes_కణుపు.size();
    CV_Assert( dims > 0 && dims <= CV_MAX_DIM );
    sizes_కణుపు.readRaw("i", sizes, dims*పరిమాణం(sizes[0]));

    m.create(dims, sizes, elem_type);

    File_కణుపు దత్తాంశం = కణుపు["data"];
    CV_Assert( దత్తాంశం.isSeq() );

    పూర్ణాంకం cn = CV_MAT_CN(elem_type);
    పూర్ణాంకం idx[CV_MAX_DIM] = {0};
    size_t i, sz = దత్తాంశం.size();
    size_t esz = m.elem_కొలత();
    File_కణుపు_Iterator it = దత్తాంశం.begin();

    కొరకు( i = 0; i < sz; )
    {
        File_కణుపు n = *it;
        పూర్ణాంకం k = (పూర్ణాంకం)n;
        ఒకవేళ( i > 0 && k >= 0 )
        {
            CV_Assert( k < sizes[dims-1] );
            idx[dims-1] = k;
        }
        లేకపోతే
        {
            ఒకవేళ( i > 0 )
            {
                // write() stores the first changed dimension as k - dims + 1
                k = dims + k - 1;
                CV_Assert( k >= 0 );
            }
            లేకపోతే
            {
                CV_Assert( k >= 0 && k < sizes[0] );
                idx[0] = k, k = 1;
            }
            కొరకు( ; k < dims; k++ )
            {
                ++it;
                i++;
                n = *it;
                CV_Assert( n.isInt() );
                పూర్ణాంకం idx_k = (పూర్ణాంకం)n;
                CV_Assert( idx_k >= 0 && idx_k < sizes[k] );
                idx[k] = idx_k;
            }
        }
        ++it;
        i++;
        uchar* valptr = m.ptr(idx, నిజం);
        it.readRaw(dt, valptr, esz);
        i += cn;
    }
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, కీలకం_బిందువు& value, స్థిరరాశి కీలకం_బిందువు& default_విలువ)
{
    ఒకవేళ( కణుపు.empty() )
    {
        value = default_విలువ;
        తిరిగి;
    }
    కణుపు >> value;
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, DMatch& value, స్థిరరాశి DMatch& default_విలువ)
{
    ఒకవేళ( కణుపు.empty() )
    {
        value = default_విలువ;
        తిరిగి;
    }
    కణుపు >> value;
}

#నిర్వచితమైతే CV__LEGACY_PERSISTENCE
శూన్య లిఖ( FileStorage& fs, స్థిరరాశి std::string& name, స్థిరరాశి std::vector<కీలకం_బిందువు>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_కణుపు::SEQ);
    లిఖ(fs, vec);
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, std::vector<కీలకం_బిందువు>& keypoints)
{
    File_కణుపు మొదటి_కణుపు = *(కణుపు.begin());
    ఒకవేళ (మొదటి_కణుపు.isSeq())
    {
        // modern scheme
#నిర్వచితమైతే OPENCV_TRAITS_ENABLE_DEPRECATED
        File_కణుపు_Iterator it = కణుపు.begin();
        size_t మొత్తం = (size_t)it.మిగిలిన;
        keypoints.resize(మొత్తం);
        కొరకు (size_t i = 0; i < మొత్తం; ++i, ++it)
        {
            (*it) >> keypoints[i];
        }
#లేకపోతే
        File_కణుపు_Iterator it = కణుపు.begin();
        it >> keypoints;
#అయితే_ముగింపు
        తిరిగి;
    }
    keypoints.clear();
    File_కణుపు_Iterator it = కణుపు.begin(), it_చివర = కణుపు.end();
    కొరకు( ; it != it_చివర; )
    {
        కీలకం_బిందువు kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి std::string& name, స్థిరరాశి std::vector<DMatch>& vec)
{
    // from template implementation
    cv::internal::WriteStructContext ws(fs, name, File_కణుపు::SEQ);
    లిఖ(fs, vec);
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, std::vector<DMatch>& matches)
{
    File_కణుపు మొదటి_కణుపు = *(కణుపు.begin());
    ఒకవేళ (మొదటి_కణుపు.isSeq())
    {
        // modern scheme
#నిర్వచితమైతే OPENCV_TRAITS_ENABLE_DEPRECATED
        File_కణుపు_Iterator it = కణుపు.begin();
        size_t మొత్తం = (size_t)it.మిగిలిన;
        matches.resize(మొత్తం);
        కొరకు (size_t i = 0; i < మొత్తం; ++i, ++it)
        {
            (*it) >> matches[i];
        }
#లేకపోతే
        File_కణుపు_Iterator it = కణుపు.begin();
        it >> matches;
#అయితే_ముగింపు
        తిరిగి;
    }
    matches.clear();
    File_కణుపు_Iterator it = కణుపు.begin(), it_చివర = కణుపు.end();
    కొరకు( ; it != it_చివర; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}
#అయితే_ముగింపు


}
