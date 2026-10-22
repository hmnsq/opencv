// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, Mat* _planes, صحيح _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, uchar** _ptrs, صحيح _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

فراغ NAryMatIterator::init(ثابت Mat** _arrays, Mat* _planes, uchar** _ptrs, صحيح _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    صحيح i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    إذا( narrays < 0 )
    {
        لكل( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    لكل( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];
        إذا( ptrs )
            ptrs[i] = A.معلومات;

        إذا( !A.معلومات )
            استمر;

        إذا( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            لكل( d1 = 0; d1 < d; d1++ )
                إذا( A.size[d1] > 1 )
                    اكسر;
        }
        وإلا
            CV_Assert( A.size == arrays[i0]->size );

        إذا( !A.isContinuous() )
        {
            CV_Assert( A.قدم[d-1] == A.elem_سائيز() );
            لكل( j = d-1; j > d1; j-- )
                إذا( A.قدم[j]*A.size[j] < A.قدم[j-1] )
                    اكسر;
            iterdepth = std::max(iterdepth, j);
        }
    }

    إذا( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        لكل( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            إذا( total1 != (صحيح)total1 )
                اكسر;
            size = (صحيح)total1;
        }

        iterdepth = j;
        إذا( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        لكل( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    وإلا
        iterdepth = 0;

    idx = 0;

    إذا( !planes )
        أرجع;

    لكل( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];

        إذا( !A.معلومات )
        {
            planes[i] = Mat();
            استمر;
        }

        planes[i] = Mat(1, (صحيح)size, A.type(), A.معلومات);
    }
}


NAryMatIterator& NAryMatIterator::عامل ++()
{
    إذا( idx >= nplanes-1 )
        أرجع *هذا;
    ++idx;

    إذا( iterdepth == 1 )
    {
        إذا( ptrs )
        {
            لكل( صحيح i = 0; i < narrays; i++ )
            {
                إذا( !ptrs[i] )
                    استمر;
                ptrs[i] = arrays[i]->معلومات + arrays[i]->قدم[0]*idx;
            }
        }
        إذا( planes )
        {
            لكل( صحيح i = 0; i < narrays; i++ )
            {
                إذا( !planes[i].معلومات )
                    استمر;
                planes[i].معلومات = arrays[i]->معلومات + arrays[i]->قدم[0]*idx;
            }
        }
    }
    وإلا
    {
        لكل( صحيح i = 0; i < narrays; i++ )
        {
            ثابت Mat& A = *arrays[i];
            إذا( !A.معلومات )
                استمر;
            صحيح _idx = (صحيح)idx;
            uchar* معلومات = A.معلومات;
            لكل( صحيح j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                صحيح szi = A.size[j], t = _idx/szi;
                معلومات += (_idx - t * szi)*A.قدم[j];
                _idx = t;
            }
            إذا( ptrs )
                ptrs[i] = معلومات;
            إذا( planes )
                planes[i].معلومات = معلومات;
        }
    }

    أرجع *هذا;
}

NAryMatIterator NAryMatIterator::عامل ++(صحيح)
{
    NAryMatIterator it = *هذا;
    ++*هذا;
    أرجع it;
}

//==================================================================================================

نقطو MatConstIterator::pos() ثابت
{
    إذا( !m )
        أرجع نقطو();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    صحيح y = (صحيح)(ofs/m->قدم[0]);
    أرجع نقطو((صحيح)((ofs - y*m->قدم[0])/elem_سائيز), y);
}

فراغ MatConstIterator::pos(صحيح* _idx) ثابت
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    لكل( صحيح i = 0; i < m->dims; i++ )
    {
        size_t s = m->قدم[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (صحيح)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ثابت
{
    إذا(!m)
        أرجع 0;
    إذا( m->isContinuous() )
        أرجع (ptr - ٽڪرو_شروعات)/elem_سائيز;
    ptrdiff_t ofs = ptr - m->ptr();
    صحيح i, d = m->dims;
    إذا( d == 2 )
    {
        ptrdiff_t y = ofs/m->قدم[0];
        أرجع y*m->کانن_ڳڻپ + (ofs - y*m->قدم[0])/elem_سائيز;
    }
    ptrdiff_t نتيجو = 0;
    لكل( i = 0; i < d; i++ )
    {
        size_t s = m->قدم[i], v = ofs/s;
        ofs -= v*s;
        نتيجو = نتيجو*m->size[i] + v;
    }
    أرجع نتيجو;
}

فراغ MatConstIterator::seek(ptrdiff_t ofs, منطقي relative)
{
    إذا( m->isContinuous() )
    {
        ptr = (relative ? ptr : ٽڪرو_شروعات) + ofs*elem_سائيز;
        إذا( ptr < ٽڪرو_شروعات )
            ptr = ٽڪرو_شروعات;
        وإلا إذا( ptr > ٽڪرو_پڇاڙي )
            ptr = ٽڪرو_پڇاڙي;
        أرجع;
    }

    صحيح d = m->dims;
    إذا( d == 2 )
    {
        ptrdiff_t ofs0, y;
        إذا( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->قدم[0];
            ofs += y*m->کانن_ڳڻپ + (ofs0 - y*m->قدم[0])/elem_سائيز;
        }
        y = ofs/m->کانن_ڳڻپ;
        صحيح y1 = std::min(std::max((صحيح)y, 0), m->صفون-1);
        ٽڪرو_شروعات = m->ptr(y1);
        ٽڪرو_پڇاڙي = ٽڪرو_شروعات + m->کانن_ڳڻپ*elem_سائيز;
        ptr = y < 0 ? ٽڪرو_شروعات : y >= m->صفون ? ٽڪرو_پڇاڙي :
            ٽڪرو_شروعات + (ofs - y*m->کانن_ڳڻپ)*elem_سائيز;
        أرجع;
    }

    إذا( relative )
        ofs += lpos();

    إذا( ofs < 0 )
        ofs = 0;

    صحيح szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    صحيح v = (صحيح)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_سائيز;
    ٽڪرو_شروعات = m->ptr();

    لكل( صحيح i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (صحيح)(ofs - t*szi);
        ofs = t;
        ٽڪرو_شروعات += v*m->قدم[i];
    }

    ٽڪرو_پڇاڙي = ٽڪرو_شروعات + m->size[d-1]*elem_سائيز;
    إذا( ofs > 0 )
        ptr = ٽڪرو_پڇاڙي;
    وإلا
        ptr = ٽڪرو_شروعات + (ptr - m->ptr());
}

فراغ MatConstIterator::seek(ثابت صحيح* _idx, منطقي relative)
{
    صحيح d = m->dims;
    ptrdiff_t ofs = 0;
    إذا( !_idx )
        ;
    وإلا إذا( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    وإلا
    {
        لكل( صحيح i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ثابت SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    إذا(!_m || !_m->hdr)
        أرجع;
    SparseMat::Hdr& hdr = *m->hdr;
    ثابت std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    لكل( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        إذا( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.قدر_وٿي;
            أرجع;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::عامل ++()
{
    إذا( !ptr || !m || !m->hdr )
        أرجع *هذا;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ثابت SparseMat::Node*)(ptr - hdr.قدر_وٿي))->next;
    إذا( next )
    {
        ptr = &hdr.pool[next] + hdr.قدر_وٿي;
        أرجع *هذا;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    لكل( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        إذا( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.قدر_وٿي;
            أرجع *هذا;
        }
    }
    hashidx = sz;
    ptr = 0;
    أرجع *هذا;
}

} // cv::
