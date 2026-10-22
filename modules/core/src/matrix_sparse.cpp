// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "opencv2/core/mat.hpp"

namespace cv {

ढाँचा<typename T1, typename T2> व्योम
convert_तथ्य(स्थिर व्योम* _from, व्योम* _to, पूर्णांक cn)
{
    स्थिर T1* from = (स्थिर T1*)_from;
    T2* to = (T2*)_to;
    अगर( cn == 1 )
        *to = saturate_cast<T2>(*from);
    अन्यथा
        क्रम( पूर्णांक i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

ढाँचा<typename T1, typename T2> व्योम
convert_Scale_तथ्य(स्थिर व्योम* _from, व्योम* _to, पूर्णांक cn, द्विगुन alpha, द्विगुन beta)
{
    स्थिर T1* from = (स्थिर T1*)_from;
    T2* to = (T2*)_to;
    अगर( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    अन्यथा
        क्रम( पूर्णांक i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

प्रकार व्योम (*Convert_तथ्य)(स्थिर व्योम* from, व्योम* to, पूर्णांक cn);
प्रकार व्योम (*Convert_Scale_तथ्य)(स्थिर व्योम* from, व्योम* to, पूर्णांक cn, द्विगुन alpha, द्विगुन beta);

अटल Convert_तथ्य getConvertElem(पूर्णांक fromType, पूर्णांक toType)
{
    अटल Convert_तथ्य tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convert_तथ्य<uchar, uchar>, convert_तथ्य<uchar, schar>,
      convert_तथ्य<uchar, ushort>, convert_तथ्य<uchar, लघु>,
      convert_तथ्य<uchar, पूर्णांक>, convert_तथ्य<uchar, भग्न>,
      convert_तथ्य<uchar, द्विगुन>, 0 },

    { convert_तथ्य<schar, uchar>, convert_तथ्य<schar, schar>,
      convert_तथ्य<schar, ushort>, convert_तथ्य<schar, लघु>,
      convert_तथ्य<schar, पूर्णांक>, convert_तथ्य<schar, भग्न>,
      convert_तथ्य<schar, द्विगुन>, 0 },

    { convert_तथ्य<ushort, uchar>, convert_तथ्य<ushort, schar>,
      convert_तथ्य<ushort, ushort>, convert_तथ्य<ushort, लघु>,
      convert_तथ्य<ushort, पूर्णांक>, convert_तथ्य<ushort, भग्न>,
      convert_तथ्य<ushort, द्विगुन>, 0 },

    { convert_तथ्य<लघु, uchar>, convert_तथ्य<लघु, schar>,
      convert_तथ्य<लघु, ushort>, convert_तथ्य<लघु, लघु>,
      convert_तथ्य<लघु, पूर्णांक>, convert_तथ्य<लघु, भग्न>,
      convert_तथ्य<लघु, द्विगुन>, 0 },

    { convert_तथ्य<पूर्णांक, uchar>, convert_तथ्य<पूर्णांक, schar>,
      convert_तथ्य<पूर्णांक, ushort>, convert_तथ्य<पूर्णांक, लघु>,
      convert_तथ्य<पूर्णांक, पूर्णांक>, convert_तथ्य<पूर्णांक, भग्न>,
      convert_तथ्य<पूर्णांक, द्विगुन>, 0 },

    { convert_तथ्य<भग्न, uchar>, convert_तथ्य<भग्न, schar>,
      convert_तथ्य<भग्न, ushort>, convert_तथ्य<भग्न, लघु>,
      convert_तथ्य<भग्न, पूर्णांक>, convert_तथ्य<भग्न, भग्न>,
      convert_तथ्य<भग्न, द्विगुन>, 0 },

    { convert_तथ्य<द्विगुन, uchar>, convert_तथ्य<द्विगुन, schar>,
      convert_तथ्य<द्विगुन, ushort>, convert_तथ्य<द्विगुन, लघु>,
      convert_तथ्य<द्विगुन, पूर्णांक>, convert_तथ्य<द्विगुन, भग्न>,
      convert_तथ्य<द्विगुन, द्विगुन>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_तथ्य func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    वापस func;
}

अटल Convert_Scale_तथ्य getConvertScaleElem(पूर्णांक fromType, पूर्णांक toType)
{
    अटल Convert_Scale_तथ्य tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convert_Scale_तथ्य<uchar, uchar>, convert_Scale_तथ्य<uchar, schar>,
      convert_Scale_तथ्य<uchar, ushort>, convert_Scale_तथ्य<uchar, लघु>,
      convert_Scale_तथ्य<uchar, पूर्णांक>, convert_Scale_तथ्य<uchar, भग्न>,
      convert_Scale_तथ्य<uchar, द्विगुन>, 0 },

    { convert_Scale_तथ्य<schar, uchar>, convert_Scale_तथ्य<schar, schar>,
      convert_Scale_तथ्य<schar, ushort>, convert_Scale_तथ्य<schar, लघु>,
      convert_Scale_तथ्य<schar, पूर्णांक>, convert_Scale_तथ्य<schar, भग्न>,
      convert_Scale_तथ्य<schar, द्विगुन>, 0 },

    { convert_Scale_तथ्य<ushort, uchar>, convert_Scale_तथ्य<ushort, schar>,
      convert_Scale_तथ्य<ushort, ushort>, convert_Scale_तथ्य<ushort, लघु>,
      convert_Scale_तथ्य<ushort, पूर्णांक>, convert_Scale_तथ्य<ushort, भग्न>,
      convert_Scale_तथ्य<ushort, द्विगुन>, 0 },

    { convert_Scale_तथ्य<लघु, uchar>, convert_Scale_तथ्य<लघु, schar>,
      convert_Scale_तथ्य<लघु, ushort>, convert_Scale_तथ्य<लघु, लघु>,
      convert_Scale_तथ्य<लघु, पूर्णांक>, convert_Scale_तथ्य<लघु, भग्न>,
      convert_Scale_तथ्य<लघु, द्विगुन>, 0 },

    { convert_Scale_तथ्य<पूर्णांक, uchar>, convert_Scale_तथ्य<पूर्णांक, schar>,
      convert_Scale_तथ्य<पूर्णांक, ushort>, convert_Scale_तथ्य<पूर्णांक, लघु>,
      convert_Scale_तथ्य<पूर्णांक, पूर्णांक>, convert_Scale_तथ्य<पूर्णांक, भग्न>,
      convert_Scale_तथ्य<पूर्णांक, द्विगुन>, 0 },

    { convert_Scale_तथ्य<भग्न, uchar>, convert_Scale_तथ्य<भग्न, schar>,
      convert_Scale_तथ्य<भग्न, ushort>, convert_Scale_तथ्य<भग्न, लघु>,
      convert_Scale_तथ्य<भग्न, पूर्णांक>, convert_Scale_तथ्य<भग्न, भग्न>,
      convert_Scale_तथ्य<भग्न, द्विगुन>, 0 },

    { convert_Scale_तथ्य<द्विगुन, uchar>, convert_Scale_तथ्य<द्विगुन, schar>,
      convert_Scale_तथ्य<द्विगुन, ushort>, convert_Scale_तथ्य<द्विगुन, लघु>,
      convert_Scale_तथ्य<द्विगुन, पूर्णांक>, convert_Scale_तथ्य<द्विगुन, भग्न>,
      convert_Scale_तथ्य<द्विगुन, द्विगुन>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_तथ्य func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    वापस func;
}

क्रमागत { HASH_SIZE0 = 8 };

अटल अंतरभूत व्योम copyElem(स्थिर uchar* from, uchar* to, माप_प्रकार elem_आकार)
{
    माप_प्रकार i;
    क्रम( i = 0; i + माप(पूर्णांक) <= elem_आकार; i += माप(पूर्णांक) )
        *(पूर्णांक*)(to + i) = *(स्थिर पूर्णांक*)(from + i);
    क्रम( ; i < elem_आकार; i++ )
        to[i] = from[i];
}

अटल अंतरभूत bool isZeroElem(स्थिर uchar* तथ्य, माप_प्रकार elem_आकार)
{
    माप_प्रकार i;
    क्रम( i = 0; i + माप(पूर्णांक) <= elem_आकार; i += माप(पूर्णांक) )
        अगर( *(पूर्णांक*)(तथ्य + i) != 0 )
            वापस false;
    क्रम( ; i < elem_आकार; i++ )
        अगर( तथ्य[i] != 0 )
            वापस false;
    वापस true;
}

SparseMat::Hdr::Hdr( पूर्णांक _dims, स्थिर पूर्णांक* _sizes, पूर्णांक _type )
{
    refcount = 1;

    dims = _dims;
    मूल्य_Offset = (पूर्णांक)align_आकार(माप(SparseMat::Node) - MAX_DIM*माप(पूर्णांक) +
                                 dims*माप(पूर्णांक), CV_ELEM_SIZE1(_type));
    गाँठ_आकार = align_आकार(मूल्य_Offset +
        CV_ELEM_आकार(_type), (पूर्णांक)माप(माप_प्रकार));

    पूर्णांक i;
    क्रम( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    क्रम( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    सफा();
}

व्योम SparseMat::Hdr::सफा()
{
    hashtab.सफा();
    hashtab.resize(HASH_SIZE0);
    pool.सफा();
#अगर defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wstringop-overflow"
#पूर्ण_अगर
    pool.resize(गाँठ_आकार);
#अगर defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#आशय GCC diagnostic pop
#पूर्ण_अगर
    गाँठ_गिनती = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(पूर्णांक _dims, स्थिर पूर्णांक* _sizes, पूर्णांक _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(स्थिर SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::चालक = (स्थिर SparseMat& m)
{
    अगर( यह != &m )
    {
        अगर( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    वापस *यह;
}

SparseMat& SparseMat::चालक=(स्थिर Mat& m)
{
    वापस (*यह = SparseMat(m));
}

व्योम SparseMat::assignTo(SparseMat& m, पूर्णांक _type) स्थिर
{
    अगर( _type < 0 )
        m = *यह;
    अन्यथा
        convertTo(m, _type);
}

व्योम SparseMat::addref()
{
    अगर( hdr )
        CV_XADD(&hdr->refcount, 1);
}

व्योम SparseMat::release()
{
    अगर( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        मिटाओ hdr;
    hdr = 0;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0) स्थिर
{
    वापस (माप_प्रकार)i0;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0, पूर्णांक i1) स्थिर
{
    वापस (माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2) स्थिर
{
    वापस ((माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1) * HASH_SCALE + (अचिन्हित)i2;
}

माप_प्रकार SparseMat::hash(स्थिर पूर्णांक* idx) स्थिर
{
    माप_प्रकार h = (अचिन्हित)idx[0];
    अगर( !hdr )
        वापस 0;
    पूर्णांक d = hdr->dims;
    क्रम(पूर्णांक i = 1; i < d; i++ )
        h = h * HASH_SCALE + (अचिन्हित)idx[i];
    वापस h;
}


SparseMat::SparseMat(स्थिर Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    पूर्णांक i, idx[CV_MAX_DIM] = {0}, d = m.dims, last_आकार = m.size[d - 1];
    माप_प्रकार esz = m.elem_आकार();
    स्थिर uchar* dptr = m.ptr();

    क्रम(;;)
    {
        क्रम( i = 0; i < last_आकार; i++, dptr += esz )
        {
            अगर( isZeroElem(dptr, esz) )
                जारी;
            idx[d-1] = i;
            uchar* to = new_गाँठ(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        क्रम( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            अगर( ++idx[i] < m.size[i] )
                अवरोध;
            idx[i] = 0;
        }
        अगर( i < 0 )
            अवरोध;
    }
}

व्योम SparseMat::create(पूर्णांक d, स्थिर पूर्णांक* _sizes, पूर्णांक _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    क्रम( पूर्णांक i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    अगर( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        पूर्णांक i;
        क्रम( i = 0; i < d; i++ )
            अगर( _sizes[i] != hdr->size[i] )
                अवरोध;
        अगर( i == d )
        {
            सफा();
            वापस;
        }
    }
    पूर्णांक _sizes_backup[CV_MAX_DIM]; // #5991
    अगर (hdr && _sizes == hdr->size)
    {
        क्रम(पूर्णांक i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = नया Hdr(d, _sizes, _type);
}

व्योम SparseMat::copyTo( SparseMat& m ) स्थिर
{
    अगर( hdr == m.hdr )
        वापस;
    अगर( !hdr )
    {
        m.release();
        वापस;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount(), esz = elem_आकार();

    क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        स्थिर Node* n = from.गाँठ();
        uchar* to = m.new_गाँठ(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

व्योम SparseMat::copyTo( Mat& m ) स्थिर
{
    CV_Assert( hdr );
    पूर्णांक ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount(), esz = elem_आकार();

    क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        स्थिर Node* n = from.गाँठ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


व्योम SparseMat::convertTo( SparseMat& m, पूर्णांक rtype, द्विगुन alpha ) स्थिर
{
    पूर्णांक cn = channels();
    अगर( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    अगर( hdr == m.hdr && rtype != type()  )
    {
        SparseMat अस्थायी_2;
        convertTo(अस्थायी_2, rtype, alpha);
        m = अस्थायी_2;
        वापस;
    }

    CV_Assert(hdr != 0);
    अगर( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    अगर( alpha == 1 )
    {
        Convert_तथ्य cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिर Node* n = from.गाँठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाँठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        Convert_Scale_तथ्य cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिर Node* n = from.गाँठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाँठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


व्योम SparseMat::convertTo( Mat& m, पूर्णांक rtype, द्विगुन alpha, द्विगुन beta ) स्थिर
{
    पूर्णांक cn = channels();
    अगर( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    अगर( alpha == 1 && beta == 0 )
    {
        Convert_तथ्य cvtfunc = getConvertElem(type(), rtype);
        क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिर Node* n = from.गाँठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        Convert_Scale_तथ्य cvtfunc = getConvertScaleElem(type(), rtype);
        क्रम( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिर Node* n = from.गाँठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

व्योम SparseMat::सफा()
{
    अगर( hdr )
        hdr->सफा();
}

uchar* SparseMat::ptr(पूर्णांक i0, bool createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    माप_प्रकार h = hashval ? *hashval : hash(i0);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h && elem->idx[0] == i0 )
            वापस &value<uchar>(elem);
        nidx = elem->next;
    }

    अगर( createMissing )
    {
        पूर्णांक idx[] = { i0 };
        वापस new_गाँठ( idx, h );
    }
    वापस शून्य;
}

uchar* SparseMat::ptr(पूर्णांक i0, पूर्णांक i1, bool createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            वापस &value<uchar>(elem);
        nidx = elem->next;
    }

    अगर( createMissing )
    {
        पूर्णांक idx[] = { i0, i1 };
        वापस new_गाँठ( idx, h );
    }
    वापस शून्य;
}

uchar* SparseMat::ptr(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2, bool createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            वापस &value<uchar>(elem);
        nidx = elem->next;
    }

    अगर( createMissing )
    {
        पूर्णांक idx[] = { i0, i1, i2 };
        वापस new_गाँठ( idx, h );
    }
    वापस शून्य;
}

uchar* SparseMat::ptr(स्थिर पूर्णांक* idx, bool createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णांक i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h )
        {
            क्रम( i = 0; i < d; i++ )
                अगर( elem->idx[i] != idx[i] )
                    अवरोध;
            अगर( i == d )
                वापस &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    वापस createMissing ? new_गाँठ(idx, h) : शून्य;
}

व्योम SparseMat::erase(पूर्णांक i0, पूर्णांक i1, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            अवरोध;
        previdx = nidx;
        nidx = elem->next;
    }

    अगर( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

व्योम SparseMat::erase(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            अवरोध;
        previdx = nidx;
        nidx = elem->next;
    }

    अगर( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

व्योम SparseMat::erase(स्थिर पूर्णांक* idx, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णांक i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    जबतक( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        अगर( elem->hashval == h )
        {
            क्रम( i = 0; i < d; i++ )
                अगर( elem->idx[i] != idx[i] )
                    अवरोध;
            अगर( i == d )
                अवरोध;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    अगर( nidx )
        remove_गाँठ(hidx, nidx, previdx);
}

व्योम SparseMat::resizeHashTab(माप_प्रकार newsize)
{
    newsize = std::max(newsize, (माप_प्रकार)8);
    अगर((newsize & (newsize-1)) != 0)
        newsize = (माप_प्रकार)1 << cvCeil(std::log((द्विगुन)newsize)/CV_LOG2);

    माप_प्रकार hsize = hdr->hashtab.size();
    std::vector<माप_प्रकार> _newh(newsize);
    माप_प्रकार* newh = &_newh[0];
    क्रम( माप_प्रकार i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    क्रम( माप_प्रकार i = 0; i < hsize; i++ )
    {
        माप_प्रकार nidx = hdr->hashtab[i];
        जबतक( nidx )
        {
            Node* elem = (Node*)(pool + nidx);
            माप_प्रकार next = elem->next;
            माप_प्रकार newhidx = elem->hashval & (newsize - 1);
            elem->next = newh[newhidx];
            newh[newhidx] = nidx;
            nidx = next;
        }
    }
    hdr->hashtab = _newh;
}

uchar* SparseMat::new_गाँठ(स्थिर पूर्णांक* idx, माप_प्रकार hashval)
{
    स्थिर पूर्णांक HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    माप_प्रकार hsize = hdr->hashtab.size();
    अगर( ++hdr->गाँठ_गिनती > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (माप_प्रकार)8));
        hsize = hdr->hashtab.size();
    }

    अगर( !hdr->freeList )
    {
        माप_प्रकार i, nsz = hdr->गाँठ_आकार, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        क्रम( i = hdr->freeList; i < newpsize - nsz; i += nsz )
            ((Node*)(pool + i))->next = i + nsz;
        ((Node*)(pool + i))->next = 0;
    }
    माप_प्रकार nidx = hdr->freeList;
    Node* elem = (Node*)&hdr->pool[nidx];
    hdr->freeList = elem->next;
    elem->hashval = hashval;
    माप_प्रकार hidx = hashval & (hsize - 1);
    elem->next = hdr->hashtab[hidx];
    hdr->hashtab[hidx] = nidx;

    पूर्णांक i, d = hdr->dims;
    क्रम( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    माप_प्रकार esz = elem_आकार();
    uchar* p = &value<uchar>(elem);
    अगर( esz == माप(भग्न) )
        *((भग्न*)p) = 0.f;
    अन्यथा अगर( esz == माप(द्विगुन) )
        *((द्विगुन*)p) = 0.;
    अन्यथा
        स_रखो(p, 0, esz);

    वापस p;
}


व्योम SparseMat::remove_गाँठ(माप_प्रकार hidx, माप_प्रकार nidx, माप_प्रकार previdx)
{
    Node* n = गाँठ(nidx);
    अगर( previdx )
    {
        Node* पूर्व = गाँठ(previdx);
        पूर्व->next = n->next;
    }
    अन्यथा
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->गाँठ_गिनती;
}

//
// Operations
//
द्विगुन norm( स्थिर SparseMat& src, पूर्णांक normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    माप_प्रकार i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    पूर्णांक type = src.type();
    द्विगुन परिणाम = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    अगर( type == CV_32F )
    {
        अगर( normType == NORM_INF )
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम = std::max(परिणाम, std::असल((द्विगुन)it.value<भग्न>()));
            }
        अन्यथा अगर( normType == NORM_L1 )
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम += std::असल(it.value<भग्न>());
            }
        अन्यथा
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुन v = it.value<भग्न>();
                परिणाम += v*v;
            }
    }
    अन्यथा अगर( type == CV_64F )
    {
        अगर( normType == NORM_INF )
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम = std::max(परिणाम, std::असल(it.value<द्विगुन>()));
            }
        अन्यथा अगर( normType == NORM_L1 )
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम += std::असल(it.value<द्विगुन>());
            }
        अन्यथा
            क्रम( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुन v = it.value<द्विगुन>();
                परिणाम += v*v;
            }
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    अगर( normType == NORM_L2 )
        परिणाम = std::वर्ग_मूल(परिणाम);
    वापस परिणाम;
}

व्योम minMaxLoc( स्थिर SparseMat& src, द्विगुन* _minval, द्विगुन* _maxval, पूर्णांक* _minidx, पूर्णांक* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    माप_प्रकार i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    पूर्णांक type = src.type();
    स्थिर पूर्णांक *minidx = 0, *maxidx = 0;

    अगर( type == CV_32F )
    {
        भग्न minval = भग्न_उच्च, maxval = -भग्न_उच्च;
        क्रम( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            भग्न v = it.value<भग्न>();
            अगर( v < minval )
            {
                minval = v;
                minidx = it.गाँठ()->idx;
            }
            अगर( v > maxval )
            {
                maxval = v;
                maxidx = it.गाँठ()->idx;
            }
        }
        अगर( _minval )
            *_minval = minval;
        अगर( _maxval )
            *_maxval = maxval;
    }
    अन्यथा अगर( type == CV_64F )
    {
        द्विगुन minval = द्विग_उच्च, maxval = -द्विग_उच्च;
        क्रम( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            द्विगुन v = it.value<द्विगुन>();
            अगर( v < minval )
            {
                minval = v;
                minidx = it.गाँठ()->idx;
            }
            अगर( v > maxval )
            {
                maxval = v;
                maxidx = it.गाँठ()->idx;
            }
        }
        अगर( _minval )
            *_minval = minval;
        अगर( _maxval )
            *_maxval = maxval;
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    अगर( _minidx && minidx )
        क्रम( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    अगर( _maxidx && maxidx )
        क्रम( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


व्योम normalize( स्थिर SparseMat& src, SparseMat& dst, द्विगुन a, पूर्णांक norm_type )
{
    CV_INSTRUMENT_REGION();

    द्विगुन scale = 1;
    अगर( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > द्विग_अंतर ? a/scale : 0.;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
