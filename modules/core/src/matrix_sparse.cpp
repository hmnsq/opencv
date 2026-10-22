// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2> ಶೂನ್ಯ
convert_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _from, ಶೂನ್ಯ* ಗಮ್ಯ_2, ಪೂರ್ಣಾಂಕ cn)
{
    ಸ್ಥಿರಾಂಕ T1* from = (ಸ್ಥಿರಾಂಕ T1*)_from;
    T2* ಗಮ್ಯ = (T2*)ಗಮ್ಯ_2;
    ಆದರೆ( cn == 1 )
        *ಗಮ್ಯ = saturate_cast<T2>(*from);
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < cn; i++ )
            ಗಮ್ಯ[i] = saturate_cast<T2>(from[i]);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2> ಶೂನ್ಯ
convert_Scale_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _from, ಶೂನ್ಯ* ಗಮ್ಯ_2, ಪೂರ್ಣಾಂಕ cn, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    ಸ್ಥಿರಾಂಕ T1* from = (ಸ್ಥಿರಾಂಕ T1*)_from;
    T2* ಗಮ್ಯ = (T2*)ಗಮ್ಯ_2;
    ಆದರೆ( cn == 1 )
        *ಗಮ್ಯ = saturate_cast<T2>(*from*alpha + beta);
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < cn; i++ )
            ಗಮ್ಯ[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*Convert_ದತ್ತಾಂಶ)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* from, ಶೂನ್ಯ* ಗಮ್ಯ, ಪೂರ್ಣಾಂಕ cn);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*Convert_Scale_ದತ್ತಾಂಶ)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* from, ಶೂನ್ಯ* ಗಮ್ಯ, ಪೂರ್ಣಾಂಕ cn, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta);

ಸ್ಥಿರ Convert_ದತ್ತಾಂಶ getConvertElem(ಪೂರ್ಣಾಂಕ fromType, ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_Type)
{
    ಸ್ಥಿರ Convert_ದತ್ತಾಂಶ tab[CV_ಆಳ_MAX][CV_ಆಳ_MAX] =
    {{ convert_ದತ್ತಾಂಶ<uchar, uchar>, convert_ದತ್ತಾಂಶ<uchar, schar>,
      convert_ದತ್ತಾಂಶ<uchar, ushort>, convert_ದತ್ತಾಂಶ<uchar, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<uchar, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<uchar, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<uchar, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<schar, uchar>, convert_ದತ್ತಾಂಶ<schar, schar>,
      convert_ದತ್ತಾಂಶ<schar, ushort>, convert_ದತ್ತಾಂಶ<schar, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<schar, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<schar, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<schar, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<ushort, uchar>, convert_ದತ್ತಾಂಶ<ushort, schar>,
      convert_ದತ್ತಾಂಶ<ushort, ushort>, convert_ದತ್ತಾಂಶ<ushort, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<ushort, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<ushort, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<ushort, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, uchar>, convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, schar>,
      convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ushort>, convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, uchar>, convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, schar>,
      convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ushort>, convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, uchar>, convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, schar>,
      convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ushort>, convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ದ್ವಿಗುಣ>, 0 },

    { convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, uchar>, convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, schar>,
      convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ushort>, convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ಗಿಡ್ಡ>,
      convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ>, convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ದಶಮಾಂಶ>,
      convert_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_ದತ್ತಾಂಶ func = tab[CV_MAT_ಆಳ(fromType)][CV_MAT_ಆಳ(ಗಮ್ಯ_Type)];
    CV_Assert( func != 0 );
    ಹಿಂತಿರುಗಿ func;
}

ಸ್ಥಿರ Convert_Scale_ದತ್ತಾಂಶ getConvertScaleElem(ಪೂರ್ಣಾಂಕ fromType, ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_Type)
{
    ಸ್ಥಿರ Convert_Scale_ದತ್ತಾಂಶ tab[CV_ಆಳ_MAX][CV_ಆಳ_MAX] =
    {{ convert_Scale_ದತ್ತಾಂಶ<uchar, uchar>, convert_Scale_ದತ್ತಾಂಶ<uchar, schar>,
      convert_Scale_ದತ್ತಾಂಶ<uchar, ushort>, convert_Scale_ದತ್ತಾಂಶ<uchar, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<uchar, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<uchar, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<uchar, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<schar, uchar>, convert_Scale_ದತ್ತಾಂಶ<schar, schar>,
      convert_Scale_ದತ್ತಾಂಶ<schar, ushort>, convert_Scale_ದತ್ತಾಂಶ<schar, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<schar, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<schar, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<schar, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<ushort, uchar>, convert_Scale_ದತ್ತಾಂಶ<ushort, schar>,
      convert_Scale_ದತ್ತಾಂಶ<ushort, ushort>, convert_Scale_ದತ್ತಾಂಶ<ushort, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<ushort, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<ushort, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<ushort, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, uchar>, convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, schar>,
      convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ushort>, convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<ಗಿಡ್ಡ, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, uchar>, convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, schar>,
      convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ushort>, convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, uchar>, convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, schar>,
      convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ushort>, convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<ದಶಮಾಂಶ, ದ್ವಿಗುಣ>, 0 },

    { convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, uchar>, convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, schar>,
      convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ushort>, convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ಗಿಡ್ಡ>,
      convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ>, convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ದಶಮಾಂಶ>,
      convert_Scale_ದತ್ತಾಂಶ<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_ದತ್ತಾಂಶ func = tab[CV_MAT_ಆಳ(fromType)][CV_MAT_ಆಳ(ಗಮ್ಯ_Type)];
    CV_Assert( func != 0 );
    ಹಿಂತಿರುಗಿ func;
}

ಎಣಿಕೆ { HASH_SIZE0 = 8 };

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ copyElem(ಸ್ಥಿರಾಂಕ uchar* from, uchar* ಗಮ್ಯ, size_t elem_ಗಾತ್ರ)
{
    size_t i;
    ಗಾಗಿ( i = 0; i + ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) <= elem_ಗಾತ್ರ; i += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) )
        *(ಪೂರ್ಣಾಂಕ*)(ಗಮ್ಯ + i) = *(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)(from + i);
    ಗಾಗಿ( ; i < elem_ಗಾತ್ರ; i++ )
        ಗಮ್ಯ[i] = from[i];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isZeroElem(ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ, size_t elem_ಗಾತ್ರ)
{
    size_t i;
    ಗಾಗಿ( i = 0; i + ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) <= elem_ಗಾತ್ರ; i += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) )
        ಆದರೆ( *(ಪೂರ್ಣಾಂಕ*)(ದತ್ತಾಂಶ + i) != 0 )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ( ; i < elem_ಗಾತ್ರ; i++ )
        ಆದರೆ( ದತ್ತಾಂಶ[i] != 0 )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

SparseMat::Hdr::Hdr( ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type )
{
    refcount = 1;

    dims = _dims;
    ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ = (ಪೂರ್ಣಾಂಕ)align_ಗಾತ್ರ(ಗಾತ್ರ(SparseMat::Node) - MAX_DIM*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) +
                                 dims*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), CV_ELEM_SIZE1(_type));
    ಗಂಟು_ಗಾತ್ರ = align_ಗಾತ್ರ(ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ +
        CV_ELEM_ಗಾತ್ರ(_type), (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(size_t));

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    ಗಾಗಿ( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

ಶೂನ್ಯ SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ಆದರೆ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ಸೂಚನೆ GCC diagnostic push
#ಸೂಚನೆ GCC diagnostic ignored "-Wstringop-overflow"
#ಆದರೆ_ಅಂತ್ಯ
    pool.resize(ಗಂಟು_ಗಾತ್ರ);
#ಆದರೆ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#ಸೂಚನೆ GCC diagnostic pop
#ಆದರೆ_ಅಂತ್ಯ
    ಗಂಟು_ಎಣಿಕೆ = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ಸ್ಥಿರಾಂಕ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ SparseMat& m)
{
    ಆದರೆ( ಇದು != &m )
    {
        ಆದರೆ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

SparseMat& SparseMat::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Mat& m)
{
    ಹಿಂತಿರುಗಿ (*ಇದು = SparseMat(m));
}

ಶೂನ್ಯ SparseMat::ನಿಯೋಜಿಸು_ಗಮ್ಯ(SparseMat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( _type < 0 )
        m = *ಇದು;
    ಇಲ್ಲದಿದ್ದರೆ
        convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ SparseMat::addref()
{
    ಆದರೆ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

ಶೂನ್ಯ SparseMat::release()
{
    ಆದರೆ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        ಅಳಿಸು hdr;
    hdr = 0;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಂಕ i0) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ (size_t)i0;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ (size_t)(ಚಿಹ್ನೆರಹಿತ)i0 * HASH_SCALE + (ಚಿಹ್ನೆರಹಿತ)i1;
}

size_t SparseMat::hash(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1, ಪೂರ್ಣಾಂಕ i2) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ((size_t)(ಚಿಹ್ನೆರಹಿತ)i0 * HASH_SCALE + (ಚಿಹ್ನೆರಹಿತ)i1) * HASH_SCALE + (ಚಿಹ್ನೆರಹಿತ)i2;
}

size_t SparseMat::hash(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* idx) ಸ್ಥಿರಾಂಕ
{
    size_t h = (ಚಿಹ್ನೆರಹಿತ)idx[0];
    ಆದರೆ( !hdr )
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ d = hdr->dims;
    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 1; i < d; i++ )
        h = h * HASH_SCALE + (ಚಿಹ್ನೆರಹಿತ)idx[i];
    ಹಿಂತಿರುಗಿ h;
}


SparseMat::SparseMat(ಸ್ಥಿರಾಂಕ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    ಪೂರ್ಣಾಂಕ i, idx[CV_MAX_DIM] = {0}, d = m.dims, ಕೊನೆಯ_ಗಾತ್ರ = m.size[d - 1];
    size_t esz = m.elem_ಗಾತ್ರ();
    ಸ್ಥಿರಾಂಕ uchar* dptr = m.ptr();

    ಗಾಗಿ(;;)
    {
        ಗಾಗಿ( i = 0; i < ಕೊನೆಯ_ಗಾತ್ರ; i++, dptr += esz )
        {
            ಆದರೆ( isZeroElem(dptr, esz) )
                ಮುಂದುವರಿ;
            idx[d-1] = i;
            uchar* ಗಮ್ಯ = new_ಗಂಟು(idx, hash(idx));
            copyElem( dptr, ಗಮ್ಯ, esz );
        }

        ಗಾಗಿ( i = d - 2; i >= 0; i-- )
        {
            dptr += m.ಹೆಜ್ಜೆ[i] - m.size[i+1]*m.ಹೆಜ್ಜೆ[i+1];
            ಆದರೆ( ++idx[i] < m.size[i] )
                ನಿಲ್ಲಿಸು;
            idx[i] = 0;
        }
        ಆದರೆ( i < 0 )
            ನಿಲ್ಲಿಸು;
    }
}

ಶೂನ್ಯ SparseMat::create(ಪೂರ್ಣಾಂಕ d, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ಆದರೆ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ( i = 0; i < d; i++ )
            ಆದರೆ( _sizes[i] != hdr->size[i] )
                ನಿಲ್ಲಿಸು;
        ಆದರೆ( i == d )
        {
            clear();
            ಹಿಂತಿರುಗಿ;
        }
    }
    ಪೂರ್ಣಾಂಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಆದರೆ (hdr && _sizes == hdr->size)
    {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = ಹೊಸ Hdr(d, _sizes, _type);
}

ಶೂನ್ಯ SparseMat::copy_ಗಮ್ಯ( SparseMat& m ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( hdr == m.hdr )
        ಹಿಂತಿರುಗಿ;
    ಆದರೆ( !hdr )
    {
        m.release();
        ಹಿಂತಿರುಗಿ;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_ಗಾತ್ರ();

    ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
    {
        ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
        uchar* ಗಮ್ಯ = m.new_ಗಂಟು(n->idx, n->hashval);
        copyElem( from.ptr, ಗಮ್ಯ, esz );
    }
}

ಶೂನ್ಯ SparseMat::copy_ಗಮ್ಯ( Mat& m ) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಂಕ ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_ಗಾತ್ರ();

    ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
    {
        ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


ಶೂನ್ಯ SparseMat::convert_ಗಮ್ಯ( SparseMat& m, ಪೂರ್ಣಾಂಕ rtype, ದ್ವಿಗುಣ alpha ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ cn = channels();
    ಆದರೆ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ಆದರೆ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat ತಾತ್ಕಾಲಿಕ_2;
        convert_ಗಮ್ಯ(ತಾತ್ಕಾಲಿಕ_2, rtype, alpha);
        m = ತಾತ್ಕಾಲಿಕ_2;
        ಹಿಂತಿರುಗಿ;
    }

    CV_Assert(hdr != 0);
    ಆದರೆ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ಆದರೆ( alpha == 1 )
    {
        Convert_ದತ್ತಾಂಶ cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
            uchar* ಗಮ್ಯ = hdr == m.hdr ? from.ptr : m.new_ಗಂಟು(n->idx, n->hashval);
            cvtfunc( from.ptr, ಗಮ್ಯ, cn );
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Convert_Scale_ದತ್ತಾಂಶ cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
            uchar* ಗಮ್ಯ = hdr == m.hdr ? from.ptr : m.new_ಗಂಟು(n->idx, n->hashval);
            cvtfunc( from.ptr, ಗಮ್ಯ, cn, alpha, 0 );
        }
    }
}


ಶೂನ್ಯ SparseMat::convert_ಗಮ್ಯ( Mat& m, ಪೂರ್ಣಾಂಕ rtype, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ cn = channels();
    ಆದರೆ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ಆದರೆ( alpha == 1 && beta == 0 )
    {
        Convert_ದತ್ತಾಂಶ cvtfunc = getConvertElem(type(), rtype);
        ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
            uchar* ಗಮ್ಯ = m.ptr(n->idx);
            cvtfunc( from.ptr, ಗಮ್ಯ, cn );
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Convert_Scale_ದತ್ತಾಂಶ cvtfunc = getConvertScaleElem(type(), rtype);
        ಗಾಗಿ( size_t i = 0; i < N; i++, ++from )
        {
            ಸ್ಥಿರಾಂಕ Node* n = from.ಗಂಟು();
            uchar* ಗಮ್ಯ = m.ptr(n->idx);
            cvtfunc( from.ptr, ಗಮ್ಯ, cn, alpha, beta );
        }
    }
}

ಶೂನ್ಯ SparseMat::clear()
{
    ಆದರೆ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಂಕ i0, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h && elem->idx[0] == i0 )
            ಹಿಂತಿರುಗಿ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಆದರೆ( createMissing )
    {
        ಪೂರ್ಣಾಂಕ idx[] = { i0 };
        ಹಿಂತಿರುಗಿ new_ಗಂಟು( idx, h );
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ಹಿಂತಿರುಗಿ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಆದರೆ( createMissing )
    {
        ಪೂರ್ಣಾಂಕ idx[] = { i0, i1 };
        ಹಿಂತಿರುಗಿ new_ಗಂಟು( idx, h );
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

uchar* SparseMat::ptr(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1, ಪೂರ್ಣಾಂಕ i2, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ಹಿಂತಿರುಗಿ &value<uchar>(elem);
        nidx = elem->next;
    }

    ಆದರೆ( createMissing )
    {
        ಪೂರ್ಣಾಂಕ idx[] = { i0, i1, i2 };
        ಹಿಂತಿರುಗಿ new_ಗಂಟು( idx, h );
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

uchar* SparseMat::ptr(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* idx, ತಾರ್ಕಿಕ createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಂಕ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h )
        {
            ಗಾಗಿ( i = 0; i < d; i++ )
                ಆದರೆ( elem->idx[i] != idx[i] )
                    ನಿಲ್ಲಿಸು;
            ಆದರೆ( i == d )
                ಹಿಂತಿರುಗಿ &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    ಹಿಂತಿರುಗಿ createMissing ? new_ಗಂಟು(idx, h) : ಖಾಲಿ;
}

ಶೂನ್ಯ SparseMat::erase(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ನಿಲ್ಲಿಸು;
        previdx = nidx;
        nidx = elem->next;
    }

    ಆದರೆ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::erase(ಪೂರ್ಣಾಂಕ i0, ಪೂರ್ಣಾಂಕ i1, ಪೂರ್ಣಾಂಕ i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ನಿಲ್ಲಿಸು;
        previdx = nidx;
        nidx = elem->next;
    }

    ಆದರೆ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::erase(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* idx, size_t* hashval)
{
    CV_Assert( hdr );
    ಪೂರ್ಣಾಂಕ i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    ಆಗ( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ಆದರೆ( elem->hashval == h )
        {
            ಗಾಗಿ( i = 0; i < d; i++ )
                ಆದರೆ( elem->idx[i] != idx[i] )
                    ನಿಲ್ಲಿಸು;
            ಆದರೆ( i == d )
                ನಿಲ್ಲಿಸು;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ಆದರೆ( nidx )
        remove_ಗಂಟು(hidx, nidx, previdx);
}

ಶೂನ್ಯ SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ಆದರೆ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::ಲಘುಗಣಕ((ದ್ವಿಗುಣ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    ಗಾಗಿ( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    ಗಾಗಿ( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        ಆಗ( nidx )
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

uchar* SparseMat::new_ಗಂಟು(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* idx, size_t hashval)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ HASH_MAX_FILL_ಗುಣಕ=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ಆದರೆ( ++hdr->ಗಂಟು_ಎಣಿಕೆ > hsize*HASH_MAX_FILL_ಗುಣಕ )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ಆದರೆ( !hdr->freeList )
    {
        size_t i, nsz = hdr->ಗಂಟು_ಗಾತ್ರ, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        ಗಾಗಿ( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    ಪೂರ್ಣಾಂಕ i, d = hdr->dims;
    ಗಾಗಿ( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_ಗಾತ್ರ();
    uchar* p = &value<uchar>(elem);
    ಆದರೆ( esz == ಗಾತ್ರ(ದಶಮಾಂಶ) )
        *((ದಶಮಾಂಶ*)p) = 0.f;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( esz == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
        *((ದ್ವಿಗುಣ*)p) = 0.;
    ಇಲ್ಲದಿದ್ದರೆ
        memset(p, 0, esz);

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ SparseMat::remove_ಗಂಟು(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ಗಂಟು(nidx);
    ಆದರೆ( previdx )
    {
        Node* prev = ಗಂಟು(previdx);
        prev->next = n->next;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ಗಂಟು_ಎಣಿಕೆ;
}

//
// Operations
//
ದ್ವಿಗುಣ norm( ಸ್ಥಿರಾಂಕ SparseMat& src, ಪೂರ್ಣಾಂಕ normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_ಮುಖವಾಡ;
    ಪೂರ್ಣಾಂಕ type = src.type();
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ಆದರೆ( type == CV_32F )
    {
        ಆದರೆ( normType == NORM_INF )
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, std::ನಿರಪೇಕ್ಷ((ದ್ವಿಗುಣ)it.value<ದಶಮಾಂಶ>()));
            }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L1 )
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ಫಲಿತಾಂಶ += std::ನಿರಪೇಕ್ಷ(it.value<ದಶಮಾಂಶ>());
            }
        ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ದ್ವಿಗುಣ v = it.value<ದಶಮಾಂಶ>();
                ಫಲಿತಾಂಶ += v*v;
            }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64F )
    {
        ಆದರೆ( normType == NORM_INF )
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, std::ನಿರಪೇಕ್ಷ(it.value<ದ್ವಿಗುಣ>()));
            }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L1 )
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ಫಲಿತಾಂಶ += std::ನಿರಪೇಕ್ಷ(it.value<ದ್ವಿಗುಣ>());
            }
        ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ದ್ವಿಗುಣ v = it.value<ದ್ವಿಗುಣ>();
                ಫಲಿತಾಂಶ += v*v;
            }
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ಆದರೆ( normType == NORM_L2 )
        ಫಲಿತಾಂಶ = std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ minMaxLoc( ಸ್ಥಿರಾಂಕ SparseMat& src, ದ್ವಿಗುಣ* _minval, ದ್ವಿಗುಣ* _maxval, ಪೂರ್ಣಾಂಕ* _minidx, ಪೂರ್ಣಾಂಕ* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    ಪೂರ್ಣಾಂಕ type = src.type();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *minidx = 0, *maxidx = 0;

    ಆದರೆ( type == CV_32F )
    {
        ದಶಮಾಂಶ minval = FLT_MAX, maxval = -FLT_MAX;
        ಗಾಗಿ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ದಶಮಾಂಶ v = it.value<ದಶಮಾಂಶ>();
            ಆದರೆ( v < minval )
            {
                minval = v;
                minidx = it.ಗಂಟು()->idx;
            }
            ಆದರೆ( v > maxval )
            {
                maxval = v;
                maxidx = it.ಗಂಟು()->idx;
            }
        }
        ಆದರೆ( _minval )
            *_minval = minval;
        ಆದರೆ( _maxval )
            *_maxval = maxval;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64F )
    {
        ದ್ವಿಗುಣ minval = DBL_MAX, maxval = -DBL_MAX;
        ಗಾಗಿ( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ದ್ವಿಗುಣ v = it.value<ದ್ವಿಗುಣ>();
            ಆದರೆ( v < minval )
            {
                minval = v;
                minidx = it.ಗಂಟು()->idx;
            }
            ಆದರೆ( v > maxval )
            {
                maxval = v;
                maxidx = it.ಗಂಟು()->idx;
            }
        }
        ಆದರೆ( _minval )
            *_minval = minval;
        ಆದರೆ( _maxval )
            *_maxval = maxval;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ಆದರೆ( _minidx && minidx )
        ಗಾಗಿ( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ಆದರೆ( _maxidx && maxidx )
        ಗಾಗಿ( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


ಶೂನ್ಯ normalize( ಸ್ಥಿರಾಂಕ SparseMat& src, SparseMat& dst, ದ್ವಿಗುಣ a, ಪೂರ್ಣಾಂಕ norm_type )
{
    CV_INSTRUMENT_REGION();

    ದ್ವಿಗುಣ scale = 1;
    ಆದರೆ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_ಗಮ್ಯ( dst, -1, scale );
}

} // cv::
