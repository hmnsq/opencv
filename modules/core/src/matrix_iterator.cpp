// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/mat.hpp"

നാമാകാശ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(സ്ഥിരം Mat** _arrays, Mat* _planes, പൂർണ്ണസംഖ്യ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(സ്ഥിരം Mat** _arrays, uchar** _ptrs, പൂർണ്ണസംഖ്യ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

ശൂന്യം NAryMatIterator::init(സ്ഥിരം Mat** _arrays, Mat* _planes, uchar** _ptrs, പൂർണ്ണസംഖ്യ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    പൂർണ്ണസംഖ്യ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    എങ്കിൽ( narrays < 0 )
    {
        വേണ്ടി( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    വേണ്ടി( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        സ്ഥിരം Mat& A = *arrays[i];
        എങ്കിൽ( ptrs )
            ptrs[i] = A.data;

        എങ്കിൽ( !A.data )
            തുടരുക;

        എങ്കിൽ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            വേണ്ടി( d1 = 0; d1 < d; d1++ )
                എങ്കിൽ( A.size[d1] > 1 )
                    നിർത്തുക;
        }
        അല്ലെങ്കിൽ
            CV_Assert( A.size == arrays[i0]->size );

        എങ്കിൽ( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            വേണ്ടി( j = d-1; j > d1; j-- )
                എങ്കിൽ( A.step[j]*A.size[j] < A.step[j-1] )
                    നിർത്തുക;
            iterdepth = std::max(iterdepth, j);
        }
    }

    എങ്കിൽ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        വേണ്ടി( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            എങ്കിൽ( total1 != (പൂർണ്ണസംഖ്യ)total1 )
                നിർത്തുക;
            size = (പൂർണ്ണസംഖ്യ)total1;
        }

        iterdepth = j;
        എങ്കിൽ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        വേണ്ടി( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    അല്ലെങ്കിൽ
        iterdepth = 0;

    idx = 0;

    എങ്കിൽ( !planes )
        തിരികെ;

    വേണ്ടി( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        സ്ഥിരം Mat& A = *arrays[i];

        എങ്കിൽ( !A.data )
        {
            planes[i] = Mat();
            തുടരുക;
        }

        planes[i] = Mat(1, (പൂർണ്ണസംഖ്യ)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    എങ്കിൽ( idx >= nplanes-1 )
        തിരികെ *ഇത്;
    ++idx;

    എങ്കിൽ( iterdepth == 1 )
    {
        എങ്കിൽ( ptrs )
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < narrays; i++ )
            {
                എങ്കിൽ( !ptrs[i] )
                    തുടരുക;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        എങ്കിൽ( planes )
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < narrays; i++ )
            {
                എങ്കിൽ( !planes[i].data )
                    തുടരുക;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < narrays; i++ )
        {
            സ്ഥിരം Mat& A = *arrays[i];
            എങ്കിൽ( !A.data )
                തുടരുക;
            പൂർണ്ണസംഖ്യ _idx = (പൂർണ്ണസംഖ്യ)idx;
            uchar* data = A.data;
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                പൂർണ്ണസംഖ്യ szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            എങ്കിൽ( ptrs )
                ptrs[i] = data;
            എങ്കിൽ( planes )
                planes[i].data = data;
        }
    }

    തിരികെ *ഇത്;
}

NAryMatIterator NAryMatIterator::operator ++(പൂർണ്ണസംഖ്യ)
{
    NAryMatIterator it = *ഇത്;
    ++*ഇത്;
    തിരികെ it;
}

//==================================================================================================

Point MatConstIterator::pos() സ്ഥിരം
{
    എങ്കിൽ( !m )
        തിരികെ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    പൂർണ്ണസംഖ്യ y = (പൂർണ്ണസംഖ്യ)(ofs/m->step[0]);
    തിരികെ Point((പൂർണ്ണസംഖ്യ)((ofs - y*m->step[0])/elemSize), y);
}

ശൂന്യം MatConstIterator::pos(പൂർണ്ണസംഖ്യ* _idx) സ്ഥിരം
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (പൂർണ്ണസംഖ്യ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() സ്ഥിരം
{
    എങ്കിൽ(!m)
        തിരികെ 0;
    എങ്കിൽ( m->isContinuous() )
        തിരികെ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    പൂർണ്ണസംഖ്യ i, d = m->dims;
    എങ്കിൽ( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        തിരികെ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    വേണ്ടി( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    തിരികെ result;
}

ശൂന്യം MatConstIterator::seek(ptrdiff_t ofs, തർക്കം relative)
{
    എങ്കിൽ( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        എങ്കിൽ( ptr < sliceStart )
            ptr = sliceStart;
        അല്ലെങ്കിൽ എങ്കിൽ( ptr > sliceEnd )
            ptr = sliceEnd;
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ d = m->dims;
    എങ്കിൽ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        എങ്കിൽ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        പൂർണ്ണസംഖ്യ y1 = std::min(std::max((പൂർണ്ണസംഖ്യ)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        തിരികെ;
    }

    എങ്കിൽ( relative )
        ofs += lpos();

    എങ്കിൽ( ofs < 0 )
        ofs = 0;

    പൂർണ്ണസംഖ്യ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (പൂർണ്ണസംഖ്യ)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    എങ്കിൽ( ofs > 0 )
        ptr = sliceEnd;
    അല്ലെങ്കിൽ
        ptr = sliceStart + (ptr - m->ptr());
}

ശൂന്യം MatConstIterator::seek(സ്ഥിരം പൂർണ്ണസംഖ്യ* _idx, തർക്കം relative)
{
    പൂർണ്ണസംഖ്യ d = m->dims;
    ptrdiff_t ofs = 0;
    എങ്കിൽ( !_idx )
        ;
    അല്ലെങ്കിൽ എങ്കിൽ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(സ്ഥിരം SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    എങ്കിൽ(!_m || !_m->hdr)
        തിരികെ;
    SparseMat::Hdr& hdr = *m->hdr;
    സ്ഥിരം std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    വേണ്ടി( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        എങ്കിൽ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.മൂല്യം_Offset;
            തിരികെ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    എങ്കിൽ( !ptr || !m || !m->hdr )
        തിരികെ *ഇത്;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((സ്ഥിരം SparseMat::Node*)(ptr - hdr.മൂല്യം_Offset))->next;
    എങ്കിൽ( next )
    {
        ptr = &hdr.pool[next] + hdr.മൂല്യം_Offset;
        തിരികെ *ഇത്;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    വേണ്ടി( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        എങ്കിൽ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.മൂല്യം_Offset;
            തിരികെ *ഇത്;
        }
    }
    hashidx = sz;
    ptr = 0;
    തിരികെ *ഇത്;
}

} // cv::
