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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, จำนวนเต็ม _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, จำนวนเต็ม _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, จำนวนเต็ม _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    จำนวนเต็ม i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ถ้า( narrays < 0 )
    {
        สำหรับ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    สำหรับ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        ถ้า( ptrs )
            ptrs[i] = A.data;

        ถ้า( !A.data )
            continue;

        ถ้า( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            สำหรับ( d1 = 0; d1 < d; d1++ )
                ถ้า( A.size[d1] > 1 )
                    bha_nga;
        }
        มิฉะนั้น
            CV_Assert( A.size == arrays[i0]->size );

        ถ้า( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            สำหรับ( j = d-1; j > d1; j-- )
                ถ้า( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ถ้า( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        สำหรับ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ถ้า( total1 != (จำนวนเต็ม)total1 )
                bha_nga;
            size = (จำนวนเต็ม)total1;
        }

        iterdepth = j;
        ถ้า( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        สำหรับ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    มิฉะนั้น
        iterdepth = 0;

    idx = 0;

    ถ้า( !planes )
        คืน;

    สำหรับ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        ถ้า( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (จำนวนเต็ม)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ถ้า( idx >= nplanes-1 )
        คืน *this;
    ++idx;

    ถ้า( iterdepth == 1 )
    {
        ถ้า( ptrs )
        {
            สำหรับ( จำนวนเต็ม i = 0; i < narrays; i++ )
            {
                ถ้า( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ถ้า( planes )
        {
            สำหรับ( จำนวนเต็ม i = 0; i < narrays; i++ )
            {
                ถ้า( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    มิฉะนั้น
    {
        สำหรับ( จำนวนเต็ม i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            ถ้า( !A.data )
                continue;
            จำนวนเต็ม _idx = (จำนวนเต็ม)idx;
            uchar* data = A.data;
            สำหรับ( จำนวนเต็ม j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                จำนวนเต็ม szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ถ้า( ptrs )
                ptrs[i] = data;
            ถ้า( planes )
                planes[i].data = data;
        }
    }

    คืน *this;
}

NAryMatIterator NAryMatIterator::operator ++(จำนวนเต็ม)
{
    NAryMatIterator it = *this;
    ++*this;
    คืน it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    ถ้า( !m )
        คืน Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    จำนวนเต็ม y = (จำนวนเต็ม)(ofs/m->step[0]);
    คืน Point((จำนวนเต็ม)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(จำนวนเต็ม* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    สำหรับ( จำนวนเต็ม i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (จำนวนเต็ม)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    ถ้า(!m)
        คืน 0;
    ถ้า( m->isContinuous() )
        คืน (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    จำนวนเต็ม i, d = m->dims;
    ถ้า( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        คืน y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    สำหรับ( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    คืน result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    ถ้า( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ถ้า( ptr < sliceStart )
            ptr = sliceStart;
        มิฉะนั้น ถ้า( ptr > sliceEnd )
            ptr = sliceEnd;
        คืน;
    }

    จำนวนเต็ม d = m->dims;
    ถ้า( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ถ้า( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        จำนวนเต็ม y1 = std::min(std::max((จำนวนเต็ม)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        คืน;
    }

    ถ้า( relative )
        ofs += lpos();

    ถ้า( ofs < 0 )
        ofs = 0;

    จำนวนเต็ม szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    จำนวนเต็ม v = (จำนวนเต็ม)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    สำหรับ( จำนวนเต็ม i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (จำนวนเต็ม)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ถ้า( ofs > 0 )
        ptr = sliceEnd;
    มิฉะนั้น
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const จำนวนเต็ม* _idx, bool relative)
{
    จำนวนเต็ม d = m->dims;
    ptrdiff_t ofs = 0;
    ถ้า( !_idx )
        ;
    มิฉะนั้น ถ้า( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    มิฉะนั้น
    {
        สำหรับ( จำนวนเต็ม i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ถ้า(!_m || !_m->hdr)
        คืน;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    สำหรับ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ถ้า( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            คืน;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ถ้า( !ptr || !m || !m->hdr )
        คืน *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    ถ้า( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        คืน *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    สำหรับ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ถ้า( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            คืน *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    คืน *this;
}

} // cv::
