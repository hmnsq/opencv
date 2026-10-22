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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, 整数 _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, 整数 _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, 整数 _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    整数 i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    もし( narrays < 0 )
    {
        ために( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ために( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        もし( ptrs )
            ptrs[i] = A.data;

        もし( !A.data )
            continue;

        もし( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ために( d1 = 0; d1 < d; d1++ )
                もし( A.size[d1] > 1 )
                    break;
        }
        否则
            CV_Assert( A.size == arrays[i0]->size );

        もし( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            ために( j = d-1; j > d1; j-- )
                もし( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    もし( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ために( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            もし( total1 != (整数)total1 )
                break;
            size = (整数)total1;
        }

        iterdepth = j;
        もし( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ために( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    否则
        iterdepth = 0;

    idx = 0;

    もし( !planes )
        戻る;

    ために( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        もし( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (整数)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    もし( idx >= nplanes-1 )
        戻る *this;
    ++idx;

    もし( iterdepth == 1 )
    {
        もし( ptrs )
        {
            ために( 整数 i = 0; i < narrays; i++ )
            {
                もし( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        もし( planes )
        {
            ために( 整数 i = 0; i < narrays; i++ )
            {
                もし( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    否则
    {
        ために( 整数 i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            もし( !A.data )
                continue;
            整数 _idx = (整数)idx;
            uchar* data = A.data;
            ために( 整数 j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                整数 szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            もし( ptrs )
                ptrs[i] = data;
            もし( planes )
                planes[i].data = data;
        }
    }

    戻る *this;
}

NAryMatIterator NAryMatIterator::operator ++(整数)
{
    NAryMatIterator it = *this;
    ++*this;
    戻る it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    もし( !m )
        戻る Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    整数 y = (整数)(ofs/m->step[0]);
    戻る Point((整数)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(整数* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ために( 整数 i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (整数)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    もし(!m)
        戻る 0;
    もし( m->isContinuous() )
        戻る (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    整数 i, d = m->dims;
    もし( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        戻る y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    ために( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    戻る result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    もし( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        もし( ptr < sliceStart )
            ptr = sliceStart;
        否则 もし( ptr > sliceEnd )
            ptr = sliceEnd;
        戻る;
    }

    整数 d = m->dims;
    もし( d == 2 )
    {
        ptrdiff_t ofs0, y;
        もし( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        整数 y1 = std::min(std::max((整数)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        戻る;
    }

    もし( relative )
        ofs += lpos();

    もし( ofs < 0 )
        ofs = 0;

    整数 szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    整数 v = (整数)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    ために( 整数 i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (整数)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    もし( ofs > 0 )
        ptr = sliceEnd;
    否则
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const 整数* _idx, bool relative)
{
    整数 d = m->dims;
    ptrdiff_t ofs = 0;
    もし( !_idx )
        ;
    否则 もし( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    否则
    {
        ために( 整数 i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    もし(!_m || !_m->hdr)
        戻る;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ために( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        もし( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            戻る;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    もし( !ptr || !m || !m->hdr )
        戻る *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    もし( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        戻る *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ために( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        もし( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            戻る *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    戻る *this;
}

} // cv::
