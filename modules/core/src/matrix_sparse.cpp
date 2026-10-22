// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

ढाँचा<typename T1, typename T2> शून्य
convertData_(स्थिरांक शून्य* _from, शून्य* _to, पूर्णांक cn)
{
    स्थिरांक T1* from = (स्थिरांक T1*)_from;
    T2* to = (T2*)_to;
    जर( cn == 1 )
        *to = saturate_cast<T2>(*from);
    अन्यथा
        साठी( पूर्णांक i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

ढाँचा<typename T1, typename T2> शून्य
convertScaleData_(स्थिरांक शून्य* _from, शून्य* _to, पूर्णांक cn, द्विगुण alpha, द्विगुण beta)
{
    स्थिरांक T1* from = (स्थिरांक T1*)_from;
    T2* to = (T2*)_to;
    जर( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    अन्यथा
        साठी( पूर्णांक i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

प्रकारव्याख्या शून्य (*ConvertData)(स्थिरांक शून्य* from, शून्य* to, पूर्णांक cn);
प्रकारव्याख्या शून्य (*ConvertScaleData)(स्थिरांक शून्य* from, शून्य* to, पूर्णांक cn, द्विगुण alpha, द्विगुण beta);

स्थिर ConvertData getConvertElem(पूर्णांक fromType, पूर्णांक toType)
{
    स्थिर ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, लघु>,
      convertData_<uchar, पूर्णांक>, convertData_<uchar, दशांश>,
      convertData_<uchar, द्विगुण>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, लघु>,
      convertData_<schar, पूर्णांक>, convertData_<schar, दशांश>,
      convertData_<schar, द्विगुण>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, लघु>,
      convertData_<ushort, पूर्णांक>, convertData_<ushort, दशांश>,
      convertData_<ushort, द्विगुण>, 0 },

    { convertData_<लघु, uchar>, convertData_<लघु, schar>,
      convertData_<लघु, ushort>, convertData_<लघु, लघु>,
      convertData_<लघु, पूर्णांक>, convertData_<लघु, दशांश>,
      convertData_<लघु, द्विगुण>, 0 },

    { convertData_<पूर्णांक, uchar>, convertData_<पूर्णांक, schar>,
      convertData_<पूर्णांक, ushort>, convertData_<पूर्णांक, लघु>,
      convertData_<पूर्णांक, पूर्णांक>, convertData_<पूर्णांक, दशांश>,
      convertData_<पूर्णांक, द्विगुण>, 0 },

    { convertData_<दशांश, uchar>, convertData_<दशांश, schar>,
      convertData_<दशांश, ushort>, convertData_<दशांश, लघु>,
      convertData_<दशांश, पूर्णांक>, convertData_<दशांश, दशांश>,
      convertData_<दशांश, द्विगुण>, 0 },

    { convertData_<द्विगुण, uchar>, convertData_<द्विगुण, schar>,
      convertData_<द्विगुण, ushort>, convertData_<द्विगुण, लघु>,
      convertData_<द्विगुण, पूर्णांक>, convertData_<द्विगुण, दशांश>,
      convertData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    परत func;
}

स्थिर ConvertScaleData getConvertScaleElem(पूर्णांक fromType, पूर्णांक toType)
{
    स्थिर ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, लघु>,
      convertScaleData_<uchar, पूर्णांक>, convertScaleData_<uchar, दशांश>,
      convertScaleData_<uchar, द्विगुण>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, लघु>,
      convertScaleData_<schar, पूर्णांक>, convertScaleData_<schar, दशांश>,
      convertScaleData_<schar, द्विगुण>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, लघु>,
      convertScaleData_<ushort, पूर्णांक>, convertScaleData_<ushort, दशांश>,
      convertScaleData_<ushort, द्विगुण>, 0 },

    { convertScaleData_<लघु, uchar>, convertScaleData_<लघु, schar>,
      convertScaleData_<लघु, ushort>, convertScaleData_<लघु, लघु>,
      convertScaleData_<लघु, पूर्णांक>, convertScaleData_<लघु, दशांश>,
      convertScaleData_<लघु, द्विगुण>, 0 },

    { convertScaleData_<पूर्णांक, uchar>, convertScaleData_<पूर्णांक, schar>,
      convertScaleData_<पूर्णांक, ushort>, convertScaleData_<पूर्णांक, लघु>,
      convertScaleData_<पूर्णांक, पूर्णांक>, convertScaleData_<पूर्णांक, दशांश>,
      convertScaleData_<पूर्णांक, द्विगुण>, 0 },

    { convertScaleData_<दशांश, uchar>, convertScaleData_<दशांश, schar>,
      convertScaleData_<दशांश, ushort>, convertScaleData_<दशांश, लघु>,
      convertScaleData_<दशांश, पूर्णांक>, convertScaleData_<दशांश, दशांश>,
      convertScaleData_<दशांश, द्विगुण>, 0 },

    { convertScaleData_<द्विगुण, uchar>, convertScaleData_<द्विगुण, schar>,
      convertScaleData_<द्विगुण, ushort>, convertScaleData_<द्विगुण, लघु>,
      convertScaleData_<द्विगुण, पूर्णांक>, convertScaleData_<द्विगुण, दशांश>,
      convertScaleData_<द्विगुण, द्विगुण>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    परत func;
}

गणना { HASH_SIZE0 = 8 };

स्थिर ओळीत शून्य copyElem(स्थिरांक uchar* from, uchar* to, माप_प्रकार elemSize)
{
    माप_प्रकार i;
    साठी( i = 0; i + आकार(पूर्णांक) <= elemSize; i += आकार(पूर्णांक) )
        *(पूर्णांक*)(to + i) = *(स्थिरांक पूर्णांक*)(from + i);
    साठी( ; i < elemSize; i++ )
        to[i] = from[i];
}

स्थिर ओळीत तार्किक isZeroElem(स्थिरांक uchar* data, माप_प्रकार elemSize)
{
    माप_प्रकार i;
    साठी( i = 0; i + आकार(पूर्णांक) <= elemSize; i += आकार(पूर्णांक) )
        जर( *(पूर्णांक*)(data + i) != 0 )
            परत असत्य;
    साठी( ; i < elemSize; i++ )
        जर( data[i] != 0 )
            परत असत्य;
    परत सत्य;
}

SparseMat::Hdr::Hdr( पूर्णांक _dims, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type )
{
    refcount = 1;

    dims = _dims;
    मूल्य_Offset = (पूर्णांक)alignSize(आकार(SparseMat::Node) - MAX_DIM*आकार(पूर्णांक) +
                                 dims*आकार(पूर्णांक), CV_ELEM_SIZE1(_type));
    गाठ_Size = alignSize(मूल्य_Offset +
        CV_ELEM_SIZE(_type), (पूर्णांक)आकार(माप_प्रकार));

    पूर्णांक i;
    साठी( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    साठी( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

शून्य SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#जर defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(गाठ_Size);
#जर defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    गाठ_मोजणी = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(पूर्णांक _dims, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(स्थिरांक SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::चालक = (स्थिरांक SparseMat& m)
{
    जर( हे != &m )
    {
        जर( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    परत *हे;
}

SparseMat& SparseMat::चालक=(स्थिरांक Mat& m)
{
    परत (*हे = SparseMat(m));
}

शून्य SparseMat::assignTo(SparseMat& m, पूर्णांक _type) स्थिरांक
{
    जर( _type < 0 )
        m = *हे;
    अन्यथा
        convertTo(m, _type);
}

शून्य SparseMat::addref()
{
    जर( hdr )
        CV_XADD(&hdr->refcount, 1);
}

शून्य SparseMat::release()
{
    जर( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0) स्थिरांक
{
    परत (माप_प्रकार)i0;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0, पूर्णांक i1) स्थिरांक
{
    परत (माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1;
}

माप_प्रकार SparseMat::hash(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2) स्थिरांक
{
    परत ((माप_प्रकार)(अचिन्हित)i0 * HASH_SCALE + (अचिन्हित)i1) * HASH_SCALE + (अचिन्हित)i2;
}

माप_प्रकार SparseMat::hash(स्थिरांक पूर्णांक* idx) स्थिरांक
{
    माप_प्रकार h = (अचिन्हित)idx[0];
    जर( !hdr )
        परत 0;
    पूर्णांक d = hdr->dims;
    साठी(पूर्णांक i = 1; i < d; i++ )
        h = h * HASH_SCALE + (अचिन्हित)idx[i];
    परत h;
}


SparseMat::SparseMat(स्थिरांक Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    पूर्णांक i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    माप_प्रकार esz = m.elemSize();
    स्थिरांक uchar* dptr = m.ptr();

    साठी(;;)
    {
        साठी( i = 0; i < lastSize; i++, dptr += esz )
        {
            जर( isZeroElem(dptr, esz) )
                सुरूठेवा;
            idx[d-1] = i;
            uchar* to = new_गाठ(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        साठी( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            जर( ++idx[i] < m.size[i] )
                थांबा;
            idx[i] = 0;
        }
        जर( i < 0 )
            थांबा;
    }
}

शून्य SparseMat::create(पूर्णांक d, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    साठी( पूर्णांक i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    जर( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        पूर्णांक i;
        साठी( i = 0; i < d; i++ )
            जर( _sizes[i] != hdr->size[i] )
                थांबा;
        जर( i == d )
        {
            clear();
            परत;
        }
    }
    पूर्णांक _sizes_backup[CV_MAX_DIM]; // #5991
    जर (hdr && _sizes == hdr->size)
    {
        साठी(पूर्णांक i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = नवीन Hdr(d, _sizes, _type);
}

शून्य SparseMat::copyTo( SparseMat& m ) स्थिरांक
{
    जर( hdr == m.hdr )
        परत;
    जर( !hdr )
    {
        m.release();
        परत;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount(), esz = elemSize();

    साठी( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        स्थिरांक Node* n = from.गाठ();
        uchar* to = m.new_गाठ(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

शून्य SparseMat::copyTo( Mat& m ) स्थिरांक
{
    CV_Assert( hdr );
    पूर्णांक ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount(), esz = elemSize();

    साठी( माप_प्रकार i = 0; i < N; i++, ++from )
    {
        स्थिरांक Node* n = from.गाठ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


शून्य SparseMat::convertTo( SparseMat& m, पूर्णांक rtype, द्विगुण alpha ) स्थिरांक
{
    पूर्णांक cn = channels();
    जर( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    जर( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        परत;
    }

    CV_Assert(hdr != 0);
    जर( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    जर( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        साठी( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिरांक Node* n = from.गाठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        साठी( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिरांक Node* n = from.गाठ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_गाठ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


शून्य SparseMat::convertTo( Mat& m, पूर्णांक rtype, द्विगुण alpha, द्विगुण beta ) स्थिरांक
{
    पूर्णांक cn = channels();
    जर( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    माप_प्रकार N = nzcount();

    जर( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        साठी( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिरांक Node* n = from.गाठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    अन्यथा
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        साठी( माप_प्रकार i = 0; i < N; i++, ++from )
        {
            स्थिरांक Node* n = from.गाठ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

शून्य SparseMat::clear()
{
    जर( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(पूर्णांक i0, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    माप_प्रकार h = hashval ? *hashval : hash(i0);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h && elem->idx[0] == i0 )
            परत &value<uchar>(elem);
        nidx = elem->next;
    }

    जर( createMissing )
    {
        पूर्णांक idx[] = { i0 };
        परत new_गाठ( idx, h );
    }
    परत रिक्त;
}

uchar* SparseMat::ptr(पूर्णांक i0, पूर्णांक i1, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            परत &value<uchar>(elem);
        nidx = elem->next;
    }

    जर( createMissing )
    {
        पूर्णांक idx[] = { i0, i1 };
        परत new_गाठ( idx, h );
    }
    परत रिक्त;
}

uchar* SparseMat::ptr(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            परत &value<uchar>(elem);
        nidx = elem->next;
    }

    जर( createMissing )
    {
        पूर्णांक idx[] = { i0, i1, i2 };
        परत new_गाठ( idx, h );
    }
    परत रिक्त;
}

uchar* SparseMat::ptr(स्थिरांक पूर्णांक* idx, तार्किक createMissing, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णांक i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h )
        {
            साठी( i = 0; i < d; i++ )
                जर( elem->idx[i] != idx[i] )
                    थांबा;
            जर( i == d )
                परत &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    परत createMissing ? new_गाठ(idx, h) : रिक्त;
}

शून्य SparseMat::erase(पूर्णांक i0, पूर्णांक i1, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            थांबा;
        previdx = nidx;
        nidx = elem->next;
    }

    जर( nidx )
        remove_गाठ(hidx, nidx, previdx);
}

शून्य SparseMat::erase(पूर्णांक i0, पूर्णांक i1, पूर्णांक i2, माप_प्रकार* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    माप_प्रकार h = hashval ? *hashval : hash(i0, i1, i2);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            थांबा;
        previdx = nidx;
        nidx = elem->next;
    }

    जर( nidx )
        remove_गाठ(hidx, nidx, previdx);
}

शून्य SparseMat::erase(स्थिरांक पूर्णांक* idx, माप_प्रकार* hashval)
{
    CV_Assert( hdr );
    पूर्णांक i, d = hdr->dims;
    माप_प्रकार h = hashval ? *hashval : hash(idx);
    माप_प्रकार hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    पर्यंत( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        जर( elem->hashval == h )
        {
            साठी( i = 0; i < d; i++ )
                जर( elem->idx[i] != idx[i] )
                    थांबा;
            जर( i == d )
                थांबा;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    जर( nidx )
        remove_गाठ(hidx, nidx, previdx);
}

शून्य SparseMat::resizeHashTab(माप_प्रकार newsize)
{
    newsize = std::max(newsize, (माप_प्रकार)8);
    जर((newsize & (newsize-1)) != 0)
        newsize = (माप_प्रकार)1 << cvCeil(std::log((द्विगुण)newsize)/CV_LOG2);

    माप_प्रकार hsize = hdr->hashtab.size();
    std::vector<माप_प्रकार> _newh(newsize);
    माप_प्रकार* newh = &_newh[0];
    साठी( माप_प्रकार i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    साठी( माप_प्रकार i = 0; i < hsize; i++ )
    {
        माप_प्रकार nidx = hdr->hashtab[i];
        पर्यंत( nidx )
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

uchar* SparseMat::new_गाठ(स्थिरांक पूर्णांक* idx, माप_प्रकार hashval)
{
    स्थिरांक पूर्णांक HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    माप_प्रकार hsize = hdr->hashtab.size();
    जर( ++hdr->गाठ_मोजणी > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (माप_प्रकार)8));
        hsize = hdr->hashtab.size();
    }

    जर( !hdr->freeList )
    {
        माप_प्रकार i, nsz = hdr->गाठ_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        साठी( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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
    साठी( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    माप_प्रकार esz = elemSize();
    uchar* p = &value<uchar>(elem);
    जर( esz == आकार(दशांश) )
        *((दशांश*)p) = 0.f;
    अन्यथा जर( esz == आकार(द्विगुण) )
        *((द्विगुण*)p) = 0.;
    अन्यथा
        स_रखो(p, 0, esz);

    परत p;
}


शून्य SparseMat::remove_गाठ(माप_प्रकार hidx, माप_प्रकार nidx, माप_प्रकार previdx)
{
    Node* n = गाठ(nidx);
    जर( previdx )
    {
        Node* prev = गाठ(previdx);
        prev->next = n->next;
    }
    अन्यथा
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->गाठ_मोजणी;
}

//
// Operations
//
द्विगुण norm( स्थिरांक SparseMat& src, पूर्णांक normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    माप_प्रकार i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    पूर्णांक type = src.type();
    द्विगुण result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    जर( type == CV_32F )
    {
        जर( normType == NORM_INF )
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::असल((द्विगुण)it.value<दशांश>()));
            }
        अन्यथा जर( normType == NORM_L1 )
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::असल(it.value<दशांश>());
            }
        अन्यथा
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<दशांश>();
                result += v*v;
            }
    }
    अन्यथा जर( type == CV_64F )
    {
        जर( normType == NORM_INF )
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::असल(it.value<द्विगुण>()));
            }
        अन्यथा जर( normType == NORM_L1 )
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::असल(it.value<द्विगुण>());
            }
        अन्यथा
            साठी( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                द्विगुण v = it.value<द्विगुण>();
                result += v*v;
            }
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    जर( normType == NORM_L2 )
        result = std::वर्ग_मूल(result);
    परत result;
}

शून्य minMaxLoc( स्थिरांक SparseMat& src, द्विगुण* _minval, द्विगुण* _maxval, पूर्णांक* _minidx, पूर्णांक* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    माप_प्रकार i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    पूर्णांक type = src.type();
    स्थिरांक पूर्णांक *minidx = 0, *maxidx = 0;

    जर( type == CV_32F )
    {
        दशांश minval = भग्न_उच्च, maxval = -भग्न_उच्च;
        साठी( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            दशांश v = it.value<दशांश>();
            जर( v < minval )
            {
                minval = v;
                minidx = it.गाठ()->idx;
            }
            जर( v > maxval )
            {
                maxval = v;
                maxidx = it.गाठ()->idx;
            }
        }
        जर( _minval )
            *_minval = minval;
        जर( _maxval )
            *_maxval = maxval;
    }
    अन्यथा जर( type == CV_64F )
    {
        द्विगुण minval = द्विग_उच्च, maxval = -द्विग_उच्च;
        साठी( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            द्विगुण v = it.value<द्विगुण>();
            जर( v < minval )
            {
                minval = v;
                minidx = it.गाठ()->idx;
            }
            जर( v > maxval )
            {
                maxval = v;
                maxidx = it.गाठ()->idx;
            }
        }
        जर( _minval )
            *_minval = minval;
        जर( _maxval )
            *_maxval = maxval;
    }
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    जर( _minidx && minidx )
        साठी( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    जर( _maxidx && maxidx )
        साठी( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


शून्य normalize( स्थिरांक SparseMat& src, SparseMat& dst, द्विगुण a, पूर्णांक norm_type )
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1;
    जर( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > द्विग_अंतर ? a/scale : 0.;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
