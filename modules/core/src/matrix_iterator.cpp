// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencv2/core/mat.hpp"

নামাকাশ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ধ্রুৱ Mat** _arrays, Mat* _planes, পূর্ণাঙ্ক _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ধ্রুৱ Mat** _arrays, uchar** _ptrs, পূর্ণাঙ্ক _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

শূন্য NAryMatIterator::init(ধ্রুৱ Mat** _arrays, Mat* _planes, uchar** _ptrs, পূর্ণাঙ্ক _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    পূর্ণাঙ্ক i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    যদি( narrays < 0 )
    {
        বাবে( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    বাবে( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ধ্রুৱ Mat& A = *arrays[i];
        যদি( ptrs )
            ptrs[i] = A.data;

        যদি( !A.data )
            অনুৱর্ত;

        যদি( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            বাবে( d1 = 0; d1 < d; d1++ )
                যদি( A.size[d1] > 1 )
                    ভঙ্গ;
        }
        অন্যথা
            CV_Assert( A.size == arrays[i0]->size );

        যদি( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            বাবে( j = d-1; j > d1; j-- )
                যদি( A.step[j]*A.size[j] < A.step[j-1] )
                    ভঙ্গ;
            iterdepth = std::max(iterdepth, j);
        }
    }

    যদি( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        বাবে( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            যদি( total1 != (পূর্ণাঙ্ক)total1 )
                ভঙ্গ;
            size = (পূর্ণাঙ্ক)total1;
        }

        iterdepth = j;
        যদি( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        বাবে( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    অন্যথা
        iterdepth = 0;

    idx = 0;

    যদি( !planes )
        ঘূৰাই;

    বাবে( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ধ্রুৱ Mat& A = *arrays[i];

        যদি( !A.data )
        {
            planes[i] = Mat();
            অনুৱর্ত;
        }

        planes[i] = Mat(1, (পূর্ণাঙ্ক)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    যদি( idx >= nplanes-1 )
        ঘূৰাই *এতত্;
    ++idx;

    যদি( iterdepth == 1 )
    {
        যদি( ptrs )
        {
            বাবে( পূর্ণাঙ্ক i = 0; i < narrays; i++ )
            {
                যদি( !ptrs[i] )
                    অনুৱর্ত;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        যদি( planes )
        {
            বাবে( পূর্ণাঙ্ক i = 0; i < narrays; i++ )
            {
                যদি( !planes[i].data )
                    অনুৱর্ত;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    অন্যথা
    {
        বাবে( পূর্ণাঙ্ক i = 0; i < narrays; i++ )
        {
            ধ্রুৱ Mat& A = *arrays[i];
            যদি( !A.data )
                অনুৱর্ত;
            পূর্ণাঙ্ক _idx = (পূর্ণাঙ্ক)idx;
            uchar* data = A.data;
            বাবে( পূর্ণাঙ্ক j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                পূর্ণাঙ্ক szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            যদি( ptrs )
                ptrs[i] = data;
            যদি( planes )
                planes[i].data = data;
        }
    }

    ঘূৰাই *এতত্;
}

NAryMatIterator NAryMatIterator::operator ++(পূর্ণাঙ্ক)
{
    NAryMatIterator it = *এতত্;
    ++*এতত্;
    ঘূৰাই it;
}

//==================================================================================================

Point MatConstIterator::pos() ধ্রুৱ
{
    যদি( !m )
        ঘূৰাই Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    পূর্ণাঙ্ক y = (পূর্ণাঙ্ক)(ofs/m->step[0]);
    ঘূৰাই Point((পূর্ণাঙ্ক)((ofs - y*m->step[0])/elemSize), y);
}

শূন্য MatConstIterator::pos(পূর্ণাঙ্ক* _idx) ধ্রুৱ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    বাবে( পূর্ণাঙ্ক i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (পূর্ণাঙ্ক)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ধ্রুৱ
{
    যদি(!m)
        ঘূৰাই 0;
    যদি( m->isContinuous() )
        ঘূৰাই (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    পূর্ণাঙ্ক i, d = m->dims;
    যদি( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ঘূৰাই y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    বাবে( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ঘূৰাই result;
}

শূন্য MatConstIterator::seek(ptrdiff_t ofs, তার্কিক relative)
{
    যদি( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        যদি( ptr < sliceStart )
            ptr = sliceStart;
        অন্যথা যদি( ptr > sliceEnd )
            ptr = sliceEnd;
        ঘূৰাই;
    }

    পূর্ণাঙ্ক d = m->dims;
    যদি( d == 2 )
    {
        ptrdiff_t ofs0, y;
        যদি( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        পূর্ণাঙ্ক y1 = std::min(std::max((পূর্ণাঙ্ক)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ঘূৰাই;
    }

    যদি( relative )
        ofs += lpos();

    যদি( ofs < 0 )
        ofs = 0;

    পূর্ণাঙ্ক szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    পূর্ণাঙ্ক v = (পূর্ণাঙ্ক)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    বাবে( পূর্ণাঙ্ক i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (পূর্ণাঙ্ক)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    যদি( ofs > 0 )
        ptr = sliceEnd;
    অন্যথা
        ptr = sliceStart + (ptr - m->ptr());
}

শূন্য MatConstIterator::seek(ধ্রুৱ পূর্ণাঙ্ক* _idx, তার্কিক relative)
{
    পূর্ণাঙ্ক d = m->dims;
    ptrdiff_t ofs = 0;
    যদি( !_idx )
        ;
    অন্যথা যদি( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    অন্যথা
    {
        বাবে( পূর্ণাঙ্ক i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ধ্রুৱ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    যদি(!_m || !_m->hdr)
        ঘূৰাই;
    SparseMat::Hdr& hdr = *m->hdr;
    ধ্রুৱ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    বাবে( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        যদি( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.মান_Offset;
            ঘূৰাই;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    যদি( !ptr || !m || !m->hdr )
        ঘূৰাই *এতত্;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ধ্রুৱ SparseMat::Node*)(ptr - hdr.মান_Offset))->next;
    যদি( next )
    {
        ptr = &hdr.pool[next] + hdr.মান_Offset;
        ঘূৰাই *এতত্;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    বাবে( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        যদি( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.মান_Offset;
            ঘূৰাই *এতত্;
        }
    }
    hashidx = sz;
    ptr = 0;
    ঘূৰাই *এতত্;
}

} // cv::
