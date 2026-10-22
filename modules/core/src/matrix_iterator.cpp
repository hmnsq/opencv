// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/mat.hpp"

નામાકાશ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(અચલ Mat** _arrays, Mat* _planes, પૂર્ણાંક _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(અચલ Mat** _arrays, uchar** _ptrs, પૂર્ણાંક _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

શૂન્ય NAryMatIterator::init(અચલ Mat** _arrays, Mat* _planes, uchar** _ptrs, પૂર્ણાંક _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    પૂર્ણાંક i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    જો( narrays < 0 )
    {
        માટે( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    માટે( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        અચલ Mat& A = *arrays[i];
        જો( ptrs )
            ptrs[i] = A.data;

        જો( !A.data )
            ચાલુ;

        જો( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            માટે( d1 = 0; d1 < d; d1++ )
                જો( A.size[d1] > 1 )
                    તોડો;
        }
        નહીંતર
            CV_Assert( A.size == arrays[i0]->size );

        જો( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            માટે( j = d-1; j > d1; j-- )
                જો( A.step[j]*A.size[j] < A.step[j-1] )
                    તોડો;
            iterdepth = std::max(iterdepth, j);
        }
    }

    જો( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        માટે( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            જો( total1 != (પૂર્ણાંક)total1 )
                તોડો;
            size = (પૂર્ણાંક)total1;
        }

        iterdepth = j;
        જો( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        માટે( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    નહીંતર
        iterdepth = 0;

    idx = 0;

    જો( !planes )
        પાછા;

    માટે( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        અચલ Mat& A = *arrays[i];

        જો( !A.data )
        {
            planes[i] = Mat();
            ચાલુ;
        }

        planes[i] = Mat(1, (પૂર્ણાંક)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    જો( idx >= nplanes-1 )
        પાછા *આ;
    ++idx;

    જો( iterdepth == 1 )
    {
        જો( ptrs )
        {
            માટે( પૂર્ણાંક i = 0; i < narrays; i++ )
            {
                જો( !ptrs[i] )
                    ચાલુ;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        જો( planes )
        {
            માટે( પૂર્ણાંક i = 0; i < narrays; i++ )
            {
                જો( !planes[i].data )
                    ચાલુ;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < narrays; i++ )
        {
            અચલ Mat& A = *arrays[i];
            જો( !A.data )
                ચાલુ;
            પૂર્ણાંક _idx = (પૂર્ણાંક)idx;
            uchar* data = A.data;
            માટે( પૂર્ણાંક j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                પૂર્ણાંક szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            જો( ptrs )
                ptrs[i] = data;
            જો( planes )
                planes[i].data = data;
        }
    }

    પાછા *આ;
}

NAryMatIterator NAryMatIterator::operator ++(પૂર્ણાંક)
{
    NAryMatIterator it = *આ;
    ++*આ;
    પાછા it;
}

//==================================================================================================

Point MatConstIterator::pos() અચલ
{
    જો( !m )
        પાછા Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    પૂર્ણાંક y = (પૂર્ણાંક)(ofs/m->step[0]);
    પાછા Point((પૂર્ણાંક)((ofs - y*m->step[0])/elemSize), y);
}

શૂન્ય MatConstIterator::pos(પૂર્ણાંક* _idx) અચલ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    માટે( પૂર્ણાંક i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (પૂર્ણાંક)v;
    }
}

ptrdiff_t MatConstIterator::lpos() અચલ
{
    જો(!m)
        પાછા 0;
    જો( m->isContinuous() )
        પાછા (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    પૂર્ણાંક i, d = m->dims;
    જો( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        પાછા y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    માટે( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    પાછા result;
}

શૂન્ય MatConstIterator::seek(ptrdiff_t ofs, તાર્કિક relative)
{
    જો( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        જો( ptr < sliceStart )
            ptr = sliceStart;
        નહીંતર જો( ptr > sliceEnd )
            ptr = sliceEnd;
        પાછા;
    }

    પૂર્ણાંક d = m->dims;
    જો( d == 2 )
    {
        ptrdiff_t ofs0, y;
        જો( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        પૂર્ણાંક y1 = std::min(std::max((પૂર્ણાંક)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        પાછા;
    }

    જો( relative )
        ofs += lpos();

    જો( ofs < 0 )
        ofs = 0;

    પૂર્ણાંક szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    પૂર્ણાંક v = (પૂર્ણાંક)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    માટે( પૂર્ણાંક i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (પૂર્ણાંક)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    જો( ofs > 0 )
        ptr = sliceEnd;
    નહીંતર
        ptr = sliceStart + (ptr - m->ptr());
}

શૂન્ય MatConstIterator::seek(અચલ પૂર્ણાંક* _idx, તાર્કિક relative)
{
    પૂર્ણાંક d = m->dims;
    ptrdiff_t ofs = 0;
    જો( !_idx )
        ;
    નહીંતર જો( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(અચલ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    જો(!_m || !_m->hdr)
        પાછા;
    SparseMat::Hdr& hdr = *m->hdr;
    અચલ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    માટે( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        જો( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.કિંમત_Offset;
            પાછા;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    જો( !ptr || !m || !m->hdr )
        પાછા *આ;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((અચલ SparseMat::Node*)(ptr - hdr.કિંમત_Offset))->next;
    જો( next )
    {
        ptr = &hdr.pool[next] + hdr.કિંમત_Offset;
        પાછા *આ;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    માટે( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        જો( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.કિંમત_Offset;
            પાછા *આ;
        }
    }
    hashidx = sz;
    ptr = 0;
    પાછા *આ;
}

} // cv::
