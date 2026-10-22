// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

प्रतिरूपम्<प्रकारनाम T1, प्रकारनाम T2> शून्य
convert_दत्त(ध्रुव शून्य* _from, शून्य* गन्तव्यम्_2, पूर्णाङ्क cn)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* गन्तव्यम् = (T2*)गन्तव्यम्_2;
    यदि( cn == 1 )
        *गन्तव्यम् = saturate_cast<T2>(*from);
    अन्यथा
        प्रति( पूर्णाङ्क i = 0; i < cn; i++ )
            गन्तव्यम्[i] = saturate_cast<T2>(from[i]);
}

प्रतिरूपम्<प्रकारनाम T1, प्रकारनाम T2> शून्य
convert_Scale_दत्त(ध्रुव शून्य* _from, शून्य* गन्तव्यम्_2, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta)
{
    ध्रुव T1* from = (ध्रुव T1*)_from;
    T2* गन्तव्यम् = (T2*)गन्तव्यम्_2;
    यदि( cn == 1 )
        *गन्तव्यम् = saturate_cast<T2>(*from*alpha + beta);
    अन्यथा
        प्रति( पूर्णाङ्क i = 0; i < cn; i++ )
            गन्तव्यम्[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

प्रकारनिर्देश शून्य (*Convert_दत्त)(ध्रुव शून्य* from, शून्य* गन्तव्यम्, पूर्णाङ्क cn);
प्रकारनिर्देश शून्य (*Convert_Scale_दत्त)(ध्रुव शून्य* from, शून्य* गन्तव्यम्, पूर्णाङ्क cn, द्विगुण alpha, द्विगुण beta);

स्थिर Convert_दत्त getConvertElem(पूर्णाङ्क fromType, पूर्णाङ्क गन्तव्यम्_Type)
{
    स्थिर Convert_दत्त tab[CV_गभीरता_MAX][CV_गभीरता_MAX] =
    {{ convert_दत्त<uchar, uchar>, convert_दत्त<uchar, schar>,
      convert_दत्त<uchar, ushort>, convert_दत्त<uchar, ह्रस्व>,
      convert_दत्त<uchar, पूर्णाङ्क>, convert_दत्त<uchar, भिन्न>,
      convert_दत्त<uchar, द्विगुण>, 0 },

    { convert_दत्त<schar, uchar>, convert_दत्त<schar, schar>,
      convert_दत्त<schar, ushort>, convert_दत्त<schar, ह्रस्व>,
      convert_दत्त<schar, पूर्णाङ्क>, convert_दत्त<schar, भिन्न>,
      convert_दत्त<schar, द्विगुण>, 0 },

    { convert_दत्त<ushort, uchar>, convert_दत्त<ushort, schar>,
      convert_दत्त<ushort, ushort>, convert_दत्त<ushort, ह्रस्व>,
      convert_दत्त<ushort, पूर्णाङ्क>, convert_दत्त<ushort, भिन्न>,
      convert_दत्त<ushort, द्विगुण>, 0 },

    { convert_दत्त<ह्रस्व, uchar>, convert_दत्त<ह्रस्व, schar>,
      convert_दत्त<ह्रस्व, ushort>, convert_दत्त<ह्रस्व, ह्रस्व>,
      convert_दत्त<ह्रस्व, पूर्णाङ्क>, convert_दत्त<ह्रस्व, भिन्न>,
      convert_दत्त<ह्रस्व, द्विगुण>, 0 },

    { convert_दत्त<पूर्णाङ्क, uchar>, convert_दत्त<पूर्णाङ्क, schar>,
      convert_दत्त<पूर्णाङ्क, ushort>, convert_दत्त<पूर्णाङ्क, ह्रस्व>,
      convert_दत्त<पूर्णाङ्क, पूर्णाङ्क>, convert_दत्त<पूर्णाङ्क, भिन्न>,
      convert_दत्त<पूर्णाङ्क, द्विगुण>, 0 },

    { convert_दत्त<भिन्न, uchar>, convert_दत्त<भिन्न, schar>,
      convert_दत्त<भिन्न, ushort>, convert_दत्त<भिन्न, ह्रस्व>,
      convert_दत्त<भिन्न, पूर्णाङ्क>, convert_दत्त<भिन्न, भिन्न>,
      convert_दत्त<भिन्न, द्विगुण>, 0 },

    { convert_दत्त<द्विगुण, uchar>, convert_दत्त<द्विगुण, schar>,
      convert_दत्त<द्विगुण, ushort>, convert_दत्त<द्विगुण, ह्रस्व>,
      convert_दत्त<द्विगुण, पूर्णाङ्क>, convert_दत्त<द्विगुण, भिन्न>,
      convert_दत्त<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_दत्त func = tab[CV_MAT_गभीरता(fromType)][CV_MAT_गभीरता(गन्तव्यम्_Type)];
    CV_Assert( func != 0 );
    प्रत्यागम func;
}

स्थिर Convert_Scale_दत्त getConvertScaleElem(पूर्णाङ्क fromType, पूर्णाङ्क गन्तव्यम्_Type)
{
    स्थिर Convert_Scale_दत्त tab[CV_गभीरता_MAX][CV_गभीरता_MAX] =
    {{ convert_Scale_दत्त<uchar, uchar>, convert_Scale_दत्त<uchar, schar>,
      convert_Scale_दत्त<uchar, ushort>, convert_Scale_दत्त<uchar, ह्रस्व>,
      convert_Scale_दत्त<uchar, पूर्णाङ्क>, convert_Scale_दत्त<uchar, भिन्न>,
      convert_Scale_दत्त<uchar, द्विगुण>, 0 },

    { convert_Scale_दत्त<schar, uchar>, convert_Scale_दत्त<schar, schar>,
      convert_Scale_दत्त<schar, ushort>, convert_Scale_दत्त<schar, ह्रस्व>,
      convert_Scale_दत्त<schar, पूर्णाङ्क>, convert_Scale_दत्त<schar, भिन्न>,
      convert_Scale_दत्त<schar, द्विगुण>, 0 },

    { convert_Scale_दत्त<ushort, uchar>, convert_Scale_दत्त<ushort, schar>,
      convert_Scale_दत्त<ushort, ushort>, convert_Scale_दत्त<ushort, ह्रस्व>,
      convert_Scale_दत्त<ushort, पूर्णाङ्क>, convert_Scale_दत्त<ushort, भिन्न>,
      convert_Scale_दत्त<ushort, द्विगुण>, 0 },

    { convert_Scale_दत्त<ह्रस्व, uchar>, convert_Scale_दत्त<ह्रस्व, schar>,
      convert_Scale_दत्त<ह्रस्व, ushort>, convert_Scale_दत्त<ह्रस्व, ह्रस्व>,
      convert_Scale_दत्त<ह्रस्व, पूर्णाङ्क>, convert_Scale_दत्त<ह्रस्व, भिन्न>,
      convert_Scale_दत्त<ह्रस्व, द्विगुण>, 0 },

    { convert_Scale_दत्त<पूर्णाङ्क, uchar>, convert_Scale_दत्त<पूर्णाङ्क, schar>,
      convert_Scale_दत्त<पूर्णाङ्क, ushort>, convert_Scale_दत्त<पूर्णाङ्क, ह्रस्व>,
      convert_Scale_दत्त<पूर्णाङ्क, पूर्णाङ्क>, convert_Scale_दत्त<पूर्णाङ्क, भिन्न>,
      convert_Scale_दत्त<पूर्णाङ्क, द्विगुण>, 0 },

    { convert_Scale_दत्त<भिन्न, uchar>, convert_Scale_दत्त<भिन्न, schar>,
      convert_Scale_दत्त<भिन्न, ushort>, convert_Scale_दत्त<भिन्न, ह्रस्व>,
      convert_Scale_दत्त<भिन्न, पूर्णाङ्क>, convert_Scale_दत्त<भिन्न, भिन्न>,
      convert_Scale_दत्त<भिन्न, द्विगुण>, 0 },

    { convert_Scale_दत्त<द्विगुण, uchar>, convert_Scale_दत्त<द्विगुण, schar>,
      convert_Scale_दत्त<द्विगुण, ushort>, convert_Scale_दत्त<द्विगुण, ह्रस्व>,
      convert_Scale_दत्त<द्विगुण, पूर्णाङ्क>, convert_Scale_दत्त<द्विगुण, भिन्न>,
      convert_Scale_दत्त<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_दत्त func = tab[CV_MAT_गभीरता(fromType)][CV_MAT_गभीरता(गन्तव्यम्_Type)];
    CV_Assert( func != 0 );
    प्रत्यागम func;
}

गणना { HASH_SIZE0 = 8 };

स्थिर अन्तःस्थ शून्य copyElem(ध्रुव uchar* from, uchar* गन्तव्यम्, size_t elem_आकार)
{
    size_t i;
    प्रति( i = 0; i + परिमाण(पूर्णाङ्क) <= elem_आकार; i += परिमाण(पूर्णाङ्क) )
        *(पूर्णाङ्क*)(गन्तव्यम् + i) = *(ध्रुव पूर्णाङ्क*)(from + i);
    प्रति( ; i < elem_आकार; i++ )
        गन्तव्यम्[i] = from[i];
}

स्थिर अन्तःस्थ तार्किक isZeroElem(ध्रुव uchar* दत्त, size_t elem_आकार)
{
    size_t i;
    प्रति( i = 0; i + परिमाण(पूर्णाङ्क) <= elem_आकार; i += परिमाण(पूर्णाङ्क) )
        यदि( *(पूर्णाङ्क*)(दत्त + i) != 0 )
            प्रत्यागम असत्य;
    प्रति( ; i < elem_आकार; i++ )
        यदि( दत्त[i] != 0 )
            प्रत्यागम असत्य;
    प्रत्यागम सत्य;
}

SparseMat::Hdr::Hdr( पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type )
{
    refcount = 1;

    dims = _dims;
    मानम्_विस्थापनम् = (पूर्णाङ्क)align_आकार(परिमाण(SparseMat::Node) - MAX_DIM*परिमाण(पूर्णाङ्क) +
                                 dims*परिमाण(पूर्णाङ्क), CV_ELEM_SIZE1(_type));
    ग्रन्थिः_आकार = align_आकार(मानम्_विस्थापनम् +
        CV_ELEM_आकार(_type), (पूर्णाङ्क)परिमाण(size_t));

    पूर्णाङ्क i;
    प्रति( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    प्रति( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

शून्य SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#यदि defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#निर्देशः GCC diagnostic push
#निर्देशः GCC diagnostic ignored "-Wstringop-overflow"
#यदि_अन्तः
    pool.resize(ग्रन्थिः_आकार);
#यदि defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#निर्देशः GCC diagnostic pop
#यदि_अन्तः
    ग्रन्थिः_गणनम् = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(ध्रुव SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::संक्रियकः = (ध्रुव SparseMat& m)
{
    यदि( एतत् != &m )
    {
        यदि( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    प्रत्यागम *एतत्;
}

SparseMat& SparseMat::संक्रियकः=(ध्रुव Mat& m)
{
    प्रत्यागम (*एतत् = SparseMat(m));
}

शून्य SparseMat::नियोजय_गन्तव्यम्(SparseMat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convert_गन्तव्यम्(m, _type);
}

शून्य SparseMat::addref()
{
    यदि( hdr )
        CV_XADD(&hdr->refcount, 1);
}

शून्य SparseMat::release()
{
    यदि( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        विलोपय hdr;
    hdr = 0;
}

size_t SparseMat::hash(पूर्णाङ्क i0) ध्रुव
{
    प्रत्यागम (size_t)i0;
}

size_t SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1) ध्रुव
{
    प्रत्यागम (size_t)(अचिह्नित)i0 * HASH_SCALE + (अचिह्नित)i1;
}

size_t SparseMat::hash(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2) ध्रुव
{
    प्रत्यागम ((size_t)(अचिह्नित)i0 * HASH_SCALE + (अचिह्नित)i1) * HASH_SCALE + (अचिह्नित)i2;
}

size_t SparseMat::hash(ध्रुव पूर्णाङ्क* idx) ध्रुव
{
    size_t h = (अचिह्नित)idx[0];
    यदि( !hdr )
        प्रत्यागम 0;
    पूर्णाङ्क d = hdr->dims;
    प्रति(पूर्णाङ्क i = 1; i < d; i++ )
        h = h * HASH_SCALE + (अचिह्नित)idx[i];
    प्रत्यागम h;
}


SparseMat::SparseMat(ध्रुव Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    पूर्णाङ्क i, idx[CV_MAX_DIM] = {0}, d = m.dims, चरमः_आकार = m.size[d - 1];
    size_t esz = m.elem_आकार();
    ध्रुव uchar* dptr = m.ptr();

    प्रति(;;)
    {
        प्रति( i = 0; i < चरमः_आकार; i++, dptr += esz )
        {
            यदि( isZeroElem(dptr, esz) )
                अनुवर्त;
            idx[d-1] = i;
            uchar* गन्तव्यम् = new_ग्रन्थिः(idx, hash(idx));
            copyElem( dptr, गन्तव्यम्, esz );
        }

        प्रति( i = d - 2; i >= 0; i-- )
        {
            dptr += m.चरणः[i] - m.size[i+1]*m.चरणः[i+1];
            यदि( ++idx[i] < m.size[i] )
                भङ्ग;
            idx[i] = 0;
        }
        यदि( i < 0 )
            भङ्ग;
    }
}

शून्य SparseMat::create(पूर्णाङ्क d, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    प्रति( पूर्णाङ्क i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    यदि( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        पूर्णाङ्क i;
        प्रति( i = 0; i < d; i++ )
            यदि( _sizes[i] != hdr->size[i] )
                भङ्ग;
        यदि( i == d )
        {
            clear();
            प्रत्यागम;
        }
    }
    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    यदि (hdr && _sizes == hdr->size)
    {
        प्रति(पूर्णाङ्क i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = नूतन Hdr(d, _sizes, _type);
}

शून्य SparseMat::copy_गन्तव्यम्( SparseMat& m ) ध्रुव
{
    यदि( hdr == m.hdr )
        प्रत्यागम;
    यदि( !hdr )
    {
        m.release();
        प्रत्यागम;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_आकार();

    प्रति( size_t i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.ग्रन्थिः();
        uchar* गन्तव्यम् = m.new_ग्रन्थिः(n->idx, n->hashval);
        copyElem( from.ptr, गन्तव्यम्, esz );
    }
}

शून्य SparseMat::copy_गन्तव्यम्( Mat& m ) ध्रुव
{
    CV_Assert( hdr );
    पूर्णाङ्क ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_आकार();

    प्रति( size_t i = 0; i < N; i++, ++from )
    {
        ध्रुव Node* n = from.ग्रन्थिः();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


शून्य SparseMat::convert_गन्तव्यम्( SparseMat& m, पूर्णाङ्क rtype, द्विगुण alpha ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    यदि( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    यदि( hdr == m.hdr && rtype != type()  )
    {
        SparseMat अस्थायी_2;
        convert_गन्तव्यम्(अस्थायी_2, rtype, alpha);
        m = अस्थायी_2;
        प्रत्यागम;
    }

    CV_Assert(hdr != 0);
    यदि( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    यदि( alpha == 1 )
    {
        Convert_दत्त cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        प्रति( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* गन्तव्यम् = hdr == m.hdr ? from.ptr : m.new_ग्रन्थिः(n->idx, n->hashval);
            cvtfunc( from.ptr, गन्तव्यम्, cn );
        }
    }
    अन्यथा
    {
        Convert_Scale_दत्त cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        प्रति( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* गन्तव्यम् = hdr == m.hdr ? from.ptr : m.new_ग्रन्थिः(n->idx, n->hashval);
            cvtfunc( from.ptr, गन्तव्यम्, cn, alpha, 0 );
        }
    }
}


शून्य SparseMat::convert_गन्तव्यम्( Mat& m, पूर्णाङ्क rtype, द्विगुण alpha, द्विगुण beta ) ध्रुव
{
    पूर्णाङ्क cn = channels();
    यदि( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    यदि( alpha == 1 && beta == 0 )
    {
        Convert_दत्त cvtfunc = getConvertElem(type(), rtype);
        प्रति( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* गन्तव्यम् = m.ptr(n->idx);
            cvtfunc( from.ptr, गन्तव्यम्, cn );
        }
    }
    अन्यथा
    {
        Convert_Scale_दत्त cvtfunc = getConvertScaleElem(type(), rtype);
        प्रति( size_t i = 0; i < N; i++, ++from )
        {
            ध्रुव Node* n = from.ग्रन्थिः();
            uchar* गन्तव्यम् = m.ptr(n->idx);
            cvtfunc( from.ptr, गन्तव्यम्, cn, alpha, beta );
        }
    }
}

शून्य SparseMat::clear()
{
    यदि( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0 };
        प्रत्यागम new_ग्रन्थिः( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1 };
        प्रत्यागम new_ग्रन्थिः( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            प्रत्यागम &value<uchar>(elem);
        nidx = elem->next;
    }

    यदि( createMissing )
    {
        पूर्णाङ्क idx[] = { i0, i1, i2 };
        प्रत्यागम new_ग्रन्थिः( idx, h );
    }
    प्रत्यागम रिक्त;
}

uchar* SparseMat::ptr(ध्रुव पूर्णाङ्क* idx, तार्किक createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h )
        {
            प्रति( i = 0; i < d; i++ )
                यदि( elem->idx[i] != idx[i] )
                    भङ्ग;
            यदि( i == d )
                प्रत्यागम &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    प्रत्यागम createMissing ? new_ग्रन्थिः(idx, h) : रिक्त;
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::erase(पूर्णाङ्क i0, पूर्णाङ्क i1, पूर्णाङ्क i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            भङ्ग;
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::erase(ध्रुव पूर्णाङ्क* idx, size_t* hashval)
{
    CV_Assert( hdr );
    पूर्णाङ्क i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    यावत्( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        यदि( elem->hashval == h )
        {
            प्रति( i = 0; i < d; i++ )
                यदि( elem->idx[i] != idx[i] )
                    भङ्ग;
            यदि( i == d )
                भङ्ग;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    यदि( nidx )
        remove_ग्रन्थिः(hidx, nidx, previdx);
}

शून्य SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    यदि((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::लघुगणकः((द्विगुण)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    प्रति( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    प्रति( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        यावत्( nidx )
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

uchar* SparseMat::new_ग्रन्थिः(ध्रुव पूर्णाङ्क* idx, size_t hashval)
{
    ध्रुव पूर्णाङ्क HASH_MAX_FILL_गुणकः=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    यदि( ++hdr->ग्रन्थिः_गणनम् > hsize*HASH_MAX_FILL_गुणकः )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    यदि( !hdr->freeList )
    {
        size_t i, nsz = hdr->ग्रन्थिः_आकार, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        प्रति( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    पूर्णाङ्क i, d = hdr->dims;
    प्रति( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_आकार();
    uchar* p = &value<uchar>(elem);
    यदि( esz == परिमाण(भिन्न) )
        *((भिन्न*)p) = 0.f;
    अन्यथा यदि( esz == परिमाण(द्विगुण) )
        *((द्विगुण*)p) = 0.;
    अन्यथा
        स्मृतिं_पूरय(p, 0, esz);

    प्रत्यागम p;
}


शून्य SparseMat::remove_ग्रन्थिः(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ग्रन्थिः(nidx);
    यदि( previdx )
    {
        Node* पूर्व = ग्रन्थिः(previdx);
        पूर्व->next = n->next;
    }
    अन्यथा
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ग्रन्थिः_गणनम्;
}

//
// Operations
//
द्विगुण norm( ध्रुव SparseMat& src, पूर्णाङ्क normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_आवरणम्;
    पूर्णाङ्क type = src.type();
    द्विगुण परिणाम = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    यदि( type == CV_32F )
    {
        यदि( normType == NORM_INF )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम = std::max(परिणाम, std::निरपेक्षम्((द्विगुण)it.value<भिन्न>()));
            }
        अन्यथा यदि( normType == NORM_L1 )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम += std::निरपेक्षम्(it.value<भिन्न>());
            }
        अन्यथा
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<भिन्न>();
                परिणाम += v*v;
            }
    }
    अन्यथा यदि( type == CV_64F )
    {
        यदि( normType == NORM_INF )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम = std::max(परिणाम, std::निरपेक्षम्(it.value<द्विगुण>()));
            }
        अन्यथा यदि( normType == NORM_L1 )
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                परिणाम += std::निरपेक्षम्(it.value<द्विगुण>());
            }
        अन्यथा
            प्रति( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<द्विगुण>();
                परिणाम += v*v;
            }
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    यदि( normType == NORM_L2 )
        परिणाम = std::वर्गमूलम्(परिणाम);
    प्रत्यागम परिणाम;
}

शून्य minMaxLoc( ध्रुव SparseMat& src, द्विगुण* _minval, द्विगुण* _maxval, पूर्णाङ्क* _minidx, पूर्णाङ्क* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    पूर्णाङ्क type = src.type();
    ध्रुव पूर्णाङ्क *minidx = 0, *maxidx = 0;

    यदि( type == CV_32F )
    {
        भिन्न minval = FLT_MAX, maxval = -FLT_MAX;
        प्रति( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            भिन्न v = it.value<भिन्न>();
            यदि( v < minval )
            {
                minval = v;
                minidx = it.ग्रन्थिः()->idx;
            }
            यदि( v > maxval )
            {
                maxval = v;
                maxidx = it.ग्रन्थिः()->idx;
            }
        }
        यदि( _minval )
            *_minval = minval;
        यदि( _maxval )
            *_maxval = maxval;
    }
    अन्यथा यदि( type == CV_64F )
    {
        द्विगुण minval = DBL_MAX, maxval = -DBL_MAX;
        प्रति( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            द्विगुण v = it.value<द्विगुण>();
            यदि( v < minval )
            {
                minval = v;
                minidx = it.ग्रन्थिः()->idx;
            }
            यदि( v > maxval )
            {
                maxval = v;
                maxidx = it.ग्रन्थिः()->idx;
            }
        }
        यदि( _minval )
            *_minval = minval;
        यदि( _maxval )
            *_maxval = maxval;
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    यदि( _minidx && minidx )
        प्रति( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    यदि( _maxidx && maxidx )
        प्रति( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


शून्य normalize( ध्रुव SparseMat& src, SparseMat& dst, द्विगुण a, पूर्णाङ्क norm_type )
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1;
    यदि( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_गन्तव्यम्( dst, -1, scale );
}

} // cv::
