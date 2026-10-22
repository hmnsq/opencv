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

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, ចំនួនគត់ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, ចំនួនគត់ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, ចំនួនគត់ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ចំនួនគត់ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ប្រសិនបើ( narrays < 0 )
    {
        សម្រាប់( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    សម្រាប់( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        ប្រសិនបើ( ptrs )
            ptrs[i] = A.data;

        ប្រសិនបើ( !A.data )
            continue;

        ប្រសិនបើ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            សម្រាប់( d1 = 0; d1 < d; d1++ )
                ប្រសិនបើ( A.size[d1] > 1 )
                    bha_nga;
        }
        បើមិនដូច្នេះ
            CV_Assert( A.size == arrays[i0]->size );

        ប្រសិនបើ( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            សម្រាប់( j = d-1; j > d1; j-- )
                ប្រសិនបើ( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ប្រសិនបើ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        សម្រាប់( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ប្រសិនបើ( total1 != (ចំនួនគត់)total1 )
                bha_nga;
            size = (ចំនួនគត់)total1;
        }

        iterdepth = j;
        ប្រសិនបើ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        សម្រាប់( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    បើមិនដូច្នេះ
        iterdepth = 0;

    idx = 0;

    ប្រសិនបើ( !planes )
        ត្រឡប់;

    សម្រាប់( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        ប្រសិនបើ( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (ចំនួនគត់)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ប្រសិនបើ( idx >= nplanes-1 )
        ត្រឡប់ *this;
    ++idx;

    ប្រសិនបើ( iterdepth == 1 )
    {
        ប្រសិនបើ( ptrs )
        {
            សម្រាប់( ចំនួនគត់ i = 0; i < narrays; i++ )
            {
                ប្រសិនបើ( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ប្រសិនបើ( planes )
        {
            សម្រាប់( ចំនួនគត់ i = 0; i < narrays; i++ )
            {
                ប្រសិនបើ( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    បើមិនដូច្នេះ
    {
        សម្រាប់( ចំនួនគត់ i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            ប្រសិនបើ( !A.data )
                continue;
            ចំនួនគត់ _idx = (ចំនួនគត់)idx;
            uchar* data = A.data;
            សម្រាប់( ចំនួនគត់ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ចំនួនគត់ szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ប្រសិនបើ( ptrs )
                ptrs[i] = data;
            ប្រសិនបើ( planes )
                planes[i].data = data;
        }
    }

    ត្រឡប់ *this;
}

NAryMatIterator NAryMatIterator::operator ++(ចំនួនគត់)
{
    NAryMatIterator it = *this;
    ++*this;
    ត្រឡប់ it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    ប្រសិនបើ( !m )
        ត្រឡប់ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ចំនួនគត់ y = (ចំនួនគត់)(ofs/m->step[0]);
    ត្រឡប់ Point((ចំនួនគត់)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(ចំនួនគត់* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    សម្រាប់( ចំនួនគត់ i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ចំនួនគត់)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    ប្រសិនបើ(!m)
        ត្រឡប់ 0;
    ប្រសិនបើ( m->isContinuous() )
        ត្រឡប់ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ចំនួនគត់ i, d = m->dims;
    ប្រសិនបើ( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ត្រឡប់ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    សម្រាប់( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ត្រឡប់ result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    ប្រសិនបើ( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ប្រសិនបើ( ptr < sliceStart )
            ptr = sliceStart;
        បើមិនដូច្នេះ ប្រសិនបើ( ptr > sliceEnd )
            ptr = sliceEnd;
        ត្រឡប់;
    }

    ចំនួនគត់ d = m->dims;
    ប្រសិនបើ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ប្រសិនបើ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ចំនួនគត់ y1 = std::min(std::max((ចំនួនគត់)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ត្រឡប់;
    }

    ប្រសិនបើ( relative )
        ofs += lpos();

    ប្រសិនបើ( ofs < 0 )
        ofs = 0;

    ចំនួនគត់ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ចំនួនគត់ v = (ចំនួនគត់)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    សម្រាប់( ចំនួនគត់ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ចំនួនគត់)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ប្រសិនបើ( ofs > 0 )
        ptr = sliceEnd;
    បើមិនដូច្នេះ
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const ចំនួនគត់* _idx, bool relative)
{
    ចំនួនគត់ d = m->dims;
    ptrdiff_t ofs = 0;
    ប្រសិនបើ( !_idx )
        ;
    បើមិនដូច្នេះ ប្រសិនបើ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    បើមិនដូច្នេះ
    {
        សម្រាប់( ចំនួនគត់ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ប្រសិនបើ(!_m || !_m->hdr)
        ត្រឡប់;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    សម្រាប់( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ប្រសិនបើ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ត្រឡប់;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ប្រសិនបើ( !ptr || !m || !m->hdr )
        ត្រឡប់ *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    ប្រសិនបើ( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ត្រឡប់ *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    សម្រាប់( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ប្រសិនបើ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ត្រឡប់ *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ត្រឡប់ *this;
}

} // cv::
