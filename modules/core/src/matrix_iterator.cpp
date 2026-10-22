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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, ကိန်းပြည့် _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, ကိန်းပြည့် _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, ကိန်းပြည့် _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ကိန်းပြည့် i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    အကယ်၍( narrays < 0 )
    {
        အတွက်( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    အတွက်( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        အကယ်၍( ptrs )
            ptrs[i] = A.data;

        အကယ်၍( !A.data )
            continue;

        အကယ်၍( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            အတွက်( d1 = 0; d1 < d; d1++ )
                အကယ်၍( A.size[d1] > 1 )
                    bha_nga;
        }
        မဟုတ်ပါက
            CV_Assert( A.size == arrays[i0]->size );

        အကယ်၍( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            အတွက်( j = d-1; j > d1; j-- )
                အကယ်၍( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    အကယ်၍( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        အတွက်( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            အကယ်၍( total1 != (ကိန်းပြည့်)total1 )
                bha_nga;
            size = (ကိန်းပြည့်)total1;
        }

        iterdepth = j;
        အကယ်၍( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        အတွက်( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    မဟုတ်ပါက
        iterdepth = 0;

    idx = 0;

    အကယ်၍( !planes )
        ပြန်ပေး;

    အတွက်( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        အကယ်၍( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (ကိန်းပြည့်)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    အကယ်၍( idx >= nplanes-1 )
        ပြန်ပေး *this;
    ++idx;

    အကယ်၍( iterdepth == 1 )
    {
        အကယ်၍( ptrs )
        {
            အတွက်( ကိန်းပြည့် i = 0; i < narrays; i++ )
            {
                အကယ်၍( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        အကယ်၍( planes )
        {
            အတွက်( ကိန်းပြည့် i = 0; i < narrays; i++ )
            {
                အကယ်၍( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    မဟုတ်ပါက
    {
        အတွက်( ကိန်းပြည့် i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            အကယ်၍( !A.data )
                continue;
            ကိန်းပြည့် _idx = (ကိန်းပြည့်)idx;
            uchar* data = A.data;
            အတွက်( ကိန်းပြည့် j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ကိန်းပြည့် szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            အကယ်၍( ptrs )
                ptrs[i] = data;
            အကယ်၍( planes )
                planes[i].data = data;
        }
    }

    ပြန်ပေး *this;
}

NAryMatIterator NAryMatIterator::operator ++(ကိန်းပြည့်)
{
    NAryMatIterator it = *this;
    ++*this;
    ပြန်ပေး it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    အကယ်၍( !m )
        ပြန်ပေး Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ကိန်းပြည့် y = (ကိန်းပြည့်)(ofs/m->step[0]);
    ပြန်ပေး Point((ကိန်းပြည့်)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(ကိန်းပြည့်* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    အတွက်( ကိန်းပြည့် i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ကိန်းပြည့်)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    အကယ်၍(!m)
        ပြန်ပေး 0;
    အကယ်၍( m->isContinuous() )
        ပြန်ပေး (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ကိန်းပြည့် i, d = m->dims;
    အကယ်၍( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ပြန်ပေး y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    အတွက်( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ပြန်ပေး result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    အကယ်၍( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        အကယ်၍( ptr < sliceStart )
            ptr = sliceStart;
        မဟုတ်ပါက အကယ်၍( ptr > sliceEnd )
            ptr = sliceEnd;
        ပြန်ပေး;
    }

    ကိန်းပြည့် d = m->dims;
    အကယ်၍( d == 2 )
    {
        ptrdiff_t ofs0, y;
        အကယ်၍( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ကိန်းပြည့် y1 = std::min(std::max((ကိန်းပြည့်)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ပြန်ပေး;
    }

    အကယ်၍( relative )
        ofs += lpos();

    အကယ်၍( ofs < 0 )
        ofs = 0;

    ကိန်းပြည့် szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ကိန်းပြည့် v = (ကိန်းပြည့်)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    အတွက်( ကိန်းပြည့် i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ကိန်းပြည့်)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    အကယ်၍( ofs > 0 )
        ptr = sliceEnd;
    မဟုတ်ပါက
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const ကိန်းပြည့်* _idx, bool relative)
{
    ကိန်းပြည့် d = m->dims;
    ptrdiff_t ofs = 0;
    အကယ်၍( !_idx )
        ;
    မဟုတ်ပါက အကယ်၍( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    မဟုတ်ပါက
    {
        အတွက်( ကိန်းပြည့် i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    အကယ်၍(!_m || !_m->hdr)
        ပြန်ပေး;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    အတွက်( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        အကယ်၍( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ပြန်ပေး;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    အကယ်၍( !ptr || !m || !m->hdr )
        ပြန်ပေး *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    အကယ်၍( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ပြန်ပေး *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    အတွက်( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        အကယ်၍( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ပြန်ပေး *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ပြန်ပေး *this;
}

} // cv::
