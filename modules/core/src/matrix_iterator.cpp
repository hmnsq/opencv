// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, ཧྲིལ _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, ཧྲིལ _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, ཧྲིལ _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    ཧྲིལ i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    གལ་སྲིད( narrays < 0 )
    {
        དོན་ལུ( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    དོན་ལུ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        གལ་སྲིད( ptrs )
            ptrs[i] = A.data;

        གལ་སྲིད( !A.data )
            continue;

        གལ་སྲིད( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            དོན་ལུ( d1 = 0; d1 < d; d1++ )
                གལ་སྲིད( A.size[d1] > 1 )
                    bha_nga;
        }
        དེ་མིན
            CV_Assert( A.size == arrays[i0]->size );

        གལ་སྲིད( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            དོན་ལུ( j = d-1; j > d1; j-- )
                གལ་སྲིད( A.step[j]*A.size[j] < A.step[j-1] )
                    bha_nga;
            iterdepth = std::max(iterdepth, j);
        }
    }

    གལ་སྲིད( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        དོན་ལུ( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            གལ་སྲིད( total1 != (ཧྲིལ)total1 )
                bha_nga;
            size = (ཧྲིལ)total1;
        }

        iterdepth = j;
        གལ་སྲིད( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        དོན་ལུ( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    དེ་མིན
        iterdepth = 0;

    idx = 0;

    གལ་སྲིད( !planes )
        ལོག;

    དོན་ལུ( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        གལ་སྲིད( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (ཧྲིལ)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    གལ་སྲིད( idx >= nplanes-1 )
        ལོག *this;
    ++idx;

    གལ་སྲིད( iterdepth == 1 )
    {
        གལ་སྲིད( ptrs )
        {
            དོན་ལུ( ཧྲིལ i = 0; i < narrays; i++ )
            {
                གལ་སྲིད( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        གལ་སྲིད( planes )
        {
            དོན་ལུ( ཧྲིལ i = 0; i < narrays; i++ )
            {
                གལ་སྲིད( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    དེ་མིན
    {
        དོན་ལུ( ཧྲིལ i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            གལ་སྲིད( !A.data )
                continue;
            ཧྲིལ _idx = (ཧྲིལ)idx;
            uchar* data = A.data;
            དོན་ལུ( ཧྲིལ j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                ཧྲིལ szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            གལ་སྲིད( ptrs )
                ptrs[i] = data;
            གལ་སྲིད( planes )
                planes[i].data = data;
        }
    }

    ལོག *this;
}

NAryMatIterator NAryMatIterator::operator ++(ཧྲིལ)
{
    NAryMatIterator it = *this;
    ++*this;
    ལོག it;
}

//==================================================================================================

Point MatConstIterator::pos() const
{
    གལ་སྲིད( !m )
        ལོག Point();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    ཧྲིལ y = (ཧྲིལ)(ofs/m->step[0]);
    ལོག Point((ཧྲིལ)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(ཧྲིལ* _idx) const
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    དོན་ལུ( ཧྲིལ i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (ཧྲིལ)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    གལ་སྲིད(!m)
        ལོག 0;
    གལ་སྲིད( m->isContinuous() )
        ལོག (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    ཧྲིལ i, d = m->dims;
    གལ་སྲིད( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        ལོག y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    དོན་ལུ( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    ལོག result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    གལ་སྲིད( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        གལ་སྲིད( ptr < sliceStart )
            ptr = sliceStart;
        དེ་མིན གལ་སྲིད( ptr > sliceEnd )
            ptr = sliceEnd;
        ལོག;
    }

    ཧྲིལ d = m->dims;
    གལ་སྲིད( d == 2 )
    {
        ptrdiff_t ofs0, y;
        གལ་སྲིད( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        ཧྲིལ y1 = std::min(std::max((ཧྲིལ)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        ལོག;
    }

    གལ་སྲིད( relative )
        ofs += lpos();

    གལ་སྲིད( ofs < 0 )
        ofs = 0;

    ཧྲིལ szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    ཧྲིལ v = (ཧྲིལ)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    དོན་ལུ( ཧྲིལ i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (ཧྲིལ)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    གལ་སྲིད( ofs > 0 )
        ptr = sliceEnd;
    དེ་མིན
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const ཧྲིལ* _idx, bool relative)
{
    ཧྲིལ d = m->dims;
    ptrdiff_t ofs = 0;
    གལ་སྲིད( !_idx )
        ;
    དེ་མིན གལ་སྲིད( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    དེ་མིན
    {
        དོན་ལུ( ཧྲིལ i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    གལ་སྲིད(!_m || !_m->hdr)
        ལོག;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    དོན་ལུ( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        གལ་སྲིད( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ལོག;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    གལ་སྲིད( !ptr || !m || !m->hdr )
        ལོག *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    གལ་སྲིད( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        ལོག *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    དོན་ལུ( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        གལ་སྲིད( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            ལོག *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    ལོག *this;
}

} // cv::
