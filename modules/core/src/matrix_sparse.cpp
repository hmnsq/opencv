// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

template<typename T1, typename T2> শূন্য
convert_তথ্য(ধ্রুবক শূন্য* _from, শূন্য* _to, পূর্ণসংখ্যা cn)
{
    ধ্রুবক T1* from = (ধ্রুবক T1*)_from;
    T2* to = (T2*)_to;
    যদি( cn == 1 )
        *to = saturate_cast<T2>(*from);
    অন্যথা
        জন্য( পূর্ণসংখ্যা i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> শূন্য
convert_Scale_তথ্য(ধ্রুবক শূন্য* _from, শূন্য* _to, পূর্ণসংখ্যা cn, দ্বিগুণ alpha, দ্বিগুণ beta)
{
    ধ্রুবক T1* from = (ধ্রুবক T1*)_from;
    T2* to = (T2*)_to;
    যদি( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    অন্যথা
        জন্য( পূর্ণসংখ্যা i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

প্রকারলেখ শূন্য (*Convert_তথ্য)(ধ্রুবক শূন্য* from, শূন্য* to, পূর্ণসংখ্যা cn);
প্রকারলেখ শূন্য (*Convert_Scale_তথ্য)(ধ্রুবক শূন্য* from, শূন্য* to, পূর্ণসংখ্যা cn, দ্বিগুণ alpha, দ্বিগুণ beta);

স্থির Convert_তথ্য getConvertElem(পূর্ণসংখ্যা fromType, পূর্ণসংখ্যা toType)
{
    স্থির Convert_তথ্য tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convert_তথ্য<uchar, uchar>, convert_তথ্য<uchar, schar>,
      convert_তথ্য<uchar, ushort>, convert_তথ্য<uchar, short>,
      convert_তথ্য<uchar, পূর্ণসংখ্যা>, convert_তথ্য<uchar, দশমিক>,
      convert_তথ্য<uchar, দ্বিগুণ>, 0 },

    { convert_তথ্য<schar, uchar>, convert_তথ্য<schar, schar>,
      convert_তথ্য<schar, ushort>, convert_তথ্য<schar, short>,
      convert_তথ্য<schar, পূর্ণসংখ্যা>, convert_তথ্য<schar, দশমিক>,
      convert_তথ্য<schar, দ্বিগুণ>, 0 },

    { convert_তথ্য<ushort, uchar>, convert_তথ্য<ushort, schar>,
      convert_তথ্য<ushort, ushort>, convert_তথ্য<ushort, short>,
      convert_তথ্য<ushort, পূর্ণসংখ্যা>, convert_তথ্য<ushort, দশমিক>,
      convert_তথ্য<ushort, দ্বিগুণ>, 0 },

    { convert_তথ্য<short, uchar>, convert_তথ্য<short, schar>,
      convert_তথ্য<short, ushort>, convert_তথ্য<short, short>,
      convert_তথ্য<short, পূর্ণসংখ্যা>, convert_তথ্য<short, দশমিক>,
      convert_তথ্য<short, দ্বিগুণ>, 0 },

    { convert_তথ্য<পূর্ণসংখ্যা, uchar>, convert_তথ্য<পূর্ণসংখ্যা, schar>,
      convert_তথ্য<পূর্ণসংখ্যা, ushort>, convert_তথ্য<পূর্ণসংখ্যা, short>,
      convert_তথ্য<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, convert_তথ্য<পূর্ণসংখ্যা, দশমিক>,
      convert_তথ্য<পূর্ণসংখ্যা, দ্বিগুণ>, 0 },

    { convert_তথ্য<দশমিক, uchar>, convert_তথ্য<দশমিক, schar>,
      convert_তথ্য<দশমিক, ushort>, convert_তথ্য<দশমিক, short>,
      convert_তথ্য<দশমিক, পূর্ণসংখ্যা>, convert_তথ্য<দশমিক, দশমিক>,
      convert_তথ্য<দশমিক, দ্বিগুণ>, 0 },

    { convert_তথ্য<দ্বিগুণ, uchar>, convert_তথ্য<দ্বিগুণ, schar>,
      convert_তথ্য<দ্বিগুণ, ushort>, convert_তথ্য<দ্বিগুণ, short>,
      convert_তথ্য<দ্বিগুণ, পূর্ণসংখ্যা>, convert_তথ্য<দ্বিগুণ, দশমিক>,
      convert_তথ্য<দ্বিগুণ, দ্বিগুণ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_তথ্য func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ফেরত func;
}

স্থির Convert_Scale_তথ্য getConvertScaleElem(পূর্ণসংখ্যা fromType, পূর্ণসংখ্যা toType)
{
    স্থির Convert_Scale_তথ্য tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convert_Scale_তথ্য<uchar, uchar>, convert_Scale_তথ্য<uchar, schar>,
      convert_Scale_তথ্য<uchar, ushort>, convert_Scale_তথ্য<uchar, short>,
      convert_Scale_তথ্য<uchar, পূর্ণসংখ্যা>, convert_Scale_তথ্য<uchar, দশমিক>,
      convert_Scale_তথ্য<uchar, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<schar, uchar>, convert_Scale_তথ্য<schar, schar>,
      convert_Scale_তথ্য<schar, ushort>, convert_Scale_তথ্য<schar, short>,
      convert_Scale_তথ্য<schar, পূর্ণসংখ্যা>, convert_Scale_তথ্য<schar, দশমিক>,
      convert_Scale_তথ্য<schar, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<ushort, uchar>, convert_Scale_তথ্য<ushort, schar>,
      convert_Scale_তথ্য<ushort, ushort>, convert_Scale_তথ্য<ushort, short>,
      convert_Scale_তথ্য<ushort, পূর্ণসংখ্যা>, convert_Scale_তথ্য<ushort, দশমিক>,
      convert_Scale_তথ্য<ushort, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<short, uchar>, convert_Scale_তথ্য<short, schar>,
      convert_Scale_তথ্য<short, ushort>, convert_Scale_তথ্য<short, short>,
      convert_Scale_তথ্য<short, পূর্ণসংখ্যা>, convert_Scale_তথ্য<short, দশমিক>,
      convert_Scale_তথ্য<short, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<পূর্ণসংখ্যা, uchar>, convert_Scale_তথ্য<পূর্ণসংখ্যা, schar>,
      convert_Scale_তথ্য<পূর্ণসংখ্যা, ushort>, convert_Scale_তথ্য<পূর্ণসংখ্যা, short>,
      convert_Scale_তথ্য<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, convert_Scale_তথ্য<পূর্ণসংখ্যা, দশমিক>,
      convert_Scale_তথ্য<পূর্ণসংখ্যা, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<দশমিক, uchar>, convert_Scale_তথ্য<দশমিক, schar>,
      convert_Scale_তথ্য<দশমিক, ushort>, convert_Scale_তথ্য<দশমিক, short>,
      convert_Scale_তথ্য<দশমিক, পূর্ণসংখ্যা>, convert_Scale_তথ্য<দশমিক, দশমিক>,
      convert_Scale_তথ্য<দশমিক, দ্বিগুণ>, 0 },

    { convert_Scale_তথ্য<দ্বিগুণ, uchar>, convert_Scale_তথ্য<দ্বিগুণ, schar>,
      convert_Scale_তথ্য<দ্বিগুণ, ushort>, convert_Scale_তথ্য<দ্বিগুণ, short>,
      convert_Scale_তথ্য<দ্বিগুণ, পূর্ণসংখ্যা>, convert_Scale_তথ্য<দ্বিগুণ, দশমিক>,
      convert_Scale_তথ্য<দ্বিগুণ, দ্বিগুণ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_তথ্য func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ফেরত func;
}

গণনা { HASH_SIZE0 = 8 };

স্থির অন্তঃরেখ শূন্য copyElem(ধ্রুবক uchar* from, uchar* to, size_t elem_আকার)
{
    size_t i;
    জন্য( i = 0; i + আকার(পূর্ণসংখ্যা) <= elem_আকার; i += আকার(পূর্ণসংখ্যা) )
        *(পূর্ণসংখ্যা*)(to + i) = *(ধ্রুবক পূর্ণসংখ্যা*)(from + i);
    জন্য( ; i < elem_আকার; i++ )
        to[i] = from[i];
}

স্থির অন্তঃরেখ যৌক্তিক isZeroElem(ধ্রুবক uchar* তথ্য, size_t elem_আকার)
{
    size_t i;
    জন্য( i = 0; i + আকার(পূর্ণসংখ্যা) <= elem_আকার; i += আকার(পূর্ণসংখ্যা) )
        যদি( *(পূর্ণসংখ্যা*)(তথ্য + i) != 0 )
            ফেরত মিথ্যা;
    জন্য( ; i < elem_আকার; i++ )
        যদি( তথ্য[i] != 0 )
            ফেরত মিথ্যা;
    ফেরত সত্য;
}

SparseMat::Hdr::Hdr( পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type )
{
    refcount = 1;

    dims = _dims;
    মান_Offset = (পূর্ণসংখ্যা)align_আকার(আকার(SparseMat::Node) - MAX_DIM*আকার(পূর্ণসংখ্যা) +
                                 dims*আকার(পূর্ণসংখ্যা), CV_ELEM_SIZE1(_type));
    গ্রন্থি_আকার = align_আকার(মান_Offset +
        CV_ELEM_আকার(_type), (পূর্ণসংখ্যা)আকার(size_t));

    পূর্ণসংখ্যা i;
    জন্য( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    জন্য( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

শূন্য SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#যদি defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(গ্রন্থি_আকার);
#যদি defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    গ্রন্থি_গুনতি = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ধ্রুবক SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ধ্রুবক SparseMat& m)
{
    যদি( এই != &m )
    {
        যদি( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ফেরত *এই;
}

SparseMat& SparseMat::operator=(ধ্রুবক Mat& m)
{
    ফেরত (*এই = SparseMat(m));
}

শূন্য SparseMat::assignTo(SparseMat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    যদি( _type < 0 )
        m = *এই;
    অন্যথা
        convertTo(m, _type);
}

শূন্য SparseMat::addref()
{
    যদি( hdr )
        CV_XADD(&hdr->refcount, 1);
}

শূন্য SparseMat::release()
{
    যদি( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(পূর্ণসংখ্যা i0) ধ্রুবক
{
    ফেরত (size_t)i0;
}

size_t SparseMat::hash(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1) ধ্রুবক
{
    ফেরত (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1, পূর্ণসংখ্যা i2) ধ্রুবক
{
    ফেরত ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ধ্রুবক পূর্ণসংখ্যা* idx) ধ্রুবক
{
    size_t h = (unsigned)idx[0];
    যদি( !hdr )
        ফেরত 0;
    পূর্ণসংখ্যা d = hdr->dims;
    জন্য(পূর্ণসংখ্যা i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ফেরত h;
}


SparseMat::SparseMat(ধ্রুবক Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    পূর্ণসংখ্যা i, idx[CV_MAX_DIM] = {0}, d = m.dims, last_আকার = m.size[d - 1];
    size_t esz = m.elem_আকার();
    ধ্রুবক uchar* dptr = m.ptr();

    জন্য(;;)
    {
        জন্য( i = 0; i < last_আকার; i++, dptr += esz )
        {
            যদি( isZeroElem(dptr, esz) )
                চালু;
            idx[d-1] = i;
            uchar* to = new_গ্রন্থি(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        জন্য( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            যদি( ++idx[i] < m.size[i] )
                বিরতি;
            idx[i] = 0;
        }
        যদি( i < 0 )
            বিরতি;
    }
}

শূন্য SparseMat::create(পূর্ণসংখ্যা d, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    জন্য( পূর্ণসংখ্যা i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    যদি( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        পূর্ণসংখ্যা i;
        জন্য( i = 0; i < d; i++ )
            যদি( _sizes[i] != hdr->size[i] )
                বিরতি;
        যদি( i == d )
        {
            clear();
            ফেরত;
        }
    }
    পূর্ণসংখ্যা _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (hdr && _sizes == hdr->size)
    {
        জন্য(পূর্ণসংখ্যা i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = নতুন Hdr(d, _sizes, _type);
}

শূন্য SparseMat::copyTo( SparseMat& m ) ধ্রুবক
{
    যদি( hdr == m.hdr )
        ফেরত;
    যদি( !hdr )
    {
        m.release();
        ফেরত;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_আকার();

    জন্য( size_t i = 0; i < N; i++, ++from )
    {
        ধ্রুবক Node* n = from.গ্রন্থি();
        uchar* to = m.new_গ্রন্থি(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

শূন্য SparseMat::copyTo( Mat& m ) ধ্রুবক
{
    CV_Assert( hdr );
    পূর্ণসংখ্যা ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_আকার();

    জন্য( size_t i = 0; i < N; i++, ++from )
    {
        ধ্রুবক Node* n = from.গ্রন্থি();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


শূন্য SparseMat::convertTo( SparseMat& m, পূর্ণসংখ্যা rtype, দ্বিগুণ alpha ) ধ্রুবক
{
    পূর্ণসংখ্যা cn = channels();
    যদি( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    যদি( hdr == m.hdr && rtype != type()  )
    {
        SparseMat অস্থায়ী_2;
        convertTo(অস্থায়ী_2, rtype, alpha);
        m = অস্থায়ী_2;
        ফেরত;
    }

    CV_Assert(hdr != 0);
    যদি( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    যদি( alpha == 1 )
    {
        Convert_তথ্য cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        জন্য( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুবক Node* n = from.গ্রন্থি();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_গ্রন্থি(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    অন্যথা
    {
        Convert_Scale_তথ্য cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        জন্য( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুবক Node* n = from.গ্রন্থি();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_গ্রন্থি(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


শূন্য SparseMat::convertTo( Mat& m, পূর্ণসংখ্যা rtype, দ্বিগুণ alpha, দ্বিগুণ beta ) ধ্রুবক
{
    পূর্ণসংখ্যা cn = channels();
    যদি( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    যদি( alpha == 1 && beta == 0 )
    {
        Convert_তথ্য cvtfunc = getConvertElem(type(), rtype);
        জন্য( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুবক Node* n = from.গ্রন্থি();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    অন্যথা
    {
        Convert_Scale_তথ্য cvtfunc = getConvertScaleElem(type(), rtype);
        জন্য( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুবক Node* n = from.গ্রন্থি();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

শূন্য SparseMat::clear()
{
    যদি( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(পূর্ণসংখ্যা i0, যৌক্তিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 )
            ফেরত &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণসংখ্যা idx[] = { i0 };
        ফেরত new_গ্রন্থি( idx, h );
    }
    ফেরত ফাঁকা;
}

uchar* SparseMat::ptr(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1, যৌক্তিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ফেরত &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণসংখ্যা idx[] = { i0, i1 };
        ফেরত new_গ্রন্থি( idx, h );
    }
    ফেরত ফাঁকা;
}

uchar* SparseMat::ptr(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1, পূর্ণসংখ্যা i2, যৌক্তিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ফেরত &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণসংখ্যা idx[] = { i0, i1, i2 };
        ফেরত new_গ্রন্থি( idx, h );
    }
    ফেরত ফাঁকা;
}

uchar* SparseMat::ptr(ধ্রুবক পূর্ণসংখ্যা* idx, যৌক্তিক createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    পূর্ণসংখ্যা i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h )
        {
            জন্য( i = 0; i < d; i++ )
                যদি( elem->idx[i] != idx[i] )
                    বিরতি;
            যদি( i == d )
                ফেরত &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ফেরত createMissing ? new_গ্রন্থি(idx, h) : ফাঁকা;
}

শূন্য SparseMat::erase(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            বিরতি;
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্রন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::erase(পূর্ণসংখ্যা i0, পূর্ণসংখ্যা i1, পূর্ণসংখ্যা i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            বিরতি;
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্রন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::erase(ধ্রুবক পূর্ণসংখ্যা* idx, size_t* hashval)
{
    CV_Assert( hdr );
    পূর্ণসংখ্যা i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যখন( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h )
        {
            জন্য( i = 0; i < d; i++ )
                যদি( elem->idx[i] != idx[i] )
                    বিরতি;
            যদি( i == d )
                বিরতি;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্রন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    যদি((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((দ্বিগুণ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    জন্য( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    জন্য( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        যখন( nidx )
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

uchar* SparseMat::new_গ্রন্থি(ধ্রুবক পূর্ণসংখ্যা* idx, size_t hashval)
{
    ধ্রুবক পূর্ণসংখ্যা HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    যদি( ++hdr->গ্রন্থি_গুনতি > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    যদি( !hdr->freeList )
    {
        size_t i, nsz = hdr->গ্রন্থি_আকার, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        জন্য( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    পূর্ণসংখ্যা i, d = hdr->dims;
    জন্য( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_আকার();
    uchar* p = &value<uchar>(elem);
    যদি( esz == আকার(দশমিক) )
        *((দশমিক*)p) = 0.f;
    অন্যথা যদি( esz == আকার(দ্বিগুণ) )
        *((দ্বিগুণ*)p) = 0.;
    অন্যথা
        memset(p, 0, esz);

    ফেরত p;
}


শূন্য SparseMat::remove_গ্রন্থি(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = গ্রন্থি(nidx);
    যদি( previdx )
    {
        Node* পূর্ব = গ্রন্থি(previdx);
        পূর্ব->next = n->next;
    }
    অন্যথা
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->গ্রন্থি_গুনতি;
}

//
// Operations
//
দ্বিগুণ norm( ধ্রুবক SparseMat& src, পূর্ণসংখ্যা normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    পূর্ণসংখ্যা type = src.type();
    দ্বিগুণ ফলাফল = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    যদি( type == CV_32F )
    {
        যদি( normType == NORM_INF )
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ফলাফল = std::max(ফলাফল, std::abs((দ্বিগুণ)it.value<দশমিক>()));
            }
        অন্যথা যদি( normType == NORM_L1 )
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ফলাফল += std::abs(it.value<দশমিক>());
            }
        অন্যথা
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                দ্বিগুণ v = it.value<দশমিক>();
                ফলাফল += v*v;
            }
    }
    অন্যথা যদি( type == CV_64F )
    {
        যদি( normType == NORM_INF )
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ফলাফল = std::max(ফলাফল, std::abs(it.value<দ্বিগুণ>()));
            }
        অন্যথা যদি( normType == NORM_L1 )
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ফলাফল += std::abs(it.value<দ্বিগুণ>());
            }
        অন্যথা
            জন্য( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                দ্বিগুণ v = it.value<দ্বিগুণ>();
                ফলাফল += v*v;
            }
    }
    অন্যথা
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    যদি( normType == NORM_L2 )
        ফলাফল = std::sqrt(ফলাফল);
    ফেরত ফলাফল;
}

শূন্য minMaxLoc( ধ্রুবক SparseMat& src, দ্বিগুণ* _minval, দ্বিগুণ* _maxval, পূর্ণসংখ্যা* _minidx, পূর্ণসংখ্যা* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    পূর্ণসংখ্যা type = src.type();
    ধ্রুবক পূর্ণসংখ্যা *minidx = 0, *maxidx = 0;

    যদি( type == CV_32F )
    {
        দশমিক minval = FLT_MAX, maxval = -FLT_MAX;
        জন্য( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            দশমিক v = it.value<দশমিক>();
            যদি( v < minval )
            {
                minval = v;
                minidx = it.গ্রন্থি()->idx;
            }
            যদি( v > maxval )
            {
                maxval = v;
                maxidx = it.গ্রন্থি()->idx;
            }
        }
        যদি( _minval )
            *_minval = minval;
        যদি( _maxval )
            *_maxval = maxval;
    }
    অন্যথা যদি( type == CV_64F )
    {
        দ্বিগুণ minval = DBL_MAX, maxval = -DBL_MAX;
        জন্য( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            দ্বিগুণ v = it.value<দ্বিগুণ>();
            যদি( v < minval )
            {
                minval = v;
                minidx = it.গ্রন্থি()->idx;
            }
            যদি( v > maxval )
            {
                maxval = v;
                maxidx = it.গ্রন্থি()->idx;
            }
        }
        যদি( _minval )
            *_minval = minval;
        যদি( _maxval )
            *_maxval = maxval;
    }
    অন্যথা
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    যদি( _minidx && minidx )
        জন্য( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    যদি( _maxidx && maxidx )
        জন্য( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


শূন্য normalize( ধ্রুবক SparseMat& src, SparseMat& dst, দ্বিগুণ a, পূর্ণসংখ্যা norm_type )
{
    CV_INSTRUMENT_REGION();

    দ্বিগুণ scale = 1;
    যদি( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    অন্যথা
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
