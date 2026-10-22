// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "opencv2/core/mat.hpp"

namespace cv {

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(स्थिर Mat** _arrays, Mat* _planes, पूर्णांक _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(स्थिर Mat** _arrays, uchar** _ptrs, पूर्णांक _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

व्योम NAryMatIterator::init(स्थिर Mat** _arrays, Mat* _planes, uchar** _ptrs, पूर्णांक _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    पूर्णांक i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    अगर( narrays < 0 )
    {
        क्रम( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    क्रम( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        स्थिर Mat& A = *arrays[i];
        अगर( ptrs )
            ptrs[i] = A.तथ्य;

        अगर( !A.तथ्य )
            जारी;

        अगर( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            क्रम( d1 = 0; d1 < d; d1++ )
                अगर( A.size[d1] > 1 )
                    अवरोध;
        }
        अन्यथा
            CV_Assert( A.size == arrays[i0]->size );

        अगर( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elem_आकार() );
            क्रम( j = d-1; j > d1; j-- )
                अगर( A.step[j]*A.size[j] < A.step[j-1] )
                    अवरोध;
            iterdepth = std::max(iterdepth, j);
        }
    }

    अगर( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        क्रम( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            अगर( total1 != (पूर्णांक)total1 )
                अवरोध;
            size = (पूर्णांक)total1;
        }

        iterdepth = j;
        अगर( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        क्रम( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    अन्यथा
        iterdepth = 0;

    idx = 0;

    अगर( !planes )
        वापस;

    क्रम( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        स्थिर Mat& A = *arrays[i];

        अगर( !A.तथ्य )
        {
            planes[i] = Mat();
            जारी;
        }

        planes[i] = Mat(1, (पूर्णांक)size, A.type(), A.तथ्य);
    }
}


NAryMatIterator& NAryMatIterator::चालक ++()
{
    अगर( idx >= nplanes-1 )
        वापस *यह;
    ++idx;

    अगर( iterdepth == 1 )
    {
        अगर( ptrs )
        {
            क्रम( पूर्णांक i = 0; i < narrays; i++ )
            {
                अगर( !ptrs[i] )
                    जारी;
                ptrs[i] = arrays[i]->तथ्य + arrays[i]->step[0]*idx;
            }
        }
        अगर( planes )
        {
            क्रम( पूर्णांक i = 0; i < narrays; i++ )
            {
                अगर( !planes[i].तथ्य )
                    जारी;
                planes[i].तथ्य = arrays[i]->तथ्य + arrays[i]->step[0]*idx;
            }
        }
    }
    अन्यथा
    {
        क्रम( पूर्णांक i = 0; i < narrays; i++ )
        {
            स्थिर Mat& A = *arrays[i];
            अगर( !A.तथ्य )
                जारी;
            पूर्णांक _idx = (पूर्णांक)idx;
            uchar* तथ्य = A.तथ्य;
            क्रम( पूर्णांक j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                पूर्णांक szi = A.size[j], t = _idx/szi;
                तथ्य += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            अगर( ptrs )
                ptrs[i] = तथ्य;
            अगर( planes )
                planes[i].तथ्य = तथ्य;
        }
    }

    वापस *यह;
}

NAryMatIterator NAryMatIterator::चालक ++(पूर्णांक)
{
    NAryMatIterator it = *यह;
    ++*यह;
    वापस it;
}

//==================================================================================================

Point MatConstIterator::pos() स्थिर
{
    अगर( !m )
        वापस Point();
    CV_DbgAssert(m->dims <= 2);

    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    पूर्णांक y = (पूर्णांक)(ofs/m->step[0]);
    वापस Point((पूर्णांक)((ofs - y*m->step[0])/elem_आकार), y);
}

व्योम MatConstIterator::pos(पूर्णांक* _idx) स्थिर
{
    CV_Assert(m != 0 && _idx);
    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    क्रम( पूर्णांक i = 0; i < m->dims; i++ )
    {
        माप_प्रकार s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (पूर्णांक)v;
    }
}

सूचक_भेद_प्रकार MatConstIterator::lpos() स्थिर
{
    अगर(!m)
        वापस 0;
    अगर( m->isContinuous() )
        वापस (ptr - sliceStart)/elem_आकार;
    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    पूर्णांक i, d = m->dims;
    अगर( d == 2 )
    {
        सूचक_भेद_प्रकार y = ofs/m->step[0];
        वापस y*m->cols + (ofs - y*m->step[0])/elem_आकार;
    }
    सूचक_भेद_प्रकार परिणाम = 0;
    क्रम( i = 0; i < d; i++ )
    {
        माप_प्रकार s = m->step[i], v = ofs/s;
        ofs -= v*s;
        परिणाम = परिणाम*m->size[i] + v;
    }
    वापस परिणाम;
}

व्योम MatConstIterator::seek(सूचक_भेद_प्रकार ofs, bool relative)
{
    अगर( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elem_आकार;
        अगर( ptr < sliceStart )
            ptr = sliceStart;
        अन्यथा अगर( ptr > sliceEnd )
            ptr = sliceEnd;
        वापस;
    }

    पूर्णांक d = m->dims;
    अगर( d == 2 )
    {
        सूचक_भेद_प्रकार ofs0, y;
        अगर( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elem_आकार;
        }
        y = ofs/m->cols;
        पूर्णांक y1 = std::min(std::max((पूर्णांक)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elem_आकार;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elem_आकार;
        वापस;
    }

    अगर( relative )
        ofs += lpos();

    अगर( ofs < 0 )
        ofs = 0;

    पूर्णांक szi = m->size[d-1];
    सूचक_भेद_प्रकार t = ofs/szi;
    पूर्णांक v = (पूर्णांक)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elem_आकार;
    sliceStart = m->ptr();

    क्रम( पूर्णांक i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (पूर्णांक)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elem_आकार;
    अगर( ofs > 0 )
        ptr = sliceEnd;
    अन्यथा
        ptr = sliceStart + (ptr - m->ptr());
}

व्योम MatConstIterator::seek(स्थिर पूर्णांक* _idx, bool relative)
{
    पूर्णांक d = m->dims;
    सूचक_भेद_प्रकार ofs = 0;
    अगर( !_idx )
        ;
    अन्यथा अगर( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    अन्यथा
    {
        क्रम( पूर्णांक i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(स्थिर SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    अगर(!_m || !_m->hdr)
        वापस;
    SparseMat::Hdr& hdr = *m->hdr;
    स्थिर std::vector<माप_प्रकार>& htab = hdr.hashtab;
    माप_प्रकार i, hsize = htab.size();
    क्रम( i = 0; i < hsize; i++ )
    {
        माप_प्रकार nidx = htab[i];
        अगर( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मूल्य_Offset;
            वापस;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::चालक ++()
{
    अगर( !ptr || !m || !m->hdr )
        वापस *यह;
    SparseMat::Hdr& hdr = *m->hdr;
    माप_प्रकार next = ((स्थिर SparseMat::Node*)(ptr - hdr.मूल्य_Offset))->next;
    अगर( next )
    {
        ptr = &hdr.pool[next] + hdr.मूल्य_Offset;
        वापस *यह;
    }
    माप_प्रकार i = hashidx + 1, sz = hdr.hashtab.size();
    क्रम( ; i < sz; i++ )
    {
        माप_प्रकार nidx = hdr.hashtab[i];
        अगर( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मूल्य_Offset;
            वापस *यह;
        }
    }
    hashidx = sz;
    ptr = 0;
    वापस *यह;
}

} // cv::
