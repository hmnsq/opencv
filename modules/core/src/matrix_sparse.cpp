// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, ціле cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    якщо( cn == 1 )
        *to = saturate_cast<T2>(*from);
    інакше
        для( ціле i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, ціле cn, double alpha, double beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    якщо( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    інакше
        для( ціле i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, ціле cn);
typedef void (*ConvertScaleData)(const void* from, void* to, ціле cn, double alpha, double beta);

static ConvertData getConvertElem(ціле fromType, ціле toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ціле>, convertData_<uchar, float>,
      convertData_<uchar, double>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ціле>, convertData_<schar, float>,
      convertData_<schar, double>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ціле>, convertData_<ushort, float>,
      convertData_<ushort, double>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ціле>, convertData_<short, float>,
      convertData_<short, double>, 0 },

    { convertData_<ціле, uchar>, convertData_<ціле, schar>,
      convertData_<ціле, ushort>, convertData_<ціле, short>,
      convertData_<ціле, ціле>, convertData_<ціле, float>,
      convertData_<ціле, double>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, ціле>, convertData_<float, float>,
      convertData_<float, double>, 0 },

    { convertData_<double, uchar>, convertData_<double, schar>,
      convertData_<double, ushort>, convertData_<double, short>,
      convertData_<double, ціле>, convertData_<double, float>,
      convertData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    повернути func;
}

static ConvertScaleData getConvertScaleElem(ціле fromType, ціле toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ціле>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, double>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ціле>, convertScaleData_<schar, float>,
      convertScaleData_<schar, double>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ціле>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, double>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ціле>, convertScaleData_<short, float>,
      convertScaleData_<short, double>, 0 },

    { convertScaleData_<ціле, uchar>, convertScaleData_<ціле, schar>,
      convertScaleData_<ціле, ushort>, convertScaleData_<ціле, short>,
      convertScaleData_<ціле, ціле>, convertScaleData_<ціле, float>,
      convertScaleData_<ціле, double>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, ціле>, convertScaleData_<float, float>,
      convertScaleData_<float, double>, 0 },

    { convertScaleData_<double, uchar>, convertScaleData_<double, schar>,
      convertScaleData_<double, ushort>, convertScaleData_<double, short>,
      convertScaleData_<double, ціле>, convertScaleData_<double, float>,
      convertScaleData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    повернути func;
}

enum { HASH_SIZE0 = 8 };

static inline void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    для( i = 0; i + sizeof(ціле) <= elemSize; i += sizeof(ціле) )
        *(ціле*)(to + i) = *(const ціле*)(from + i);
    для( ; i < elemSize; i++ )
        to[i] = from[i];
}

static inline bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    для( i = 0; i + sizeof(ціле) <= elemSize; i += sizeof(ціле) )
        якщо( *(ціле*)(data + i) != 0 )
            повернути false;
    для( ; i < elemSize; i++ )
        якщо( data[i] != 0 )
            повернути false;
    повернути true;
}

SparseMat::Hdr::Hdr( ціле _dims, const ціле* _sizes, ціле _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (ціле)alignSize(sizeof(SparseMat::Node) - MAX_DIM*sizeof(ціле) +
                                 dims*sizeof(ціле), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (ціле)sizeof(size_t));

    ціле i;
    для( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    для( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#якщо defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#якщо defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ціле _dims, const ціле* _sizes, ціле _type)
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
    якщо( this != &m )
    {
        якщо( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    повернути *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    повернути (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, ціле _type) const
{
    якщо( _type < 0 )
        m = *this;
    інакше
        convertTo(m, _type);
}

void SparseMat::addref()
{
    якщо( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    якщо( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ціле i0) const
{
    повернути (size_t)i0;
}

size_t SparseMat::hash(ціле i0, ціле i1) const
{
    повернути (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ціле i0, ціле i1, ціле i2) const
{
    повернути ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const ціле* idx) const
{
    size_t h = (unsigned)idx[0];
    якщо( !hdr )
        повернути 0;
    ціле d = hdr->dims;
    для(ціле i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    повернути h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ціле i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    для(;;)
    {
        для( i = 0; i < lastSize; i++, dptr += esz )
        {
            якщо( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        для( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            якщо( ++idx[i] < m.size[i] )
                break;
            idx[i] = 0;
        }
        якщо( i < 0 )
            break;
    }
}

void SparseMat::create(ціле d, const ціле* _sizes, ціле _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    для( ціле i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    якщо( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ціле i;
        для( i = 0; i < d; i++ )
            якщо( _sizes[i] != hdr->size[i] )
                break;
        якщо( i == d )
        {
            clear();
            повернути;
        }
    }
    ціле _sizes_backup[CV_MAX_DIM]; // #5991
    якщо (hdr && _sizes == hdr->size)
    {
        для(ціле i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    якщо( hdr == m.hdr )
        повернути;
    якщо( !hdr )
    {
        m.release();
        повернути;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    для( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    ціле ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    для( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, ціле rtype, double alpha ) const
{
    ціле cn = channels();
    якщо( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    якщо( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        повернути;
    }

    CV_Assert(hdr != 0);
    якщо( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    якщо( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        для( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    інакше
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        для( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, ціле rtype, double alpha, double beta ) const
{
    ціле cn = channels();
    якщо( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    якщо( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        для( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    інакше
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        для( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    якщо( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ціле i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h && elem->idx[0] == i0 )
            повернути &value<uchar>(elem);
        nidx = elem->next;
    }

    якщо( createMissing )
    {
        ціле idx[] = { i0 };
        повернути newNode( idx, h );
    }
    повернути NULL;
}

uchar* SparseMat::ptr(ціле i0, ціле i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            повернути &value<uchar>(elem);
        nidx = elem->next;
    }

    якщо( createMissing )
    {
        ціле idx[] = { i0, i1 };
        повернути newNode( idx, h );
    }
    повернути NULL;
}

uchar* SparseMat::ptr(ціле i0, ціле i1, ціле i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            повернути &value<uchar>(elem);
        nidx = elem->next;
    }

    якщо( createMissing )
    {
        ціле idx[] = { i0, i1, i2 };
        повернути newNode( idx, h );
    }
    повернути NULL;
}

uchar* SparseMat::ptr(const ціле* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ціле i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h )
        {
            для( i = 0; i < d; i++ )
                якщо( elem->idx[i] != idx[i] )
                    break;
            якщо( i == d )
                повернути &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    повернути createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(ціле i0, ціле i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    якщо( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(ціле i0, ціле i1, ціле i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    якщо( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const ціле* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ціле i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    поки( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        якщо( elem->hashval == h )
        {
            для( i = 0; i < d; i++ )
                якщо( elem->idx[i] != idx[i] )
                    break;
            якщо( i == d )
                break;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    якщо( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    якщо((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((double)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    для( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    для( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        поки( nidx )
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

uchar* SparseMat::newNode(const ціле* idx, size_t hashval)
{
    const ціле HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    якщо( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    якщо( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        для( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ціле i, d = hdr->dims;
    для( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    якщо( esz == sizeof(float) )
        *((float*)p) = 0.f;
    інакше якщо( esz == sizeof(double) )
        *((double*)p) = 0.;
    інакше
        memset(p, 0, esz);

    повернути p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    якщо( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    інакше
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
double norm( const SparseMat& src, ціле normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ціле type = src.type();
    double result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    якщо( type == CV_32F )
    {
        якщо( normType == NORM_INF )
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((double)it.value<float>()));
            }
        інакше якщо( normType == NORM_L1 )
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        інакше
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<float>();
                result += v*v;
            }
    }
    інакше якщо( type == CV_64F )
    {
        якщо( normType == NORM_INF )
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<double>()));
            }
        інакше якщо( normType == NORM_L1 )
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<double>());
            }
        інакше
            для( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<double>();
                result += v*v;
            }
    }
    інакше
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    якщо( normType == NORM_L2 )
        result = std::sqrt(result);
    повернути result;
}

void minMaxLoc( const SparseMat& src, double* _minval, double* _maxval, ціле* _minidx, ціле* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ціле type = src.type();
    const ціле *minidx = 0, *maxidx = 0;

    якщо( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        для( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            якщо( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            якщо( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        якщо( _minval )
            *_minval = minval;
        якщо( _maxval )
            *_maxval = maxval;
    }
    інакше якщо( type == CV_64F )
    {
        double minval = DBL_MAX, maxval = -DBL_MAX;
        для( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            double v = it.value<double>();
            якщо( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            якщо( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        якщо( _minval )
            *_minval = minval;
        якщо( _maxval )
            *_maxval = maxval;
    }
    інакше
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    якщо( _minidx && minidx )
        для( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    якщо( _maxidx && maxidx )
        для( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, double a, ціле norm_type )
{
    CV_INSTRUMENT_REGION();

    double scale = 1;
    якщо( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    інакше
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
