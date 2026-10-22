// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, ចំនួនគត់ cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ប្រសិនបើ( cn == 1 )
        *to = saturate_cast<T2>(*from);
    បើមិនដូច្នេះ
        សម្រាប់( ចំនួនគត់ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, ចំនួនគត់ cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ប្រសិនបើ( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    បើមិនដូច្នេះ
        សម្រាប់( ចំនួនគត់ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, ចំនួនគត់ cn);
typedef void (*ConvertScaleData)(const void* from, void* to, ចំនួនគត់ cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(ចំនួនគត់ fromType, ចំនួនគត់ toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ចំនួនគត់>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ចំនួនគត់>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ចំនួនគត់>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ចំនួនគត់>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<ចំនួនគត់, uchar>, convertData_<ចំនួនគត់, schar>,
      convertData_<ចំនួនគត់, ushort>, convertData_<ចំនួនគត់, short>,
      convertData_<ចំនួនគត់, ចំនួនគត់>, convertData_<ចំនួនគត់, float>,
      convertData_<ចំនួនគត់, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, ចំនួនគត់>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, ចំនួនគត់>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ត្រឡប់ func;
}

static ConvertScaleData getConvertScaleElem(ចំនួនគត់ fromType, ចំនួនគត់ toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ចំនួនគត់>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ចំនួនគត់>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ចំនួនគត់>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ចំនួនគត់>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<ចំនួនគត់, uchar>, convertScaleData_<ចំនួនគត់, schar>,
      convertScaleData_<ចំនួនគត់, ushort>, convertScaleData_<ចំនួនគត់, short>,
      convertScaleData_<ចំនួនគត់, ចំនួនគត់>, convertScaleData_<ចំនួនគត់, float>,
      convertScaleData_<ចំនួនគត់, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, ចំនួនគត់>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, ចំនួនគត់>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ត្រឡប់ func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    សម្រាប់( i = 0; i + parimaa_na(ចំនួនគត់) <= elemSize; i += parimaa_na(ចំនួនគត់) )
        *(ចំនួនគត់*)(to + i) = *(const ចំនួនគត់*)(from + i);
    សម្រាប់( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    សម្រាប់( i = 0; i + parimaa_na(ចំនួនគត់) <= elemSize; i += parimaa_na(ចំនួនគត់) )
        ប្រសិនបើ( *(ចំនួនគត់*)(data + i) != 0 )
            ត្រឡប់ false;
    សម្រាប់( ; i < elemSize; i++ )
        ប្រសិនបើ( data[i] != 0 )
            ត្រឡប់ false;
    ត្រឡប់ true;
}

SparseMat::Hdr::Hdr( ចំនួនគត់ _dims, const ចំនួនគត់* _sizes, ចំនួនគត់ _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (ចំនួនគត់)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(ចំនួនគត់) +
                                 dims*parimaa_na(ចំនួនគត់), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (ចំនួនគត់)parimaa_na(size_t));

    ចំនួនគត់ i;
    សម្រាប់( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    សម្រាប់( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ប្រសិនបើ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#ប្រសិនបើ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ចំនួនគត់ _dims, const ចំនួនគត់* _sizes, ចំនួនគត់ _type)
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
    ប្រសិនបើ( this != &m )
    {
        ប្រសិនបើ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ត្រឡប់ *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ត្រឡប់ (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, ចំនួនគត់ _type) const
{
    ប្រសិនបើ( _type < 0 )
        m = *this;
    បើមិនដូច្នេះ
        convertTo(m, _type);
}

void SparseMat::addref()
{
    ប្រសិនបើ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    ប្រសិនបើ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ចំនួនគត់ i0) const
{
    ត្រឡប់ (size_t)i0;
}

size_t SparseMat::hash(ចំនួនគត់ i0, ចំនួនគត់ i1) const
{
    ត្រឡប់ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ចំនួនគត់ i0, ចំនួនគត់ i1, ចំនួនគត់ i2) const
{
    ត្រឡប់ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const ចំនួនគត់* idx) const
{
    size_t h = (unsigned)idx[0];
    ប្រសិនបើ( !hdr )
        ត្រឡប់ 0;
    ចំនួនគត់ d = hdr->dims;
    សម្រាប់(ចំនួនគត់ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ត្រឡប់ h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ចំនួនគត់ i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    សម្រាប់(;;)
    {
        សម្រាប់( i = 0; i < lastSize; i++, dptr += esz )
        {
            ប្រសិនបើ( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        សម្រាប់( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ប្រសិនបើ( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        ប្រសិនបើ( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(ចំនួនគត់ d, const ចំនួនគត់* _sizes, ចំនួនគត់ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    សម្រាប់( ចំនួនគត់ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ប្រសិនបើ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ចំនួនគត់ i;
        សម្រាប់( i = 0; i < d; i++ )
            ប្រសិនបើ( _sizes[i] != hdr->size[i] )
                bha_nga;
        ប្រសិនបើ( i == d )
        {
            clear();
            ត្រឡប់;
        }
    }
    ចំនួនគត់ _sizes_backup[CV_MAX_DIM]; // #5991
    ប្រសិនបើ (hdr && _sizes == hdr->size)
    {
        សម្រាប់(ចំនួនគត់ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    ប្រសិនបើ( hdr == m.hdr )
        ត្រឡប់;
    ប្រសិនបើ( !hdr )
    {
        m.release();
        ត្រឡប់;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    សម្រាប់( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    ចំនួនគត់ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    សម្រាប់( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, ចំនួនគត់ rtype, dviguu_na alpha ) const
{
    ចំនួនគត់ cn = channels();
    ប្រសិនបើ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ប្រសិនបើ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ត្រឡប់;
    }

    CV_Assert(hdr != 0);
    ប្រសិនបើ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ប្រសិនបើ( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        សម្រាប់( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    បើមិនដូច្នេះ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        សម្រាប់( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, ចំនួនគត់ rtype, dviguu_na alpha, dviguu_na beta ) const
{
    ចំនួនគត់ cn = channels();
    ប្រសិនបើ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ប្រសិនបើ( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        សម្រាប់( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    បើមិនដូច្នេះ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        សម្រាប់( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    ប្រសិនបើ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ចំនួនគត់ i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h && elem->idx[0] == i0 )
            ត្រឡប់ &value<uchar>(elem);
        nidx = elem->next;
    }

    ប្រសិនបើ( createMissing )
    {
        ចំនួនគត់ idx[] = { i0 };
        ត្រឡប់ newNode( idx, h );
    }
    ត្រឡប់ NULL;
}

uchar* SparseMat::ptr(ចំនួនគត់ i0, ចំនួនគត់ i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ត្រឡប់ &value<uchar>(elem);
        nidx = elem->next;
    }

    ប្រសិនបើ( createMissing )
    {
        ចំនួនគត់ idx[] = { i0, i1 };
        ត្រឡប់ newNode( idx, h );
    }
    ត្រឡប់ NULL;
}

uchar* SparseMat::ptr(ចំនួនគត់ i0, ចំនួនគត់ i1, ចំនួនគត់ i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ត្រឡប់ &value<uchar>(elem);
        nidx = elem->next;
    }

    ប្រសិនបើ( createMissing )
    {
        ចំនួនគត់ idx[] = { i0, i1, i2 };
        ត្រឡប់ newNode( idx, h );
    }
    ត្រឡប់ NULL;
}

uchar* SparseMat::ptr(const ចំនួនគត់* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ចំនួនគត់ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h )
        {
            សម្រាប់( i = 0; i < d; i++ )
                ប្រសិនបើ( elem->idx[i] != idx[i] )
                    bha_nga;
            ប្រសិនបើ( i == d )
                ត្រឡប់ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ត្រឡប់ createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(ចំនួនគត់ i0, ចំនួនគត់ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ប្រសិនបើ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(ចំនួនគត់ i0, ចំនួនគត់ i1, ចំនួនគត់ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ប្រសិនបើ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const ចំនួនគត់* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ចំនួនគត់ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ខណៈពេល( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ប្រសិនបើ( elem->hashval == h )
        {
            សម្រាប់( i = 0; i < d; i++ )
                ប្រសិនបើ( elem->idx[i] != idx[i] )
                    bha_nga;
            ប្រសិនបើ( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ប្រសិនបើ( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ប្រសិនបើ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    សម្រាប់( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    សម្រាប់( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ខណៈពេល( nidx )
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

uchar* SparseMat::newNode(const ចំនួនគត់* idx, size_t hashval)
{
    const ចំនួនគត់ HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ប្រសិនបើ( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ប្រសិនបើ( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        សម្រាប់( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ចំនួនគត់ i, d = hdr->dims;
    សម្រាប់( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ប្រសិនបើ( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    បើមិនដូច្នេះ ប្រសិនបើ( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    បើមិនដូច្នេះ
        memset(p, 0, esz);

    ត្រឡប់ p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    ប្រសិនបើ( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    បើមិនដូច្នេះ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, ចំនួនគត់ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ចំនួនគត់ type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ប្រសិនបើ( type == CV_32F )
    {
        ប្រសិនបើ( normType == NORM_INF )
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        បើមិនដូច្នេះ ប្រសិនបើ( normType == NORM_L1 )
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        បើមិនដូច្នេះ
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == CV_64F )
    {
        ប្រសិនបើ( normType == NORM_INF )
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        បើមិនដូច្នេះ ប្រសិនបើ( normType == NORM_L1 )
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        បើមិនដូច្នេះ
            សម្រាប់( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    បើមិនដូច្នេះ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ប្រសិនបើ( normType == NORM_L2 )
        result = std::sqrt(result);
    ត្រឡប់ result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, ចំនួនគត់* _minidx, ចំនួនគត់* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ចំនួនគត់ type = src.type();
    const ចំនួនគត់ *minidx = 0, *maxidx = 0;

    ប្រសិនបើ( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        សម្រាប់( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            ប្រសិនបើ( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ប្រសិនបើ( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ប្រសិនបើ( _minval )
            *_minval = minval;
        ប្រសិនបើ( _maxval )
            *_maxval = maxval;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        សម្រាប់( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            ប្រសិនបើ( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ប្រសិនបើ( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ប្រសិនបើ( _minval )
            *_minval = minval;
        ប្រសិនបើ( _maxval )
            *_maxval = maxval;
    }
    បើមិនដូច្នេះ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ប្រសិនបើ( _minidx && minidx )
        សម្រាប់( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ប្រសិនបើ( _maxidx && maxidx )
        សម្រាប់( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, ចំនួនគត់ norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    ប្រសិនបើ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    បើមិនដូច្នេះ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
