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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, цео _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, цео _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, цео _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    цео i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ако( narrays < 0 )
    {
        за( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    за( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        ако( ptrs )
            ptrs[i] = A.data;

        ако( !A.data )
            continue;

        ако( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            за( d1 = 0; d1 < d; d1++ )
                ако( A.size[d1] > 1 )
                    break;
        }
        инако
            CV_Assert( A.size == arrays[i0]->size );

        ако( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            за( j = d-1; j > d1; j-- )
                ако( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ако( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        за( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ако( total1 != (цео)total1 )
                break;
            size = (цео)total1;
        }

        iterdepth = j;
        ако( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        за( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    инако
        iterdepth = 0;

    idx = 0;

    ако( !planes )
        врати;

    за( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        ако( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (цео)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ако( idx >= nplanes-1 )
        врати *this;
    ++idx;

    ако( iterdepth == 1 )
    {
        ако( ptrs )
        {
            за( цео i = 0; i < narrays; i++ )
            {
                ако( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ако( planes )
        {
            за( цео i = 0; i < narrays; i++ )
            {
                ако( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    инако
    {
        за( цео i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            ако( !A.data )
                continue;
            цео _idx = (цео)idx;
            uchar* data = A.data;
            за( цео j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                цео szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ако( ptrs )
                ptrs[i] = data;
            ако( planes )
                planes[i].data = data;
        }
    }

    врати *this;
}

NAryMatIterator NAryMatIterator::operator ++(цео)
{
    NAryMatIterator it = *this;
    ++*this;
    врати it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    ако( !m )
        врати Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    цео y = (цео)(ofs/m->step[0]);
    врати Point((цео)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(цео* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    за( цео i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (цео)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    ако(!m)
        врати 0;
    ако( m->isContinuous() )
        врати (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    цео i, d = m->dims;
    ако( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        врати y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    за( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    врати result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    ако( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ако( ptr < sliceStart )
            ptr = sliceStart;
        инако ако( ptr > sliceEnd )
            ptr = sliceEnd;
        врати;
    }

    цео d = m->dims;
    ако( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ако( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        цео y1 = std::min(std::max((цео)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        врати;
    }

    ако( relative )
        ofs += lpos();

    ако( ofs < 0 )
        ofs = 0;

    цео szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    цео v = (цео)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    за( цео i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (цео)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ако( ofs > 0 )
        ptr = sliceEnd;
    инако
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const цео* _idx, bool relative)
{
    цео d = m->dims;
    ptrdiff_t ofs = 0;
    ако( !_idx )
        ;
    инако ако( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    инако
    {
        за( цео i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ако(!_m || !_m->hdr)
        врати;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    за( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ако( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            врати;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ако( !ptr || !m || !m->hdr )
        врати *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    ако( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        врати *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    за( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ако( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            врати *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    врати *this;
}

} // cv::
