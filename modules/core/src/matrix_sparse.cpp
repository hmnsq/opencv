// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, 정수 cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    만약( cn == 1 )
        *to = saturate_cast<T2>(*from);
    아니면
        위해( 정수 i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, 정수 cn, double alpha, double beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    만약( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    아니면
        위해( 정수 i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, 정수 cn);
typedef void (*ConvertScaleData)(const void* from, void* to, 정수 cn, double alpha, double beta);

static ConvertData getConvertElem(정수 fromType, 정수 toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, 정수>, convertData_<uchar, float>,
      convertData_<uchar, double>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, 정수>, convertData_<schar, float>,
      convertData_<schar, double>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, 정수>, convertData_<ushort, float>,
      convertData_<ushort, double>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, 정수>, convertData_<short, float>,
      convertData_<short, double>, 0 },

    { convertData_<정수, uchar>, convertData_<정수, schar>,
      convertData_<정수, ushort>, convertData_<정수, short>,
      convertData_<정수, 정수>, convertData_<정수, float>,
      convertData_<정수, double>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, 정수>, convertData_<float, float>,
      convertData_<float, double>, 0 },

    { convertData_<double, uchar>, convertData_<double, schar>,
      convertData_<double, ushort>, convertData_<double, short>,
      convertData_<double, 정수>, convertData_<double, float>,
      convertData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    반환 func;
}

static ConvertScaleData getConvertScaleElem(정수 fromType, 정수 toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, 정수>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, double>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, 정수>, convertScaleData_<schar, float>,
      convertScaleData_<schar, double>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, 정수>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, double>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, 정수>, convertScaleData_<short, float>,
      convertScaleData_<short, double>, 0 },

    { convertScaleData_<정수, uchar>, convertScaleData_<정수, schar>,
      convertScaleData_<정수, ushort>, convertScaleData_<정수, short>,
      convertScaleData_<정수, 정수>, convertScaleData_<정수, float>,
      convertScaleData_<정수, double>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, 정수>, convertScaleData_<float, float>,
      convertScaleData_<float, double>, 0 },

    { convertScaleData_<double, uchar>, convertScaleData_<double, schar>,
      convertScaleData_<double, ushort>, convertScaleData_<double, short>,
      convertScaleData_<double, 정수>, convertScaleData_<double, float>,
      convertScaleData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    반환 func;
}

enum { HASH_SIZE0 = 8 };

static inline void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    위해( i = 0; i + sizeof(정수) <= elemSize; i += sizeof(정수) )
        *(정수*)(to + i) = *(const 정수*)(from + i);
    위해( ; i < elemSize; i++ )
        to[i] = from[i];
}

static inline bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    위해( i = 0; i + sizeof(정수) <= elemSize; i += sizeof(정수) )
        만약( *(정수*)(data + i) != 0 )
            반환 false;
    위해( ; i < elemSize; i++ )
        만약( data[i] != 0 )
            반환 false;
    반환 true;
}

SparseMat::Hdr::Hdr( 정수 _dims, const 정수* _sizes, 정수 _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (정수)alignSize(sizeof(SparseMat::Node) - MAX_DIM*sizeof(정수) +
                                 dims*sizeof(정수), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (정수)sizeof(size_t));

    정수 i;
    위해( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    위해( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#만약 defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#만약 defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(정수 _dims, const 정수* _sizes, 정수 _type)
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
    만약( this != &m )
    {
        만약( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    반환 *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    반환 (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, 정수 _type) const
{
    만약( _type < 0 )
        m = *this;
    아니면
        convertTo(m, _type);
}

void SparseMat::addref()
{
    만약( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    만약( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(정수 i0) const
{
    반환 (size_t)i0;
}

size_t SparseMat::hash(정수 i0, 정수 i1) const
{
    반환 (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(정수 i0, 정수 i1, 정수 i2) const
{
    반환 ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const 정수* idx) const
{
    size_t h = (unsigned)idx[0];
    만약( !hdr )
        반환 0;
    정수 d = hdr->dims;
    위해(정수 i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    반환 h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    정수 i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    위해(;;)
    {
        위해( i = 0; i < lastSize; i++, dptr += esz )
        {
            만약( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        위해( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            만약( ++idx[i] < m.size[i] )
                break;
            idx[i] = 0;
        }
        만약( i < 0 )
            break;
    }
}

void SparseMat::create(정수 d, const 정수* _sizes, 정수 _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    위해( 정수 i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    만약( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        정수 i;
        위해( i = 0; i < d; i++ )
            만약( _sizes[i] != hdr->size[i] )
                break;
        만약( i == d )
        {
            clear();
            반환;
        }
    }
    정수 _sizes_backup[CV_MAX_DIM]; // #5991
    만약 (hdr && _sizes == hdr->size)
    {
        위해(정수 i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    만약( hdr == m.hdr )
        반환;
    만약( !hdr )
    {
        m.release();
        반환;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    위해( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    정수 ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    위해( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, 정수 rtype, double alpha ) const
{
    정수 cn = channels();
    만약( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    만약( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        반환;
    }

    CV_Assert(hdr != 0);
    만약( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    만약( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        위해( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    아니면
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        위해( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, 정수 rtype, double alpha, double beta ) const
{
    정수 cn = channels();
    만약( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    만약( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        위해( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    아니면
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        위해( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    만약( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(정수 i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h && elem->idx[0] == i0 )
            반환 &value<uchar>(elem);
        nidx = elem->next;
    }

    만약( createMissing )
    {
        정수 idx[] = { i0 };
        반환 newNode( idx, h );
    }
    반환 NULL;
}

uchar* SparseMat::ptr(정수 i0, 정수 i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            반환 &value<uchar>(elem);
        nidx = elem->next;
    }

    만약( createMissing )
    {
        정수 idx[] = { i0, i1 };
        반환 newNode( idx, h );
    }
    반환 NULL;
}

uchar* SparseMat::ptr(정수 i0, 정수 i1, 정수 i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            반환 &value<uchar>(elem);
        nidx = elem->next;
    }

    만약( createMissing )
    {
        정수 idx[] = { i0, i1, i2 };
        반환 newNode( idx, h );
    }
    반환 NULL;
}

uchar* SparseMat::ptr(const 정수* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    정수 i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h )
        {
            위해( i = 0; i < d; i++ )
                만약( elem->idx[i] != idx[i] )
                    break;
            만약( i == d )
                반환 &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    반환 createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(정수 i0, 정수 i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    만약( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(정수 i0, 정수 i1, 정수 i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    만약( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const 정수* idx, size_t* hashval)
{
    CV_Assert( hdr );
    정수 i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    동안( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        만약( elem->hashval == h )
        {
            위해( i = 0; i < d; i++ )
                만약( elem->idx[i] != idx[i] )
                    break;
            만약( i == d )
                break;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    만약( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    만약((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((double)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    위해( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    위해( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        동안( nidx )
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

uchar* SparseMat::newNode(const 정수* idx, size_t hashval)
{
    const 정수 HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    만약( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    만약( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        위해( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    정수 i, d = hdr->dims;
    위해( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    만약( esz == sizeof(float) )
        *((float*)p) = 0.f;
    아니면 만약( esz == sizeof(double) )
        *((double*)p) = 0.;
    아니면
        memset(p, 0, esz);

    반환 p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    만약( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    아니면
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
double norm( const SparseMat& src, 정수 normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    정수 type = src.type();
    double result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    만약( type == CV_32F )
    {
        만약( normType == NORM_INF )
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((double)it.value<float>()));
            }
        아니면 만약( normType == NORM_L1 )
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        아니면
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<float>();
                result += v*v;
            }
    }
    아니면 만약( type == CV_64F )
    {
        만약( normType == NORM_INF )
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<double>()));
            }
        아니면 만약( normType == NORM_L1 )
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<double>());
            }
        아니면
            위해( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<double>();
                result += v*v;
            }
    }
    아니면
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    만약( normType == NORM_L2 )
        result = std::sqrt(result);
    반환 result;
}

void minMaxLoc( const SparseMat& src, double* _minval, double* _maxval, 정수* _minidx, 정수* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    정수 type = src.type();
    const 정수 *minidx = 0, *maxidx = 0;

    만약( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        위해( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            만약( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            만약( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        만약( _minval )
            *_minval = minval;
        만약( _maxval )
            *_maxval = maxval;
    }
    아니면 만약( type == CV_64F )
    {
        double minval = DBL_MAX, maxval = -DBL_MAX;
        위해( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            double v = it.value<double>();
            만약( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            만약( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        만약( _minval )
            *_minval = minval;
        만약( _maxval )
            *_maxval = maxval;
    }
    아니면
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    만약( _minidx && minidx )
        위해( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    만약( _maxidx && maxidx )
        위해( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, double a, 정수 norm_type )
{
    CV_INSTRUMENT_REGION();

    double scale = 1;
    만약( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    아니면
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
