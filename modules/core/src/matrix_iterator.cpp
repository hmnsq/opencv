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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, ຈຳນວນເຕັມ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, ຈຳນວນເຕັມ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, ຈຳນວນເຕັມ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ຈຳນວນເຕັມ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ຖ້າ( narrays < 0 )
    {
        ສຳລັບ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ສຳລັບ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        ຖ້າ( ptrs )
            ptrs[i] = A.data;

        ຖ້າ( !A.data )
            continue;

        ຖ້າ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ສຳລັບ( d1 = 0; d1 < d; d1++ )
                ຖ້າ( A.size[d1] > 1 )
                    bha_nga;
        }
        ບໍ່ດັ່ງນັ້ນ
            CV_Assert( A.size == arrays[i0]->size );

        ຖ້າ( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            ສຳລັບ( j = d-1; j > d1; j-- )
                ຖ້າ( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ຖ້າ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ສຳລັບ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ຖ້າ( total1 != (ຈຳນວນເຕັມ)total1 )
                bha_nga;
            size = (ຈຳນວນເຕັມ)total1;
        }

        iterdepth = j;
        ຖ້າ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ສຳລັບ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ບໍ່ດັ່ງນັ້ນ
        iterdepth = 0;

    idx = 0;

    ຖ້າ( !planes )
        ສົ່ງຄືນ;

    ສຳລັບ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        ຖ້າ( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (ຈຳນວນເຕັມ)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ຖ້າ( idx >= nplanes-1 )
        ສົ່ງຄືນ *this;
    ++idx;

    ຖ້າ( iterdepth == 1 )
    {
        ຖ້າ( ptrs )
        {
            ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < narrays; i++ )
            {
                ຖ້າ( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ຖ້າ( planes )
        {
            ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < narrays; i++ )
            {
                ຖ້າ( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            ຖ້າ( !A.data )
                continue;
            ຈຳນວນເຕັມ _idx = (ຈຳນວນເຕັມ)idx;
            uchar* data = A.data;
            ສຳລັບ( ຈຳນວນເຕັມ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ຈຳນວນເຕັມ szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ຖ້າ( ptrs )
                ptrs[i] = data;
            ຖ້າ( planes )
                planes[i].data = data;
        }
    }

    ສົ່ງຄືນ *this;
}

NAryMatIterator NAryMatIterator::operator ++(ຈຳນວນເຕັມ)
{
    NAryMatIterator it = *this;
    ++*this;
    ສົ່ງຄືນ it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    ຖ້າ( !m )
        ສົ່ງຄືນ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ຈຳນວນເຕັມ y = (ຈຳນວນເຕັມ)(ofs/m->step[0]);
    ສົ່ງຄືນ Point((ຈຳນວນເຕັມ)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(ຈຳນວນເຕັມ* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ຈຳນວນເຕັມ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    ຖ້າ(!m)
        ສົ່ງຄືນ 0;
    ຖ້າ( m->isContinuous() )
        ສົ່ງຄືນ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ຈຳນວນເຕັມ i, d = m->dims;
    ຖ້າ( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ສົ່ງຄືນ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    ສຳລັບ( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ສົ່ງຄືນ result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    ຖ້າ( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ຖ້າ( ptr < sliceStart )
            ptr = sliceStart;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( ptr > sliceEnd )
            ptr = sliceEnd;
        ສົ່ງຄືນ;
    }

    ຈຳນວນເຕັມ d = m->dims;
    ຖ້າ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ຖ້າ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ຈຳນວນເຕັມ y1 = std::min(std::max((ຈຳນວນເຕັມ)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ສົ່ງຄືນ;
    }

    ຖ້າ( relative )
        ofs += lpos();

    ຖ້າ( ofs < 0 )
        ofs = 0;

    ຈຳນວນເຕັມ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ຈຳນວນເຕັມ v = (ຈຳນວນເຕັມ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    ສຳລັບ( ຈຳນວນເຕັມ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ຈຳນວນເຕັມ)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ຖ້າ( ofs > 0 )
        ptr = sliceEnd;
    ບໍ່ດັ່ງນັ້ນ
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const ຈຳນວນເຕັມ* _idx, bool relative)
{
    ຈຳນວນເຕັມ d = m->dims;
    ptrdiff_t ofs = 0;
    ຖ້າ( !_idx )
        ;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ບໍ່ດັ່ງນັ້ນ
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ຖ້າ(!_m || !_m->hdr)
        ສົ່ງຄືນ;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ສຳລັບ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ຖ້າ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ສົ່ງຄືນ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ຖ້າ( !ptr || !m || !m->hdr )
        ສົ່ງຄືນ *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    ຖ້າ( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ສົ່ງຄືນ *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ສຳລັບ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ຖ້າ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ສົ່ງຄືນ *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ສົ່ງຄືນ *this;
}

} // cv::
