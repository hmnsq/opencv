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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, පූර්ණ_සංඛ්‍යා _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, පූර්ණ_සංඛ්‍යා _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, පූර්ණ_සංඛ්‍යා _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    පූර්ණ_සංඛ්‍යා i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    නම්( narrays < 0 )
    {
        සඳහා( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    සඳහා( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        නම්( ptrs )
            ptrs[i] = A.data;

        නම්( !A.data )
            continue;

        නම්( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            සඳහා( d1 = 0; d1 < d; d1++ )
                නම්( A.size[d1] > 1 )
                    bha_nga;
        }
        නැතහොත්
            CV_Assert( A.size == arrays[i0]->size );

        නම්( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            සඳහා( j = d-1; j > d1; j-- )
                නම්( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    නම්( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        සඳහා( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            නම්( total1 != (පූර්ණ_සංඛ්‍යා)total1 )
                bha_nga;
            size = (පූර්ණ_සංඛ්‍යා)total1;
        }

        iterdepth = j;
        නම්( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        සඳහා( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    නැතහොත්
        iterdepth = 0;

    idx = 0;

    නම්( !planes )
        ආපසු;

    සඳහා( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        නම්( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (පූර්ණ_සංඛ්‍යා)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    නම්( idx >= nplanes-1 )
        ආපසු *this;
    ++idx;

    නම්( iterdepth == 1 )
    {
        නම්( ptrs )
        {
            සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < narrays; i++ )
            {
                නම්( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        නම්( planes )
        {
            සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < narrays; i++ )
            {
                නම්( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    නැතහොත්
    {
        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            නම්( !A.data )
                continue;
            පූර්ණ_සංඛ්‍යා _idx = (පූර්ණ_සංඛ්‍යා)idx;
            uchar* data = A.data;
            සඳහා( පූර්ණ_සංඛ්‍යා j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                පූර්ණ_සංඛ්‍යා szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            නම්( ptrs )
                ptrs[i] = data;
            නම්( planes )
                planes[i].data = data;
        }
    }

    ආපසු *this;
}

NAryMatIterator NAryMatIterator::operator ++(පූර්ණ_සංඛ්‍යා)
{
    NAryMatIterator it = *this;
    ++*this;
    ආපසු it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    නම්( !m )
        ආපසු Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    පූර්ණ_සංඛ්‍යා y = (පූර්ණ_සංඛ්‍යා)(ofs/m->step[0]);
    ආපසු Point((පූර්ණ_සංඛ්‍යා)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(පූර්ණ_සංඛ්‍යා* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (පූර්ණ_සංඛ්‍යා)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    නම්(!m)
        ආපසු 0;
    නම්( m->isContinuous() )
        ආපසු (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    පූර්ණ_සංඛ්‍යා i, d = m->dims;
    නම්( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ආපසු y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    සඳහා( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ආපසු result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    නම්( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        නම්( ptr < sliceStart )
            ptr = sliceStart;
        නැතහොත් නම්( ptr > sliceEnd )
            ptr = sliceEnd;
        ආපසු;
    }

    පූර්ණ_සංඛ්‍යා d = m->dims;
    නම්( d == 2 )
    {
        ptrdiff_t ofs0, y;
        නම්( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        පූර්ණ_සංඛ්‍යා y1 = std::min(std::max((පූර්ණ_සංඛ්‍යා)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ආපසු;
    }

    නම්( relative )
        ofs += lpos();

    නම්( ofs < 0 )
        ofs = 0;

    පූර්ණ_සංඛ්‍යා szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    පූර්ණ_සංඛ්‍යා v = (පූර්ණ_සංඛ්‍යා)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    සඳහා( පූර්ණ_සංඛ්‍යා i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (පූර්ණ_සංඛ්‍යා)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    නම්( ofs > 0 )
        ptr = sliceEnd;
    නැතහොත්
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const පූර්ණ_සංඛ්‍යා* _idx, bool relative)
{
    පූර්ණ_සංඛ්‍යා d = m->dims;
    ptrdiff_t ofs = 0;
    නම්( !_idx )
        ;
    නැතහොත් නම්( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    නැතහොත්
    {
        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    නම්(!_m || !_m->hdr)
        ආපසු;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    සඳහා( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        නම්( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ආපසු;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    නම්( !ptr || !m || !m->hdr )
        ආපසු *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    නම්( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ආපසු *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    සඳහා( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        නම්( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ආපසු *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ආපසු *this;
}

} // cv::
