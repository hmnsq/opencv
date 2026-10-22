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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, целое _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, целое _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, целое _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    целое i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    если( narrays < 0 )
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
        если( ptrs )
            ptrs[i] = A.data;

        если( !A.data )
            continue;

        если( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            для( d1 = 0; d1 < d; d1++ )
                если( A.size[d1] > 1 )
                    break;
        }
        иначе
            CV_Assert( A.size == arrays[i0]->size );

        если( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            для( j = d-1; j > d1; j-- )
                если( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    если( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        для( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            если( total1 != (целое)total1 )
                break;
            size = (целое)total1;
        }

        iterdepth = j;
        если( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        для( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    иначе
        iterdepth = 0;

    idx = 0;

    если( !planes )
        вернуть;

    для( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        если( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (целое)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    если( idx >= nplanes-1 )
        вернуть *this;
    ++idx;

    если( iterdepth == 1 )
    {
        если( ptrs )
        {
            для( целое i = 0; i < narrays; i++ )
            {
                если( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        если( planes )
        {
            для( целое i = 0; i < narrays; i++ )
            {
                если( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    иначе
    {
        для( целое i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            если( !A.data )
                continue;
            целое _idx = (целое)idx;
            uchar* data = A.data;
            для( целое j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                целое szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            если( ptrs )
                ptrs[i] = data;
            если( planes )
                planes[i].data = data;
        }
    }

    вернуть *this;
}

NAryMatIterator NAryMatIterator::operator ++(целое)
{
    NAryMatIterator it = *this;
    ++*this;
    вернуть it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    если( !m )
        вернуть Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    целое y = (целое)(ofs/m->step[0]);
    вернуть Point((целое)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(целое* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    для( целое i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (целое)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    если(!m)
        вернуть 0;
    если( m->isContinuous() )
        вернуть (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    целое i, d = m->dims;
    если( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        вернуть y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    для( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    вернуть result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    если( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        если( ptr < sliceStart )
            ptr = sliceStart;
        иначе если( ptr > sliceEnd )
            ptr = sliceEnd;
        вернуть;
    }

    целое d = m->dims;
    если( d == 2 )
    {
        ptrdiff_t ofs0, y;
        если( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        целое y1 = std::min(std::max((целое)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        вернуть;
    }

    если( relative )
        ofs += lpos();

    если( ofs < 0 )
        ofs = 0;

    целое szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    целое v = (целое)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    для( целое i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (целое)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    если( ofs > 0 )
        ptr = sliceEnd;
    иначе
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const целое* _idx, bool relative)
{
    целое d = m->dims;
    ptrdiff_t ofs = 0;
    если( !_idx )
        ;
    иначе если( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    иначе
    {
        для( целое i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    если(!_m || !_m->hdr)
        вернуть;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    для( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        если( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            вернуть;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    если( !ptr || !m || !m->hdr )
        вернуть *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    если( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        вернуть *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    для( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        если( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            вернуть *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    вернуть *this;
}

} // cv::
