// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

نامخلا cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, Mat* _planes, صحیح _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(ثابت Mat** _arrays, uchar** _ptrs, صحیح _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

خالی NAryMatIterator::init(ثابت Mat** _arrays, Mat* _planes, uchar** _ptrs, صحیح _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    صحیح i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    اگر( narrays < 0 )
    {
        برائے( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    برائے( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];
        اگر( ptrs )
            ptrs[i] = A.ڈیٹا;

        اگر( !A.ڈیٹا )
            جاری;

        اگر( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            برائے( d1 = 0; d1 < d; d1++ )
                اگر( A.size[d1] > 1 )
                    توڑو;
        }
        ورنہ
            CV_Assert( A.size == arrays[i0]->size );

        اگر( !A.isContinuous() )
        {
            CV_Assert( A.قدم[d-1] == A.elem_سائز() );
            برائے( j = d-1; j > d1; j-- )
                اگر( A.قدم[j]*A.size[j] < A.قدم[j-1] )
                    توڑو;
            iterdepth = std::max(iterdepth, j);
        }
    }

    اگر( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        برائے( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            اگر( total1 != (صحیح)total1 )
                توڑو;
            size = (صحیح)total1;
        }

        iterdepth = j;
        اگر( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        برائے( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    ورنہ
        iterdepth = 0;

    idx = 0;

    اگر( !planes )
        واپس;

    برائے( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        ثابت Mat& A = *arrays[i];

        اگر( !A.ڈیٹا )
        {
            planes[i] = Mat();
            جاری;
        }

        planes[i] = Mat(1, (صحیح)size, A.type(), A.ڈیٹا);
    }
}


NAryMatIterator& NAryMatIterator::عامل ++()
{
    اگر( idx >= nplanes-1 )
        واپس *یہ;
    ++idx;

    اگر( iterdepth == 1 )
    {
        اگر( ptrs )
        {
            برائے( صحیح i = 0; i < narrays; i++ )
            {
                اگر( !ptrs[i] )
                    جاری;
                ptrs[i] = arrays[i]->ڈیٹا + arrays[i]->قدم[0]*idx;
            }
        }
        اگر( planes )
        {
            برائے( صحیح i = 0; i < narrays; i++ )
            {
                اگر( !planes[i].ڈیٹا )
                    جاری;
                planes[i].ڈیٹا = arrays[i]->ڈیٹا + arrays[i]->قدم[0]*idx;
            }
        }
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < narrays; i++ )
        {
            ثابت Mat& A = *arrays[i];
            اگر( !A.ڈیٹا )
                جاری;
            صحیح _idx = (صحیح)idx;
            uchar* ڈیٹا = A.ڈیٹا;
            برائے( صحیح j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                صحیح szi = A.size[j], t = _idx/szi;
                ڈیٹا += (_idx - t * szi)*A.قدم[j];
                _idx = t;
            }
            اگر( ptrs )
                ptrs[i] = ڈیٹا;
            اگر( planes )
                planes[i].ڈیٹا = ڈیٹا;
        }
    }

    واپس *یہ;
}

NAryMatIterator NAryMatIterator::عامل ++(صحیح)
{
    NAryMatIterator it = *یہ;
    ++*یہ;
    واپس it;
}

//==================================================================================================

نقطہ MatConstIterator::pos() ثابت
{
    اگر( !m )
        واپس نقطہ();
    CV_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    صحیح y = (صحیح)(ofs/m->قدم[0]);
    واپس نقطہ((صحیح)((ofs - y*m->قدم[0])/elem_سائز), y);
}

خالی MatConstIterator::pos(صحیح* _idx) ثابت
{
    CV_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    برائے( صحیح i = 0; i < m->dims; i++ )
    {
        size_t s = m->قدم[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (صحیح)v;
    }
}

ptrdiff_t MatConstIterator::lpos() ثابت
{
    اگر(!m)
        واپس 0;
    اگر( m->isContinuous() )
        واپس (ptr - قاش_آغاز)/elem_سائز;
    ptrdiff_t ofs = ptr - m->ptr();
    صحیح i, d = m->dims;
    اگر( d == 2 )
    {
        ptrdiff_t y = ofs/m->قدم[0];
        واپس y*m->ستون_تعداد + (ofs - y*m->قدم[0])/elem_سائز;
    }
    ptrdiff_t نتیجہ = 0;
    برائے( i = 0; i < d; i++ )
    {
        size_t s = m->قدم[i], v = ofs/s;
        ofs -= v*s;
        نتیجہ = نتیجہ*m->size[i] + v;
    }
    واپس نتیجہ;
}

خالی MatConstIterator::seek(ptrdiff_t ofs, منطقی relative)
{
    اگر( m->isContinuous() )
    {
        ptr = (relative ? ptr : قاش_آغاز) + ofs*elem_سائز;
        اگر( ptr < قاش_آغاز )
            ptr = قاش_آغاز;
        ورنہ اگر( ptr > قاش_انتہا )
            ptr = قاش_انتہا;
        واپس;
    }

    صحیح d = m->dims;
    اگر( d == 2 )
    {
        ptrdiff_t ofs0, y;
        اگر( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->قدم[0];
            ofs += y*m->ستون_تعداد + (ofs0 - y*m->قدم[0])/elem_سائز;
        }
        y = ofs/m->ستون_تعداد;
        صحیح y1 = std::min(std::max((صحیح)y, 0), m->صفیں-1);
        قاش_آغاز = m->ptr(y1);
        قاش_انتہا = قاش_آغاز + m->ستون_تعداد*elem_سائز;
        ptr = y < 0 ? قاش_آغاز : y >= m->صفیں ? قاش_انتہا :
            قاش_آغاز + (ofs - y*m->ستون_تعداد)*elem_سائز;
        واپس;
    }

    اگر( relative )
        ofs += lpos();

    اگر( ofs < 0 )
        ofs = 0;

    صحیح szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    صحیح v = (صحیح)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_سائز;
    قاش_آغاز = m->ptr();

    برائے( صحیح i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (صحیح)(ofs - t*szi);
        ofs = t;
        قاش_آغاز += v*m->قدم[i];
    }

    قاش_انتہا = قاش_آغاز + m->size[d-1]*elem_سائز;
    اگر( ofs > 0 )
        ptr = قاش_انتہا;
    ورنہ
        ptr = قاش_آغاز + (ptr - m->ptr());
}

خالی MatConstIterator::seek(ثابت صحیح* _idx, منطقی relative)
{
    صحیح d = m->dims;
    ptrdiff_t ofs = 0;
    اگر( !_idx )
        ;
    ورنہ اگر( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    ورنہ
    {
        برائے( صحیح i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(ثابت SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    اگر(!_m || !_m->hdr)
        واپس;
    SparseMat::Hdr& hdr = *m->hdr;
    ثابت std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    برائے( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        اگر( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.قدر_انحراف;
            واپس;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::عامل ++()
{
    اگر( !ptr || !m || !m->hdr )
        واپس *یہ;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((ثابت SparseMat::Node*)(ptr - hdr.قدر_انحراف))->next;
    اگر( next )
    {
        ptr = &hdr.pool[next] + hdr.قدر_انحراف;
        واپس *یہ;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    برائے( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        اگر( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.قدر_انحراف;
            واپس *یہ;
        }
    }
    hashidx = sz;
    ptr = 0;
    واپس *یہ;
}

} // cv::
