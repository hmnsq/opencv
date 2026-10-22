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

NAryMatIterator::NAryMatIterator(स्थिरांक Mat** _arrays, Mat* _planes, पूर्णांक _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(स्थिरांक Mat** _arrays, uchar** _ptrs, पूर्णांक _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

शून्य NAryMatIterator::init(स्थिरांक Mat** _arrays, Mat* _planes, uchar** _ptrs, पूर्णांक _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    पूर्णांक i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    जर( narrays < 0 )
    {
        साठी( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    साठी( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        स्थिरांक Mat& A = *arrays[i];
        जर( ptrs )
            ptrs[i] = A.data;

        जर( !A.data )
            सुरूठेवा;

        जर( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            साठी( d1 = 0; d1 < d; d1++ )
                जर( A.size[d1] > 1 )
                    थांबा;
        }
        अन्यथा
            CV_Assert( A.size == arrays[i0]->size );

        जर( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            साठी( j = d-1; j > d1; j-- )
                जर( A.step[j]*A.size[j] < A.step[j-1] )
                    थांबा;
            iterdepth = std::max(iterdepth, j);
        }
    }

    जर( i0 >= 0 )
    {
        size = arrays[i0]->size[std::max(d-1, 0)];
        साठी( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            जर( total1 != (पूर्णांक)total1 )
                थांबा;
            size = (पूर्णांक)total1;
        }

        iterdepth = j;
        जर( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        साठी( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    अन्यथा
        iterdepth = 0;

    idx = 0;

    जर( !planes )
        परत;

    साठी( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        स्थिरांक Mat& A = *arrays[i];

        जर( !A.data )
        {
            planes[i] = Mat();
            सुरूठेवा;
        }

        planes[i] = Mat(1, (पूर्णांक)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::चालक ++()
{
    जर( idx >= nplanes-1 )
        परत *हे;
    ++idx;

    जर( iterdepth == 1 )
    {
        जर( ptrs )
        {
            साठी( पूर्णांक i = 0; i < narrays; i++ )
            {
                जर( !ptrs[i] )
                    सुरूठेवा;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        जर( planes )
        {
            साठी( पूर्णांक i = 0; i < narrays; i++ )
            {
                जर( !planes[i].data )
                    सुरूठेवा;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < narrays; i++ )
        {
            स्थिरांक Mat& A = *arrays[i];
            जर( !A.data )
                सुरूठेवा;
            पूर्णांक _idx = (पूर्णांक)idx;
            uchar* data = A.data;
            साठी( पूर्णांक j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                पूर्णांक szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            जर( ptrs )
                ptrs[i] = data;
            जर( planes )
                planes[i].data = data;
        }
    }

    परत *हे;
}

NAryMatIterator NAryMatIterator::चालक ++(पूर्णांक)
{
    NAryMatIterator it = *हे;
    ++*हे;
    परत it;
}

//==================================================================================================

Point MatConstIterator::pos() स्थिरांक
{
    जर( !m )
        परत Point();
    CV_DbgAssert(m->dims <= 2);

    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    पूर्णांक y = (पूर्णांक)(ofs/m->step[0]);
    परत Point((पूर्णांक)((ofs - y*m->step[0])/elemSize), y);
}

शून्य MatConstIterator::pos(पूर्णांक* _idx) स्थिरांक
{
    CV_Assert(m != 0 && _idx);
    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    साठी( पूर्णांक i = 0; i < m->dims; i++ )
    {
        माप_प्रकार s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (पूर्णांक)v;
    }
}

सूचक_भेद_प्रकार MatConstIterator::lpos() स्थिरांक
{
    जर(!m)
        परत 0;
    जर( m->isContinuous() )
        परत (ptr - sliceStart)/elemSize;
    सूचक_भेद_प्रकार ofs = ptr - m->ptr();
    पूर्णांक i, d = m->dims;
    जर( d == 2 )
    {
        सूचक_भेद_प्रकार y = ofs/m->step[0];
        परत y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    सूचक_भेद_प्रकार result = 0;
    साठी( i = 0; i < d; i++ )
    {
        माप_प्रकार s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    परत result;
}

शून्य MatConstIterator::seek(सूचक_भेद_प्रकार ofs, तार्किक relative)
{
    जर( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        जर( ptr < sliceStart )
            ptr = sliceStart;
        अन्यथा जर( ptr > sliceEnd )
            ptr = sliceEnd;
        परत;
    }

    पूर्णांक d = m->dims;
    जर( d == 2 )
    {
        सूचक_भेद_प्रकार ofs0, y;
        जर( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        पूर्णांक y1 = std::min(std::max((पूर्णांक)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        परत;
    }

    जर( relative )
        ofs += lpos();

    जर( ofs < 0 )
        ofs = 0;

    पूर्णांक szi = m->size[d-1];
    सूचक_भेद_प्रकार t = ofs/szi;
    पूर्णांक v = (पूर्णांक)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    साठी( पूर्णांक i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (पूर्णांक)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    जर( ofs > 0 )
        ptr = sliceEnd;
    अन्यथा
        ptr = sliceStart + (ptr - m->ptr());
}

शून्य MatConstIterator::seek(स्थिरांक पूर्णांक* _idx, तार्किक relative)
{
    पूर्णांक d = m->dims;
    सूचक_भेद_प्रकार ofs = 0;
    जर( !_idx )
        ;
    अन्यथा जर( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//==================================================================================================

SparseMatConstIterator::SparseMatConstIterator(स्थिरांक SparseMat* _m) : m((SparseMat*)_m), hashidx(0), ptr(0)
{
    जर(!_m || !_m->hdr)
        परत;
    SparseMat::Hdr& hdr = *m->hdr;
    स्थिरांक std::vector<माप_प्रकार>& htab = hdr.hashtab;
    माप_प्रकार i, hsize = htab.size();
    साठी( i = 0; i < hsize; i++ )
    {
        माप_प्रकार nidx = htab[i];
        जर( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मूल्य_Offset;
            परत;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::चालक ++()
{
    जर( !ptr || !m || !m->hdr )
        परत *हे;
    SparseMat::Hdr& hdr = *m->hdr;
    माप_प्रकार next = ((स्थिरांक SparseMat::Node*)(ptr - hdr.मूल्य_Offset))->next;
    जर( next )
    {
        ptr = &hdr.pool[next] + hdr.मूल्य_Offset;
        परत *हे;
    }
    माप_प्रकार i = hashidx + 1, sz = hdr.hashtab.size();
    साठी( ; i < sz; i++ )
    {
        माप_प्रकार nidx = hdr.hashtab[i];
        जर( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.मूल्य_Offset;
            परत *हे;
        }
    }
    hashidx = sz;
    ptr = 0;
    परत *हे;
}

} // cv::
