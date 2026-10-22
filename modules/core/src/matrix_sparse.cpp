// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, จำนวนเต็ม cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ถ้า( cn == 1 )
        *to = saturate_cast<T2>(*from);
    มิฉะนั้น
        สำหรับ( จำนวนเต็ม i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, จำนวนเต็ม cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ถ้า( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    มิฉะนั้น
        สำหรับ( จำนวนเต็ม i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, จำนวนเต็ม cn);
typedef void (*ConvertScaleData)(const void* from, void* to, จำนวนเต็ม cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(จำนวนเต็ม fromType, จำนวนเต็ม toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, จำนวนเต็ม>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, จำนวนเต็ม>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, จำนวนเต็ม>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, จำนวนเต็ม>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<จำนวนเต็ม, uchar>, convertData_<จำนวนเต็ม, schar>,
      convertData_<จำนวนเต็ม, ushort>, convertData_<จำนวนเต็ม, short>,
      convertData_<จำนวนเต็ม, จำนวนเต็ม>, convertData_<จำนวนเต็ม, float>,
      convertData_<จำนวนเต็ม, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, จำนวนเต็ม>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, จำนวนเต็ม>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    คืน func;
}

static ConvertScaleData getConvertScaleElem(จำนวนเต็ม fromType, จำนวนเต็ม toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, จำนวนเต็ม>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, จำนวนเต็ม>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, จำนวนเต็ม>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, จำนวนเต็ม>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<จำนวนเต็ม, uchar>, convertScaleData_<จำนวนเต็ม, schar>,
      convertScaleData_<จำนวนเต็ม, ushort>, convertScaleData_<จำนวนเต็ม, short>,
      convertScaleData_<จำนวนเต็ม, จำนวนเต็ม>, convertScaleData_<จำนวนเต็ม, float>,
      convertScaleData_<จำนวนเต็ม, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, จำนวนเต็ม>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, จำนวนเต็ม>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    คืน func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    สำหรับ( i = 0; i + parimaa_na(จำนวนเต็ม) <= elemSize; i += parimaa_na(จำนวนเต็ม) )
        *(จำนวนเต็ม*)(to + i) = *(const จำนวนเต็ม*)(from + i);
    สำหรับ( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    สำหรับ( i = 0; i + parimaa_na(จำนวนเต็ม) <= elemSize; i += parimaa_na(จำนวนเต็ม) )
        ถ้า( *(จำนวนเต็ม*)(data + i) != 0 )
            คืน false;
    สำหรับ( ; i < elemSize; i++ )
        ถ้า( data[i] != 0 )
            คืน false;
    คืน true;
}

SparseMat::Hdr::Hdr( จำนวนเต็ม _dims, const จำนวนเต็ม* _sizes, จำนวนเต็ม _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (จำนวนเต็ม)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(จำนวนเต็ม) +
                                 dims*parimaa_na(จำนวนเต็ม), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (จำนวนเต็ม)parimaa_na(size_t));

    จำนวนเต็ม i;
    สำหรับ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    สำหรับ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ถ้า defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#ถ้า defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(จำนวนเต็ม _dims, const จำนวนเต็ม* _sizes, จำนวนเต็ม _type)
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
    ถ้า( this != &m )
    {
        ถ้า( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    คืน *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    คืน (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, จำนวนเต็ม _type) const
{
    ถ้า( _type < 0 )
        m = *this;
    มิฉะนั้น
        convertTo(m, _type);
}

void SparseMat::addref()
{
    ถ้า( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    ถ้า( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(จำนวนเต็ม i0) const
{
    คืน (size_t)i0;
}

size_t SparseMat::hash(จำนวนเต็ม i0, จำนวนเต็ม i1) const
{
    คืน (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(จำนวนเต็ม i0, จำนวนเต็ม i1, จำนวนเต็ม i2) const
{
    คืน ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const จำนวนเต็ม* idx) const
{
    size_t h = (unsigned)idx[0];
    ถ้า( !hdr )
        คืน 0;
    จำนวนเต็ม d = hdr->dims;
    สำหรับ(จำนวนเต็ม i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    คืน h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    จำนวนเต็ม i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    สำหรับ(;;)
    {
        สำหรับ( i = 0; i < lastSize; i++, dptr += esz )
        {
            ถ้า( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        สำหรับ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ถ้า( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        ถ้า( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(จำนวนเต็ม d, const จำนวนเต็ม* _sizes, จำนวนเต็ม _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    สำหรับ( จำนวนเต็ม i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ถ้า( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        จำนวนเต็ม i;
        สำหรับ( i = 0; i < d; i++ )
            ถ้า( _sizes[i] != hdr->size[i] )
                bha_nga;
        ถ้า( i == d )
        {
            clear();
            คืน;
        }
    }
    จำนวนเต็ม _sizes_backup[CV_MAX_DIM]; // #5991
    ถ้า (hdr && _sizes == hdr->size)
    {
        สำหรับ(จำนวนเต็ม i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    ถ้า( hdr == m.hdr )
        คืน;
    ถ้า( !hdr )
    {
        m.release();
        คืน;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    สำหรับ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    จำนวนเต็ม ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    สำหรับ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, จำนวนเต็ม rtype, dviguu_na alpha ) const
{
    จำนวนเต็ม cn = channels();
    ถ้า( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ถ้า( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        คืน;
    }

    CV_Assert(hdr != 0);
    ถ้า( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ถ้า( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        สำหรับ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    มิฉะนั้น
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        สำหรับ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, จำนวนเต็ม rtype, dviguu_na alpha, dviguu_na beta ) const
{
    จำนวนเต็ม cn = channels();
    ถ้า( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ถ้า( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        สำหรับ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    มิฉะนั้น
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        สำหรับ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    ถ้า( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(จำนวนเต็ม i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h && elem->idx[0] == i0 )
            คืน &value<uchar>(elem);
        nidx = elem->next;
    }

    ถ้า( createMissing )
    {
        จำนวนเต็ม idx[] = { i0 };
        คืน newNode( idx, h );
    }
    คืน NULL;
}

uchar* SparseMat::ptr(จำนวนเต็ม i0, จำนวนเต็ม i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            คืน &value<uchar>(elem);
        nidx = elem->next;
    }

    ถ้า( createMissing )
    {
        จำนวนเต็ม idx[] = { i0, i1 };
        คืน newNode( idx, h );
    }
    คืน NULL;
}

uchar* SparseMat::ptr(จำนวนเต็ม i0, จำนวนเต็ม i1, จำนวนเต็ม i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            คืน &value<uchar>(elem);
        nidx = elem->next;
    }

    ถ้า( createMissing )
    {
        จำนวนเต็ม idx[] = { i0, i1, i2 };
        คืน newNode( idx, h );
    }
    คืน NULL;
}

uchar* SparseMat::ptr(const จำนวนเต็ม* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    จำนวนเต็ม i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h )
        {
            สำหรับ( i = 0; i < d; i++ )
                ถ้า( elem->idx[i] != idx[i] )
                    bha_nga;
            ถ้า( i == d )
                คืน &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    คืน createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(จำนวนเต็ม i0, จำนวนเต็ม i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ถ้า( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(จำนวนเต็ม i0, จำนวนเต็ม i1, จำนวนเต็ม i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    ถ้า( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const จำนวนเต็ม* idx, size_t* hashval)
{
    CV_Assert( hdr );
    จำนวนเต็ม i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ขณะ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ถ้า( elem->hashval == h )
        {
            สำหรับ( i = 0; i < d; i++ )
                ถ้า( elem->idx[i] != idx[i] )
                    bha_nga;
            ถ้า( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ถ้า( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ถ้า((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    สำหรับ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    สำหรับ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ขณะ( nidx )
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

uchar* SparseMat::newNode(const จำนวนเต็ม* idx, size_t hashval)
{
    const จำนวนเต็ม HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ถ้า( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ถ้า( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        สำหรับ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    จำนวนเต็ม i, d = hdr->dims;
    สำหรับ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ถ้า( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    มิฉะนั้น ถ้า( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    มิฉะนั้น
        memset(p, 0, esz);

    คืน p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    ถ้า( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    มิฉะนั้น
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, จำนวนเต็ม normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    จำนวนเต็ม type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ถ้า( type == CV_32F )
    {
        ถ้า( normType == NORM_INF )
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        มิฉะนั้น ถ้า( normType == NORM_L1 )
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        มิฉะนั้น
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    มิฉะนั้น ถ้า( type == CV_64F )
    {
        ถ้า( normType == NORM_INF )
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        มิฉะนั้น ถ้า( normType == NORM_L1 )
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        มิฉะนั้น
            สำหรับ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    มิฉะนั้น
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ถ้า( normType == NORM_L2 )
        result = std::sqrt(result);
    คืน result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, จำนวนเต็ม* _minidx, จำนวนเต็ม* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    จำนวนเต็ม type = src.type();
    const จำนวนเต็ม *minidx = 0, *maxidx = 0;

    ถ้า( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        สำหรับ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            ถ้า( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ถ้า( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ถ้า( _minval )
            *_minval = minval;
        ถ้า( _maxval )
            *_maxval = maxval;
    }
    มิฉะนั้น ถ้า( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        สำหรับ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            ถ้า( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ถ้า( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ถ้า( _minval )
            *_minval = minval;
        ถ้า( _maxval )
            *_maxval = maxval;
    }
    มิฉะนั้น
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ถ้า( _minidx && minidx )
        สำหรับ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ถ้า( _maxidx && maxidx )
        สำหรับ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, จำนวนเต็ม norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    ถ้า( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    มิฉะนั้น
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
