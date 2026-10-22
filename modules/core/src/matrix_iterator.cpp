// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/mat.hpp"

நாமாகாஶ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(மாறிலி Mat** _arrays, Mat* _planes, முழு _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(மாறிலி Mat** _arrays, uchar** _ptrs, முழு _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

வெறுமை NAryMatIterator::init(மாறிலி Mat** _arrays, Mat* _planes, uchar** _ptrs, முழு _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    முழு i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    எனில்( narrays < 0 )
    {
        க்கு( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    க்கு( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        மாறிலி Mat& A = *arrays[i];
        எனில்( ptrs )
            ptrs[i] = A.data;

        எனில்( !A.data )
            தொடர்;

        எனில்( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            க்கு( d1 = 0; d1 < d; d1++ )
                எனில்( A.size[d1] > 1 )
                    நிறுத்து;
        }
        இல்லையெனில்
            CV_Assert( A.size == arrays[i0]->size );

        எனில்( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            க்கு( j = d-1; j > d1; j-- )
                எனில்( A.step[j]*A.size[j] < A.step[j-1] )
                    நிறுத்து;
            iterdepth = std::max(iterdepth, j);
        }
    }

    எனில்( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        க்கு( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            எனில்( total1 != (முழு)total1 )
                நிறுத்து;
            size = (முழு)total1;
        }

        iterdepth = j;
        எனில்( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        க்கு( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    இல்லையெனில்
        iterdepth = 0;

    idx = 0;

    எனில்( !planes )
        திரும்பு;

    க்கு( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        மாறிலி Mat& A = *arrays[i];

        எனில்( !A.data )
        {
            planes[i] = Mat();
            தொடர்;
        }

        planes[i] = Mat(1, (முழு)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    எனில்( idx >= nplanes-1 )
        திரும்பு *இது;
    ++idx;

    எனில்( iterdepth == 1 )
    {
        எனில்( ptrs )
        {
            க்கு( முழு i = 0; i < narrays; i++ )
            {
                எனில்( !ptrs[i] )
                    தொடர்;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        எனில்( planes )
        {
            க்கு( முழு i = 0; i < narrays; i++ )
            {
                எனில்( !planes[i].data )
                    தொடர்;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < narrays; i++ )
        {
            மாறிலி Mat& A = *arrays[i];
            எனில்( !A.data )
                தொடர்;
            முழு _idx = (முழு)idx;
            uchar* data = A.data;
            க்கு( முழு j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                முழு szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            எனில்( ptrs )
                ptrs[i] = data;
            எனில்( planes )
                planes[i].data = data;
        }
    }

    திரும்பு *இது;
}

NAryMatIterator NAryMatIterator::operator ++(முழு)
{
    NAryMatIterator it = *இது;
    ++*இது;
    திரும்பு it;
}

//==================================================================================================

Point MatConstIterator::pos() மாறிலி
{
    எனில்( !m )
        திரும்பு Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    முழு y = (முழு)(ofs/m->step[0]);
    திரும்பு Point((முழு)((ofs - y*m->step[0])/elemSize), y);
}

வெறுமை MatConstIterator::pos(முழு* _idx) மாறிலி
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    க்கு( முழு i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (முழு)v;
    }
}

ptrdiff_t MatConstIterator::lpos() மாறிலி
{
    எனில்(!m)
        திரும்பு 0;
    எனில்( m->isContinuous() )
        திரும்பு (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    முழு i, d = m->dims;
    எனில்( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        திரும்பு y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    க்கு( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    திரும்பு result;
}

வெறுமை MatConstIterator::seek(ptrdiff_t ofs, தருக்க relative)
{
    எனில்( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        எனில்( ptr < sliceStart )
            ptr = sliceStart;
        இல்லையெனில் எனில்( ptr > sliceEnd )
            ptr = sliceEnd;
        திரும்பு;
    }

    முழு d = m->dims;
    எனில்( d == 2 )
    {
        ptrdiff_t ofs0, y;
        எனில்( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        முழு y1 = std::min(std::max((முழு)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        திரும்பு;
    }

    எனில்( relative )
        ofs += lpos();

    எனில்( ofs < 0 )
        ofs = 0;

    முழு szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    முழு v = (முழு)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    க்கு( முழு i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (முழு)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    எனில்( ofs > 0 )
        ptr = sliceEnd;
    இல்லையெனில்
        ptr = sliceStart + (ptr - m->ptr());
}

வெறுமை MatConstIterator::seek(மாறிலி முழு* _idx, தருக்க relative)
{
    முழு d = m->dims;
    ptrdiff_t ofs = 0;
    எனில்( !_idx )
        ;
    இல்லையெனில் எனில்( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(மாறிலி SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    எனில்(!_m || !_m->hdr)
        திரும்பு;
    SparseMat::Hdr& hdr = *m->hdr;
    மாறிலி std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    க்கு( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        எனில்( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.மதிப்பு_Offset;
            திரும்பு;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    எனில்( !ptr || !m || !m->hdr )
        திரும்பு *இது;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((மாறிலி SparseMat::Node*)(ptr - hdr.மதிப்பு_Offset))->next;
    எனில்( next )
    {
        ptr = &hdr.pool[next] + hdr.மதிப்பு_Offset;
        திரும்பு *இது;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    க்கு( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        எனில்( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.மதிப்பு_Offset;
            திரும்பு *இது;
        }
    }
    hashidx = sz;
    ptr = 0;
    திரும்பு *இது;
}

} // cv::
