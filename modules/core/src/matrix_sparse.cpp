// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

سانچا<قسم_ناں T1, قسم_ناں T2> فراغ
convert_انکڑے(ثابت فراغ* _from, فراغ* منزل_2, صحيح cn)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* منزل = (T2*)منزل_2;
    إذا( cn == 1 )
        *منزل = saturate_cast<T2>(*from);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            منزل[i] = saturate_cast<T2>(from[i]);
}

سانچا<قسم_ناں T1, قسم_ناں T2> فراغ
convert_Scale_انکڑے(ثابت فراغ* _from, فراغ* منزل_2, صحيح cn, مزدوج alpha, مزدوج beta)
{
    ثابت T1* from = (ثابت T1*)_from;
    T2* منزل = (T2*)منزل_2;
    إذا( cn == 1 )
        *منزل = saturate_cast<T2>(*from*alpha + beta);
    وإلا
        لكل( صحيح i = 0; i < cn; i++ )
            منزل[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

تعريفنوع فراغ (*Convert_انکڑے)(ثابت فراغ* from, فراغ* منزل, صحيح cn);
تعريفنوع فراغ (*Convert_Scale_انکڑے)(ثابت فراغ* from, فراغ* منزل, صحيح cn, مزدوج alpha, مزدوج beta);

ساكن Convert_انکڑے getConvertElem(صحيح fromType, صحيح منزل_Type)
{
    ساكن Convert_انکڑے tab[CV_ڈونگھائی_MAX][CV_ڈونگھائی_MAX] =
    {{ convert_انکڑے<uchar, uchar>, convert_انکڑے<uchar, schar>,
      convert_انکڑے<uchar, ushort>, convert_انکڑے<uchar, چھوٹا>,
      convert_انکڑے<uchar, صحيح>, convert_انکڑے<uchar, عائم>,
      convert_انکڑے<uchar, مزدوج>, 0 },

    { convert_انکڑے<schar, uchar>, convert_انکڑے<schar, schar>,
      convert_انکڑے<schar, ushort>, convert_انکڑے<schar, چھوٹا>,
      convert_انکڑے<schar, صحيح>, convert_انکڑے<schar, عائم>,
      convert_انکڑے<schar, مزدوج>, 0 },

    { convert_انکڑے<ushort, uchar>, convert_انکڑے<ushort, schar>,
      convert_انکڑے<ushort, ushort>, convert_انکڑے<ushort, چھوٹا>,
      convert_انکڑے<ushort, صحيح>, convert_انکڑے<ushort, عائم>,
      convert_انکڑے<ushort, مزدوج>, 0 },

    { convert_انکڑے<چھوٹا, uchar>, convert_انکڑے<چھوٹا, schar>,
      convert_انکڑے<چھوٹا, ushort>, convert_انکڑے<چھوٹا, چھوٹا>,
      convert_انکڑے<چھوٹا, صحيح>, convert_انکڑے<چھوٹا, عائم>,
      convert_انکڑے<چھوٹا, مزدوج>, 0 },

    { convert_انکڑے<صحيح, uchar>, convert_انکڑے<صحيح, schar>,
      convert_انکڑے<صحيح, ushort>, convert_انکڑے<صحيح, چھوٹا>,
      convert_انکڑے<صحيح, صحيح>, convert_انکڑے<صحيح, عائم>,
      convert_انکڑے<صحيح, مزدوج>, 0 },

    { convert_انکڑے<عائم, uchar>, convert_انکڑے<عائم, schar>,
      convert_انکڑے<عائم, ushort>, convert_انکڑے<عائم, چھوٹا>,
      convert_انکڑے<عائم, صحيح>, convert_انکڑے<عائم, عائم>,
      convert_انکڑے<عائم, مزدوج>, 0 },

    { convert_انکڑے<مزدوج, uchar>, convert_انکڑے<مزدوج, schar>,
      convert_انکڑے<مزدوج, ushort>, convert_انکڑے<مزدوج, چھوٹا>,
      convert_انکڑے<مزدوج, صحيح>, convert_انکڑے<مزدوج, عائم>,
      convert_انکڑے<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_انکڑے func = tab[CV_MAT_ڈونگھائی(fromType)][CV_MAT_ڈونگھائی(منزل_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

ساكن Convert_Scale_انکڑے getConvertScaleElem(صحيح fromType, صحيح منزل_Type)
{
    ساكن Convert_Scale_انکڑے tab[CV_ڈونگھائی_MAX][CV_ڈونگھائی_MAX] =
    {{ convert_Scale_انکڑے<uchar, uchar>, convert_Scale_انکڑے<uchar, schar>,
      convert_Scale_انکڑے<uchar, ushort>, convert_Scale_انکڑے<uchar, چھوٹا>,
      convert_Scale_انکڑے<uchar, صحيح>, convert_Scale_انکڑے<uchar, عائم>,
      convert_Scale_انکڑے<uchar, مزدوج>, 0 },

    { convert_Scale_انکڑے<schar, uchar>, convert_Scale_انکڑے<schar, schar>,
      convert_Scale_انکڑے<schar, ushort>, convert_Scale_انکڑے<schar, چھوٹا>,
      convert_Scale_انکڑے<schar, صحيح>, convert_Scale_انکڑے<schar, عائم>,
      convert_Scale_انکڑے<schar, مزدوج>, 0 },

    { convert_Scale_انکڑے<ushort, uchar>, convert_Scale_انکڑے<ushort, schar>,
      convert_Scale_انکڑے<ushort, ushort>, convert_Scale_انکڑے<ushort, چھوٹا>,
      convert_Scale_انکڑے<ushort, صحيح>, convert_Scale_انکڑے<ushort, عائم>,
      convert_Scale_انکڑے<ushort, مزدوج>, 0 },

    { convert_Scale_انکڑے<چھوٹا, uchar>, convert_Scale_انکڑے<چھوٹا, schar>,
      convert_Scale_انکڑے<چھوٹا, ushort>, convert_Scale_انکڑے<چھوٹا, چھوٹا>,
      convert_Scale_انکڑے<چھوٹا, صحيح>, convert_Scale_انکڑے<چھوٹا, عائم>,
      convert_Scale_انکڑے<چھوٹا, مزدوج>, 0 },

    { convert_Scale_انکڑے<صحيح, uchar>, convert_Scale_انکڑے<صحيح, schar>,
      convert_Scale_انکڑے<صحيح, ushort>, convert_Scale_انکڑے<صحيح, چھوٹا>,
      convert_Scale_انکڑے<صحيح, صحيح>, convert_Scale_انکڑے<صحيح, عائم>,
      convert_Scale_انکڑے<صحيح, مزدوج>, 0 },

    { convert_Scale_انکڑے<عائم, uchar>, convert_Scale_انکڑے<عائم, schar>,
      convert_Scale_انکڑے<عائم, ushort>, convert_Scale_انکڑے<عائم, چھوٹا>,
      convert_Scale_انکڑے<عائم, صحيح>, convert_Scale_انکڑے<عائم, عائم>,
      convert_Scale_انکڑے<عائم, مزدوج>, 0 },

    { convert_Scale_انکڑے<مزدوج, uchar>, convert_Scale_انکڑے<مزدوج, schar>,
      convert_Scale_انکڑے<مزدوج, ushort>, convert_Scale_انکڑے<مزدوج, چھوٹا>,
      convert_Scale_انکڑے<مزدوج, صحيح>, convert_Scale_انکڑے<مزدوج, عائم>,
      convert_Scale_انکڑے<مزدوج, مزدوج>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_انکڑے func = tab[CV_MAT_ڈونگھائی(fromType)][CV_MAT_ڈونگھائی(منزل_Type)];
    CV_Assert( func != 0 );
    أرجع func;
}

تعداد { HASH_SIZE0 = 8 };

ساكن مضمن فراغ copyElem(ثابت uchar* from, uchar* منزل, size_t elem_ناپ)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_ناپ; i += حجم(صحيح) )
        *(صحيح*)(منزل + i) = *(ثابت صحيح*)(from + i);
    لكل( ; i < elem_ناپ; i++ )
        منزل[i] = from[i];
}

ساكن مضمن منطقي isZeroElem(ثابت uchar* انکڑے, size_t elem_ناپ)
{
    size_t i;
    لكل( i = 0; i + حجم(صحيح) <= elem_ناپ; i += حجم(صحيح) )
        إذا( *(صحيح*)(انکڑے + i) != 0 )
            أرجع خطأ;
    لكل( ; i < elem_ناپ; i++ )
        إذا( انکڑے[i] != 0 )
            أرجع خطأ;
    أرجع سچ;
}

SparseMat::Hdr::Hdr( صحيح _dims, ثابت صحيح* _sizes, صحيح _type )
{
    refcount = 1;

    dims = _dims;
    قدر_وتھ = (صحيح)align_ناپ(حجم(SparseMat::Node) - MAX_DIM*حجم(صحيح) +
                                 dims*حجم(صحيح), CV_ELEM_SIZE1(_type));
    گرہ_ناپ = align_ناپ(قدر_وتھ +
        CV_ELEM_ناپ(_type), (صحيح)حجم(size_t));

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
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wstringop-overflow"
#جے_انت
    pool.resize(گرہ_ناپ);
#إذا defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ہدایت GCC diagnostic pop
#جے_انت
    گرہ_گنتی = freeList = 0;
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

SparseMat& SparseMat::عامل = (ثابت SparseMat& m)
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

SparseMat& SparseMat::عامل=(ثابت Mat& m)
{
    أرجع (*هذا = SparseMat(m));
}

فراغ SparseMat::سونپو_منزل(SparseMat& m, صحيح _type) ثابت
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
        مٹاؤ hdr;
    hdr = 0;
}

size_t SparseMat::hash(صحيح i0) ثابت
{
    أرجع (size_t)i0;
}

size_t SparseMat::hash(صحيح i0, صحيح i1) ثابت
{
    أرجع (size_t)(بنانشانی)i0 * HASH_SCALE + (بنانشانی)i1;
}

size_t SparseMat::hash(صحيح i0, صحيح i1, صحيح i2) ثابت
{
    أرجع ((size_t)(بنانشانی)i0 * HASH_SCALE + (بنانشانی)i1) * HASH_SCALE + (بنانشانی)i2;
}

size_t SparseMat::hash(ثابت صحيح* idx) ثابت
{
    size_t h = (بنانشانی)idx[0];
    إذا( !hdr )
        أرجع 0;
    صحيح d = hdr->dims;
    لكل(صحيح i = 1; i < d; i++ )
        h = h * HASH_SCALE + (بنانشانی)idx[i];
    أرجع h;
}


SparseMat::SparseMat(ثابت Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    صحيح i, idx[CV_MAX_DIM] = {0}, d = m.dims, آخری_ناپ = m.size[d - 1];
    size_t esz = m.elem_ناپ();
    ثابت uchar* dptr = m.ptr();

    لكل(;;)
    {
        لكل( i = 0; i < آخری_ناپ; i++, dptr += esz )
        {
            إذا( isZeroElem(dptr, esz) )
                استمر;
            idx[d-1] = i;
            uchar* منزل = new_گرہ(idx, hash(idx));
            copyElem( dptr, منزل, esz );
        }

        لكل( i = d - 2; i >= 0; i-- )
        {
            dptr += m.قدم[i] - m.size[i+1]*m.قدم[i+1];
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
    size_t N = nzcount(), esz = elem_ناپ();

    لكل( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گرہ();
        uchar* منزل = m.new_گرہ(n->idx, n->hashval);
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
    size_t N = nzcount(), esz = elem_ناپ();

    لكل( size_t i = 0; i < N; i++, ++from )
    {
        ثابت Node* n = from.گرہ();
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
        SparseMat عارضی_2;
        convert_منزل(عارضی_2, rtype, alpha);
        m = عارضی_2;
        أرجع;
    }

    CV_Assert(hdr != 0);
    إذا( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    إذا( alpha == 1 )
    {
        Convert_انکڑے cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* منزل = hdr == m.hdr ? from.ptr : m.new_گرہ(n->idx, n->hashval);
            cvtfunc( from.ptr, منزل, cn );
        }
    }
    وإلا
    {
        Convert_Scale_انکڑے cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* منزل = hdr == m.hdr ? from.ptr : m.new_گرہ(n->idx, n->hashval);
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
        Convert_انکڑے cvtfunc = getConvertElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
            uchar* منزل = m.ptr(n->idx);
            cvtfunc( from.ptr, منزل, cn );
        }
    }
    وإلا
    {
        Convert_Scale_انکڑے cvtfunc = getConvertScaleElem(type(), rtype);
        لكل( size_t i = 0; i < N; i++, ++from )
        {
            ثابت Node* n = from.گرہ();
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
        أرجع new_گرہ( idx, h );
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
        أرجع new_گرہ( idx, h );
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
        أرجع new_گرہ( idx, h );
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

    أرجع createMissing ? new_گرہ(idx, h) : عدم;
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
        remove_گرہ(hidx, nidx, previdx);
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
        remove_گرہ(hidx, nidx, previdx);
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
        remove_گرہ(hidx, nidx, previdx);
}

فراغ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    إذا((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::لوگارتھم((مزدوج)newsize)/CV_LOG2);

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

uchar* SparseMat::new_گرہ(ثابت صحيح* idx, size_t hashval)
{
    ثابت صحيح HASH_MAX_FILL_ضارب=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    إذا( ++hdr->گرہ_گنتی > hsize*HASH_MAX_FILL_ضارب )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    إذا( !hdr->freeList )
    {
        size_t i, nsz = hdr->گرہ_ناپ, psize = hdr->pool.size(),
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
    size_t esz = elem_ناپ();
    uchar* p = &value<uchar>(elem);
    إذا( esz == حجم(عائم) )
        *((عائم*)p) = 0.f;
    وإلا إذا( esz == حجم(مزدوج) )
        *((مزدوج*)p) = 0.;
    وإلا
        memset(p, 0, esz);

    أرجع p;
}


فراغ SparseMat::remove_گرہ(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = گرہ(nidx);
    إذا( previdx )
    {
        Node* prev = گرہ(previdx);
        prev->next = n->next;
    }
    وإلا
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->گرہ_گنتی;
}

//
// Operations
//
مزدوج norm( ثابت SparseMat& src, صحيح normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_نقاب;
    صحيح type = src.type();
    مزدوج نتیجہ = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    إذا( type == CV_32F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ = std::max(نتیجہ, std::مطلق_قدر((مزدوج)it.value<عائم>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ += std::مطلق_قدر(it.value<عائم>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<عائم>();
                نتیجہ += v*v;
            }
    }
    وإلا إذا( type == CV_64F )
    {
        إذا( normType == NORM_INF )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ = std::max(نتیجہ, std::مطلق_قدر(it.value<مزدوج>()));
            }
        وإلا إذا( normType == NORM_L1 )
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                نتیجہ += std::مطلق_قدر(it.value<مزدوج>());
            }
        وإلا
            لكل( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                مزدوج v = it.value<مزدوج>();
                نتیجہ += v*v;
            }
    }
    وإلا
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    إذا( normType == NORM_L2 )
        نتیجہ = std::جذر(نتیجہ);
    أرجع نتیجہ;
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
                minidx = it.گرہ()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.گرہ()->idx;
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
                minidx = it.گرہ()->idx;
            }
            إذا( v > maxval )
            {
                maxval = v;
                maxidx = it.گرہ()->idx;
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
