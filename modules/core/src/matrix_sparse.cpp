// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/mat.hpp"

నామాకాశ cv {

మూస<రకం_పేరు T1, రకం_పేరు T2> శూన్య
convert_దత్తాంశం(స్థిరరాశి శూన్య* _from, శూన్య* గమ్యం_2, పూర్ణాంకం cn)
{
    స్థిరరాశి T1* from = (స్థిరరాశి T1*)_from;
    T2* గమ్యం = (T2*)గమ్యం_2;
    ఒకవేళ( cn == 1 )
        *గమ్యం = saturate_cast<T2>(*from);
    లేకపోతే
        కొరకు( పూర్ణాంకం i = 0; i < cn; i++ )
            గమ్యం[i] = saturate_cast<T2>(from[i]);
}

మూస<రకం_పేరు T1, రకం_పేరు T2> శూన్య
convert_Scale_దత్తాంశం(స్థిరరాశి శూన్య* _from, శూన్య* గమ్యం_2, పూర్ణాంకం cn, ద్విగుణ alpha, ద్విగుణ beta)
{
    స్థిరరాశి T1* from = (స్థిరరాశి T1*)_from;
    T2* గమ్యం = (T2*)గమ్యం_2;
    ఒకవేళ( cn == 1 )
        *గమ్యం = saturate_cast<T2>(*from*alpha + beta);
    లేకపోతే
        కొరకు( పూర్ణాంకం i = 0; i < cn; i++ )
            గమ్యం[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

రకనిర్వచనం శూన్య (*Convert_దత్తాంశం)(స్థిరరాశి శూన్య* from, శూన్య* గమ్యం, పూర్ణాంకం cn);
రకనిర్వచనం శూన్య (*Convert_Scale_దత్తాంశం)(స్థిరరాశి శూన్య* from, శూన్య* గమ్యం, పూర్ణాంకం cn, ద్విగుణ alpha, ద్విగుణ beta);

స్థిర Convert_దత్తాంశం getConvertElem(పూర్ణాంకం fromType, పూర్ణాంకం గమ్యం_Type)
{
    స్థిర Convert_దత్తాంశం tab[CV_లోతు_MAX][CV_లోతు_MAX] =
    {{ convert_దత్తాంశం<uchar, uchar>, convert_దత్తాంశం<uchar, schar>,
      convert_దత్తాంశం<uchar, ushort>, convert_దత్తాంశం<uchar, పొట్టి>,
      convert_దత్తాంశం<uchar, పూర్ణాంకం>, convert_దత్తాంశం<uchar, దశాంశం>,
      convert_దత్తాంశం<uchar, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<schar, uchar>, convert_దత్తాంశం<schar, schar>,
      convert_దత్తాంశం<schar, ushort>, convert_దత్తాంశం<schar, పొట్టి>,
      convert_దత్తాంశం<schar, పూర్ణాంకం>, convert_దత్తాంశం<schar, దశాంశం>,
      convert_దత్తాంశం<schar, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<ushort, uchar>, convert_దత్తాంశం<ushort, schar>,
      convert_దత్తాంశం<ushort, ushort>, convert_దత్తాంశం<ushort, పొట్టి>,
      convert_దత్తాంశం<ushort, పూర్ణాంకం>, convert_దత్తాంశం<ushort, దశాంశం>,
      convert_దత్తాంశం<ushort, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<పొట్టి, uchar>, convert_దత్తాంశం<పొట్టి, schar>,
      convert_దత్తాంశం<పొట్టి, ushort>, convert_దత్తాంశం<పొట్టి, పొట్టి>,
      convert_దత్తాంశం<పొట్టి, పూర్ణాంకం>, convert_దత్తాంశం<పొట్టి, దశాంశం>,
      convert_దత్తాంశం<పొట్టి, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<పూర్ణాంకం, uchar>, convert_దత్తాంశం<పూర్ణాంకం, schar>,
      convert_దత్తాంశం<పూర్ణాంకం, ushort>, convert_దత్తాంశం<పూర్ణాంకం, పొట్టి>,
      convert_దత్తాంశం<పూర్ణాంకం, పూర్ణాంకం>, convert_దత్తాంశం<పూర్ణాంకం, దశాంశం>,
      convert_దత్తాంశం<పూర్ణాంకం, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<దశాంశం, uchar>, convert_దత్తాంశం<దశాంశం, schar>,
      convert_దత్తాంశం<దశాంశం, ushort>, convert_దత్తాంశం<దశాంశం, పొట్టి>,
      convert_దత్తాంశం<దశాంశం, పూర్ణాంకం>, convert_దత్తాంశం<దశాంశం, దశాంశం>,
      convert_దత్తాంశం<దశాంశం, ద్విగుణ>, 0 },

    { convert_దత్తాంశం<ద్విగుణ, uchar>, convert_దత్తాంశం<ద్విగుణ, schar>,
      convert_దత్తాంశం<ద్విగుణ, ushort>, convert_దత్తాంశం<ద్విగుణ, పొట్టి>,
      convert_దత్తాంశం<ద్విగుణ, పూర్ణాంకం>, convert_దత్తాంశం<ద్విగుణ, దశాంశం>,
      convert_దత్తాంశం<ద్విగుణ, ద్విగుణ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_దత్తాంశం func = tab[CV_MAT_లోతు(fromType)][CV_MAT_లోతు(గమ్యం_Type)];
    CV_Assert( func != 0 );
    తిరిగి func;
}

స్థిర Convert_Scale_దత్తాంశం getConvertScaleElem(పూర్ణాంకం fromType, పూర్ణాంకం గమ్యం_Type)
{
    స్థిర Convert_Scale_దత్తాంశం tab[CV_లోతు_MAX][CV_లోతు_MAX] =
    {{ convert_Scale_దత్తాంశం<uchar, uchar>, convert_Scale_దత్తాంశం<uchar, schar>,
      convert_Scale_దత్తాంశం<uchar, ushort>, convert_Scale_దత్తాంశం<uchar, పొట్టి>,
      convert_Scale_దత్తాంశం<uchar, పూర్ణాంకం>, convert_Scale_దత్తాంశం<uchar, దశాంశం>,
      convert_Scale_దత్తాంశం<uchar, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<schar, uchar>, convert_Scale_దత్తాంశం<schar, schar>,
      convert_Scale_దత్తాంశం<schar, ushort>, convert_Scale_దత్తాంశం<schar, పొట్టి>,
      convert_Scale_దత్తాంశం<schar, పూర్ణాంకం>, convert_Scale_దత్తాంశం<schar, దశాంశం>,
      convert_Scale_దత్తాంశం<schar, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<ushort, uchar>, convert_Scale_దత్తాంశం<ushort, schar>,
      convert_Scale_దత్తాంశం<ushort, ushort>, convert_Scale_దత్తాంశం<ushort, పొట్టి>,
      convert_Scale_దత్తాంశం<ushort, పూర్ణాంకం>, convert_Scale_దత్తాంశం<ushort, దశాంశం>,
      convert_Scale_దత్తాంశం<ushort, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<పొట్టి, uchar>, convert_Scale_దత్తాంశం<పొట్టి, schar>,
      convert_Scale_దత్తాంశం<పొట్టి, ushort>, convert_Scale_దత్తాంశం<పొట్టి, పొట్టి>,
      convert_Scale_దత్తాంశం<పొట్టి, పూర్ణాంకం>, convert_Scale_దత్తాంశం<పొట్టి, దశాంశం>,
      convert_Scale_దత్తాంశం<పొట్టి, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<పూర్ణాంకం, uchar>, convert_Scale_దత్తాంశం<పూర్ణాంకం, schar>,
      convert_Scale_దత్తాంశం<పూర్ణాంకం, ushort>, convert_Scale_దత్తాంశం<పూర్ణాంకం, పొట్టి>,
      convert_Scale_దత్తాంశం<పూర్ణాంకం, పూర్ణాంకం>, convert_Scale_దత్తాంశం<పూర్ణాంకం, దశాంశం>,
      convert_Scale_దత్తాంశం<పూర్ణాంకం, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<దశాంశం, uchar>, convert_Scale_దత్తాంశం<దశాంశం, schar>,
      convert_Scale_దత్తాంశం<దశాంశం, ushort>, convert_Scale_దత్తాంశం<దశాంశం, పొట్టి>,
      convert_Scale_దత్తాంశం<దశాంశం, పూర్ణాంకం>, convert_Scale_దత్తాంశం<దశాంశం, దశాంశం>,
      convert_Scale_దత్తాంశం<దశాంశం, ద్విగుణ>, 0 },

    { convert_Scale_దత్తాంశం<ద్విగుణ, uchar>, convert_Scale_దత్తాంశం<ద్విగుణ, schar>,
      convert_Scale_దత్తాంశం<ద్విగుణ, ushort>, convert_Scale_దత్తాంశం<ద్విగుణ, పొట్టి>,
      convert_Scale_దత్తాంశం<ద్విగుణ, పూర్ణాంకం>, convert_Scale_దత్తాంశం<ద్విగుణ, దశాంశం>,
      convert_Scale_దత్తాంశం<ద్విగుణ, ద్విగుణ>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    Convert_Scale_దత్తాంశం func = tab[CV_MAT_లోతు(fromType)][CV_MAT_లోతు(గమ్యం_Type)];
    CV_Assert( func != 0 );
    తిరిగి func;
}

గణన { HASH_SIZE0 = 8 };

స్థిర అంతఃవరుస శూన్య copyElem(స్థిరరాశి uchar* from, uchar* గమ్యం, size_t elem_కొలత)
{
    size_t i;
    కొరకు( i = 0; i + పరిమాణం(పూర్ణాంకం) <= elem_కొలత; i += పరిమాణం(పూర్ణాంకం) )
        *(పూర్ణాంకం*)(గమ్యం + i) = *(స్థిరరాశి పూర్ణాంకం*)(from + i);
    కొరకు( ; i < elem_కొలత; i++ )
        గమ్యం[i] = from[i];
}

స్థిర అంతఃవరుస తార్కిక isZeroElem(స్థిరరాశి uchar* దత్తాంశం, size_t elem_కొలత)
{
    size_t i;
    కొరకు( i = 0; i + పరిమాణం(పూర్ణాంకం) <= elem_కొలత; i += పరిమాణం(పూర్ణాంకం) )
        ఒకవేళ( *(పూర్ణాంకం*)(దత్తాంశం + i) != 0 )
            తిరిగి అబద్ధం;
    కొరకు( ; i < elem_కొలత; i++ )
        ఒకవేళ( దత్తాంశం[i] != 0 )
            తిరిగి అబద్ధం;
    తిరిగి నిజం;
}

SparseMat::Hdr::Hdr( పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type )
{
    refcount = 1;

    dims = _dims;
    విలువ_స్థానభ్రంశం = (పూర్ణాంకం)align_కొలత(పరిమాణం(SparseMat::Node) - MAX_DIM*పరిమాణం(పూర్ణాంకం) +
                                 dims*పరిమాణం(పూర్ణాంకం), CV_ELEM_SIZE1(_type));
    కణుపు_కొలత = align_కొలత(విలువ_స్థానభ్రంశం +
        CV_ELEM_కొలత(_type), (పూర్ణాంకం)పరిమాణం(size_t));

    పూర్ణాంకం i;
    కొరకు( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    కొరకు( ; i < CV_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

శూన్య SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
#ఒకవేళ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wstringop-overflow"
#అయితే_ముగింపు
    pool.resize(కణుపు_కొలత);
#ఒకవేళ defined(__GNUC__)  && (__GNUC__ == 13) && !defined(__clang__) && (__cplusplus >= 202002L)
#సూచన GCC diagnostic pop
#అయితే_ముగింపు
    కణుపు_లెక్క = freeList = 0;
}

///////////////////////////// SparseMat /////////////////////////////

SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

SparseMat::SparseMat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

SparseMat::SparseMat(స్థిరరాశి SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

SparseMat::~SparseMat()
{
    release();
}

SparseMat& SparseMat::క్రియాకారకం = (స్థిరరాశి SparseMat& m)
{
    ఒకవేళ( ఇది != &m )
    {
        ఒకవేళ( m.hdr )
            CV_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    తిరిగి *ఇది;
}

SparseMat& SparseMat::క్రియాకారకం=(స్థిరరాశి Mat& m)
{
    తిరిగి (*ఇది = SparseMat(m));
}

శూన్య SparseMat::కేటాయించు_గమ్యం(SparseMat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    ఒకవేళ( _type < 0 )
        m = *ఇది;
    లేకపోతే
        convert_గమ్యం(m, _type);
}

శూన్య SparseMat::addref()
{
    ఒకవేళ( hdr )
        CV_XADD(&hdr->refcount, 1);
}

శూన్య SparseMat::release()
{
    ఒకవేళ( hdr && CV_XADD(&hdr->refcount, -1) == 1 )
        తొలగించు hdr;
    hdr = 0;
}

size_t SparseMat::hash(పూర్ణాంకం i0) స్థిరరాశి
{
    తిరిగి (size_t)i0;
}

size_t SparseMat::hash(పూర్ణాంకం i0, పూర్ణాంకం i1) స్థిరరాశి
{
    తిరిగి (size_t)(గుర్తులేని)i0 * HASH_SCALE + (గుర్తులేని)i1;
}

size_t SparseMat::hash(పూర్ణాంకం i0, పూర్ణాంకం i1, పూర్ణాంకం i2) స్థిరరాశి
{
    తిరిగి ((size_t)(గుర్తులేని)i0 * HASH_SCALE + (గుర్తులేని)i1) * HASH_SCALE + (గుర్తులేని)i2;
}

size_t SparseMat::hash(స్థిరరాశి పూర్ణాంకం* idx) స్థిరరాశి
{
    size_t h = (గుర్తులేని)idx[0];
    ఒకవేళ( !hdr )
        తిరిగి 0;
    పూర్ణాంకం d = hdr->dims;
    కొరకు(పూర్ణాంకం i = 1; i < d; i++ )
        h = h * HASH_SCALE + (గుర్తులేని)idx[i];
    తిరిగి h;
}


SparseMat::SparseMat(స్థిరరాశి Mat& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size.p, m.type() );

    పూర్ణాంకం i, idx[CV_MAX_DIM] = {0}, d = m.dims, చివరి_కొలత = m.size[d - 1];
    size_t esz = m.elem_కొలత();
    స్థిరరాశి uchar* dptr = m.ptr();

    కొరకు(;;)
    {
        కొరకు( i = 0; i < చివరి_కొలత; i++, dptr += esz )
        {
            ఒకవేళ( isZeroElem(dptr, esz) )
                కొనసాగు;
            idx[d-1] = i;
            uchar* గమ్యం = new_కణుపు(idx, hash(idx));
            copyElem( dptr, గమ్యం, esz );
        }

        కొరకు( i = d - 2; i >= 0; i-- )
        {
            dptr += m.అడుగు[i] - m.size[i+1]*m.అడుగు[i+1];
            ఒకవేళ( ++idx[i] < m.size[i] )
                ఆపు;
            idx[i] = 0;
        }
        ఒకవేళ( i < 0 )
            ఆపు;
    }
}

శూన్య SparseMat::create(పూర్ణాంకం d, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type)
{
    CV_Assert( _sizes && 0 < d && d <= CV_MAX_DIM );
    కొరకు( పూర్ణాంకం i = 0; i < d; i++ )
        CV_Assert( _sizes[i] > 0 );
    _type = CV_MAT_TYPE(_type);
    ఒకవేళ( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        పూర్ణాంకం i;
        కొరకు( i = 0; i < d; i++ )
            ఒకవేళ( _sizes[i] != hdr->size[i] )
                ఆపు;
        ఒకవేళ( i == d )
        {
            clear();
            తిరిగి;
        }
    }
    పూర్ణాంకం _sizes_backup[CV_MAX_DIM]; // #5991
    ఒకవేళ (hdr && _sizes == hdr->size)
    {
        కొరకు(పూర్ణాంకం i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = కొత్త Hdr(d, _sizes, _type);
}

శూన్య SparseMat::copy_గమ్యం( SparseMat& m ) స్థిరరాశి
{
    ఒకవేళ( hdr == m.hdr )
        తిరిగి;
    ఒకవేళ( !hdr )
    {
        m.release();
        తిరిగి;
    }
    m.create( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_కొలత();

    కొరకు( size_t i = 0; i < N; i++, ++from )
    {
        స్థిరరాశి Node* n = from.కణుపు();
        uchar* గమ్యం = m.new_కణుపు(n->idx, n->hashval);
        copyElem( from.ptr, గమ్యం, esz );
    }
}

శూన్య SparseMat::copy_గమ్యం( Mat& m ) స్థిరరాశి
{
    CV_Assert( hdr );
    పూర్ణాంకం ndims = dims();
    m.create( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elem_కొలత();

    కొరకు( size_t i = 0; i < N; i++, ++from )
    {
        స్థిరరాశి Node* n = from.కణుపు();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


శూన్య SparseMat::convert_గమ్యం( SparseMat& m, పూర్ణాంకం rtype, ద్విగుణ alpha ) స్థిరరాశి
{
    పూర్ణాంకం cn = channels();
    ఒకవేళ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);
    ఒకవేళ( hdr == m.hdr && rtype != type()  )
    {
        SparseMat తాత్కాలిక_2;
        convert_గమ్యం(తాత్కాలిక_2, rtype, alpha);
        m = తాత్కాలిక_2;
        తిరిగి;
    }

    CV_Assert(hdr != 0);
    ఒకవేళ( hdr != m.hdr )
        m.create( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ఒకవేళ( alpha == 1 )
    {
        Convert_దత్తాంశం cvtfunc = getConvertElem(type(), rtype);
        CV_Assert(cvtfunc);
        కొరకు( size_t i = 0; i < N; i++, ++from )
        {
            స్థిరరాశి Node* n = from.కణుపు();
            uchar* గమ్యం = hdr == m.hdr ? from.ptr : m.new_కణుపు(n->idx, n->hashval);
            cvtfunc( from.ptr, గమ్యం, cn );
        }
    }
    లేకపోతే
    {
        Convert_Scale_దత్తాంశం cvtfunc = getConvertScaleElem(type(), rtype);
        CV_Assert(cvtfunc);
        కొరకు( size_t i = 0; i < N; i++, ++from )
        {
            స్థిరరాశి Node* n = from.కణుపు();
            uchar* గమ్యం = hdr == m.hdr ? from.ptr : m.new_కణుపు(n->idx, n->hashval);
            cvtfunc( from.ptr, గమ్యం, cn, alpha, 0 );
        }
    }
}


శూన్య SparseMat::convert_గమ్యం( Mat& m, పూర్ణాంకం rtype, ద్విగుణ alpha, ద్విగుణ beta ) స్థిరరాశి
{
    పూర్ణాంకం cn = channels();
    ఒకవేళ( rtype < 0 )
        rtype = type();
    rtype = CV_MAKETYPE(rtype, cn);

    CV_Assert( hdr );
    m.create( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    ఒకవేళ( alpha == 1 && beta == 0 )
    {
        Convert_దత్తాంశం cvtfunc = getConvertElem(type(), rtype);
        కొరకు( size_t i = 0; i < N; i++, ++from )
        {
            స్థిరరాశి Node* n = from.కణుపు();
            uchar* గమ్యం = m.ptr(n->idx);
            cvtfunc( from.ptr, గమ్యం, cn );
        }
    }
    లేకపోతే
    {
        Convert_Scale_దత్తాంశం cvtfunc = getConvertScaleElem(type(), rtype);
        కొరకు( size_t i = 0; i < N; i++, ++from )
        {
            స్థిరరాశి Node* n = from.కణుపు();
            uchar* గమ్యం = m.ptr(n->idx);
            cvtfunc( from.ptr, గమ్యం, cn, alpha, beta );
        }
    }
}

శూన్య SparseMat::clear()
{
    ఒకవేళ( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(పూర్ణాంకం i0, తార్కిక createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h && elem->idx[0] == i0 )
            తిరిగి &value<uchar>(elem);
        nidx = elem->next;
    }

    ఒకవేళ( createMissing )
    {
        పూర్ణాంకం idx[] = { i0 };
        తిరిగి new_కణుపు( idx, h );
    }
    తిరిగి ఖాళీ;
}

uchar* SparseMat::ptr(పూర్ణాంకం i0, పూర్ణాంకం i1, తార్కిక createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            తిరిగి &value<uchar>(elem);
        nidx = elem->next;
    }

    ఒకవేళ( createMissing )
    {
        పూర్ణాంకం idx[] = { i0, i1 };
        తిరిగి new_కణుపు( idx, h );
    }
    తిరిగి ఖాళీ;
}

uchar* SparseMat::ptr(పూర్ణాంకం i0, పూర్ణాంకం i1, పూర్ణాంకం i2, తార్కిక createMissing, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            తిరిగి &value<uchar>(elem);
        nidx = elem->next;
    }

    ఒకవేళ( createMissing )
    {
        పూర్ణాంకం idx[] = { i0, i1, i2 };
        తిరిగి new_కణుపు( idx, h );
    }
    తిరిగి ఖాళీ;
}

uchar* SparseMat::ptr(స్థిరరాశి పూర్ణాంకం* idx, తార్కిక createMissing, size_t* hashval)
{
    CV_Assert( hdr );
    పూర్ణాంకం i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h )
        {
            కొరకు( i = 0; i < d; i++ )
                ఒకవేళ( elem->idx[i] != idx[i] )
                    ఆపు;
            ఒకవేళ( i == d )
                తిరిగి &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    తిరిగి createMissing ? new_కణుపు(idx, h) : ఖాళీ;
}

శూన్య SparseMat::erase(పూర్ణాంకం i0, పూర్ణాంకం i1, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            ఆపు;
        previdx = nidx;
        nidx = elem->next;
    }

    ఒకవేళ( nidx )
        remove_కణుపు(hidx, nidx, previdx);
}

శూన్య SparseMat::erase(పూర్ణాంకం i0, పూర్ణాంకం i1, పూర్ణాంకం i2, size_t* hashval)
{
    CV_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            ఆపు;
        previdx = nidx;
        nidx = elem->next;
    }

    ఒకవేళ( nidx )
        remove_కణుపు(hidx, nidx, previdx);
}

శూన్య SparseMat::erase(స్థిరరాశి పూర్ణాంకం* idx, size_t* hashval)
{
    CV_Assert( hdr );
    పూర్ణాంకం i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    అయితే( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        ఒకవేళ( elem->hashval == h )
        {
            కొరకు( i = 0; i < d; i++ )
                ఒకవేళ( elem->idx[i] != idx[i] )
                    ఆపు;
            ఒకవేళ( i == d )
                ఆపు;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    ఒకవేళ( nidx )
        remove_కణుపు(hidx, nidx, previdx);
}

శూన్య SparseMat::resizeHashTab(size_t newsize)
{
    newsize = std::max(newsize, (size_t)8);
    ఒకవేళ((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cvCeil(std::సంవర్గమానం((ద్విగుణ)newsize)/CV_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    కొరకు( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    కొరకు( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        అయితే( nidx )
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

uchar* SparseMat::new_కణుపు(స్థిరరాశి పూర్ణాంకం* idx, size_t hashval)
{
    స్థిరరాశి పూర్ణాంకం HASH_MAX_FILL_గుణకం=3;
    CV_Assert(hdr);
    size_t hsize = hdr->hashtab.size();
    ఒకవేళ( ++hdr->కణుపు_లెక్క > hsize*HASH_MAX_FILL_గుణకం )
    {
        resizeHashTab(std::max(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    ఒకవేళ( !hdr->freeList )
    {
        size_t i, nsz = hdr->కణుపు_కొలత, psize = hdr->pool.size(),
            newpsize = std::max(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = std::max(psize, nsz);
        కొరకు( i = hdr->freeList; i < newpsize - nsz; i += nsz )
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

    పూర్ణాంకం i, d = hdr->dims;
    కొరకు( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elem_కొలత();
    uchar* p = &value<uchar>(elem);
    ఒకవేళ( esz == పరిమాణం(దశాంశం) )
        *((దశాంశం*)p) = 0.f;
    లేకపోతే ఒకవేళ( esz == పరిమాణం(ద్విగుణ) )
        *((ద్విగుణ*)p) = 0.;
    లేకపోతే
        memset(p, 0, esz);

    తిరిగి p;
}


శూన్య SparseMat::remove_కణుపు(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = కణుపు(nidx);
    ఒకవేళ( previdx )
    {
        Node* prev = కణుపు(previdx);
        prev->next = n->next;
    }
    లేకపోతే
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->కణుపు_లెక్క;
}

//
// Operations
//
ద్విగుణ norm( స్థిరరాశి SparseMat& src, పూర్ణాంకం normType )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_ముసుగు;
    పూర్ణాంకం type = src.type();
    ద్విగుణ ఫలితం = 0;

    CV_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    ఒకవేళ( type == CV_32F )
    {
        ఒకవేళ( normType == NORM_INF )
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ఫలితం = std::max(ఫలితం, std::నిరపేక్ష((ద్విగుణ)it.value<దశాంశం>()));
            }
        లేకపోతే ఒకవేళ( normType == NORM_L1 )
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ఫలితం += std::నిరపేక్ష(it.value<దశాంశం>());
            }
        లేకపోతే
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ద్విగుణ v = it.value<దశాంశం>();
                ఫలితం += v*v;
            }
    }
    లేకపోతే ఒకవేళ( type == CV_64F )
    {
        ఒకవేళ( normType == NORM_INF )
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ఫలితం = std::max(ఫలితం, std::నిరపేక్ష(it.value<ద్విగుణ>()));
            }
        లేకపోతే ఒకవేళ( normType == NORM_L1 )
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ఫలితం += std::నిరపేక్ష(it.value<ద్విగుణ>());
            }
        లేకపోతే
            కొరకు( i = 0; i < N; i++, ++it )
            {
                CV_Assert(it.ptr);
                ద్విగుణ v = it.value<ద్విగుణ>();
                ఫలితం += v*v;
            }
    }
    లేకపోతే
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ఒకవేళ( normType == NORM_L2 )
        ఫలితం = std::వర్గమూలం(ఫలితం);
    తిరిగి ఫలితం;
}

శూన్య minMaxLoc( స్థిరరాశి SparseMat& src, ద్విగుణ* _minval, ద్విగుణ* _maxval, పూర్ణాంకం* _minidx, పూర్ణాంకం* _maxidx )
{
    CV_INSTRUMENT_REGION();

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    పూర్ణాంకం type = src.type();
    స్థిరరాశి పూర్ణాంకం *minidx = 0, *maxidx = 0;

    ఒకవేళ( type == CV_32F )
    {
        దశాంశం minval = FLT_MAX, maxval = -FLT_MAX;
        కొరకు( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            దశాంశం v = it.value<దశాంశం>();
            ఒకవేళ( v < minval )
            {
                minval = v;
                minidx = it.కణుపు()->idx;
            }
            ఒకవేళ( v > maxval )
            {
                maxval = v;
                maxidx = it.కణుపు()->idx;
            }
        }
        ఒకవేళ( _minval )
            *_minval = minval;
        ఒకవేళ( _maxval )
            *_maxval = maxval;
    }
    లేకపోతే ఒకవేళ( type == CV_64F )
    {
        ద్విగుణ minval = DBL_MAX, maxval = -DBL_MAX;
        కొరకు( i = 0; i < N; i++, ++it )
        {
            CV_Assert(it.ptr);
            ద్విగుణ v = it.value<ద్విగుణ>();
            ఒకవేళ( v < minval )
            {
                minval = v;
                minidx = it.కణుపు()->idx;
            }
            ఒకవేళ( v > maxval )
            {
                maxval = v;
                maxidx = it.కణుపు()->idx;
            }
        }
        ఒకవేళ( _minval )
            *_minval = minval;
        ఒకవేళ( _maxval )
            *_maxval = maxval;
    }
    లేకపోతే
        CV_Error( cv::Error::StsUnsupportedFormat, "Only 32f and 64f are supported" );

    ఒకవేళ( _minidx && minidx )
        కొరకు( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    ఒకవేళ( _maxidx && maxidx )
        కొరకు( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


శూన్య normalize( స్థిరరాశి SparseMat& src, SparseMat& dst, ద్విగుణ a, పూర్ణాంకం norm_type )
{
    CV_INSTRUMENT_REGION();

    ద్విగుణ scale = 1;
    ఒకవేళ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    లేకపోతే
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    src.convert_గమ్యం( dst, -1, scale );
}

} // cv::
