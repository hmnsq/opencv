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

NAryMatIterator::NAryMatIterator(ಧ್ರುವ Mat** _arrays, Mat* _planes, ಪೂರ್ಣಾಙ್ಕ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ಧ್ರುವ Mat** _arrays, uchar** _ptrs, ಪೂರ್ಣಾಙ್ಕ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

ಶೂನ್ಯ NAryMatIterator::init(ಧ್ರುವ Mat** _arrays, Mat* _planes, uchar** _ptrs, ಪೂರ್ಣಾಙ್ಕ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ಪೂರ್ಣಾಙ್ಕ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    ಯದಿ( narrays < 0 )
    {
        ಪ್ರತಿ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    ಪ್ರತಿ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ಧ್ರುವ Mat& A = *arrays[i];
        ಯದಿ( ptrs )
            ptrs[i] = A.data;

        ಯದಿ( !A.data )
            ಅನುವರ್ತ;

        ಯದಿ( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            ಪ್ರತಿ( d1 = 0; d1 < d; d1++ )
                ಯದಿ( A.size[d1] > 1 )
                    ಭಙ್ಗ;
        }
        ಅನ್ಯಥಾ
            CV_Assert( A.size == arrays[i0]->size );

        ಯದಿ( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            ಪ್ರತಿ( j = d-1; j > d1; j-- )
                ಯದಿ( A.step[j]*A.size[j] < A.step[j-1] )
                    ಭಙ್ಗ;
            iterdepth = std::max(iterdepth, j);
        }
    }

    ಯದಿ( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        ಪ್ರತಿ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            ಯದಿ( total1 != (ಪೂರ್ಣಾಙ್ಕ)total1 )
                ಭಙ್ಗ;
            size = (ಪೂರ್ಣಾಙ್ಕ)total1;
        }

        iterdepth = j;
        ಯದಿ( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        ಪ್ರತಿ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ಅನ್ಯಥಾ
        iterdepth = 0;

    idx = 0;

    ಯದಿ( !planes )
        ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ಧ್ರುವ Mat& A = *arrays[i];

        ಯದಿ( !A.data )
        {
            planes[i] = Mat();
            ಅನುವರ್ತ;
        }

        planes[i] = Mat(1, (ಪೂರ್ಣಾಙ್ಕ)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    ಯದಿ( idx >= nplanes-1 )
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    ++idx;

    ಯದಿ( iterdepth == 1 )
    {
        ಯದಿ( ptrs )
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < narrays; i++ )
            {
                ಯದಿ( !ptrs[i] )
                    ಅನುವರ್ತ;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        ಯದಿ( planes )
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < narrays; i++ )
            {
                ಯದಿ( !planes[i].data )
                    ಅನುವರ್ತ;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < narrays; i++ )
        {
            ಧ್ರುವ Mat& A = *arrays[i];
            ಯದಿ( !A.data )
                ಅನುವರ್ತ;
            ಪೂರ್ಣಾಙ್ಕ _idx = (ಪೂರ್ಣಾಙ್ಕ)idx;
            uchar* data = A.data;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ಪೂರ್ಣಾಙ್ಕ szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            ಯದಿ( ptrs )
                ptrs[i] = data;
            ಯದಿ( planes )
                planes[i].data = data;
        }
    }

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

NAryMatIterator NAryMatIterator::operator ++(ಪೂರ್ಣಾಙ್ಕ)
{
    NAryMatIterator it = *ಏತತ್;
    ++*ಏತತ್;
    ಪ್ರತ್ಯಾಗಮ it;
}

//==================================================================================================

Point MatConstIterator::pos() ಧ್ರುವ
{
    ಯದಿ( !m )
        ಪ್ರತ್ಯಾಗಮ Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ಪೂರ್ಣಾಙ್ಕ y = (ಪೂರ್ಣಾಙ್ಕ)(ofs/m->step[0]);
    ಪ್ರತ್ಯಾಗಮ Point((ಪೂರ್ಣಾಙ್ಕ)((ofs - y*m->step[0])/elemSize), y);
}

ಶೂನ್ಯ MatConstIterator::pos(ಪೂರ್ಣಾಙ್ಕ* _idx) ಧ್ರುವ
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ಪೂರ್ಣಾಙ್ಕ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ಧ್ರುವ
{
    ಯದಿ(!m)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ( m->isContinuous() )
        ಪ್ರತ್ಯಾಗಮ (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ಪೂರ್ಣಾಙ್ಕ i, d = m->dims;
    ಯದಿ( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ಪ್ರತ್ಯಾಗಮ y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    ಪ್ರತಿ( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ MatConstIterator::seek(ptrdiff_t ofs, ತಾರ್ಕಿಕ relative)
{
    ಯದಿ( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        ಯದಿ( ptr < sliceStart )
            ptr = sliceStart;
        ಅನ್ಯಥಾ ಯದಿ( ptr > sliceEnd )
            ptr = sliceEnd;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ d = m->dims;
    ಯದಿ( d == 2 )
    {
        ptrdiff_t ofs0, y;
        ಯದಿ( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ಪೂರ್ಣಾಙ್ಕ y1 = std::min(std::max((ಪೂರ್ಣಾಙ್ಕ)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( relative )
        ofs += lpos();

    ಯದಿ( ofs < 0 )
        ofs = 0;

    ಪೂರ್ಣಾಙ್ಕ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ಪೂರ್ಣಾಙ್ಕ)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    ಯದಿ( ofs > 0 )
        ptr = sliceEnd;
    ಅನ್ಯಥಾ
        ptr = sliceStart + (ptr - m->ptr());
}

ಶೂನ್ಯ MatConstIterator::seek(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _idx, ತಾರ್ಕಿಕ relative)
{
    ಪೂರ್ಣಾಙ್ಕ d = m->dims;
    ptrdiff_t ofs = 0;
    ಯದಿ( !_idx )
        ;
    ಅನ್ಯಥಾ ಯದಿ( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ಧ್ರುವ SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    ಯದಿ(!_m || !_m->hdr)
        ಪ್ರತ್ಯಾಗಮ;
    SparseMat::Hdr& hdr = *m->hdr;
    ಧ್ರುವ std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    ಪ್ರತಿ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        ಯದಿ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ಮೌಲ್ಯ_Offset;
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    ಯದಿ( !ptr || !m || !m->hdr )
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ಧ್ರುವ SparseMat::Node*)(ptr - hdr.ಮೌಲ್ಯ_Offset))->next;
    ಯದಿ( next )
    {
        ptr = &hdr.pool[next] + hdr.ಮೌಲ್ಯ_Offset;
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    ಪ್ರತಿ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        ಯದಿ( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.ಮೌಲ್ಯ_Offset;
            ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
        }
    }
    hashidx = sz;
    ptr = 0;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

} // cv::
