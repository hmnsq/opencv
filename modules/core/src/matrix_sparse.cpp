// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

ढाँचा<typename T1, typename T2> शून्य
convertData_(ध्रुव शून्य* _from, शून्य* _to, पूर्णाङ्क cn)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* to = (T2*)_to;
    यदि( cn == 1 )
        *to = saturate_cast<T2>(*from);
    अन्यथा
        प्रति( पूर्णाङ्क i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

ढाँचा<typename T1, typename T2> शून्य
convertScaleData_(ध्रुव शून्य* _from, शून्य* _to, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* to = (T2*)_to;
    यदि( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    अन्यथा
        प्रति( पूर्णाङ्क i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

प्रकारनिर्देश शून्य (*ConvertData)(ध्रुव शून्य* from, शून्य* to, पूर्णाङ्क cn);
प्रकारनिर्देश शून्य (*ConvertScaleData)(ध्रुव शून्य* from, शून्य* to, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta);

स्थिर ConvertData getConvertElem(पूर्णाङ्क fromType, पूर्णाङ्क toType)
{
    स्थिर ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, लघु>,
      convertData_<uchar, पूर्णाङ्क>, convertData_<uchar, भिन्न>,
      convertData_<uchar, द्विगुण>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, लघु>,
      convertData_<schar, पूर्णाङ्क>, convertData_<schar, भिन्न>,
      convertData_<schar, द्विगुण>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, लघु>,
      convertData_<ushort, पूर्णाङ्क>, convertData_<ushort, भिन्न>,
      convertData_<ushort, द्विगुण>, 0 },

    { convertData_<लघु, uchar>, convertData_<लघु, schar>,
      convertData_<लघु, ushort>, convertData_<लघु, लघु>,
      convertData_<लघु, पूर्णाङ्क>, convertData_<लघु, भिन्न>,
      convertData_<लघु, द्विगुण>, 0 },

    { convertData_<पूर्णाङ्क, uchar>, convertData_<पूर्णाङ्क, schar>,
      convertData_<पूर्णाङ्क, ushort>, convertData_<पूर्णाङ्क, लघु>,
      convertData_<पूर्णाङ्क, पूर्णाङ्क>, convertData_<पूर्णाङ्क, भिन्न>,
      convertData_<पूर्णाङ्क, द्विगुण>, 0 },

    { convertData_<भिन्न, uchar>, convertData_<भिन्न, schar>,
      convertData_<भिन्न, ushort>, convertData_<भिन्न, लघु>,
      convertData_<भिन्न, पूर्णाङ्क>, convertData_<भिन्न, भिन्न>,
      convertData_<भिन्न, द्विगुण>, 0 },

    { convertData_<द्विगुण, uchar>, convertData_<द्विगुण, schar>,
      convertData_<द्विगुण, ushort>, convertData_<द्विगुण, लघु>,
      convertData_<द्विगुण, पूर्णाङ्क>, convertData_<द्विगुण, भिन्न>,
      convertData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    प्रत्यागम func;
}

स्थिर ConvertScaleData getConvertScaleElem(पूर्णाङ्क fromType, पूर्णाङ्क toType)
{
    स्थिर ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, लघु>,
      convertScaleData_<uchar, पूर्णाङ्क>, convertScaleData_<uchar, भिन्न>,
      convertScaleData_<uchar, द्विगुण>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, लघु>,
      convertScaleData_<schar, पूर्णाङ्क>, convertScaleData_<schar, भिन्न>,
      convertScaleData_<schar, द्विगुण>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, लघु>,
      convertScaleData_<ushort, पूर्णाङ्क>, convertScaleData_<ushort, भिन्न>,
      convertScaleData_<ushort, द्विगुण>, 0 },

    { convertScaleData_<लघु, uchar>, convertScaleData_<लघु, schar>,
      convertScaleData_<लघु, ushort>, convertScaleData_<लघु, लघु>,
      convertScaleData_<लघु, पूर्णाङ्क>, convertScaleData_<लघु, भिन्न>,
      convertScaleData_<लघु, द्विगुण>, 0 },

    { convertScaleData_<पूर्णाङ्क, uchar>, convertScaleData_<पूर्णाङ्क, schar>,
      convertScaleData_<पूर्णाङ्क, ushort>, convertScaleData_<पूर्णाङ्क, लघु>,
      convertScaleData_<पूर्णाङ्क, पूर्णाङ्क>, convertScaleData_<पूर्णाङ्क, भिन्न>,
      convertScaleData_<पूर्णाङ्क, द्विगुण>, 0 },

    { convertScaleData_<भिन्न, uchar>, convertScaleData_<भिन्न, schar>,
      convertScaleData_<भिन्न, ushort>, convertScaleData_<भिन्न, लघु>,
      convertScaleData_<भिन्न, पूर्णाङ्क>, convertScaleData_<भिन्न, भिन्न>,
      convertScaleData_<भिन्न, द्विगुण>, 0 },

    { convertScaleData_<द्विगुण, uchar>, convertScaleData_<द्विगुण, schar>,
      convertScaleData_<द्विगुण, ushort>, convertScaleData_<द्विगुण, लघु>,
      convertScaleData_<द्विगुण, पूर्णाङ्क>, convertScaleData_<द्विगुण, भिन्न>,
      convertScaleData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    प्रत्यागम func;
}

गणना { HASH_SIZE0 = 8 };

स्थिर अन्तःस्थ शून्य copyElem(ध्रुव uchar* from, uchar* to, माप_प्रकार elemSize)
{
    माप_प्रकार i;
    प्रति( i = 0; i + परिमाण(पूर्णाङ्क) <= elemSize; i += परिमाण(पूर्णाङ्क) )
        *(पूर्णाङ्क*)(to + i) = *(ध्रुव पूर्णाङ्क*)(from + i);
    प्रति( ; i < elemSize; i++ )
        to[i] = from[i];
}

स्थिर अन्तःस्थ तार्किक isZeroElem(ध्रुव uchar* data, माप_प्रकार elemSize)
{
    माप_प्रकार i;
    प्रति( i = 0; i + परिमाण(पूर्णाङ्क) <= elemSize; i += परिमाण(पूर्णाङ्क) )
        यदि( *(पूर्णाङ्क*)(data + i) != 0 )
            प्रत्यागम असत्य;
    प्रति( ; i < elemSize; i++ )
        यदि( data[i] != 0 )
            प्रत्यागम असत्य;
    प्रत्यागम सत्य;
}

SparseMat::Hdr::Hdr( पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type )
{
    refcount = 1;

    dims = _dims;
    मूल्य_Offset = (पूर्णाङ्क)alignSize(परिमाण(SparseMat::Node) - MAX_DIM*परिमाण(पूर्णाङ्क) +
                                 dims*परिमाण(पूर्णाङ्क), CV_ELEM_SIZE1(_type));
    गाँठ_Size = alignSize(मूल्य_Offset +
        CV_ELEM_SIZE(_type), (पूर्णाङ्क)परिमाण(माप_प्रकार));

    पूर्णाङ्क i;
    प्रति( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    प्रति( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

शून्य SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#यदि defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(गाँठ_Size);
#यदि defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    गाँठ_गिनती = freeList = 0;
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

SparseMat& SparseMat::चालक = (ध्रुव SparseMat& m)
{
    यदि( एतत् != &m )
    {
        यदि( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    प्रत्यागम *एतत्;
}

SparseMat& SparseMat::चालक=(ध्रुव Mat& m)
{
    प्रत्यागम (*एतत् = SparseMat(m));
}

शून्य SparseMat::assignTo(SparseMat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convertTo(m, _type);
}

शून्य SparseMat::addref()
{
    यदि( hdr )
        CV_XADD(&hdr->refcount, 1);
}

शून्य SparseMat::release()
{
    यदि( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

माप_प्रकार SparseMat::hash(पूर्णाङ्क i0) ध्रुव
{
    प्रत्यागम (माप_प्रकार)i0;
}

माप_प्रकार SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1) ध्रुव
{
    प्रत्यागम (माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1;
}

माप_प्रकार SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2) ध्रुव
{
    प्रत्यागम ((माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1) * HASH_SCALE + (अचिन्हित)i2;
}

माप_प्रकार SparseMat::hash(ध्रुव पूर्णाङ्क* idx) ध्रुव
{
    माप_प्रकार h = (अचिन्हित)idx[0];
    यदि( !hdr )
        प्रत्यागम 0;
    पूर्णाङ्क d = hdr->dims;
    प्रति(पूर्णाङ्क i = 1; i < d; i++ )
        h = h * HASH_SCALE + (अचिन्हित)idx[i];
    प्रत्यागम h;
}


SparseMat::SparseMat(ध्रुव Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    पूर्णाङ्क i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    माप_प्रकार esz = m.elemSize();
    ध्रुव uchar* dptr = m.ptr();

    प्रति(;;)
    {
        प्रति( i = 0; i < lastSize; i++, dptr += esz )
        {
            यदि( isZeroElem(dptr, esz) )
                अनुवर्त;
            idx[d-1] = i;
            uchar* to = new_गाँठ(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        प्रति( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            यदि( ++idx[i] < m.size[i] )
                भङ्ग;
            idx[i] = 0;
        }
        यदि( i < 0 )
            भङ्ग;
    }
}

शून्य SparseMat::create(पूर्णाङ्क d, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    प्रति( पूर्णाङ्क i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    यदि( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        पूर्णाङ्क i;
        प्रति( i = 0; i < d; i++ )
            यदि( _sizes[i] != hdr->size[i] )
                भङ्ग;
        यदि( i == d )
        {
            clear();
            प्रत्यागम;
        }
    }
    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    यदि (hdr && _sizes == hdr->size)
    {
        प्रति(पूर्णाङ्क i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = नूतन Hdr(d, _sizes, _type);
}

शून्य SparseMat::copyTo( SparseMat& m ) ध्रुव
{
    यदि( hdr == m.hdr )
        प्रत्यागम;
    यदि( !hdr )
    {
        m.release();
        प्रत्यागम;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount(), esz = elemSize();

    प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.गाँठ();
        uchar* to = m.new_गाँठ(n->idx, n->hashval);
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
    माप_प्रकार N = nzcount(), esz = elemSize();

    प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.गाँठ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


शून्य SparseMat::convertTo( SparseMat& m, पूर्णाङ्क rtype, द्विगुण alpha ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    यदि( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    यदि( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        प्रत्यागम;
    }

    CV_Assert(hdr != 0);
    यदि( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    यदि( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.गाँठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाँठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.गाँठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाँठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


शून्य SparseMat::convertTo( Mat& m, पूर्णाङ्क rtype, द्विगुण alpha, द्विगुण beta ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    यदि( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    यदि( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.गाँठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        प्रति( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.गाँठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

शून्य SparseMat::clear()
{
    यदि( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    माप_प्रकार h = hashval ? *hashval : hash(i0);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0 };
        प्रत्यागम new_गाँठ( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1 };
        प्रत्यागम new_गाँठ( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1, i2 };
        प्रत्यागम new_गाँठ( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(ध्रुव पूर्णाङ्क* idx, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h )
        {
            प्रति( i = 0; i < d; i++ )
                यदि( elem->idx[i] != idx[i] )
                    भङ्ग;
            यदि( i == d )
                प्रत्यागम &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    प्रत्यागम createMissing ? new_गाँठ(idx, h) : रिक्त;
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

शून्य SparseMat::erase(ध्रुव पूर्णाङ्क* idx, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h )
        {
            प्रति( i = 0; i < d; i++ )
                यदि( elem->idx[i] != idx[i] )
                    भङ्ग;
            यदि( i == d )
                भङ्ग;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

शून्य SparseMat::resizeHashTab(माप_प्रकार newsize)
{
    newsize = std::max(newsize, (माप_प्रकार)8);
    यदि((newsize & (newsize-1)) != 0)
        newsize = (माप_प्रकार)1 << cvCeil(std::log((द्विगुण)newsize)/CV_LOG2);

    माप_प्रकार hsize = hdr->hashtab.size();
    std::vector<माप_प्रकार> _newh(newsize);
    माप_प्रकार* newh = &_newh[0];
    प्रति( माप_प्रकार i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    प्रति( माप_प्रकार i = 0; i < hsize; i++ )
    {
        माप_प्रकार nidx = hdr->hashtab[i];
        यावत्( nidx )
        {
            Node* elem = (Node*)(pool + nidx);
            माप_प्रकार next = elem->next;
            माप_प्रकार newhidx = elem->hashval & (newsize - 1);
            elem->next = newh[newhidx];
            newh[newhidx] = nidx;
            nidx = next;
        }
    }
    hdr->hashtab = _newh;
}

uchar* SparseMat::new_गाँठ(ध्रुव पूर्णाङ्क* idx, माप_प्रकार hashval)
{
    ध्रुव पूर्णाङ्क HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    माप_प्रकार hsize = hdr->hashtab.size();
    यदि( ++hdr->गाँठ_गिनती > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (माप_प्रकार)8));
        hsize = hdr->hashtab.size();
    }

    यदि( !hdr->freeList )
    {
        माप_प्रकार i, nsz = hdr->गाँठ_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        प्रति( i = hdr->freeList; i < newpsize - nsz; i += nsz )
            ((Node*)(pool + i))->next = i + nsz;
        ((Node*)(pool + i))->next = 0;
    }
    माप_प्रकार nidx = hdr->freeList;
    Node* elem = (Node*)&hdr->pool[nidx];
    hdr->freeList = elem->next;
    elem->hashval = hashval;
    माप_प्रकार hidx = hashval & (hsize - 1);
    elem->next = hdr->hashtab[hidx];
    hdr->hashtab[hidx] = nidx;

    पूर्णाङ्क i, d = hdr->dims;
    प्रति( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    माप_प्रकार esz = elemSize();
    uchar* p = &value<uchar>(elem);
    यदि( esz == परिमाण(भिन्न) )
        *((भिन्न*)p) = 0.f;
    अन्यथा यदि( esz == परिमाण(द्विगुण) )
        *((द्विगुण*)p) = 0.;
    अन्यथा
        स_रखो(p, 0, esz);

    प्रत्यागम p;
}


शून्य SparseMat::remove_गाँठ(माप_प्रकार hidx, माप_प्रकार nidx, माप_प्रकार previdx)
{
    Node* n = गाँठ(nidx);
    यदि( previdx )
    {
        Node* prev = गाँठ(previdx);
        prev->next = n->next;
    }
    अन्यथा
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->गाँठ_गिनती;
}

//
// Operations
//
द्विगुण norm( ध्रुव SparseMat& src, पूर्णाङ्क normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    माप_प्रकार i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    पूर्णाङ्क type = src.type();
    द्विगुण result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    यदि( type == CV_32F )
    {
        यदि( normType == NORM_INF )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::असल((द्विगुण)it.value<भिन्न>()));
            }
        अन्यथा यदि( normType == NORM_L1 )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::असल(it.value<भिन्न>());
            }
        अन्यथा
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<भिन्न>();
                result += v*v;
            }
    }
    अन्यथा यदि( type == CV_64F )
    {
        यदि( normType == NORM_INF )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::असल(it.value<द्विगुण>()));
            }
        अन्यथा यदि( normType == NORM_L1 )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::असल(it.value<द्विगुण>());
            }
        अन्यथा
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<द्विगुण>();
                result += v*v;
            }
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    यदि( normType == NORM_L2 )
        result = std::वर्ग_मूल(result);
    प्रत्यागम result;
}

शून्य minMaxLoc( ध्रुव SparseMat& src, द्विगुण* _minval, द्विगुण* _maxval, पूर्णाङ्क* _minidx, पूर्णाङ्क* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    माप_प्रकार i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    पूर्णाङ्क type = src.type();
    ध्रुव पूर्णाङ्क *minidx = 0, *maxidx = 0;

    यदि( type == CV_32F )
    {
        भिन्न minval = भग्न_उच्च, maxval = -भग्न_उच्च;
        प्रति( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            भिन्न v = it.value<भिन्न>();
            यदि( v < minval )
            {
                minval = v;
                minidx = it.गाँठ()->idx;
            }
            यदि( v > maxval )
            {
                maxval = v;
                maxidx = it.गाँठ()->idx;
            }
        }
        यदि( _minval )
            *_minval = minval;
        यदि( _maxval )
            *_maxval = maxval;
    }
    अन्यथा यदि( type == CV_64F )
    {
        द्विगुण minval = द्विग_उच्च, maxval = -द्विग_उच्च;
        प्रति( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            द्विगुण v = it.value<द्विगुण>();
            यदि( v < minval )
            {
                minval = v;
                minidx = it.गाँठ()->idx;
            }
            यदि( v > maxval )
            {
                maxval = v;
                maxidx = it.गाँठ()->idx;
            }
        }
        यदि( _minval )
            *_minval = minval;
        यदि( _maxval )
            *_maxval = maxval;
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    यदि( _minidx && minidx )
        प्रति( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    यदि( _maxidx && maxidx )
        प्रति( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


शून्य normalize( ध्रुव SparseMat& src, SparseMat& dst, द्विगुण a, पूर्णाङ्क norm_type )
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1;
    यदि( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > द्विग_अंतर ? a/scale : 0.;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
