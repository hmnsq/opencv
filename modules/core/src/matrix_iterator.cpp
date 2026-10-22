// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ध्रुव Mat** _arrays, Mat* _planes, पूर्णाङ्क _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ध्रुव Mat** _arrays, uchar** _ptrs, पूर्णाङ्क _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

शून्य NAryMatIterator::init(ध्रुव Mat** _arrays, Mat* _planes, uchar** _ptrs, पूर्णाङ्क _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    पूर्णाङ्क i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    सचे( narrays < 0 )
    {
        अत्थाय( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    अत्थाय( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ध्रुव Mat& A = *arrays[i];
        सचे( ptrs )
            ptrs[i] = A.data;

        सचे( !A.data )
            अनुवर्त;

        सचे( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            अत्थाय( d1 = 0; d1 < d; d1++ )
                सचे( A.size[d1] > 1 )
                    भङ्ग;
        }
        अञ्ञथा
            CV_Assert( A.size == arrays[i0]->size );

        सचे( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            अत्थाय( j = d-1; j > d1; j-- )
                सचे( A.step[j]*A.size[j] < A.step[j-1] )
                    भङ्ग;
            iterdepth = std::max(iterdepth, j);
        }
    }

    सचे( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        अत्थाय( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            सचे( total1 != (पूर्णाङ्क)total1 )
                भङ्ग;
            size = (पूर्णाङ्क)total1;
        }

        iterdepth = j;
        सचे( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        अत्थाय( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    अञ्ञथा
        iterdepth = 0;

    idx = 0;

    सचे( !planes )
        पटिनिवत्त;

    अत्थाय( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ध्रुव Mat& A = *arrays[i];

        सचे( !A.data )
        {
            planes[i] = Mat();
            अनुवर्त;
        }

        planes[i] = Mat(1, (पूर्णाङ्क)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    सचे( idx >= nplanes-1 )
        पटिनिवत्त *एतत्;
    ++idx;

    सचे( iterdepth == 1 )
    {
        सचे( ptrs )
        {
            अत्थाय( पूर्णाङ्क i = 0; i < narrays; i++ )
            {
                सचे( !ptrs[i] )
                    अनुवर्त;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        सचे( planes )
        {
            अत्थाय( पूर्णाङ्क i = 0; i < narrays; i++ )
            {
                सचे( !planes[i].data )
                    अनुवर्त;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    अञ्ञथा
    {
        अत्थाय( पूर्णाङ्क i = 0; i < narrays; i++ )
        {
            ध्रुव Mat& A = *arrays[i];
            सचे( !A.data )
                अनुवर्त;
            पूर्णाङ्क _idx = (पूर्णाङ्क)idx;
            uchar* data = A.data;
            अत्थाय( पूर्णाङ्क j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                पूर्णाङ्क szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            सचे( ptrs )
                ptrs[i] = data;
            सचे( planes )
                planes[i].data = data;
        }
    }

    पटिनिवत्त *एतत्;
}

NAryMatIterator NAryMatIterator::operator ++(पूर्णाङ्क)
{
    NAryMatIterator it = *एतत्;
    ++*एतत्;
    पटिनिवत्त it;
}

//==================================================================================================

Point MatConstIterator::pos() ध्रुव
{
    सचे( !m )
        पटिनिवत्त Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    पूर्णाङ्क y = (पूर्णाङ्क)(ofs/m->step[0]);
    पटिनिवत्त Point((पूर्णाङ्क)((ofs - y*m->step[0])/elemSize), y);
}

शून्य MatConstIterator::pos(पूर्णाङ्क* _idx) ध्रुव
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    अत्थाय( पूर्णाङ्क i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (पूर्णाङ्क)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ध्रुव
{
    सचे(!m)
        पटिनिवत्त 0;
    सचे( m->isContinuous() )
        पटिनिवत्त (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    पूर्णाङ्क i, d = m->dims;
    सचे( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        पटिनिवत्त y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    अत्थाय( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    पटिनिवत्त result;
}

शून्य MatConstIterator::seek(ptrdiff_t ofs, तार्किक relative)
{
    सचे( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        सचे( ptr < sliceStart )
            ptr = sliceStart;
        अञ्ञथा सचे( ptr > sliceEnd )
            ptr = sliceEnd;
        पटिनिवत्त;
    }

    पूर्णाङ्क d = m->dims;
    सचे( d == 2 )
    {
        ptrdiff_t ofs0, y;
        सचे( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        पूर्णाङ्क y1 = std::min(std::max((पूर्णाङ्क)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        पटिनिवत्त;
    }

    सचे( relative )
        ofs += lpos();

    सचे( ofs < 0 )
        ofs = 0;

    पूर्णाङ्क szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    पूर्णाङ्क v = (पूर्णाङ्क)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    अत्थाय( पूर्णाङ्क i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (पूर्णाङ्क)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    सचे( ofs > 0 )
        ptr = sliceEnd;
    अञ्ञथा
        ptr = sliceStart + (ptr - m->ptr());
}

शून्य MatConstIterator::seek(ध्रुव पूर्णाङ्क* _idx, तार्किक relative)
{
    पूर्णाङ्क d = m->dims;
    ptrdiff_t ofs = 0;
    सचे( !_idx )
        ;
    अञ्ञथा सचे( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    अञ्ञथा
    {
        अत्थाय( पूर्णाङ्क i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ध्रुव SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    सचे(!_m || !_m->hdr)
        पटिनिवत्त;
    SparseMat::Hdr& hdr = *m->hdr;
    ध्रुव std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    अत्थाय( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        सचे( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मानम्_Offset;
            पटिनिवत्त;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    सचे( !ptr || !m || !m->hdr )
        पटिनिवत्त *एतत्;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ध्रुव SparseMat::Node*)(ptr - hdr.मानम्_Offset))->next;
    सचे( next )
    {
        ptr = &hdr.pool[next] + hdr.मानम्_Offset;
        पटिनिवत्त *एतत्;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    अत्थाय( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        सचे( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मानम्_Offset;
            पटिनिवत्त *एतत्;
        }
    }
    hashidx = sz;
    ptr = 0;
    पटिनिवत्त *एतत्;
}

} // cv::
