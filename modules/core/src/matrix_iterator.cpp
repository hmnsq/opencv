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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, số_nguyên _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, số_nguyên _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, số_nguyên _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    số_nguyên i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    nếu( narrays < 0 )
    {
        cho( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    cho( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        nếu( ptrs )
            ptrs[i] = A.data;

        nếu( !A.data )
            continue;

        nếu( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            cho( d1 = 0; d1 < d; d1++ )
                nếu( A.size[d1] > 1 )
                    break;
        }
        không_thì
            CV_Assert( A.size == arrays[i0]->size );

        nếu( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            cho( j = d-1; j > d1; j-- )
                nếu( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    nếu( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        cho( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            nếu( total1 != (số_nguyên)total1 )
                break;
            size = (số_nguyên)total1;
        }

        iterdepth = j;
        nếu( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        cho( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    không_thì
        iterdepth = 0;

    idx = 0;

    nếu( !planes )
        trả_về;

    cho( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        nếu( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (số_nguyên)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    nếu( idx >= nplanes-1 )
        trả_về *this;
    ++idx;

    nếu( iterdepth == 1 )
    {
        nếu( ptrs )
        {
            cho( số_nguyên i = 0; i < narrays; i++ )
            {
                nếu( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        nếu( planes )
        {
            cho( số_nguyên i = 0; i < narrays; i++ )
            {
                nếu( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    không_thì
    {
        cho( số_nguyên i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            nếu( !A.data )
                continue;
            số_nguyên _idx = (số_nguyên)idx;
            uchar* data = A.data;
            cho( số_nguyên j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                số_nguyên szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            nếu( ptrs )
                ptrs[i] = data;
            nếu( planes )
                planes[i].data = data;
        }
    }

    trả_về *this;
}

NAryMatIterator NAryMatIterator::operator ++(số_nguyên)
{
    NAryMatIterator it = *this;
    ++*this;
    trả_về it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    nếu( !m )
        trả_về Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    số_nguyên y = (số_nguyên)(ofs/m->step[0]);
    trả_về Point((số_nguyên)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(số_nguyên* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    cho( số_nguyên i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (số_nguyên)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    nếu(!m)
        trả_về 0;
    nếu( m->isContinuous() )
        trả_về (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    số_nguyên i, d = m->dims;
    nếu( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        trả_về y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    cho( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    trả_về result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    nếu( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        nếu( ptr < sliceStart )
            ptr = sliceStart;
        không_thì nếu( ptr > sliceEnd )
            ptr = sliceEnd;
        trả_về;
    }

    số_nguyên d = m->dims;
    nếu( d == 2 )
    {
        ptrdiff_t ofs0, y;
        nếu( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        số_nguyên y1 = std::min(std::max((số_nguyên)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        trả_về;
    }

    nếu( relative )
        ofs += lpos();

    nếu( ofs < 0 )
        ofs = 0;

    số_nguyên szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    số_nguyên v = (số_nguyên)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    cho( số_nguyên i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (số_nguyên)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    nếu( ofs > 0 )
        ptr = sliceEnd;
    không_thì
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const số_nguyên* _idx, bool relative)
{
    số_nguyên d = m->dims;
    ptrdiff_t ofs = 0;
    nếu( !_idx )
        ;
    không_thì nếu( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    không_thì
    {
        cho( số_nguyên i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    nếu(!_m || !_m->hdr)
        trả_về;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    cho( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        nếu( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            trả_về;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    nếu( !ptr || !m || !m->hdr )
        trả_về *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    nếu( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        trả_về *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    cho( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        nếu( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            trả_về *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    trả_về *this;
}

} // cv::
