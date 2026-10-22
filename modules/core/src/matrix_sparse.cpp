// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, ကိန်းပြည့် cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    အကယ်၍( cn == 1 )
        *to = saturate_cast<T2>(*from);
    မဟုတ်ပါက
        အတွက်( ကိန်းပြည့် i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, ကိန်းပြည့် cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    အကယ်၍( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    မဟုတ်ပါက
        အတွက်( ကိန်းပြည့် i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, ကိန်းပြည့် cn);
typedef void (*ConvertScaleData)(const void* from, void* to, ကိန်းပြည့် cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(ကိန်းပြည့် fromType, ကိန်းပြည့် toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ကိန်းပြည့်>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ကိန်းပြည့်>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ကိန်းပြည့်>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ကိန်းပြည့်>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<ကိန်းပြည့်, uchar>, convertData_<ကိန်းပြည့်, schar>,
      convertData_<ကိန်းပြည့်, ushort>, convertData_<ကိန်းပြည့်, short>,
      convertData_<ကိန်းပြည့်, ကိန်းပြည့်>, convertData_<ကိန်းပြည့်, float>,
      convertData_<ကိန်းပြည့်, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, ကိန်းပြည့်>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, ကိန်းပြည့်>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ပြန်ပေး func;
}

static ConvertScaleData getConvertScaleElem(ကိန်းပြည့် fromType, ကိန်းပြည့် toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ကိန်းပြည့်>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ကိန်းပြည့်>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ကိန်းပြည့်>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ကိန်းပြည့်>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<ကိန်းပြည့်, uchar>, convertScaleData_<ကိန်းပြည့်, schar>,
      convertScaleData_<ကိန်းပြည့်, ushort>, convertScaleData_<ကိန်းပြည့်, short>,
      convertScaleData_<ကိန်းပြည့်, ကိန်းပြည့်>, convertScaleData_<ကိန်းပြည့်, float>,
      convertScaleData_<ကိန်းပြည့်, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, ကိန်းပြည့်>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, ကိန်းပြည့်>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ပြန်ပေး func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    အတွက်( i = 0; i + parimaa_na(ကိန်းပြည့်) <= elemSize; i += parimaa_na(ကိန်းပြည့်) )
        *(ကိန်းပြည့်*)(to + i) = *(const ကိန်းပြည့်*)(from + i);
    အတွက်( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    အတွက်( i = 0; i + parimaa_na(ကိန်းပြည့်) <= elemSize; i += parimaa_na(ကိန်းပြည့်) )
        အကယ်၍( *(ကိန်းပြည့်*)(data + i) != 0 )
            ပြန်ပေး false;
    အတွက်( ; i < elemSize; i++ )
        အကယ်၍( data[i] != 0 )
            ပြန်ပေး false;
    ပြန်ပေး true;
}

SparseMat::Hdr::Hdr( ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sizes, ကိန်းပြည့် _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (ကိန်းပြည့်)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(ကိန်းပြည့်) +
                                 dims*parimaa_na(ကိန်းပြည့်), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (ကိန်းပြည့်)parimaa_na(size_t));

    ကိန်းပြည့် i;
    အတွက်( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    အတွက်( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#အကယ်၍ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#အကယ်၍ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sizes, ကိန်းပြည့် _type)
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
    အကယ်၍( this != &m )
    {
        အကယ်၍( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ပြန်ပေး *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ပြန်ပေး (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, ကိန်းပြည့် _type) const
{
    အကယ်၍( _type < 0 )
        m = *this;
    မဟုတ်ပါက
        convertTo(m, _type);
}

void SparseMat::addref()
{
    အကယ်၍( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    အကယ်၍( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ကိန်းပြည့် i0) const
{
    ပြန်ပေး (size_t)i0;
}

size_t SparseMat::hash(ကိန်းပြည့် i0, ကိန်းပြည့် i1) const
{
    ပြန်ပေး (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ကိန်းပြည့် i0, ကိန်းပြည့် i1, ကိန်းပြည့် i2) const
{
    ပြန်ပေး ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const ကိန်းပြည့်* idx) const
{
    size_t h = (unsigned)idx[0];
    အကယ်၍( !hdr )
        ပြန်ပေး 0;
    ကိန်းပြည့် d = hdr->dims;
    အတွက်(ကိန်းပြည့် i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ပြန်ပေး h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ကိန်းပြည့် i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    အတွက်(;;)
    {
        အတွက်( i = 0; i < lastSize; i++, dptr += esz )
        {
            အကယ်၍( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        အတွက်( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            အကယ်၍( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        အကယ်၍( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(ကိန်းပြည့် d, const ကိန်းပြည့်* _sizes, ကိန်းပြည့် _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    အတွက်( ကိန်းပြည့် i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    အကယ်၍( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ကိန်းပြည့် i;
        အတွက်( i = 0; i < d; i++ )
            အကယ်၍( _sizes[i] != hdr->size[i] )
                bha_nga;
        အကယ်၍( i == d )
        {
            clear();
            ပြန်ပေး;
        }
    }
    ကိန်းပြည့် _sizes_backup[CV_MAX_DIM]; // #5991
    အကယ်၍ (hdr && _sizes == hdr->size)
    {
        အတွက်(ကိန်းပြည့် i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    အကယ်၍( hdr == m.hdr )
        ပြန်ပေး;
    အကယ်၍( !hdr )
    {
        m.release();
        ပြန်ပေး;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    အတွက်( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    ကိန်းပြည့် ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    အတွက်( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, ကိန်းပြည့် rtype, dviguu_na alpha ) const
{
    ကိန်းပြည့် cn = channels();
    အကယ်၍( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    အကယ်၍( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ပြန်ပေး;
    }

    CV_Assert(hdr != 0);
    အကယ်၍( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    အကယ်၍( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        အတွက်( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    မဟုတ်ပါက
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        အတွက်( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, ကိန်းပြည့် rtype, dviguu_na alpha, dviguu_na beta ) const
{
    ကိန်းပြည့် cn = channels();
    အကယ်၍( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    အကယ်၍( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        အတွက်( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    မဟုတ်ပါက
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        အတွက်( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    အကယ်၍( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ကိန်းပြည့် i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h && elem->idx[0] == i0 )
            ပြန်ပေး &value<uchar>(elem);
        nidx = elem->next;
    }

    အကယ်၍( createMissing )
    {
        ကိန်းပြည့် idx[] = { i0 };
        ပြန်ပေး newNode( idx, h );
    }
    ပြန်ပေး NULL;
}

uchar* SparseMat::ptr(ကိန်းပြည့် i0, ကိန်းပြည့် i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ပြန်ပေး &value<uchar>(elem);
        nidx = elem->next;
    }

    အကယ်၍( createMissing )
    {
        ကိန်းပြည့် idx[] = { i0, i1 };
        ပြန်ပေး newNode( idx, h );
    }
    ပြန်ပေး NULL;
}

uchar* SparseMat::ptr(ကိန်းပြည့် i0, ကိန်းပြည့် i1, ကိန်းပြည့် i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ပြန်ပေး &value<uchar>(elem);
        nidx = elem->next;
    }

    အကယ်၍( createMissing )
    {
        ကိန်းပြည့် idx[] = { i0, i1, i2 };
        ပြန်ပေး newNode( idx, h );
    }
    ပြန်ပေး NULL;
}

uchar* SparseMat::ptr(const ကိန်းပြည့်* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ကိန်းပြည့် i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h )
        {
            အတွက်( i = 0; i < d; i++ )
                အကယ်၍( elem->idx[i] != idx[i] )
                    bha_nga;
            အကယ်၍( i == d )
                ပြန်ပေး &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ပြန်ပေး createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(ကိန်းပြည့် i0, ကိန်းပြည့် i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    အကယ်၍( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(ကိန်းပြည့် i0, ကိန်းပြည့် i1, ကိန်းပြည့် i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    အကယ်၍( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const ကိန်းပြည့်* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ကိန်းပြည့် i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    နေစဉ်( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        အကယ်၍( elem->hashval == h )
        {
            အတွက်( i = 0; i < d; i++ )
                အကယ်၍( elem->idx[i] != idx[i] )
                    bha_nga;
            အကယ်၍( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    အကယ်၍( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    အကယ်၍((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    အတွက်( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    အတွက်( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        နေစဉ်( nidx )
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

uchar* SparseMat::newNode(const ကိန်းပြည့်* idx, size_t hashval)
{
    const ကိန်းပြည့် HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    အကယ်၍( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    အကယ်၍( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        အတွက်( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ကိန်းပြည့် i, d = hdr->dims;
    အတွက်( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    အကယ်၍( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    မဟုတ်ပါက အကယ်၍( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    မဟုတ်ပါက
        memset(p, 0, esz);

    ပြန်ပေး p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    အကယ်၍( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    မဟုတ်ပါက
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, ကိန်းပြည့် normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ကိန်းပြည့် type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    အကယ်၍( type == CV_32F )
    {
        အကယ်၍( normType == NORM_INF )
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        မဟုတ်ပါက အကယ်၍( normType == NORM_L1 )
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        မဟုတ်ပါက
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    မဟုတ်ပါက အကယ်၍( type == CV_64F )
    {
        အကယ်၍( normType == NORM_INF )
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        မဟုတ်ပါက အကယ်၍( normType == NORM_L1 )
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        မဟုတ်ပါက
            အတွက်( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    မဟုတ်ပါက
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    အကယ်၍( normType == NORM_L2 )
        result = std::sqrt(result);
    ပြန်ပေး result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, ကိန်းပြည့်* _minidx, ကိန်းပြည့်* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ကိန်းပြည့် type = src.type();
    const ကိန်းပြည့် *minidx = 0, *maxidx = 0;

    အကယ်၍( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        အတွက်( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            အကယ်၍( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            အကယ်၍( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        အကယ်၍( _minval )
            *_minval = minval;
        အကယ်၍( _maxval )
            *_maxval = maxval;
    }
    မဟုတ်ပါက အကယ်၍( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        အတွက်( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            အကယ်၍( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            အကယ်၍( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        အကယ်၍( _minval )
            *_minval = minval;
        အကယ်၍( _maxval )
            *_maxval = maxval;
    }
    မဟုတ်ပါက
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    အကယ်၍( _minidx && minidx )
        အတွက်( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    အကယ်၍( _maxidx && maxidx )
        အတွက်( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, ကိန်းပြည့် norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    အကယ်၍( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    မဟုတ်ပါက
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
