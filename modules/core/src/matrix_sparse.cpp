// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, ཧྲིལ cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    གལ་སྲིད( cn == 1 )
        *to = saturate_cast<T2>(*from);
    དེ་མིན
        དོན་ལུ( ཧྲིལ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, ཧྲིལ cn, dviguu_na alpha, dviguu_na beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    གལ་སྲིད( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    དེ་མིན
        དོན་ལུ( ཧྲིལ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, ཧྲིལ cn);
typedef void (*ConvertScaleData)(const void* from, void* to, ཧྲིལ cn, dviguu_na alpha, dviguu_na beta);

static ConvertData getConvertElem(ཧྲིལ fromType, ཧྲིལ toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ཧྲིལ>, convertData_<uchar, float>,
      convertData_<uchar, dviguu_na>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ཧྲིལ>, convertData_<schar, float>,
      convertData_<schar, dviguu_na>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ཧྲིལ>, convertData_<ushort, float>,
      convertData_<ushort, dviguu_na>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ཧྲིལ>, convertData_<short, float>,
      convertData_<short, dviguu_na>, 0 },

    { convertData_<ཧྲིལ, uchar>, convertData_<ཧྲིལ, schar>,
      convertData_<ཧྲིལ, ushort>, convertData_<ཧྲིལ, short>,
      convertData_<ཧྲིལ, ཧྲིལ>, convertData_<ཧྲིལ, float>,
      convertData_<ཧྲིལ, dviguu_na>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, ཧྲིལ>, convertData_<float, float>,
      convertData_<float, dviguu_na>, 0 },

    { convertData_<dviguu_na, uchar>, convertData_<dviguu_na, schar>,
      convertData_<dviguu_na, ushort>, convertData_<dviguu_na, short>,
      convertData_<dviguu_na, ཧྲིལ>, convertData_<dviguu_na, float>,
      convertData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ལོག func;
}

static ConvertScaleData getConvertScaleElem(ཧྲིལ fromType, ཧྲིལ toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ཧྲིལ>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, dviguu_na>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ཧྲིལ>, convertScaleData_<schar, float>,
      convertScaleData_<schar, dviguu_na>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ཧྲིལ>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, dviguu_na>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ཧྲིལ>, convertScaleData_<short, float>,
      convertScaleData_<short, dviguu_na>, 0 },

    { convertScaleData_<ཧྲིལ, uchar>, convertScaleData_<ཧྲིལ, schar>,
      convertScaleData_<ཧྲིལ, ushort>, convertScaleData_<ཧྲིལ, short>,
      convertScaleData_<ཧྲིལ, ཧྲིལ>, convertScaleData_<ཧྲིལ, float>,
      convertScaleData_<ཧྲིལ, dviguu_na>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, ཧྲིལ>, convertScaleData_<float, float>,
      convertScaleData_<float, dviguu_na>, 0 },

    { convertScaleData_<dviguu_na, uchar>, convertScaleData_<dviguu_na, schar>,
      convertScaleData_<dviguu_na, ushort>, convertScaleData_<dviguu_na, short>,
      convertScaleData_<dviguu_na, ཧྲིལ>, convertScaleData_<dviguu_na, float>,
      convertScaleData_<dviguu_na, dviguu_na>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ལོག func;
}

ga_nanaa { HASH_SIZE0 = 8 };

static anta_hstha void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    དོན་ལུ( i = 0; i + parimaa_na(ཧྲིལ) <= elemSize; i += parimaa_na(ཧྲིལ) )
        *(ཧྲིལ*)(to + i) = *(const ཧྲིལ*)(from + i);
    དོན་ལུ( ; i < elemSize; i++ )
        to[i] = from[i];
}

static anta_hstha bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    དོན་ལུ( i = 0; i + parimaa_na(ཧྲིལ) <= elemSize; i += parimaa_na(ཧྲིལ) )
        གལ་སྲིད( *(ཧྲིལ*)(data + i) != 0 )
            ལོག false;
    དོན་ལུ( ; i < elemSize; i++ )
        གལ་སྲིད( data[i] != 0 )
            ལོག false;
    ལོག true;
}

SparseMat::Hdr::Hdr( ཧྲིལ _dims, const ཧྲིལ* _sizes, ཧྲིལ _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (ཧྲིལ)alignSize(parimaa_na(SparseMat::Node) - MAX_DIM*parimaa_na(ཧྲིལ) +
                                 dims*parimaa_na(ཧྲིལ), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (ཧྲིལ)parimaa_na(size_t));

    ཧྲིལ i;
    དོན་ལུ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    དོན་ལུ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#གལ་སྲིད defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#གལ་སྲིད defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ཧྲིལ _dims, const ཧྲིལ* _sizes, ཧྲིལ _type)
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
    གལ་སྲིད( this != &m )
    {
        གལ་སྲིད( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ལོག *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    ལོག (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, ཧྲིལ _type) const
{
    གལ་སྲིད( _type < 0 )
        m = *this;
    དེ་མིན
        convertTo(m, _type);
}

void SparseMat::addref()
{
    གལ་སྲིད( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    གལ་སྲིད( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ཧྲིལ i0) const
{
    ལོག (size_t)i0;
}

size_t SparseMat::hash(ཧྲིལ i0, ཧྲིལ i1) const
{
    ལོག (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ཧྲིལ i0, ཧྲིལ i1, ཧྲིལ i2) const
{
    ལོག ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const ཧྲིལ* idx) const
{
    size_t h = (unsigned)idx[0];
    གལ་སྲིད( !hdr )
        ལོག 0;
    ཧྲིལ d = hdr->dims;
    དོན་ལུ(ཧྲིལ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ལོག h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ཧྲིལ i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    དོན་ལུ(;;)
    {
        དོན་ལུ( i = 0; i < lastSize; i++, dptr += esz )
        {
            གལ་སྲིད( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        དོན་ལུ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            གལ་སྲིད( ++idx[i] < m.size[i] )
                bha_nga;
            idx[i] = 0;
        }
        གལ་སྲིད( i < 0 )
            bha_nga;
    }
}

void SparseMat::create(ཧྲིལ d, const ཧྲིལ* _sizes, ཧྲིལ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    དོན་ལུ( ཧྲིལ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    གལ་སྲིད( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ཧྲིལ i;
        དོན་ལུ( i = 0; i < d; i++ )
            གལ་སྲིད( _sizes[i] != hdr->size[i] )
                bha_nga;
        གལ་སྲིད( i == d )
        {
            clear();
            ལོག;
        }
    }
    ཧྲིལ _sizes_backup[CV_MAX_DIM]; // #5991
    གལ་སྲིད (hdr && _sizes == hdr->size)
    {
        དོན་ལུ(ཧྲིལ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    གལ་སྲིད( hdr == m.hdr )
        ལོག;
    གལ་སྲིད( !hdr )
    {
        m.release();
        ལོག;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    དོན་ལུ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    ཧྲིལ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    དོན་ལུ( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, ཧྲིལ rtype, dviguu_na alpha ) const
{
    ཧྲིལ cn = channels();
    གལ་སྲིད( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    གལ་སྲིད( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ལོག;
    }

    CV_Assert(hdr != 0);
    གལ་སྲིད( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    གལ་སྲིད( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        དོན་ལུ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    དེ་མིན
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        དོན་ལུ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, ཧྲིལ rtype, dviguu_na alpha, dviguu_na beta ) const
{
    ཧྲིལ cn = channels();
    གལ་སྲིད( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    གལ་སྲིད( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        དོན་ལུ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    དེ་མིན
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        དོན་ལུ( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    གལ་སྲིད( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ཧྲིལ i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h && elem->idx[0] == i0 )
            ལོག &value<uchar>(elem);
        nidx = elem->next;
    }

    གལ་སྲིད( createMissing )
    {
        ཧྲིལ idx[] = { i0 };
        ལོག newNode( idx, h );
    }
    ལོག NULL;
}

uchar* SparseMat::ptr(ཧྲིལ i0, ཧྲིལ i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ལོག &value<uchar>(elem);
        nidx = elem->next;
    }

    གལ་སྲིད( createMissing )
    {
        ཧྲིལ idx[] = { i0, i1 };
        ལོག newNode( idx, h );
    }
    ལོག NULL;
}

uchar* SparseMat::ptr(ཧྲིལ i0, ཧྲིལ i1, ཧྲིལ i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ལོག &value<uchar>(elem);
        nidx = elem->next;
    }

    གལ་སྲིད( createMissing )
    {
        ཧྲིལ idx[] = { i0, i1, i2 };
        ལོག newNode( idx, h );
    }
    ལོག NULL;
}

uchar* SparseMat::ptr(const ཧྲིལ* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ཧྲིལ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h )
        {
            དོན་ལུ( i = 0; i < d; i++ )
                གལ་སྲིད( elem->idx[i] != idx[i] )
                    bha_nga;
            གལ་སྲིད( i == d )
                ལོག &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ལོག createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(ཧྲིལ i0, ཧྲིལ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    གལ་སྲིད( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(ཧྲིལ i0, ཧྲིལ i1, ཧྲིལ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            bha_nga;
        previdx = nidx;
        nidx = elem->next;
    }

    གལ་སྲིད( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const ཧྲིལ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ཧྲིལ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    སྐབས( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        གལ་སྲིད( elem->hashval == h )
        {
            དོན་ལུ( i = 0; i < d; i++ )
                གལ་སྲིད( elem->idx[i] != idx[i] )
                    bha_nga;
            གལ་སྲིད( i == d )
                bha_nga;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    གལ་སྲིད( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    གལ་སྲིད((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((dviguu_na)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    དོན་ལུ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    དོན་ལུ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        སྐབས( nidx )
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

uchar* SparseMat::newNode(const ཧྲིལ* idx, size_t hashval)
{
    const ཧྲིལ HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    གལ་སྲིད( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    གལ་སྲིད( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        དོན་ལུ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ཧྲིལ i, d = hdr->dims;
    དོན་ལུ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    གལ་སྲིད( esz == parimaa_na(float) )
        *((float*)p) = 0.f;
    དེ་མིན གལ་སྲིད( esz == parimaa_na(dviguu_na) )
        *((dviguu_na*)p) = 0.;
    དེ་མིན
        memset(p, 0, esz);

    ལོག p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    གལ་སྲིད( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    དེ་མིན
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
dviguu_na norm( const SparseMat& src, ཧྲིལ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ཧྲིལ type = src.type();
    dviguu_na result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    གལ་སྲིད( type == CV_32F )
    {
        གལ་སྲིད( normType == NORM_INF )
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((dviguu_na)it.value<float>()));
            }
        དེ་མིན གལ་སྲིད( normType == NORM_L1 )
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        དེ་མིན
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<float>();
                result += v*v;
            }
    }
    དེ་མིན གལ་སྲིད( type == CV_64F )
    {
        གལ་སྲིད( normType == NORM_INF )
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<dviguu_na>()));
            }
        དེ་མིན གལ་སྲིད( normType == NORM_L1 )
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<dviguu_na>());
            }
        དེ་མིན
            དོན་ལུ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                dviguu_na v = it.value<dviguu_na>();
                result += v*v;
            }
    }
    དེ་མིན
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    གལ་སྲིད( normType == NORM_L2 )
        result = std::sqrt(result);
    ལོག result;
}

void minMaxLoc( const SparseMat& src, dviguu_na* _minval, dviguu_na* _maxval, ཧྲིལ* _minidx, ཧྲིལ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ཧྲིལ type = src.type();
    const ཧྲིལ *minidx = 0, *maxidx = 0;

    གལ་སྲིད( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        དོན་ལུ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            གལ་སྲིད( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            གལ་སྲིད( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        གལ་སྲིད( _minval )
            *_minval = minval;
        གལ་སྲིད( _maxval )
            *_maxval = maxval;
    }
    དེ་མིན གལ་སྲིད( type == CV_64F )
    {
        dviguu_na minval = DBL_MAX, maxval = -DBL_MAX;
        དོན་ལུ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            dviguu_na v = it.value<dviguu_na>();
            གལ་སྲིད( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            གལ་སྲིད( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        གལ་སྲིད( _minval )
            *_minval = minval;
        གལ་སྲིད( _maxval )
            *_maxval = maxval;
    }
    དེ་མིན
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    གལ་སྲིད( _minidx && minidx )
        དོན་ལུ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    གལ་སྲིད( _maxidx && maxidx )
        དོན་ལུ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, dviguu_na a, ཧྲིལ norm_type )
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1;
    གལ་སྲིད( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    དེ་མིན
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
