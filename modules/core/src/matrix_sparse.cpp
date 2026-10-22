// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

کينډۍ<ډول_نوم T1, ډول_نوم T2> فراغ
convert_معلومات(ثابت فراغ* _from, فراغ* منزل_2, صحيح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* منزل = (T2*)منزل_2;
    إذا( cn == 1 )
        *منزل = saturate_cast<T2>(*from);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            منزل[i] = saturate_cast<T2>(from[i]);
}

کينډۍ<ډول_نوم T1, ډول_نوم T2> فراغ
convert_Scale_معلومات(ثابت فراغ* _from, فراغ* منزل_2, صحيح cn, مزدوج alpha, مزدوج beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* منزل = (T2*)منزل_2;
    إذا( cn == 1 )
        *منزل = saturate_cast<T2>(*from*alpha + beta);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            منزل[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

تعريفنوع فراغ (*Convert_معلومات)(ثابت فراغ* from, فراغ* منزل, صحيح cn);
تعريفنوع فراغ (*Convert_Scale_معلومات)(ثابت فراغ* from, فراغ* منزل, صحيح cn, مزدوج alpha, مزدوج beta);

ساكن Convert_معلومات getConvertElem(صحيح fromType, صحيح منزل_Type)
{
    ساكن Convert_معلومات tab[CV_ژوروالی_MAX][CV_ژوروالی_MAX] =
    {{ convert_معلومات<uchar, uchar>, convert_معلومات<uchar, schar>,
      convert_معلومات<uchar, ushort>, convert_معلومات<uchar, لنډ>,
      convert_معلومات<uchar, صحيح>, convert_معلومات<uchar, عائم>,
      convert_معلومات<uchar, مزدوج>, 0 },

    { convert_معلومات<schar, uchar>, convert_معلومات<schar, schar>,
      convert_معلومات<schar, ushort>, convert_معلومات<schar, لنډ>,
      convert_معلومات<schar, صحيح>, convert_معلومات<schar, عائم>,
      convert_معلومات<schar, مزدوج>, 0 },

    { convert_معلومات<ushort, uchar>, convert_معلومات<ushort, schar>,
      convert_معلومات<ushort, ushort>, convert_معلومات<ushort, لنډ>,
      convert_معلومات<ushort, صحيح>, convert_معلومات<ushort, عائم>,
      convert_معلومات<ushort, مزدوج>, 0 },

    { convert_معلومات<لنډ, uchar>, convert_معلومات<لنډ, schar>,
      convert_معلومات<لنډ, ushort>, convert_معلومات<لنډ, لنډ>,
      convert_معلومات<لنډ, صحيح>, convert_معلومات<لنډ, عائم>,
      convert_معلومات<لنډ, مزدوج>, 0 },

    { convert_معلومات<صحيح, uchar>, convert_معلومات<صحيح, schar>,
      convert_معلومات<صحيح, ushort>, convert_معلومات<صحيح, لنډ>,
      convert_معلومات<صحيح, صحيح>, convert_معلومات<صحيح, عائم>,
      convert_معلومات<صحيح, مزدوج>, 0 },

    { convert_معلومات<عائم, uchar>, convert_معلومات<عائم, schar>,
      convert_معلومات<عائم, ushort>, convert_معلومات<عائم, لنډ>,
      convert_معلومات<عائم, صحيح>, convert_معلومات<عائم, عائم>,
      convert_معلومات<عائم, مزدوج>, 0 },

    { convert_معلومات<مزدوج, uchar>, convert_معلومات<مزدوج, schar>,
      convert_معلومات<مزدوج, ushort>, convert_معلومات<مزدوج, لنډ>,
      convert_معلومات<مزدوج, صحيح>, convert_معلومات<مزدوج, عائم>,
      convert_معلومات<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_معلومات func = tab[CV_MAT_ژوروالی(fromType)][CV_MAT_ژوروالی(منزل_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

ساكن Convert_Scale_معلومات getConvertScaleElem(صحيح fromType, صحيح منزل_Type)
{
    ساكن Convert_Scale_معلومات tab[CV_ژوروالی_MAX][CV_ژوروالی_MAX] =
    {{ convert_Scale_معلومات<uchar, uchar>, convert_Scale_معلومات<uchar, schar>,
      convert_Scale_معلومات<uchar, ushort>, convert_Scale_معلومات<uchar, لنډ>,
      convert_Scale_معلومات<uchar, صحيح>, convert_Scale_معلومات<uchar, عائم>,
      convert_Scale_معلومات<uchar, مزدوج>, 0 },

    { convert_Scale_معلومات<schar, uchar>, convert_Scale_معلومات<schar, schar>,
      convert_Scale_معلومات<schar, ushort>, convert_Scale_معلومات<schar, لنډ>,
      convert_Scale_معلومات<schar, صحيح>, convert_Scale_معلومات<schar, عائم>,
      convert_Scale_معلومات<schar, مزدوج>, 0 },

    { convert_Scale_معلومات<ushort, uchar>, convert_Scale_معلومات<ushort, schar>,
      convert_Scale_معلومات<ushort, ushort>, convert_Scale_معلومات<ushort, لنډ>,
      convert_Scale_معلومات<ushort, صحيح>, convert_Scale_معلومات<ushort, عائم>,
      convert_Scale_معلومات<ushort, مزدوج>, 0 },

    { convert_Scale_معلومات<لنډ, uchar>, convert_Scale_معلومات<لنډ, schar>,
      convert_Scale_معلومات<لنډ, ushort>, convert_Scale_معلومات<لنډ, لنډ>,
      convert_Scale_معلومات<لنډ, صحيح>, convert_Scale_معلومات<لنډ, عائم>,
      convert_Scale_معلومات<لنډ, مزدوج>, 0 },

    { convert_Scale_معلومات<صحيح, uchar>, convert_Scale_معلومات<صحيح, schar>,
      convert_Scale_معلومات<صحيح, ushort>, convert_Scale_معلومات<صحيح, لنډ>,
      convert_Scale_معلومات<صحيح, صحيح>, convert_Scale_معلومات<صحيح, عائم>,
      convert_Scale_معلومات<صحيح, مزدوج>, 0 },

    { convert_Scale_معلومات<عائم, uchar>, convert_Scale_معلومات<عائم, schar>,
      convert_Scale_معلومات<عائم, ushort>, convert_Scale_معلومات<عائم, لنډ>,
      convert_Scale_معلومات<عائم, صحيح>, convert_Scale_معلومات<عائم, عائم>,
      convert_Scale_معلومات<عائم, مزدوج>, 0 },

    { convert_Scale_معلومات<مزدوج, uchar>, convert_Scale_معلومات<مزدوج, schar>,
      convert_Scale_معلومات<مزدوج, ushort>, convert_Scale_معلومات<مزدوج, لنډ>,
      convert_Scale_معلومات<مزدوج, صحيح>, convert_Scale_معلومات<مزدوج, عائم>,
      convert_Scale_معلومات<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_معلومات func = tab[CV_MAT_ژوروالی(fromType)][CV_MAT_ژوروالی(منزل_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

تعداد { HASH_SIZE0 = 8 };

ساكن مضمن فراغ copyElem(ثابت uchar* from, uchar* منزل, size_t elem_اندازه)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_اندازه; i += حجم(صحيح) )
        *(صحيح*)(منزل + i) = *(ثابت صحيح*)(from + i);
    لكل( ; i < elem_اندازه; i++ )
        منزل[i] = from[i];
}

ساكن مضمن منطقي isZeroElem(ثابت uchar* معلومات, size_t elem_اندازه)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_اندازه; i += حجم(صحيح) )
        إذا( *(صحيح*)(معلومات + i) != 0 )
            أرجع خطأ;
    لكل( ; i < elem_اندازه; i++ )
        إذا( معلومات[i] != 0 )
            أرجع خطأ;
    أرجع ریښتیا;
}

SparseMat::Hdr::Hdr( صحيح _dims, ثابت صحيح* _sizes, صحيح _type )
{
    refcount = 1;

    dims = _dims;
    ارزښت_دننه_واټن = (صحيح)align_اندازه(حجم(SparseMat::Node) - MAX_DIM*حجم(صحيح) +
                                 dims*حجم(صحيح), CV_ELEM_SIZE1(_type));
    غوټه_اندازه = align_اندازه(ارزښت_دننه_واټن +
        CV_ELEM_اندازه(_type), (صحيح)حجم(size_t));

    صحيح i;
    لكل( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    لكل( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

فراغ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#إذا defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wstringop-overflow"
#د_که_پای
    pool.resize(غوټه_اندازه);
#إذا defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#لارښوونه GCC diagnostic pop
#د_که_پای
    غوټه_شمېر = freeList = 0;
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

SparseMat& SparseMat::چلوونکی = (ثابت SparseMat& m)
{
    إذا( هذا != &m )
    {
        إذا( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    أرجع *هذا;
}

SparseMat& SparseMat::چلوونکی=(ثابت Mat& m)
{
    أرجع (*هذا = SparseMat(m));
}

فراغ SparseMat::ټاکل_منزل(SparseMat& m, صحيح _type) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_منزل(m, _type);
}

فراغ SparseMat::addref()
{
    إذا( hdr )
        CV_XADD(&hdr->refcount, 1);
}

فراغ SparseMat::release()
{
    إذا( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        ړنګول hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحيح i0) ثابت
{
    أرجع (size_t)i0;
}

size_t SparseMat::hash(صحيح i0, صحيح i1) ثابت
{
    أرجع (size_t)(بینښه)i0 * HASH_SCALE + (بینښه)i1;
}

size_t SparseMat::hash(صحيح i0, صحيح i1, صحيح i2) ثابت
{
    أرجع ((size_t)(بینښه)i0 * HASH_SCALE + (بینښه)i1) * HASH_SCALE + (بینښه)i2;
}

size_t SparseMat::hash(ثابت صحيح* idx) ثابت
{
    size_t h = (بینښه)idx[0];
    إذا( !hdr )
        أرجع 0;
    صحيح d = hdr->dims;
    لكل(صحيح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (بینښه)idx[i];
    أرجع h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحيح i, idx[CV_MAX_DIM] = {0}, d = m.dims, وروستی_اندازه = m.size[d - 1];
    size_t esz = m.elem_اندازه();
    ثابت uchar* dptr = m.ptr();

    لكل(;;)
    {
        لكل( i = 0; i < وروستی_اندازه; i++, dptr += esz )
        {
            إذا( isZeroElem(dptr, esz) )
                استمر;
            idx[d-1] = i;
            uchar* منزل = new_غوټه(idx, hash(idx));
            copyElem( dptr, منزل, esz );
        }

        لكل( i = d - 2; i >= 0; i-- )
        {
            dptr += m.ګام[i] - m.size[i+1]*m.ګام[i+1];
            إذا( ++idx[i] < m.size[i] )
                اكسر;
            idx[i] = 0;
        }
        إذا( i < 0 )
            اكسر;
    }
}

فراغ SparseMat::create(صحيح d, ثابت صحيح* _sizes, صحيح _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    لكل( صحيح i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    إذا( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        صحيح i;
        لكل( i = 0; i < d; i++ )
            إذا( _sizes[i] != hdr->size[i] )
                اكسر;
        إذا( i == d )
        {
            clear();
            أرجع;
        }
    }
    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    إذا (hdr && _sizes == hdr->size)
    {
        لكل(صحيح i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = جديد Hdr(d, _sizes, _type);
}

فراغ SparseMat::copy_منزل( SparseMat& m ) ثابت
{
    إذا( hdr == m.hdr )
        أرجع;
    إذا( !hdr )
    {
        m.release();
        أرجع;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_اندازه();

    لكل( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.غوټه();
        uchar* منزل = m.new_غوټه(n->idx, n->hashval);
        copyElem( from.ptr, منزل, esz );
    }
}

فراغ SparseMat::copy_منزل( Mat& m ) ثابت
{
    CV_Assert( hdr );
    صحيح ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_اندازه();

    لكل( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.غوټه();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


فراغ SparseMat::convert_منزل( SparseMat& m, صحيح rtype, مزدوج alpha ) ثابت
{
    صحيح cn = channels();
    إذا( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    إذا( hdr == m.hdr && rtype != type()  )
    {
        SparseMat لنډمهاله_2;
        convert_منزل(لنډمهاله_2, rtype, alpha);
        m = لنډمهاله_2;
        أرجع;
    }

    CV_Assert(hdr != 0);
    إذا( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    إذا( alpha == 1 )
    {
        Convert_معلومات cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.غوټه();
            uchar* منزل = hdr == m.hdr ? from.ptr : m.new_غوټه(n->idx, n->hashval);
            cvtfunc( from.ptr, منزل, cn );
        }
    }
    وإلا
    {
        Convert_Scale_معلومات cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.غوټه();
            uchar* منزل = hdr == m.hdr ? from.ptr : m.new_غوټه(n->idx, n->hashval);
            cvtfunc( from.ptr, منزل, cn, alpha, 0 );
        }
    }
}


فراغ SparseMat::convert_منزل( Mat& m, صحيح rtype, مزدوج alpha, مزدوج beta ) ثابت
{
    صحيح cn = channels();
    إذا( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    إذا( alpha == 1 && beta == 0 )
    {
        Convert_معلومات cvtfunc = getConvertElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.غوټه();
            uchar* منزل = m.ptr(n->idx);
            cvtfunc( from.ptr, منزل, cn );
        }
    }
    وإلا
    {
        Convert_Scale_معلومات cvtfunc = getConvertScaleElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.غوټه();
            uchar* منزل = m.ptr(n->idx);
            cvtfunc( from.ptr, منزل, cn, alpha, beta );
        }
    }
}

فراغ SparseMat::clear()
{
    إذا( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(صحيح i0, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h && elem->idx[0] == i0 )
            أرجع &value<uchar>(elem);
        nidx = elem->next;
    }

    إذا( createMissing )
    {
        صحيح idx[] = { i0 };
        أرجع new_غوټه( idx, h );
    }
    أرجع عدم;
}

uchar* SparseMat::ptr(صحيح i0, صحيح i1, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            أرجع &value<uchar>(elem);
        nidx = elem->next;
    }

    إذا( createMissing )
    {
        صحيح idx[] = { i0, i1 };
        أرجع new_غوټه( idx, h );
    }
    أرجع عدم;
}

uchar* SparseMat::ptr(صحيح i0, صحيح i1, صحيح i2, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            أرجع &value<uchar>(elem);
        nidx = elem->next;
    }

    إذا( createMissing )
    {
        صحيح idx[] = { i0, i1, i2 };
        أرجع new_غوټه( idx, h );
    }
    أرجع عدم;
}

uchar* SparseMat::ptr(ثابت صحيح* idx, منطقي createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    صحيح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h )
        {
            لكل( i = 0; i < d; i++ )
                إذا( elem->idx[i] != idx[i] )
                    اكسر;
            إذا( i == d )
                أرجع &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    أرجع createMissing ? new_غوټه(idx, h) : عدم;
}

فراغ SparseMat::erase(صحيح i0, صحيح i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            اكسر;
        previdx = nidx;
        nidx = elem->next;
    }

    إذا( nidx )
        remove_غوټه(hidx, nidx, previdx);
}

فراغ SparseMat::erase(صحيح i0, صحيح i1, صحيح i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            اكسر;
        previdx = nidx;
        nidx = elem->next;
    }

    إذا( nidx )
        remove_غوټه(hidx, nidx, previdx);
}

فراغ SparseMat::erase(ثابت صحيح* idx, size_t* hashval)
{
    CV_Assert( hdr );
    صحيح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    طالما( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        إذا( elem->hashval == h )
        {
            لكل( i = 0; i < d; i++ )
                إذا( elem->idx[i] != idx[i] )
                    اكسر;
            إذا( i == d )
                اكسر;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    إذا( nidx )
        remove_غوټه(hidx, nidx, previdx);
}

فراغ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    إذا((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::لوګاریتم((مزدوج)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    لكل( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    لكل( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        طالما( nidx )
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

uchar* SparseMat::new_غوټه(ثابت صحيح* idx, size_t hashval)
{
    ثابت صحيح HASH_MAX_FILL_ضریب=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    إذا( ++hdr->غوټه_شمېر > hsize*HASH_MAX_FILL_ضریب )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    إذا( !hdr->freeList )
    {
        size_t i, nsz = hdr->غوټه_اندازه, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        لكل( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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
    لكل( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_اندازه();
    uchar* p = &value<uchar>(elem);
    إذا( esz == حجم(عائم) )
        *((عائم*)p) = 0.f;
    وإلا إذا( esz == حجم(مزدوج) )
        *((مزدوج*)p) = 0.;
    وإلا
        memset(p, 0, esz);

    أرجع p;
}


فراغ SparseMat::remove_غوټه(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = غوټه(nidx);
    إذا( previdx )
    {
        Node* prev = غوټه(previdx);
        prev->next = n->next;
    }
    وإلا
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->غوټه_شمېر;
}

//
// Operations
//
مزدوج norm( ثابت SparseMat& src, صحيح normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_ماسک;
    صحيح type = src.type();
    مزدوج پایله = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    إذا( type == CV_32F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                پایله = std::max(پایله, std::مطلق_قیمت((مزدوج)it.value<عائم>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                پایله += std::مطلق_قیمت(it.value<عائم>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<عائم>();
                پایله += v*v;
            }
    }
    وإلا إذا( type == CV_64F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                پایله = std::max(پایله, std::مطلق_قیمت(it.value<مزدوج>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                پایله += std::مطلق_قیمت(it.value<مزدوج>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<مزدوج>();
                پایله += v*v;
            }
    }
    وإلا
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    إذا( normType == NORM_L2 )
        پایله = std::جذر(پایله);
    أرجع پایله;
}

فراغ minMaxLoc( ثابت SparseMat& src, مزدوج* _minval, مزدوج* _maxval, صحيح* _minidx, صحيح* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    صحيح type = src.type();
    ثابت صحيح *minidx = 0, *maxidx = 0;

    إذا( type == CV_32F )
    {
        عائم minval = FLT_MAX, maxval = -FLT_MAX;
        لكل( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            عائم v = it.value<عائم>();
            إذا( v < minval )
            {
                minval = v;
                minidx = it.غوټه()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.غوټه()->idx;
            }
        }
        إذا( _minval )
            *_minval = minval;
        إذا( _maxval )
            *_maxval = maxval;
    }
    وإلا إذا( type == CV_64F )
    {
        مزدوج minval = DBL_MAX, maxval = -DBL_MAX;
        لكل( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            مزدوج v = it.value<مزدوج>();
            إذا( v < minval )
            {
                minval = v;
                minidx = it.غوټه()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.غوټه()->idx;
            }
        }
        إذا( _minval )
            *_minval = minval;
        إذا( _maxval )
            *_maxval = maxval;
    }
    وإلا
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    إذا( _minidx && minidx )
        لكل( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    إذا( _maxidx && maxidx )
        لكل( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


فراغ normalize( ثابت SparseMat& src, SparseMat& dst, مزدوج a, صحيح norm_type )
{
    CV_INSTRUMENT_REGION();

    مزدوج scale = 1;
    إذا( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_منزل( dst, -1, scale );
}

} // cv::
