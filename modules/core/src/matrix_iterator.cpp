// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, Mat* _planes, صحيح _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, uchar** _ptrs, صحيح _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

فراغ NAryMatIterator::init(ثابت Mat** _arrays, Mat* _planes, uchar** _ptrs, صحيح _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    صحيح i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    אם( narrays < 0 )
    {
        עבור( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    עבור( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];
        אם( ptrs )
            ptrs[i] = A.data;

        אם( !A.data )
            استمر;

        אם( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            עבור( d1 = 0; d1 < d; d1++ )
                אם( A.size[d1] > 1 )
                    اكسر;
        }
        אחרת
            CV_Assert( A.size == arrays[i0]->size );

        אם( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            עבור( j = d-1; j > d1; j-- )
                אם( A.step[j]*A.size[j] < A.step[j-1] )
                    اكسر;
            iterdepth = std::max(iterdepth, j);
        }
    }

    אם( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        עבור( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            אם( total1 != (صحيح)total1 )
                اكسر;
            size = (صحيح)total1;
        }

        iterdepth = j;
        אם( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        עבור( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    אחרת
        iterdepth = 0;

    idx = 0;

    אם( !planes )
        החזר;

    עבור( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];

        אם( !A.data )
        {
            planes[i] = Mat();
            استمر;
        }

        planes[i] = Mat(1, (صحيح)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    אם( idx >= nplanes-1 )
        החזר *هذا;
    ++idx;

    אם( iterdepth == 1 )
    {
        אם( ptrs )
        {
            עבור( صحيح i = 0; i < narrays; i++ )
            {
                אם( !ptrs[i] )
                    استمر;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        אם( planes )
        {
            עבור( صحيح i = 0; i < narrays; i++ )
            {
                אם( !planes[i].data )
                    استمر;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    אחרת
    {
        עבור( صحيح i = 0; i < narrays; i++ )
        {
            ثابت Mat& A = *arrays[i];
            אם( !A.data )
                استمر;
            صحيح _idx = (صحيح)idx;
            uchar* data = A.data;
            עבור( صحيح j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                صحيح szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            אם( ptrs )
                ptrs[i] = data;
            אם( planes )
                planes[i].data = data;
        }
    }

    החזר *هذا;
}

NAryMatIterator NAryMatIterator::operator ++(صحيح)
{
    NAryMatIterator it = *هذا;
    ++*هذا;
    החזר it;
}

//==================================================================================================

Point MatConstIterator::pos() ثابت
{
    אם( !m )
        החזר Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    صحيح y = (صحيح)(ofs/m->step[0]);
    החזר Point((صحيح)((ofs - y*m->step[0])/elemSize), y);
}

فراغ MatConstIterator::pos(صحيح* _idx) ثابت
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    עבור( صحيح i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (صحيح)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ثابت
{
    אם(!m)
        החזר 0;
    אם( m->isContinuous() )
        החזר (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    صحيح i, d = m->dims;
    אם( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        החזר y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    עבור( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    החזר result;
}

فراغ MatConstIterator::seek(ptrdiff_t ofs, منطقي relative)
{
    אם( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        אם( ptr < sliceStart )
            ptr = sliceStart;
        אחרת אם( ptr > sliceEnd )
            ptr = sliceEnd;
        החזר;
    }

    صحيح d = m->dims;
    אם( d == 2 )
    {
        ptrdiff_t ofs0, y;
        אם( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        صحيح y1 = std::min(std::max((صحيح)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        החזר;
    }

    אם( relative )
        ofs += lpos();

    אם( ofs < 0 )
        ofs = 0;

    صحيح szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    صحيح v = (صحيح)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    עבור( صحيح i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (صحيح)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    אם( ofs > 0 )
        ptr = sliceEnd;
    אחרת
        ptr = sliceStart + (ptr - m->ptr());
}

فراغ MatConstIterator::seek(ثابت صحيح* _idx, منطقي relative)
{
    صحيح d = m->dims;
    ptrdiff_t ofs = 0;
    אם( !_idx )
        ;
    אחרת אם( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    אחרת
    {
        עבור( صحيح i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ثابت SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    אם(!_m || !_m->hdr)
        החזר;
    SparseMat::Hdr& hdr = *m->hdr;
    ثابت std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    עבור( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        אם( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            החזר;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    אם( !ptr || !m || !m->hdr )
        החזר *هذا;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ثابت SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    אם( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        החזר *هذا;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    עבור( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        אם( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            החזר *هذا;
        }
    }
    hashidx = sz;
    ptr = 0;
    החזר *هذا;
}

} // cv::
