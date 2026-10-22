// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/mat.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਖਾਲੀ
convert_ਅੰਕੜੇ(ਅਚਲ ਖਾਲੀ* _from, ਖਾਲੀ* ਮੰਜ਼ਿਲ_2, ਪੂਰਨ cn)
{
    ਅਚਲ T1* from = (ਅਚਲ T1*)_from;
    T2* ਮੰਜ਼ਿਲ = (T2*)ਮੰਜ਼ਿਲ_2;
    ਜੇ( cn == 1 )
        *ਮੰਜ਼ਿਲ = saturate_cast<T2>(*from);
    ਨਹੀਂਤਾਂ
        ਲਈ( ਪੂਰਨ i = 0; i < cn; i++ )
            ਮੰਜ਼ਿਲ[i] = saturate_cast<T2>(from[i]);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਖਾਲੀ
convert_Scale_ਅੰਕੜੇ(ਅਚਲ ਖਾਲੀ* _from, ਖਾਲੀ* ਮੰਜ਼ਿਲ_2, ਪੂਰਨ cn, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta)
{
    ਅਚਲ T1* from = (ਅਚਲ T1*)_from;
    T2* ਮੰਜ਼ਿਲ = (T2*)ਮੰਜ਼ਿਲ_2;
    ਜੇ( cn == 1 )
        *ਮੰਜ਼ਿਲ = saturate_cast<T2>(*from*alpha + beta);
    ਨਹੀਂਤਾਂ
        ਲਈ( ਪੂਰਨ i = 0; i < cn; i++ )
            ਮੰਜ਼ਿਲ[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Convert_ਅੰਕੜੇ)(ਅਚਲ ਖਾਲੀ* from, ਖਾਲੀ* ਮੰਜ਼ਿਲ, ਪੂਰਨ cn);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Convert_Scale_ਅੰਕੜੇ)(ਅਚਲ ਖਾਲੀ* from, ਖਾਲੀ* ਮੰਜ਼ਿਲ, ਪੂਰਨ cn, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta);

ਸਥਿਰ Convert_ਅੰਕੜੇ getConvertElem(ਪੂਰਨ fromType, ਪੂਰਨ ਮੰਜ਼ਿਲ_Type)
{
    ਸਥਿਰ Convert_ਅੰਕੜੇ tab[CV_ਡੂੰਘਾਈ_MAX][CV_ਡੂੰਘਾਈ_MAX] =
    {{ convert_ਅੰਕੜੇ<uchar, uchar>, convert_ਅੰਕੜੇ<uchar, schar>,
      convert_ਅੰਕੜੇ<uchar, ushort>, convert_ਅੰਕੜੇ<uchar, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<uchar, ਪੂਰਨ>, convert_ਅੰਕੜੇ<uchar, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<uchar, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<schar, uchar>, convert_ਅੰਕੜੇ<schar, schar>,
      convert_ਅੰਕੜੇ<schar, ushort>, convert_ਅੰਕੜੇ<schar, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<schar, ਪੂਰਨ>, convert_ਅੰਕੜੇ<schar, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<schar, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<ushort, uchar>, convert_ਅੰਕੜੇ<ushort, schar>,
      convert_ਅੰਕੜੇ<ushort, ushort>, convert_ਅੰਕੜੇ<ushort, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<ushort, ਪੂਰਨ>, convert_ਅੰਕੜੇ<ushort, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<ushort, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<ਛੋਟਾ, uchar>, convert_ਅੰਕੜੇ<ਛੋਟਾ, schar>,
      convert_ਅੰਕੜੇ<ਛੋਟਾ, ushort>, convert_ਅੰਕੜੇ<ਛੋਟਾ, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<ਛੋਟਾ, ਪੂਰਨ>, convert_ਅੰਕੜੇ<ਛੋਟਾ, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<ਛੋਟਾ, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<ਪੂਰਨ, uchar>, convert_ਅੰਕੜੇ<ਪੂਰਨ, schar>,
      convert_ਅੰਕੜੇ<ਪੂਰਨ, ushort>, convert_ਅੰਕੜੇ<ਪੂਰਨ, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<ਪੂਰਨ, ਪੂਰਨ>, convert_ਅੰਕੜੇ<ਪੂਰਨ, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<ਪੂਰਨ, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, uchar>, convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, schar>,
      convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ushort>, convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਪੂਰਨ>, convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ>, 0 },

    { convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, uchar>, convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, schar>,
      convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ushort>, convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਛੋਟਾ>,
      convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਪੂਰਨ>, convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਦਸ਼ਮਲਵ>,
      convert_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_ਅੰਕੜੇ func = tab[CV_MAT_ਡੂੰਘਾਈ(fromType)][CV_MAT_ਡੂੰਘਾਈ(ਮੰਜ਼ਿਲ_Type)];
    CV_Assert( func != 0 );
    ਵਾਪਸ func;
}

ਸਥਿਰ Convert_Scale_ਅੰਕੜੇ getConvertScaleElem(ਪੂਰਨ fromType, ਪੂਰਨ ਮੰਜ਼ਿਲ_Type)
{
    ਸਥਿਰ Convert_Scale_ਅੰਕੜੇ tab[CV_ਡੂੰਘਾਈ_MAX][CV_ਡੂੰਘਾਈ_MAX] =
    {{ convert_Scale_ਅੰਕੜੇ<uchar, uchar>, convert_Scale_ਅੰਕੜੇ<uchar, schar>,
      convert_Scale_ਅੰਕੜੇ<uchar, ushort>, convert_Scale_ਅੰਕੜੇ<uchar, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<uchar, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<uchar, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<uchar, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<schar, uchar>, convert_Scale_ਅੰਕੜੇ<schar, schar>,
      convert_Scale_ਅੰਕੜੇ<schar, ushort>, convert_Scale_ਅੰਕੜੇ<schar, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<schar, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<schar, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<schar, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<ushort, uchar>, convert_Scale_ਅੰਕੜੇ<ushort, schar>,
      convert_Scale_ਅੰਕੜੇ<ushort, ushort>, convert_Scale_ਅੰਕੜੇ<ushort, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<ushort, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<ushort, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<ushort, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, uchar>, convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, schar>,
      convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, ushort>, convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<ਛੋਟਾ, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, uchar>, convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, schar>,
      convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, ushort>, convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<ਪੂਰਨ, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, uchar>, convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, schar>,
      convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ushort>, convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ>, 0 },

    { convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, uchar>, convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, schar>,
      convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ushort>, convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਛੋਟਾ>,
      convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਪੂਰਨ>, convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਦਸ਼ਮਲਵ>,
      convert_Scale_ਅੰਕੜੇ<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_ਅੰਕੜੇ func = tab[CV_MAT_ਡੂੰਘਾਈ(fromType)][CV_MAT_ਡੂੰਘਾਈ(ਮੰਜ਼ਿਲ_Type)];
    CV_Assert( func != 0 );
    ਵਾਪਸ func;
}

ਗਿਣਤੀ { HASH_SIZE0 = 8 };

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ copyElem(ਅਚਲ uchar* from, uchar* ਮੰਜ਼ਿਲ, size_t elem_ਮਾਪ)
{
    size_t i;
    ਲਈ( i = 0; i + ਆਕਾਰ(ਪੂਰਨ) <= elem_ਮਾਪ; i += ਆਕਾਰ(ਪੂਰਨ) )
        *(ਪੂਰਨ*)(ਮੰਜ਼ਿਲ + i) = *(ਅਚਲ ਪੂਰਨ*)(from + i);
    ਲਈ( ; i < elem_ਮਾਪ; i++ )
        ਮੰਜ਼ਿਲ[i] = from[i];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isZeroElem(ਅਚਲ uchar* ਅੰਕੜੇ, size_t elem_ਮਾਪ)
{
    size_t i;
    ਲਈ( i = 0; i + ਆਕਾਰ(ਪੂਰਨ) <= elem_ਮਾਪ; i += ਆਕਾਰ(ਪੂਰਨ) )
        ਜੇ( *(ਪੂਰਨ*)(ਅੰਕੜੇ + i) != 0 )
            ਵਾਪਸ ਝੂਠ;
    ਲਈ( ; i < elem_ਮਾਪ; i++ )
        ਜੇ( ਅੰਕੜੇ[i] != 0 )
            ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
}

SparseMat::Hdr::Hdr( ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type )
{
    refcount = 1;

    dims = _dims;
    ਮੁੱਲ_ਵਿੱਥ = (ਪੂਰਨ)align_ਮਾਪ(ਆਕਾਰ(SparseMat::Node) - MAX_DIM*ਆਕਾਰ(ਪੂਰਨ) +
                                 dims*ਆਕਾਰ(ਪੂਰਨ), CV_ELEM_SIZE1(_type));
    ਗੰਢ_ਮਾਪ = align_ਮਾਪ(ਮੁੱਲ_ਵਿੱਥ +
        CV_ELEM_ਮਾਪ(_type), (ਪੂਰਨ)ਆਕਾਰ(size_t));

    ਪੂਰਨ i;
    ਲਈ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    ਲਈ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

ਖਾਲੀ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ਜੇ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wstringop-overflow"
#ਜੇ_ਅੰਤ
    pool.resize(ਗੰਢ_ਮਾਪ);
#ਜੇ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ਹਦਾਇਤ GCC diagnostic pop
#ਜੇ_ਅੰਤ
    ਗੰਢ_ਗਿਣਤੀ = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ਅਚਲ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::ਸੰਚਾਲਕ = (ਅਚਲ SparseMat& m)
{
    ਜੇ( ਇਹ != &m )
    {
        ਜੇ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ਵਾਪਸ *ਇਹ;
}

SparseMat& SparseMat::ਸੰਚਾਲਕ=(ਅਚਲ Mat& m)
{
    ਵਾਪਸ (*ਇਹ = SparseMat(m));
}

ਖਾਲੀ SparseMat::ਸੌਂਪੋ_ਮੰਜ਼ਿਲ(SparseMat& m, ਪੂਰਨ _type) ਅਚਲ
{
    ਜੇ( _type < 0 )
        m = *ਇਹ;
    ਨਹੀਂਤਾਂ
        convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ SparseMat::addref()
{
    ਜੇ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

ਖਾਲੀ SparseMat::release()
{
    ਜੇ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        ਮਿਟਾਓ hdr;
    hdr = 0;
}

size_t SparseMat::hash(ਪੂਰਨ i0) ਅਚਲ
{
    ਵਾਪਸ (size_t)i0;
}

size_t SparseMat::hash(ਪੂਰਨ i0, ਪੂਰਨ i1) ਅਚਲ
{
    ਵਾਪਸ (size_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ)i0 * HASH_SCALE + (ਬਿਨਾਨਿਸ਼ਾਨੀ)i1;
}

size_t SparseMat::hash(ਪੂਰਨ i0, ਪੂਰਨ i1, ਪੂਰਨ i2) ਅਚਲ
{
    ਵਾਪਸ ((size_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ)i0 * HASH_SCALE + (ਬਿਨਾਨਿਸ਼ਾਨੀ)i1) * HASH_SCALE + (ਬਿਨਾਨਿਸ਼ਾਨੀ)i2;
}

size_t SparseMat::hash(ਅਚਲ ਪੂਰਨ* idx) ਅਚਲ
{
    size_t h = (ਬਿਨਾਨਿਸ਼ਾਨੀ)idx[0];
    ਜੇ( !hdr )
        ਵਾਪਸ 0;
    ਪੂਰਨ d = hdr->dims;
    ਲਈ(ਪੂਰਨ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (ਬਿਨਾਨਿਸ਼ਾਨੀ)idx[i];
    ਵਾਪਸ h;
}


SparseMat::SparseMat(ਅਚਲ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ਪੂਰਨ i, idx[CV_MAX_DIM] = {0}, d = m.dims, ਆਖਰੀ_ਮਾਪ = m.size[d - 1];
    size_t esz = m.elem_ਮਾਪ();
    ਅਚਲ uchar* dptr = m.ptr();

    ਲਈ(;;)
    {
        ਲਈ( i = 0; i < ਆਖਰੀ_ਮਾਪ; i++, dptr += esz )
        {
            ਜੇ( isZeroElem(dptr, esz) )
                ਜਾਰੀ;
            idx[d-1] = i;
            uchar* ਮੰਜ਼ਿਲ = new_ਗੰਢ(idx, hash(idx));
            copyElem( dptr, ਮੰਜ਼ਿਲ, esz );
        }

        ਲਈ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.ਕਦਮ[i] - m.size[i+1]*m.ਕਦਮ[i+1];
            ਜੇ( ++idx[i] < m.size[i] )
                ਰੋਕੋ;
            idx[i] = 0;
        }
        ਜੇ( i < 0 )
            ਰੋਕੋ;
    }
}

ਖਾਲੀ SparseMat::create(ਪੂਰਨ d, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    ਲਈ( ਪੂਰਨ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ਜੇ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ਪੂਰਨ i;
        ਲਈ( i = 0; i < d; i++ )
            ਜੇ( _sizes[i] != hdr->size[i] )
                ਰੋਕੋ;
        ਜੇ( i == d )
        {
            clear();
            ਵਾਪਸ;
        }
    }
    ਪੂਰਨ _sizes_backup[CV_MAX_DIM]; // #5991
    ਜੇ (hdr && _sizes == hdr->size)
    {
        ਲਈ(ਪੂਰਨ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = ਨਵਾਂ Hdr(d, _sizes, _type);
}

ਖਾਲੀ SparseMat::copy_ਮੰਜ਼ਿਲ( SparseMat& m ) ਅਚਲ
{
    ਜੇ( hdr == m.hdr )
        ਵਾਪਸ;
    ਜੇ( !hdr )
    {
        m.release();
        ਵਾਪਸ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_ਮਾਪ();

    ਲਈ( size_t i = 0; i < N; i++, ++from )
    {
        ਅਚਲ Node* n = from.ਗੰਢ();
        uchar* ਮੰਜ਼ਿਲ = m.new_ਗੰਢ(n->idx, n->hashval);
        copyElem( from.ptr, ਮੰਜ਼ਿਲ, esz );
    }
}

ਖਾਲੀ SparseMat::copy_ਮੰਜ਼ਿਲ( Mat& m ) ਅਚਲ
{
    CV_Assert( hdr );
    ਪੂਰਨ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_ਮਾਪ();

    ਲਈ( size_t i = 0; i < N; i++, ++from )
    {
        ਅਚਲ Node* n = from.ਗੰਢ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


ਖਾਲੀ SparseMat::convert_ਮੰਜ਼ਿਲ( SparseMat& m, ਪੂਰਨ rtype, ਦੁੱਗਣਾ alpha ) ਅਚਲ
{
    ਪੂਰਨ cn = channels();
    ਜੇ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ਜੇ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat ਆਰਜ਼ੀ_2;
        convert_ਮੰਜ਼ਿਲ(ਆਰਜ਼ੀ_2, rtype, alpha);
        m = ਆਰਜ਼ੀ_2;
        ਵਾਪਸ;
    }

    CV_Assert(hdr != 0);
    ਜੇ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ਜੇ( alpha == 1 )
    {
        Convert_ਅੰਕੜੇ cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        ਲਈ( size_t i = 0; i < N; i++, ++from )
        {
            ਅਚਲ Node* n = from.ਗੰਢ();
            uchar* ਮੰਜ਼ਿਲ = hdr == m.hdr ? from.ptr : m.new_ਗੰਢ(n->idx, n->hashval);
            cvtfunc( from.ptr, ਮੰਜ਼ਿਲ, cn );
        }
    }
    ਨਹੀਂਤਾਂ
    {
        Convert_Scale_ਅੰਕੜੇ cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        ਲਈ( size_t i = 0; i < N; i++, ++from )
        {
            ਅਚਲ Node* n = from.ਗੰਢ();
            uchar* ਮੰਜ਼ਿਲ = hdr == m.hdr ? from.ptr : m.new_ਗੰਢ(n->idx, n->hashval);
            cvtfunc( from.ptr, ਮੰਜ਼ਿਲ, cn, alpha, 0 );
        }
    }
}


ਖਾਲੀ SparseMat::convert_ਮੰਜ਼ਿਲ( Mat& m, ਪੂਰਨ rtype, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta ) ਅਚਲ
{
    ਪੂਰਨ cn = channels();
    ਜੇ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ਜੇ( alpha == 1 && beta == 0 )
    {
        Convert_ਅੰਕੜੇ cvtfunc = getConvertElem(type(), rtype);
        ਲਈ( size_t i = 0; i < N; i++, ++from )
        {
            ਅਚਲ Node* n = from.ਗੰਢ();
            uchar* ਮੰਜ਼ਿਲ = m.ptr(n->idx);
            cvtfunc( from.ptr, ਮੰਜ਼ਿਲ, cn );
        }
    }
    ਨਹੀਂਤਾਂ
    {
        Convert_Scale_ਅੰਕੜੇ cvtfunc = getConvertScaleElem(type(), rtype);
        ਲਈ( size_t i = 0; i < N; i++, ++from )
        {
            ਅਚਲ Node* n = from.ਗੰਢ();
            uchar* ਮੰਜ਼ਿਲ = m.ptr(n->idx);
            cvtfunc( from.ptr, ਮੰਜ਼ਿਲ, cn, alpha, beta );
        }
    }
}

ਖਾਲੀ SparseMat::clear()
{
    ਜੇ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ਪੂਰਨ i0, ਤਾਰਕਿਕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h && elem->idx[0] == i0 )
            ਵਾਪਸ &value<uchar>(elem);
        nidx = elem->next;
    }

    ਜੇ( createMissing )
    {
        ਪੂਰਨ idx[] = { i0 };
        ਵਾਪਸ new_ਗੰਢ( idx, h );
    }
    ਵਾਪਸ ਨਿਰਾ;
}

uchar* SparseMat::ptr(ਪੂਰਨ i0, ਪੂਰਨ i1, ਤਾਰਕਿਕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ਵਾਪਸ &value<uchar>(elem);
        nidx = elem->next;
    }

    ਜੇ( createMissing )
    {
        ਪੂਰਨ idx[] = { i0, i1 };
        ਵਾਪਸ new_ਗੰਢ( idx, h );
    }
    ਵਾਪਸ ਨਿਰਾ;
}

uchar* SparseMat::ptr(ਪੂਰਨ i0, ਪੂਰਨ i1, ਪੂਰਨ i2, ਤਾਰਕਿਕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ਵਾਪਸ &value<uchar>(elem);
        nidx = elem->next;
    }

    ਜੇ( createMissing )
    {
        ਪੂਰਨ idx[] = { i0, i1, i2 };
        ਵਾਪਸ new_ਗੰਢ( idx, h );
    }
    ਵਾਪਸ ਨਿਰਾ;
}

uchar* SparseMat::ptr(ਅਚਲ ਪੂਰਨ* idx, ਤਾਰਕਿਕ createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ਪੂਰਨ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h )
        {
            ਲਈ( i = 0; i < d; i++ )
                ਜੇ( elem->idx[i] != idx[i] )
                    ਰੋਕੋ;
            ਜੇ( i == d )
                ਵਾਪਸ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ਵਾਪਸ createMissing ? new_ਗੰਢ(idx, h) : ਨਿਰਾ;
}

ਖਾਲੀ SparseMat::erase(ਪੂਰਨ i0, ਪੂਰਨ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ਰੋਕੋ;
        previdx = nidx;
        nidx = elem->next;
    }

    ਜੇ( nidx )
        remove_ਗੰਢ(hidx, nidx, previdx);
}

ਖਾਲੀ SparseMat::erase(ਪੂਰਨ i0, ਪੂਰਨ i1, ਪੂਰਨ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ਰੋਕੋ;
        previdx = nidx;
        nidx = elem->next;
    }

    ਜੇ( nidx )
        remove_ਗੰਢ(hidx, nidx, previdx);
}

ਖਾਲੀ SparseMat::erase(ਅਚਲ ਪੂਰਨ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ਪੂਰਨ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ਜਦੋਂ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ਜੇ( elem->hashval == h )
        {
            ਲਈ( i = 0; i < d; i++ )
                ਜੇ( elem->idx[i] != idx[i] )
                    ਰੋਕੋ;
            ਜੇ( i == d )
                ਰੋਕੋ;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ਜੇ( nidx )
        remove_ਗੰਢ(hidx, nidx, previdx);
}

ਖਾਲੀ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ਜੇ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::ਲਘੂਗਣਕ((ਦੁੱਗਣਾ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    ਲਈ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    ਲਈ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ਜਦੋਂ( nidx )
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

uchar* SparseMat::new_ਗੰਢ(ਅਚਲ ਪੂਰਨ* idx, size_t hashval)
{
    ਅਚਲ ਪੂਰਨ HASH_MAX_FILL_ਗੁਣਕ=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ਜੇ( ++hdr->ਗੰਢ_ਗਿਣਤੀ > hsize*HASH_MAX_FILL_ਗੁਣਕ )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ਜੇ( !hdr->freeList )
    {
        size_t i, nsz = hdr->ਗੰਢ_ਮਾਪ, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        ਲਈ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ਪੂਰਨ i, d = hdr->dims;
    ਲਈ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_ਮਾਪ();
    uchar* p = &value<uchar>(elem);
    ਜੇ( esz == ਆਕਾਰ(ਦਸ਼ਮਲਵ) )
        *((ਦਸ਼ਮਲਵ*)p) = 0.f;
    ਨਹੀਂਤਾਂ ਜੇ( esz == ਆਕਾਰ(ਦੁੱਗਣਾ) )
        *((ਦੁੱਗਣਾ*)p) = 0.;
    ਨਹੀਂਤਾਂ
        memset(p, 0, esz);

    ਵਾਪਸ p;
}


ਖਾਲੀ SparseMat::remove_ਗੰਢ(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ਗੰਢ(nidx);
    ਜੇ( previdx )
    {
        Node* prev = ਗੰਢ(previdx);
        prev->next = n->next;
    }
    ਨਹੀਂਤਾਂ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ਗੰਢ_ਗਿਣਤੀ;
}

//
// Operations
//
ਦੁੱਗਣਾ norm( ਅਚਲ SparseMat& src, ਪੂਰਨ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_ਨਕਾਬ;
    ਪੂਰਨ type = src.type();
    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ਜੇ( type == CV_32F )
    {
        ਜੇ( normType == NORM_INF )
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, std::ਨਿਰਪੇਖ((ਦੁੱਗਣਾ)it.value<ਦਸ਼ਮਲਵ>()));
            }
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L1 )
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਨਤੀਜਾ += std::ਨਿਰਪੇਖ(it.value<ਦਸ਼ਮਲਵ>());
            }
        ਨਹੀਂਤਾਂ
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਦੁੱਗਣਾ v = it.value<ਦਸ਼ਮਲਵ>();
                ਨਤੀਜਾ += v*v;
            }
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64F )
    {
        ਜੇ( normType == NORM_INF )
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, std::ਨਿਰਪੇਖ(it.value<ਦੁੱਗਣਾ>()));
            }
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L1 )
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਨਤੀਜਾ += std::ਨਿਰਪੇਖ(it.value<ਦੁੱਗਣਾ>());
            }
        ਨਹੀਂਤਾਂ
            ਲਈ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ਦੁੱਗਣਾ v = it.value<ਦੁੱਗਣਾ>();
                ਨਤੀਜਾ += v*v;
            }
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ਜੇ( normType == NORM_L2 )
        ਨਤੀਜਾ = std::ਵਰਗਮੂਲ(ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਖਾਲੀ minMaxLoc( ਅਚਲ SparseMat& src, ਦੁੱਗਣਾ* _minval, ਦੁੱਗਣਾ* _maxval, ਪੂਰਨ* _minidx, ਪੂਰਨ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ਪੂਰਨ type = src.type();
    ਅਚਲ ਪੂਰਨ *minidx = 0, *maxidx = 0;

    ਜੇ( type == CV_32F )
    {
        ਦਸ਼ਮਲਵ minval = FLT_MAX, maxval = -FLT_MAX;
        ਲਈ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ਦਸ਼ਮਲਵ v = it.value<ਦਸ਼ਮਲਵ>();
            ਜੇ( v < minval )
            {
                minval = v;
                minidx = it.ਗੰਢ()->idx;
            }
            ਜੇ( v > maxval )
            {
                maxval = v;
                maxidx = it.ਗੰਢ()->idx;
            }
        }
        ਜੇ( _minval )
            *_minval = minval;
        ਜੇ( _maxval )
            *_maxval = maxval;
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64F )
    {
        ਦੁੱਗਣਾ minval = DBL_MAX, maxval = -DBL_MAX;
        ਲਈ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ਦੁੱਗਣਾ v = it.value<ਦੁੱਗਣਾ>();
            ਜੇ( v < minval )
            {
                minval = v;
                minidx = it.ਗੰਢ()->idx;
            }
            ਜੇ( v > maxval )
            {
                maxval = v;
                maxidx = it.ਗੰਢ()->idx;
            }
        }
        ਜੇ( _minval )
            *_minval = minval;
        ਜੇ( _maxval )
            *_maxval = maxval;
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ਜੇ( _minidx && minidx )
        ਲਈ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ਜੇ( _maxidx && maxidx )
        ਲਈ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


ਖਾਲੀ normalize( ਅਚਲ SparseMat& src, SparseMat& dst, ਦੁੱਗਣਾ a, ਪੂਰਨ norm_type )
{
    CV_INSTRUMENT_REGION();

    ਦੁੱਗਣਾ scale = 1;
    ਜੇ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_ਮੰਜ਼ਿਲ( dst, -1, scale );
}

} // cv::
