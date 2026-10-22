// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/mat.hpp"

ନାମାକାଶ cv {

template<typename T1, typename T2> ଶୂନ୍ୟ
convertData_(ଅଚଳ ଶୂନ୍ୟ* _from, ଶୂନ୍ୟ* _to, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଅଚଳ T1* from = (ଅଚଳ T1*)_from;
    T2* to = (T2*)_to;
    ଯଦି( cn == 1 )
        *to = saturate_cast<T2>(*from);
    ଅନ୍ୟଥା
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> ଶୂନ୍ୟ
convertScaleData_(ଅଚଳ ଶୂନ୍ୟ* _from, ଶୂନ୍ୟ* _to, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta)
{
    ଅଚଳ T1* from = (ଅଚଳ T1*)_from;
    T2* to = (T2*)_to;
    ଯଦି( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    ଅନ୍ୟଥା
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ConvertData)(ଅଚଳ ଶୂନ୍ୟ* from, ଶୂନ୍ୟ* to, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ConvertScaleData)(ଅଚଳ ଶୂନ୍ୟ* from, ଶୂନ୍ୟ* to, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta);

ସ୍ଥିର ConvertData getConvertElem(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fromType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା toType)
{
    ସ୍ଥିର ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<uchar, ଦଶମିକ>,
      convertData_<uchar, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<schar, ଦଶମିକ>,
      convertData_<schar, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<ushort, ଦଶମିକ>,
      convertData_<ushort, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<short, ଦଶମିକ>,
      convertData_<short, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, uchar>, convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, schar>,
      convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ushort>, convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, short>,
      convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦଶମିକ>,
      convertData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<ଦଶମିକ, uchar>, convertData_<ଦଶମିକ, schar>,
      convertData_<ଦଶମିକ, ushort>, convertData_<ଦଶମିକ, short>,
      convertData_<ଦଶମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<ଦଶମିକ, ଦଶମିକ>,
      convertData_<ଦଶମିକ, ଦ୍ୱିଗୁଣ>, 0 },

    { convertData_<ଦ୍ୱିଗୁଣ, uchar>, convertData_<ଦ୍ୱିଗୁଣ, schar>,
      convertData_<ଦ୍ୱିଗୁଣ, ushort>, convertData_<ଦ୍ୱିଗୁଣ, short>,
      convertData_<ଦ୍ୱିଗୁଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertData_<ଦ୍ୱିଗୁଣ, ଦଶମିକ>,
      convertData_<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ଫେରନ୍ତୁ func;
}

ସ୍ଥିର ConvertScaleData getConvertScaleElem(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fromType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା toType)
{
    ସ୍ଥିର ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<uchar, ଦଶମିକ>,
      convertScaleData_<uchar, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<schar, ଦଶମିକ>,
      convertScaleData_<schar, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<ushort, ଦଶମିକ>,
      convertScaleData_<ushort, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<short, ଦଶମିକ>,
      convertScaleData_<short, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, uchar>, convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, schar>,
      convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ushort>, convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, short>,
      convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦଶମିକ>,
      convertScaleData_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<ଦଶମିକ, uchar>, convertScaleData_<ଦଶମିକ, schar>,
      convertScaleData_<ଦଶମିକ, ushort>, convertScaleData_<ଦଶମିକ, short>,
      convertScaleData_<ଦଶମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<ଦଶମିକ, ଦଶମିକ>,
      convertScaleData_<ଦଶମିକ, ଦ୍ୱିଗୁଣ>, 0 },

    { convertScaleData_<ଦ୍ୱିଗୁଣ, uchar>, convertScaleData_<ଦ୍ୱିଗୁଣ, schar>,
      convertScaleData_<ଦ୍ୱିଗୁଣ, ushort>, convertScaleData_<ଦ୍ୱିଗୁଣ, short>,
      convertScaleData_<ଦ୍ୱିଗୁଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, convertScaleData_<ଦ୍ୱିଗୁଣ, ଦଶମିକ>,
      convertScaleData_<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ଫେରନ୍ତୁ func;
}

ଗଣନା { HASH_SIZE0 = 8 };

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ copyElem(ଅଚଳ uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    ପାଇଁ( i = 0; i + ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) <= elemSize; i += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) )
        *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(to + i) = *(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(from + i);
    ପାଇଁ( ; i < elemSize; i++ )
        to[i] = from[i];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isZeroElem(ଅଚଳ uchar* data, size_t elemSize)
{
    size_t i;
    ପାଇଁ( i = 0; i + ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) <= elemSize; i += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) )
        ଯଦି( *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(data + i) != 0 )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ( ; i < elemSize; i++ )
        ଯଦି( data[i] != 0 )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

SparseMat::Hdr::Hdr( ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type )
{
    refcount = 1;

    dims = _dims;
    ମୂଲ୍ୟ_Offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)alignSize(ଆକାର(SparseMat::Node) - MAX_DIM*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) +
                                 dims*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), CV_ELEM_SIZE1(_type));
    ଗଣ୍ଠି_Size = alignSize(ମୂଲ୍ୟ_Offset +
        CV_ELEM_SIZE(_type), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(size_t));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    ପାଇଁ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

ଶୂନ୍ୟ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ଯଦି defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(ଗଣ୍ଠି_Size);
#ଯଦି defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    ଗଣ୍ଠି_ଗଣନା = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ଅଚଳ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ଅଚଳ SparseMat& m)
{
    ଯଦି( ଏହି != &m )
    {
        ଯଦି( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

SparseMat& SparseMat::operator=(ଅଚଳ Mat& m)
{
    ଫେରନ୍ତୁ (*ଏହି = SparseMat(m));
}

ଶୂନ୍ୟ SparseMat::assignTo(SparseMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    ଯଦି( _type < 0 )
        m = *ଏହି;
    ଅନ୍ୟଥା
        convertTo(m, _type);
}

ଶୂନ୍ୟ SparseMat::addref()
{
    ଯଦି( hdr )
        CV_XADD(&hdr->refcount, 1);
}

ଶୂନ୍ୟ SparseMat::release()
{
    ଯଦି( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0) ଅଚଳ
{
    ଫେରନ୍ତୁ (size_t)i0;
}

size_t SparseMat::hash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1) ଅଚଳ
{
    ଫେରନ୍ତୁ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2) ଅଚଳ
{
    ଫେରନ୍ତୁ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx) ଅଚଳ
{
    size_t h = (unsigned)idx[0];
    ଯଦି( !hdr )
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = hdr->dims;
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ଫେରନ୍ତୁ h;
}


SparseMat::SparseMat(ଅଚଳ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ଅଚଳ uchar* dptr = m.ptr();

    ପାଇଁ(;;)
    {
        ପାଇଁ( i = 0; i < lastSize; i++, dptr += esz )
        {
            ଯଦି( isZeroElem(dptr, esz) )
                ଜାରି;
            idx[d-1] = i;
            uchar* to = new_ଗଣ୍ଠି(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        ପାଇଁ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ଯଦି( ++idx[i] < m.size[i] )
                ଭାଙ୍ଗ;
            idx[i] = 0;
        }
        ଯଦି( i < 0 )
            ଭାଙ୍ଗ;
    }
}

ଶୂନ୍ୟ SparseMat::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ଯଦି( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ( i = 0; i < d; i++ )
            ଯଦି( _sizes[i] != hdr->size[i] )
                ଭାଙ୍ଗ;
        ଯଦି( i == d )
        {
            clear();
            ଫେରନ୍ତୁ;
        }
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _sizes_backup[CV_MAX_DIM]; // #5991
    ଯଦି (hdr && _sizes == hdr->size)
    {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = ନୂତନ Hdr(d, _sizes, _type);
}

ଶୂନ୍ୟ SparseMat::copyTo( SparseMat& m ) ଅଚଳ
{
    ଯଦି( hdr == m.hdr )
        ଫେରନ୍ତୁ;
    ଯଦି( !hdr )
    {
        m.release();
        ଫେରନ୍ତୁ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ପାଇଁ( size_t i = 0; i < N; i++, ++from )
    {
        ଅଚଳ Node* n = from.ଗଣ୍ଠି();
        uchar* to = m.new_ଗଣ୍ଠି(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

ଶୂନ୍ୟ SparseMat::copyTo( Mat& m ) ଅଚଳ
{
    CV_Assert( hdr );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ପାଇଁ( size_t i = 0; i < N; i++, ++from )
    {
        ଅଚଳ Node* n = from.ଗଣ୍ଠି();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


ଶୂନ୍ୟ SparseMat::convertTo( SparseMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rtype, ଦ୍ୱିଗୁଣ alpha ) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    ଯଦି( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ଯଦି( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ଫେରନ୍ତୁ;
    }

    CV_Assert(hdr != 0);
    ଯଦି( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ଯଦି( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        ପାଇଁ( size_t i = 0; i < N; i++, ++from )
        {
            ଅଚଳ Node* n = from.ଗଣ୍ଠି();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ଗଣ୍ଠି(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ଅନ୍ୟଥା
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        ପାଇଁ( size_t i = 0; i < N; i++, ++from )
        {
            ଅଚଳ Node* n = from.ଗଣ୍ଠି();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ଗଣ୍ଠି(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


ଶୂନ୍ୟ SparseMat::convertTo( Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rtype, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta ) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    ଯଦି( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ଯଦି( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        ପାଇଁ( size_t i = 0; i < N; i++, ++from )
        {
            ଅଚଳ Node* n = from.ଗଣ୍ଠି();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ଅନ୍ୟଥା
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        ପାଇଁ( size_t i = 0; i < N; i++, ++from )
        {
            ଅଚଳ Node* n = from.ଗଣ୍ଠି();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

ଶୂନ୍ୟ SparseMat::clear()
{
    ଯଦି( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ତାର୍କିକ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h && elem->idx[0] == i0 )
            ଫେରନ୍ତୁ &value<uchar>(elem);
        nidx = elem->next;
    }

    ଯଦି( createMissing )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[] = { i0 };
        ଫେରନ୍ତୁ new_ଗଣ୍ଠି( idx, h );
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

uchar* SparseMat::ptr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1, ତାର୍କିକ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ଫେରନ୍ତୁ &value<uchar>(elem);
        nidx = elem->next;
    }

    ଯଦି( createMissing )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[] = { i0, i1 };
        ଫେରନ୍ତୁ new_ଗଣ୍ଠି( idx, h );
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

uchar* SparseMat::ptr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2, ତାର୍କିକ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ଫେରନ୍ତୁ &value<uchar>(elem);
        nidx = elem->next;
    }

    ଯଦି( createMissing )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[] = { i0, i1, i2 };
        ଫେରନ୍ତୁ new_ଗଣ୍ଠି( idx, h );
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

uchar* SparseMat::ptr(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx, ତାର୍କିକ createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h )
        {
            ପାଇଁ( i = 0; i < d; i++ )
                ଯଦି( elem->idx[i] != idx[i] )
                    ଭାଙ୍ଗ;
            ଯଦି( i == d )
                ଫେରନ୍ତୁ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ଫେରନ୍ତୁ createMissing ? new_ଗଣ୍ଠି(idx, h) : ଖାଲି;
}

ଶୂନ୍ୟ SparseMat::erase(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ଭାଙ୍ଗ;
        previdx = nidx;
        nidx = elem->next;
    }

    ଯଦି( nidx )
        remove_ଗଣ୍ଠି(hidx, nidx, previdx);
}

ଶୂନ୍ୟ SparseMat::erase(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ଭାଙ୍ଗ;
        previdx = nidx;
        nidx = elem->next;
    }

    ଯଦି( nidx )
        remove_ଗଣ୍ଠି(hidx, nidx, previdx);
}

ଶୂନ୍ୟ SparseMat::erase(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ଯେତେବେଳେ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ଯଦି( elem->hashval == h )
        {
            ପାଇଁ( i = 0; i < d; i++ )
                ଯଦି( elem->idx[i] != idx[i] )
                    ଭାଙ୍ଗ;
            ଯଦି( i == d )
                ଭାଙ୍ଗ;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ଯଦି( nidx )
        remove_ଗଣ୍ଠି(hidx, nidx, previdx);
}

ଶୂନ୍ୟ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ଯଦି((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((ଦ୍ୱିଗୁଣ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    ପାଇଁ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    ପାଇଁ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ଯେତେବେଳେ( nidx )
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

uchar* SparseMat::new_ଗଣ୍ଠି(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx, size_t hashval)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ଯଦି( ++hdr->ଗଣ୍ଠି_ଗଣନା > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ଯଦି( !hdr->freeList )
    {
        size_t i, nsz = hdr->ଗଣ୍ଠି_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        ପାଇଁ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = hdr->dims;
    ପାଇଁ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ଯଦି( esz == ଆକାର(ଦଶମିକ) )
        *((ଦଶମିକ*)p) = 0.f;
    ଅନ୍ୟଥା ଯଦି( esz == ଆକାର(ଦ୍ୱିଗୁଣ) )
        *((ଦ୍ୱିଗୁଣ*)p) = 0.;
    ଅନ୍ୟଥା
        memset(p, 0, esz);

    ଫେରନ୍ତୁ p;
}


ଶୂନ୍ୟ SparseMat::remove_ଗଣ୍ଠି(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ଗଣ୍ଠି(nidx);
    ଯଦି( previdx )
    {
        Node* prev = ଗଣ୍ଠି(previdx);
        prev->next = n->next;
    }
    ଅନ୍ୟଥା
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ଗଣ୍ଠି_ଗଣନା;
}

//
// Operations
//
ଦ୍ୱିଗୁଣ norm( ଅଚଳ SparseMat& src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ଦ୍ୱିଗୁଣ result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ଯଦି( type == CV_32F )
    {
        ଯଦି( normType == NORM_INF )
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((ଦ୍ୱିଗୁଣ)it.value<ଦଶମିକ>()));
            }
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L1 )
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ଦଶମିକ>());
            }
        ଅନ୍ୟଥା
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ଦ୍ୱିଗୁଣ v = it.value<ଦଶମିକ>();
                result += v*v;
            }
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_64F )
    {
        ଯଦି( normType == NORM_INF )
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<ଦ୍ୱିଗୁଣ>()));
            }
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L1 )
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ଦ୍ୱିଗୁଣ>());
            }
        ଅନ୍ୟଥା
            ପାଇଁ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ଦ୍ୱିଗୁଣ v = it.value<ଦ୍ୱିଗୁଣ>();
                result += v*v;
            }
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ଯଦି( normType == NORM_L2 )
        result = std::sqrt(result);
    ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ minMaxLoc( ଅଚଳ SparseMat& src, ଦ୍ୱିଗୁଣ* _minval, ଦ୍ୱିଗୁଣ* _maxval, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _minidx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *minidx = 0, *maxidx = 0;

    ଯଦି( type == CV_32F )
    {
        ଦଶମିକ minval = FLT_MAX, maxval = -FLT_MAX;
        ପାଇଁ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ଦଶମିକ v = it.value<ଦଶମିକ>();
            ଯଦି( v < minval )
            {
                minval = v;
                minidx = it.ଗଣ୍ଠି()->idx;
            }
            ଯଦି( v > maxval )
            {
                maxval = v;
                maxidx = it.ଗଣ୍ଠି()->idx;
            }
        }
        ଯଦି( _minval )
            *_minval = minval;
        ଯଦି( _maxval )
            *_maxval = maxval;
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_64F )
    {
        ଦ୍ୱିଗୁଣ minval = DBL_MAX, maxval = -DBL_MAX;
        ପାଇଁ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ଦ୍ୱିଗୁଣ v = it.value<ଦ୍ୱିଗୁଣ>();
            ଯଦି( v < minval )
            {
                minval = v;
                minidx = it.ଗଣ୍ଠି()->idx;
            }
            ଯଦି( v > maxval )
            {
                maxval = v;
                maxidx = it.ଗଣ୍ଠି()->idx;
            }
        }
        ଯଦି( _minval )
            *_minval = minval;
        ଯଦି( _maxval )
            *_maxval = maxval;
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ଯଦି( _minidx && minidx )
        ପାଇଁ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ଯଦି( _maxidx && maxidx )
        ପାଇଁ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


ଶୂନ୍ୟ normalize( ଅଚଳ SparseMat& src, SparseMat& dst, ଦ୍ୱିଗୁଣ a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା norm_type )
{
    CV_INSTRUMENT_REGION();

    ଦ୍ୱିଗୁଣ scale = 1;
    ଯଦି( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
