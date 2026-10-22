// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

فضاینام cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, Mat* _planes, صحیح _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, uchar** _ptrs, صحیح _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

تهی NAryMatIterator::init(ثابت Mat** _arrays, Mat* _planes, uchar** _ptrs, صحیح _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    صحیح i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    اگر( narrays < 0 )
    {
        برای( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    برای( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];
        اگر( ptrs )
            ptrs[i] = A.data;

        اگر( !A.data )
            ادامه;

        اگر( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            برای( d1 = 0; d1 < d; d1++ )
                اگر( A.size[d1] > 1 )
                    بشکن;
        }
        وگرنه
            CV_Assert( A.size == arrays[i0]->size );

        اگر( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            برای( j = d-1; j > d1; j-- )
                اگر( A.step[j]*A.size[j] < A.step[j-1] )
                    بشکن;
            iterdepth = std::max(iterdepth, j);
        }
    }

    اگر( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        برای( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            اگر( total1 != (صحیح)total1 )
                بشکن;
            size = (صحیح)total1;
        }

        iterdepth = j;
        اگر( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        برای( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    وگرنه
        iterdepth = 0;

    idx = 0;

    اگر( !planes )
        بازگشت;

    برای( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];

        اگر( !A.data )
        {
            planes[i] = Mat();
            ادامه;
        }

        planes[i] = Mat(1, (صحیح)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    اگر( idx >= nplanes-1 )
        بازگشت *این;
    ++idx;

    اگر( iterdepth == 1 )
    {
        اگر( ptrs )
        {
            برای( صحیح i = 0; i < narrays; i++ )
            {
                اگر( !ptrs[i] )
                    ادامه;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        اگر( planes )
        {
            برای( صحیح i = 0; i < narrays; i++ )
            {
                اگر( !planes[i].data )
                    ادامه;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    وگرنه
    {
        برای( صحیح i = 0; i < narrays; i++ )
        {
            ثابت Mat& A = *arrays[i];
            اگر( !A.data )
                ادامه;
            صحیح _idx = (صحیح)idx;
            uchar* data = A.data;
            برای( صحیح j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                صحیح szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            اگر( ptrs )
                ptrs[i] = data;
            اگر( planes )
                planes[i].data = data;
        }
    }

    بازگشت *این;
}

NAryMatIterator NAryMatIterator::operator ++(صحیح)
{
    NAryMatIterator it = *این;
    ++*این;
    بازگشت it;
}

//==================================================================================================

Point MatConstIterator::pos() ثابت
{
    اگر( !m )
        بازگشت Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    صحیح y = (صحیح)(ofs/m->step[0]);
    بازگشت Point((صحیح)((ofs - y*m->step[0])/elemSize), y);
}

تهی MatConstIterator::pos(صحیح* _idx) ثابت
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    برای( صحیح i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (صحیح)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ثابت
{
    اگر(!m)
        بازگشت 0;
    اگر( m->isContinuous() )
        بازگشت (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    صحیح i, d = m->dims;
    اگر( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        بازگشت y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    برای( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    بازگشت result;
}

تهی MatConstIterator::seek(ptrdiff_t ofs, منطقی relative)
{
    اگر( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        اگر( ptr < sliceStart )
            ptr = sliceStart;
        وگرنه اگر( ptr > sliceEnd )
            ptr = sliceEnd;
        بازگشت;
    }

    صحیح d = m->dims;
    اگر( d == 2 )
    {
        ptrdiff_t ofs0, y;
        اگر( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        صحیح y1 = std::min(std::max((صحیح)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        بازگشت;
    }

    اگر( relative )
        ofs += lpos();

    اگر( ofs < 0 )
        ofs = 0;

    صحیح szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    صحیح v = (صحیح)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    برای( صحیح i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (صحیح)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    اگر( ofs > 0 )
        ptr = sliceEnd;
    وگرنه
        ptr = sliceStart + (ptr - m->ptr());
}

تهی MatConstIterator::seek(ثابت صحیح* _idx, منطقی relative)
{
    صحیح d = m->dims;
    ptrdiff_t ofs = 0;
    اگر( !_idx )
        ;
    وگرنه اگر( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    وگرنه
    {
        برای( صحیح i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ثابت SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    اگر(!_m || !_m->hdr)
        بازگشت;
    SparseMat::Hdr& hdr = *m->hdr;
    ثابت std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    برای( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        اگر( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.مقدار_Offset;
            بازگشت;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    اگر( !ptr || !m || !m->hdr )
        بازگشت *این;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ثابت SparseMat::Node*)(ptr - hdr.مقدار_Offset))->next;
    اگر( next )
    {
        ptr = &hdr.pool[next] + hdr.مقدار_Offset;
        بازگشت *این;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    برای( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        اگر( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.مقدار_Offset;
            بازگشت *این;
        }
    }
    hashidx = sz;
    ptr = 0;
    بازگشت *این;
}

} // cv::
