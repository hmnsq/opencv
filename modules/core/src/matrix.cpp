// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "bufferpool.impl.hpp"

نطاق cv {

std::string layout_منزل_String(انکڑے_Layout layout)
{
    أرجع
        layout == انکڑے_LAYOUT_ND ? "ND" :
        layout == انکڑے_LAYOUT_NCHW ? "NCHW" :
        layout == انکڑے_LAYOUT_NHWC ? "NHWC" :
        layout == انکڑے_LAYOUT_بلاک ? "BLOCK" :
        layout == انکڑے_LAYOUT_NCDHW ? "NCDHW" :
        layout == انکڑے_LAYOUT_NDHWC ? "NDHWC" :
        layout == انکڑے_LAYOUT_PLANAR ? "PLANAR" :
        layout == انکڑے_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

منطقي عامل == (ثابت MatShape& size1, ثابت MatShape& size2)
{
    إذا (size1.dims != size2.dims)
        أرجع خطأ;
    إذا (size1.layout != size2.layout &&
        size1.layout != انکڑے_LAYOUT_UNKNOWN &&
        size2.layout != انکڑے_LAYOUT_UNKNOWN)
        أرجع خطأ;
    إذا (size1.layout == انکڑے_LAYOUT_بلاک &&
        size2.layout == انکڑے_LAYOUT_بلاک &&
        size1.C != size2.C)
        أرجع خطأ;
    لكل (صحيح i = 0; i < size1.dims; i++) {
        إذا (size1.p[i] != size2.p[i])
            أرجع خطأ;
    }
    أرجع سچ;
}

منطقي عامل != (ثابت MatShape& size1, ثابت MatShape& size2)
{
    أرجع !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    أرجع MatShape(0);
}

فراغ MatShape::clear()
{
    dims = -1;
    layout = انکڑے_LAYOUT_UNKNOWN;
    C = 0;
    لكل (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

فراغ MatShape::resize(size_t new_ناپ, صحيح value)
{
    CV_Assert(new_ناپ < (size_t)MAX_DIMS);
    صحيح old_dims = std::max(dims, 0);
    dims = (صحيح)new_ناپ;
    لكل (صحيح i = old_dims; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

فراغ MatShape::assign(size_t new_ناپ, صحيح value)
{
    CV_Assert(new_ناپ < (size_t)MAX_DIMS);
    dims = (صحيح)new_ناپ;
    لكل (صحيح i = 0; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::assign(صحيح new_ناپ, صحيح value)
{
    assign((size_t)new_ناپ, value);
}

فراغ MatShape::assign(ثابت صحيح* begin, ثابت صحيح* end)
{
    سونپو(begin, end);
}

فراغ MatShape::سونپو(ثابت صحيح* begin, ثابت صحيح* end)
{
    ptrdiff_t new_ناپ = end - begin;
    CV_Assert(0 <= new_ناپ && new_ناپ < (ptrdiff_t)MAX_DIMS);
    dims = (صحيح)new_ناپ;
    لكل (صحيح i = 0; i < dims; i++)
        p[i] = begin[i];
}

صحيح* MatShape::begin() { أرجع p; }
ثابت صحيح* MatShape::begin() ثابت { أرجع p; }
صحيح* MatShape::end() { أرجع p + std::max(dims, 0); }
ثابت صحيح* MatShape::end() ثابت { أرجع p + std::max(dims, 0); }
صحيح& MatShape::back() { أرجع p[std::max(dims-1, 0)]; }
ثابت صحيح& MatShape::back() ثابت { أرجع p[std::max(dims-1, 0)]; }

فراغ MatShape::push_back(صحيح value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

فراغ MatShape::emplace_back(صحيح value)
{
    push_back(value);
}

فراغ MatShape::insert(صحيح* where, صحيح value)
{
    صحيح old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

فراغ MatShape::insert(صحيح* where, size_t گنتی_2, صحيح value)
{
    صحيح old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+گنتی_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+گنتی_2);
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+گنتی_2] = p[i];
    لكل (صحيح i = 0; i < (صحيح)گنتی_2; i++)
        p[i+ofs] = value;
}

فراغ MatShape::insert(صحيح* where, صحيح گنتی_2, صحيح value)
{
    insert(where, (size_t)گنتی_2, value);
}

فراغ MatShape::insert(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    داخل_کرو(where, begin, end);
}

فراغ MatShape::داخل_کرو(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    صحيح old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+delta);
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+delta] = p[i];
    لكل (صحيح i = 0; i < (صحيح)delta; i++)
        p[i+ofs] = begin[i];
}

فراغ MatShape::erase(صحيح* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    إذا (ofs == dims)
        أرجع;
    dims--;
    لكل (صحيح i = (صحيح)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::کل() ثابت
{
    size_t نتیجہ = 1;
    إذا (dims < 0)
        أرجع 0;
    لكل (صحيح i = 0; i < dims; i++)
        نتیجہ *= p[i];
    أرجع نتیجہ;
}

std::vector<صحيح> MatShape::vec() ثابت
{
    إذا (dims < 0)
        أرجع std::vector<صحيح>(1, 0);
    أرجع std::vector<صحيح>(p, p + dims);
}

std::string MatShape::str() ثابت
{
    std::stringstream sstrm;
    إذا (empty()) {
        sstrm << "<empty>";
    } وإلا إذا (dims == 0) {
        sstrm << "<scalar>";
    } وإلا {
        sstrm << "[";
        لكل (صحيح i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    أرجع sstrm.str();
}

ساكن فراغ finalize_بلاک_Layout(MatShape& size, صحيح C=0)
{
    إذا (size.layout == انکڑے_LAYOUT_بلاک) {
        CV_Assert(size.dims >= 4);
        صحيح C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } وإلا {
        size.C = 0;
    }
    لكل (صحيح i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    إذا (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ثابت صحيح* ناپ_3, انکڑے_Layout layout_, صحيح C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = ناپ_3 ? ناپ_3[i] : 0;
    }
    finalize_بلاک_Layout(*هذا, C_);
}

MatShape::MatShape(size_t dims_, صحيح value, انکڑے_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلاک_Layout(*هذا, 0);
}

MatShape::MatShape(std::initializer_list<صحيح> shape)
{
    layout = انکڑے_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (صحيح)shape.size();
    خودکار it = shape.begin();
    لكل (صحيح i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_بلاک_Layout(*هذا, 0);
}

MatShape::MatShape(صحيح dims_, صحيح value, انکڑے_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلاک_Layout(*هذا, 0);
}

MatShape::MatShape(ثابت std::vector<صحيح>& shape_, انکڑے_Layout layout_, صحيح C_)
{
    layout = layout_;
    size_t shape_ناپ = shape_.size();
    CV_Assert(shape_ناپ < (size_t)MAX_DIMS);
    dims = (صحيح)shape_ناپ;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_بلاک_Layout(*هذا, C_);
}

MatShape::MatShape(ثابت صحيح* begin, ثابت صحيح* end, انکڑے_Layout layout_, صحيح C_)
{
    layout = layout_;
    ptrdiff_t shape_ناپ = end - begin;
    CV_Assert(0 <= shape_ناپ && shape_ناپ < MAX_DIMS);
    dims = (صحيح)shape_ناپ;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_بلاک_Layout(*هذا, C_);
}

MatShape::MatShape(ثابت MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    لكل (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::عامل = (ثابت MatShape& shape)
{
    إذا (هذا != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        لكل (صحيح i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    أرجع *هذا;
}

منطقي MatShape::hasSymbols() ثابت
{
    لكل (صحيح i = 0; i < dims; i++) {
        إذا (p[i] < 0)
            أرجع سچ;
    }
    أرجع خطأ;
}

صحيح MatShape::channels() ثابت
{
    CV_Assert(layout == انکڑے_LAYOUT_بلاک || layout == انکڑے_LAYOUT_NCHW || layout == انکڑے_LAYOUT_NHWC);
    أرجع layout == انکڑے_LAYOUT_بلاک ? C : p[layout == انکڑے_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::منزل_Layout(انکڑے_Layout newLayout, صحيح C0) ثابت
{
    CV_Assert(layout == انکڑے_LAYOUT_بلاک || layout == انکڑے_LAYOUT_NCHW || layout == انکڑے_LAYOUT_NHWC);
    CV_Assert(newLayout == انکڑے_LAYOUT_بلاک || newLayout == انکڑے_LAYOUT_NCHW || newLayout == انکڑے_LAYOUT_NHWC);

    MatShape newsize = *هذا;
    newsize.layout = newLayout;

    إذا (newLayout == انکڑے_LAYOUT_بلاک) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        صحيح Corig = channels();
        newsize.C = Corig;

        إذا (layout == انکڑے_LAYOUT_NHWC) {
            لكل (صحيح i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != انکڑے_LAYOUT_بلاک;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } وإلا إذا (layout == انکڑے_LAYOUT_بلاک) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        إذا (newLayout == انکڑے_LAYOUT_NHWC) {
            لكل (صحيح i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == انکڑے_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } وإلا {
        CV_Assert_N(C0 <= 1);

        إذا (newLayout == layout)
            أرجع newsize;

        // NHWC => NCHW
        إذا (newLayout == انکڑے_LAYOUT_NCHW) {
            لكل (صحيح i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } وإلا {
            // NCHW => NHWC
            لكل (صحيح i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    أرجع newsize;
}

ساكن صحيح normalizeShapeAxis(صحيح axis, صحيح dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    أرجع axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ساكن MatShape منزل_Semantic(ثابت MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    أرجع shape.منزل_Layout(انکڑے_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ساكن MatShape منزل_بلاک_دا(MatShape نتیجہ, ثابت MatShape& src, ثابت حرف* op)
{
    إذا (نتیجہ.dims < 2 || نتیجہ.p[0] != src.p[0] || نتیجہ.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(نتیجہ.dims + 1, (صحيح)MatShape::MAX_DIMS, "too many dimensions");
    نتیجہ.layout = انکڑے_LAYOUT_NCHW;
    أرجع نتیجہ.منزل_Layout(انکڑے_LAYOUT_بلاک, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == انکڑے_LAYOUT_بلاک)
        أرجع منزل_بلاک_دا(منزل_Semantic(*هذا).squeeze(axes), *هذا, "squeeze");
    منطقي remove[MAX_DIMS] = {خطأ};
    إذا (axes.empty())
    {
        لكل (صحيح i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    لكل (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = سچ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = 0;
    لكل (صحيح i = 0; i < dims; i++)
        إذا (!remove[i])
            نتیجہ.p[نتیجہ.dims++] = p[i];
    أرجع نتیجہ;
}

MatShape MatShape::unsqueeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == انکڑے_LAYOUT_بلاک)
        أرجع منزل_بلاک_دا(منزل_Semantic(*هذا).unsqueeze(axes), *هذا, "unsqueeze");
    ثابت صحيح باہرلا_Dims = dims + (صحيح)axes.size();
    CV_CheckLE(باہرلا_Dims, (صحيح)MAX_DIMS, "unsqueeze: too many dimensions");
    منطقي insert[MAX_DIMS] = {خطأ};
    لكل (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, باہرلا_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = سچ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = باہرلا_Dims;
    لكل (صحيح i = 0, j = 0; i < باہرلا_Dims; i++)
        نتیجہ.p[i] = insert[i] ? 1 : p[j++];
    أرجع نتیجہ;
}

MatShape MatShape::flatten(صحيح شروع_Axis, صحيح سرا_Axis) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == انکڑے_LAYOUT_بلاک)
        أرجع منزل_بلاک_دا(منزل_Semantic(*هذا).flatten(شروع_Axis, سرا_Axis), *هذا, "flatten");
    إذا (dims == 0)
    {
        CV_Check(شروع_Axis, شروع_Axis == 0 || شروع_Axis == -1, "flatten: axis is out of range");
        CV_Check(سرا_Axis, سرا_Axis == 0 || سرا_Axis == -1, "flatten: axis is out of range");
        أرجع MatShape({1});
    }
    صحيح شروع = normalizeShapeAxis(شروع_Axis, dims), end = normalizeShapeAxis(سرا_Axis, dims);
    CV_CheckLE(شروع, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    لكل (صحيح i = شروع; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((مزدوج)merged, (مزدوج)ودھ_توں_ودھ_عدد, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = 0;
    لكل (صحيح i = 0; i < dims; i++)
    {
        إذا (i == شروع)
            نتیجہ.p[نتیجہ.dims++] = (صحيح)merged;
        وإلا إذا (i < شروع || i > end)
            نتیجہ.p[نتیجہ.dims++] = p[i];
    }
    أرجع نتیجہ;
}

MatShape MatShape::reshape(ثابت MatShape& newShape, منطقي allowZero) ثابت
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    إذا (layout == انکڑے_LAYOUT_بلاک)
        أرجع منزل_بلاک_دا(منزل_Semantic(*هذا).reshape(newShape, allowZero), *هذا, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape نتیجہ;
    نتیجہ.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, نتیجہ.p);
    صحيح inferred = -1;
    int64 معلوم = 1;
    لكل (صحيح i = 0; i < نتیجہ.dims; i++)
    {
        صحيح& sz = نتیجہ.p[i];
        إذا (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            استمر;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        إذا (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        معلوم *= sz;
    }
    ثابت int64 کل_2 = (int64)کل();
    إذا (inferred >= 0)
    {
        CV_Check((مزدوج)معلوم, معلوم != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((مزدوج)کل_2, کل_2 % معلوم == 0, "reshape: the number of elements does not match");
        CV_CheckLE((مزدوج)(کل_2/معلوم), (مزدوج)ودھ_توں_ودھ_عدد, "reshape: the result is too big");
        نتیجہ.p[inferred] = (صحيح)(کل_2/معلوم);
    }
    وإلا إذا (معلوم != کل_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (لما لما)معلوم, (لما لما)کل_2));
    أرجع نتیجہ;
}

MatShape MatShape::expand(ثابت MatShape& another) ثابت
{
    إذا (dims == 0)
        أرجع another;
    إذا (another.dims == 0)
        أرجع *هذا;

    إذا ((layout == انکڑے_LAYOUT_NCHW || layout == انکڑے_LAYOUT_NHWC) &&
        (another.layout == انکڑے_LAYOUT_NCHW || another.layout == انکڑے_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != انکڑے_LAYOUT_بلاک && another.layout != انکڑے_LAYOUT_بلاک);

    MatShape نتیجہ;

    إذا (dims < 0 || another.dims < 0)
        أرجع نتیجہ;

    نتیجہ = *هذا;
    نتیجہ.dims = std::max(dims, another.dims);
    نتیجہ.layout = layout == انکڑے_LAYOUT_UNKNOWN ? another.layout :
        layout == انکڑے_LAYOUT_ND && (another.layout == انکڑے_LAYOUT_NCHW ||
        another.layout == انکڑے_LAYOUT_NHWC) ? another.layout : layout;
    لكل (صحيح i = نتیجہ.dims-1; i >= 0; i--) {
        صحيح i1 = i - (نتیجہ.dims - dims);
        صحيح i2 = i - (نتیجہ.dims - another.dims);
        صحيح sz1 = i1 < 0 ? 1 : p[i1];
        صحيح sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        نتیجہ.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    أرجع نتیجہ;
}

/////////////////////////// MatAllocator ////////////////////////////

فراغ MatAllocator::map(UMat_انکڑے*, AccessFlag) ثابت
{
}

فراغ MatAllocator::unmap(UMat_انکڑے* u) ثابت
{
    إذا(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

فراغ MatAllocator::download(UMat_انکڑے* u, فراغ* dstptr,
         صحيح dims, ثابت size_t sz[],
         ثابت size_t srcofs[], ثابت size_t srcstep[],
         ثابت size_t dststep[]) ثابت
{
    إذا(!u)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = u->انکڑے;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}


فراغ MatAllocator::upload(UMat_انکڑے* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                    ثابت size_t dstofs[], ثابت size_t dststep[],
                    ثابت size_t srcstep[]) ثابت
{
    إذا(!u)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* dstptr = u->انکڑے;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, (فراغ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}

فراغ MatAllocator::copy(UMat_انکڑے* usrc, UMat_انکڑے* udst, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dstofs[], ثابت size_t dststep[], منطقي /*sync*/) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا(!usrc || !udst)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = usrc->انکڑے;
    uchar* dstptr = udst->انکڑے;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        إذا( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}

بفر_Pool_Controller* MatAllocator::get_بفر_Pool_Controller(ثابت حرف* id) ثابت
{
    CV_بن_ورتیا(id);
    ساكن Dummy_بفر_Pool_Controller dummy;
    أرجع &dummy;
}

صنف StdMatAllocator CV_FINAL : عام MatAllocator
{
عام:
    UMat_انکڑے* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* قدم, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t کل = CV_ELEM_ناپ(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( قدم )
            {
                إذا( data0 && قدم[i] != CV_AUTOSTEP )
                {
                    CV_Assert(کل <= قدم[i]);
                    کل = قدم[i];
                }
                وإلا
                    قدم[i] = کل;
            }
            کل *= sizes[i];
        }
        uchar* انکڑے = data0 ? (uchar*)data0 : (uchar*)fastMalloc(کل);
        UMat_انکڑے* u = جديد UMat_انکڑے(هذا);
        u->انکڑے = u->origdata = انکڑے;
        u->size = کل;
        إذا(data0)
            u->flags |= UMat_انکڑے::USER_ALLOCATED;

        أرجع u;
    }

    منطقي allocate(UMat_انکڑے* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        إذا(!u) أرجع خطأ;
        أرجع سچ;
    }

    فراغ deallocate(UMat_انکڑے* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        إذا( !(u->flags & UMat_انکڑے::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        مٹاؤ u;
    }
};

ساكن
MatAllocator*& getDefaultAllocatorMatRef()
{
    ساكن MatAllocator* g_matAllocator = Mat::getStdAllocator();
    أرجع g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    أرجع getDefaultAllocatorMatRef();
}

فراغ Mat::سیٹ_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, جديد StdMatAllocator())
}

//==================================================================================================

فراغ سیٹ_ناپ( Mat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* قدماں_2, منطقي auto_قدماں)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.قدم[std::max(_dims-1, 0)] = CV_ELEM_ناپ(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.قدم[i+1];
        إذا (قدماں_2) {
            m.قدم[i] = قدماں_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا إذا (auto_قدماں) {
            m.قدم[i] = autostep;
        } وإلا {
            m.قدم[i] = 0;
        }
    }

    إذا( _dims <= 2 )
    {
        m.خانے_گنتی = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.صفاں = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.خانے_گنتی = m.صفاں = -1;
    }
}

صحيح updateContinuityFlag(صحيح flags, صحيح dims, ثابت صحيح* size, ثابت size_t* قدم)
{
    صحيح i, j;
    إذا (dims <= 1)
        أرجع flags | Mat::CONTINUOUS_FLAG;
    لكل( i = 0; i < dims; i++ )
    {
        إذا( size[i] > 1 )
            اكسر;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    لكل( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        إذا( قدم[j]*size[j] < قدم[j-1] )
            اكسر;
    }

    إذا( j <= i && t == (uint64)(صحيح)t )
        أرجع flags | Mat::CONTINUOUS_FLAG;
    أرجع flags & ~Mat::CONTINUOUS_FLAG;
}

فراغ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, قدم.p);
}

فراغ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    إذا( d > 2 )
        m.صفاں = m.خانے_گنتی = -1;
    إذا(m.u)
        m.datastart = m.انکڑے = m.u->انکڑے;
    إذا( m.انکڑے && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.قدم[0];
        إذا( m.size[0] > 0 )
        {
            صحيح lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.قدم[lastdim];
            لكل( صحيح i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.قدم[i];
        }
        وإلا
            m.dataend = m.datalimit;
    }
    وإلا
        m.dataend = m.datalimit = m.انکڑے;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(صفاں_3, خانے_گنتی_3, _type);
}

Mat::Mat(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(صفاں_3, خانے_گنتی_3, _type);
    *هذا = _s;
}

Mat::Mat(ناپ _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.اچائی, _sz.چوڑائی, _type );
}

Mat::Mat(ناپ _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.اچائی, _sz.چوڑائی, _type);
    *هذا = _s;
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(ثابت Mat& m)
    : flags(m.flags), dims(m.dims), صفاں(m.صفاں), خانے_گنتی(m.خانے_گنتی), انکڑے(m.انکڑے),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), قدم(m.قدم)
{
    إذا( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, فراغ* انکڑے_3, size_t قدم_3)
    : flags(MAGIC_VAL + (_type & TYPE_نقاب)), dims(2), صفاں(صفاں_3), خانے_گنتی(خانے_گنتی_3),
      انکڑے((uchar*)انکڑے_3), datastart((uchar*)انکڑے_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(کل() == 0 || انکڑے != عدم);

    size_t esz = CV_ELEM_ناپ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = خانے_گنتی * esz;
    إذا( قدم_3 == AUTO_قدم )
    {
        قدم_3 = minstep;
    }
    وإلا
    {
        CV_Assert( قدم_3 >= minstep );
        إذا (قدم_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_قدم, "Step must be a multiple of esz1");
        }
    }
    size[0] = صفاں;
    size[1] = خانے_گنتی;
    قدم[0] = قدم_3;
    قدم[1] = esz;
    datalimit = datastart + قدم_3 * صفاں;
    dataend = datalimit - قدم_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(ناپ _sz, صحيح _type, فراغ* انکڑے_3, size_t قدم_3)
    : flags(MAGIC_VAL + (_type & TYPE_نقاب)), dims(2), صفاں(_sz.اچائی), خانے_گنتی(_sz.چوڑائی),
      انکڑے((uchar*)انکڑے_3), datastart((uchar*)انکڑے_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(کل() == 0 || انکڑے != عدم);

    size_t esz = CV_ELEM_ناپ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = خانے_گنتی*esz;
    إذا( قدم_3 == AUTO_قدم || (قدم_3 < minstep && صفاں == 1))
    {
        قدم_3 = minstep;
    }
    وإلا
    {
        CV_CheckGE(قدم_3, minstep, "");

        إذا (قدم_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_قدم, "Step must be a multiple of esz1");
        }
    }
    size[0] = صفاں;
    size[1] = خانے_گنتی;
    قدم[0] = قدم_3;
    قدم[1] = esz;
    datalimit = datastart + قدم_3*صفاں;
    dataend = datalimit - قدم_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::عامل = (ثابت Mat& m)
{
    إذا( هذا != &m )
    {
        إذا( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        صفاں = m.صفاں;
        خانے_گنتی = m.خانے_گنتی;
        size = m.size;
        قدم = m.قدم;
        انکڑے = m.انکڑے;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    أرجع *هذا;
}

Mat Mat::clone() ثابت
{
    Mat m;
    copy_منزل(m);
    أرجع m;
}

فراغ Mat::سونپو_منزل( Mat& m, صحيح _type ) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_منزل(m, _type);
}

فراغ Mat::create(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type)
{
    _type &= TYPE_نقاب;
    إذا( dims <= 2 && صفاں == صفاں_3 && خانے_گنتی == خانے_گنتی_3 && type() == _type && انکڑے )
        أرجع;
    صحيح sz[] = {صفاں_3, خانے_گنتی_3};
    create(2, sz, _type);
}

فراغ Mat::create(ناپ _sz, صحيح _type)
{
    create(_sz.اچائی, _sz.چوڑائی, _type);
}

فراغ Mat::create_Same_ناپ(داخلہ_Array m, صحيح type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

فراغ Mat::fit(صحيح _dims, ثابت صحيح* _sizes, صحيح _type)
{
    size_t old_کل_بائٹاں = u ? u->size : 0;
    size_t esz = CV_ELEM_ناپ(_type), new_کل = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        new_کل *= _sizes[i];
    size_t new_کل_بائٹاں = new_کل*esz;
    إذا (new_کل_بائٹاں > 0 && (!isContinuous() ||
                              new_کل_بائٹاں > old_کل_بائٹاں ||
                              انکڑے != datastart)) {
        create(_dims, _sizes, _type);
    } وإلا {
        flags = (flags & ~Mat::TYPE_نقاب) | CV_MAT_TYPE(_type);
        صحيح dummy_ناپ = 0;
        سیٹ_ناپ(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ناپ), خالی_سوچک, سچ);
        finalizeHdr(*هذا);
    }
}

فراغ Mat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type)
{
    fit((صحيح)_shape.size(), _shape.انکڑے(), _type);
}

فراغ Mat::fit(ثابت MatShape& _shape, صحيح _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ Mat::fit(std::initializer_list<صحيح> _shape, صحيح _type)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح new_ndims = (صحيح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

فراغ Mat::fit(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type)
{
    _type &= TYPE_نقاب;
    صحيح sz[] = {صفاں_3, خانے_گنتی_3};
    fit(2, sz, _type);
}

فراغ Mat::fit(ناپ _sz, صحيح _type)
{
    fit(_sz.اچائی, _sz.چوڑائی, _type);
}

فراغ Mat::fit_Same_ناپ(داخلہ_Array m, صحيح _type)
{
    fit(m.shape(), _type);
}

فراغ Mat::addref()
{
    إذا( u )
        CV_XADD(&u->refcount, 1);
}

فراغ Mat::release()
{
    إذا( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = عدم;
    datastart = dataend = datalimit = انکڑے = 0;
    لكل(صحيح i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    dims = صفاں = خانے_گنتی = 0;
    size.clear();
}

size_t Mat::step1(صحيح i) ثابت
{
    أرجع قدم.p[i] / elemSize1();
}

منطقي Mat::empty() ثابت
{
    أرجع انکڑے == 0 || کل() == 0;
}

size_t Mat::کل() ثابت
{
    إذا( dims <= 2 )
        أرجع (size_t)صفاں * خانے_گنتی;
    size_t p = 1;
    لكل( صحيح i = 0; i < dims; i++ )
        p *= size[i];
    أرجع p;
}

size_t Mat::کل(صحيح شروع_Dim, صحيح سرا_Dim) ثابت
{
    CV_Assert( 0 <= شروع_Dim && شروع_Dim <= سرا_Dim);
    size_t p = 1;
    صحيح سرا_Dim_2 = سرا_Dim <= dims ? سرا_Dim : dims;
    لكل( صحيح i = شروع_Dim; i < سرا_Dim_2; i++ )
        p *= size[i];
    أرجع p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), صفاں(m.صفاں), خانے_گنتی(m.خانے_گنتی), انکڑے(m.انکڑے),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), قدم(m.قدم)
{
    m.flags = MAGIC_VAL; m.dims = m.صفاں = m.خانے_گنتی = 0;
    m.size.clear(); m.قدم.clear();
    m.انکڑے = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
}


Mat& Mat::عامل = (Mat&& m)
{
    إذا (هذا == &m)
      أرجع *هذا;

    release();
    flags = m.flags; dims = m.dims; صفاں = m.صفاں; خانے_گنتی = m.خانے_گنتی; انکڑے = m.انکڑے;
    size = m.size; قدم = m.قدم;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.صفاں = m.خانے_گنتی = 0;
    m.size.clear(); m.قدم.clear();
    m.انکڑے = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
    أرجع *هذا;
}


فراغ Mat::create(صحيح d0, ثابت صحيح* _sizes, صحيح _type)
{
    صحيح sz1 = 1, d = d0;
    صحيح i;
    إذا (d == 0) {
        d = 1;
        _sizes = (ثابت صحيح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    إذا( انکڑے && d == dims && _type == type() )
    {
        لكل( i = 0; i < d; i++ )
            إذا( size[i] != _sizes[i] )
                اكسر;
        إذا( i == d )
            أرجع;
    }

    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    إذا (_sizes == (هذا->size.p))
    {
        لكل(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    إذا( d == 0 )
        أرجع;
    flags = (_type & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    سیٹ_ناپ(*هذا, d, _sizes, 0, سچ);

    إذا( کل() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        إذا(!a)
            a = a0;
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        امسك (...)
        {
            إذا (a == a0)
                ارم;
            u = a0->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( قدم[dims-1] == (size_t)CV_ELEM_ناپ(flags) );
    }

    addref();
    finalizeHdr(*هذا);
    size.dims = dims = d0;
}

فراغ Mat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type)
{
    create((صحيح)_sizes.size(), _sizes.انکڑے(), _type);
}

فراغ Mat::create(ثابت MatShape& _shape, صحيح _type)
{
    إذا (_shape.dims < 0) {
        release();
        أرجع;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ Mat::create(std::initializer_list<صحيح> _shape, صحيح _type)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح new_ndims = (صحيح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

فراغ Mat::copy_ناپ(ثابت Mat& m)
{
    dims = m.dims;
    خانے_گنتی = m.خانے_گنتی;
    صفاں = m.صفاں;
    size = m.size;
    قدم = m.قدم;
}

فراغ Mat::deallocate()
{
    إذا(u)
    {
        UMat_انکڑے* u_ = u;
        u = عدم;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ثابت Mat& m, ثابت Range& صف_Range_3, ثابت Range& خانہ_Range_3)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (صف_Range_3 == Range::all() ||
                               صف_Range_3 == Range(0, m.صفاں))));
    إذا( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = صف_Range_3;
        rs[1] = خانہ_Range_3;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    حاول
    {
        إذا( صف_Range_3 != Range::all() && صف_Range_3 != Range(0,صفاں) )
        {
            CV_Assert( 0 <= صف_Range_3.شروع && صف_Range_3.شروع <= صف_Range_3.end
                       && صف_Range_3.end <= m.صفاں );
            size[0] = صفاں = صف_Range_3.size();
            انکڑے += قدم*صف_Range_3.شروع;
            flags |= SUBMATRIX_FLAG;
        }

        إذا( خانہ_Range_3 != Range::all() && خانہ_Range_3 != Range(0,خانے_گنتی) )
        {
            CV_Assert( 0 <= خانہ_Range_3.شروع && خانہ_Range_3.شروع <= خانہ_Range_3.end
                       && خانہ_Range_3.end <= m.خانے_گنتی );
            size[dims > 1] = خانے_گنتی = خانہ_Range_3.size();
            انکڑے += خانہ_Range_3.شروع*elem_ناپ();
            flags |= SUBMATRIX_FLAG;
        }
    }
    امسك(...)
    {
        release();
        ارم;
    }

    updateContinuityFlag();

    إذا( صفاں <= 0 || خانے_گنتی <= 0 )
    {
        release();
        صفاں = خانے_گنتی = 0;
    }
}


Mat::Mat(ثابت Mat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), صفاں(roi.اچائی), خانے_گنتی(roi.چوڑائی),
    انکڑے(m.انکڑے + roi.y*m.قدم[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ناپ(flags);
    انکڑے += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی && roi.x + roi.چوڑائی <= m.خانے_گنتی &&
              0 <= roi.y && 0 <= roi.اچائی && roi.y + roi.اچائی <= m.صفاں );
    إذا( roi.چوڑائی < m.خانے_گنتی || roi.اچائی < m.صفاں )
        flags |= SUBMATRIX_FLAG;

    size[0] = صفاں;
    size[1] = خانے_گنتی;
    قدم[0] = m.قدم[0];
    قدم[1] = esz;
    updateContinuityFlag();

    addref();
    إذا( صفاں <= 0 || خانے_گنتی <= 0 )
    {
        صفاں = خانے_گنتی = 0;
        release();
    }
}


Mat::Mat(صحيح _dims, ثابت صحيح* _sizes, صحيح _type, فراغ* انکڑے_3, ثابت size_t* قدماں_2)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = انکڑے = (uchar*)انکڑے_3;
    إذا (_dims > 0 || انکڑے_3 != خالی_سوچک)
        سیٹ_ناپ(*هذا, _dims, _sizes, قدماں_2, سچ);
    finalizeHdr(*هذا);
}


Mat::Mat(ثابت std::vector<صحيح>& _sizes, صحيح _type, فراغ* انکڑے_3, ثابت size_t* قدماں_2)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = انکڑے = (uchar*)انکڑے_3;
    صحيح _dims = (صحيح)_sizes.size();
    إذا (_dims > 0 || انکڑے_3 != خالی_سوچک)
        سیٹ_ناپ(*هذا, _dims, _sizes.انکڑے(), قدماں_2, سچ);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, فراغ* انکڑے_3, ثابت size_t* قدماں_2)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = انکڑے = (uchar*)انکڑے_3;
    إذا (_shape.dims >= 0) {
        سیٹ_ناپ(*هذا, _shape.dims, _shape.p, قدماں_2, سچ);
    }
    وإلا {
        CV_Assert(!انکڑے);
    }
    finalizeHdr(*هذا);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, فراغ* انکڑے_3, ثابت size_t* قدماں_2)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح _dims = (صحيح)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = انکڑے = (uchar*)انکڑے_3;
    إذا (_dims > 0 || انکڑے_3 != خالی_سوچک)
        سیٹ_ناپ(*هذا, _dims, new_shape, قدماں_2, سچ);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت Mat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert(ranges);
    لكل( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        إذا( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            انکڑے += r.شروع*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفاں = d == 2 ? size[0] : 1;
        خانے_گنتی = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ثابت Mat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), انکڑے(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            انکڑے += r.شروع*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفاں = d == 2 ? size[0] : 1;
        خانے_گنتی = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(صحيح d) ثابت
{
    CV_Assert( dims == 2 );
    Mat m = *هذا;
    size_t esz = elem_ناپ();
    صحيح len;

    إذا( d >= 0 )
    {
        len = std::min(خانے_گنتی - d, صفاں);
        m.انکڑے += esz*d;
    }
    وإلا
    {
        len = std::min(صفاں + d, خانے_گنتی);
        m.انکڑے -= قدم[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.صفاں = len;
    m.size[1] = m.خانے_گنتی = 1;
    m.قدم[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    إذا( size() != ناپ(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}


فراغ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    إذا( isSubmatrix() )
        *هذا = صف_Range_2(0, size.p[0] - (صحيح)nelems);
    وإلا
    {
        size.p[0] -= (صحيح)nelems;
        إذا (dims == 2)
            صفاں = size.p[0];
        وإلا إذا (dims == 1)
            خانے_گنتی = size.p[0];
        dataend -= nelems*قدم.p[0];
    }
}


فراغ Mat::ڈالو_پٹھ(ثابت فراغ* elem)
{
    size_t r = size.p[0];
    إذا( isSubmatrix() || dataend + قدم.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_ناپ();
    حافظہ_نقل(انکڑے + r*قدم.p[0], elem, esz);
    size.p[0] = صحيح(r + 1);
    إذا (dims == 2)
        صفاں = size.p[0];
    وإلا إذا (dims == 1)
        خانے_گنتی = size.p[0];
    dataend += قدم.p[0];
    uint64 tsz = size.p[0];
    لكل( صحيح i = 1; i < dims; i++ )
        tsz *= size.p[i];
    إذا( esz < قدم.p[0] || tsz != (uint64)(صحيح)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


فراغ Mat::reserve(size_t nelems)
{
    ثابت size_t MIN_ناپ = 64;

    CV_Assert( (صحيح)nelems >= 0 );
    إذا( !isSubmatrix() && قدم.p[0] != 0 && انکڑے + قدم.p[0]*nelems <= datalimit )
        أرجع;

    صحيح r = size.p[0];
    إذا( (size_t)r >= nelems)
        أرجع;

    MatShape newsize = size;
    size_t esz = elem_ناپ();
    newsize.dims = std::max(newsize.dims, 1);
    إذا (dims > 1 && r == 0 && قدم.p[0] == 0) {
        قدم.p[0] = size.p[1] * (dims == 2 ? esz : قدم.p[1]);
    }
    newsize.p[0] = (صحيح)nelems;
    size_t newbytes = newsize.کل() * esz;
    إذا (newbytes < MIN_ناپ) {
        newsize.p[0] = 1;
        newsize.p[0] = صحيح(MIN_ناپ / newsize.کل() / esz);
    }

    Mat m(newsize, type());
    إذا( r > 0)
    {
        Mat mpart = m.صف_Range_2(0, r);
        copy_منزل(mpart);
    }

    *هذا = m;
    size.p[0] = r;

    إذا (dims == 2)
        صفاں = size.p[0];
    وإلا إذا (dims == 1)
        خانے_گنتی = size.p[0];
    dataend = انکڑے + قدم.p[0]*r;
}


فراغ Mat::reserve_بفر(size_t nbytes)
{
    size_t esz = 1;
    صحيح mtype = CV_8UC1;
    إذا (!empty())
    {
        إذا (!isSubmatrix() && انکڑے + nbytes <= dataend)//Should it be datalimit?
            أرجع;
        esz = elem_ناپ();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#إذا SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(ودھ_توں_ودھ_عدد)*size_t(ودھ_توں_ودھ_عدد));
    صحيح newrows = nelems > size_t(ودھ_توں_ودھ_عدد) ? nelems > 0x400*size_t(ودھ_توں_ودھ_عدد) ? nelems > 0x100000 * size_t(ودھ_توں_ودھ_عدد) ? nelems > 0x40000000 * size_t(ودھ_توں_ودھ_عدد) ?
                  size_t(ودھ_توں_ودھ_عدد) : 0x40000000 : 0x100000 : 0x400 : 1;
#وإلا
    صحيح newrows = nelems > size_t(ودھ_توں_ودھ_عدد) ? 2 : 1;
#جے_انت
    صحيح newcols = (صحيح)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


فراغ Mat::resize(size_t nelems)
{
    صحيح r = size.p[0];
    إذا( r == (صحيح)nelems )
        أرجع;
    CV_Assert( (صحيح)nelems >= 0 );

    إذا( isSubmatrix() || انکڑے + قدم.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (صحيح)nelems;
    إذا (dims == 2)
        صفاں = size.p[0];
    وإلا إذا (dims == 1)
        خانے_گنتی = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)قدم.p[0];

    //updateContinuityFlag(*this);
}


فراغ Mat::resize(size_t nelems, ثابت Scalar& s)
{
    صحيح save_صفاں = size.p[0];
    resize(nelems);

    إذا( size.p[0] > save_صفاں )
    {
        Mat part = صف_Range_2(save_صفاں, size.p[0]);
        part = s;
    }
}

فراغ Mat::push_back(ثابت Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    إذا( delta == 0 )
        أرجع;
    إذا( هذا == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        أرجع;
    }
    إذا( !انکڑے )
    {
        *هذا = elems.clone();
        أرجع;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    منطقي eq = size == elems.size;
    size.p[0] = صحيح(r);
    إذا( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    إذا( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_ناپ();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : قدم.p[1]);
    size_t step0 = قدم.p[0];
    إذا (step0 < minstep) {
        إذا (size.p[0] <= 1)
            قدم.p[0] = step0 = minstep;
    }

    إذا( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += صحيح(delta);
    إذا (dims == 2)
        صفاں = size.p[0];
    وإلا إذا (dims == 1)
        خانے_گنتی = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    إذا( isContinuous() && elems.isContinuous() )
        حافظہ_نقل(انکڑے + r*step0, elems.انکڑے, elems.کل()*elems.elem_ناپ());
    وإلا
    {
        Mat part = صف_Range_2(صحيح(r), صحيح(r + delta));
        elems.copy_منزل(part);
    }
}


فراغ Mat::locateROI( ناپ& سارا_ناپ, نقطہ& ofs ) ثابت
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    size_t esz = elem_ناپ(), minstep;
    ptrdiff_t delta1 = انکڑے - datastart, delta2 = dataend - datastart;

    إذا( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/قدم[0]);
        ofs.x = (صحيح)((delta1 - قدم[0]*ofs.y)/esz);
        CV_DbgAssert( انکڑے == datastart + ofs.y*قدم[0] + ofs.x*esz );
    }
    minstep = (ofs.x + خانے_گنتی)*esz;
    سارا_ناپ.اچائی = (صحيح)((delta2 - minstep)/قدم[0] + 1);
    سارا_ناپ.اچائی = std::max(سارا_ناپ.اچائی, ofs.y + صفاں);
    سارا_ناپ.چوڑائی = (صحيح)((delta2 - قدم*(سارا_ناپ.اچائی-1))/esz);
    سارا_ناپ.چوڑائی = std::max(سارا_ناپ.چوڑائی, ofs.x + خانے_گنتی);
}

Mat& Mat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    ناپ سارا_ناپ; نقطہ ofs;
    size_t esz = elem_ناپ();
    locateROI( سارا_ناپ, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), سارا_ناپ.اچائی), row2 = std::max(0, std::min(ofs.y + صفاں + dbottom, سارا_ناپ.اچائی));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), سارا_ناپ.چوڑائی), col2 = std::max(0, std::min(ofs.x + خانے_گنتی + dright, سارا_ناپ.چوڑائی));
    إذا(row1 > row2)
        std::swap(row1, row2);
    إذا(col1 > col2)
        std::swap(col1, col2);

    إذا (dims == 1) {
        انکڑے += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        خانے_گنتی = col2 - col1;
        size.p[0] = خانے_گنتی;
    } وإلا {
        انکڑے += (row1 - ofs.y)*(std::ptrdiff_t)قدم + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        صفاں = row2 - row1; خانے_گنتی = col2 - col1;
        size.p[0] = صفاں; size.p[1] = خانے_گنتی;
        updateContinuityFlag();
    }
    أرجع *هذا;
}

Mat Mat::reshape(صحيح new_cn, صحيح new_صفاں) ثابت
{
    صحيح cn = channels();
    Mat hdr = *هذا;

    إذا( new_cn == 0 )
        new_cn = cn;

    إذا( dims > 2 )
    {
        إذا( new_صفاں == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_کھسکاؤ);
            hdr.قدم[dims-1] = CV_ELEM_ناپ(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            أرجع hdr;
        }
        CV_Assert( new_صفاں > 0 );
        صحيح sz[] = { new_صفاں, (صحيح)(کل()*cn/new_صفاں) };
        أرجع reshape(new_cn, 2, sz);
    }

    صحيح کل_چوڑائی = خانے_گنتی * cn;

    إذا( (new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفاں == 0 )
        new_صفاں = صفاں * کل_چوڑائی / new_cn;

    إذا( new_صفاں != 0 && new_صفاں != صفاں )
    {
        صحيح کل_ناپ_2 = کل_چوڑائی * صفاں;
        إذا( !isContinuous() )
            CV_Error( cv::Error::Bad_قدم,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        کل_چوڑائی = کل_ناپ_2 / new_صفاں;

        إذا( کل_چوڑائی * new_صفاں != کل_ناپ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.صفاں = new_صفاں;
        hdr.قدم[0] = کل_چوڑائی * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.صفاں = صفاں;
        إذا (dims <= 1)
            hdr.قدم[0] = خانے_گنتی * CV_ELEM_ناپ(flags);
    }

    صحيح new_چوڑائی = کل_چوڑائی / new_cn;

    إذا( new_چوڑائی * new_cn != کل_چوڑائی )
        CV_Error( cv::Error::Bad_عدد_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.خانے_گنتی = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_کھسکاؤ);
    hdr.قدم[1] = CV_ELEM_ناپ(hdr.flags);
    أرجع hdr;
}

Mat Mat::reshape(صحيح _cn, صحيح _newndims, ثابت صحيح* _newsz) ثابت
{
    إذا(_newndims == dims)
    {
        إذا(_newsz == 0)
            أرجع reshape(_cn);
        إذا(_newndims == 2)
            أرجع reshape(_cn, _newsz[0]);
    }

    إذا (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        إذا (_cn == 0)
            _cn = هذا->channels();
        وإلا
            CV_Assert(_cn <= CV_CN_MAX);

        size_t کل_elem1_ref = هذا->کل() * هذا->channels();
        size_t کل_elem1 = _cn;

        Auto_بفر<صحيح, 4> newsz_buf( (size_t)_newndims );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            إذا (_newsz[i] >= 0) {
                إذا (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                کل_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                إذا (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ناپ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        إذا (m1_idx >= 0) {
            إذا (کل_elem1 == 0) {
                CV_Assert(کل_elem1_ref == 0);
                کل_elem1 = 1;
            }
            CV_Assert(کل_elem1_ref % کل_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(کل_elem1_ref / کل_elem1);
        } وإلا إذا (کل_elem1 != کل_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((_cn-1) << CV_CN_کھسکاؤ);
        سیٹ_ناپ(hdr, _newndims, newsz_buf.انکڑے(), عدم, سچ);

        أرجع hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(صحيح _cn, ثابت std::vector<صحيح>& _newshape) ثابت
{
    صحيح newdims = (صحيح)_newshape.size();
    إذا(newdims == 0 && empty())
    {
        أرجع *هذا;
    }

    أرجع reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(صحيح _cn, ثابت MatShape& _newshape) ثابت
{
    إذا (_newshape.dims < 0) {
        صحيح newshape[] = {0};
        أرجع reshape(_cn, 1, newshape);
    }
    أرجع reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(صحيح _cn, std::initializer_list<صحيح> newshape_) ثابت
{
    صحيح newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    خودکار it = newshape_.begin();
    لكل (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    أرجع reshape(_cn, (صحيح)newshape_dims, newshape);
}

Mat Mat::reinterpret(صحيح type) ثابت
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_ناپ(هذا->type()) == CV_ELEM_ناپ(type));
    Mat m = *هذا;
    m.flags = (m.flags & ~CV_MAT_TYPE_نقاب) | type;
    m.updateContinuityFlag();
    أرجع m;
}

Mat Mat::diag(ثابت Mat& d)
{
    CV_Assert( d.خانے_گنتی == 1 || d.صفاں == 1 );
    صحيح len = d.صفاں + d.خانے_گنتی - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    إذا( d.خانے_گنتی == 1 )
        d.copy_منزل(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح Mat::check_سمتیہ(صحيح _elemChannels, صحيح ڈونگھائی_2, منطقي _requireContinuous) ثابت
{
    أرجع انکڑے && (ڈونگھائی() == ڈونگھائی_2 || ڈونگھائی_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((صفاں == 1 || خانے_گنتی == 1) && channels() == _elemChannels) ||
                        (خانے_گنتی == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || قدم.p[1] == قدم.p[2]*size.p[2])))
    ? (صحيح)(کل()*channels()/_elemChannels) : -1;
}


ساكن مضمن ناپ get_Continuous_ناپ(صحيح flags, صحيح خانے_گنتی, صحيح صفاں, صحيح چوڑائی_Scale)
{
    int64 sz = (int64)خانے_گنتی * صفاں * چوڑائی_Scale;
    منطقي has_int_حد_توں_ودھ = sz >= ودھ_توں_ودھ_عدد;
    منطقي isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    أرجع (isContiguous && !has_int_حد_توں_ودھ)
            ? ناپ((صحيح)sz, 1)
            : ناپ(خانے_گنتی * چوڑائی_Scale, صفاں);
}

ناپ getContinuousSize2D(Mat& m1, صحيح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    أرجع get_Continuous_ناپ(m1.flags,
                              m1.خانے_گنتی, m1.صفاں, چوڑائی_Scale);
}
ناپ getContinuousSize2D(Mat& m1, Mat& m2, صحيح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ثابت ناپ sz1 = m1.size();
    إذا (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t کل_sz = m1.کل();
        CV_CheckEQ(کل_sz, m2.کل(), "");
        منطقي is_m1_سمتیہ = m1.خانے_گنتی == 1 || m1.صفاں == 1;
        منطقي is_m2_سمتیہ = m2.خانے_گنتی == 1 || m2.صفاں == 1;
        CV_Assert(is_m1_سمتیہ); CV_Assert(is_m2_سمتیہ);
        صحيح کل = (صحيح)کل_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_حد_توں_ودھ = ((int64)کل_sz * چوڑائی_Scale) >= ودھ_توں_ودھ_عدد;
        إذا (isContiguous && !has_int_حد_توں_ودھ)
            کل = 1; // vector-row
        m1 = m1.reshape(0, کل);
        m2 = m2.reshape(0, کل);
        CV_Assert(m1.خانے_گنتی == m2.خانے_گنتی && m1.صفاں == m2.صفاں);
        أرجع ناپ(m1.خانے_گنتی * چوڑائی_Scale, m1.صفاں);
    }
    أرجع get_Continuous_ناپ(m1.flags & m2.flags,
                              m1.خانے_گنتی, m1.صفاں, چوڑائی_Scale);
}

ناپ getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحيح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ثابت ناپ sz1 = m1.size();
    إذا (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t کل_sz = m1.کل();
        CV_CheckEQ(کل_sz, m2.کل(), "");
        CV_CheckEQ(کل_sz, m3.کل(), "");
        منطقي is_m1_سمتیہ = m1.خانے_گنتی == 1 || m1.صفاں == 1;
        منطقي is_m2_سمتیہ = m2.خانے_گنتی == 1 || m2.صفاں == 1;
        منطقي is_m3_سمتیہ = m3.خانے_گنتی == 1 || m3.صفاں == 1;
        CV_Assert(is_m1_سمتیہ); CV_Assert(is_m2_سمتیہ); CV_Assert(is_m3_سمتیہ);
        صحيح کل = (صحيح)کل_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_حد_توں_ودھ = ((int64)کل_sz * چوڑائی_Scale) >= ودھ_توں_ودھ_عدد;
        إذا (isContiguous && !has_int_حد_توں_ودھ)
            کل = 1; // vector-row
        m1 = m1.reshape(0, کل);
        m2 = m2.reshape(0, کل);
        m3 = m3.reshape(0, کل);
        CV_Assert(m1.خانے_گنتی == m2.خانے_گنتی && m1.صفاں == m2.صفاں && m1.خانے_گنتی == m3.خانے_گنتی && m1.صفاں == m3.صفاں);
        أرجع ناپ(m1.خانے_گنتی * چوڑائی_Scale, m1.صفاں);
    }
    أرجع get_Continuous_ناپ(m1.flags & m2.flags & m3.flags,
                              m1.خانے_گنتی, m1.صفاں, چوڑائی_Scale);
}

} // cv::
