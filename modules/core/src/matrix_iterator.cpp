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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, ціле _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, ціле _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, ціле _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ціле i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    якщо( narrays < 0 )
    {
        для( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    для( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        якщо( ptrs )
            ptrs[i] = A.data;

        якщо( !A.data )
            continue;

        якщо( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            для( d1 = 0; d1 < d; d1++ )
                якщо( A.size[d1] > 1 )
                    break;
        }
        інакше
            CV_Assert( A.size == arrays[i0]->size );

        якщо( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            для( j = d-1; j > d1; j-- )
                якщо( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    якщо( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        для( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            якщо( total1 != (ціле)total1 )
                break;
            size = (ціле)total1;
        }

        iterdepth = j;
        якщо( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        для( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    інакше
        iterdepth = 0;

    idx = 0;

    якщо( !planes )
        повернути;

    для( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        якщо( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (ціле)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    якщо( idx >= nplanes-1 )
        повернути *this;
    ++idx;

    якщо( iterdepth == 1 )
    {
        якщо( ptrs )
        {
            для( ціле i = 0; i < narrays; i++ )
            {
                якщо( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        якщо( planes )
        {
            для( ціле i = 0; i < narrays; i++ )
            {
                якщо( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    інакше
    {
        для( ціле i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            якщо( !A.data )
                continue;
            ціле _idx = (ціле)idx;
            uchar* data = A.data;
            для( ціле j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ціле szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            якщо( ptrs )
                ptrs[i] = data;
            якщо( planes )
                planes[i].data = data;
        }
    }

    повернути *this;
}

NAryMatIterator NAryMatIterator::operator ++(ціле)
{
    NAryMatIterator it = *this;
    ++*this;
    повернути it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    якщо( !m )
        повернути Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ціле y = (ціле)(ofs/m->step[0]);
    повернути Point((ціле)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(ціле* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    для( ціле i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ціле)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    якщо(!m)
        повернути 0;
    якщо( m->isContinuous() )
        повернути (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ціле i, d = m->dims;
    якщо( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        повернути y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    для( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    повернути result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    якщо( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        якщо( ptr < sliceStart )
            ptr = sliceStart;
        інакше якщо( ptr > sliceEnd )
            ptr = sliceEnd;
        повернути;
    }

    ціле d = m->dims;
    якщо( d == 2 )
    {
        ptrdiff_t ofs0, y;
        якщо( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ціле y1 = std::min(std::max((ціле)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        повернути;
    }

    якщо( relative )
        ofs += lpos();

    якщо( ofs < 0 )
        ofs = 0;

    ціле szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ціле v = (ціле)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    для( ціле i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ціле)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    якщо( ofs > 0 )
        ptr = sliceEnd;
    інакше
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const ціле* _idx, bool relative)
{
    ціле d = m->dims;
    ptrdiff_t ofs = 0;
    якщо( !_idx )
        ;
    інакше якщо( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    інакше
    {
        для( ціле i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    якщо(!_m || !_m->hdr)
        повернути;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    для( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        якщо( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            повернути;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    якщо( !ptr || !m || !m->hdr )
        повернути *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    якщо( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        повернути *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    для( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        якщо( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            повернути *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    повернути *this;
}

} // cv::
