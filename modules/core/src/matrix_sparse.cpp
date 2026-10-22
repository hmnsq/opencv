// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, ຈຳນວນເຕັມ cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ຖ້າ( cn == 1 )
        *to = saturate_cast<T2>(*from);
    ບໍ່ດັ່ງນັ້ນ
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, ຈຳນວນເຕັມ cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ຖ້າ( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    ບໍ່ດັ່ງນັ້ນ
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, ຈຳນວນເຕັມ cn);
typedef void (*ConvertScaleData)(const void* from, void* to, ຈຳນວນເຕັມ cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(ຈຳນວນເຕັມ fromType, ຈຳນວນເຕັມ toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ຈຳນວນເຕັມ>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ຈຳນວນເຕັມ>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ຈຳນວນເຕັມ>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ຈຳນວນເຕັມ>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<ຈຳນວນເຕັມ, uchar>, convertData_<ຈຳນວນເຕັມ, schar>,
      convertData_<ຈຳນວນເຕັມ, ushort>, convertData_<ຈຳນວນເຕັມ, short>,
      convertData_<ຈຳນວນເຕັມ, ຈຳນວນເຕັມ>, convertData_<ຈຳນວນເຕັມ, float>,
      convertData_<ຈຳນວນເຕັມ, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, ຈຳນວນເຕັມ>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, ຈຳນວນເຕັມ>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ສົ່ງຄືນ func;
}

static ConvertScaleData getConvertScaleElem(ຈຳນວນເຕັມ fromType, ຈຳນວນເຕັມ toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ຈຳນວນເຕັມ>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ຈຳນວນເຕັມ>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ຈຳນວນເຕັມ>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ຈຳນວນເຕັມ>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<ຈຳນວນເຕັມ, uchar>, convertScaleData_<ຈຳນວນເຕັມ, schar>,
      convertScaleData_<ຈຳນວນເຕັມ, ushort>, convertScaleData_<ຈຳນວນເຕັມ, short>,
      convertScaleData_<ຈຳນວນເຕັມ, ຈຳນວນເຕັມ>, convertScaleData_<ຈຳນວນເຕັມ, float>,
      convertScaleData_<ຈຳນວນເຕັມ, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, ຈຳນວນເຕັມ>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, ຈຳນວນເຕັມ>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ສົ່ງຄືນ func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    ສຳລັບ( i = 0; i + parimaa_na(ຈຳນວນເຕັມ) <= elemSize; i += parimaa_na(ຈຳນວນເຕັມ) )
        *(ຈຳນວນເຕັມ*)(to + i) = *(const ຈຳນວນເຕັມ*)(from + i);
    ສຳລັບ( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    ສຳລັບ( i = 0; i + parimaa_na(ຈຳນວນເຕັມ) <= elemSize; i += parimaa_na(ຈຳນວນເຕັມ) )
        ຖ້າ( *(ຈຳນວນເຕັມ*)(data + i) != 0 )
            ສົ່ງຄືນ false;
    ສຳລັບ( ; i < elemSize; i++ )
        ຖ້າ( data[i] != 0 )
            ສົ່ງຄືນ false;
    ສົ່ງຄືນ true;
}

SparseMat::Hdr::Hdr( ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (ຈຳນວນເຕັມ)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(ຈຳນວນເຕັມ) +
                                 dims*parimaa_na(ຈຳນວນເຕັມ), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (ຈຳນວນເຕັມ)parimaa_na(size_t));

    ຈຳນວນເຕັມ i;
    ສຳລັບ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    ສຳລັບ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ຖ້າ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#ຖ້າ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type)
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
    ຖ້າ( this != &m )
    {
        ຖ້າ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ສົ່ງຄືນ *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ສົ່ງຄືນ (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, ຈຳນວນເຕັມ _type) const
{
    ຖ້າ( _type < 0 )
        m = *this;
    ບໍ່ດັ່ງນັ້ນ
        convertTo(m, _type);
}

void SparseMat::addref()
{
    ຖ້າ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    ຖ້າ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ຈຳນວນເຕັມ i0) const
{
    ສົ່ງຄືນ (size_t)i0;
}

size_t SparseMat::hash(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1) const
{
    ສົ່ງຄືນ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1, ຈຳນວນເຕັມ i2) const
{
    ສົ່ງຄືນ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const ຈຳນວນເຕັມ* idx) const
{
    size_t h = (unsigned)idx[0];
    ຖ້າ( !hdr )
        ສົ່ງຄືນ 0;
    ຈຳນວນເຕັມ d = hdr->dims;
    ສຳລັບ(ຈຳນວນເຕັມ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ສົ່ງຄືນ h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ຈຳນວນເຕັມ i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    ສຳລັບ(;;)
    {
        ສຳລັບ( i = 0; i < lastSize; i++, dptr += esz )
        {
            ຖ້າ( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        ສຳລັບ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ຖ້າ( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        ຖ້າ( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(ຈຳນວນເຕັມ d, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ຖ້າ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ຈຳນວນເຕັມ i;
        ສຳລັບ( i = 0; i < d; i++ )
            ຖ້າ( _sizes[i] != hdr->size[i] )
                bha_nga;
        ຖ້າ( i == d )
        {
            clear();
            ສົ່ງຄືນ;
        }
    }
    ຈຳນວນເຕັມ _sizes_backup[CV_MAX_DIM]; // #5991
    ຖ້າ (hdr && _sizes == hdr->size)
    {
        ສຳລັບ(ຈຳນວນເຕັມ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    ຖ້າ( hdr == m.hdr )
        ສົ່ງຄືນ;
    ຖ້າ( !hdr )
    {
        m.release();
        ສົ່ງຄືນ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ສຳລັບ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    ຈຳນວນເຕັມ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ສຳລັບ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, ຈຳນວນເຕັມ rtype, dviguu_na alpha ) const
{
    ຈຳນວນເຕັມ cn = channels();
    ຖ້າ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ຖ້າ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ສົ່ງຄືນ;
    }

    CV_Assert(hdr != 0);
    ຖ້າ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ຖ້າ( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        ສຳລັບ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        ສຳລັບ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, ຈຳນວນເຕັມ rtype, dviguu_na alpha, dviguu_na beta ) const
{
    ຈຳນວນເຕັມ cn = channels();
    ຖ້າ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ຖ້າ( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        ສຳລັບ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        ສຳລັບ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    ຖ້າ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ຈຳນວນເຕັມ i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h && elem->idx[0] == i0 )
            ສົ່ງຄືນ &value<uchar>(elem);
        nidx = elem->next;
    }

    ຖ້າ( createMissing )
    {
        ຈຳນວນເຕັມ idx[] = { i0 };
        ສົ່ງຄືນ newNode( idx, h );
    }
    ສົ່ງຄືນ NULL;
}

uchar* SparseMat::ptr(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ສົ່ງຄືນ &value<uchar>(elem);
        nidx = elem->next;
    }

    ຖ້າ( createMissing )
    {
        ຈຳນວນເຕັມ idx[] = { i0, i1 };
        ສົ່ງຄືນ newNode( idx, h );
    }
    ສົ່ງຄືນ NULL;
}

uchar* SparseMat::ptr(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1, ຈຳນວນເຕັມ i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ສົ່ງຄືນ &value<uchar>(elem);
        nidx = elem->next;
    }

    ຖ້າ( createMissing )
    {
        ຈຳນວນເຕັມ idx[] = { i0, i1, i2 };
        ສົ່ງຄືນ newNode( idx, h );
    }
    ສົ່ງຄືນ NULL;
}

uchar* SparseMat::ptr(const ຈຳນວນເຕັມ* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ຈຳນວນເຕັມ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h )
        {
            ສຳລັບ( i = 0; i < d; i++ )
                ຖ້າ( elem->idx[i] != idx[i] )
                    bha_nga;
            ຖ້າ( i == d )
                ສົ່ງຄືນ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ສົ່ງຄືນ createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ຖ້າ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(ຈຳນວນເຕັມ i0, ຈຳນວນເຕັມ i1, ຈຳນວນເຕັມ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ຖ້າ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const ຈຳນວນເຕັມ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ຈຳນວນເຕັມ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ໃນຂະນະທີ່( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ຖ້າ( elem->hashval == h )
        {
            ສຳລັບ( i = 0; i < d; i++ )
                ຖ້າ( elem->idx[i] != idx[i] )
                    bha_nga;
            ຖ້າ( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ຖ້າ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ຖ້າ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    ສຳລັບ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    ສຳລັບ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ໃນຂະນະທີ່( nidx )
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

uchar* SparseMat::newNode(const ຈຳນວນເຕັມ* idx, size_t hashval)
{
    const ຈຳນວນເຕັມ HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ຖ້າ( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ຖ້າ( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        ສຳລັບ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ຈຳນວນເຕັມ i, d = hdr->dims;
    ສຳລັບ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ຖ້າ( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    ບໍ່ດັ່ງນັ້ນ
        memset(p, 0, esz);

    ສົ່ງຄືນ p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    ຖ້າ( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    ບໍ່ດັ່ງນັ້ນ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, ຈຳນວນເຕັມ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ຈຳນວນເຕັມ type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ຖ້າ( type == CV_32F )
    {
        ຖ້າ( normType == NORM_INF )
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L1 )
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        ບໍ່ດັ່ງນັ້ນ
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == CV_64F )
    {
        ຖ້າ( normType == NORM_INF )
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L1 )
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        ບໍ່ດັ່ງນັ້ນ
            ສຳລັບ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    ບໍ່ດັ່ງນັ້ນ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ຖ້າ( normType == NORM_L2 )
        result = std::sqrt(result);
    ສົ່ງຄືນ result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, ຈຳນວນເຕັມ* _minidx, ຈຳນວນເຕັມ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ຈຳນວນເຕັມ type = src.type();
    const ຈຳນວນເຕັມ *minidx = 0, *maxidx = 0;

    ຖ້າ( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        ສຳລັບ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            ຖ້າ( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ຖ້າ( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ຖ້າ( _minval )
            *_minval = minval;
        ຖ້າ( _maxval )
            *_maxval = maxval;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        ສຳລັບ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            ຖ້າ( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ຖ້າ( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ຖ້າ( _minval )
            *_minval = minval;
        ຖ້າ( _maxval )
            *_maxval = maxval;
    }
    ບໍ່ດັ່ງນັ້ນ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ຖ້າ( _minidx && minidx )
        ສຳລັບ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ຖ້າ( _maxidx && maxidx )
        ສຳລັບ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, ຈຳນວນເຕັມ norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    ຖ້າ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ບໍ່ດັ່ງນັ້ນ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
