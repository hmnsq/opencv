// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ধ্রুবক Mat** _arrays, Mat* _planes, পূর্ণসংখ্যা _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ধ্রুবক Mat** _arrays, uchar** _ptrs, পূর্ণসংখ্যা _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

শূন্য NAryMatIterator::init(ধ্রুবক Mat** _arrays, Mat* _planes, uchar** _ptrs, পূর্ণসংখ্যা _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    পূর্ণসংখ্যা i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    যদি( narrays < 0 )
    {
        জন্য( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    জন্য( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ধ্রুবক Mat& A = *arrays[i];
        যদি( ptrs )
            ptrs[i] = A.তথ্য;

        যদি( !A.তথ্য )
            চালু;

        যদি( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            জন্য( d1 = 0; d1 < d; d1++ )
                যদি( A.size[d1] > 1 )
                    বিরতি;
        }
        অন্যথা
            CV_Assert( A.size == arrays[i0]->size );

        যদি( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elem_আকার() );
            জন্য( j = d-1; j > d1; j-- )
                যদি( A.step[j]*A.size[j] < A.step[j-1] )
                    বিরতি;
            iterdepth = std::max(iterdepth, j);
        }
    }

    যদি( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        জন্য( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            যদি( total1 != (পূর্ণসংখ্যা)total1 )
                বিরতি;
            size = (পূর্ণসংখ্যা)total1;
        }

        iterdepth = j;
        যদি( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        জন্য( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    অন্যথা
        iterdepth = 0;

    idx = 0;

    যদি( !planes )
        ফেরত;

    জন্য( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ধ্রুবক Mat& A = *arrays[i];

        যদি( !A.তথ্য )
        {
            planes[i] = Mat();
            চালু;
        }

        planes[i] = Mat(1, (পূর্ণসংখ্যা)size, A.type(), A.তথ্য);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    যদি( idx >= nplanes-1 )
        ফেরত *এই;
    ++idx;

    যদি( iterdepth == 1 )
    {
        যদি( ptrs )
        {
            জন্য( পূর্ণসংখ্যা i = 0; i < narrays; i++ )
            {
                যদি( !ptrs[i] )
                    চালু;
                ptrs[i] = arrays[i]->তথ্য + arrays[i]->step[0]*idx;
            }
        }
        যদি( planes )
        {
            জন্য( পূর্ণসংখ্যা i = 0; i < narrays; i++ )
            {
                যদি( !planes[i].তথ্য )
                    চালু;
                planes[i].তথ্য = arrays[i]->তথ্য + arrays[i]->step[0]*idx;
            }
        }
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < narrays; i++ )
        {
            ধ্রুবক Mat& A = *arrays[i];
            যদি( !A.তথ্য )
                চালু;
            পূর্ণসংখ্যা _idx = (পূর্ণসংখ্যা)idx;
            uchar* তথ্য = A.তথ্য;
            জন্য( পূর্ণসংখ্যা j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                পূর্ণসংখ্যা szi = A.size[j], t = _idx/szi;
                তথ্য += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            যদি( ptrs )
                ptrs[i] = তথ্য;
            যদি( planes )
                planes[i].তথ্য = তথ্য;
        }
    }

    ফেরত *এই;
}

NAryMatIterator NAryMatIterator::operator ++(পূর্ণসংখ্যা)
{
    NAryMatIterator it = *এই;
    ++*এই;
    ফেরত it;
}

//==================================================================================================

Point MatConstIterator::pos() ধ্রুবক
{
    যদি( !m )
        ফেরত Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    পূর্ণসংখ্যা y = (পূর্ণসংখ্যা)(ofs/m->step[0]);
    ফেরত Point((পূর্ণসংখ্যা)((ofs - y*m->step[0])/elem_আকার), y);
}

শূন্য MatConstIterator::pos(পূর্ণসংখ্যা* _idx) ধ্রুবক
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    জন্য( পূর্ণসংখ্যা i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (পূর্ণসংখ্যা)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ধ্রুবক
{
    যদি(!m)
        ফেরত 0;
    যদি( m->isContinuous() )
        ফেরত (ptr - sliceStart)/elem_আকার;
    ptrdiff_t ofs = ptr - m->ptr();
    পূর্ণসংখ্যা i, d = m->dims;
    যদি( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ফেরত y*m->cols + (ofs - y*m->step[0])/elem_আকার;
    }
    ptrdiff_t ফলাফল = 0;
    জন্য( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        ফলাফল = ফলাফল*m->size[i] + v;
    }
    ফেরত ফলাফল;
}

শূন্য MatConstIterator::seek(ptrdiff_t ofs, যৌক্তিক relative)
{
    যদি( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elem_আকার;
        যদি( ptr < sliceStart )
            ptr = sliceStart;
        অন্যথা যদি( ptr > sliceEnd )
            ptr = sliceEnd;
        ফেরত;
    }

    পূর্ণসংখ্যা d = m->dims;
    যদি( d == 2 )
    {
        ptrdiff_t ofs0, y;
        যদি( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elem_আকার;
        }
        y = ofs/m->cols;
        পূর্ণসংখ্যা y1 = std::min(std::max((পূর্ণসংখ্যা)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elem_আকার;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elem_আকার;
        ফেরত;
    }

    যদি( relative )
        ofs += lpos();

    যদি( ofs < 0 )
        ofs = 0;

    পূর্ণসংখ্যা szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_আকার;
    sliceStart = m->ptr();

    জন্য( পূর্ণসংখ্যা i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (পূর্ণসংখ্যা)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elem_আকার;
    যদি( ofs > 0 )
        ptr = sliceEnd;
    অন্যথা
        ptr = sliceStart + (ptr - m->ptr());
}

শূন্য MatConstIterator::seek(ধ্রুবক পূর্ণসংখ্যা* _idx, যৌক্তিক relative)
{
    পূর্ণসংখ্যা d = m->dims;
    ptrdiff_t ofs = 0;
    যদি( !_idx )
        ;
    অন্যথা যদি( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ধ্রুবক SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    যদি(!_m || !_m->hdr)
        ফেরত;
    SparseMat::Hdr& hdr = *m->hdr;
    ধ্রুবক std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    জন্য( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        যদি( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.মান_Offset;
            ফেরত;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    যদি( !ptr || !m || !m->hdr )
        ফেরত *এই;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ধ্রুবক SparseMat::Node*)(ptr - hdr.মান_Offset))->next;
    যদি( next )
    {
        ptr = &hdr.pool[next] + hdr.মান_Offset;
        ফেরত *এই;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    জন্য( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        যদি( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.মান_Offset;
            ফেরত *এই;
        }
    }
    hashidx = sz;
    ptr = 0;
    ফেরত *এই;
}

} // cv::
