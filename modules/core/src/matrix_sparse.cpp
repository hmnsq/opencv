// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, số_nguyên cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    nếu( cn == 1 )
        *to = saturate_cast<T2>(*from);
    không_thì
        cho( số_nguyên i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, số_nguyên cn, double alpha, double beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    nếu( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    không_thì
        cho( số_nguyên i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, số_nguyên cn);
typedef void (*ConvertScaleData)(const void* from, void* to, số_nguyên cn, double alpha, double beta);

static ConvertData getConvertElem(số_nguyên fromType, số_nguyên toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, số_nguyên>, convertData_<uchar, float>,
      convertData_<uchar, double>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, số_nguyên>, convertData_<schar, float>,
      convertData_<schar, double>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, số_nguyên>, convertData_<ushort, float>,
      convertData_<ushort, double>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, số_nguyên>, convertData_<short, float>,
      convertData_<short, double>, 0 },

    { convertData_<số_nguyên, uchar>, convertData_<số_nguyên, schar>,
      convertData_<số_nguyên, ushort>, convertData_<số_nguyên, short>,
      convertData_<số_nguyên, số_nguyên>, convertData_<số_nguyên, float>,
      convertData_<số_nguyên, double>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, số_nguyên>, convertData_<float, float>,
      convertData_<float, double>, 0 },

    { convertData_<double, uchar>, convertData_<double, schar>,
      convertData_<double, ushort>, convertData_<double, short>,
      convertData_<double, số_nguyên>, convertData_<double, float>,
      convertData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    trả_về func;
}

static ConvertScaleData getConvertScaleElem(số_nguyên fromType, số_nguyên toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, số_nguyên>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, double>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, số_nguyên>, convertScaleData_<schar, float>,
      convertScaleData_<schar, double>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, số_nguyên>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, double>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, số_nguyên>, convertScaleData_<short, float>,
      convertScaleData_<short, double>, 0 },

    { convertScaleData_<số_nguyên, uchar>, convertScaleData_<số_nguyên, schar>,
      convertScaleData_<số_nguyên, ushort>, convertScaleData_<số_nguyên, short>,
      convertScaleData_<số_nguyên, số_nguyên>, convertScaleData_<số_nguyên, float>,
      convertScaleData_<số_nguyên, double>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, số_nguyên>, convertScaleData_<float, float>,
      convertScaleData_<float, double>, 0 },

    { convertScaleData_<double, uchar>, convertScaleData_<double, schar>,
      convertScaleData_<double, ushort>, convertScaleData_<double, short>,
      convertScaleData_<double, số_nguyên>, convertScaleData_<double, float>,
      convertScaleData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    trả_về func;
}

enum { HASH_SIZE0 = 8 };

static inline void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    cho( i = 0; i + sizeof(số_nguyên) <= elemSize; i += sizeof(số_nguyên) )
        *(số_nguyên*)(to + i) = *(const số_nguyên*)(from + i);
    cho( ; i < elemSize; i++ )
        to[i] = from[i];
}

static inline bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    cho( i = 0; i + sizeof(số_nguyên) <= elemSize; i += sizeof(số_nguyên) )
        nếu( *(số_nguyên*)(data + i) != 0 )
            trả_về false;
    cho( ; i < elemSize; i++ )
        nếu( data[i] != 0 )
            trả_về false;
    trả_về true;
}

SparseMat::Hdr::Hdr( số_nguyên _dims, const số_nguyên* _sizes, số_nguyên _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (số_nguyên)alignSize(sizeof(SparseMat::Node) - MAX_DIM*sizeof(số_nguyên) +
                                 dims*sizeof(số_nguyên), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (số_nguyên)sizeof(size_t));

    số_nguyên i;
    cho( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    cho( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#nếu defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#nếu defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(số_nguyên _dims, const số_nguyên* _sizes, số_nguyên _type)
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
    nếu( this != &m )
    {
        nếu( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    trả_về *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    trả_về (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, số_nguyên _type) const
{
    nếu( _type < 0 )
        m = *this;
    không_thì
        convertTo(m, _type);
}

void SparseMat::addref()
{
    nếu( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    nếu( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(số_nguyên i0) const
{
    trả_về (size_t)i0;
}

size_t SparseMat::hash(số_nguyên i0, số_nguyên i1) const
{
    trả_về (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(số_nguyên i0, số_nguyên i1, số_nguyên i2) const
{
    trả_về ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const số_nguyên* idx) const
{
    size_t h = (unsigned)idx[0];
    nếu( !hdr )
        trả_về 0;
    số_nguyên d = hdr->dims;
    cho(số_nguyên i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    trả_về h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    số_nguyên i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    cho(;;)
    {
        cho( i = 0; i < lastSize; i++, dptr += esz )
        {
            nếu( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        cho( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            nếu( ++idx[i] < m.size[i] )
                break;
            idx[i] = 0;
        }
        nếu( i < 0 )
            break;
    }
}

void SparseMat::create(số_nguyên d, const số_nguyên* _sizes, số_nguyên _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    cho( số_nguyên i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    nếu( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        số_nguyên i;
        cho( i = 0; i < d; i++ )
            nếu( _sizes[i] != hdr->size[i] )
                break;
        nếu( i == d )
        {
            clear();
            trả_về;
        }
    }
    số_nguyên _sizes_backup[CV_MAX_DIM]; // #5991
    nếu (hdr && _sizes == hdr->size)
    {
        cho(số_nguyên i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    nếu( hdr == m.hdr )
        trả_về;
    nếu( !hdr )
    {
        m.release();
        trả_về;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    cho( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    số_nguyên ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    cho( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, số_nguyên rtype, double alpha ) const
{
    số_nguyên cn = channels();
    nếu( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    nếu( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        trả_về;
    }

    CV_Assert(hdr != 0);
    nếu( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    nếu( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        cho( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    không_thì
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        cho( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, số_nguyên rtype, double alpha, double beta ) const
{
    số_nguyên cn = channels();
    nếu( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    nếu( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        cho( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    không_thì
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        cho( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    nếu( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(số_nguyên i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h && elem->idx[0] == i0 )
            trả_về &value<uchar>(elem);
        nidx = elem->next;
    }

    nếu( createMissing )
    {
        số_nguyên idx[] = { i0 };
        trả_về newNode( idx, h );
    }
    trả_về NULL;
}

uchar* SparseMat::ptr(số_nguyên i0, số_nguyên i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            trả_về &value<uchar>(elem);
        nidx = elem->next;
    }

    nếu( createMissing )
    {
        số_nguyên idx[] = { i0, i1 };
        trả_về newNode( idx, h );
    }
    trả_về NULL;
}

uchar* SparseMat::ptr(số_nguyên i0, số_nguyên i1, số_nguyên i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            trả_về &value<uchar>(elem);
        nidx = elem->next;
    }

    nếu( createMissing )
    {
        số_nguyên idx[] = { i0, i1, i2 };
        trả_về newNode( idx, h );
    }
    trả_về NULL;
}

uchar* SparseMat::ptr(const số_nguyên* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    số_nguyên i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h )
        {
            cho( i = 0; i < d; i++ )
                nếu( elem->idx[i] != idx[i] )
                    break;
            nếu( i == d )
                trả_về &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    trả_về createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(số_nguyên i0, số_nguyên i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    nếu( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(số_nguyên i0, số_nguyên i1, số_nguyên i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    nếu( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const số_nguyên* idx, size_t* hashval)
{
    CV_Assert( hdr );
    số_nguyên i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    trong_khi( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        nếu( elem->hashval == h )
        {
            cho( i = 0; i < d; i++ )
                nếu( elem->idx[i] != idx[i] )
                    break;
            nếu( i == d )
                break;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    nếu( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    nếu((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((double)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    cho( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    cho( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        trong_khi( nidx )
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

uchar* SparseMat::newNode(const số_nguyên* idx, size_t hashval)
{
    const số_nguyên HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    nếu( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    nếu( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        cho( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    số_nguyên i, d = hdr->dims;
    cho( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    nếu( esz == sizeof(float) )
        *((float*)p) = 0.f;
    không_thì nếu( esz == sizeof(double) )
        *((double*)p) = 0.;
    không_thì
        memset(p, 0, esz);

    trả_về p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    nếu( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    không_thì
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
double norm( const SparseMat& src, số_nguyên normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    số_nguyên type = src.type();
    double result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    nếu( type == CV_32F )
    {
        nếu( normType == NORM_INF )
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((double)it.value<float>()));
            }
        không_thì nếu( normType == NORM_L1 )
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        không_thì
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<float>();
                result += v*v;
            }
    }
    không_thì nếu( type == CV_64F )
    {
        nếu( normType == NORM_INF )
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<double>()));
            }
        không_thì nếu( normType == NORM_L1 )
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<double>());
            }
        không_thì
            cho( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<double>();
                result += v*v;
            }
    }
    không_thì
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    nếu( normType == NORM_L2 )
        result = std::sqrt(result);
    trả_về result;
}

void minMaxLoc( const SparseMat& src, double* _minval, double* _maxval, số_nguyên* _minidx, số_nguyên* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    số_nguyên type = src.type();
    const số_nguyên *minidx = 0, *maxidx = 0;

    nếu( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        cho( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            nếu( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            nếu( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        nếu( _minval )
            *_minval = minval;
        nếu( _maxval )
            *_maxval = maxval;
    }
    không_thì nếu( type == CV_64F )
    {
        double minval = DBL_MAX, maxval = -DBL_MAX;
        cho( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            double v = it.value<double>();
            nếu( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            nếu( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        nếu( _minval )
            *_minval = minval;
        nếu( _maxval )
            *_maxval = maxval;
    }
    không_thì
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    nếu( _minidx && minidx )
        cho( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    nếu( _maxidx && maxidx )
        cho( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, double a, số_nguyên norm_type )
{
    CV_INSTRUMENT_REGION();

    double scale = 1;
    nếu( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    không_thì
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
