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

    यदि( narrays < 0 )
    {
        प्रति( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    प्रति( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ध्रुव Mat& A = *arrays[i];
        यदि( ptrs )
            ptrs[i] = A.दत्त;

        यदि( !A.दत्त )
            अनुवर्त;

        यदि( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            प्रति( d1 = 0; d1 < d; d1++ )
                यदि( A.size[d1] > 1 )
                    भङ्ग;
        }
        अन्यथा
            CV_Assert( A.size == arrays[i0]->size );

        यदि( !A.isContinuous() )
        {
            CV_Assert( A.चरणः[d-1] == A.elem_आकार() );
            प्रति( j = d-1; j > d1; j-- )
                यदि( A.चरणः[j]*A.size[j] < A.चरणः[j-1] )
                    भङ्ग;
            iterdepth = std::max(iterdepth, j);
        }
    }

    यदि( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        प्रति( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            यदि( total1 != (पूर्णाङ्क)total1 )
                भङ्ग;
            size = (पूर्णाङ्क)total1;
        }

        iterdepth = j;
        यदि( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        प्रति( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    अन्यथा
        iterdepth = 0;

    idx = 0;

    यदि( !planes )
        प्रत्यागम;

    प्रति( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ध्रुव Mat& A = *arrays[i];

        यदि( !A.दत्त )
        {
            planes[i] = Mat();
            अनुवर्त;
        }

        planes[i] = Mat(1, (पूर्णाङ्क)size, A.type(), A.दत्त);
    }
}


NAryMatIterator& NAryMatIterator::संक्रियकः ++()
{
    यदि( idx >= nplanes-1 )
        प्रत्यागम *एतत्;
    ++idx;

    यदि( iterdepth == 1 )
    {
        यदि( ptrs )
        {
            प्रति( पूर्णाङ्क i = 0; i < narrays; i++ )
            {
                यदि( !ptrs[i] )
                    अनुवर्त;
                ptrs[i] = arrays[i]->दत्त + arrays[i]->चरणः[0]*idx;
            }
        }
        यदि( planes )
        {
            प्रति( पूर्णाङ्क i = 0; i < narrays; i++ )
            {
                यदि( !planes[i].दत्त )
                    अनुवर्त;
                planes[i].दत्त = arrays[i]->दत्त + arrays[i]->चरणः[0]*idx;
            }
        }
    }
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < narrays; i++ )
        {
            ध्रुव Mat& A = *arrays[i];
            यदि( !A.दत्त )
                अनुवर्त;
            पूर्णाङ्क _idx = (पूर्णाङ्क)idx;
            uchar* दत्त = A.दत्त;
            प्रति( पूर्णाङ्क j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                पूर्णाङ्क szi = A.size[j], t = _idx/szi;
                दत्त += (_idx - t * szi)*A.चरणः[j];
                _idx = t;
            }
            यदि( ptrs )
                ptrs[i] = दत्त;
            यदि( planes )
                planes[i].दत्त = दत्त;
        }
    }

    प्रत्यागम *एतत्;
}

NAryMatIterator NAryMatIterator::संक्रियकः ++(पूर्णाङ्क)
{
    NAryMatIterator it = *एतत्;
    ++*एतत्;
    प्रत्यागम it;
}

//==================================================================================================

Point MatConstIterator::pos() ध्रुव
{
    यदि( !m )
        प्रत्यागम Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    पूर्णाङ्क y = (पूर्णाङ्क)(ofs/m->चरणः[0]);
    प्रत्यागम Point((पूर्णाङ्क)((ofs - y*m->चरणः[0])/elem_आकार), y);
}

शून्य MatConstIterator::pos(पूर्णाङ्क* _idx) ध्रुव
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    प्रति( पूर्णाङ्क i = 0; i < m->dims; i++ )
    {
        size_t s = m->चरणः[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (पूर्णाङ्क)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ध्रुव
{
    यदि(!m)
        प्रत्यागम 0;
    यदि( m->isContinuous() )
        प्रत्यागम (ptr - खण्डिका_Start)/elem_आकार;
    ptrdiff_t ofs = ptr - m->ptr();
    पूर्णाङ्क i, d = m->dims;
    यदि( d == 2 )
    {
        ptrdiff_t y = ofs/m->चरणः[0];
        प्रत्यागम y*m->स्तम्भसङ्ख्या + (ofs - y*m->चरणः[0])/elem_आकार;
    }
    ptrdiff_t परिणाम = 0;
    प्रति( i = 0; i < d; i++ )
    {
        size_t s = m->चरणः[i], v = ofs/s;
        ofs -= v*s;
        परिणाम = परिणाम*m->size[i] + v;
    }
    प्रत्यागम परिणाम;
}

शून्य MatConstIterator::seek(ptrdiff_t ofs, तार्किक relative)
{
    यदि( m->isContinuous() )
    {
        ptr = (relative ? ptr : खण्डिका_Start) + ofs*elem_आकार;
        यदि( ptr < खण्डिका_Start )
            ptr = खण्डिका_Start;
        अन्यथा यदि( ptr > खण्डिका_अन्तः )
            ptr = खण्डिका_अन्तः;
        प्रत्यागम;
    }

    पूर्णाङ्क d = m->dims;
    यदि( d == 2 )
    {
        ptrdiff_t ofs0, y;
        यदि( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->चरणः[0];
            ofs += y*m->स्तम्भसङ्ख्या + (ofs0 - y*m->चरणः[0])/elem_आकार;
        }
        y = ofs/m->स्तम्भसङ्ख्या;
        पूर्णाङ्क y1 = std::min(std::max((पूर्णाङ्क)y, 0), m->पङ्क्तिकाः-1);
        खण्डिका_Start = m->ptr(y1);
        खण्डिका_अन्तः = खण्डिका_Start + m->स्तम्भसङ्ख्या*elem_आकार;
        ptr = y < 0 ? खण्डिका_Start : y >= m->पङ्क्तिकाः ? खण्डिका_अन्तः :
            खण्डिका_Start + (ofs - y*m->स्तम्भसङ्ख्या)*elem_आकार;
        प्रत्यागम;
    }

    यदि( relative )
        ofs += lpos();

    यदि( ofs < 0 )
        ofs = 0;

    पूर्णाङ्क szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    पूर्णाङ्क v = (पूर्णाङ्क)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_आकार;
    खण्डिका_Start = m->ptr();

    प्रति( पूर्णाङ्क i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (पूर्णाङ्क)(ofs - t*szi);
        ofs = t;
        खण्डिका_Start += v*m->चरणः[i];
    }

    खण्डिका_अन्तः = खण्डिका_Start + m->size[d-1]*elem_आकार;
    यदि( ofs > 0 )
        ptr = खण्डिका_अन्तः;
    अन्यथा
        ptr = खण्डिका_Start + (ptr - m->ptr());
}

शून्य MatConstIterator::seek(ध्रुव पूर्णाङ्क* _idx, तार्किक relative)
{
    पूर्णाङ्क d = m->dims;
    ptrdiff_t ofs = 0;
    यदि( !_idx )
        ;
    अन्यथा यदि( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ध्रुव SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    यदि(!_m || !_m->hdr)
        प्रत्यागम;
    SparseMat::Hdr& hdr = *m->hdr;
    ध्रुव std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    प्रति( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        यदि( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मानम्_विस्थापनम्;
            प्रत्यागम;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::संक्रियकः ++()
{
    यदि( !ptr || !m || !m->hdr )
        प्रत्यागम *एतत्;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ध्रुव SparseMat::Node*)(ptr - hdr.मानम्_विस्थापनम्))->next;
    यदि( next )
    {
        ptr = &hdr.pool[next] + hdr.मानम्_विस्थापनम्;
        प्रत्यागम *एतत्;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    प्रति( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        यदि( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मानम्_विस्थापनम्;
            प्रत्यागम *एतत्;
        }
    }
    hashidx = sz;
    ptr = 0;
    प्रत्यागम *एतत्;
}

} // cv::
