// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

فضاینام cv {

template<typename T1, typename T2> تهی
convertData_(ثابت تهی* _from, تهی* _to, صحیح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* to = (T2*)_to;
    اگر( cn == 1 )
        *to = saturate_cast<T2>(*from);
    وگرنه
        برای( صحیح i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> تهی
convertScaleData_(ثابت تهی* _from, تهی* _to, صحیح cn, دوگانه alpha, دوگانه beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* to = (T2*)_to;
    اگر( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    وگرنه
        برای( صحیح i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

تعریفنوع تهی (*ConvertData)(ثابت تهی* from, تهی* to, صحیح cn);
تعریفنوع تهی (*ConvertScaleData)(ثابت تهی* from, تهی* to, صحیح cn, دوگانه alpha, دوگانه beta);

ایستا ConvertData getConvertElem(صحیح fromType, صحیح toType)
{
    ایستا ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, صحیح>, convertData_<uchar, اعشاری>,
      convertData_<uchar, دوگانه>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, صحیح>, convertData_<schar, اعشاری>,
      convertData_<schar, دوگانه>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, صحیح>, convertData_<ushort, اعشاری>,
      convertData_<ushort, دوگانه>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, صحیح>, convertData_<short, اعشاری>,
      convertData_<short, دوگانه>, 0 },

    { convertData_<صحیح, uchar>, convertData_<صحیح, schar>,
      convertData_<صحیح, ushort>, convertData_<صحیح, short>,
      convertData_<صحیح, صحیح>, convertData_<صحیح, اعشاری>,
      convertData_<صحیح, دوگانه>, 0 },

    { convertData_<اعشاری, uchar>, convertData_<اعشاری, schar>,
      convertData_<اعشاری, ushort>, convertData_<اعشاری, short>,
      convertData_<اعشاری, صحیح>, convertData_<اعشاری, اعشاری>,
      convertData_<اعشاری, دوگانه>, 0 },

    { convertData_<دوگانه, uchar>, convertData_<دوگانه, schar>,
      convertData_<دوگانه, ushort>, convertData_<دوگانه, short>,
      convertData_<دوگانه, صحیح>, convertData_<دوگانه, اعشاری>,
      convertData_<دوگانه, دوگانه>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    بازگشت func;
}

ایستا ConvertScaleData getConvertScaleElem(صحیح fromType, صحیح toType)
{
    ایستا ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, صحیح>, convertScaleData_<uchar, اعشاری>,
      convertScaleData_<uchar, دوگانه>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, صحیح>, convertScaleData_<schar, اعشاری>,
      convertScaleData_<schar, دوگانه>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, صحیح>, convertScaleData_<ushort, اعشاری>,
      convertScaleData_<ushort, دوگانه>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, صحیح>, convertScaleData_<short, اعشاری>,
      convertScaleData_<short, دوگانه>, 0 },

    { convertScaleData_<صحیح, uchar>, convertScaleData_<صحیح, schar>,
      convertScaleData_<صحیح, ushort>, convertScaleData_<صحیح, short>,
      convertScaleData_<صحیح, صحیح>, convertScaleData_<صحیح, اعشاری>,
      convertScaleData_<صحیح, دوگانه>, 0 },

    { convertScaleData_<اعشاری, uchar>, convertScaleData_<اعشاری, schar>,
      convertScaleData_<اعشاری, ushort>, convertScaleData_<اعشاری, short>,
      convertScaleData_<اعشاری, صحیح>, convertScaleData_<اعشاری, اعشاری>,
      convertScaleData_<اعشاری, دوگانه>, 0 },

    { convertScaleData_<دوگانه, uchar>, convertScaleData_<دوگانه, schar>,
      convertScaleData_<دوگانه, ushort>, convertScaleData_<دوگانه, short>,
      convertScaleData_<دوگانه, صحیح>, convertScaleData_<دوگانه, اعشاری>,
      convertScaleData_<دوگانه, دوگانه>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    بازگشت func;
}

شمارش { HASH_SIZE0 = 8 };

ایستا درونخطی تهی copyElem(ثابت uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    برای( i = 0; i + اندازه(صحیح) <= elemSize; i += اندازه(صحیح) )
        *(صحیح*)(to + i) = *(ثابت صحیح*)(from + i);
    برای( ; i < elemSize; i++ )
        to[i] = from[i];
}

ایستا درونخطی منطقی isZeroElem(ثابت uchar* data, size_t elemSize)
{
    size_t i;
    برای( i = 0; i + اندازه(صحیح) <= elemSize; i += اندازه(صحیح) )
        اگر( *(صحیح*)(data + i) != 0 )
            بازگشت نادرست;
    برای( ; i < elemSize; i++ )
        اگر( data[i] != 0 )
            بازگشت نادرست;
    بازگشت درست;
}

SparseMat::Hdr::Hdr( صحیح _dims, ثابت صحیح* _sizes, صحیح _type )
{
    refcount = 1;

    dims = _dims;
    مقدار_Offset = (صحیح)alignSize(اندازه(SparseMat::Node) - MAX_DIM*اندازه(صحیح) +
                                 dims*اندازه(صحیح), CV_ELEM_SIZE1(_type));
    گره_Size = alignSize(مقدار_Offset +
        CV_ELEM_SIZE(_type), (صحیح)اندازه(size_t));

    صحیح i;
    برای( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    برای( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

تهی SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#اگر defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(گره_Size);
#اگر defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    گره_شمار = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(صحیح _dims, ثابت صحیح* _sizes, صحیح _type)
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
    اگر( این != &m )
    {
        اگر( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    بازگشت *این;
}

SparseMat& SparseMat::operator=(ثابت Mat& m)
{
    بازگشت (*این = SparseMat(m));
}

تهی SparseMat::assignTo(SparseMat& m, صحیح _type) ثابت
{
    اگر( _type < 0 )
        m = *این;
    وگرنه
        convertTo(m, _type);
}

تهی SparseMat::addref()
{
    اگر( hdr )
        CV_XADD(&hdr->refcount, 1);
}

تهی SparseMat::release()
{
    اگر( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحیح i0) ثابت
{
    بازگشت (size_t)i0;
}

size_t SparseMat::hash(صحیح i0, صحیح i1) ثابت
{
    بازگشت (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(صحیح i0, صحیح i1, صحیح i2) ثابت
{
    بازگشت ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ثابت صحیح* idx) ثابت
{
    size_t h = (unsigned)idx[0];
    اگر( !hdr )
        بازگشت 0;
    صحیح d = hdr->dims;
    برای(صحیح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    بازگشت h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحیح i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ثابت uchar* dptr = m.ptr();

    برای(;;)
    {
        برای( i = 0; i < lastSize; i++, dptr += esz )
        {
            اگر( isZeroElem(dptr, esz) )
                ادامه;
            idx[d-1] = i;
            uchar* to = new_گره(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        برای( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            اگر( ++idx[i] < m.size[i] )
                بشکن;
            idx[i] = 0;
        }
        اگر( i < 0 )
            بشکن;
    }
}

تهی SparseMat::create(صحیح d, ثابت صحیح* _sizes, صحیح _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    برای( صحیح i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    اگر( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        صحیح i;
        برای( i = 0; i < d; i++ )
            اگر( _sizes[i] != hdr->size[i] )
                بشکن;
        اگر( i == d )
        {
            clear();
            بازگشت;
        }
    }
    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (hdr && _sizes == hdr->size)
    {
        برای(صحیح i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = جدید Hdr(d, _sizes, _type);
}

تهی SparseMat::copyTo( SparseMat& m ) ثابت
{
    اگر( hdr == m.hdr )
        بازگشت;
    اگر( !hdr )
    {
        m.release();
        بازگشت;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    برای( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گره();
        uchar* to = m.new_گره(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

تهی SparseMat::copyTo( Mat& m ) ثابت
{
    CV_Assert( hdr );
    صحیح ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    برای( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گره();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


تهی SparseMat::convertTo( SparseMat& m, صحیح rtype, دوگانه alpha ) ثابت
{
    صحیح cn = channels();
    اگر( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    اگر( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        بازگشت;
    }

    CV_Assert(hdr != 0);
    اگر( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    اگر( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        برای( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_گره(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    وگرنه
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        برای( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_گره(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


تهی SparseMat::convertTo( Mat& m, صحیح rtype, دوگانه alpha, دوگانه beta ) ثابت
{
    صحیح cn = channels();
    اگر( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    اگر( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        برای( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    وگرنه
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        برای( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

تهی SparseMat::clear()
{
    اگر( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(صحیح i0, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 )
            بازگشت &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0 };
        بازگشت new_گره( idx, h );
    }
    بازگشت پوچ;
}

uchar* SparseMat::ptr(صحیح i0, صحیح i1, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            بازگشت &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0, i1 };
        بازگشت new_گره( idx, h );
    }
    بازگشت پوچ;
}

uchar* SparseMat::ptr(صحیح i0, صحیح i1, صحیح i2, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            بازگشت &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0, i1, i2 };
        بازگشت new_گره( idx, h );
    }
    بازگشت پوچ;
}

uchar* SparseMat::ptr(ثابت صحیح* idx, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    صحیح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h )
        {
            برای( i = 0; i < d; i++ )
                اگر( elem->idx[i] != idx[i] )
                    بشکن;
            اگر( i == d )
                بازگشت &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    بازگشت createMissing ? new_گره(idx, h) : پوچ;
}

تهی SparseMat::erase(صحیح i0, صحیح i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            بشکن;
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گره(hidx, nidx, previdx);
}

تهی SparseMat::erase(صحیح i0, صحیح i1, صحیح i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            بشکن;
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گره(hidx, nidx, previdx);
}

تهی SparseMat::erase(ثابت صحیح* idx, size_t* hashval)
{
    CV_Assert( hdr );
    صحیح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    وقتی( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h )
        {
            برای( i = 0; i < d; i++ )
                اگر( elem->idx[i] != idx[i] )
                    بشکن;
            اگر( i == d )
                بشکن;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گره(hidx, nidx, previdx);
}

تهی SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    اگر((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((دوگانه)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    برای( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    برای( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        وقتی( nidx )
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

uchar* SparseMat::new_گره(ثابت صحیح* idx, size_t hashval)
{
    ثابت صحیح HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    اگر( ++hdr->گره_شمار > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    اگر( !hdr->freeList )
    {
        size_t i, nsz = hdr->گره_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        برای( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    صحیح i, d = hdr->dims;
    برای( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    اگر( esz == اندازه(اعشاری) )
        *((اعشاری*)p) = 0.f;
    وگرنه اگر( esz == اندازه(دوگانه) )
        *((دوگانه*)p) = 0.;
    وگرنه
        memset(p, 0, esz);

    بازگشت p;
}


تهی SparseMat::remove_گره(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = گره(nidx);
    اگر( previdx )
    {
        Node* prev = گره(previdx);
        prev->next = n->next;
    }
    وگرنه
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->گره_شمار;
}

//
// Operations
//
دوگانه norm( ثابت SparseMat& src, صحیح normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    صحیح type = src.type();
    دوگانه result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    اگر( type == CV_32F )
    {
        اگر( normType == NORM_INF )
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((دوگانه)it.value<اعشاری>()));
            }
        وگرنه اگر( normType == NORM_L1 )
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<اعشاری>());
            }
        وگرنه
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                دوگانه v = it.value<اعشاری>();
                result += v*v;
            }
    }
    وگرنه اگر( type == CV_64F )
    {
        اگر( normType == NORM_INF )
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<دوگانه>()));
            }
        وگرنه اگر( normType == NORM_L1 )
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<دوگانه>());
            }
        وگرنه
            برای( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                دوگانه v = it.value<دوگانه>();
                result += v*v;
            }
    }
    وگرنه
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    اگر( normType == NORM_L2 )
        result = std::sqrt(result);
    بازگشت result;
}

تهی minMaxLoc( ثابت SparseMat& src, دوگانه* _minval, دوگانه* _maxval, صحیح* _minidx, صحیح* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    صحیح type = src.type();
    ثابت صحیح *minidx = 0, *maxidx = 0;

    اگر( type == CV_32F )
    {
        اعشاری minval = FLT_MAX, maxval = -FLT_MAX;
        برای( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            اعشاری v = it.value<اعشاری>();
            اگر( v < minval )
            {
                minval = v;
                minidx = it.گره()->idx;
            }
            اگر( v > maxval )
            {
                maxval = v;
                maxidx = it.گره()->idx;
            }
        }
        اگر( _minval )
            *_minval = minval;
        اگر( _maxval )
            *_maxval = maxval;
    }
    وگرنه اگر( type == CV_64F )
    {
        دوگانه minval = DBL_MAX, maxval = -DBL_MAX;
        برای( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            دوگانه v = it.value<دوگانه>();
            اگر( v < minval )
            {
                minval = v;
                minidx = it.گره()->idx;
            }
            اگر( v > maxval )
            {
                maxval = v;
                maxidx = it.گره()->idx;
            }
        }
        اگر( _minval )
            *_minval = minval;
        اگر( _maxval )
            *_maxval = maxval;
    }
    وگرنه
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    اگر( _minidx && minidx )
        برای( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    اگر( _maxidx && maxidx )
        برای( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


تهی normalize( ثابت SparseMat& src, SparseMat& dst, دوگانه a, صحیح norm_type )
{
    CV_INSTRUMENT_REGION();

    دوگانه scale = 1;
    اگر( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    وگرنه
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
