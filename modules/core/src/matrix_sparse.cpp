// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/mat.hpp"

நாமாகாஶ cv {

template<typename T1, typename T2> வெறுமை
convertData_(மாறிலி வெறுமை* _from, வெறுமை* _to, முழு cn)
{
    மாறிலி T1* from = (மாறிலி T1*)_from;
    T2* to = (T2*)_to;
    எனில்( cn == 1 )
        *to = saturate_cast<T2>(*from);
    இல்லையெனில்
        க்கு( முழு i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> வெறுமை
convertScaleData_(மாறிலி வெறுமை* _from, வெறுமை* _to, முழு cn, இரட்டை alpha, இரட்டை beta)
{
    மாறிலி T1* from = (மாறிலி T1*)_from;
    T2* to = (T2*)_to;
    எனில்( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    இல்லையெனில்
        க்கு( முழு i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

வகைவரை வெறுமை (*ConvertData)(மாறிலி வெறுமை* from, வெறுமை* to, முழு cn);
வகைவரை வெறுமை (*ConvertScaleData)(மாறிலி வெறுமை* from, வெறுமை* to, முழு cn, இரட்டை alpha, இரட்டை beta);

நிலையான ConvertData getConvertElem(முழு fromType, முழு toType)
{
    நிலையான ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, முழு>, convertData_<uchar, தசம>,
      convertData_<uchar, இரட்டை>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, முழு>, convertData_<schar, தசம>,
      convertData_<schar, இரட்டை>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, முழு>, convertData_<ushort, தசம>,
      convertData_<ushort, இரட்டை>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, முழு>, convertData_<short, தசம>,
      convertData_<short, இரட்டை>, 0 },

    { convertData_<முழு, uchar>, convertData_<முழு, schar>,
      convertData_<முழு, ushort>, convertData_<முழு, short>,
      convertData_<முழு, முழு>, convertData_<முழு, தசம>,
      convertData_<முழு, இரட்டை>, 0 },

    { convertData_<தசம, uchar>, convertData_<தசம, schar>,
      convertData_<தசம, ushort>, convertData_<தசம, short>,
      convertData_<தசம, முழு>, convertData_<தசம, தசம>,
      convertData_<தசம, இரட்டை>, 0 },

    { convertData_<இரட்டை, uchar>, convertData_<இரட்டை, schar>,
      convertData_<இரட்டை, ushort>, convertData_<இரட்டை, short>,
      convertData_<இரட்டை, முழு>, convertData_<இரட்டை, தசம>,
      convertData_<இரட்டை, இரட்டை>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    திரும்பு func;
}

நிலையான ConvertScaleData getConvertScaleElem(முழு fromType, முழு toType)
{
    நிலையான ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, முழு>, convertScaleData_<uchar, தசம>,
      convertScaleData_<uchar, இரட்டை>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, முழு>, convertScaleData_<schar, தசம>,
      convertScaleData_<schar, இரட்டை>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, முழு>, convertScaleData_<ushort, தசம>,
      convertScaleData_<ushort, இரட்டை>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, முழு>, convertScaleData_<short, தசம>,
      convertScaleData_<short, இரட்டை>, 0 },

    { convertScaleData_<முழு, uchar>, convertScaleData_<முழு, schar>,
      convertScaleData_<முழு, ushort>, convertScaleData_<முழு, short>,
      convertScaleData_<முழு, முழு>, convertScaleData_<முழு, தசம>,
      convertScaleData_<முழு, இரட்டை>, 0 },

    { convertScaleData_<தசம, uchar>, convertScaleData_<தசம, schar>,
      convertScaleData_<தசம, ushort>, convertScaleData_<தசம, short>,
      convertScaleData_<தசம, முழு>, convertScaleData_<தசம, தசம>,
      convertScaleData_<தசம, இரட்டை>, 0 },

    { convertScaleData_<இரட்டை, uchar>, convertScaleData_<இரட்டை, schar>,
      convertScaleData_<இரட்டை, ushort>, convertScaleData_<இரட்டை, short>,
      convertScaleData_<இரட்டை, முழு>, convertScaleData_<இரட்டை, தசம>,
      convertScaleData_<இரட்டை, இரட்டை>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    திரும்பு func;
}

எண்ணிக்கை { HASH_SIZE0 = 8 };

நிலையான உள்வரி வெறுமை copyElem(மாறிலி uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    க்கு( i = 0; i + அளவு(முழு) <= elemSize; i += அளவு(முழு) )
        *(முழு*)(to + i) = *(மாறிலி முழு*)(from + i);
    க்கு( ; i < elemSize; i++ )
        to[i] = from[i];
}

நிலையான உள்வரி தருக்க isZeroElem(மாறிலி uchar* data, size_t elemSize)
{
    size_t i;
    க்கு( i = 0; i + அளவு(முழு) <= elemSize; i += அளவு(முழு) )
        எனில்( *(முழு*)(data + i) != 0 )
            திரும்பு பொய்;
    க்கு( ; i < elemSize; i++ )
        எனில்( data[i] != 0 )
            திரும்பு பொய்;
    திரும்பு உண்மை;
}

SparseMat::Hdr::Hdr( முழு _dims, மாறிலி முழு* _sizes, முழு _type )
{
    refcount = 1;

    dims = _dims;
    மதிப்பு_Offset = (முழு)alignSize(அளவு(SparseMat::Node) - MAX_DIM*அளவு(முழு) +
                                 dims*அளவு(முழு), CV_ELEM_SIZE1(_type));
    முனை_Size = alignSize(மதிப்பு_Offset +
        CV_ELEM_SIZE(_type), (முழு)அளவு(size_t));

    முழு i;
    க்கு( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    க்கு( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

வெறுமை SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#எனில் defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(முனை_Size);
#எனில் defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    முனை_மொத்தம் = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(முழு _dims, மாறிலி முழு* _sizes, முழு _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(மாறிலி SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (மாறிலி SparseMat& m)
{
    எனில்( இது != &m )
    {
        எனில்( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    திரும்பு *இது;
}

SparseMat& SparseMat::operator=(மாறிலி Mat& m)
{
    திரும்பு (*இது = SparseMat(m));
}

வெறுமை SparseMat::assignTo(SparseMat& m, முழு _type) மாறிலி
{
    எனில்( _type < 0 )
        m = *இது;
    இல்லையெனில்
        convertTo(m, _type);
}

வெறுமை SparseMat::addref()
{
    எனில்( hdr )
        CV_XADD(&hdr->refcount, 1);
}

வெறுமை SparseMat::release()
{
    எனில்( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(முழு i0) மாறிலி
{
    திரும்பு (size_t)i0;
}

size_t SparseMat::hash(முழு i0, முழு i1) மாறிலி
{
    திரும்பு (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(முழு i0, முழு i1, முழு i2) மாறிலி
{
    திரும்பு ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(மாறிலி முழு* idx) மாறிலி
{
    size_t h = (unsigned)idx[0];
    எனில்( !hdr )
        திரும்பு 0;
    முழு d = hdr->dims;
    க்கு(முழு i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    திரும்பு h;
}


SparseMat::SparseMat(மாறிலி Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    முழு i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    மாறிலி uchar* dptr = m.ptr();

    க்கு(;;)
    {
        க்கு( i = 0; i < lastSize; i++, dptr += esz )
        {
            எனில்( isZeroElem(dptr, esz) )
                தொடர்;
            idx[d-1] = i;
            uchar* to = new_முனை(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        க்கு( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            எனில்( ++idx[i] < m.size[i] )
                நிறுத்து;
            idx[i] = 0;
        }
        எனில்( i < 0 )
            நிறுத்து;
    }
}

வெறுமை SparseMat::create(முழு d, மாறிலி முழு* _sizes, முழு _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    க்கு( முழு i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    எனில்( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        முழு i;
        க்கு( i = 0; i < d; i++ )
            எனில்( _sizes[i] != hdr->size[i] )
                நிறுத்து;
        எனில்( i == d )
        {
            clear();
            திரும்பு;
        }
    }
    முழு _sizes_backup[CV_MAX_DIM]; // #5991
    எனில் (hdr && _sizes == hdr->size)
    {
        க்கு(முழு i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = புதிய Hdr(d, _sizes, _type);
}

வெறுமை SparseMat::copyTo( SparseMat& m ) மாறிலி
{
    எனில்( hdr == m.hdr )
        திரும்பு;
    எனில்( !hdr )
    {
        m.release();
        திரும்பு;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    க்கு( size_t i = 0; i < N; i++, ++from )
    {
        மாறிலி Node* n = from.முனை();
        uchar* to = m.new_முனை(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

வெறுமை SparseMat::copyTo( Mat& m ) மாறிலி
{
    CV_Assert( hdr );
    முழு ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    க்கு( size_t i = 0; i < N; i++, ++from )
    {
        மாறிலி Node* n = from.முனை();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


வெறுமை SparseMat::convertTo( SparseMat& m, முழு rtype, இரட்டை alpha ) மாறிலி
{
    முழு cn = channels();
    எனில்( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    எனில்( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        திரும்பு;
    }

    CV_Assert(hdr != 0);
    எனில்( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    எனில்( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        க்கு( size_t i = 0; i < N; i++, ++from )
        {
            மாறிலி Node* n = from.முனை();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_முனை(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    இல்லையெனில்
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        க்கு( size_t i = 0; i < N; i++, ++from )
        {
            மாறிலி Node* n = from.முனை();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_முனை(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


வெறுமை SparseMat::convertTo( Mat& m, முழு rtype, இரட்டை alpha, இரட்டை beta ) மாறிலி
{
    முழு cn = channels();
    எனில்( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    எனில்( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        க்கு( size_t i = 0; i < N; i++, ++from )
        {
            மாறிலி Node* n = from.முனை();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    இல்லையெனில்
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        க்கு( size_t i = 0; i < N; i++, ++from )
        {
            மாறிலி Node* n = from.முனை();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

வெறுமை SparseMat::clear()
{
    எனில்( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(முழு i0, தருக்க createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h && elem->idx[0] == i0 )
            திரும்பு &value<uchar>(elem);
        nidx = elem->next;
    }

    எனில்( createMissing )
    {
        முழு idx[] = { i0 };
        திரும்பு new_முனை( idx, h );
    }
    திரும்பு வெற்று;
}

uchar* SparseMat::ptr(முழு i0, முழு i1, தருக்க createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            திரும்பு &value<uchar>(elem);
        nidx = elem->next;
    }

    எனில்( createMissing )
    {
        முழு idx[] = { i0, i1 };
        திரும்பு new_முனை( idx, h );
    }
    திரும்பு வெற்று;
}

uchar* SparseMat::ptr(முழு i0, முழு i1, முழு i2, தருக்க createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            திரும்பு &value<uchar>(elem);
        nidx = elem->next;
    }

    எனில்( createMissing )
    {
        முழு idx[] = { i0, i1, i2 };
        திரும்பு new_முனை( idx, h );
    }
    திரும்பு வெற்று;
}

uchar* SparseMat::ptr(மாறிலி முழு* idx, தருக்க createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    முழு i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h )
        {
            க்கு( i = 0; i < d; i++ )
                எனில்( elem->idx[i] != idx[i] )
                    நிறுத்து;
            எனில்( i == d )
                திரும்பு &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    திரும்பு createMissing ? new_முனை(idx, h) : வெற்று;
}

வெறுமை SparseMat::erase(முழு i0, முழு i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            நிறுத்து;
        previdx = nidx;
        nidx = elem->next;
    }

    எனில்( nidx )
        remove_முனை(hidx, nidx, previdx);
}

வெறுமை SparseMat::erase(முழு i0, முழு i1, முழு i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            நிறுத்து;
        previdx = nidx;
        nidx = elem->next;
    }

    எனில்( nidx )
        remove_முனை(hidx, nidx, previdx);
}

வெறுமை SparseMat::erase(மாறிலி முழு* idx, size_t* hashval)
{
    CV_Assert( hdr );
    முழு i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    போது( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        எனில்( elem->hashval == h )
        {
            க்கு( i = 0; i < d; i++ )
                எனில்( elem->idx[i] != idx[i] )
                    நிறுத்து;
            எனில்( i == d )
                நிறுத்து;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    எனில்( nidx )
        remove_முனை(hidx, nidx, previdx);
}

வெறுமை SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    எனில்((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((இரட்டை)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    க்கு( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    க்கு( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        போது( nidx )
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

uchar* SparseMat::new_முனை(மாறிலி முழு* idx, size_t hashval)
{
    மாறிலி முழு HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    எனில்( ++hdr->முனை_மொத்தம் > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    எனில்( !hdr->freeList )
    {
        size_t i, nsz = hdr->முனை_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        க்கு( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    முழு i, d = hdr->dims;
    க்கு( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    எனில்( esz == அளவு(தசம) )
        *((தசம*)p) = 0.f;
    இல்லையெனில் எனில்( esz == அளவு(இரட்டை) )
        *((இரட்டை*)p) = 0.;
    இல்லையெனில்
        memset(p, 0, esz);

    திரும்பு p;
}


வெறுமை SparseMat::remove_முனை(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = முனை(nidx);
    எனில்( previdx )
    {
        Node* prev = முனை(previdx);
        prev->next = n->next;
    }
    இல்லையெனில்
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->முனை_மொத்தம்;
}

//
// Operations
//
இரட்டை norm( மாறிலி SparseMat& src, முழு normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    முழு type = src.type();
    இரட்டை result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    எனில்( type == CV_32F )
    {
        எனில்( normType == NORM_INF )
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((இரட்டை)it.value<தசம>()));
            }
        இல்லையெனில் எனில்( normType == NORM_L1 )
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<தசம>());
            }
        இல்லையெனில்
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                இரட்டை v = it.value<தசம>();
                result += v*v;
            }
    }
    இல்லையெனில் எனில்( type == CV_64F )
    {
        எனில்( normType == NORM_INF )
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<இரட்டை>()));
            }
        இல்லையெனில் எனில்( normType == NORM_L1 )
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<இரட்டை>());
            }
        இல்லையெனில்
            க்கு( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                இரட்டை v = it.value<இரட்டை>();
                result += v*v;
            }
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    எனில்( normType == NORM_L2 )
        result = std::sqrt(result);
    திரும்பு result;
}

வெறுமை minMaxLoc( மாறிலி SparseMat& src, இரட்டை* _minval, இரட்டை* _maxval, முழு* _minidx, முழு* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    முழு type = src.type();
    மாறிலி முழு *minidx = 0, *maxidx = 0;

    எனில்( type == CV_32F )
    {
        தசம minval = FLT_MAX, maxval = -FLT_MAX;
        க்கு( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            தசம v = it.value<தசம>();
            எனில்( v < minval )
            {
                minval = v;
                minidx = it.முனை()->idx;
            }
            எனில்( v > maxval )
            {
                maxval = v;
                maxidx = it.முனை()->idx;
            }
        }
        எனில்( _minval )
            *_minval = minval;
        எனில்( _maxval )
            *_maxval = maxval;
    }
    இல்லையெனில் எனில்( type == CV_64F )
    {
        இரட்டை minval = DBL_MAX, maxval = -DBL_MAX;
        க்கு( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            இரட்டை v = it.value<இரட்டை>();
            எனில்( v < minval )
            {
                minval = v;
                minidx = it.முனை()->idx;
            }
            எனில்( v > maxval )
            {
                maxval = v;
                maxidx = it.முனை()->idx;
            }
        }
        எனில்( _minval )
            *_minval = minval;
        எனில்( _maxval )
            *_maxval = maxval;
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    எனில்( _minidx && minidx )
        க்கு( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    எனில்( _maxidx && maxidx )
        க்கு( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


வெறுமை normalize( மாறிலி SparseMat& src, SparseMat& dst, இரட்டை a, முழு norm_type )
{
    CV_INSTRUMENT_REGION();

    இரட்டை scale = 1;
    எனில்( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
