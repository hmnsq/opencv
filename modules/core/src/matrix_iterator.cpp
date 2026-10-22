// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/mat.hpp"

ନାମାକାଶ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ଅଚଳ Mat** _arrays, Mat* _planes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ଅଚଳ Mat** _arrays, uchar** _ptrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

ଶୂନ୍ୟ NAryMatIterator::init(ଅଚଳ Mat** _arrays, Mat* _planes, uchar** _ptrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ଯଦି( narrays < 0 )
    {
        ପାଇଁ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ପାଇଁ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ଅଚଳ Mat& A = *arrays[i];
        ଯଦି( ptrs )
            ptrs[i] = A.data;

        ଯଦି( !A.data )
            ଜାରି;

        ଯଦି( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ପାଇଁ( d1 = 0; d1 < d; d1++ )
                ଯଦି( A.size[d1] > 1 )
                    ଭାଙ୍ଗ;
        }
        ଅନ୍ୟଥା
            CV_Assert( A.size == arrays[i0]->size );

        ଯଦି( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            ପାଇଁ( j = d-1; j > d1; j-- )
                ଯଦି( A.step[j]*A.size[j] < A.step[j-1] )
                    ଭାଙ୍ଗ;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ଯଦି( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ପାଇଁ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ଯଦି( total1 != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total1 )
                ଭାଙ୍ଗ;
            size = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total1;
        }

        iterdepth = j;
        ଯଦି( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ପାଇଁ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ଅନ୍ୟଥା
        iterdepth = 0;

    idx = 0;

    ଯଦି( !planes )
        ଫେରନ୍ତୁ;

    ପାଇଁ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ଅଚଳ Mat& A = *arrays[i];

        ଯଦି( !A.data )
        {
            planes[i] = Mat();
            ଜାରି;
        }

        planes[i] = Mat(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ଯଦି( idx >= nplanes-1 )
        ଫେରନ୍ତୁ *ଏହି;
    ++idx;

    ଯଦି( iterdepth == 1 )
    {
        ଯଦି( ptrs )
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < narrays; i++ )
            {
                ଯଦି( !ptrs[i] )
                    ଜାରି;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ଯଦି( planes )
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < narrays; i++ )
            {
                ଯଦି( !planes[i].data )
                    ଜାରି;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < narrays; i++ )
        {
            ଅଚଳ Mat& A = *arrays[i];
            ଯଦି( !A.data )
                ଜାରି;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା _idx = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)idx;
            uchar* data = A.data;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ଯଦି( ptrs )
                ptrs[i] = data;
            ଯଦି( planes )
                planes[i].data = data;
        }
    }

    ଫେରନ୍ତୁ *ଏହି;
}

NAryMatIterator NAryMatIterator::operator ++(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
{
    NAryMatIterator it = *ଏହି;
    ++*ଏହି;
    ଫେରନ୍ତୁ it;
}

//==================================================================================================

Point MatConstIterator::pos() ଅଚଳ
{
    ଯଦି( !m )
        ଫେରନ୍ତୁ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ofs/m->step[0]);
    ଫେରନ୍ତୁ Point((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((ofs - y*m->step[0])/elemSize), y);
}

ଶୂନ୍ୟ MatConstIterator::pos(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _idx) ଅଚଳ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ଅଚଳ
{
    ଯଦି(!m)
        ଫେରନ୍ତୁ 0;
    ଯଦି( m->isContinuous() )
        ଫେରନ୍ତୁ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = m->dims;
    ଯଦି( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ଫେରନ୍ତୁ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    ପାଇଁ( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ MatConstIterator::seek(ptrdiff_t ofs, ତାର୍କିକ relative)
{
    ଯଦି( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ଯଦି( ptr < sliceStart )
            ptr = sliceStart;
        ଅନ୍ୟଥା ଯଦି( ptr > sliceEnd )
            ptr = sliceEnd;
        ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m->dims;
    ଯଦି( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ଯଦି( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1 = std::min(std::max((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ଫେରନ୍ତୁ;
    }

    ଯଦି( relative )
        ofs += lpos();

    ଯଦି( ofs < 0 )
        ofs = 0;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ଯଦି( ofs > 0 )
        ptr = sliceEnd;
    ଅନ୍ୟଥା
        ptr = sliceStart + (ptr - m->ptr());
}

ଶୂନ୍ୟ MatConstIterator::seek(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _idx, ତାର୍କିକ relative)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m->dims;
    ptrdiff_t ofs = 0;
    ଯଦି( !_idx )
        ;
    ଅନ୍ୟଥା ଯଦି( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ଅଚଳ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ଯଦି(!_m || !_m->hdr)
        ଫେରନ୍ତୁ;
    SparseMat::Hdr& hdr = *m->hdr;
    ଅଚଳ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ପାଇଁ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ଯଦି( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ମୂଲ୍ୟ_Offset;
            ଫେରନ୍ତୁ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ଯଦି( !ptr || !m || !m->hdr )
        ଫେରନ୍ତୁ *ଏହି;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ଅଚଳ SparseMat::Node*)(ptr - hdr.ମୂଲ୍ୟ_Offset))->next;
    ଯଦି( next )
    {
        ptr = &hdr.pool[next] + hdr.ମୂଲ୍ୟ_Offset;
        ଫେରନ୍ତୁ *ଏହି;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ପାଇଁ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ଯଦି( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ମୂଲ୍ୟ_Offset;
            ଫେରନ୍ତୁ *ଏହି;
        }
    }
    hashidx = sz;
    ptr = 0;
    ଫେରନ୍ତୁ *ଏହି;
}

} // cv::
