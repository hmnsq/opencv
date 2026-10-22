// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, цео cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ако( cn == 1 )
        *to = saturate_cast<T2>(*from);
    инако
        за( цео i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, цео cn, double alpha, double beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    ако( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    инако
        за( цео i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, цео cn);
typedef void (*ConvertScaleData)(const void* from, void* to, цео cn, double alpha, double beta);

static ConvertData getConvertElem(цео fromType, цео toType)
{
    static ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, цео>, convertData_<uchar, float>,
      convertData_<uchar, double>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, цео>, convertData_<schar, float>,
      convertData_<schar, double>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, цео>, convertData_<ushort, float>,
      convertData_<ushort, double>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, цео>, convertData_<short, float>,
      convertData_<short, double>, 0 },

    { convertData_<цео, uchar>, convertData_<цео, schar>,
      convertData_<цео, ushort>, convertData_<цео, short>,
      convertData_<цео, цео>, convertData_<цео, float>,
      convertData_<цео, double>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, цео>, convertData_<float, float>,
      convertData_<float, double>, 0 },

    { convertData_<double, uchar>, convertData_<double, schar>,
      convertData_<double, ushort>, convertData_<double, short>,
      convertData_<double, цео>, convertData_<double, float>,
      convertData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    врати func;
}

static ConvertScaleData getConvertScaleElem(цео fromType, цео toType)
{
    static ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, цео>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, double>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, цео>, convertScaleData_<schar, float>,
      convertScaleData_<schar, double>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, цео>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, double>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, цео>, convertScaleData_<short, float>,
      convertScaleData_<short, double>, 0 },

    { convertScaleData_<цео, uchar>, convertScaleData_<цео, schar>,
      convertScaleData_<цео, ushort>, convertScaleData_<цео, short>,
      convertScaleData_<цео, цео>, convertScaleData_<цео, float>,
      convertScaleData_<цео, double>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, цео>, convertScaleData_<float, float>,
      convertScaleData_<float, double>, 0 },

    { convertScaleData_<double, uchar>, convertScaleData_<double, schar>,
      convertScaleData_<double, ushort>, convertScaleData_<double, short>,
      convertScaleData_<double, цео>, convertScaleData_<double, float>,
      convertScaleData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    врати func;
}

enum { HASH_SIZE0 = 8 };

static inline void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    за( i = 0; i + sizeof(цео) <= elemSize; i += sizeof(цео) )
        *(цео*)(to + i) = *(const цео*)(from + i);
    за( ; i < elemSize; i++ )
        to[i] = from[i];
}

static inline bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    за( i = 0; i + sizeof(цео) <= elemSize; i += sizeof(цео) )
        ако( *(цео*)(data + i) != 0 )
            врати false;
    за( ; i < elemSize; i++ )
        ако( data[i] != 0 )
            врати false;
    врати true;
}

SparseMat::Hdr::Hdr( цео _dims, const цео* _sizes, цео _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (цео)alignSize(sizeof(SparseMat::Node) - MAX_DIM*sizeof(цео) +
                                 dims*sizeof(цео), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (цео)sizeof(size_t));

    цео i;
    за( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    за( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ако defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#ако defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(цео _dims, const цео* _sizes, цео _type)
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
    ако( this != &m )
    {
        ако( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    врати *this;
}

SparseMat& SparseMat::operator=(const Mat& m)
{
    врати (*this = SparseMat(m));
}

void SparseMat::assignTo(SparseMat& m, цео _type) const
{
    ако( _type < 0 )
        m = *this;
    инако
        convertTo(m, _type);
}

void SparseMat::addref()
{
    ако( hdr )
        CV_XADD(&hdr->refcount, 1);
}

void SparseMat::release()
{
    ако( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(цео i0) const
{
    врати (size_t)i0;
}

size_t SparseMat::hash(цео i0, цео i1) const
{
    врати (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(цео i0, цео i1, цео i2) const
{
    врати ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(const цео* idx) const
{
    size_t h = (unsigned)idx[0];
    ако( !hdr )
        врати 0;
    цео d = hdr->dims;
    за(цео i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    врати h;
}


SparseMat::SparseMat(const Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    цео i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    const uchar* dptr = m.ptr();

    за(;;)
    {
        за( i = 0; i < lastSize; i++, dptr += esz )
        {
            ако( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        за( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ако( ++idx[i] < m.size[i] )
                break;
            idx[i] = 0;
        }
        ако( i < 0 )
            break;
    }
}

void SparseMat::create(цео d, const цео* _sizes, цео _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    за( цео i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ако( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        цео i;
        за( i = 0; i < d; i++ )
            ако( _sizes[i] != hdr->size[i] )
                break;
        ако( i == d )
        {
            clear();
            врати;
        }
    }
    цео _sizes_backup[CV_MAX_DIM]; // #5991
    ако (hdr && _sizes == hdr->size)
    {
        за(цео i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    ако( hdr == m.hdr )
        врати;
    ако( !hdr )
    {
        m.release();
        врати;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    за( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( Mat& m ) const
{
    CV_Assert( hdr );
    цео ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    за( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, цео rtype, double alpha ) const
{
    цео cn = channels();
    ако( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ако( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        врати;
    }

    CV_Assert(hdr != 0);
    ако( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ако( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        за( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    инако
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        за( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( Mat& m, цео rtype, double alpha, double beta ) const
{
    цео cn = channels();
    ако( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ако( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        за( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    инако
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        за( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    ако( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(цео i0, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h && elem->idx[0] == i0 )
            врати &value<uchar>(elem);
        nidx = elem->next;
    }

    ако( createMissing )
    {
        цео idx[] = { i0 };
        врати newNode( idx, h );
    }
    врати NULL;
}

uchar* SparseMat::ptr(цео i0, цео i1, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            врати &value<uchar>(elem);
        nidx = elem->next;
    }

    ако( createMissing )
    {
        цео idx[] = { i0, i1 };
        врати newNode( idx, h );
    }
    врати NULL;
}

uchar* SparseMat::ptr(цео i0, цео i1, цео i2, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            врати &value<uchar>(elem);
        nidx = elem->next;
    }

    ако( createMissing )
    {
        цео idx[] = { i0, i1, i2 };
        врати newNode( idx, h );
    }
    врати NULL;
}

uchar* SparseMat::ptr(const цео* idx, bool createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    цео i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h )
        {
            за( i = 0; i < d; i++ )
                ако( elem->idx[i] != idx[i] )
                    break;
            ако( i == d )
                врати &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    врати createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(цео i0, цео i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    ако( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(цео i0, цео i1, цео i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    ако( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const цео* idx, size_t* hashval)
{
    CV_Assert( hdr );
    цео i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    док( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ако( elem->hashval == h )
        {
            за( i = 0; i < d; i++ )
                ако( elem->idx[i] != idx[i] )
                    break;
            ако( i == d )
                break;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ако( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ако((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((double)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    за( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    за( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        док( nidx )
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

uchar* SparseMat::newNode(const цео* idx, size_t hashval)
{
    const цео HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ако( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ако( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        за( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    цео i, d = hdr->dims;
    за( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ако( esz == sizeof(float) )
        *((float*)p) = 0.f;
    инако ако( esz == sizeof(double) )
        *((double*)p) = 0.;
    инако
        memset(p, 0, esz);

    врати p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    ако( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    инако
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
double norm( const SparseMat& src, цео normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    цео type = src.type();
    double result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ако( type == CV_32F )
    {
        ако( normType == NORM_INF )
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((double)it.value<float>()));
            }
        инако ако( normType == NORM_L1 )
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<float>());
            }
        инако
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<float>();
                result += v*v;
            }
    }
    инако ако( type == CV_64F )
    {
        ако( normType == NORM_INF )
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<double>()));
            }
        инако ако( normType == NORM_L1 )
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<double>());
            }
        инако
            за( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                double v = it.value<double>();
                result += v*v;
            }
    }
    инако
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ако( normType == NORM_L2 )
        result = std::sqrt(result);
    врати result;
}

void minMaxLoc( const SparseMat& src, double* _minval, double* _maxval, цео* _minidx, цео* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    цео type = src.type();
    const цео *minidx = 0, *maxidx = 0;

    ако( type == CV_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        за( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            float v = it.value<float>();
            ако( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ако( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ако( _minval )
            *_minval = minval;
        ако( _maxval )
            *_maxval = maxval;
    }
    инако ако( type == CV_64F )
    {
        double minval = DBL_MAX, maxval = -DBL_MAX;
        за( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            double v = it.value<double>();
            ако( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            ако( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        ако( _minval )
            *_minval = minval;
        ако( _maxval )
            *_maxval = maxval;
    }
    инако
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ако( _minidx && minidx )
        за( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ако( _maxidx && maxidx )
        за( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, double a, цео norm_type )
{
    CV_INSTRUMENT_REGION();

    double scale = 1;
    ако( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    инако
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
