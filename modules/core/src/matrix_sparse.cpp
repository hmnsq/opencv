// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencv2/core/mat.hpp"

নামাকাশ cv {

template<typename T1, typename T2> শূন্য
convertData_(ধ্রুৱ শূন্য* _from, শূন্য* _to, পূর্ণাঙ্ক cn)
{
    ধ্রুৱ T1* from = (ধ্রুৱ T1*)_from;
    T2* to = (T2*)_to;
    যদি( cn == 1 )
        *to = saturate_cast<T2>(*from);
    অন্যথা
        বাবে( পূর্ণাঙ্ক i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> শূন্য
convertScaleData_(ধ্রুৱ শূন্য* _from, শূন্য* _to, পূর্ণাঙ্ক cn, দ্ৱিগুণ alpha, দ্ৱিগুণ beta)
{
    ধ্রুৱ T1* from = (ধ্রুৱ T1*)_from;
    T2* to = (T2*)_to;
    যদি( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    অন্যথা
        বাবে( পূর্ণাঙ্ক i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

প্রকারনির্দেশ শূন্য (*ConvertData)(ধ্রুৱ শূন্য* from, শূন্য* to, পূর্ণাঙ্ক cn);
প্রকারনির্দেশ শূন্য (*ConvertScaleData)(ধ্রুৱ শূন্য* from, শূন্য* to, পূর্ণাঙ্ক cn, দ্ৱিগুণ alpha, দ্ৱিগুণ beta);

স্থির ConvertData getConvertElem(পূর্ণাঙ্ক fromType, পূর্ণাঙ্ক toType)
{
    স্থির ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, পূর্ণাঙ্ক>, convertData_<uchar, ভিন্ন>,
      convertData_<uchar, দ্ৱিগুণ>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, পূর্ণাঙ্ক>, convertData_<schar, ভিন্ন>,
      convertData_<schar, দ্ৱিগুণ>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, পূর্ণাঙ্ক>, convertData_<ushort, ভিন্ন>,
      convertData_<ushort, দ্ৱিগুণ>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, পূর্ণাঙ্ক>, convertData_<short, ভিন্ন>,
      convertData_<short, দ্ৱিগুণ>, 0 },

    { convertData_<পূর্ণাঙ্ক, uchar>, convertData_<পূর্ণাঙ্ক, schar>,
      convertData_<পূর্ণাঙ্ক, ushort>, convertData_<পূর্ণাঙ্ক, short>,
      convertData_<পূর্ণাঙ্ক, পূর্ণাঙ্ক>, convertData_<পূর্ণাঙ্ক, ভিন্ন>,
      convertData_<পূর্ণাঙ্ক, দ্ৱিগুণ>, 0 },

    { convertData_<ভিন্ন, uchar>, convertData_<ভিন্ন, schar>,
      convertData_<ভিন্ন, ushort>, convertData_<ভিন্ন, short>,
      convertData_<ভিন্ন, পূর্ণাঙ্ক>, convertData_<ভিন্ন, ভিন্ন>,
      convertData_<ভিন্ন, দ্ৱিগুণ>, 0 },

    { convertData_<দ্ৱিগুণ, uchar>, convertData_<দ্ৱিগুণ, schar>,
      convertData_<দ্ৱিগুণ, ushort>, convertData_<দ্ৱিগুণ, short>,
      convertData_<দ্ৱিগুণ, পূর্ণাঙ্ক>, convertData_<দ্ৱিগুণ, ভিন্ন>,
      convertData_<দ্ৱিগুণ, দ্ৱিগুণ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ঘূৰাই func;
}

স্থির ConvertScaleData getConvertScaleElem(পূর্ণাঙ্ক fromType, পূর্ণাঙ্ক toType)
{
    স্থির ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, পূর্ণাঙ্ক>, convertScaleData_<uchar, ভিন্ন>,
      convertScaleData_<uchar, দ্ৱিগুণ>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, পূর্ণাঙ্ক>, convertScaleData_<schar, ভিন্ন>,
      convertScaleData_<schar, দ্ৱিগুণ>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, পূর্ণাঙ্ক>, convertScaleData_<ushort, ভিন্ন>,
      convertScaleData_<ushort, দ্ৱিগুণ>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, পূর্ণাঙ্ক>, convertScaleData_<short, ভিন্ন>,
      convertScaleData_<short, দ্ৱিগুণ>, 0 },

    { convertScaleData_<পূর্ণাঙ্ক, uchar>, convertScaleData_<পূর্ণাঙ্ক, schar>,
      convertScaleData_<পূর্ণাঙ্ক, ushort>, convertScaleData_<পূর্ণাঙ্ক, short>,
      convertScaleData_<পূর্ণাঙ্ক, পূর্ণাঙ্ক>, convertScaleData_<পূর্ণাঙ্ক, ভিন্ন>,
      convertScaleData_<পূর্ণাঙ্ক, দ্ৱিগুণ>, 0 },

    { convertScaleData_<ভিন্ন, uchar>, convertScaleData_<ভিন্ন, schar>,
      convertScaleData_<ভিন্ন, ushort>, convertScaleData_<ভিন্ন, short>,
      convertScaleData_<ভিন্ন, পূর্ণাঙ্ক>, convertScaleData_<ভিন্ন, ভিন্ন>,
      convertScaleData_<ভিন্ন, দ্ৱিগুণ>, 0 },

    { convertScaleData_<দ্ৱিগুণ, uchar>, convertScaleData_<দ্ৱিগুণ, schar>,
      convertScaleData_<দ্ৱিগুণ, ushort>, convertScaleData_<দ্ৱিগুণ, short>,
      convertScaleData_<দ্ৱিগুণ, পূর্ণাঙ্ক>, convertScaleData_<দ্ৱিগুণ, ভিন্ন>,
      convertScaleData_<দ্ৱিগুণ, দ্ৱিগুণ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ঘূৰাই func;
}

গণনা { HASH_SIZE0 = 8 };

স্থির অন্তঃস্থ শূন্য copyElem(ধ্রুৱ uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    বাবে( i = 0; i + পরিমাণ(পূর্ণাঙ্ক) <= elemSize; i += পরিমাণ(পূর্ণাঙ্ক) )
        *(পূর্ণাঙ্ক*)(to + i) = *(ধ্রুৱ পূর্ণাঙ্ক*)(from + i);
    বাবে( ; i < elemSize; i++ )
        to[i] = from[i];
}

স্থির অন্তঃস্থ তার্কিক isZeroElem(ধ্রুৱ uchar* data, size_t elemSize)
{
    size_t i;
    বাবে( i = 0; i + পরিমাণ(পূর্ণাঙ্ক) <= elemSize; i += পরিমাণ(পূর্ণাঙ্ক) )
        যদি( *(পূর্ণাঙ্ক*)(data + i) != 0 )
            ঘূৰাই মিছা;
    বাবে( ; i < elemSize; i++ )
        যদি( data[i] != 0 )
            ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
}

SparseMat::Hdr::Hdr( পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type )
{
    refcount = 1;

    dims = _dims;
    মান_Offset = (পূর্ণাঙ্ক)alignSize(পরিমাণ(SparseMat::Node) - MAX_DIM*পরিমাণ(পূর্ণাঙ্ক) +
                                 dims*পরিমাণ(পূর্ণাঙ্ক), CV_ELEM_SIZE1(_type));
    গ্ৰন্থি_Size = alignSize(মান_Offset +
        CV_ELEM_SIZE(_type), (পূর্ণাঙ্ক)পরিমাণ(size_t));

    পূর্ণাঙ্ক i;
    বাবে( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    বাবে( ; i < CV_MAX_DIM; i++ )
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
    pool.resize(গ্ৰন্থি_Size);
#যদি defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    গ্ৰন্থি_গুনতি = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ধ্রুৱ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ধ্রুৱ SparseMat& m)
{
    যদি( এতত্ != &m )
    {
        যদি( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ঘূৰাই *এতত্;
}

SparseMat& SparseMat::operator=(ধ্রুৱ Mat& m)
{
    ঘূৰাই (*এতত্ = SparseMat(m));
}

শূন্য SparseMat::assignTo(SparseMat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    যদি( _type < 0 )
        m = *এতত্;
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

size_t SparseMat::hash(পূর্ণাঙ্ক i0) ধ্রুৱ
{
    ঘূৰাই (size_t)i0;
}

size_t SparseMat::hash(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1) ধ্রুৱ
{
    ঘূৰাই (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1, পূর্ণাঙ্ক i2) ধ্রুৱ
{
    ঘূৰাই ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ধ্রুৱ পূর্ণাঙ্ক* idx) ধ্রুৱ
{
    size_t h = (unsigned)idx[0];
    যদি( !hdr )
        ঘূৰাই 0;
    পূর্ণাঙ্ক d = hdr->dims;
    বাবে(পূর্ণাঙ্ক i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ঘূৰাই h;
}


SparseMat::SparseMat(ধ্রুৱ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    পূর্ণাঙ্ক i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ধ্রুৱ uchar* dptr = m.ptr();

    বাবে(;;)
    {
        বাবে( i = 0; i < lastSize; i++, dptr += esz )
        {
            যদি( isZeroElem(dptr, esz) )
                অনুৱর্ত;
            idx[d-1] = i;
            uchar* to = new_গ্ৰন্থি(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        বাবে( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            যদি( ++idx[i] < m.size[i] )
                ভঙ্গ;
            idx[i] = 0;
        }
        যদি( i < 0 )
            ভঙ্গ;
    }
}

শূন্য SparseMat::create(পূর্ণাঙ্ক d, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    বাবে( পূর্ণাঙ্ক i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    যদি( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        পূর্ণাঙ্ক i;
        বাবে( i = 0; i < d; i++ )
            যদি( _sizes[i] != hdr->size[i] )
                ভঙ্গ;
        যদি( i == d )
        {
            clear();
            ঘূৰাই;
        }
    }
    পূর্ণাঙ্ক _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (hdr && _sizes == hdr->size)
    {
        বাবে(পূর্ণাঙ্ক i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = নূতন Hdr(d, _sizes, _type);
}

শূন্য SparseMat::copyTo( SparseMat& m ) ধ্রুৱ
{
    যদি( hdr == m.hdr )
        ঘূৰাই;
    যদি( !hdr )
    {
        m.release();
        ঘূৰাই;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    বাবে( size_t i = 0; i < N; i++, ++from )
    {
        ধ্রুৱ Node* n = from.গ্ৰন্থি();
        uchar* to = m.new_গ্ৰন্থি(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

শূন্য SparseMat::copyTo( Mat& m ) ধ্রুৱ
{
    CV_Assert( hdr );
    পূর্ণাঙ্ক ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    বাবে( size_t i = 0; i < N; i++, ++from )
    {
        ধ্রুৱ Node* n = from.গ্ৰন্থি();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


শূন্য SparseMat::convertTo( SparseMat& m, পূর্ণাঙ্ক rtype, দ্ৱিগুণ alpha ) ধ্রুৱ
{
    পূর্ণাঙ্ক cn = channels();
    যদি( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    যদি( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ঘূৰাই;
    }

    CV_Assert(hdr != 0);
    যদি( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    যদি( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        বাবে( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুৱ Node* n = from.গ্ৰন্থি();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_গ্ৰন্থি(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    অন্যথা
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        বাবে( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুৱ Node* n = from.গ্ৰন্থি();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_গ্ৰন্থি(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


শূন্য SparseMat::convertTo( Mat& m, পূর্ণাঙ্ক rtype, দ্ৱিগুণ alpha, দ্ৱিগুণ beta ) ধ্রুৱ
{
    পূর্ণাঙ্ক cn = channels();
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
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        বাবে( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুৱ Node* n = from.গ্ৰন্থি();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    অন্যথা
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        বাবে( size_t i = 0; i < N; i++, ++from )
        {
            ধ্রুৱ Node* n = from.গ্ৰন্থি();
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

uchar* SparseMat::ptr(পূর্ণাঙ্ক i0, তার্কিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 )
            ঘূৰাই &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণাঙ্ক idx[] = { i0 };
        ঘূৰাই new_গ্ৰন্থি( idx, h );
    }
    ঘূৰাই রিক্ত;
}

uchar* SparseMat::ptr(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1, তার্কিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ঘূৰাই &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণাঙ্ক idx[] = { i0, i1 };
        ঘূৰাই new_গ্ৰন্থি( idx, h );
    }
    ঘূৰাই রিক্ত;
}

uchar* SparseMat::ptr(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1, পূর্ণাঙ্ক i2, তার্কিক createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ঘূৰাই &value<uchar>(elem);
        nidx = elem->next;
    }

    যদি( createMissing )
    {
        পূর্ণাঙ্ক idx[] = { i0, i1, i2 };
        ঘূৰাই new_গ্ৰন্থি( idx, h );
    }
    ঘূৰাই রিক্ত;
}

uchar* SparseMat::ptr(ধ্রুৱ পূর্ণাঙ্ক* idx, তার্কিক createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    পূর্ণাঙ্ক i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h )
        {
            বাবে( i = 0; i < d; i++ )
                যদি( elem->idx[i] != idx[i] )
                    ভঙ্গ;
            যদি( i == d )
                ঘূৰাই &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ঘূৰাই createMissing ? new_গ্ৰন্থি(idx, h) : রিক্ত;
}

শূন্য SparseMat::erase(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ভঙ্গ;
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্ৰন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::erase(পূর্ণাঙ্ক i0, পূর্ণাঙ্ক i1, পূর্ণাঙ্ক i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ভঙ্গ;
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্ৰন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::erase(ধ্রুৱ পূর্ণাঙ্ক* idx, size_t* hashval)
{
    CV_Assert( hdr );
    পূর্ণাঙ্ক i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    যেতিয়ালৈ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        যদি( elem->hashval == h )
        {
            বাবে( i = 0; i < d; i++ )
                যদি( elem->idx[i] != idx[i] )
                    ভঙ্গ;
            যদি( i == d )
                ভঙ্গ;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    যদি( nidx )
        remove_গ্ৰন্থি(hidx, nidx, previdx);
}

শূন্য SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    যদি((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((দ্ৱিগুণ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    বাবে( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    বাবে( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        যেতিয়ালৈ( nidx )
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

uchar* SparseMat::new_গ্ৰন্থি(ধ্রুৱ পূর্ণাঙ্ক* idx, size_t hashval)
{
    ধ্রুৱ পূর্ণাঙ্ক HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    যদি( ++hdr->গ্ৰন্থি_গুনতি > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    যদি( !hdr->freeList )
    {
        size_t i, nsz = hdr->গ্ৰন্থি_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        বাবে( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    পূর্ণাঙ্ক i, d = hdr->dims;
    বাবে( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    যদি( esz == পরিমাণ(ভিন্ন) )
        *((ভিন্ন*)p) = 0.f;
    অন্যথা যদি( esz == পরিমাণ(দ্ৱিগুণ) )
        *((দ্ৱিগুণ*)p) = 0.;
    অন্যথা
        memset(p, 0, esz);

    ঘূৰাই p;
}


শূন্য SparseMat::remove_গ্ৰন্থি(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = গ্ৰন্থি(nidx);
    যদি( previdx )
    {
        Node* prev = গ্ৰন্থি(previdx);
        prev->next = n->next;
    }
    অন্যথা
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->গ্ৰন্থি_গুনতি;
}

//
// Operations
//
দ্ৱিগুণ norm( ধ্রুৱ SparseMat& src, পূর্ণাঙ্ক normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    পূর্ণাঙ্ক type = src.type();
    দ্ৱিগুণ result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    যদি( type == CV_32F )
    {
        যদি( normType == NORM_INF )
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((দ্ৱিগুণ)it.value<ভিন্ন>()));
            }
        অন্যথা যদি( normType == NORM_L1 )
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ভিন্ন>());
            }
        অন্যথা
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                দ্ৱিগুণ v = it.value<ভিন্ন>();
                result += v*v;
            }
    }
    অন্যথা যদি( type == CV_64F )
    {
        যদি( normType == NORM_INF )
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<দ্ৱিগুণ>()));
            }
        অন্যথা যদি( normType == NORM_L1 )
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<দ্ৱিগুণ>());
            }
        অন্যথা
            বাবে( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                দ্ৱিগুণ v = it.value<দ্ৱিগুণ>();
                result += v*v;
            }
    }
    অন্যথা
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    যদি( normType == NORM_L2 )
        result = std::sqrt(result);
    ঘূৰাই result;
}

শূন্য minMaxLoc( ধ্রুৱ SparseMat& src, দ্ৱিগুণ* _minval, দ্ৱিগুণ* _maxval, পূর্ণাঙ্ক* _minidx, পূর্ণাঙ্ক* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    পূর্ণাঙ্ক type = src.type();
    ধ্রুৱ পূর্ণাঙ্ক *minidx = 0, *maxidx = 0;

    যদি( type == CV_32F )
    {
        ভিন্ন minval = FLT_MAX, maxval = -FLT_MAX;
        বাবে( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ভিন্ন v = it.value<ভিন্ন>();
            যদি( v < minval )
            {
                minval = v;
                minidx = it.গ্ৰন্থি()->idx;
            }
            যদি( v > maxval )
            {
                maxval = v;
                maxidx = it.গ্ৰন্থি()->idx;
            }
        }
        যদি( _minval )
            *_minval = minval;
        যদি( _maxval )
            *_maxval = maxval;
    }
    অন্যথা যদি( type == CV_64F )
    {
        দ্ৱিগুণ minval = DBL_MAX, maxval = -DBL_MAX;
        বাবে( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            দ্ৱিগুণ v = it.value<দ্ৱিগুণ>();
            যদি( v < minval )
            {
                minval = v;
                minidx = it.গ্ৰন্থি()->idx;
            }
            যদি( v > maxval )
            {
                maxval = v;
                maxidx = it.গ্ৰন্থি()->idx;
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
        বাবে( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    যদি( _maxidx && maxidx )
        বাবে( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


শূন্য normalize( ধ্রুৱ SparseMat& src, SparseMat& dst, দ্ৱিগুণ a, পূর্ণাঙ্ক norm_type )
{
    CV_INSTRUMENT_REGION();

    দ্ৱিগুণ scale = 1;
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
