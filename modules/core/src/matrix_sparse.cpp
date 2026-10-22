// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/mat.hpp"

നാമാകാശ cv {

template<typename T1, typename T2> ശൂന്യം
convertData_(സ്ഥിരം ശൂന്യം* _from, ശൂന്യം* _to, പൂർണ്ണസംഖ്യ cn)
{
    സ്ഥിരം T1* from = (സ്ഥിരം T1*)_from;
    T2* to = (T2*)_to;
    എങ്കിൽ( cn == 1 )
        *to = saturate_cast<T2>(*from);
    അല്ലെങ്കിൽ
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> ശൂന്യം
convertScaleData_(സ്ഥിരം ശൂന്യം* _from, ശൂന്യം* _to, പൂർണ്ണസംഖ്യ cn, ഇരട്ട alpha, ഇരട്ട beta)
{
    സ്ഥിരം T1* from = (സ്ഥിരം T1*)_from;
    T2* to = (T2*)_to;
    എങ്കിൽ( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    അല്ലെങ്കിൽ
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

തരനിർവചനം ശൂന്യം (*ConvertData)(സ്ഥിരം ശൂന്യം* from, ശൂന്യം* to, പൂർണ്ണസംഖ്യ cn);
തരനിർവചനം ശൂന്യം (*ConvertScaleData)(സ്ഥിരം ശൂന്യം* from, ശൂന്യം* to, പൂർണ്ണസംഖ്യ cn, ഇരട്ട alpha, ഇരട്ട beta);

സ്ഥിതം ConvertData getConvertElem(പൂർണ്ണസംഖ്യ fromType, പൂർണ്ണസംഖ്യ toType)
{
    സ്ഥിതം ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, പൂർണ്ണസംഖ്യ>, convertData_<uchar, ദശാംശം>,
      convertData_<uchar, ഇരട്ട>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, പൂർണ്ണസംഖ്യ>, convertData_<schar, ദശാംശം>,
      convertData_<schar, ഇരട്ട>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, പൂർണ്ണസംഖ്യ>, convertData_<ushort, ദശാംശം>,
      convertData_<ushort, ഇരട്ട>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, പൂർണ്ണസംഖ്യ>, convertData_<short, ദശാംശം>,
      convertData_<short, ഇരട്ട>, 0 },

    { convertData_<പൂർണ്ണസംഖ്യ, uchar>, convertData_<പൂർണ്ണസംഖ്യ, schar>,
      convertData_<പൂർണ്ണസംഖ്യ, ushort>, convertData_<പൂർണ്ണസംഖ്യ, short>,
      convertData_<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, convertData_<പൂർണ്ണസംഖ്യ, ദശാംശം>,
      convertData_<പൂർണ്ണസംഖ്യ, ഇരട്ട>, 0 },

    { convertData_<ദശാംശം, uchar>, convertData_<ദശാംശം, schar>,
      convertData_<ദശാംശം, ushort>, convertData_<ദശാംശം, short>,
      convertData_<ദശാംശം, പൂർണ്ണസംഖ്യ>, convertData_<ദശാംശം, ദശാംശം>,
      convertData_<ദശാംശം, ഇരട്ട>, 0 },

    { convertData_<ഇരട്ട, uchar>, convertData_<ഇരട്ട, schar>,
      convertData_<ഇരട്ട, ushort>, convertData_<ഇരട്ട, short>,
      convertData_<ഇരട്ട, പൂർണ്ണസംഖ്യ>, convertData_<ഇരട്ട, ദശാംശം>,
      convertData_<ഇരട്ട, ഇരട്ട>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    തിരികെ func;
}

സ്ഥിതം ConvertScaleData getConvertScaleElem(പൂർണ്ണസംഖ്യ fromType, പൂർണ്ണസംഖ്യ toType)
{
    സ്ഥിതം ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, പൂർണ്ണസംഖ്യ>, convertScaleData_<uchar, ദശാംശം>,
      convertScaleData_<uchar, ഇരട്ട>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, പൂർണ്ണസംഖ്യ>, convertScaleData_<schar, ദശാംശം>,
      convertScaleData_<schar, ഇരട്ട>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, പൂർണ്ണസംഖ്യ>, convertScaleData_<ushort, ദശാംശം>,
      convertScaleData_<ushort, ഇരട്ട>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, പൂർണ്ണസംഖ്യ>, convertScaleData_<short, ദശാംശം>,
      convertScaleData_<short, ഇരട്ട>, 0 },

    { convertScaleData_<പൂർണ്ണസംഖ്യ, uchar>, convertScaleData_<പൂർണ്ണസംഖ്യ, schar>,
      convertScaleData_<പൂർണ്ണസംഖ്യ, ushort>, convertScaleData_<പൂർണ്ണസംഖ്യ, short>,
      convertScaleData_<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, convertScaleData_<പൂർണ്ണസംഖ്യ, ദശാംശം>,
      convertScaleData_<പൂർണ്ണസംഖ്യ, ഇരട്ട>, 0 },

    { convertScaleData_<ദശാംശം, uchar>, convertScaleData_<ദശാംശം, schar>,
      convertScaleData_<ദശാംശം, ushort>, convertScaleData_<ദശാംശം, short>,
      convertScaleData_<ദശാംശം, പൂർണ്ണസംഖ്യ>, convertScaleData_<ദശാംശം, ദശാംശം>,
      convertScaleData_<ദശാംശം, ഇരട്ട>, 0 },

    { convertScaleData_<ഇരട്ട, uchar>, convertScaleData_<ഇരട്ട, schar>,
      convertScaleData_<ഇരട്ട, ushort>, convertScaleData_<ഇരട്ട, short>,
      convertScaleData_<ഇരട്ട, പൂർണ്ണസംഖ്യ>, convertScaleData_<ഇരട്ട, ദശാംശം>,
      convertScaleData_<ഇരട്ട, ഇരട്ട>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    തിരികെ func;
}

എണ്ണൽ { HASH_SIZE0 = 8 };

സ്ഥിതം വരിയിൽ ശൂന്യം copyElem(സ്ഥിരം uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    വേണ്ടി( i = 0; i + വലുപ്പം(പൂർണ്ണസംഖ്യ) <= elemSize; i += വലുപ്പം(പൂർണ്ണസംഖ്യ) )
        *(പൂർണ്ണസംഖ്യ*)(to + i) = *(സ്ഥിരം പൂർണ്ണസംഖ്യ*)(from + i);
    വേണ്ടി( ; i < elemSize; i++ )
        to[i] = from[i];
}

സ്ഥിതം വരിയിൽ തർക്കം isZeroElem(സ്ഥിരം uchar* data, size_t elemSize)
{
    size_t i;
    വേണ്ടി( i = 0; i + വലുപ്പം(പൂർണ്ണസംഖ്യ) <= elemSize; i += വലുപ്പം(പൂർണ്ണസംഖ്യ) )
        എങ്കിൽ( *(പൂർണ്ണസംഖ്യ*)(data + i) != 0 )
            തിരികെ അസത്യം;
    വേണ്ടി( ; i < elemSize; i++ )
        എങ്കിൽ( data[i] != 0 )
            തിരികെ അസത്യം;
    തിരികെ സത്യം;
}

SparseMat::Hdr::Hdr( പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type )
{
    refcount = 1;

    dims = _dims;
    മൂല്യം_Offset = (പൂർണ്ണസംഖ്യ)alignSize(വലുപ്പം(SparseMat::Node) - MAX_DIM*വലുപ്പം(പൂർണ്ണസംഖ്യ) +
                                 dims*വലുപ്പം(പൂർണ്ണസംഖ്യ), CV_ELEM_SIZE1(_type));
    കണ്ണി_Size = alignSize(മൂല്യം_Offset +
        CV_ELEM_SIZE(_type), (പൂർണ്ണസംഖ്യ)വലുപ്പം(size_t));

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    വേണ്ടി( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

ശൂന്യം SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#എങ്കിൽ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(കണ്ണി_Size);
#എങ്കിൽ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    കണ്ണി_എണ്ണം = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(സ്ഥിരം SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (സ്ഥിരം SparseMat& m)
{
    എങ്കിൽ( ഇത് != &m )
    {
        എങ്കിൽ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    തിരികെ *ഇത്;
}

SparseMat& SparseMat::operator=(സ്ഥിരം Mat& m)
{
    തിരികെ (*ഇത് = SparseMat(m));
}

ശൂന്യം SparseMat::assignTo(SparseMat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    എങ്കിൽ( _type < 0 )
        m = *ഇത്;
    അല്ലെങ്കിൽ
        convertTo(m, _type);
}

ശൂന്യം SparseMat::addref()
{
    എങ്കിൽ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

ശൂന്യം SparseMat::release()
{
    എങ്കിൽ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(പൂർണ്ണസംഖ്യ i0) സ്ഥിരം
{
    തിരികെ (size_t)i0;
}

size_t SparseMat::hash(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1) സ്ഥിരം
{
    തിരികെ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1, പൂർണ്ണസംഖ്യ i2) സ്ഥിരം
{
    തിരികെ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(സ്ഥിരം പൂർണ്ണസംഖ്യ* idx) സ്ഥിരം
{
    size_t h = (unsigned)idx[0];
    എങ്കിൽ( !hdr )
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ d = hdr->dims;
    വേണ്ടി(പൂർണ്ണസംഖ്യ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    തിരികെ h;
}


SparseMat::SparseMat(സ്ഥിരം Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    പൂർണ്ണസംഖ്യ i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    സ്ഥിരം uchar* dptr = m.ptr();

    വേണ്ടി(;;)
    {
        വേണ്ടി( i = 0; i < lastSize; i++, dptr += esz )
        {
            എങ്കിൽ( isZeroElem(dptr, esz) )
                തുടരുക;
            idx[d-1] = i;
            uchar* to = new_കണ്ണി(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        വേണ്ടി( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            എങ്കിൽ( ++idx[i] < m.size[i] )
                നിർത്തുക;
            idx[i] = 0;
        }
        എങ്കിൽ( i < 0 )
            നിർത്തുക;
    }
}

ശൂന്യം SparseMat::create(പൂർണ്ണസംഖ്യ d, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    എങ്കിൽ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി( i = 0; i < d; i++ )
            എങ്കിൽ( _sizes[i] != hdr->size[i] )
                നിർത്തുക;
        എങ്കിൽ( i == d )
        {
            clear();
            തിരികെ;
        }
    }
    പൂർണ്ണസംഖ്യ _sizes_backup[CV_MAX_DIM]; // #5991
    എങ്കിൽ (hdr && _sizes == hdr->size)
    {
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = പുതിയ Hdr(d, _sizes, _type);
}

ശൂന്യം SparseMat::copyTo( SparseMat& m ) സ്ഥിരം
{
    എങ്കിൽ( hdr == m.hdr )
        തിരികെ;
    എങ്കിൽ( !hdr )
    {
        m.release();
        തിരികെ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    വേണ്ടി( size_t i = 0; i < N; i++, ++from )
    {
        സ്ഥിരം Node* n = from.കണ്ണി();
        uchar* to = m.new_കണ്ണി(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

ശൂന്യം SparseMat::copyTo( Mat& m ) സ്ഥിരം
{
    CV_Assert( hdr );
    പൂർണ്ണസംഖ്യ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    വേണ്ടി( size_t i = 0; i < N; i++, ++from )
    {
        സ്ഥിരം Node* n = from.കണ്ണി();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


ശൂന്യം SparseMat::convertTo( SparseMat& m, പൂർണ്ണസംഖ്യ rtype, ഇരട്ട alpha ) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ cn = channels();
    എങ്കിൽ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    എങ്കിൽ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        തിരികെ;
    }

    CV_Assert(hdr != 0);
    എങ്കിൽ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    എങ്കിൽ( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        വേണ്ടി( size_t i = 0; i < N; i++, ++from )
        {
            സ്ഥിരം Node* n = from.കണ്ണി();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_കണ്ണി(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    അല്ലെങ്കിൽ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        വേണ്ടി( size_t i = 0; i < N; i++, ++from )
        {
            സ്ഥിരം Node* n = from.കണ്ണി();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_കണ്ണി(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


ശൂന്യം SparseMat::convertTo( Mat& m, പൂർണ്ണസംഖ്യ rtype, ഇരട്ട alpha, ഇരട്ട beta ) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ cn = channels();
    എങ്കിൽ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    എങ്കിൽ( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        വേണ്ടി( size_t i = 0; i < N; i++, ++from )
        {
            സ്ഥിരം Node* n = from.കണ്ണി();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    അല്ലെങ്കിൽ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        വേണ്ടി( size_t i = 0; i < N; i++, ++from )
        {
            സ്ഥിരം Node* n = from.കണ്ണി();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

ശൂന്യം SparseMat::clear()
{
    എങ്കിൽ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(പൂർണ്ണസംഖ്യ i0, തർക്കം createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h && elem->idx[0] == i0 )
            തിരികെ &value<uchar>(elem);
        nidx = elem->next;
    }

    എങ്കിൽ( createMissing )
    {
        പൂർണ്ണസംഖ്യ idx[] = { i0 };
        തിരികെ new_കണ്ണി( idx, h );
    }
    തിരികെ ശൂന്യ;
}

uchar* SparseMat::ptr(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1, തർക്കം createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            തിരികെ &value<uchar>(elem);
        nidx = elem->next;
    }

    എങ്കിൽ( createMissing )
    {
        പൂർണ്ണസംഖ്യ idx[] = { i0, i1 };
        തിരികെ new_കണ്ണി( idx, h );
    }
    തിരികെ ശൂന്യ;
}

uchar* SparseMat::ptr(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1, പൂർണ്ണസംഖ്യ i2, തർക്കം createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            തിരികെ &value<uchar>(elem);
        nidx = elem->next;
    }

    എങ്കിൽ( createMissing )
    {
        പൂർണ്ണസംഖ്യ idx[] = { i0, i1, i2 };
        തിരികെ new_കണ്ണി( idx, h );
    }
    തിരികെ ശൂന്യ;
}

uchar* SparseMat::ptr(സ്ഥിരം പൂർണ്ണസംഖ്യ* idx, തർക്കം createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    പൂർണ്ണസംഖ്യ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h )
        {
            വേണ്ടി( i = 0; i < d; i++ )
                എങ്കിൽ( elem->idx[i] != idx[i] )
                    നിർത്തുക;
            എങ്കിൽ( i == d )
                തിരികെ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    തിരികെ createMissing ? new_കണ്ണി(idx, h) : ശൂന്യ;
}

ശൂന്യം SparseMat::erase(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            നിർത്തുക;
        previdx = nidx;
        nidx = elem->next;
    }

    എങ്കിൽ( nidx )
        remove_കണ്ണി(hidx, nidx, previdx);
}

ശൂന്യം SparseMat::erase(പൂർണ്ണസംഖ്യ i0, പൂർണ്ണസംഖ്യ i1, പൂർണ്ണസംഖ്യ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            നിർത്തുക;
        previdx = nidx;
        nidx = elem->next;
    }

    എങ്കിൽ( nidx )
        remove_കണ്ണി(hidx, nidx, previdx);
}

ശൂന്യം SparseMat::erase(സ്ഥിരം പൂർണ്ണസംഖ്യ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    പൂർണ്ണസംഖ്യ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ആകുമ്പോൾ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        എങ്കിൽ( elem->hashval == h )
        {
            വേണ്ടി( i = 0; i < d; i++ )
                എങ്കിൽ( elem->idx[i] != idx[i] )
                    നിർത്തുക;
            എങ്കിൽ( i == d )
                നിർത്തുക;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    എങ്കിൽ( nidx )
        remove_കണ്ണി(hidx, nidx, previdx);
}

ശൂന്യം SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    എങ്കിൽ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((ഇരട്ട)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    വേണ്ടി( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    വേണ്ടി( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ആകുമ്പോൾ( nidx )
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

uchar* SparseMat::new_കണ്ണി(സ്ഥിരം പൂർണ്ണസംഖ്യ* idx, size_t hashval)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    എങ്കിൽ( ++hdr->കണ്ണി_എണ്ണം > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    എങ്കിൽ( !hdr->freeList )
    {
        size_t i, nsz = hdr->കണ്ണി_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        വേണ്ടി( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    പൂർണ്ണസംഖ്യ i, d = hdr->dims;
    വേണ്ടി( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    എങ്കിൽ( esz == വലുപ്പം(ദശാംശം) )
        *((ദശാംശം*)p) = 0.f;
    അല്ലെങ്കിൽ എങ്കിൽ( esz == വലുപ്പം(ഇരട്ട) )
        *((ഇരട്ട*)p) = 0.;
    അല്ലെങ്കിൽ
        memset(p, 0, esz);

    തിരികെ p;
}


ശൂന്യം SparseMat::remove_കണ്ണി(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = കണ്ണി(nidx);
    എങ്കിൽ( previdx )
    {
        Node* prev = കണ്ണി(previdx);
        prev->next = n->next;
    }
    അല്ലെങ്കിൽ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->കണ്ണി_എണ്ണം;
}

//
// Operations
//
ഇരട്ട norm( സ്ഥിരം SparseMat& src, പൂർണ്ണസംഖ്യ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    പൂർണ്ണസംഖ്യ type = src.type();
    ഇരട്ട result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    എങ്കിൽ( type == CV_32F )
    {
        എങ്കിൽ( normType == NORM_INF )
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((ഇരട്ട)it.value<ദശാംശം>()));
            }
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L1 )
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ദശാംശം>());
            }
        അല്ലെങ്കിൽ
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ഇരട്ട v = it.value<ദശാംശം>();
                result += v*v;
            }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64F )
    {
        എങ്കിൽ( normType == NORM_INF )
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<ഇരട്ട>()));
            }
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L1 )
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ഇരട്ട>());
            }
        അല്ലെങ്കിൽ
            വേണ്ടി( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ഇരട്ട v = it.value<ഇരട്ട>();
                result += v*v;
            }
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    എങ്കിൽ( normType == NORM_L2 )
        result = std::sqrt(result);
    തിരികെ result;
}

ശൂന്യം minMaxLoc( സ്ഥിരം SparseMat& src, ഇരട്ട* _minval, ഇരട്ട* _maxval, പൂർണ്ണസംഖ്യ* _minidx, പൂർണ്ണസംഖ്യ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    പൂർണ്ണസംഖ്യ type = src.type();
    സ്ഥിരം പൂർണ്ണസംഖ്യ *minidx = 0, *maxidx = 0;

    എങ്കിൽ( type == CV_32F )
    {
        ദശാംശം minval = FLT_MAX, maxval = -FLT_MAX;
        വേണ്ടി( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ദശാംശം v = it.value<ദശാംശം>();
            എങ്കിൽ( v < minval )
            {
                minval = v;
                minidx = it.കണ്ണി()->idx;
            }
            എങ്കിൽ( v > maxval )
            {
                maxval = v;
                maxidx = it.കണ്ണി()->idx;
            }
        }
        എങ്കിൽ( _minval )
            *_minval = minval;
        എങ്കിൽ( _maxval )
            *_maxval = maxval;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64F )
    {
        ഇരട്ട minval = DBL_MAX, maxval = -DBL_MAX;
        വേണ്ടി( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ഇരട്ട v = it.value<ഇരട്ട>();
            എങ്കിൽ( v < minval )
            {
                minval = v;
                minidx = it.കണ്ണി()->idx;
            }
            എങ്കിൽ( v > maxval )
            {
                maxval = v;
                maxidx = it.കണ്ണി()->idx;
            }
        }
        എങ്കിൽ( _minval )
            *_minval = minval;
        എങ്കിൽ( _maxval )
            *_maxval = maxval;
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    എങ്കിൽ( _minidx && minidx )
        വേണ്ടി( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    എങ്കിൽ( _maxidx && maxidx )
        വേണ്ടി( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


ശൂന്യം normalize( സ്ഥിരം SparseMat& src, SparseMat& dst, ഇരട്ട a, പൂർണ്ണസംഖ്യ norm_type )
{
    CV_INSTRUMENT_REGION();

    ഇരട്ട scale = 1;
    എങ്കിൽ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
