// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

نامخلا cv {

سانچہ<قسم_نام T1, قسم_نام T2> خالی
convert_ڈیٹا(ثابت خالی* _from, خالی* مقصد_2, صحیح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* مقصد = (T2*)مقصد_2;
    اگر( cn == 1 )
        *مقصد = saturate_cast<T2>(*from);
    ورنہ
        برائے( صحیح i = 0; i < cn; i++ )
            مقصد[i] = saturate_cast<T2>(from[i]);
}

سانچہ<قسم_نام T1, قسم_نام T2> خالی
convert_Scale_ڈیٹا(ثابت خالی* _from, خالی* مقصد_2, صحیح cn, دوہرا alpha, دوہرا beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* مقصد = (T2*)مقصد_2;
    اگر( cn == 1 )
        *مقصد = saturate_cast<T2>(*from*alpha + beta);
    ورنہ
        برائے( صحیح i = 0; i < cn; i++ )
            مقصد[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

قسمتعریف خالی (*Convert_ڈیٹا)(ثابت خالی* from, خالی* مقصد, صحیح cn);
قسمتعریف خالی (*Convert_Scale_ڈیٹا)(ثابت خالی* from, خالی* مقصد, صحیح cn, دوہرا alpha, دوہرا beta);

ساکن Convert_ڈیٹا getConvertElem(صحیح fromType, صحیح مقصد_Type)
{
    ساکن Convert_ڈیٹا tab[CV_گہرائی_MAX][CV_گہرائی_MAX] =
    {{ convert_ڈیٹا<uchar, uchar>, convert_ڈیٹا<uchar, schar>,
      convert_ڈیٹا<uchar, ushort>, convert_ڈیٹا<uchar, مختصر>,
      convert_ڈیٹا<uchar, صحیح>, convert_ڈیٹا<uchar, اعشاری>,
      convert_ڈیٹا<uchar, دوہرا>, 0 },

    { convert_ڈیٹا<schar, uchar>, convert_ڈیٹا<schar, schar>,
      convert_ڈیٹا<schar, ushort>, convert_ڈیٹا<schar, مختصر>,
      convert_ڈیٹا<schar, صحیح>, convert_ڈیٹا<schar, اعشاری>,
      convert_ڈیٹا<schar, دوہرا>, 0 },

    { convert_ڈیٹا<ushort, uchar>, convert_ڈیٹا<ushort, schar>,
      convert_ڈیٹا<ushort, ushort>, convert_ڈیٹا<ushort, مختصر>,
      convert_ڈیٹا<ushort, صحیح>, convert_ڈیٹا<ushort, اعشاری>,
      convert_ڈیٹا<ushort, دوہرا>, 0 },

    { convert_ڈیٹا<مختصر, uchar>, convert_ڈیٹا<مختصر, schar>,
      convert_ڈیٹا<مختصر, ushort>, convert_ڈیٹا<مختصر, مختصر>,
      convert_ڈیٹا<مختصر, صحیح>, convert_ڈیٹا<مختصر, اعشاری>,
      convert_ڈیٹا<مختصر, دوہرا>, 0 },

    { convert_ڈیٹا<صحیح, uchar>, convert_ڈیٹا<صحیح, schar>,
      convert_ڈیٹا<صحیح, ushort>, convert_ڈیٹا<صحیح, مختصر>,
      convert_ڈیٹا<صحیح, صحیح>, convert_ڈیٹا<صحیح, اعشاری>,
      convert_ڈیٹا<صحیح, دوہرا>, 0 },

    { convert_ڈیٹا<اعشاری, uchar>, convert_ڈیٹا<اعشاری, schar>,
      convert_ڈیٹا<اعشاری, ushort>, convert_ڈیٹا<اعشاری, مختصر>,
      convert_ڈیٹا<اعشاری, صحیح>, convert_ڈیٹا<اعشاری, اعشاری>,
      convert_ڈیٹا<اعشاری, دوہرا>, 0 },

    { convert_ڈیٹا<دوہرا, uchar>, convert_ڈیٹا<دوہرا, schar>,
      convert_ڈیٹا<دوہرا, ushort>, convert_ڈیٹا<دوہرا, مختصر>,
      convert_ڈیٹا<دوہرا, صحیح>, convert_ڈیٹا<دوہرا, اعشاری>,
      convert_ڈیٹا<دوہرا, دوہرا>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_ڈیٹا func = tab[CV_MAT_گہرائی(fromType)][CV_MAT_گہرائی(مقصد_Type)];
    CV_Assert( func != 0 );
    واپس func;
}

ساکن Convert_Scale_ڈیٹا getConvertScaleElem(صحیح fromType, صحیح مقصد_Type)
{
    ساکن Convert_Scale_ڈیٹا tab[CV_گہرائی_MAX][CV_گہرائی_MAX] =
    {{ convert_Scale_ڈیٹا<uchar, uchar>, convert_Scale_ڈیٹا<uchar, schar>,
      convert_Scale_ڈیٹا<uchar, ushort>, convert_Scale_ڈیٹا<uchar, مختصر>,
      convert_Scale_ڈیٹا<uchar, صحیح>, convert_Scale_ڈیٹا<uchar, اعشاری>,
      convert_Scale_ڈیٹا<uchar, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<schar, uchar>, convert_Scale_ڈیٹا<schar, schar>,
      convert_Scale_ڈیٹا<schar, ushort>, convert_Scale_ڈیٹا<schar, مختصر>,
      convert_Scale_ڈیٹا<schar, صحیح>, convert_Scale_ڈیٹا<schar, اعشاری>,
      convert_Scale_ڈیٹا<schar, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<ushort, uchar>, convert_Scale_ڈیٹا<ushort, schar>,
      convert_Scale_ڈیٹا<ushort, ushort>, convert_Scale_ڈیٹا<ushort, مختصر>,
      convert_Scale_ڈیٹا<ushort, صحیح>, convert_Scale_ڈیٹا<ushort, اعشاری>,
      convert_Scale_ڈیٹا<ushort, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<مختصر, uchar>, convert_Scale_ڈیٹا<مختصر, schar>,
      convert_Scale_ڈیٹا<مختصر, ushort>, convert_Scale_ڈیٹا<مختصر, مختصر>,
      convert_Scale_ڈیٹا<مختصر, صحیح>, convert_Scale_ڈیٹا<مختصر, اعشاری>,
      convert_Scale_ڈیٹا<مختصر, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<صحیح, uchar>, convert_Scale_ڈیٹا<صحیح, schar>,
      convert_Scale_ڈیٹا<صحیح, ushort>, convert_Scale_ڈیٹا<صحیح, مختصر>,
      convert_Scale_ڈیٹا<صحیح, صحیح>, convert_Scale_ڈیٹا<صحیح, اعشاری>,
      convert_Scale_ڈیٹا<صحیح, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<اعشاری, uchar>, convert_Scale_ڈیٹا<اعشاری, schar>,
      convert_Scale_ڈیٹا<اعشاری, ushort>, convert_Scale_ڈیٹا<اعشاری, مختصر>,
      convert_Scale_ڈیٹا<اعشاری, صحیح>, convert_Scale_ڈیٹا<اعشاری, اعشاری>,
      convert_Scale_ڈیٹا<اعشاری, دوہرا>, 0 },

    { convert_Scale_ڈیٹا<دوہرا, uchar>, convert_Scale_ڈیٹا<دوہرا, schar>,
      convert_Scale_ڈیٹا<دوہرا, ushort>, convert_Scale_ڈیٹا<دوہرا, مختصر>,
      convert_Scale_ڈیٹا<دوہرا, صحیح>, convert_Scale_ڈیٹا<دوہرا, اعشاری>,
      convert_Scale_ڈیٹا<دوہرا, دوہرا>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_ڈیٹا func = tab[CV_MAT_گہرائی(fromType)][CV_MAT_گہرائی(مقصد_Type)];
    CV_Assert( func != 0 );
    واپس func;
}

شمار { HASH_SIZE0 = 8 };

ساکن اندرونی خالی copyElem(ثابت uchar* from, uchar* مقصد, size_t elem_سائز)
{
    size_t i;
    برائے( i = 0; i + حجم(صحیح) <= elem_سائز; i += حجم(صحیح) )
        *(صحیح*)(مقصد + i) = *(ثابت صحیح*)(from + i);
    برائے( ; i < elem_سائز; i++ )
        مقصد[i] = from[i];
}

ساکن اندرونی منطقی isZeroElem(ثابت uchar* ڈیٹا, size_t elem_سائز)
{
    size_t i;
    برائے( i = 0; i + حجم(صحیح) <= elem_سائز; i += حجم(صحیح) )
        اگر( *(صحیح*)(ڈیٹا + i) != 0 )
            واپس جھوٹ;
    برائے( ; i < elem_سائز; i++ )
        اگر( ڈیٹا[i] != 0 )
            واپس جھوٹ;
    واپس سچ;
}

SparseMat::Hdr::Hdr( صحیح _dims, ثابت صحیح* _sizes, صحیح _type )
{
    refcount = 1;

    dims = _dims;
    قدر_انحراف = (صحیح)align_سائز(حجم(SparseMat::Node) - MAX_DIM*حجم(صحیح) +
                                 dims*حجم(صحیح), CV_ELEM_SIZE1(_type));
    گرہ_سائز = align_سائز(قدر_انحراف +
        CV_ELEM_سائز(_type), (صحیح)حجم(size_t));

    صحیح i;
    برائے( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    برائے( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

خالی SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#اگر defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wstringop-overflow"
#اگر_ختم
    pool.resize(گرہ_سائز);
#اگر defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ہدایت GCC diagnostic pop
#اگر_ختم
    گرہ_گنتی = freeList = 0;
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

SparseMat& SparseMat::عامل = (ثابت SparseMat& m)
{
    اگر( یہ != &m )
    {
        اگر( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    واپس *یہ;
}

SparseMat& SparseMat::عامل=(ثابت Mat& m)
{
    واپس (*یہ = SparseMat(m));
}

خالی SparseMat::تفویض_مقصد(SparseMat& m, صحیح _type) ثابت
{
    اگر( _type < 0 )
        m = *یہ;
    ورنہ
        convert_مقصد(m, _type);
}

خالی SparseMat::addref()
{
    اگر( hdr )
        CV_XADD(&hdr->refcount, 1);
}

خالی SparseMat::release()
{
    اگر( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        حذف hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحیح i0) ثابت
{
    واپس (size_t)i0;
}

size_t SparseMat::hash(صحیح i0, صحیح i1) ثابت
{
    واپس (size_t)(غیرعلامتی)i0 * HASH_SCALE + (غیرعلامتی)i1;
}

size_t SparseMat::hash(صحیح i0, صحیح i1, صحیح i2) ثابت
{
    واپس ((size_t)(غیرعلامتی)i0 * HASH_SCALE + (غیرعلامتی)i1) * HASH_SCALE + (غیرعلامتی)i2;
}

size_t SparseMat::hash(ثابت صحیح* idx) ثابت
{
    size_t h = (غیرعلامتی)idx[0];
    اگر( !hdr )
        واپس 0;
    صحیح d = hdr->dims;
    برائے(صحیح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (غیرعلامتی)idx[i];
    واپس h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحیح i, idx[CV_MAX_DIM] = {0}, d = m.dims, آخری_سائز = m.size[d - 1];
    size_t esz = m.elem_سائز();
    ثابت uchar* dptr = m.ptr();

    برائے(;;)
    {
        برائے( i = 0; i < آخری_سائز; i++, dptr += esz )
        {
            اگر( isZeroElem(dptr, esz) )
                جاری;
            idx[d-1] = i;
            uchar* مقصد = new_گرہ(idx, hash(idx));
            copyElem( dptr, مقصد, esz );
        }

        برائے( i = d - 2; i >= 0; i-- )
        {
            dptr += m.قدم[i] - m.size[i+1]*m.قدم[i+1];
            اگر( ++idx[i] < m.size[i] )
                توڑو;
            idx[i] = 0;
        }
        اگر( i < 0 )
            توڑو;
    }
}

خالی SparseMat::create(صحیح d, ثابت صحیح* _sizes, صحیح _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    برائے( صحیح i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    اگر( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        صحیح i;
        برائے( i = 0; i < d; i++ )
            اگر( _sizes[i] != hdr->size[i] )
                توڑو;
        اگر( i == d )
        {
            clear();
            واپس;
        }
    }
    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (hdr && _sizes == hdr->size)
    {
        برائے(صحیح i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = نیا Hdr(d, _sizes, _type);
}

خالی SparseMat::copy_مقصد( SparseMat& m ) ثابت
{
    اگر( hdr == m.hdr )
        واپس;
    اگر( !hdr )
    {
        m.release();
        واپس;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_سائز();

    برائے( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گرہ();
        uchar* مقصد = m.new_گرہ(n->idx, n->hashval);
        copyElem( from.ptr, مقصد, esz );
    }
}

خالی SparseMat::copy_مقصد( Mat& m ) ثابت
{
    CV_Assert( hdr );
    صحیح ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_سائز();

    برائے( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گرہ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


خالی SparseMat::convert_مقصد( SparseMat& m, صحیح rtype, دوہرا alpha ) ثابت
{
    صحیح cn = channels();
    اگر( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    اگر( hdr == m.hdr && rtype != type()  )
    {
        SparseMat عارضی_2;
        convert_مقصد(عارضی_2, rtype, alpha);
        m = عارضی_2;
        واپس;
    }

    CV_Assert(hdr != 0);
    اگر( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    اگر( alpha == 1 )
    {
        Convert_ڈیٹا cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        برائے( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* مقصد = hdr == m.hdr ? from.ptr : m.new_گرہ(n->idx, n->hashval);
            cvtfunc( from.ptr, مقصد, cn );
        }
    }
    ورنہ
    {
        Convert_Scale_ڈیٹا cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        برائے( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* مقصد = hdr == m.hdr ? from.ptr : m.new_گرہ(n->idx, n->hashval);
            cvtfunc( from.ptr, مقصد, cn, alpha, 0 );
        }
    }
}


خالی SparseMat::convert_مقصد( Mat& m, صحیح rtype, دوہرا alpha, دوہرا beta ) ثابت
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
        Convert_ڈیٹا cvtfunc = getConvertElem(type(), rtype);
        برائے( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* مقصد = m.ptr(n->idx);
            cvtfunc( from.ptr, مقصد, cn );
        }
    }
    ورنہ
    {
        Convert_Scale_ڈیٹا cvtfunc = getConvertScaleElem(type(), rtype);
        برائے( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* مقصد = m.ptr(n->idx);
            cvtfunc( from.ptr, مقصد, cn, alpha, beta );
        }
    }
}

خالی SparseMat::clear()
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
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 )
            واپس &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0 };
        واپس new_گرہ( idx, h );
    }
    واپس تہی;
}

uchar* SparseMat::ptr(صحیح i0, صحیح i1, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            واپس &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0, i1 };
        واپس new_گرہ( idx, h );
    }
    واپس تہی;
}

uchar* SparseMat::ptr(صحیح i0, صحیح i1, صحیح i2, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            واپس &value<uchar>(elem);
        nidx = elem->next;
    }

    اگر( createMissing )
    {
        صحیح idx[] = { i0, i1, i2 };
        واپس new_گرہ( idx, h );
    }
    واپس تہی;
}

uchar* SparseMat::ptr(ثابت صحیح* idx, منطقی createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    صحیح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h )
        {
            برائے( i = 0; i < d; i++ )
                اگر( elem->idx[i] != idx[i] )
                    توڑو;
            اگر( i == d )
                واپس &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    واپس createMissing ? new_گرہ(idx, h) : تہی;
}

خالی SparseMat::erase(صحیح i0, صحیح i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            توڑو;
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گرہ(hidx, nidx, previdx);
}

خالی SparseMat::erase(صحیح i0, صحیح i1, صحیح i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            توڑو;
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گرہ(hidx, nidx, previdx);
}

خالی SparseMat::erase(ثابت صحیح* idx, size_t* hashval)
{
    CV_Assert( hdr );
    صحیح i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    جب( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        اگر( elem->hashval == h )
        {
            برائے( i = 0; i < d; i++ )
                اگر( elem->idx[i] != idx[i] )
                    توڑو;
            اگر( i == d )
                توڑو;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    اگر( nidx )
        remove_گرہ(hidx, nidx, previdx);
}

خالی SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    اگر((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::لوگارتھم((دوہرا)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    برائے( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    برائے( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        جب( nidx )
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

uchar* SparseMat::new_گرہ(ثابت صحیح* idx, size_t hashval)
{
    ثابت صحیح HASH_MAX_FILL_ضربی_عدد=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    اگر( ++hdr->گرہ_گنتی > hsize*HASH_MAX_FILL_ضربی_عدد )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    اگر( !hdr->freeList )
    {
        size_t i, nsz = hdr->گرہ_سائز, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        برائے( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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
    برائے( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_سائز();
    uchar* p = &value<uchar>(elem);
    اگر( esz == حجم(اعشاری) )
        *((اعشاری*)p) = 0.f;
    ورنہ اگر( esz == حجم(دوہرا) )
        *((دوہرا*)p) = 0.;
    ورنہ
        memset(p, 0, esz);

    واپس p;
}


خالی SparseMat::remove_گرہ(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = گرہ(nidx);
    اگر( previdx )
    {
        Node* پچھلا = گرہ(previdx);
        پچھلا->next = n->next;
    }
    ورنہ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->گرہ_گنتی;
}

//
// Operations
//
دوہرا norm( ثابت SparseMat& src, صحیح normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_نقاب;
    صحیح type = src.type();
    دوہرا نتیجہ = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    اگر( type == CV_32F )
    {
        اگر( normType == NORM_INF )
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ = std::max(نتیجہ, std::مطلق_قدر((دوہرا)it.value<اعشاری>()));
            }
        ورنہ اگر( normType == NORM_L1 )
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ += std::مطلق_قدر(it.value<اعشاری>());
            }
        ورنہ
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                دوہرا v = it.value<اعشاری>();
                نتیجہ += v*v;
            }
    }
    ورنہ اگر( type == CV_64F )
    {
        اگر( normType == NORM_INF )
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ = std::max(نتیجہ, std::مطلق_قدر(it.value<دوہرا>()));
            }
        ورنہ اگر( normType == NORM_L1 )
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ += std::مطلق_قدر(it.value<دوہرا>());
            }
        ورنہ
            برائے( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                دوہرا v = it.value<دوہرا>();
                نتیجہ += v*v;
            }
    }
    ورنہ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    اگر( normType == NORM_L2 )
        نتیجہ = std::جذر(نتیجہ);
    واپس نتیجہ;
}

خالی minMaxLoc( ثابت SparseMat& src, دوہرا* _minval, دوہرا* _maxval, صحیح* _minidx, صحیح* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    صحیح type = src.type();
    ثابت صحیح *minidx = 0, *maxidx = 0;

    اگر( type == CV_32F )
    {
        اعشاری minval = FLT_MAX, maxval = -FLT_MAX;
        برائے( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            اعشاری v = it.value<اعشاری>();
            اگر( v < minval )
            {
                minval = v;
                minidx = it.گرہ()->idx;
            }
            اگر( v > maxval )
            {
                maxval = v;
                maxidx = it.گرہ()->idx;
            }
        }
        اگر( _minval )
            *_minval = minval;
        اگر( _maxval )
            *_maxval = maxval;
    }
    ورنہ اگر( type == CV_64F )
    {
        دوہرا minval = DBL_MAX, maxval = -DBL_MAX;
        برائے( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            دوہرا v = it.value<دوہرا>();
            اگر( v < minval )
            {
                minval = v;
                minidx = it.گرہ()->idx;
            }
            اگر( v > maxval )
            {
                maxval = v;
                maxidx = it.گرہ()->idx;
            }
        }
        اگر( _minval )
            *_minval = minval;
        اگر( _maxval )
            *_maxval = maxval;
    }
    ورنہ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    اگر( _minidx && minidx )
        برائے( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    اگر( _maxidx && maxidx )
        برائے( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


خالی normalize( ثابت SparseMat& src, SparseMat& dst, دوہرا a, صحیح norm_type )
{
    CV_INSTRUMENT_REGION();

    دوہرا scale = 1;
    اگر( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ورنہ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_مقصد( dst, -1, scale );
}

} // cv::
