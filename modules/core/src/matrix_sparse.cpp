// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

template<typename T1, typename T2> فراغ
convertData_(ثابت فراغ* _from, فراغ* _to, صحيح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* to = (T2*)_to;
    אם( cn == 1 )
        *to = saturate_cast<T2>(*from);
    אחרת
        עבור( صحيح i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> فراغ
convertScaleData_(ثابت فراغ* _from, فراغ* _to, صحيح cn, مزدوج alpha, مزدوج beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* to = (T2*)_to;
    אם( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    אחרת
        עבור( صحيح i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

تعريفنوع فراغ (*ConvertData)(ثابت فراغ* from, فراغ* to, صحيح cn);
تعريفنوع فراغ (*ConvertScaleData)(ثابت فراغ* from, فراغ* to, صحيح cn, مزدوج alpha, مزدوج beta);

ساكن ConvertData getConvertElem(صحيح fromType, صحيح toType)
{
    ساكن ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, صحيح>, convertData_<uchar, عائم>,
      convertData_<uchar, مزدوج>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, صحيح>, convertData_<schar, عائم>,
      convertData_<schar, مزدوج>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, صحيح>, convertData_<ushort, عائم>,
      convertData_<ushort, مزدوج>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, صحيح>, convertData_<short, عائم>,
      convertData_<short, مزدوج>, 0 },

    { convertData_<صحيح, uchar>, convertData_<صحيح, schar>,
      convertData_<صحيح, ushort>, convertData_<صحيح, short>,
      convertData_<صحيح, صحيح>, convertData_<صحيح, عائم>,
      convertData_<صحيح, مزدوج>, 0 },

    { convertData_<عائم, uchar>, convertData_<عائم, schar>,
      convertData_<عائم, ushort>, convertData_<عائم, short>,
      convertData_<عائم, صحيح>, convertData_<عائم, عائم>,
      convertData_<عائم, مزدوج>, 0 },

    { convertData_<مزدوج, uchar>, convertData_<مزدوج, schar>,
      convertData_<مزدوج, ushort>, convertData_<مزدوج, short>,
      convertData_<مزدوج, صحيح>, convertData_<مزدوج, عائم>,
      convertData_<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    החזר func;
}

ساكن ConvertScaleData getConvertScaleElem(صحيح fromType, صحيح toType)
{
    ساكن ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, صحيح>, convertScaleData_<uchar, عائم>,
      convertScaleData_<uchar, مزدوج>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, صحيح>, convertScaleData_<schar, عائم>,
      convertScaleData_<schar, مزدوج>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, صحيح>, convertScaleData_<ushort, عائم>,
      convertScaleData_<ushort, مزدوج>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, صحيح>, convertScaleData_<short, عائم>,
      convertScaleData_<short, مزدوج>, 0 },

    { convertScaleData_<صحيح, uchar>, convertScaleData_<صحيح, schar>,
      convertScaleData_<صحيح, ushort>, convertScaleData_<صحيح, short>,
      convertScaleData_<صحيح, صحيح>, convertScaleData_<صحيح, عائم>,
      convertScaleData_<صحيح, مزدوج>, 0 },

    { convertScaleData_<عائم, uchar>, convertScaleData_<عائم, schar>,
      convertScaleData_<عائم, ushort>, convertScaleData_<عائم, short>,
      convertScaleData_<عائم, صحيح>, convertScaleData_<عائم, عائم>,
      convertScaleData_<عائم, مزدوج>, 0 },

    { convertScaleData_<مزدوج, uchar>, convertScaleData_<مزدوج, schar>,
      convertScaleData_<مزدوج, ushort>, convertScaleData_<مزدوج, short>,
      convertScaleData_<مزدوج, صحيح>, convertScaleData_<مزدوج, عائم>,
      convertScaleData_<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    החזר func;
}

تعداد { HASH_SIZE0 = 8 };

ساكن مضمن فراغ copyElem(ثابت uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    עבור( i = 0; i + حجم(صحيح) <= elemSize; i += حجم(صحيح) )
        *(صحيح*)(to + i) = *(ثابت صحيح*)(from + i);
    עבור( ; i < elemSize; i++ )
        to[i] = from[i];
}

ساكن مضمن منطقي isZeroElem(ثابت uchar* data, size_t elemSize)
{
    size_t i;
    עבור( i = 0; i + حجم(صحيح) <= elemSize; i += حجم(صحيح) )
        אם( *(صحيح*)(data + i) != 0 )
            החזר שקר;
    עבור( ; i < elemSize; i++ )
        אם( data[i] != 0 )
            החזר שקר;
    החזר אמת;
}

SparseMat::Hdr::Hdr( صحيح _dims, ثابت صحيح* _sizes, صحيح _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (صحيح)alignSize(حجم(SparseMat::Node) - MAX_DIM*حجم(صحيح) +
                                 dims*حجم(صحيح), CV_ELEM_SIZE1(_type));
    nodeSize = alignSize(valueOffset +
        CV_ELEM_SIZE(_type), (صحيح)حجم(size_t));

    صحيح i;
    עבור( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    עבור( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

فراغ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#אם defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(nodeSize);
#אם defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    nodeCount = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(صحيح _dims, ثابت صحيح* _sizes, صحيح _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ثابت SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ثابت SparseMat& m)
{
    אם( هذا != &m )
    {
        אם( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    החזר *هذا;
}

SparseMat& SparseMat::operator=(ثابت Mat& m)
{
    החזר (*هذا = SparseMat(m));
}

فراغ SparseMat::assignTo(SparseMat& m, صحيح _type) ثابت
{
    אם( _type < 0 )
        m = *هذا;
    אחרת
        convertTo(m, _type);
}

فراغ SparseMat::addref()
{
    אם( hdr )
        CV_XADD(&hdr->refcount, 1);
}

فراغ SparseMat::release()
{
    אם( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحيح i0) ثابت
{
    החזר (size_t)i0;
}

size_t SparseMat::hash(صحيح i0, صحيح i1) ثابت
{
    החזר (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(صحيح i0, صحيح i1, صحيح i2) ثابت
{
    החזר ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ثابت صحيح* idx) ثابت
{
    size_t h = (unsigned)idx[0];
    אם( !hdr )
        החזר 0;
    صحيح d = hdr->dims;
    עבור(صحيح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    החזר h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحيح i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ثابت uchar* dptr = m.ptr();

    עבור(;;)
    {
        עבור( i = 0; i < lastSize; i++, dptr += esz )
        {
            אם( isZeroElem(dptr, esz) )
                استمر;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        עבור( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            אם( ++idx[i] < m.size[i] )
                اكسر;
            idx[i] = 0;
        }
        אם( i < 0 )
            اكسر;
    }
}

فراغ SparseMat::create(صحيح d, ثابت صحيح* _sizes, صحيح _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    עבור( صحيح i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    אם( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        صحيح i;
        עבור( i = 0; i < d; i++ )
            אם( _sizes[i] != hdr->size[i] )
                اكسر;
        אם( i == d )
        {
            clear();
            החזר;
        }
    }
    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    אם (hdr && _sizes == hdr->size)
    {
        עבור(صحيح i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = جديد Hdr(d, _sizes, _type);
}

فراغ SparseMat::copyTo( SparseMat& m ) ثابت
{
    אם( hdr == m.hdr )
        החזר;
    אם( !hdr )
    {
        m.release();
        החזר;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    עבור( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

فراغ SparseMat::copyTo( Mat& m ) ثابت
{
    CV_Assert( hdr );
    صحيح ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    עבור( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


فراغ SparseMat::convertTo( SparseMat& m, صحيح rtype, مزدوج alpha ) ثابت
{
    صحيح cn = channels();
    אם( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    אם( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        החזר;
    }

    CV_Assert(hdr != 0);
    אם( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    אם( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        עבור( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    אחרת
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        עבור( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


فراغ SparseMat::convertTo( Mat& m, صحيح rtype, مزدوج alpha, مزدوج beta ) ثابت
{
    صحيح cn = channels();
    אם( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    אם( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        עבור( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    אחרת
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        עבור( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

فراغ SparseMat::clear()
{
    אם( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(صحيح i0, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h && elem->idx[0] == i0 )
            החזר &value<uchar>(elem);
        nidx = elem->next;
    }

    אם( createMissing )
    {
        صحيح idx[] = { i0 };
        החזר newNode( idx, h );
    }
    החזר عدم;
}

uchar* SparseMat::ptr(صحيح i0, صحيح i1, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            החזר &value<uchar>(elem);
        nidx = elem->next;
    }

    אם( createMissing )
    {
        صحيح idx[] = { i0, i1 };
        החזר newNode( idx, h );
    }
    החזר عدم;
}

uchar* SparseMat::ptr(صحيح i0, صحيح i1, صحيح i2, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            החזר &value<uchar>(elem);
        nidx = elem->next;
    }

    אם( createMissing )
    {
        صحيح idx[] = { i0, i1, i2 };
        החזר newNode( idx, h );
    }
    החזר عدم;
}

uchar* SparseMat::ptr(ثابت صحيح* idx, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    صحيح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h )
        {
            עבור( i = 0; i < d; i++ )
                אם( elem->idx[i] != idx[i] )
                    اكسر;
            אם( i == d )
                החזר &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    החזר createMissing ? newNode(idx, h) : عدم;
}

فراغ SparseMat::erase(صحيح i0, صحيح i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            اكسر;
        previdx = nidx;
        nidx = elem->next;
    }

    אם( nidx )
        removeNode(hidx, nidx, previdx);
}

فراغ SparseMat::erase(صحيح i0, صحيح i1, صحيح i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            اكسر;
        previdx = nidx;
        nidx = elem->next;
    }

    אם( nidx )
        removeNode(hidx, nidx, previdx);
}

فراغ SparseMat::erase(ثابت صحيح* idx, size_t* hashval)
{
    CV_Assert( hdr );
    صحيح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    כלעוד( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        אם( elem->hashval == h )
        {
            עבור( i = 0; i < d; i++ )
                אם( elem->idx[i] != idx[i] )
                    اكسر;
            אם( i == d )
                اكسر;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    אם( nidx )
        removeNode(hidx, nidx, previdx);
}

فراغ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    אם((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((مزدوج)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    עבור( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    עבור( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        כלעוד( nidx )
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

uchar* SparseMat::newNode(ثابت صحيح* idx, size_t hashval)
{
    ثابت صحيح HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    אם( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    אם( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        עבור( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    صحيح i, d = hdr->dims;
    עבור( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    אם( esz == حجم(عائم) )
        *((عائم*)p) = 0.f;
    אחרת אם( esz == حجم(مزدوج) )
        *((مزدوج*)p) = 0.;
    אחרת
        memset(p, 0, esz);

    החזר p;
}


فراغ SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    אם( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    אחרת
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}

//
// Operations
//
مزدوج norm( ثابت SparseMat& src, صحيح normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    صحيح type = src.type();
    مزدوج result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    אם( type == CV_32F )
    {
        אם( normType == NORM_INF )
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((مزدوج)it.value<عائم>()));
            }
        אחרת אם( normType == NORM_L1 )
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<عائم>());
            }
        אחרת
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<عائم>();
                result += v*v;
            }
    }
    אחרת אם( type == CV_64F )
    {
        אם( normType == NORM_INF )
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<مزدوج>()));
            }
        אחרת אם( normType == NORM_L1 )
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<مزدوج>());
            }
        אחרת
            עבור( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<مزدوج>();
                result += v*v;
            }
    }
    אחרת
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    אם( normType == NORM_L2 )
        result = std::sqrt(result);
    החזר result;
}

فراغ minMaxLoc( ثابت SparseMat& src, مزدوج* _minval, مزدوج* _maxval, صحيح* _minidx, صحيح* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    صحيح type = src.type();
    ثابت صحيح *minidx = 0, *maxidx = 0;

    אם( type == CV_32F )
    {
        عائم minval = FLT_MAX, maxval = -FLT_MAX;
        עבור( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            عائم v = it.value<عائم>();
            אם( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            אם( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        אם( _minval )
            *_minval = minval;
        אם( _maxval )
            *_maxval = maxval;
    }
    אחרת אם( type == CV_64F )
    {
        مزدوج minval = DBL_MAX, maxval = -DBL_MAX;
        עבור( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            مزدوج v = it.value<مزدوج>();
            אם( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            אם( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        אם( _minval )
            *_minval = minval;
        אם( _maxval )
            *_maxval = maxval;
    }
    אחרת
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    אם( _minidx && minidx )
        עבור( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    אם( _maxidx && maxidx )
        עבור( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


فراغ normalize( ثابت SparseMat& src, SparseMat& dst, مزدوج a, صحيح norm_type )
{
    CV_INSTRUMENT_REGION();

    مزدوج scale = 1;
    אם( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    אחרת
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
