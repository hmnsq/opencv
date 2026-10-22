// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/mat.hpp"

నామాకాశ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(స్థిరరాశి Mat** _arrays, Mat* _planes, పూర్ణాంకం _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(స్థిరరాశి Mat** _arrays, uchar** _ptrs, పూర్ణాంకం _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

శూన్య NAryMatIterator::init(స్థిరరాశి Mat** _arrays, Mat* _planes, uchar** _ptrs, పూర్ణాంకం _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    పూర్ణాంకం i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ఒకవేళ( narrays < 0 )
    {
        కొరకు( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    కొరకు( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        స్థిరరాశి Mat& A = *arrays[i];
        ఒకవేళ( ptrs )
            ptrs[i] = A.దత్తాంశం;

        ఒకవేళ( !A.దత్తాంశం )
            కొనసాగు;

        ఒకవేళ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            కొరకు( d1 = 0; d1 < d; d1++ )
                ఒకవేళ( A.size[d1] > 1 )
                    ఆపు;
        }
        లేకపోతే
            CV_Assert( A.size == arrays[i0]->size );

        ఒకవేళ( !A.isContinuous() )
        {
            CV_Assert( A.అడుగు[d-1] == A.elem_కొలత() );
            కొరకు( j = d-1; j > d1; j-- )
                ఒకవేళ( A.అడుగు[j]*A.size[j] < A.అడుగు[j-1] )
                    ఆపు;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ఒకవేళ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        కొరకు( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ఒకవేళ( total1 != (పూర్ణాంకం)total1 )
                ఆపు;
            size = (పూర్ణాంకం)total1;
        }

        iterdepth = j;
        ఒకవేళ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        కొరకు( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    లేకపోతే
        iterdepth = 0;

    idx = 0;

    ఒకవేళ( !planes )
        తిరిగి;

    కొరకు( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        స్థిరరాశి Mat& A = *arrays[i];

        ఒకవేళ( !A.దత్తాంశం )
        {
            planes[i] = Mat();
            కొనసాగు;
        }

        planes[i] = Mat(1, (పూర్ణాంకం)size, A.type(), A.దత్తాంశం);
    }
}


NAryMatIterator& NAryMatIterator::క్రియాకారకం ++()
{
    ఒకవేళ( idx >= nplanes-1 )
        తిరిగి *ఇది;
    ++idx;

    ఒకవేళ( iterdepth == 1 )
    {
        ఒకవేళ( ptrs )
        {
            కొరకు( పూర్ణాంకం i = 0; i < narrays; i++ )
            {
                ఒకవేళ( !ptrs[i] )
                    కొనసాగు;
                ptrs[i] = arrays[i]->దత్తాంశం + arrays[i]->అడుగు[0]*idx;
            }
        }
        ఒకవేళ( planes )
        {
            కొరకు( పూర్ణాంకం i = 0; i < narrays; i++ )
            {
                ఒకవేళ( !planes[i].దత్తాంశం )
                    కొనసాగు;
                planes[i].దత్తాంశం = arrays[i]->దత్తాంశం + arrays[i]->అడుగు[0]*idx;
            }
        }
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < narrays; i++ )
        {
            స్థిరరాశి Mat& A = *arrays[i];
            ఒకవేళ( !A.దత్తాంశం )
                కొనసాగు;
            పూర్ణాంకం _idx = (పూర్ణాంకం)idx;
            uchar* దత్తాంశం = A.దత్తాంశం;
            కొరకు( పూర్ణాంకం j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                పూర్ణాంకం szi = A.size[j], t = _idx/szi;
                దత్తాంశం += (_idx - t * szi)*A.అడుగు[j];
                _idx = t;
            }
            ఒకవేళ( ptrs )
                ptrs[i] = దత్తాంశం;
            ఒకవేళ( planes )
                planes[i].దత్తాంశం = దత్తాంశం;
        }
    }

    తిరిగి *ఇది;
}

NAryMatIterator NAryMatIterator::క్రియాకారకం ++(పూర్ణాంకం)
{
    NAryMatIterator it = *ఇది;
    ++*ఇది;
    తిరిగి it;
}

//==================================================================================================

బిందువు MatConstIterator::pos() స్థిరరాశి
{
    ఒకవేళ( !m )
        తిరిగి బిందువు();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    పూర్ణాంకం y = (పూర్ణాంకం)(ofs/m->అడుగు[0]);
    తిరిగి బిందువు((పూర్ణాంకం)((ofs - y*m->అడుగు[0])/elem_కొలత), y);
}

శూన్య MatConstIterator::pos(పూర్ణాంకం* _idx) స్థిరరాశి
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    కొరకు( పూర్ణాంకం i = 0; i < m->dims; i++ )
    {
        size_t s = m->అడుగు[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (పూర్ణాంకం)v;
    }
}

ptrdiff_t MatConstIterator::lpos() స్థిరరాశి
{
    ఒకవేళ(!m)
        తిరిగి 0;
    ఒకవేళ( m->isContinuous() )
        తిరిగి (ptr - ముక్క_ప్రారంభం)/elem_కొలత;
    ptrdiff_t ofs = ptr - m->ptr();
    పూర్ణాంకం i, d = m->dims;
    ఒకవేళ( d == 2 )
    {
        ptrdiff_t y = ofs/m->అడుగు[0];
        తిరిగి y*m->నిలువుల_సంఖ్య + (ofs - y*m->అడుగు[0])/elem_కొలత;
    }
    ptrdiff_t ఫలితం = 0;
    కొరకు( i = 0; i < d; i++ )
    {
        size_t s = m->అడుగు[i], v = ofs/s;
        ofs -= v*s;
        ఫలితం = ఫలితం*m->size[i] + v;
    }
    తిరిగి ఫలితం;
}

శూన్య MatConstIterator::seek(ptrdiff_t ofs, తార్కిక relative)
{
    ఒకవేళ( m->isContinuous() )
    {
        ptr = (relative ? ptr : ముక్క_ప్రారంభం) + ofs*elem_కొలత;
        ఒకవేళ( ptr < ముక్క_ప్రారంభం )
            ptr = ముక్క_ప్రారంభం;
        లేకపోతే ఒకవేళ( ptr > ముక్క_చివర )
            ptr = ముక్క_చివర;
        తిరిగి;
    }

    పూర్ణాంకం d = m->dims;
    ఒకవేళ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ఒకవేళ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->అడుగు[0];
            ofs += y*m->నిలువుల_సంఖ్య + (ofs0 - y*m->అడుగు[0])/elem_కొలత;
        }
        y = ofs/m->నిలువుల_సంఖ్య;
        పూర్ణాంకం y1 = std::min(std::max((పూర్ణాంకం)y, 0), m->అడ్డువరుసలు-1);
        ముక్క_ప్రారంభం = m->ptr(y1);
        ముక్క_చివర = ముక్క_ప్రారంభం + m->నిలువుల_సంఖ్య*elem_కొలత;
        ptr = y < 0 ? ముక్క_ప్రారంభం : y >= m->అడ్డువరుసలు ? ముక్క_చివర :
            ముక్క_ప్రారంభం + (ofs - y*m->నిలువుల_సంఖ్య)*elem_కొలత;
        తిరిగి;
    }

    ఒకవేళ( relative )
        ofs += lpos();

    ఒకవేళ( ofs < 0 )
        ofs = 0;

    పూర్ణాంకం szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    పూర్ణాంకం v = (పూర్ణాంకం)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_కొలత;
    ముక్క_ప్రారంభం = m->ptr();

    కొరకు( పూర్ణాంకం i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (పూర్ణాంకం)(ofs - t*szi);
        ofs = t;
        ముక్క_ప్రారంభం += v*m->అడుగు[i];
    }

    ముక్క_చివర = ముక్క_ప్రారంభం + m->size[d-1]*elem_కొలత;
    ఒకవేళ( ofs > 0 )
        ptr = ముక్క_చివర;
    లేకపోతే
        ptr = ముక్క_ప్రారంభం + (ptr - m->ptr());
}

శూన్య MatConstIterator::seek(స్థిరరాశి పూర్ణాంకం* _idx, తార్కిక relative)
{
    పూర్ణాంకం d = m->dims;
    ptrdiff_t ofs = 0;
    ఒకవేళ( !_idx )
        ;
    లేకపోతే ఒకవేళ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(స్థిరరాశి SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ఒకవేళ(!_m || !_m->hdr)
        తిరిగి;
    SparseMat::Hdr& hdr = *m->hdr;
    స్థిరరాశి std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    కొరకు( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ఒకవేళ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.విలువ_స్థానభ్రంశం;
            తిరిగి;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::క్రియాకారకం ++()
{
    ఒకవేళ( !ptr || !m || !m->hdr )
        తిరిగి *ఇది;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((స్థిరరాశి SparseMat::Node*)(ptr - hdr.విలువ_స్థానభ్రంశం))->next;
    ఒకవేళ( next )
    {
        ptr = &hdr.pool[next] + hdr.విలువ_స్థానభ్రంశం;
        తిరిగి *ఇది;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    కొరకు( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ఒకవేళ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.విలువ_స్థానభ్రంశం;
            తిరిగి *ఇది;
        }
    }
    hashidx = sz;
    ptr = 0;
    తిరిగి *ఇది;
}

} // cv::
