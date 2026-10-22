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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, 정수 _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, 정수 _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, 정수 _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    정수 i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    만약( narrays < 0 )
    {
        위해( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    위해( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        만약( ptrs )
            ptrs[i] = A.data;

        만약( !A.data )
            continue;

        만약( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            위해( d1 = 0; d1 < d; d1++ )
                만약( A.size[d1] > 1 )
                    break;
        }
        아니면
            CV_Assert( A.size == arrays[i0]->size );

        만약( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            위해( j = d-1; j > d1; j-- )
                만약( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    만약( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        위해( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            만약( total1 != (정수)total1 )
                break;
            size = (정수)total1;
        }

        iterdepth = j;
        만약( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        위해( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    아니면
        iterdepth = 0;

    idx = 0;

    만약( !planes )
        반환;

    위해( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        만약( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (정수)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    만약( idx >= nplanes-1 )
        반환 *this;
    ++idx;

    만약( iterdepth == 1 )
    {
        만약( ptrs )
        {
            위해( 정수 i = 0; i < narrays; i++ )
            {
                만약( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        만약( planes )
        {
            위해( 정수 i = 0; i < narrays; i++ )
            {
                만약( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    아니면
    {
        위해( 정수 i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            만약( !A.data )
                continue;
            정수 _idx = (정수)idx;
            uchar* data = A.data;
            위해( 정수 j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                정수 szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            만약( ptrs )
                ptrs[i] = data;
            만약( planes )
                planes[i].data = data;
        }
    }

    반환 *this;
}

NAryMatIterator NAryMatIterator::operator ++(정수)
{
    NAryMatIterator it = *this;
    ++*this;
    반환 it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    만약( !m )
        반환 Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    정수 y = (정수)(ofs/m->step[0]);
    반환 Point((정수)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(정수* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    위해( 정수 i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (정수)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    만약(!m)
        반환 0;
    만약( m->isContinuous() )
        반환 (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    정수 i, d = m->dims;
    만약( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        반환 y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    위해( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    반환 result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    만약( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        만약( ptr < sliceStart )
            ptr = sliceStart;
        아니면 만약( ptr > sliceEnd )
            ptr = sliceEnd;
        반환;
    }

    정수 d = m->dims;
    만약( d == 2 )
    {
        ptrdiff_t ofs0, y;
        만약( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        정수 y1 = std::min(std::max((정수)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        반환;
    }

    만약( relative )
        ofs += lpos();

    만약( ofs < 0 )
        ofs = 0;

    정수 szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    정수 v = (정수)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    위해( 정수 i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (정수)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    만약( ofs > 0 )
        ptr = sliceEnd;
    아니면
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const 정수* _idx, bool relative)
{
    정수 d = m->dims;
    ptrdiff_t ofs = 0;
    만약( !_idx )
        ;
    아니면 만약( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    아니면
    {
        위해( 정수 i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    만약(!_m || !_m->hdr)
        반환;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    위해( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        만약( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            반환;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    만약( !ptr || !m || !m->hdr )
        반환 *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    만약( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        반환 *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    위해( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        만약( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            반환 *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    반환 *this;
}

} // cv::
