// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ಸ್ಥಿರಾಂಕ Mat** _arrays, Mat* _planes, ಪೂರ್ಣಾಂಕ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ಸ್ಥಿರಾಂಕ Mat** _arrays, uchar** _ptrs, ಪೂರ್ಣಾಂಕ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

ಶೂನ್ಯ NAryMatIterator::init(ಸ್ಥಿರಾಂಕ Mat** _arrays, Mat* _planes, uchar** _ptrs, ಪೂರ್ಣಾಂಕ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ಪೂರ್ಣಾಂಕ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ಆದರೆ( narrays < 0 )
    {
        ಗಾಗಿ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ಗಾಗಿ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ಸ್ಥಿರಾಂಕ Mat& A = *arrays[i];
        ಆದರೆ( ptrs )
            ptrs[i] = A.ದತ್ತಾಂಶ;

        ಆದರೆ( !A.ದತ್ತಾಂಶ )
            ಮುಂದುವರಿ;

        ಆದರೆ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ಗಾಗಿ( d1 = 0; d1 < d; d1++ )
                ಆದರೆ( A.size[d1] > 1 )
                    ನಿಲ್ಲಿಸು;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            CV_Assert( A.size == arrays[i0]->size );

        ಆದರೆ( !A.isContinuous() )
        {
            CV_Assert( A.ಹೆಜ್ಜೆ[d-1] == A.elem_ಗಾತ್ರ() );
            ಗಾಗಿ( j = d-1; j > d1; j-- )
                ಆದರೆ( A.ಹೆಜ್ಜೆ[j]*A.size[j] < A.ಹೆಜ್ಜೆ[j-1] )
                    ನಿಲ್ಲಿಸು;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ಆದರೆ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ಗಾಗಿ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ಆದರೆ( total1 != (ಪೂರ್ಣಾಂಕ)total1 )
                ನಿಲ್ಲಿಸು;
            size = (ಪೂರ್ಣಾಂಕ)total1;
        }

        iterdepth = j;
        ಆದರೆ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ಗಾಗಿ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ
        iterdepth = 0;

    idx = 0;

    ಆದರೆ( !planes )
        ಹಿಂತಿರುಗಿ;

    ಗಾಗಿ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ಸ್ಥಿರಾಂಕ Mat& A = *arrays[i];

        ಆದರೆ( !A.ದತ್ತಾಂಶ )
        {
            planes[i] = Mat();
            ಮುಂದುವರಿ;
        }

        planes[i] = Mat(1, (ಪೂರ್ಣಾಂಕ)size, A.type(), A.ದತ್ತಾಂಶ);
    }
}


NAryMatIterator& NAryMatIterator::ಕ್ರಿಯಾಕಾರಕ ++()
{
    ಆದರೆ( idx >= nplanes-1 )
        ಹಿಂತಿರುಗಿ *ಇದು;
    ++idx;

    ಆದರೆ( iterdepth == 1 )
    {
        ಆದರೆ( ptrs )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < narrays; i++ )
            {
                ಆದರೆ( !ptrs[i] )
                    ಮುಂದುವರಿ;
                ptrs[i] = arrays[i]->ದತ್ತಾಂಶ + arrays[i]->ಹೆಜ್ಜೆ[0]*idx;
            }
        }
        ಆದರೆ( planes )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < narrays; i++ )
            {
                ಆದರೆ( !planes[i].ದತ್ತಾಂಶ )
                    ಮುಂದುವರಿ;
                planes[i].ದತ್ತಾಂಶ = arrays[i]->ದತ್ತಾಂಶ + arrays[i]->ಹೆಜ್ಜೆ[0]*idx;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < narrays; i++ )
        {
            ಸ್ಥಿರಾಂಕ Mat& A = *arrays[i];
            ಆದರೆ( !A.ದತ್ತಾಂಶ )
                ಮುಂದುವರಿ;
            ಪೂರ್ಣಾಂಕ _idx = (ಪೂರ್ಣಾಂಕ)idx;
            uchar* ದತ್ತಾಂಶ = A.ದತ್ತಾಂಶ;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ಪೂರ್ಣಾಂಕ szi = A.size[j], t = _idx/szi;
                ದತ್ತಾಂಶ += (_idx - t * szi)*A.ಹೆಜ್ಜೆ[j];
                _idx = t;
            }
            ಆದರೆ( ptrs )
                ptrs[i] = ದತ್ತಾಂಶ;
            ಆದರೆ( planes )
                planes[i].ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
        }
    }

    ಹಿಂತಿರುಗಿ *ಇದು;
}

NAryMatIterator NAryMatIterator::ಕ್ರಿಯಾಕಾರಕ ++(ಪೂರ್ಣಾಂಕ)
{
    NAryMatIterator it = *ಇದು;
    ++*ಇದು;
    ಹಿಂತಿರುಗಿ it;
}

//==================================================================================================

ಬಿಂದು MatConstIterator::pos() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( !m )
        ಹಿಂತಿರುಗಿ ಬಿಂದು();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ಪೂರ್ಣಾಂಕ y = (ಪೂರ್ಣಾಂಕ)(ofs/m->ಹೆಜ್ಜೆ[0]);
    ಹಿಂತಿರುಗಿ ಬಿಂದು((ಪೂರ್ಣಾಂಕ)((ofs - y*m->ಹೆಜ್ಜೆ[0])/elem_ಗಾತ್ರ), y);
}

ಶೂನ್ಯ MatConstIterator::pos(ಪೂರ್ಣಾಂಕ* _idx) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < m->dims; i++ )
    {
        size_t s = m->ಹೆಜ್ಜೆ[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ಪೂರ್ಣಾಂಕ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!m)
        ಹಿಂತಿರುಗಿ 0;
    ಆದರೆ( m->isContinuous() )
        ಹಿಂತಿರುಗಿ (ptr - ತುಂಡು_ಆರಂಭ)/elem_ಗಾತ್ರ;
    ptrdiff_t ofs = ptr - m->ptr();
    ಪೂರ್ಣಾಂಕ i, d = m->dims;
    ಆದರೆ( d == 2 )
    {
        ptrdiff_t y = ofs/m->ಹೆಜ್ಜೆ[0];
        ಹಿಂತಿರುಗಿ y*m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + (ofs - y*m->ಹೆಜ್ಜೆ[0])/elem_ಗಾತ್ರ;
    }
    ptrdiff_t ಫಲಿತಾಂಶ = 0;
    ಗಾಗಿ( i = 0; i < d; i++ )
    {
        size_t s = m->ಹೆಜ್ಜೆ[i], v = ofs/s;
        ofs -= v*s;
        ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ*m->size[i] + v;
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ MatConstIterator::seek(ptrdiff_t ofs, ತಾರ್ಕಿಕ relative)
{
    ಆದರೆ( m->isContinuous() )
    {
        ptr = (relative ? ptr : ತುಂಡು_ಆರಂಭ) + ofs*elem_ಗಾತ್ರ;
        ಆದರೆ( ptr < ತುಂಡು_ಆರಂಭ )
            ptr = ತುಂಡು_ಆರಂಭ;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ptr > ತುಂಡು_ಕೊನೆ )
            ptr = ತುಂಡು_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ d = m->dims;
    ಆದರೆ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ಆದರೆ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->ಹೆಜ್ಜೆ[0];
            ofs += y*m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + (ofs0 - y*m->ಹೆಜ್ಜೆ[0])/elem_ಗಾತ್ರ;
        }
        y = ofs/m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಪೂರ್ಣಾಂಕ y1 = std::min(std::max((ಪೂರ್ಣಾಂಕ)y, 0), m->ಅಡ್ಡಸಾಲುಗಳು-1);
        ತುಂಡು_ಆರಂಭ = m->ptr(y1);
        ತುಂಡು_ಕೊನೆ = ತುಂಡು_ಆರಂಭ + m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*elem_ಗಾತ್ರ;
        ptr = y < 0 ? ತುಂಡು_ಆರಂಭ : y >= m->ಅಡ್ಡಸಾಲುಗಳು ? ತುಂಡು_ಕೊನೆ :
            ತುಂಡು_ಆರಂಭ + (ofs - y*m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)*elem_ಗಾತ್ರ;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( relative )
        ofs += lpos();

    ಆದರೆ( ofs < 0 )
        ofs = 0;

    ಪೂರ್ಣಾಂಕ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_ಗಾತ್ರ;
    ತುಂಡು_ಆರಂಭ = m->ptr();

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ಪೂರ್ಣಾಂಕ)(ofs - t*szi);
        ofs = t;
        ತುಂಡು_ಆರಂಭ += v*m->ಹೆಜ್ಜೆ[i];
    }

    ತುಂಡು_ಕೊನೆ = ತುಂಡು_ಆರಂಭ + m->size[d-1]*elem_ಗಾತ್ರ;
    ಆದರೆ( ofs > 0 )
        ptr = ತುಂಡು_ಕೊನೆ;
    ಇಲ್ಲದಿದ್ದರೆ
        ptr = ತುಂಡು_ಆರಂಭ + (ptr - m->ptr());
}

ಶೂನ್ಯ MatConstIterator::seek(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _idx, ತಾರ್ಕಿಕ relative)
{
    ಪೂರ್ಣಾಂಕ d = m->dims;
    ptrdiff_t ofs = 0;
    ಆದರೆ( !_idx )
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ಸ್ಥಿರಾಂಕ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ಆದರೆ(!_m || !_m->hdr)
        ಹಿಂತಿರುಗಿ;
    SparseMat::Hdr& hdr = *m->hdr;
    ಸ್ಥಿರಾಂಕ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ಗಾಗಿ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ಆದರೆ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;
            ಹಿಂತಿರುಗಿ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::ಕ್ರಿಯಾಕಾರಕ ++()
{
    ಆದರೆ( !ptr || !m || !m->hdr )
        ಹಿಂತಿರುಗಿ *ಇದು;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ಸ್ಥಿರಾಂಕ SparseMat::Node*)(ptr - hdr.ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ))->next;
    ಆದರೆ( next )
    {
        ptr = &hdr.pool[next] + hdr.ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;
        ಹಿಂತಿರುಗಿ *ಇದು;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ಗಾಗಿ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ಆದರೆ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;
            ಹಿಂತಿರುಗಿ *ಇದು;
        }
    }
    hashidx = sz;
    ptr = 0;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

} // cv::
