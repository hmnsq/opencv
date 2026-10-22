// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/mat.hpp"

ਨਾਮਾਕਾਸ਼ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ਅਚਲ Mat** _arrays, Mat* _planes, ਪੂਰਨ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ਅਚਲ Mat** _arrays, uchar** _ptrs, ਪੂਰਨ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

ਖਾਲੀ NAryMatIterator::init(ਅਚਲ Mat** _arrays, Mat* _planes, uchar** _ptrs, ਪੂਰਨ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ਪੂਰਨ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ਜੇ( narrays < 0 )
    {
        ਲਈ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ਲਈ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ਅਚਲ Mat& A = *arrays[i];
        ਜੇ( ptrs )
            ptrs[i] = A.ਅੰਕੜੇ;

        ਜੇ( !A.ਅੰਕੜੇ )
            ਜਾਰੀ;

        ਜੇ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ਲਈ( d1 = 0; d1 < d; d1++ )
                ਜੇ( A.size[d1] > 1 )
                    ਰੋਕੋ;
        }
        ਨਹੀਂਤਾਂ
            CV_Assert( A.size == arrays[i0]->size );

        ਜੇ( !A.isContinuous() )
        {
            CV_Assert( A.ਕਦਮ[d-1] == A.elem_ਮਾਪ() );
            ਲਈ( j = d-1; j > d1; j-- )
                ਜੇ( A.ਕਦਮ[j]*A.size[j] < A.ਕਦਮ[j-1] )
                    ਰੋਕੋ;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ਜੇ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ਲਈ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ਜੇ( total1 != (ਪੂਰਨ)total1 )
                ਰੋਕੋ;
            size = (ਪੂਰਨ)total1;
        }

        iterdepth = j;
        ਜੇ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ਲਈ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ਨਹੀਂਤਾਂ
        iterdepth = 0;

    idx = 0;

    ਜੇ( !planes )
        ਵਾਪਸ;

    ਲਈ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ਅਚਲ Mat& A = *arrays[i];

        ਜੇ( !A.ਅੰਕੜੇ )
        {
            planes[i] = Mat();
            ਜਾਰੀ;
        }

        planes[i] = Mat(1, (ਪੂਰਨ)size, A.type(), A.ਅੰਕੜੇ);
    }
}


NAryMatIterator& NAryMatIterator::ਸੰਚਾਲਕ ++()
{
    ਜੇ( idx >= nplanes-1 )
        ਵਾਪਸ *ਇਹ;
    ++idx;

    ਜੇ( iterdepth == 1 )
    {
        ਜੇ( ptrs )
        {
            ਲਈ( ਪੂਰਨ i = 0; i < narrays; i++ )
            {
                ਜੇ( !ptrs[i] )
                    ਜਾਰੀ;
                ptrs[i] = arrays[i]->ਅੰਕੜੇ + arrays[i]->ਕਦਮ[0]*idx;
            }
        }
        ਜੇ( planes )
        {
            ਲਈ( ਪੂਰਨ i = 0; i < narrays; i++ )
            {
                ਜੇ( !planes[i].ਅੰਕੜੇ )
                    ਜਾਰੀ;
                planes[i].ਅੰਕੜੇ = arrays[i]->ਅੰਕੜੇ + arrays[i]->ਕਦਮ[0]*idx;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < narrays; i++ )
        {
            ਅਚਲ Mat& A = *arrays[i];
            ਜੇ( !A.ਅੰਕੜੇ )
                ਜਾਰੀ;
            ਪੂਰਨ _idx = (ਪੂਰਨ)idx;
            uchar* ਅੰਕੜੇ = A.ਅੰਕੜੇ;
            ਲਈ( ਪੂਰਨ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ਪੂਰਨ szi = A.size[j], t = _idx/szi;
                ਅੰਕੜੇ += (_idx - t * szi)*A.ਕਦਮ[j];
                _idx = t;
            }
            ਜੇ( ptrs )
                ptrs[i] = ਅੰਕੜੇ;
            ਜੇ( planes )
                planes[i].ਅੰਕੜੇ = ਅੰਕੜੇ;
        }
    }

    ਵਾਪਸ *ਇਹ;
}

NAryMatIterator NAryMatIterator::ਸੰਚਾਲਕ ++(ਪੂਰਨ)
{
    NAryMatIterator it = *ਇਹ;
    ++*ਇਹ;
    ਵਾਪਸ it;
}

//==================================================================================================

ਬਿੰਦੂ MatConstIterator::pos() ਅਚਲ
{
    ਜੇ( !m )
        ਵਾਪਸ ਬਿੰਦੂ();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ਪੂਰਨ y = (ਪੂਰਨ)(ofs/m->ਕਦਮ[0]);
    ਵਾਪਸ ਬਿੰਦੂ((ਪੂਰਨ)((ofs - y*m->ਕਦਮ[0])/elem_ਮਾਪ), y);
}

ਖਾਲੀ MatConstIterator::pos(ਪੂਰਨ* _idx) ਅਚਲ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ਲਈ( ਪੂਰਨ i = 0; i < m->dims; i++ )
    {
        size_t s = m->ਕਦਮ[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ਪੂਰਨ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ਅਚਲ
{
    ਜੇ(!m)
        ਵਾਪਸ 0;
    ਜੇ( m->isContinuous() )
        ਵਾਪਸ (ptr - ਟੁਕੜਾ_ਸ਼ੁਰੂ)/elem_ਮਾਪ;
    ptrdiff_t ofs = ptr - m->ptr();
    ਪੂਰਨ i, d = m->dims;
    ਜੇ( d == 2 )
    {
        ptrdiff_t y = ofs/m->ਕਦਮ[0];
        ਵਾਪਸ y*m->ਖਾਨੇ_ਗਿਣਤੀ + (ofs - y*m->ਕਦਮ[0])/elem_ਮਾਪ;
    }
    ptrdiff_t ਨਤੀਜਾ = 0;
    ਲਈ( i = 0; i < d; i++ )
    {
        size_t s = m->ਕਦਮ[i], v = ofs/s;
        ofs -= v*s;
        ਨਤੀਜਾ = ਨਤੀਜਾ*m->size[i] + v;
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਖਾਲੀ MatConstIterator::seek(ptrdiff_t ofs, ਤਾਰਕਿਕ relative)
{
    ਜੇ( m->isContinuous() )
    {
        ptr = (relative ? ptr : ਟੁਕੜਾ_ਸ਼ੁਰੂ) + ofs*elem_ਮਾਪ;
        ਜੇ( ptr < ਟੁਕੜਾ_ਸ਼ੁਰੂ )
            ptr = ਟੁਕੜਾ_ਸ਼ੁਰੂ;
        ਨਹੀਂਤਾਂ ਜੇ( ptr > ਟੁਕੜਾ_ਸਿਰਾ )
            ptr = ਟੁਕੜਾ_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    ਪੂਰਨ d = m->dims;
    ਜੇ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ਜੇ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->ਕਦਮ[0];
            ofs += y*m->ਖਾਨੇ_ਗਿਣਤੀ + (ofs0 - y*m->ਕਦਮ[0])/elem_ਮਾਪ;
        }
        y = ofs/m->ਖਾਨੇ_ਗਿਣਤੀ;
        ਪੂਰਨ y1 = std::min(std::max((ਪੂਰਨ)y, 0), m->ਪੰਕਤੀਆਂ-1);
        ਟੁਕੜਾ_ਸ਼ੁਰੂ = m->ptr(y1);
        ਟੁਕੜਾ_ਸਿਰਾ = ਟੁਕੜਾ_ਸ਼ੁਰੂ + m->ਖਾਨੇ_ਗਿਣਤੀ*elem_ਮਾਪ;
        ptr = y < 0 ? ਟੁਕੜਾ_ਸ਼ੁਰੂ : y >= m->ਪੰਕਤੀਆਂ ? ਟੁਕੜਾ_ਸਿਰਾ :
            ਟੁਕੜਾ_ਸ਼ੁਰੂ + (ofs - y*m->ਖਾਨੇ_ਗਿਣਤੀ)*elem_ਮਾਪ;
        ਵਾਪਸ;
    }

    ਜੇ( relative )
        ofs += lpos();

    ਜੇ( ofs < 0 )
        ofs = 0;

    ਪੂਰਨ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ਪੂਰਨ v = (ਪੂਰਨ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_ਮਾਪ;
    ਟੁਕੜਾ_ਸ਼ੁਰੂ = m->ptr();

    ਲਈ( ਪੂਰਨ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ਪੂਰਨ)(ofs - t*szi);
        ofs = t;
        ਟੁਕੜਾ_ਸ਼ੁਰੂ += v*m->ਕਦਮ[i];
    }

    ਟੁਕੜਾ_ਸਿਰਾ = ਟੁਕੜਾ_ਸ਼ੁਰੂ + m->size[d-1]*elem_ਮਾਪ;
    ਜੇ( ofs > 0 )
        ptr = ਟੁਕੜਾ_ਸਿਰਾ;
    ਨਹੀਂਤਾਂ
        ptr = ਟੁਕੜਾ_ਸ਼ੁਰੂ + (ptr - m->ptr());
}

ਖਾਲੀ MatConstIterator::seek(ਅਚਲ ਪੂਰਨ* _idx, ਤਾਰਕਿਕ relative)
{
    ਪੂਰਨ d = m->dims;
    ptrdiff_t ofs = 0;
    ਜੇ( !_idx )
        ;
    ਨਹੀਂਤਾਂ ਜੇ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ਅਚਲ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ਜੇ(!_m || !_m->hdr)
        ਵਾਪਸ;
    SparseMat::Hdr& hdr = *m->hdr;
    ਅਚਲ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ਲਈ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ਜੇ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ਮੁੱਲ_ਵਿੱਥ;
            ਵਾਪਸ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::ਸੰਚਾਲਕ ++()
{
    ਜੇ( !ptr || !m || !m->hdr )
        ਵਾਪਸ *ਇਹ;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ਅਚਲ SparseMat::Node*)(ptr - hdr.ਮੁੱਲ_ਵਿੱਥ))->next;
    ਜੇ( next )
    {
        ptr = &hdr.pool[next] + hdr.ਮੁੱਲ_ਵਿੱਥ;
        ਵਾਪਸ *ਇਹ;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ਲਈ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ਜੇ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ਮੁੱਲ_ਵਿੱਥ;
            ਵਾਪਸ *ਇਹ;
        }
    }
    hashidx = sz;
    ptr = 0;
    ਵਾਪਸ *ਇਹ;
}

} // cv::
