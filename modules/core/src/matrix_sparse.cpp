// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

template<typename T1, typename T2> शून्य
convertData_(ध्रुव शून्य* _from, शून्य* _to, पूर्णाङ्क cn)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* to = (T2*)_to;
    सचे( cn == 1 )
        *to = saturate_cast<T2>(*from);
    अञ्ञथा
        अत्थाय( पूर्णाङ्क i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> शून्य
convertScaleData_(ध्रुव शून्य* _from, शून्य* _to, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* to = (T2*)_to;
    सचे( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    अञ्ञथा
        अत्थाय( पूर्णाङ्क i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

प्रकारनिर्देश शून्य (*ConvertData)(ध्रुव शून्य* from, शून्य* to, पूर्णाङ्क cn);
प्रकारनिर्देश शून्य (*ConvertScaleData)(ध्रुव शून्य* from, शून्य* to, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta);

स्थिर ConvertData getConvertElem(पूर्णाङ्क fromType, पूर्णाङ्क toType)
{
    स्थिर ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, पूर्णाङ्क>, convertData_<uchar, भिन्न>,
      convertData_<uchar, द्विगुण>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, पूर्णाङ्क>, convertData_<schar, भिन्न>,
      convertData_<schar, द्विगुण>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, पूर्णाङ्क>, convertData_<ushort, भिन्न>,
      convertData_<ushort, द्विगुण>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, पूर्णाङ्क>, convertData_<short, भिन्न>,
      convertData_<short, द्विगुण>, 0 },

    { convertData_<पूर्णाङ्क, uchar>, convertData_<पूर्णाङ्क, schar>,
      convertData_<पूर्णाङ्क, ushort>, convertData_<पूर्णाङ्क, short>,
      convertData_<पूर्णाङ्क, पूर्णाङ्क>, convertData_<पूर्णाङ्क, भिन्न>,
      convertData_<पूर्णाङ्क, द्विगुण>, 0 },

    { convertData_<भिन्न, uchar>, convertData_<भिन्न, schar>,
      convertData_<भिन्न, ushort>, convertData_<भिन्न, short>,
      convertData_<भिन्न, पूर्णाङ्क>, convertData_<भिन्न, भिन्न>,
      convertData_<भिन्न, द्विगुण>, 0 },

    { convertData_<द्विगुण, uchar>, convertData_<द्विगुण, schar>,
      convertData_<द्विगुण, ushort>, convertData_<द्विगुण, short>,
      convertData_<द्विगुण, पूर्णाङ्क>, convertData_<द्विगुण, भिन्न>,
      convertData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    पटिनिवत्त func;
}

स्थिर ConvertScaleData getConvertScaleElem(पूर्णाङ्क fromType, पूर्णाङ्क toType)
{
    स्थिर ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, पूर्णाङ्क>, convertScaleData_<uchar, भिन्न>,
      convertScaleData_<uchar, द्विगुण>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, पूर्णाङ्क>, convertScaleData_<schar, भिन्न>,
      convertScaleData_<schar, द्विगुण>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, पूर्णाङ्क>, convertScaleData_<ushort, भिन्न>,
      convertScaleData_<ushort, द्विगुण>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, पूर्णाङ्क>, convertScaleData_<short, भिन्न>,
      convertScaleData_<short, द्विगुण>, 0 },

    { convertScaleData_<पूर्णाङ्क, uchar>, convertScaleData_<पूर्णाङ्क, schar>,
      convertScaleData_<पूर्णाङ्क, ushort>, convertScaleData_<पूर्णाङ्क, short>,
      convertScaleData_<पूर्णाङ्क, पूर्णाङ्क>, convertScaleData_<पूर्णाङ्क, भिन्न>,
      convertScaleData_<पूर्णाङ्क, द्विगुण>, 0 },

    { convertScaleData_<भिन्न, uchar>, convertScaleData_<भिन्न, schar>,
      convertScaleData_<भिन्न, ushort>, convertScaleData_<भिन्न, short>,
      convertScaleData_<भिन्न, पूर्णाङ्क>, convertScaleData_<भिन्न, भिन्न>,
      convertScaleData_<भिन्न, द्विगुण>, 0 },

    { convertScaleData_<द्विगुण, uchar>, convertScaleData_<द्विगुण, schar>,
      convertScaleData_<द्विगुण, ushort>, convertScaleData_<द्विगुण, short>,
      convertScaleData_<द्विगुण, पूर्णाङ्क>, convertScaleData_<द्विगुण, भिन्न>,
      convertScaleData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    पटिनिवत्त func;
}

गणना { HASH_SIZE0 = 8 };

स्थिर अन्तःस्थ शून्य copyElem(ध्रुव uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    अत्थाय( i = 0; i + परिमाण(पूर्णाङ्क) <= elemSize; i += परिमाण(पूर्णाङ्क) )
        *(पूर्णाङ्क*)(to + i) = *(ध्रुव पूर्णाङ्क*)(from + i);
    अत्थाय( ; i < elemSize; i++ )
        to[i] = from[i];
}

स्थिर अन्तःस्थ तार्किक isZeroElem(ध्रुव uchar* data, size_t elemSize)
{
    size_t i;
    अत्थाय( i = 0; i + परिमाण(पूर्णाङ्क) <= elemSize; i += परिमाण(पूर्णाङ्क) )
        सचे( *(पूर्णाङ्क*)(data + i) != 0 )
            पटिनिवत्त मिच्छा;
    अत्थाय( ; i < elemSize; i++ )
        सचे( data[i] != 0 )
            पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
}

SparseMat::Hdr::Hdr( पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type )
{
    refcount = 1;

    dims = _dims;
    मानम्_Offset = (पूर्णाङ्क)alignSize(परिमाण(SparseMat::Node) - MAX_DIM*परिमाण(पूर्णाङ्क) +
                                 dims*परिमाण(पूर्णाङ्क), CV_ELEM_SIZE1(_type));
    ग्रन्थिः_Size = alignSize(मानम्_Offset +
        CV_ELEM_SIZE(_type), (पूर्णाङ्क)परिमाण(size_t));

    पूर्णाङ्क i;
    अत्थाय( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    अत्थाय( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

शून्य SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#सचे defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(ग्रन्थिः_Size);
#सचे defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    ग्रन्थिः_गणनम् = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ध्रुव SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ध्रुव SparseMat& m)
{
    सचे( एतत् != &m )
    {
        सचे( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    पटिनिवत्त *एतत्;
}

SparseMat& SparseMat::operator=(ध्रुव Mat& m)
{
    पटिनिवत्त (*एतत् = SparseMat(m));
}

शून्य SparseMat::assignTo(SparseMat& m, पूर्णाङ्क _type) ध्रुव
{
    सचे( _type < 0 )
        m = *एतत्;
    अञ्ञथा
        convertTo(m, _type);
}

शून्य SparseMat::addref()
{
    सचे( hdr )
        CV_XADD(&hdr->refcount, 1);
}

शून्य SparseMat::release()
{
    सचे( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(पूर्णाङ्क i0) ध्रुव
{
    पटिनिवत्त (size_t)i0;
}

size_t SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1) ध्रुव
{
    पटिनिवत्त (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2) ध्रुव
{
    पटिनिवत्त ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ध्रुव पूर्णाङ्क* idx) ध्रुव
{
    size_t h = (unsigned)idx[0];
    सचे( !hdr )
        पटिनिवत्त 0;
    पूर्णाङ्क d = hdr->dims;
    अत्थाय(पूर्णाङ्क i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    पटिनिवत्त h;
}


SparseMat::SparseMat(ध्रुव Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    पूर्णाङ्क i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ध्रुव uchar* dptr = m.ptr();

    अत्थाय(;;)
    {
        अत्थाय( i = 0; i < lastSize; i++, dptr += esz )
        {
            सचे( isZeroElem(dptr, esz) )
                अनुवर्त;
            idx[d-1] = i;
            uchar* to = new_ग्रन्थिः(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        अत्थाय( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            सचे( ++idx[i] < m.size[i] )
                भङ्ग;
            idx[i] = 0;
        }
        सचे( i < 0 )
            भङ्ग;
    }
}

शून्य SparseMat::create(पूर्णाङ्क d, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    अत्थाय( पूर्णाङ्क i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    सचे( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        पूर्णाङ्क i;
        अत्थाय( i = 0; i < d; i++ )
            सचे( _sizes[i] != hdr->size[i] )
                भङ्ग;
        सचे( i == d )
        {
            clear();
            पटिनिवत्त;
        }
    }
    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    सचे (hdr && _sizes == hdr->size)
    {
        अत्थाय(पूर्णाङ्क i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = नूतन Hdr(d, _sizes, _type);
}

शून्य SparseMat::copyTo( SparseMat& m ) ध्रुव
{
    सचे( hdr == m.hdr )
        पटिनिवत्त;
    सचे( !hdr )
    {
        m.release();
        पटिनिवत्त;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    अत्थाय( size_t i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.ग्रन्थिः();
        uchar* to = m.new_ग्रन्थिः(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

शून्य SparseMat::copyTo( Mat& m ) ध्रुव
{
    CV_Assert( hdr );
    पूर्णाङ्क ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    अत्थाय( size_t i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.ग्रन्थिः();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


शून्य SparseMat::convertTo( SparseMat& m, पूर्णाङ्क rtype, द्विगुण alpha ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    सचे( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    सचे( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        पटिनिवत्त;
    }

    CV_Assert(hdr != 0);
    सचे( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    सचे( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        अत्थाय( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ग्रन्थिः(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अञ्ञथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        अत्थाय( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ग्रन्थिः(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


शून्य SparseMat::convertTo( Mat& m, पूर्णाङ्क rtype, द्विगुण alpha, द्विगुण beta ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    सचे( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    सचे( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        अत्थाय( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अञ्ञथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        अत्थाय( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

शून्य SparseMat::clear()
{
    सचे( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h && elem->idx[0] == i0 )
            पटिनिवत्त &value<uchar>(elem);
        nidx = elem->next;
    }

    सचे( createMissing )
    {
        पूर्णाङ्क idx[] = { i0 };
        पटिनिवत्त new_ग्रन्थिः( idx, h );
    }
    पटिनिवत्त रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            पटिनिवत्त &value<uchar>(elem);
        nidx = elem->next;
    }

    सचे( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1 };
        पटिनिवत्त new_ग्रन्थिः( idx, h );
    }
    पटिनिवत्त रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            पटिनिवत्त &value<uchar>(elem);
        nidx = elem->next;
    }

    सचे( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1, i2 };
        पटिनिवत्त new_ग्रन्थिः( idx, h );
    }
    पटिनिवत्त रिक्त;
}

uchar* SparseMat::ptr(ध्रुव पूर्णाङ्क* idx, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h )
        {
            अत्थाय( i = 0; i < d; i++ )
                सचे( elem->idx[i] != idx[i] )
                    भङ्ग;
            सचे( i == d )
                पटिनिवत्त &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    पटिनिवत्त createMissing ? new_ग्रन्थिः(idx, h) : रिक्त;
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    सचे( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    सचे( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::erase(ध्रुव पूर्णाङ्क* idx, size_t* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    याव( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        सचे( elem->hashval == h )
        {
            अत्थाय( i = 0; i < d; i++ )
                सचे( elem->idx[i] != idx[i] )
                    भङ्ग;
            सचे( i == d )
                भङ्ग;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    सचे( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    सचे((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((द्विगुण)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    अत्थाय( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    अत्थाय( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        याव( nidx )
        {
            Node* elem = (Node*)(pool + nidx);
            size_t next = elem->next;
            size_t newhidx = elem->hashval & (newsize - 1);
            elem->next = newh[newhidx];
            newh[newhidx] = nidx;
            nidx = next;
        }
    }
    hdr->hashtab = _newh;
}

uchar* SparseMat::new_ग्रन्थिः(ध्रुव पूर्णाङ्क* idx, size_t hashval)
{
    ध्रुव पूर्णाङ्क HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    सचे( ++hdr->ग्रन्थिः_गणनम् > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    सचे( !hdr->freeList )
    {
        size_t i, nsz = hdr->ग्रन्थिः_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        अत्थाय( i = hdr->freeList; i < newpsize - nsz; i += nsz )
            ((Node*)(pool + i))->next = i + nsz;
        ((Node*)(pool + i))->next = 0;
    }
    size_t nidx = hdr->freeList;
    Node* elem = (Node*)&hdr->pool[nidx];
    hdr->freeList = elem->next;
    elem->hashval = hashval;
    size_t hidx = hashval & (hsize - 1);
    elem->next = hdr->hashtab[hidx];
    hdr->hashtab[hidx] = nidx;

    पूर्णाङ्क i, d = hdr->dims;
    अत्थाय( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    सचे( esz == परिमाण(भिन्न) )
        *((भिन्न*)p) = 0.f;
    अञ्ञथा सचे( esz == परिमाण(द्विगुण) )
        *((द्विगुण*)p) = 0.;
    अञ्ञथा
        memset(p, 0, esz);

    पटिनिवत्त p;
}


शून्य SparseMat::remove_ग्रन्थिः(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ग्रन्थिः(nidx);
    सचे( previdx )
    {
        Node* prev = ग्रन्थिः(previdx);
        prev->next = n->next;
    }
    अञ्ञथा
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ग्रन्थिः_गणनम्;
}

//
// Operations
//
द्विगुण norm( ध्रुव SparseMat& src, पूर्णाङ्क normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    पूर्णाङ्क type = src.type();
    द्विगुण result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    सचे( type == CV_32F )
    {
        सचे( normType == NORM_INF )
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((द्विगुण)it.value<भिन्न>()));
            }
        अञ्ञथा सचे( normType == NORM_L1 )
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<भिन्न>());
            }
        अञ्ञथा
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<भिन्न>();
                result += v*v;
            }
    }
    अञ्ञथा सचे( type == CV_64F )
    {
        सचे( normType == NORM_INF )
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<द्विगुण>()));
            }
        अञ्ञथा सचे( normType == NORM_L1 )
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<द्विगुण>());
            }
        अञ्ञथा
            अत्थाय( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<द्विगुण>();
                result += v*v;
            }
    }
    अञ्ञथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    सचे( normType == NORM_L2 )
        result = std::sqrt(result);
    पटिनिवत्त result;
}

शून्य minMaxLoc( ध्रुव SparseMat& src, द्विगुण* _minval, द्विगुण* _maxval, पूर्णाङ्क* _minidx, पूर्णाङ्क* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    पूर्णाङ्क type = src.type();
    ध्रुव पूर्णाङ्क *minidx = 0, *maxidx = 0;

    सचे( type == CV_32F )
    {
        भिन्न minval = FLT_MAX, maxval = -FLT_MAX;
        अत्थाय( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            भिन्न v = it.value<भिन्न>();
            सचे( v < minval )
            {
                minval = v;
                minidx = it.ग्रन्थिः()->idx;
            }
            सचे( v > maxval )
            {
                maxval = v;
                maxidx = it.ग्रन्थिः()->idx;
            }
        }
        सचे( _minval )
            *_minval = minval;
        सचे( _maxval )
            *_maxval = maxval;
    }
    अञ्ञथा सचे( type == CV_64F )
    {
        द्विगुण minval = DBL_MAX, maxval = -DBL_MAX;
        अत्थाय( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            द्विगुण v = it.value<द्विगुण>();
            सचे( v < minval )
            {
                minval = v;
                minidx = it.ग्रन्थिः()->idx;
            }
            सचे( v > maxval )
            {
                maxval = v;
                maxidx = it.ग्रन्थिः()->idx;
            }
        }
        सचे( _minval )
            *_minval = minval;
        सचे( _maxval )
            *_maxval = maxval;
    }
    अञ्ञथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    सचे( _minidx && minidx )
        अत्थाय( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    सचे( _maxidx && maxidx )
        अत्थाय( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


शून्य normalize( ध्रुव SparseMat& src, SparseMat& dst, द्विगुण a, पूर्णाङ्क norm_type )
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1;
    सचे( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    अञ्ञथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
