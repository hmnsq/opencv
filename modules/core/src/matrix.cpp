// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "bufferpool.impl.hpp"

فضاینام cv {

std::string layoutToString(DataLayout layout)
{
    بازگشت
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

منطقی operator == (ثابت MatShape& size1, ثابت MatShape& size2)
{
    اگر (size1.dims != size2.dims)
        بازگشت نادرست;
    اگر (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        بازگشت نادرست;
    اگر (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        بازگشت نادرست;
    برای (صحیح i = 0; i < size1.dims; i++) {
        اگر (size1.p[i] != size2.p[i])
            بازگشت نادرست;
    }
    بازگشت درست;
}

منطقی operator != (ثابت MatShape& size1, ثابت MatShape& size2)
{
    بازگشت !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    بازگشت MatShape(0);
}

تهی MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    برای (صحیح i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

تهی MatShape::resize(size_t newSize, صحیح value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    صحیح old_dims = std::max(dims, 0);
    dims = (صحیح)newSize;
    برای (صحیح i = old_dims; i < dims; i++)
        p[i] = value;
}

تهی MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

تهی MatShape::assign(size_t newSize, صحیح value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (صحیح)newSize;
    برای (صحیح i = 0; i < dims; i++)
        p[i] = value;
}

تهی MatShape::assign(صحیح newSize, صحیح value)
{
    assign((size_t)newSize, value);
}

تهی MatShape::assign(ثابت صحیح* begin, ثابت صحیح* end)
{
    assign_(begin, end);
}

تهی MatShape::assign_(ثابت صحیح* begin, ثابت صحیح* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (صحیح)newSize;
    برای (صحیح i = 0; i < dims; i++)
        p[i] = begin[i];
}

صحیح* MatShape::begin() { بازگشت p; }
ثابت صحیح* MatShape::begin() ثابت { بازگشت p; }
صحیح* MatShape::end() { بازگشت p + std::max(dims, 0); }
ثابت صحیح* MatShape::end() ثابت { بازگشت p + std::max(dims, 0); }
صحیح& MatShape::back() { بازگشت p[std::max(dims-1, 0)]; }
ثابت صحیح& MatShape::back() ثابت { بازگشت p[std::max(dims-1, 0)]; }

تهی MatShape::push_back(صحیح value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

تهی MatShape::emplace_back(صحیح value)
{
    push_back(value);
}

تهی MatShape::insert(صحیح* where, صحیح value)
{
    صحیح old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    برای (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

تهی MatShape::insert(صحیح* where, size_t شمار_2, صحیح value)
{
    صحیح old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+شمار_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحیح)(old_dims+شمار_2);
    برای (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+شمار_2] = p[i];
    برای (صحیح i = 0; i < (صحیح)شمار_2; i++)
        p[i+ofs] = value;
}

تهی MatShape::insert(صحیح* where, صحیح شمار_2, صحیح value)
{
    insert(where, (size_t)شمار_2, value);
}

تهی MatShape::insert(صحیح* where, ثابت صحیح* begin, ثابت صحیح* end)
{
    درج(where, begin, end);
}

تهی MatShape::درج(صحیح* where, ثابت صحیح* begin, ثابت صحیح* end)
{
    صحیح old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحیح)(old_dims+delta);
    برای (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+delta] = p[i];
    برای (صحیح i = 0; i < (صحیح)delta; i++)
        p[i+ofs] = begin[i];
}

تهی MatShape::erase(صحیح* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    اگر (ofs == dims)
        بازگشت;
    dims--;
    برای (صحیح i = (صحیح)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() ثابت
{
    size_t result = 1;
    اگر (dims < 0)
        بازگشت 0;
    برای (صحیح i = 0; i < dims; i++)
        result *= p[i];
    بازگشت result;
}

std::vector<صحیح> MatShape::vec() ثابت
{
    اگر (dims < 0)
        بازگشت std::vector<صحیح>(1, 0);
    بازگشت std::vector<صحیح>(p, p + dims);
}

std::string MatShape::str() ثابت
{
    std::stringstream sstrm;
    اگر (empty()) {
        sstrm << "<empty>";
    } وگرنه اگر (dims == 0) {
        sstrm << "<scalar>";
    } وگرنه {
        sstrm << "[";
        برای (صحیح i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    بازگشت sstrm.str();
}

ایستا تهی finalizeBlockLayout(MatShape& size, صحیح C=0)
{
    اگر (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        صحیح C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } وگرنه {
        size.C = 0;
    }
    برای (صحیح i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    اگر (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ثابت صحیح* size_, DataLayout layout_, صحیح C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحیح)dims_;
    برای (صحیح i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*این, C_);
}

MatShape::MatShape(size_t dims_, صحیح value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحیح)dims_;
    برای (صحیح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*این, 0);
}

MatShape::MatShape(std::initializer_list<صحیح> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (صحیح)shape.size();
    auto it = shape.begin();
    برای (صحیح i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*این, 0);
}

MatShape::MatShape(صحیح dims_, صحیح value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    برای (صحیح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*این, 0);
}

MatShape::MatShape(ثابت std::vector<صحیح>& shape_, DataLayout layout_, صحیح C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (صحیح)shape_size;
    برای (صحیح i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*این, C_);
}

MatShape::MatShape(ثابت صحیح* begin, ثابت صحیح* end, DataLayout layout_, صحیح C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (صحیح)shape_size;
    برای (صحیح i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*این, C_);
}

MatShape::MatShape(ثابت MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    برای (صحیح i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ثابت MatShape& shape)
{
    اگر (این != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        برای (صحیح i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    بازگشت *این;
}

منطقی MatShape::hasSymbols() ثابت
{
    برای (صحیح i = 0; i < dims; i++) {
        اگر (p[i] < 0)
            بازگشت درست;
    }
    بازگشت نادرست;
}

صحیح MatShape::channels() ثابت
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    بازگشت layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, صحیح C0) ثابت
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *این;
    newsize.layout = newLayout;

    اگر (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        صحیح Corig = channels();
        newsize.C = Corig;

        اگر (layout == DATA_LAYOUT_NHWC) {
            برای (صحیح i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } وگرنه اگر (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        اگر (newLayout == DATA_LAYOUT_NHWC) {
            برای (صحیح i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } وگرنه {
        CV_Assert_N(C0 <= 1);

        اگر (newLayout == layout)
            بازگشت newsize;

        // NHWC => NCHW
        اگر (newLayout == DATA_LAYOUT_NCHW) {
            برای (صحیح i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } وگرنه {
            // NCHW => NHWC
            برای (صحیح i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    بازگشت newsize;
}

ایستا صحیح normalizeShapeAxis(صحیح axis, صحیح dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    بازگشت axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ایستا MatShape toSemantic(ثابت MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    بازگشت shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ایستا MatShape toBlockOf(MatShape result, ثابت MatShape& src, ثابت نویسه* op)
{
    اگر (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (صحیح)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    بازگشت result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ثابت std::vector<صحیح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == DATA_LAYOUT_BLOCK)
        بازگشت toBlockOf(toSemantic(*این).squeeze(axes), *این, "squeeze");
    منطقی remove[MAX_DIMS] = {نادرست};
    اگر (axes.empty())
    {
        برای (صحیح i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    برای (صحیح a : axes)
    {
        صحیح i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = درست;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    برای (صحیح i = 0; i < dims; i++)
        اگر (!remove[i])
            result.p[result.dims++] = p[i];
    بازگشت result;
}

MatShape MatShape::unsqueeze(ثابت std::vector<صحیح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == DATA_LAYOUT_BLOCK)
        بازگشت toBlockOf(toSemantic(*این).unsqueeze(axes), *این, "unsqueeze");
    ثابت صحیح outDims = dims + (صحیح)axes.size();
    CV_CheckLE(outDims, (صحیح)MAX_DIMS, "unsqueeze: too many dimensions");
    منطقی insert[MAX_DIMS] = {نادرست};
    برای (صحیح a : axes)
    {
        صحیح i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = درست;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    برای (صحیح i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    بازگشت result;
}

MatShape MatShape::flatten(صحیح startAxis, صحیح endAxis) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == DATA_LAYOUT_BLOCK)
        بازگشت toBlockOf(toSemantic(*این).flatten(startAxis, endAxis), *این, "flatten");
    اگر (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        بازگشت MatShape({1});
    }
    صحیح start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    برای (صحیح i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((دوگانه)merged, (دوگانه)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    برای (صحیح i = 0; i < dims; i++)
    {
        اگر (i == start)
            result.p[result.dims++] = (صحیح)merged;
        وگرنه اگر (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    بازگشت result;
}

MatShape MatShape::reshape(ثابت MatShape& newShape, منطقی allowZero) ثابت
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    اگر (layout == DATA_LAYOUT_BLOCK)
        بازگشت toBlockOf(toSemantic(*این).reshape(newShape, allowZero), *این, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    صحیح inferred = -1;
    int64 known = 1;
    برای (صحیح i = 0; i < result.dims; i++)
    {
        صحیح& sz = result.p[i];
        اگر (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ادامه;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        اگر (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ثابت int64 total_ = (int64)total();
    اگر (inferred >= 0)
    {
        CV_Check((دوگانه)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((دوگانه)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((دوگانه)(total_/known), (دوگانه)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (صحیح)(total_/known);
    }
    وگرنه اگر (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    بازگشت result;
}

MatShape MatShape::expand(ثابت MatShape& another) ثابت
{
    اگر (dims == 0)
        بازگشت another;
    اگر (another.dims == 0)
        بازگشت *این;

    اگر ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    اگر (dims < 0 || another.dims < 0)
        بازگشت result;

    result = *این;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    برای (صحیح i = result.dims-1; i >= 0; i--) {
        صحیح i1 = i - (result.dims - dims);
        صحیح i2 = i - (result.dims - another.dims);
        صحیح sz1 = i1 < 0 ? 1 : p[i1];
        صحیح sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    بازگشت result;
}

/////////////////////////// MatAllocator ////////////////////////////

تهی MatAllocator::map(UMatData*, AccessFlag) ثابت
{
}

تهی MatAllocator::unmap(UMatData* u) ثابت
{
    اگر(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

تهی MatAllocator::download(UMatData* u, تهی* dstptr,
         صحیح dims, ثابت size_t sz[],
         ثابت size_t srcofs[], ثابت size_t srcstep[],
         ثابت size_t dststep[]) ثابت
{
    اگر(!u)
        بازگشت;
    صحیح isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    برای( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        اگر( sz[i] == 0 )
            بازگشت;
        اگر( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برای( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


تهی MatAllocator::upload(UMatData* u, ثابت تهی* srcptr, صحیح dims, ثابت size_t sz[],
                    ثابت size_t dstofs[], ثابت size_t dststep[],
                    ثابت size_t srcstep[]) ثابت
{
    اگر(!u)
        بازگشت;
    صحیح isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    برای( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        اگر( sz[i] == 0 )
            بازگشت;
        اگر( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, (تهی*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برای( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

تهی MatAllocator::copy(UMatData* usrc, UMatData* udst, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dstofs[], ثابت size_t dststep[], منطقی /*sync*/) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر(!usrc || !udst)
        بازگشت;
    صحیح isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    برای( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        اگر( sz[i] == 0 )
            بازگشت;
        اگر( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        اگر( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برای( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ثابت نویسه* id) ثابت
{
    CV_UNUSED(id);
    ایستا DummyBufferPoolController dummy;
    بازگشت &dummy;
}

رده StdMatAllocator CV_FINAL : عمومی MatAllocator
{
عمومی:
    UMatData* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       تهی* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        برای( صحیح i = dims-1; i >= 0; i-- )
        {
            اگر( step )
            {
                اگر( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                وگرنه
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = جدید UMatData(این);
        u->data = u->origdata = data;
        u->size = total;
        اگر(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        بازگشت u;
    }

    منطقی allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        اگر(!u) بازگشت نادرست;
        بازگشت درست;
    }

    تهی deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        اگر( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

ایستا
MatAllocator*& getDefaultAllocatorMatRef()
{
    ایستا MatAllocator* g_matAllocator = Mat::getStdAllocator();
    بازگشت g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    بازگشت getDefaultAllocatorMatRef();
}

تهی Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, جدید StdMatAllocator())
}

//==================================================================================================

تهی setSize( Mat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* _steps, منطقی autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    برای (صحیح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        اگر (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } وگرنه اگر (autoSteps) {
            m.step[i] = autostep;
        } وگرنه {
            m.step[i] = 0;
        }
    }

    اگر( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وگرنه {
        m.cols = m.rows = -1;
    }
}

صحیح updateContinuityFlag(صحیح flags, صحیح dims, ثابت صحیح* size, ثابت size_t* step)
{
    صحیح i, j;
    اگر (dims <= 1)
        بازگشت flags | Mat::CONTINUOUS_FLAG;
    برای( i = 0; i < dims; i++ )
    {
        اگر( size[i] > 1 )
            بشکن;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    برای( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        اگر( step[j]*size[j] < step[j-1] )
            بشکن;
    }

    اگر( j <= i && t == (uint64)(صحیح)t )
        بازگشت flags | Mat::CONTINUOUS_FLAG;
    بازگشت flags & ~Mat::CONTINUOUS_FLAG;
}

تهی Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

تهی finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    صحیح d = m.dims;
    اگر( d > 2 )
        m.rows = m.cols = -1;
    اگر(m.u)
        m.datastart = m.data = m.u->data;
    اگر( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        اگر( m.size[0] > 0 )
        {
            صحیح lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            برای( صحیح i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        وگرنه
            m.dataend = m.datalimit;
    }
    وگرنه
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(صحیح _rows, صحیح _cols, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(صحیح _rows, صحیح _cols, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *این = _s;
}

Mat::Mat(Size _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *این = _s;
}

Mat::Mat(صحیح _dims, ثابت صحیح* _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *این = _s;
}

Mat::Mat(ثابت std::vector<صحیح>& _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ثابت std::vector<صحیح>& _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *این = _s;
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *این = _s;
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *این = _s;
}

Mat::Mat(ثابت Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    اگر( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(صحیح _rows, صحیح _cols, صحیح _type, تهی* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != پوچ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    اگر( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    وگرنه
    {
        CV_Assert( _step >= minstep );
        اگر (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }
    }
    size[0] = rows;
    size[1] = cols;
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step * rows;
    dataend = datalimit - _step + minstep;
    updateContinuityFlag();
}

Mat::Mat(Size _sz, صحیح _type, تهی* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != پوچ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    اگر( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    وگرنه
    {
        CV_CheckGE(_step, minstep, "");

        اگر (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }
    }
    size[0] = rows;
    size[1] = cols;
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::operator = (ثابت Mat& m)
{
    اگر( این != &m )
    {
        اگر( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        size = m.size;
        step = m.step;
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    بازگشت *این;
}

Mat Mat::clone() ثابت
{
    Mat m;
    copyTo(m);
    بازگشت m;
}

تهی Mat::assignTo( Mat& m, صحیح _type ) ثابت
{
    اگر( _type < 0 )
        m = *این;
    وگرنه
        convertTo(m, _type);
}

تهی Mat::create(صحیح _rows, صحیح _cols, صحیح _type)
{
    _type &= TYPE_MASK;
    اگر( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        بازگشت;
    صحیح sz[] = {_rows, _cols};
    create(2, sz, _type);
}

تهی Mat::create(Size _sz, صحیح _type)
{
    create(_sz.height, _sz.width, _type);
}

تهی Mat::createSameSize(درون_داد_Array m, صحیح type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

تهی Mat::fit(صحیح _dims, ثابت صحیح* _sizes, صحیح _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    برای (صحیح i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    اگر (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } وگرنه {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        صحیح _dummy_size = 0;
        setSize(*این, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, درست);
        finalizeHdr(*این);
    }
}

تهی Mat::fit(ثابت std::vector<صحیح>& _shape, صحیح _type)
{
    fit((صحیح)_shape.size(), _shape.data(), _type);
}

تهی Mat::fit(ثابت MatShape& _shape, صحیح _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

تهی Mat::fit(std::initializer_list<صحیح> _shape, صحیح _type)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح new_ndims = (صحیح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    برای (صحیح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

تهی Mat::fit(صحیح _rows, صحیح _cols, صحیح _type)
{
    _type &= TYPE_MASK;
    صحیح sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

تهی Mat::fit(Size _sz, صحیح _type)
{
    fit(_sz.height, _sz.width, _type);
}

تهی Mat::fitSameSize(درون_داد_Array m, صحیح _type)
{
    fit(m.shape(), _type);
}

تهی Mat::addref()
{
    اگر( u )
        CV_XADD(&u->refcount, 1);
}

تهی Mat::release()
{
    اگر( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = پوچ;
    datastart = dataend = datalimit = data = 0;
    برای(صحیح i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(صحیح i) ثابت
{
    بازگشت step.p[i] / elemSize1();
}

منطقی Mat::empty() ثابت
{
    بازگشت data == 0 || total() == 0;
}

size_t Mat::total() ثابت
{
    اگر( dims <= 2 )
        بازگشت (size_t)rows * cols;
    size_t p = 1;
    برای( صحیح i = 0; i < dims; i++ )
        p *= size[i];
    بازگشت p;
}

size_t Mat::total(صحیح startDim, صحیح endDim) ثابت
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    صحیح endDim_ = endDim <= dims ? endDim : dims;
    برای( صحیح i = startDim; i < endDim_; i++ )
        p *= size[i];
    بازگشت p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = پوچ; m.datastart = پوچ; m.dataend = پوچ; m.datalimit = پوچ;
    m.allocator = پوچ;
    m.u = پوچ;
}


Mat& Mat::operator = (Mat&& m)
{
    اگر (این == &m)
      بازگشت *این;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = پوچ; m.datastart = پوچ; m.dataend = پوچ; m.datalimit = پوچ;
    m.allocator = پوچ;
    m.u = پوچ;
    بازگشت *این;
}


تهی Mat::create(صحیح d0, ثابت صحیح* _sizes, صحیح _type)
{
    صحیح sz1 = 1, d = d0;
    صحیح i;
    اگر (d == 0) {
        d = 1;
        _sizes = (ثابت صحیح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    اگر( data && d == dims && _type == type() )
    {
        برای( i = 0; i < d; i++ )
            اگر( size[i] != _sizes[i] )
                بشکن;
        اگر( i == d )
            بازگشت;
    }

    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (_sizes == (این->size.p))
    {
        برای(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    اگر( d == 0 )
        بازگشت;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*این, d, _sizes, 0, درست);

    اگر( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        اگر(!a)
            a = a0;
        تلاش
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        بگیر (...)
        {
            اگر (a == a0)
                پرتابکن;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*این);
    size.dims = dims = d0;
}

تهی Mat::create(ثابت std::vector<صحیح>& _sizes, صحیح _type)
{
    create((صحیح)_sizes.size(), _sizes.data(), _type);
}

تهی Mat::create(ثابت MatShape& _shape, صحیح _type)
{
    اگر (_shape.dims < 0) {
        release();
        بازگشت;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

تهی Mat::create(std::initializer_list<صحیح> _shape, صحیح _type)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح new_ndims = (صحیح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    برای (صحیح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

تهی Mat::copySize(ثابت Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

تهی Mat::deallocate()
{
    اگر(u)
    {
        UMatData* u_ = u;
        u = پوچ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ثابت Mat& m, ثابت Range& _rowRange, ثابت Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    اگر( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        برای( صحیح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *این = m(rs);
        بازگشت;
    }

    *این = m;
    تلاش
    {
        اگر( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        اگر( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    بگیر(...)
    {
        release();
        پرتابکن;
    }

    updateContinuityFlag();

    اگر( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ثابت Mat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    data += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    اگر( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    اگر( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(صحیح _dims, ثابت صحیح* _sizes, صحیح _type, تهی* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    اگر (_dims > 0 || _data != nullptr)
        setSize(*این, _dims, _sizes, _steps, درست);
    finalizeHdr(*این);
}


Mat::Mat(ثابت std::vector<صحیح>& _sizes, صحیح _type, تهی* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    صحیح _dims = (صحیح)_sizes.size();
    اگر (_dims > 0 || _data != nullptr)
        setSize(*این, _dims, _sizes.data(), _steps, درست);
    finalizeHdr(*این);
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type, تهی* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    اگر (_shape.dims >= 0) {
        setSize(*این, _shape.dims, _shape.p, _steps, درست);
    }
    وگرنه {
        CV_Assert(!data);
    }
    finalizeHdr(*این);
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type, تهی* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح _dims = (صحیح)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    برای (صحیح i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    اگر (_dims > 0 || _data != nullptr)
        setSize(*این, _dims, new_shape, _steps, درست);
    finalizeHdr(*این);
}

Mat::Mat(ثابت Mat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحیح d = m.dims;

    CV_Assert(ranges);
    برای( صحیح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *این = m;
    برای( صحیح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        اگر( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ثابت Mat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    صحیح d = m.dims;

    CV_Assert((صحیح)ranges.size() == d);
    برای (صحیح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *این = m;
    برای (صحیح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(صحیح d) ثابت
{
    CV_Assert( dims == 2 );
    Mat m = *این;
    size_t esz = elemSize();
    صحیح len;

    اگر( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    وگرنه
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    اگر( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    بازگشت m;
}


تهی Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    اگر( isSubmatrix() )
        *این = rowRange(0, size.p[0] - (صحیح)nelems);
    وگرنه
    {
        size.p[0] -= (صحیح)nelems;
        اگر (dims == 2)
            rows = size.p[0];
        وگرنه اگر (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


تهی Mat::افزودن_back(ثابت تهی* elem)
{
    size_t r = size.p[0];
    اگر( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = صحیح(r + 1);
    اگر (dims == 2)
        rows = size.p[0];
    وگرنه اگر (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    برای( صحیح i = 1; i < dims; i++ )
        tsz *= size.p[i];
    اگر( esz < step.p[0] || tsz != (uint64)(صحیح)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


تهی Mat::reserve(size_t nelems)
{
    ثابت size_t MIN_SIZE = 64;

    CV_Assert( (صحیح)nelems >= 0 );
    اگر( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        بازگشت;

    صحیح r = size.p[0];
    اگر( (size_t)r >= nelems)
        بازگشت;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    اگر (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (صحیح)nelems;
    size_t newbytes = newsize.total() * esz;
    اگر (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = صحیح(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    اگر( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *این = m;
    size.p[0] = r;

    اگر (dims == 2)
        rows = size.p[0];
    وگرنه اگر (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


تهی Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    صحیح mtype = CV_8UC1;
    اگر (!empty())
    {
        اگر (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            بازگشت;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#اگر SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    صحیح newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#وگرنه
    صحیح newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    صحیح newcols = (صحیح)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


تهی Mat::resize(size_t nelems)
{
    صحیح r = size.p[0];
    اگر( r == (صحیح)nelems )
        بازگشت;
    CV_Assert( (صحیح)nelems >= 0 );

    اگر( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (صحیح)nelems;
    اگر (dims == 2)
        rows = size.p[0];
    وگرنه اگر (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


تهی Mat::resize(size_t nelems, ثابت Scalar& s)
{
    صحیح saveRows = size.p[0];
    resize(nelems);

    اگر( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

تهی Mat::push_back(ثابت Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    اگر( delta == 0 )
        بازگشت;
    اگر( این == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        بازگشت;
    }
    اگر( !data )
    {
        *این = elems.clone();
        بازگشت;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    منطقی eq = size == elems.size;
    size.p[0] = صحیح(r);
    اگر( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    اگر( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    اگر (step0 < minstep) {
        اگر (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    اگر( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += صحیح(delta);
    اگر (dims == 2)
        rows = size.p[0];
    وگرنه اگر (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    اگر( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    وگرنه
    {
        Mat part = rowRange(صحیح(r), صحیح(r + delta));
        elems.copyTo(part);
    }
}


تهی Mat::locateROI( Size& wholeSize, Point& ofs ) ثابت
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    اگر( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وگرنه
    {
        ofs.y = (صحیح)(delta1/step[0]);
        ofs.x = (صحیح)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (صحیح)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (صحیح)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( صحیح dtop, صحیح dbottom, صحیح dleft, صحیح dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    صحیح row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    صحیح col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    اگر(row1 > row2)
        std::swap(row1, row2);
    اگر(col1 > col2)
        std::swap(col1, col2);

    اگر (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } وگرنه {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    بازگشت *این;
}

Mat Mat::reshape(صحیح new_cn, صحیح new_rows) ثابت
{
    صحیح cn = channels();
    Mat hdr = *این;

    اگر( new_cn == 0 )
        new_cn = cn;

    اگر( dims > 2 )
    {
        اگر( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            بازگشت hdr;
        }
        CV_Assert( new_rows > 0 );
        صحیح sz[] = { new_rows, (صحیح)(total()*cn/new_rows) };
        بازگشت reshape(new_cn, 2, sz);
    }

    صحیح total_width = cols * cn;

    اگر( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    اگر( new_rows != 0 && new_rows != rows )
    {
        صحیح total_size = total_width * rows;
        اگر( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        اگر( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } وگرنه {
        hdr.size[0] = hdr.rows = rows;
        اگر (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    صحیح new_width = total_width / new_cn;

    اگر( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_رقم_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    بازگشت hdr;
}

Mat Mat::reshape(صحیح _cn, صحیح _newndims, ثابت صحیح* _newsz) ثابت
{
    اگر(_newndims == dims)
    {
        اگر(_newsz == 0)
            بازگشت reshape(_cn);
        اگر(_newndims == 2)
            بازگشت reshape(_cn, _newsz[0]);
    }

    اگر (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        اگر (_cn == 0)
            _cn = این->channels();
        وگرنه
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = این->total() * این->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<صحیح, 4> newsz_buf( (size_t)_newndims );

        صحیح m1_idx = -1;

        برای (صحیح i = 0; i < _newndims; i++)
        {
            اگر (_newsz[i] >= 0) {
                اگر (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وگرنه
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } وگرنه {
                اگر (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        اگر (m1_idx >= 0) {
            اگر (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (صحیح)(total_elem1_ref / total_elem1);
        } وگرنه اگر (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *این;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), پوچ, درست);

        بازگشت hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(صحیح _cn, ثابت std::vector<صحیح>& _newshape) ثابت
{
    صحیح newdims = (صحیح)_newshape.size();
    اگر(newdims == 0 && empty())
    {
        بازگشت *این;
    }

    بازگشت reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(صحیح _cn, ثابت MatShape& _newshape) ثابت
{
    اگر (_newshape.dims < 0) {
        صحیح newshape[] = {0};
        بازگشت reshape(_cn, 1, newshape);
    }
    بازگشت reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(صحیح _cn, std::initializer_list<صحیح> newshape_) ثابت
{
    صحیح newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    برای (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    بازگشت reshape(_cn, (صحیح)newshape_dims, newshape);
}

Mat Mat::reinterpret(صحیح type) ثابت
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(این->type()) == CV_ELEM_SIZE(type));
    Mat m = *این;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    بازگشت m;
}

Mat Mat::diag(ثابت Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    صحیح len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    اگر( d.cols == 1 )
        d.copyTo(md);
    وگرنه
        transpose(d, md);
    بازگشت m;
}

صحیح Mat::checkVector(صحیح _elemChannels, صحیح _depth, منطقی _requireContinuous) ثابت
{
    بازگشت data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (صحیح)(total()*channels()/_elemChannels) : -1;
}


ایستا درونخطی Size getContinuousSize_(صحیح flags, صحیح cols, صحیح rows, صحیح widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    منطقی has_int_overflow = sz >= INT_MAX;
    منطقی isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    بازگشت (isContiguous && !has_int_overflow)
            ? Size((صحیح)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, صحیح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    بازگشت getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, صحیح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ثابت Size sz1 = m1.size();
    اگر (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        منطقی is_m1_vector = m1.cols == 1 || m1.rows == 1;
        منطقی is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        صحیح total = (صحیح)total_sz;  // vector-column
        منطقی isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقی has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        اگر (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        بازگشت Size(m1.cols * widthScale, m1.rows);
    }
    بازگشت getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحیح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ثابت Size sz1 = m1.size();
    اگر (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        منطقی is_m1_vector = m1.cols == 1 || m1.rows == 1;
        منطقی is_m2_vector = m2.cols == 1 || m2.rows == 1;
        منطقی is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        صحیح total = (صحیح)total_sz;  // vector-column
        منطقی isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقی has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        اگر (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        بازگشت Size(m1.cols * widthScale, m1.rows);
    }
    بازگشت getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
