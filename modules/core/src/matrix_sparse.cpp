// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

template<typename T1, typename T2> ಶೂನ್ಯ
convertData_(ಧ್ರುವ ಶೂನ್ಯ* _from, ಶೂನ್ಯ* _to, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಧ್ರುವ T1* from = (ಧ್ರುವ T1*)_from;
    T2* to = (T2*)_to;
    ಯದಿ( cn == 1 )
        *to = saturate_cast<T2>(*from);
    ಅನ್ಯಥಾ
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> ಶೂನ್ಯ
convertScaleData_(ಧ್ರುವ ಶೂನ್ಯ* _from, ಶೂನ್ಯ* _to, ಪೂರ್ಣಾಙ್ಕ cn, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    ಧ್ರುವ T1* from = (ಧ್ರುವ T1*)_from;
    T2* to = (T2*)_to;
    ಯದಿ( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    ಅನ್ಯಥಾ
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ConvertData)(ಧ್ರುವ ಶೂನ್ಯ* from, ಶೂನ್ಯ* to, ಪೂರ್ಣಾಙ್ಕ cn);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ConvertScaleData)(ಧ್ರುವ ಶೂನ್ಯ* from, ಶೂನ್ಯ* to, ಪೂರ್ಣಾಙ್ಕ cn, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta);

ಸ್ಥಿರ ConvertData getConvertElem(ಪೂರ್ಣಾಙ್ಕ fromType, ಪೂರ್ಣಾಙ್ಕ toType)
{
    ಸ್ಥಿರ ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, ಪೂರ್ಣಾಙ್ಕ>, convertData_<uchar, ಭಿನ್ನ>,
      convertData_<uchar, ದ್ವಿಗುಣ>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, ಪೂರ್ಣಾಙ್ಕ>, convertData_<schar, ಭಿನ್ನ>,
      convertData_<schar, ದ್ವಿಗುಣ>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, ಪೂರ್ಣಾಙ್ಕ>, convertData_<ushort, ಭಿನ್ನ>,
      convertData_<ushort, ದ್ವಿಗುಣ>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, ಪೂರ್ಣಾಙ್ಕ>, convertData_<short, ಭಿನ್ನ>,
      convertData_<short, ದ್ವಿಗುಣ>, 0 },

    { convertData_<ಪೂರ್ಣಾಙ್ಕ, uchar>, convertData_<ಪೂರ್ಣಾಙ್ಕ, schar>,
      convertData_<ಪೂರ್ಣಾಙ್ಕ, ushort>, convertData_<ಪೂರ್ಣಾಙ್ಕ, short>,
      convertData_<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, convertData_<ಪೂರ್ಣಾಙ್ಕ, ಭಿನ್ನ>,
      convertData_<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ>, 0 },

    { convertData_<ಭಿನ್ನ, uchar>, convertData_<ಭಿನ್ನ, schar>,
      convertData_<ಭಿನ್ನ, ushort>, convertData_<ಭಿನ್ನ, short>,
      convertData_<ಭಿನ್ನ, ಪೂರ್ಣಾಙ್ಕ>, convertData_<ಭಿನ್ನ, ಭಿನ್ನ>,
      convertData_<ಭಿನ್ನ, ದ್ವಿಗುಣ>, 0 },

    { convertData_<ದ್ವಿಗುಣ, uchar>, convertData_<ದ್ವಿಗುಣ, schar>,
      convertData_<ದ್ವಿಗುಣ, ushort>, convertData_<ದ್ವಿಗುಣ, short>,
      convertData_<ದ್ವಿಗುಣ, ಪೂರ್ಣಾಙ್ಕ>, convertData_<ದ್ವಿಗುಣ, ಭಿನ್ನ>,
      convertData_<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ಪ್ರತ್ಯಾಗಮ func;
}

ಸ್ಥಿರ ConvertScaleData getConvertScaleElem(ಪೂರ್ಣಾಙ್ಕ fromType, ಪೂರ್ಣಾಙ್ಕ toType)
{
    ಸ್ಥಿರ ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<uchar, ಭಿನ್ನ>,
      convertScaleData_<uchar, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<schar, ಭಿನ್ನ>,
      convertScaleData_<schar, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<ushort, ಭಿನ್ನ>,
      convertScaleData_<ushort, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<short, ಭಿನ್ನ>,
      convertScaleData_<short, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, uchar>, convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, schar>,
      convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, ushort>, convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, short>,
      convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, ಭಿನ್ನ>,
      convertScaleData_<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<ಭಿನ್ನ, uchar>, convertScaleData_<ಭಿನ್ನ, schar>,
      convertScaleData_<ಭಿನ್ನ, ushort>, convertScaleData_<ಭಿನ್ನ, short>,
      convertScaleData_<ಭಿನ್ನ, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<ಭಿನ್ನ, ಭಿನ್ನ>,
      convertScaleData_<ಭಿನ್ನ, ದ್ವಿಗುಣ>, 0 },

    { convertScaleData_<ದ್ವಿಗುಣ, uchar>, convertScaleData_<ದ್ವಿಗುಣ, schar>,
      convertScaleData_<ದ್ವಿಗುಣ, ushort>, convertScaleData_<ದ್ವಿಗುಣ, short>,
      convertScaleData_<ದ್ವಿಗುಣ, ಪೂರ್ಣಾಙ್ಕ>, convertScaleData_<ದ್ವಿಗುಣ, ಭಿನ್ನ>,
      convertScaleData_<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    ಪ್ರತ್ಯಾಗಮ func;
}

ಗಣನಾ { HASH_SIZE0 = 8 };

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ copyElem(ಧ್ರುವ uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    ಪ್ರತಿ( i = 0; i + ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) <= elemSize; i += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) )
        *(ಪೂರ್ಣಾಙ್ಕ*)(to + i) = *(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)(from + i);
    ಪ್ರತಿ( ; i < elemSize; i++ )
        to[i] = from[i];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isZeroElem(ಧ್ರುವ uchar* data, size_t elemSize)
{
    size_t i;
    ಪ್ರತಿ( i = 0; i + ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) <= elemSize; i += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) )
        ಯದಿ( *(ಪೂರ್ಣಾಙ್ಕ*)(data + i) != 0 )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ( ; i < elemSize; i++ )
        ಯದಿ( data[i] != 0 )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

SparseMat::Hdr::Hdr( ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type )
{
    refcount = 1;

    dims = _dims;
    ಮೌಲ್ಯ_Offset = (ಪೂರ್ಣಾಙ್ಕ)alignSize(ಪರಿಮಾಣ(SparseMat::Node) - MAX_DIM*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) +
                                 dims*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), CV_ELEM_SIZE1(_type));
    ಗಂಟು_Size = alignSize(ಮೌಲ್ಯ_Offset +
        CV_ELEM_SIZE(_type), (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(size_t));

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    ಪ್ರತಿ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

ಶೂನ್ಯ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ಯದಿ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(ಗಂಟು_Size);
#ಯದಿ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    ಗಂಟು_ಎಣಿಕೆ = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ಧ್ರುವ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (ಧ್ರುವ SparseMat& m)
{
    ಯದಿ( ಏತತ್ != &m )
    {
        ಯದಿ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

SparseMat& SparseMat::operator=(ಧ್ರುವ Mat& m)
{
    ಪ್ರತ್ಯಾಗಮ (*ಏತತ್ = SparseMat(m));
}

ಶೂನ್ಯ SparseMat::assignTo(SparseMat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    ಯದಿ( _type < 0 )
        m = *ಏತತ್;
    ಅನ್ಯಥಾ
        convertTo(m, _type);
}

ಶೂನ್ಯ SparseMat::addref()
{
    ಯದಿ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

ಶೂನ್ಯ SparseMat::release()
{
    ಯದಿ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಙ್ಕ i0) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ (size_t)i0;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1, ಪೂರ್ಣಾಙ್ಕ i2) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* idx) ಧ್ರುವ
{
    size_t h = (unsigned)idx[0];
    ಯದಿ( !hdr )
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ d = hdr->dims;
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    ಪ್ರತ್ಯಾಗಮ h;
}


SparseMat::SparseMat(ಧ್ರುವ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ಪೂರ್ಣಾಙ್ಕ i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    ಧ್ರುವ uchar* dptr = m.ptr();

    ಪ್ರತಿ(;;)
    {
        ಪ್ರತಿ( i = 0; i < lastSize; i++, dptr += esz )
        {
            ಯದಿ( isZeroElem(dptr, esz) )
                ಅನುವರ್ತ;
            idx[d-1] = i;
            uchar* to = new_ಗಂಟು(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        ಪ್ರತಿ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            ಯದಿ( ++idx[i] < m.size[i] )
                ಭಙ್ಗ;
            idx[i] = 0;
        }
        ಯದಿ( i < 0 )
            ಭಙ್ಗ;
    }
}

ಶೂನ್ಯ SparseMat::create(ಪೂರ್ಣಾಙ್ಕ d, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ಯದಿ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ( i = 0; i < d; i++ )
            ಯದಿ( _sizes[i] != hdr->size[i] )
                ಭಙ್ಗ;
        ಯದಿ( i == d )
        {
            clear();
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
    ಪೂರ್ಣಾಙ್ಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಯದಿ (hdr && _sizes == hdr->size)
    {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = ನೂತನ Hdr(d, _sizes, _type);
}

ಶೂನ್ಯ SparseMat::copyTo( SparseMat& m ) ಧ್ರುವ
{
    ಯದಿ( hdr == m.hdr )
        ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ( !hdr )
    {
        m.release();
        ಪ್ರತ್ಯಾಗಮ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
    {
        ಧ್ರುವ Node* n = from.ಗಂಟು();
        uchar* to = m.new_ಗಂಟು(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

ಶೂನ್ಯ SparseMat::copyTo( Mat& m ) ಧ್ರುವ
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಙ್ಕ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
    {
        ಧ್ರುವ Node* n = from.ಗಂಟು();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


ಶೂನ್ಯ SparseMat::convertTo( SparseMat& m, ಪೂರ್ಣಾಙ್ಕ rtype, ದ್ವಿಗುಣ alpha ) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    ಯದಿ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ಯದಿ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_Assert(hdr != 0);
    ಯದಿ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ಯದಿ( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಧ್ರುವ Node* n = from.ಗಂಟು();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ಗಂಟು(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ಅನ್ಯಥಾ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಧ್ರುವ Node* n = from.ಗಂಟು();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ಗಂಟು(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


ಶೂನ್ಯ SparseMat::convertTo( Mat& m, ಪೂರ್ಣಾಙ್ಕ rtype, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta ) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    ಯದಿ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ಯದಿ( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಧ್ರುವ Node* n = from.ಗಂಟು();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    ಅನ್ಯಥಾ
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        ಪ್ರತಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಧ್ರುವ Node* n = from.ಗಂಟು();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

ಶೂನ್ಯ SparseMat::clear()
{
    ಯದಿ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಙ್ಕ i0, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h && elem->idx[0] == i0 )
            ಪ್ರತ್ಯಾಗಮ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಯದಿ( createMissing )
    {
        ಪೂರ್ಣಾಙ್ಕ idx[] = { i0 };
        ಪ್ರತ್ಯಾಗಮ new_ಗಂಟು( idx, h );
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ಪ್ರತ್ಯಾಗಮ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಯದಿ( createMissing )
    {
        ಪೂರ್ಣಾಙ್ಕ idx[] = { i0, i1 };
        ಪ್ರತ್ಯಾಗಮ new_ಗಂಟು( idx, h );
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1, ಪೂರ್ಣಾಙ್ಕ i2, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ಪ್ರತ್ಯಾಗಮ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಯದಿ( createMissing )
    {
        ಪೂರ್ಣಾಙ್ಕ idx[] = { i0, i1, i2 };
        ಪ್ರತ್ಯಾಗಮ new_ಗಂಟು( idx, h );
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

uchar* SparseMat::ptr(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* idx, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಙ್ಕ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h )
        {
            ಪ್ರತಿ( i = 0; i < d; i++ )
                ಯದಿ( elem->idx[i] != idx[i] )
                    ಭಙ್ಗ;
            ಯದಿ( i == d )
                ಪ್ರತ್ಯಾಗಮ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ಪ್ರತ್ಯಾಗಮ createMissing ? new_ಗಂಟು(idx, h) : ರಿಕ್ತ;
}

ಶೂನ್ಯ SparseMat::erase(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ಭಙ್ಗ;
        previdx = nidx;
        nidx = elem->next;
    }

    ಯದಿ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::erase(ಪೂರ್ಣಾಙ್ಕ i0, ಪೂರ್ಣಾಙ್ಕ i1, ಪೂರ್ಣಾಙ್ಕ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ಭಙ್ಗ;
        previdx = nidx;
        nidx = elem->next;
    }

    ಯದಿ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::erase(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಙ್ಕ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಯಾವತ್( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಯದಿ( elem->hashval == h )
        {
            ಪ್ರತಿ( i = 0; i < d; i++ )
                ಯದಿ( elem->idx[i] != idx[i] )
                    ಭಙ್ಗ;
            ಯದಿ( i == d )
                ಭಙ್ಗ;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ಯದಿ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ಯದಿ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((ದ್ವಿಗುಣ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    ಪ್ರತಿ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    ಪ್ರತಿ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ಯಾವತ್( nidx )
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

uchar* SparseMat::new_ಗಂಟು(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* idx, size_t hashval)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ಯದಿ( ++hdr->ಗಂಟು_ಎಣಿಕೆ > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ಯದಿ( !hdr->freeList )
    {
        size_t i, nsz = hdr->ಗಂಟು_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        ಪ್ರತಿ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ಪೂರ್ಣಾಙ್ಕ i, d = hdr->dims;
    ಪ್ರತಿ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    ಯದಿ( esz == ಪರಿಮಾಣ(ಭಿನ್ನ) )
        *((ಭಿನ್ನ*)p) = 0.f;
    ಅನ್ಯಥಾ ಯದಿ( esz == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
        *((ದ್ವಿಗುಣ*)p) = 0.;
    ಅನ್ಯಥಾ
        memset(p, 0, esz);

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ SparseMat::remove_ಗಂಟು(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ಗಂಟು(nidx);
    ಯದಿ( previdx )
    {
        Node* prev = ಗಂಟು(previdx);
        prev->next = n->next;
    }
    ಅನ್ಯಥಾ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ಗಂಟು_ಎಣಿಕೆ;
}

//
// Operations
//
ದ್ವಿಗುಣ norm( ಧ್ರುವ SparseMat& src, ಪೂರ್ಣಾಙ್ಕ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ದ್ವಿಗುಣ result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ಯದಿ( type == CV_32F )
    {
        ಯದಿ( normType == NORM_INF )
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((ದ್ವಿಗುಣ)it.value<ಭಿನ್ನ>()));
            }
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L1 )
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ಭಿನ್ನ>());
            }
        ಅನ್ಯಥಾ
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ದ್ವಿಗುಣ v = it.value<ಭಿನ್ನ>();
                result += v*v;
            }
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64F )
    {
        ಯದಿ( normType == NORM_INF )
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<ದ್ವಿಗುಣ>()));
            }
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L1 )
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<ದ್ವಿಗುಣ>());
            }
        ಅನ್ಯಥಾ
            ಪ್ರತಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ದ್ವಿಗುಣ v = it.value<ದ್ವಿಗುಣ>();
                result += v*v;
            }
    }
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ಯದಿ( normType == NORM_L2 )
        result = std::sqrt(result);
    ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ minMaxLoc( ಧ್ರುವ SparseMat& src, ದ್ವಿಗುಣ* _minval, ದ್ವಿಗುಣ* _maxval, ಪೂರ್ಣಾಙ್ಕ* _minidx, ಪೂರ್ಣಾಙ್ಕ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *minidx = 0, *maxidx = 0;

    ಯದಿ( type == CV_32F )
    {
        ಭಿನ್ನ minval = FLT_MAX, maxval = -FLT_MAX;
        ಪ್ರತಿ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ಭಿನ್ನ v = it.value<ಭಿನ್ನ>();
            ಯದಿ( v < minval )
            {
                minval = v;
                minidx = it.ಗಂಟು()->idx;
            }
            ಯದಿ( v > maxval )
            {
                maxval = v;
                maxidx = it.ಗಂಟು()->idx;
            }
        }
        ಯದಿ( _minval )
            *_minval = minval;
        ಯದಿ( _maxval )
            *_maxval = maxval;
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64F )
    {
        ದ್ವಿಗುಣ minval = DBL_MAX, maxval = -DBL_MAX;
        ಪ್ರತಿ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ದ್ವಿಗುಣ v = it.value<ದ್ವಿಗುಣ>();
            ಯದಿ( v < minval )
            {
                minval = v;
                minidx = it.ಗಂಟು()->idx;
            }
            ಯದಿ( v > maxval )
            {
                maxval = v;
                maxidx = it.ಗಂಟು()->idx;
            }
        }
        ಯದಿ( _minval )
            *_minval = minval;
        ಯದಿ( _maxval )
            *_maxval = maxval;
    }
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ಯದಿ( _minidx && minidx )
        ಪ್ರತಿ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ಯದಿ( _maxidx && maxidx )
        ಪ್ರತಿ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


ಶೂನ್ಯ normalize( ಧ್ರುವ SparseMat& src, SparseMat& dst, ದ್ವಿಗುಣ a, ಪೂರ್ಣಾಙ್ಕ norm_type )
{
    CV_INSTRUMENT_REGION();

    ದ್ವಿಗುಣ scale = 1;
    ಯದಿ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
