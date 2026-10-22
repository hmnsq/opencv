// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, puur_naa_nka cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ᤀᤡ᤺( cn == 1 )
        *to = saturate_cast<T2>(*from);
    ᤀᤡ᤺ᤆᤡ
        for( puur_naa_nka i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, puur_naa_nka cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ᤀᤡ᤺( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    ᤀᤡ᤺ᤆᤡ
        for( puur_naa_nka i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, puur_naa_nka cn);
typedef void (*ConvertScaleData)(const void* from, void* to, puur_naa_nka cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(puur_naa_nka fromType, puur_naa_nka toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, puur_naa_nka>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, puur_naa_nka>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, puur_naa_nka>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, puur_naa_nka>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<puur_naa_nka, uchar>, convertData_<puur_naa_nka, schar>,
      convertData_<puur_naa_nka, ushort>, convertData_<puur_naa_nka, short>,
      convertData_<puur_naa_nka, puur_naa_nka>, convertData_<puur_naa_nka, float>,
      convertData_<puur_naa_nka, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, puur_naa_nka>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, puur_naa_nka>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ᤀᤡᤴ func;
}

static ConvertScaleData getConvertScaleElem(puur_naa_nka fromType, puur_naa_nka toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, puur_naa_nka>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, puur_naa_nka>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, puur_naa_nka>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, puur_naa_nka>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<puur_naa_nka, uchar>, convertScaleData_<puur_naa_nka, schar>,
      convertScaleData_<puur_naa_nka, ushort>, convertScaleData_<puur_naa_nka, short>,
      convertScaleData_<puur_naa_nka, puur_naa_nka>, convertScaleData_<puur_naa_nka, float>,
      convertScaleData_<puur_naa_nka, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, puur_naa_nka>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, puur_naa_nka>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ᤀᤡᤴ func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    for( i = 0; i + parimaa_na(puur_naa_nka) <= elemSize; i += parimaa_na(puur_naa_nka) )
        *(puur_naa_nka*)(to + i) = *(const puur_naa_nka*)(from + i);
    for( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    for( i = 0; i + parimaa_na(puur_naa_nka) <= elemSize; i += parimaa_na(puur_naa_nka) )
        ᤀᤡ᤺( *(puur_naa_nka*)(data + i) != 0 )
            ᤀᤡᤴ false;
    for( ; i < elemSize; i++ )
        ᤀᤡ᤺( data[i] != 0 )
            ᤀᤡᤴ false;
    ᤀᤡᤴ true;
}

SparseMat::Hdr::Hdr( puur_naa_nka _dims, const puur_naa_nka* _sizes, puur_naa_nka _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (puur_naa_nka)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(puur_naa_nka) +
                                 dims*parimaa_na(puur_naa_nka), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (puur_naa_nka)parimaa_na(size_t));

    puur_naa_nka i;
    for( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    for( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ᤀᤡ᤺ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#ᤀᤡ᤺ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(puur_naa_nka _dims, const puur_naa_nka* _sizes, puur_naa_nka _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(const SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (const SparseMat& m)
{
    ᤀᤡ᤺( this != &m )
    {
        ᤀᤡ᤺( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ᤀᤡᤴ *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ᤀᤡᤴ (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, puur_naa_nka _type) const
{
    ᤀᤡ᤺( _type < 0 )
        m = *this;
    ᤀᤡ᤺ᤆᤡ
        convertTo(m, _type);
}

void SparseMat::addref()
{
    ᤀᤡ᤺( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    ᤀᤡ᤺( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(puur_naa_nka i0) const
{
    ᤀᤡᤴ (size_t)i0;
}

size_t SparseMat::hash(puur_naa_nka i0, puur_naa_nka i1) const
{
    ᤀᤡᤴ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(puur_naa_nka i0, puur_naa_nka i1, puur_naa_nka i2) const
{
    ᤀᤡᤴ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const puur_naa_nka* idx) const
{
    size_t h = (unsigned)idx[0];
    ᤀᤡ᤺( !hdr )
        ᤀᤡᤴ 0;
    puur_naa_nka d = hdr->dims;
    for(puur_naa_nka i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ᤀᤡᤴ h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    puur_naa_nka i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    for(;;)
    {
        for( i = 0; i < lastSize; i++, dptr += esz )
        {
            ᤀᤡ᤺( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        for( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ᤀᤡ᤺( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        ᤀᤡ᤺( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(puur_naa_nka d, const puur_naa_nka* _sizes, puur_naa_nka _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    for( puur_naa_nka i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ᤀᤡ᤺( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        puur_naa_nka i;
        for( i = 0; i < d; i++ )
            ᤀᤡ᤺( _sizes[i] != hdr->size[i] )
                bha_nga;
        ᤀᤡ᤺( i == d )
        {
            clear();
            ᤀᤡᤴ;
        }
    }
    puur_naa_nka _sizes_backup[CV_MAX_DIM]; // #5991
    ᤀᤡ᤺ (hdr && _sizes == hdr->size)
    {
        for(puur_naa_nka i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    ᤀᤡ᤺( hdr == m.hdr )
        ᤀᤡᤴ;
    ᤀᤡ᤺( !hdr )
    {
        m.release();
        ᤀᤡᤴ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    for( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    puur_naa_nka ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    for( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, puur_naa_nka rtype, dviguu_na alpha ) const
{
    puur_naa_nka cn = channels();
    ᤀᤡ᤺( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ᤀᤡ᤺( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ᤀᤡᤴ;
    }

    CV_Assert(hdr != 0);
    ᤀᤡ᤺( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ᤀᤡ᤺( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ᤀᤡ᤺ᤆᤡ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, puur_naa_nka rtype, dviguu_na alpha, dviguu_na beta ) const
{
    puur_naa_nka cn = channels();
    ᤀᤡ᤺( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ᤀᤡ᤺( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ᤀᤡ᤺ᤆᤡ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    ᤀᤡ᤺( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(puur_naa_nka i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h && elem->idx[0] == i0 )
            ᤀᤡᤴ &value<uchar>(elem);
        nidx = elem->next;
    }

    ᤀᤡ᤺( createMissing )
    {
        puur_naa_nka idx[] = { i0 };
        ᤀᤡᤴ newNode( idx, h );
    }
    ᤀᤡᤴ NULL;
}

uchar* SparseMat::ptr(puur_naa_nka i0, puur_naa_nka i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ᤀᤡᤴ &value<uchar>(elem);
        nidx = elem->next;
    }

    ᤀᤡ᤺( createMissing )
    {
        puur_naa_nka idx[] = { i0, i1 };
        ᤀᤡᤴ newNode( idx, h );
    }
    ᤀᤡᤴ NULL;
}

uchar* SparseMat::ptr(puur_naa_nka i0, puur_naa_nka i1, puur_naa_nka i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ᤀᤡᤴ &value<uchar>(elem);
        nidx = elem->next;
    }

    ᤀᤡ᤺( createMissing )
    {
        puur_naa_nka idx[] = { i0, i1, i2 };
        ᤀᤡᤴ newNode( idx, h );
    }
    ᤀᤡᤴ NULL;
}

uchar* SparseMat::ptr(const puur_naa_nka* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    puur_naa_nka i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h )
        {
            for( i = 0; i < d; i++ )
                ᤀᤡ᤺( elem->idx[i] != idx[i] )
                    bha_nga;
            ᤀᤡ᤺( i == d )
                ᤀᤡᤴ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ᤀᤡᤴ createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(puur_naa_nka i0, puur_naa_nka i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ᤀᤡ᤺( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(puur_naa_nka i0, puur_naa_nka i1, puur_naa_nka i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ᤀᤡ᤺( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const puur_naa_nka* idx, size_t* hashval)
{
    CV_Assert( hdr );
    puur_naa_nka i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ᤀᤠ᤺( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ᤀᤡ᤺( elem->hashval == h )
        {
            for( i = 0; i < d; i++ )
                ᤀᤡ᤺( elem->idx[i] != idx[i] )
                    bha_nga;
            ᤀᤡ᤺( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ᤀᤡ᤺( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ᤀᤡ᤺((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    for( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    for( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ᤀᤠ᤺( nidx )
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

uchar* SparseMat::newNode(const puur_naa_nka* idx, size_t hashval)
{
    const puur_naa_nka HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ᤀᤡ᤺( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ᤀᤡ᤺( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        for( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    puur_naa_nka i, d = hdr->dims;
    for( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ᤀᤡ᤺( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    ᤀᤡ᤺ᤆᤡ
        memset(p, 0, esz);

    ᤀᤡᤴ p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    ᤀᤡ᤺( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    ᤀᤡ᤺ᤆᤡ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, puur_naa_nka normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    puur_naa_nka type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ᤀᤡ᤺( type == CV_32F )
    {
        ᤀᤡ᤺( normType == NORM_INF )
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( normType == NORM_L1 )
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        ᤀᤡ᤺ᤆᤡ
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( type == CV_64F )
    {
        ᤀᤡ᤺( normType == NORM_INF )
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( normType == NORM_L1 )
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        ᤀᤡ᤺ᤆᤡ
            for( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    ᤀᤡ᤺ᤆᤡ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ᤀᤡ᤺( normType == NORM_L2 )
        result = std::sqrt(result);
    ᤀᤡᤴ result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, puur_naa_nka* _minidx, puur_naa_nka* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    puur_naa_nka type = src.type();
    const puur_naa_nka *minidx = 0, *maxidx = 0;

    ᤀᤡ᤺( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        for( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            ᤀᤡ᤺( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ᤀᤡ᤺( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ᤀᤡ᤺( _minval )
            *_minval = minval;
        ᤀᤡ᤺( _maxval )
            *_maxval = maxval;
    }
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        for( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            ᤀᤡ᤺( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ᤀᤡ᤺( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ᤀᤡ᤺( _minval )
            *_minval = minval;
        ᤀᤡ᤺( _maxval )
            *_maxval = maxval;
    }
    ᤀᤡ᤺ᤆᤡ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ᤀᤡ᤺( _minidx && minidx )
        for( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ᤀᤡ᤺( _maxidx && maxidx )
        for( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, puur_naa_nka norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    ᤀᤡ᤺( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ᤀᤡ᤺ᤆᤡ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
