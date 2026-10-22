// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/mat.hpp"

નામાકાશ cv {

template<typename T1, typename T2> શૂન્ય
convertData_(અચલ શૂન્ય* _from, શૂન્ય* _to, પૂર્ણાંક cn)
{
    અચલ T1* from = (અચલ T1*)_from;
    T2* to = (T2*)_to;
    જો( cn == 1 )
        *to = saturate_cast<T2>(*from);
    નહીંતર
        માટે( પૂર્ણાંક i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> શૂન્ય
convertScaleData_(અચલ શૂન્ય* _from, શૂન્ય* _to, પૂર્ણાંક cn, દ્વિગુણ alpha, દ્વિગુણ beta)
{
    અચલ T1* from = (અચલ T1*)_from;
    T2* to = (T2*)_to;
    જો( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    નહીંતર
        માટે( પૂર્ણાંક i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

પ્રકારવ્યાખ્યા શૂન્ય (*ConvertData)(અચલ શૂન્ય* from, શૂન્ય* to, પૂર્ણાંક cn);
પ્રકારવ્યાખ્યા શૂન્ય (*ConvertScaleData)(અચલ શૂન્ય* from, શૂન્ય* to, પૂર્ણાંક cn, દ્વિગુણ alpha, દ્વિગુણ beta);

સ્થિર ConvertData getConvertElem(પૂર્ણાંક fromType, પૂર્ણાંક toType)
{
    સ્થિર ConvertData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, પૂર્ણાંક>, convertData_<uchar, દશાંશ>,
      convertData_<uchar, દ્વિગુણ>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, પૂર્ણાંક>, convertData_<schar, દશાંશ>,
      convertData_<schar, દ્વિગુણ>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, પૂર્ણાંક>, convertData_<ushort, દશાંશ>,
      convertData_<ushort, દ્વિગુણ>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, પૂર્ણાંક>, convertData_<short, દશાંશ>,
      convertData_<short, દ્વિગુણ>, 0 },

    { convertData_<પૂર્ણાંક, uchar>, convertData_<પૂર્ણાંક, schar>,
      convertData_<પૂર્ણાંક, ushort>, convertData_<પૂર્ણાંક, short>,
      convertData_<પૂર્ણાંક, પૂર્ણાંક>, convertData_<પૂર્ણાંક, દશાંશ>,
      convertData_<પૂર્ણાંક, દ્વિગુણ>, 0 },

    { convertData_<દશાંશ, uchar>, convertData_<દશાંશ, schar>,
      convertData_<દશાંશ, ushort>, convertData_<દશાંશ, short>,
      convertData_<દશાંશ, પૂર્ણાંક>, convertData_<દશાંશ, દશાંશ>,
      convertData_<દશાંશ, દ્વિગુણ>, 0 },

    { convertData_<દ્વિગુણ, uchar>, convertData_<દ્વિગુણ, schar>,
      convertData_<દ્વિગુણ, ushort>, convertData_<દ્વિગુણ, short>,
      convertData_<દ્વિગુણ, પૂર્ણાંક>, convertData_<દ્વિગુણ, દશાંશ>,
      convertData_<દ્વિગુણ, દ્વિગુણ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    પાછા func;
}

સ્થિર ConvertScaleData getConvertScaleElem(પૂર્ણાંક fromType, પૂર્ણાંક toType)
{
    સ્થિર ConvertScaleData tab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, પૂર્ણાંક>, convertScaleData_<uchar, દશાંશ>,
      convertScaleData_<uchar, દ્વિગુણ>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, પૂર્ણાંક>, convertScaleData_<schar, દશાંશ>,
      convertScaleData_<schar, દ્વિગુણ>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, પૂર્ણાંક>, convertScaleData_<ushort, દશાંશ>,
      convertScaleData_<ushort, દ્વિગુણ>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, પૂર્ણાંક>, convertScaleData_<short, દશાંશ>,
      convertScaleData_<short, દ્વિગુણ>, 0 },

    { convertScaleData_<પૂર્ણાંક, uchar>, convertScaleData_<પૂર્ણાંક, schar>,
      convertScaleData_<પૂર્ણાંક, ushort>, convertScaleData_<પૂર્ણાંક, short>,
      convertScaleData_<પૂર્ણાંક, પૂર્ણાંક>, convertScaleData_<પૂર્ણાંક, દશાંશ>,
      convertScaleData_<પૂર્ણાંક, દ્વિગુણ>, 0 },

    { convertScaleData_<દશાંશ, uchar>, convertScaleData_<દશાંશ, schar>,
      convertScaleData_<દશાંશ, ushort>, convertScaleData_<દશાંશ, short>,
      convertScaleData_<દશાંશ, પૂર્ણાંક>, convertScaleData_<દશાંશ, દશાંશ>,
      convertScaleData_<દશાંશ, દ્વિગુણ>, 0 },

    { convertScaleData_<દ્વિગુણ, uchar>, convertScaleData_<દ્વિગુણ, schar>,
      convertScaleData_<દ્વિગુણ, ushort>, convertScaleData_<દ્વિગુણ, short>,
      convertScaleData_<દ્વિગુણ, પૂર્ણાંક>, convertScaleData_<દ્વિગુણ, દશાંશ>,
      convertScaleData_<દ્વિગુણ, દ્વિગુણ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CV_MAT_DEPTH(fromType)][CV_MAT_DEPTH(toType)];
    CV_Assert( func != 0 );
    પાછા func;
}

ગણના { HASH_SIZE0 = 8 };

સ્થિર અંતઃરેખ શૂન્ય copyElem(અચલ uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    માટે( i = 0; i + કદ(પૂર્ણાંક) <= elemSize; i += કદ(પૂર્ણાંક) )
        *(પૂર્ણાંક*)(to + i) = *(અચલ પૂર્ણાંક*)(from + i);
    માટે( ; i < elemSize; i++ )
        to[i] = from[i];
}

સ્થિર અંતઃરેખ તાર્કિક isZeroElem(અચલ uchar* data, size_t elemSize)
{
    size_t i;
    માટે( i = 0; i + કદ(પૂર્ણાંક) <= elemSize; i += કદ(પૂર્ણાંક) )
        જો( *(પૂર્ણાંક*)(data + i) != 0 )
            પાછા ખોટું;
    માટે( ; i < elemSize; i++ )
        જો( data[i] != 0 )
            પાછા ખોટું;
    પાછા સાચું;
}

SparseMat::Hdr::Hdr( પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type )
{
    refcount = 1;

    dims = _dims;
    કિંમત_Offset = (પૂર્ણાંક)alignSize(કદ(SparseMat::Node) - MAX_DIM*કદ(પૂર્ણાંક) +
                                 dims*કદ(પૂર્ણાંક), CV_ELEM_SIZE1(_type));
    ગાંઠ_Size = alignSize(કિંમત_Offset +
        CV_ELEM_SIZE(_type), (પૂર્ણાંક)કદ(size_t));

    પૂર્ણાંક i;
    માટે( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    માટે( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

શૂન્ય SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#જો defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    pool.resize(ગાંઠ_Size);
#જો defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#pragma GCC diagnostic pop
#endif
    ગાંઠ_ગણતરી = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(અચલ SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::operator = (અચલ SparseMat& m)
{
    જો( આ != &m )
    {
        જો( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    પાછા *આ;
}

SparseMat& SparseMat::operator=(અચલ Mat& m)
{
    પાછા (*આ = SparseMat(m));
}

શૂન્ય SparseMat::assignTo(SparseMat& m, પૂર્ણાંક _type) અચલ
{
    જો( _type < 0 )
        m = *આ;
    નહીંતર
        convertTo(m, _type);
}

શૂન્ય SparseMat::addref()
{
    જો( hdr )
        CV_XADD(&hdr->refcount, 1);
}

શૂન્ય SparseMat::release()
{
    જો( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

size_t SparseMat::hash(પૂર્ણાંક i0) અચલ
{
    પાછા (size_t)i0;
}

size_t SparseMat::hash(પૂર્ણાંક i0, પૂર્ણાંક i1) અચલ
{
    પાછા (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

size_t SparseMat::hash(પૂર્ણાંક i0, પૂર્ણાંક i1, પૂર્ણાંક i2) અચલ
{
    પાછા ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

size_t SparseMat::hash(અચલ પૂર્ણાંક* idx) અચલ
{
    size_t h = (unsigned)idx[0];
    જો( !hdr )
        પાછા 0;
    પૂર્ણાંક d = hdr->dims;
    માટે(પૂર્ણાંક i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    પાછા h;
}


SparseMat::SparseMat(અચલ Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    પૂર્ણાંક i, idx[CV_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m.elemSize();
    અચલ uchar* dptr = m.ptr();

    માટે(;;)
    {
        માટે( i = 0; i < lastSize; i++, dptr += esz )
        {
            જો( isZeroElem(dptr, esz) )
                ચાલુ;
            idx[d-1] = i;
            uchar* to = new_ગાંઠ(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        માટે( i = d - 2; i >= 0; i-- )
        {
            dptr += m.step[i] - m.size[i+1]*m.step[i+1];
            જો( ++idx[i] < m.size[i] )
                તોડો;
            idx[i] = 0;
        }
        જો( i < 0 )
            તોડો;
    }
}

શૂન્ય SparseMat::create(પૂર્ણાંક d, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    માટે( પૂર્ણાંક i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    જો( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        પૂર્ણાંક i;
        માટે( i = 0; i < d; i++ )
            જો( _sizes[i] != hdr->size[i] )
                તોડો;
        જો( i == d )
        {
            clear();
            પાછા;
        }
    }
    પૂર્ણાંક _sizes_backup[CV_MAX_DIM]; // #5991
    જો (hdr && _sizes == hdr->size)
    {
        માટે(પૂર્ણાંક i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = નવું Hdr(d, _sizes, _type);
}

શૂન્ય SparseMat::copyTo( SparseMat& m ) અચલ
{
    જો( hdr == m.hdr )
        પાછા;
    જો( !hdr )
    {
        m.release();
        પાછા;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    માટે( size_t i = 0; i < N; i++, ++from )
    {
        અચલ Node* n = from.ગાંઠ();
        uchar* to = m.new_ગાંઠ(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

શૂન્ય SparseMat::copyTo( Mat& m ) અચલ
{
    CV_Assert( hdr );
    પૂર્ણાંક ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    માટે( size_t i = 0; i < N; i++, ++from )
    {
        અચલ Node* n = from.ગાંઠ();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


શૂન્ય SparseMat::convertTo( SparseMat& m, પૂર્ણાંક rtype, દ્વિગુણ alpha ) અચલ
{
    પૂર્ણાંક cn = channels();
    જો( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    જો( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        પાછા;
    }

    CV_Assert(hdr != 0);
    જો( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    જો( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        માટે( size_t i = 0; i < N; i++, ++from )
        {
            અચલ Node* n = from.ગાંઠ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ગાંઠ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    નહીંતર
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        માટે( size_t i = 0; i < N; i++, ++from )
        {
            અચલ Node* n = from.ગાંઠ();
            uchar* to = hdr == m.hdr ? from.ptr : m.new_ગાંઠ(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


શૂન્ય SparseMat::convertTo( Mat& m, પૂર્ણાંક rtype, દ્વિગુણ alpha, દ્વિગુણ beta ) અચલ
{
    પૂર્ણાંક cn = channels();
    જો( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    જો( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        માટે( size_t i = 0; i < N; i++, ++from )
        {
            અચલ Node* n = from.ગાંઠ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    નહીંતર
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        માટે( size_t i = 0; i < N; i++, ++from )
        {
            અચલ Node* n = from.ગાંઠ();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

શૂન્ય SparseMat::clear()
{
    જો( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(પૂર્ણાંક i0, તાર્કિક createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h && elem->idx[0] == i0 )
            પાછા &value<uchar>(elem);
        nidx = elem->next;
    }

    જો( createMissing )
    {
        પૂર્ણાંક idx[] = { i0 };
        પાછા new_ગાંઠ( idx, h );
    }
    પાછા ખાલી;
}

uchar* SparseMat::ptr(પૂર્ણાંક i0, પૂર્ણાંક i1, તાર્કિક createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            પાછા &value<uchar>(elem);
        nidx = elem->next;
    }

    જો( createMissing )
    {
        પૂર્ણાંક idx[] = { i0, i1 };
        પાછા new_ગાંઠ( idx, h );
    }
    પાછા ખાલી;
}

uchar* SparseMat::ptr(પૂર્ણાંક i0, પૂર્ણાંક i1, પૂર્ણાંક i2, તાર્કિક createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            પાછા &value<uchar>(elem);
        nidx = elem->next;
    }

    જો( createMissing )
    {
        પૂર્ણાંક idx[] = { i0, i1, i2 };
        પાછા new_ગાંઠ( idx, h );
    }
    પાછા ખાલી;
}

uchar* SparseMat::ptr(અચલ પૂર્ણાંક* idx, તાર્કિક createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    પૂર્ણાંક i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h )
        {
            માટે( i = 0; i < d; i++ )
                જો( elem->idx[i] != idx[i] )
                    તોડો;
            જો( i == d )
                પાછા &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    પાછા createMissing ? new_ગાંઠ(idx, h) : ખાલી;
}

શૂન્ય SparseMat::erase(પૂર્ણાંક i0, પૂર્ણાંક i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            તોડો;
        previdx = nidx;
        nidx = elem->next;
    }

    જો( nidx )
        remove_ગાંઠ(hidx, nidx, previdx);
}

શૂન્ય SparseMat::erase(પૂર્ણાંક i0, પૂર્ણાંક i1, પૂર્ણાંક i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            તોડો;
        previdx = nidx;
        nidx = elem->next;
    }

    જો( nidx )
        remove_ગાંઠ(hidx, nidx, previdx);
}

શૂન્ય SparseMat::erase(અચલ પૂર્ણાંક* idx, size_t* hashval)
{
    CV_Assert( hdr );
    પૂર્ણાંક i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    જ્યારે( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        જો( elem->hashval == h )
        {
            માટે( i = 0; i < d; i++ )
                જો( elem->idx[i] != idx[i] )
                    તોડો;
            જો( i == d )
                તોડો;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    જો( nidx )
        remove_ગાંઠ(hidx, nidx, previdx);
}

શૂન્ય SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    જો((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::log((દ્વિગુણ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    માટે( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    માટે( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        જ્યારે( nidx )
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

uchar* SparseMat::new_ગાંઠ(અચલ પૂર્ણાંક* idx, size_t hashval)
{
    અચલ પૂર્ણાંક HASH_MAX_FILL_FACTOR=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    જો( ++hdr->ગાંઠ_ગણતરી > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    જો( !hdr->freeList )
    {
        size_t i, nsz = hdr->ગાંઠ_Size, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        માટે( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    પૂર્ણાંક i, d = hdr->dims;
    માટે( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    જો( esz == કદ(દશાંશ) )
        *((દશાંશ*)p) = 0.f;
    નહીંતર જો( esz == કદ(દ્વિગુણ) )
        *((દ્વિગુણ*)p) = 0.;
    નહીંતર
        memset(p, 0, esz);

    પાછા p;
}


શૂન્ય SparseMat::remove_ગાંઠ(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = ગાંઠ(nidx);
    જો( previdx )
    {
        Node* prev = ગાંઠ(previdx);
        prev->next = n->next;
    }
    નહીંતર
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->ગાંઠ_ગણતરી;
}

//
// Operations
//
દ્વિગુણ norm( અચલ SparseMat& src, પૂર્ણાંક normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    પૂર્ણાંક type = src.type();
    દ્વિગુણ result = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    જો( type == CV_32F )
    {
        જો( normType == NORM_INF )
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs((દ્વિગુણ)it.value<દશાંશ>()));
            }
        નહીંતર જો( normType == NORM_L1 )
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<દશાંશ>());
            }
        નહીંતર
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                દ્વિગુણ v = it.value<દશાંશ>();
                result += v*v;
            }
    }
    નહીંતર જો( type == CV_64F )
    {
        જો( normType == NORM_INF )
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result = std::max(result, std::abs(it.value<દ્વિગુણ>()));
            }
        નહીંતર જો( normType == NORM_L1 )
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                result += std::abs(it.value<દ્વિગુણ>());
            }
        નહીંતર
            માટે( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                દ્વિગુણ v = it.value<દ્વિગુણ>();
                result += v*v;
            }
    }
    નહીંતર
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    જો( normType == NORM_L2 )
        result = std::sqrt(result);
    પાછા result;
}

શૂન્ય minMaxLoc( અચલ SparseMat& src, દ્વિગુણ* _minval, દ્વિગુણ* _maxval, પૂર્ણાંક* _minidx, પૂર્ણાંક* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    પૂર્ણાંક type = src.type();
    અચલ પૂર્ણાંક *minidx = 0, *maxidx = 0;

    જો( type == CV_32F )
    {
        દશાંશ minval = FLT_MAX, maxval = -FLT_MAX;
        માટે( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            દશાંશ v = it.value<દશાંશ>();
            જો( v < minval )
            {
                minval = v;
                minidx = it.ગાંઠ()->idx;
            }
            જો( v > maxval )
            {
                maxval = v;
                maxidx = it.ગાંઠ()->idx;
            }
        }
        જો( _minval )
            *_minval = minval;
        જો( _maxval )
            *_maxval = maxval;
    }
    નહીંતર જો( type == CV_64F )
    {
        દ્વિગુણ minval = DBL_MAX, maxval = -DBL_MAX;
        માટે( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            દ્વિગુણ v = it.value<દ્વિગુણ>();
            જો( v < minval )
            {
                minval = v;
                minidx = it.ગાંઠ()->idx;
            }
            જો( v > maxval )
            {
                maxval = v;
                maxidx = it.ગાંઠ()->idx;
            }
        }
        જો( _minval )
            *_minval = minval;
        જો( _maxval )
            *_maxval = maxval;
    }
    નહીંતર
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    જો( _minidx && minidx )
        માટે( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    જો( _maxidx && maxidx )
        માટે( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


શૂન્ય normalize( અચલ SparseMat& src, SparseMat& dst, દ્વિગુણ a, પૂર્ણાંક norm_type )
{
    CV_INSTRUMENT_REGION();

    દ્વિગુણ scale = 1;
    જો( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    નહીંતર
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

} // cv::
