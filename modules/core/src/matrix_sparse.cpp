// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

قالب<نام_نوع T1, نام_نوع T2> فراغ
convert_داده(ثابت فراغ* _from, فراغ* مقصد_2, صحيح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* مقصد = (T2*)مقصد_2;
    إذا( cn == 1 )
        *مقصد = saturate_cast<T2>(*from);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            مقصد[i] = saturate_cast<T2>(from[i]);
}

قالب<نام_نوع T1, نام_نوع T2> فراغ
convert_Scale_داده(ثابت فراغ* _from, فراغ* مقصد_2, صحيح cn, مزدوج alpha, مزدوج beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* مقصد = (T2*)مقصد_2;
    إذا( cn == 1 )
        *مقصد = saturate_cast<T2>(*from*alpha + beta);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            مقصد[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

تعريفنوع فراغ (*Convert_داده)(ثابت فراغ* from, فراغ* مقصد, صحيح cn);
تعريفنوع فراغ (*Convert_Scale_داده)(ثابت فراغ* from, فراغ* مقصد, صحيح cn, مزدوج alpha, مزدوج beta);

ساكن Convert_داده getConvertElem(صحيح fromType, صحيح مقصد_Type)
{
    ساكن Convert_داده tab[CV_عمق_MAX][CV_عمق_MAX] =
    {{ convert_داده<uchar, uchar>, convert_داده<uchar, schar>,
      convert_داده<uchar, ushort>, convert_داده<uchar, کوتاه>,
      convert_داده<uchar, صحيح>, convert_داده<uchar, عائم>,
      convert_داده<uchar, مزدوج>, 0 },

    { convert_داده<schar, uchar>, convert_داده<schar, schar>,
      convert_داده<schar, ushort>, convert_داده<schar, کوتاه>,
      convert_داده<schar, صحيح>, convert_داده<schar, عائم>,
      convert_داده<schar, مزدوج>, 0 },

    { convert_داده<ushort, uchar>, convert_داده<ushort, schar>,
      convert_داده<ushort, ushort>, convert_داده<ushort, کوتاه>,
      convert_داده<ushort, صحيح>, convert_داده<ushort, عائم>,
      convert_داده<ushort, مزدوج>, 0 },

    { convert_داده<کوتاه, uchar>, convert_داده<کوتاه, schar>,
      convert_داده<کوتاه, ushort>, convert_داده<کوتاه, کوتاه>,
      convert_داده<کوتاه, صحيح>, convert_داده<کوتاه, عائم>,
      convert_داده<کوتاه, مزدوج>, 0 },

    { convert_داده<صحيح, uchar>, convert_داده<صحيح, schar>,
      convert_داده<صحيح, ushort>, convert_داده<صحيح, کوتاه>,
      convert_داده<صحيح, صحيح>, convert_داده<صحيح, عائم>,
      convert_داده<صحيح, مزدوج>, 0 },

    { convert_داده<عائم, uchar>, convert_داده<عائم, schar>,
      convert_داده<عائم, ushort>, convert_داده<عائم, کوتاه>,
      convert_داده<عائم, صحيح>, convert_داده<عائم, عائم>,
      convert_داده<عائم, مزدوج>, 0 },

    { convert_داده<مزدوج, uchar>, convert_داده<مزدوج, schar>,
      convert_داده<مزدوج, ushort>, convert_داده<مزدوج, کوتاه>,
      convert_داده<مزدوج, صحيح>, convert_داده<مزدوج, عائم>,
      convert_داده<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_داده func = tab[CV_MAT_عمق(fromType)][CV_MAT_عمق(مقصد_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

ساكن Convert_Scale_داده getConvertScaleElem(صحيح fromType, صحيح مقصد_Type)
{
    ساكن Convert_Scale_داده tab[CV_عمق_MAX][CV_عمق_MAX] =
    {{ convert_Scale_داده<uchar, uchar>, convert_Scale_داده<uchar, schar>,
      convert_Scale_داده<uchar, ushort>, convert_Scale_داده<uchar, کوتاه>,
      convert_Scale_داده<uchar, صحيح>, convert_Scale_داده<uchar, عائم>,
      convert_Scale_داده<uchar, مزدوج>, 0 },

    { convert_Scale_داده<schar, uchar>, convert_Scale_داده<schar, schar>,
      convert_Scale_داده<schar, ushort>, convert_Scale_داده<schar, کوتاه>,
      convert_Scale_داده<schar, صحيح>, convert_Scale_داده<schar, عائم>,
      convert_Scale_داده<schar, مزدوج>, 0 },

    { convert_Scale_داده<ushort, uchar>, convert_Scale_داده<ushort, schar>,
      convert_Scale_داده<ushort, ushort>, convert_Scale_داده<ushort, کوتاه>,
      convert_Scale_داده<ushort, صحيح>, convert_Scale_داده<ushort, عائم>,
      convert_Scale_داده<ushort, مزدوج>, 0 },

    { convert_Scale_داده<کوتاه, uchar>, convert_Scale_داده<کوتاه, schar>,
      convert_Scale_داده<کوتاه, ushort>, convert_Scale_داده<کوتاه, کوتاه>,
      convert_Scale_داده<کوتاه, صحيح>, convert_Scale_داده<کوتاه, عائم>,
      convert_Scale_داده<کوتاه, مزدوج>, 0 },

    { convert_Scale_داده<صحيح, uchar>, convert_Scale_داده<صحيح, schar>,
      convert_Scale_داده<صحيح, ushort>, convert_Scale_داده<صحيح, کوتاه>,
      convert_Scale_داده<صحيح, صحيح>, convert_Scale_داده<صحيح, عائم>,
      convert_Scale_داده<صحيح, مزدوج>, 0 },

    { convert_Scale_داده<عائم, uchar>, convert_Scale_داده<عائم, schar>,
      convert_Scale_داده<عائم, ushort>, convert_Scale_داده<عائم, کوتاه>,
      convert_Scale_داده<عائم, صحيح>, convert_Scale_داده<عائم, عائم>,
      convert_Scale_داده<عائم, مزدوج>, 0 },

    { convert_Scale_داده<مزدوج, uchar>, convert_Scale_داده<مزدوج, schar>,
      convert_Scale_داده<مزدوج, ushort>, convert_Scale_داده<مزدوج, کوتاه>,
      convert_Scale_داده<مزدوج, صحيح>, convert_Scale_داده<مزدوج, عائم>,
      convert_Scale_داده<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_داده func = tab[CV_MAT_عمق(fromType)][CV_MAT_عمق(مقصد_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

تعداد { HASH_SIZE0 = 8 };

ساكن مضمن فراغ copyElem(ثابت uchar* from, uchar* مقصد, size_t elem_اندازه)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_اندازه; i += حجم(صحيح) )
        *(صحيح*)(مقصد + i) = *(ثابت صحيح*)(from + i);
    لكل( ; i < elem_اندازه; i++ )
        مقصد[i] = from[i];
}

ساكن مضمن منطقي isZeroElem(ثابت uchar* داده, size_t elem_اندازه)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_اندازه; i += حجم(صحيح) )
        إذا( *(صحيح*)(داده + i) != 0 )
            أرجع خطأ;
    لكل( ; i < elem_اندازه; i++ )
        إذا( داده[i] != 0 )
            أرجع خطأ;
    أرجع درست;
}

SparseMat::Hdr::Hdr( صحيح _dims, ثابت صحيح* _sizes, صحيح _type )
{
    refcount = 1;

    dims = _dims;
    مقدار_جابه_جایی = (صحيح)align_اندازه(حجم(SparseMat::Node) - MAX_DIM*حجم(صحيح) +
                                 dims*حجم(صحيح), CV_ELEM_SIZE1(_type));
    گره_اندازه = align_اندازه(مقدار_جابه_جایی +
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
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wstringop-overflow"
#پایان_اگر
    pool.resize(گره_اندازه);
#إذا defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#رهنمود GCC diagnostic pop
#پایان_اگر
    گره_شمار = freeList = 0;
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

SparseMat& SparseMat::عملگر = (ثابت SparseMat& m)
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

SparseMat& SparseMat::عملگر=(ثابت Mat& m)
{
    أرجع (*هذا = SparseMat(m));
}

فراغ SparseMat::تخصیص_بده_مقصد(SparseMat& m, صحيح _type) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_مقصد(m, _type);
}

فراغ SparseMat::addref()
{
    إذا( hdr )
        CV_XADD(&hdr->refcount, 1);
}

فراغ SparseMat::release()
{
    إذا( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        حذف hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحيح i0) ثابت
{
    أرجع (size_t)i0;
}

size_t SparseMat::hash(صحيح i0, صحيح i1) ثابت
{
    أرجع (size_t)(بیعلامت)i0 * HASH_SCALE + (بیعلامت)i1;
}

size_t SparseMat::hash(صحيح i0, صحيح i1, صحيح i2) ثابت
{
    أرجع ((size_t)(بیعلامت)i0 * HASH_SCALE + (بیعلامت)i1) * HASH_SCALE + (بیعلامت)i2;
}

size_t SparseMat::hash(ثابت صحيح* idx) ثابت
{
    size_t h = (بیعلامت)idx[0];
    إذا( !hdr )
        أرجع 0;
    صحيح d = hdr->dims;
    لكل(صحيح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (بیعلامت)idx[i];
    أرجع h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحيح i, idx[CV_MAX_DIM] = {0}, d = m.dims, آخرین_اندازه = m.size[d - 1];
    size_t esz = m.elem_اندازه();
    ثابت uchar* dptr = m.ptr();

    لكل(;;)
    {
        لكل( i = 0; i < آخرین_اندازه; i++, dptr += esz )
        {
            إذا( isZeroElem(dptr, esz) )
                استمر;
            idx[d-1] = i;
            uchar* مقصد = new_گره(idx, hash(idx));
            copyElem( dptr, مقصد, esz );
        }

        لكل( i = d - 2; i >= 0; i-- )
        {
            dptr += m.گام[i] - m.size[i+1]*m.گام[i+1];
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

فراغ SparseMat::copy_مقصد( SparseMat& m ) ثابت
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
        ثابت Node* n = from.گره();
        uchar* مقصد = m.new_گره(n->idx, n->hashval);
        copyElem( from.ptr, مقصد, esz );
    }
}

فراغ SparseMat::copy_مقصد( Mat& m ) ثابت
{
    CV_Assert( hdr );
    صحيح ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_اندازه();

    لكل( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گره();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


فراغ SparseMat::convert_مقصد( SparseMat& m, صحيح rtype, مزدوج alpha ) ثابت
{
    صحيح cn = channels();
    إذا( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    إذا( hdr == m.hdr && rtype != type()  )
    {
        SparseMat موقت_2;
        convert_مقصد(موقت_2, rtype, alpha);
        m = موقت_2;
        أرجع;
    }

    CV_Assert(hdr != 0);
    إذا( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    إذا( alpha == 1 )
    {
        Convert_داده cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* مقصد = hdr == m.hdr ? from.ptr : m.new_گره(n->idx, n->hashval);
            cvtfunc( from.ptr, مقصد, cn );
        }
    }
    وإلا
    {
        Convert_Scale_داده cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* مقصد = hdr == m.hdr ? from.ptr : m.new_گره(n->idx, n->hashval);
            cvtfunc( from.ptr, مقصد, cn, alpha, 0 );
        }
    }
}


فراغ SparseMat::convert_مقصد( Mat& m, صحيح rtype, مزدوج alpha, مزدوج beta ) ثابت
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
        Convert_داده cvtfunc = getConvertElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* مقصد = m.ptr(n->idx);
            cvtfunc( from.ptr, مقصد, cn );
        }
    }
    وإلا
    {
        Convert_Scale_داده cvtfunc = getConvertScaleElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گره();
            uchar* مقصد = m.ptr(n->idx);
            cvtfunc( from.ptr, مقصد, cn, alpha, beta );
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
        أرجع new_گره( idx, h );
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
        أرجع new_گره( idx, h );
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
        أرجع new_گره( idx, h );
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

    أرجع createMissing ? new_گره(idx, h) : عدم;
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
        remove_گره(hidx, nidx, previdx);
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
        remove_گره(hidx, nidx, previdx);
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
        remove_گره(hidx, nidx, previdx);
}

فراغ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    إذا((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::لگاریتم((مزدوج)newsize)/CV_LOG2);

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

uchar* SparseMat::new_گره(ثابت صحيح* idx, size_t hashval)
{
    ثابت صحيح HASH_MAX_FILL_ضریب=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    إذا( ++hdr->گره_شمار > hsize*HASH_MAX_FILL_ضریب )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    إذا( !hdr->freeList )
    {
        size_t i, nsz = hdr->گره_اندازه, psize = hdr->pool.size(),
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


فراغ SparseMat::remove_گره(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = گره(nidx);
    إذا( previdx )
    {
        Node* prev = گره(previdx);
        prev->next = n->next;
    }
    وإلا
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->گره_شمار;
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
    مزدوج نتیجه = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    إذا( type == CV_32F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجه = std::max(نتیجه, std::قدر_مطلق((مزدوج)it.value<عائم>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجه += std::قدر_مطلق(it.value<عائم>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<عائم>();
                نتیجه += v*v;
            }
    }
    وإلا إذا( type == CV_64F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجه = std::max(نتیجه, std::قدر_مطلق(it.value<مزدوج>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجه += std::قدر_مطلق(it.value<مزدوج>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<مزدوج>();
                نتیجه += v*v;
            }
    }
    وإلا
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    إذا( normType == NORM_L2 )
        نتیجه = std::جذر(نتیجه);
    أرجع نتیجه;
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
                minidx = it.گره()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.گره()->idx;
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
                minidx = it.گره()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.گره()->idx;
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

    src.convert_مقصد( dst, -1, scale );
}

} // cv::
