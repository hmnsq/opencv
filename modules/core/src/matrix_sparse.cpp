// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, පූර්ණ_සංඛ්‍යා cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    නම්( cn == 1 )
        *to = saturate_cast<T2>(*from);
    නැතහොත්
        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, පූර්ණ_සංඛ්‍යා cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    නම්( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    නැතහොත්
        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, පූර්ණ_සංඛ්‍යා cn);
typedef void (*ConvertScaleData)(const void* from, void* to, පූර්ණ_සංඛ්‍යා cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(පූර්ණ_සංඛ්‍යා fromType, පූර්ණ_සංඛ්‍යා toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, පූර්ණ_සංඛ්‍යා>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, පූර්ණ_සංඛ්‍යා>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, පූර්ණ_සංඛ්‍යා>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, පූර්ණ_සංඛ්‍යා>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<පූර්ණ_සංඛ්‍යා, uchar>, convertData_<පූර්ණ_සංඛ්‍යා, schar>,
      convertData_<පූර්ණ_සංඛ්‍යා, ushort>, convertData_<පූර්ණ_සංඛ්‍යා, short>,
      convertData_<පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා>, convertData_<පූර්ණ_සංඛ්‍යා, float>,
      convertData_<පූර්ණ_සංඛ්‍යා, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, පූර්ණ_සංඛ්‍යා>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, පූර්ණ_සංඛ්‍යා>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ආපසු func;
}

static ConvertScaleData getConvertScaleElem(පූර්ණ_සංඛ්‍යා fromType, පූර්ණ_සංඛ්‍යා toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, පූර්ණ_සංඛ්‍යා>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, පූර්ණ_සංඛ්‍යා>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, පූර්ණ_සංඛ්‍යා>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, පූර්ණ_සංඛ්‍යා>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<පූර්ණ_සංඛ්‍යා, uchar>, convertScaleData_<පූර්ණ_සංඛ්‍යා, schar>,
      convertScaleData_<පූර්ණ_සංඛ්‍යා, ushort>, convertScaleData_<පූර්ණ_සංඛ්‍යා, short>,
      convertScaleData_<පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා>, convertScaleData_<පූර්ණ_සංඛ්‍යා, float>,
      convertScaleData_<පූර්ණ_සංඛ්‍යා, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, පූර්ණ_සංඛ්‍යා>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, පූර්ණ_සංඛ්‍යා>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ආපසු func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    සඳහා( i = 0; i + parimaa_na(පූර්ණ_සංඛ්‍යා) <= elemSize; i += parimaa_na(පූර්ණ_සංඛ්‍යා) )
        *(පූර්ණ_සංඛ්‍යා*)(to + i) = *(const පූර්ණ_සංඛ්‍යා*)(from + i);
    සඳහා( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    සඳහා( i = 0; i + parimaa_na(පූර්ණ_සංඛ්‍යා) <= elemSize; i += parimaa_na(පූර්ණ_සංඛ්‍යා) )
        නම්( *(පූර්ණ_සංඛ්‍යා*)(data + i) != 0 )
            ආපසු false;
    සඳහා( ; i < elemSize; i++ )
        නම්( data[i] != 0 )
            ආපසු false;
    ආපසු true;
}

SparseMat::Hdr::Hdr( පූර්ණ_සංඛ්‍යා _dims, const පූර්ණ_සංඛ්‍යා* _sizes, පූර්ණ_සංඛ්‍යා _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (පූර්ණ_සංඛ්‍යා)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(පූර්ණ_සංඛ්‍යා) +
                                 dims*parimaa_na(පූර්ණ_සංඛ්‍යා), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (පූර්ණ_සංඛ්‍යා)parimaa_na(size_t));

    පූර්ණ_සංඛ්‍යා i;
    සඳහා( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    සඳහා( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#නම් defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#නම් defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(පූර්ණ_සංඛ්‍යා _dims, const පූර්ණ_සංඛ්‍යා* _sizes, පූර්ණ_සංඛ්‍යා _type)
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
    නම්( this != &m )
    {
        නම්( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ආපසු *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ආපසු (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, පූර්ණ_සංඛ්‍යා _type) const
{
    නම්( _type < 0 )
        m = *this;
    නැතහොත්
        convertTo(m, _type);
}

void SparseMat::addref()
{
    නම්( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    නම්( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(පූර්ණ_සංඛ්‍යා i0) const
{
    ආපසු (size_t)i0;
}

size_t SparseMat::hash(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1) const
{
    ආපසු (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1, පූර්ණ_සංඛ්‍යා i2) const
{
    ආපසු ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const පූර්ණ_සංඛ්‍යා* idx) const
{
    size_t h = (unsigned)idx[0];
    නම්( !hdr )
        ආපසු 0;
    පූර්ණ_සංඛ්‍යා d = hdr->dims;
    සඳහා(පූර්ණ_සංඛ්‍යා i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ආපසු h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    පූර්ණ_සංඛ්‍යා i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    සඳහා(;;)
    {
        සඳහා( i = 0; i < lastSize; i++, dptr += esz )
        {
            නම්( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        සඳහා( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            නම්( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        නම්( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(පූර්ණ_සංඛ්‍යා d, const පූර්ණ_සංඛ්‍යා* _sizes, පූර්ණ_සංඛ්‍යා _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    නම්( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        පූර්ණ_සංඛ්‍යා i;
        සඳහා( i = 0; i < d; i++ )
            නම්( _sizes[i] != hdr->size[i] )
                bha_nga;
        නම්( i == d )
        {
            clear();
            ආපසු;
        }
    }
    පූර්ණ_සංඛ්‍යා _sizes_backup[CV_MAX_DIM]; // #5991
    නම් (hdr && _sizes == hdr->size)
    {
        සඳහා(පූර්ණ_සංඛ්‍යා i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    නම්( hdr == m.hdr )
        ආපසු;
    නම්( !hdr )
    {
        m.release();
        ආපසු;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    සඳහා( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    පූර්ණ_සංඛ්‍යා ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    සඳහා( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, පූර්ණ_සංඛ්‍යා rtype, dviguu_na alpha ) const
{
    පූර්ණ_සංඛ්‍යා cn = channels();
    නම්( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    නම්( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ආපසු;
    }

    CV_Assert(hdr != 0);
    නම්( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    නම්( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        සඳහා( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    නැතහොත්
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        සඳහා( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, පූර්ණ_සංඛ්‍යා rtype, dviguu_na alpha, dviguu_na beta ) const
{
    පූර්ණ_සංඛ්‍යා cn = channels();
    නම්( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    නම්( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        සඳහා( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    නැතහොත්
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        සඳහා( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    නම්( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(පූර්ණ_සංඛ්‍යා i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h && elem->idx[0] == i0 )
            ආපසු &value<uchar>(elem);
        nidx = elem->next;
    }

    නම්( createMissing )
    {
        පූර්ණ_සංඛ්‍යා idx[] = { i0 };
        ආපසු newNode( idx, h );
    }
    ආපසු NULL;
}

uchar* SparseMat::ptr(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ආපසු &value<uchar>(elem);
        nidx = elem->next;
    }

    නම්( createMissing )
    {
        පූර්ණ_සංඛ්‍යා idx[] = { i0, i1 };
        ආපසු newNode( idx, h );
    }
    ආපසු NULL;
}

uchar* SparseMat::ptr(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1, පූර්ණ_සංඛ්‍යා i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ආපසු &value<uchar>(elem);
        nidx = elem->next;
    }

    නම්( createMissing )
    {
        පූර්ණ_සංඛ්‍යා idx[] = { i0, i1, i2 };
        ආපසු newNode( idx, h );
    }
    ආපසු NULL;
}

uchar* SparseMat::ptr(const පූර්ණ_සංඛ්‍යා* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    පූර්ණ_සංඛ්‍යා i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h )
        {
            සඳහා( i = 0; i < d; i++ )
                නම්( elem->idx[i] != idx[i] )
                    bha_nga;
            නම්( i == d )
                ආපසු &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ආපසු createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    නම්( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(පූර්ණ_සංඛ්‍යා i0, පූර්ණ_සංඛ්‍යා i1, පූර්ණ_සංඛ්‍යා i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    නම්( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const පූර්ණ_සංඛ්‍යා* idx, size_t* hashval)
{
    CV_Assert( hdr );
    පූර්ණ_සංඛ්‍යා i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    අතරතුර( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        නම්( elem->hashval == h )
        {
            සඳහා( i = 0; i < d; i++ )
                නම්( elem->idx[i] != idx[i] )
                    bha_nga;
            නම්( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    නම්( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    නම්((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    සඳහා( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    සඳහා( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        අතරතුර( nidx )
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

uchar* SparseMat::newNode(const පූර්ණ_සංඛ්‍යා* idx, size_t hashval)
{
    const පූර්ණ_සංඛ්‍යා HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    නම්( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    නම්( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        සඳහා( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    පූර්ණ_සංඛ්‍යා i, d = hdr->dims;
    සඳහා( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    නම්( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    නැතහොත් නම්( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    නැතහොත්
        memset(p, 0, esz);

    ආපසු p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    නම්( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    නැතහොත්
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, පූර්ණ_සංඛ්‍යා normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    පූර්ණ_සංඛ්‍යා type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    නම්( type == CV_32F )
    {
        නම්( normType == NORM_INF )
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        නැතහොත් නම්( normType == NORM_L1 )
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        නැතහොත්
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    නැතහොත් නම්( type == CV_64F )
    {
        නම්( normType == NORM_INF )
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        නැතහොත් නම්( normType == NORM_L1 )
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        නැතහොත්
            සඳහා( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    නැතහොත්
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    නම්( normType == NORM_L2 )
        result = std::sqrt(result);
    ආපසු result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, පූර්ණ_සංඛ්‍යා* _minidx, පූර්ණ_සංඛ්‍යා* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    පූර්ණ_සංඛ්‍යා type = src.type();
    const පූර්ණ_සංඛ්‍යා *minidx = 0, *maxidx = 0;

    නම්( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        සඳහා( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            නම්( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            නම්( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        නම්( _minval )
            *_minval = minval;
        නම්( _maxval )
            *_maxval = maxval;
    }
    නැතහොත් නම්( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        සඳහා( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            නම්( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            නම්( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        නම්( _minval )
            *_minval = minval;
        නම්( _maxval )
            *_maxval = maxval;
    }
    නැතහොත්
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    නම්( _minidx && minidx )
        සඳහා( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    නම්( _maxidx && maxidx )
        සඳහා( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, පූර්ණ_සංඛ්‍යා norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    නම්( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    නැතහොත්
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
