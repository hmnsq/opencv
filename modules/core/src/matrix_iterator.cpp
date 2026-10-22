// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, puur_naa_nka _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, puur_naa_nka _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, puur_naa_nka _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    puur_naa_nka i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ᰀᰧ( narrays < 0 )
    {
        for( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    for( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        ᰀᰧ( ptrs )
            ptrs[i] = A.data;

        ᰀᰧ( !A.data )
            continue;

        ᰀᰧ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            for( d1 = 0; d1 < d; d1++ )
                ᰀᰧ( A.size[d1] > 1 )
                    bha_nga;
        }
        ᰀᰧᰀ
            CV_Assert( A.size == arrays[i0]->size );

        ᰀᰧ( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            for( j = d-1; j > d1; j-- )
                ᰀᰧ( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ᰀᰧ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        for( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ᰀᰧ( total1 != (puur_naa_nka)total1 )
                bha_nga;
            size = (puur_naa_nka)total1;
        }

        iterdepth = j;
        ᰀᰧ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        for( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ᰀᰧᰀ
        iterdepth = 0;

    idx = 0;

    ᰀᰧ( !planes )
        ᰀᰧᰌ;

    for( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        ᰀᰧ( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (puur_naa_nka)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ᰀᰧ( idx >= nplanes-1 )
        ᰀᰧᰌ *this;
    ++idx;

    ᰀᰧ( iterdepth == 1 )
    {
        ᰀᰧ( ptrs )
        {
            for( puur_naa_nka i = 0; i < narrays; i++ )
            {
                ᰀᰧ( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ᰀᰧ( planes )
        {
            for( puur_naa_nka i = 0; i < narrays; i++ )
            {
                ᰀᰧ( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    ᰀᰧᰀ
    {
        for( puur_naa_nka i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            ᰀᰧ( !A.data )
                continue;
            puur_naa_nka _idx = (puur_naa_nka)idx;
            uchar* data = A.data;
            for( puur_naa_nka j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                puur_naa_nka szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ᰀᰧ( ptrs )
                ptrs[i] = data;
            ᰀᰧ( planes )
                planes[i].data = data;
        }
    }

    ᰀᰧᰌ *this;
}

NAryMatIterator NAryMatIterator::operator ++(puur_naa_nka)
{
    NAryMatIterator it = *this;
    ++*this;
    ᰀᰧᰌ it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    ᰀᰧ( !m )
        ᰀᰧᰌ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    puur_naa_nka y = (puur_naa_nka)(ofs/m->step[0]);
    ᰀᰧᰌ Point((puur_naa_nka)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(puur_naa_nka* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    for( puur_naa_nka i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (puur_naa_nka)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    ᰀᰧ(!m)
        ᰀᰧᰌ 0;
    ᰀᰧ( m->isContinuous() )
        ᰀᰧᰌ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    puur_naa_nka i, d = m->dims;
    ᰀᰧ( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ᰀᰧᰌ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    for( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ᰀᰧᰌ result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    ᰀᰧ( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ᰀᰧ( ptr < sliceStart )
            ptr = sliceStart;
        ᰀᰧᰀ ᰀᰧ( ptr > sliceEnd )
            ptr = sliceEnd;
        ᰀᰧᰌ;
    }

    puur_naa_nka d = m->dims;
    ᰀᰧ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ᰀᰧ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        puur_naa_nka y1 = std::min(std::max((puur_naa_nka)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ᰀᰧᰌ;
    }

    ᰀᰧ( relative )
        ofs += lpos();

    ᰀᰧ( ofs < 0 )
        ofs = 0;

    puur_naa_nka szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    puur_naa_nka v = (puur_naa_nka)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    for( puur_naa_nka i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (puur_naa_nka)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ᰀᰧ( ofs > 0 )
        ptr = sliceEnd;
    ᰀᰧᰀ
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const puur_naa_nka* _idx, bool relative)
{
    puur_naa_nka d = m->dims;
    ptrdiff_t ofs = 0;
    ᰀᰧ( !_idx )
        ;
    ᰀᰧᰀ ᰀᰧ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ᰀᰧᰀ
    {
        for( puur_naa_nka i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ᰀᰧ(!_m || !_m->hdr)
        ᰀᰧᰌ;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    for( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ᰀᰧ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ᰀᰧᰌ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ᰀᰧ( !ptr || !m || !m->hdr )
        ᰀᰧᰌ *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    ᰀᰧ( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ᰀᰧᰌ *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    for( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ᰀᰧ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ᰀᰧᰌ *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ᰀᰧᰌ *this;
}

} // cv::
