// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "bufferpool.impl.hpp"

নামাকাশ cv {

std::string layoutToString(তথ্য_Layout layout)
{
    ফেরত
        layout == তথ্য_LAYOUT_ND ? "ND" :
        layout == তথ্য_LAYOUT_NCHW ? "NCHW" :
        layout == তথ্য_LAYOUT_NHWC ? "NHWC" :
        layout == তথ্য_LAYOUT_BLOCK ? "BLOCK" :
        layout == তথ্য_LAYOUT_NCDHW ? "NCDHW" :
        layout == তথ্য_LAYOUT_NDHWC ? "NDHWC" :
        layout == তথ্য_LAYOUT_PLANAR ? "PLANAR" :
        layout == তথ্য_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

যৌক্তিক operator == (ধ্রুবক MatShape& size1, ধ্রুবক MatShape& size2)
{
    যদি (size1.dims != size2.dims)
        ফেরত মিথ্যা;
    যদি (size1.layout != size2.layout &&
        size1.layout != তথ্য_LAYOUT_UNKNOWN &&
        size2.layout != তথ্য_LAYOUT_UNKNOWN)
        ফেরত মিথ্যা;
    যদি (size1.layout == তথ্য_LAYOUT_BLOCK &&
        size2.layout == তথ্য_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ফেরত মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < size1.dims; i++) {
        যদি (size1.p[i] != size2.p[i])
            ফেরত মিথ্যা;
    }
    ফেরত সত্য;
}

যৌক্তিক operator != (ধ্রুবক MatShape& size1, ধ্রুবক MatShape& size2)
{
    ফেরত !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ফেরত MatShape(0);
}

শূন্য MatShape::clear()
{
    dims = -1;
    layout = তথ্য_LAYOUT_UNKNOWN;
    C = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

শূন্য MatShape::resize(size_t new_আকার, পূর্ণসংখ্যা value)
{
    CV_Assert(new_আকার < (size_t)MAX_DIMS);
    পূর্ণসংখ্যা old_dims = std::max(dims, 0);
    dims = (পূর্ণসংখ্যা)new_আকার;
    জন্য (পূর্ণসংখ্যা i = old_dims; i < dims; i++)
        p[i] = value;
}

শূন্য MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

শূন্য MatShape::assign(size_t new_আকার, পূর্ণসংখ্যা value)
{
    CV_Assert(new_আকার < (size_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)new_আকার;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
        p[i] = value;
}

শূন্য MatShape::assign(পূর্ণসংখ্যা new_আকার, পূর্ণসংখ্যা value)
{
    assign((size_t)new_আকার, value);
}

শূন্য MatShape::assign(ধ্রুবক পূর্ণসংখ্যা* begin, ধ্রুবক পূর্ণসংখ্যা* end)
{
    assign_(begin, end);
}

শূন্য MatShape::assign_(ধ্রুবক পূর্ণসংখ্যা* begin, ধ্রুবক পূর্ণসংখ্যা* end)
{
    ptrdiff_t new_আকার = end - begin;
    CV_Assert(0 <= new_আকার && new_আকার < (ptrdiff_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)new_আকার;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
        p[i] = begin[i];
}

পূর্ণসংখ্যা* MatShape::begin() { ফেরত p; }
ধ্রুবক পূর্ণসংখ্যা* MatShape::begin() ধ্রুবক { ফেরত p; }
পূর্ণসংখ্যা* MatShape::end() { ফেরত p + std::max(dims, 0); }
ধ্রুবক পূর্ণসংখ্যা* MatShape::end() ধ্রুবক { ফেরত p + std::max(dims, 0); }
পূর্ণসংখ্যা& MatShape::back() { ফেরত p[std::max(dims-1, 0)]; }
ধ্রুবক পূর্ণসংখ্যা& MatShape::back() ধ্রুবক { ফেরত p[std::max(dims-1, 0)]; }

শূন্য MatShape::push_back(পূর্ণসংখ্যা value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

শূন্য MatShape::emplace_back(পূর্ণসংখ্যা value)
{
    push_back(value);
}

শূন্য MatShape::insert(পূর্ণসংখ্যা* where, পূর্ণসংখ্যা value)
{
    পূর্ণসংখ্যা old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    জন্য (পূর্ণসংখ্যা i = old_dims-1; i >= (পূর্ণসংখ্যা)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

শূন্য MatShape::insert(পূর্ণসংখ্যা* where, size_t গুনতি_2, পূর্ণসংখ্যা value)
{
    পূর্ণসংখ্যা old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+গুনতি_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (পূর্ণসংখ্যা)(old_dims+গুনতি_2);
    জন্য (পূর্ণসংখ্যা i = old_dims-1; i >= (পূর্ণসংখ্যা)ofs; i--)
        p[i+গুনতি_2] = p[i];
    জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)গুনতি_2; i++)
        p[i+ofs] = value;
}

শূন্য MatShape::insert(পূর্ণসংখ্যা* where, পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা value)
{
    insert(where, (size_t)গুনতি_2, value);
}

শূন্য MatShape::insert(পূর্ণসংখ্যা* where, ধ্রুবক পূর্ণসংখ্যা* begin, ধ্রুবক পূর্ণসংখ্যা* end)
{
    ঢোকাও(where, begin, end);
}

শূন্য MatShape::ঢোকাও(পূর্ণসংখ্যা* where, ধ্রুবক পূর্ণসংখ্যা* begin, ধ্রুবক পূর্ণসংখ্যা* end)
{
    পূর্ণসংখ্যা old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (পূর্ণসংখ্যা)(old_dims+delta);
    জন্য (পূর্ণসংখ্যা i = old_dims-1; i >= (পূর্ণসংখ্যা)ofs; i--)
        p[i+delta] = p[i];
    জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)delta; i++)
        p[i+ofs] = begin[i];
}

শূন্য MatShape::erase(পূর্ণসংখ্যা* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    যদি (ofs == dims)
        ফেরত;
    dims--;
    জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::যোগ() ধ্রুবক
{
    size_t ফলাফল = 1;
    যদি (dims < 0)
        ফেরত 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
        ফলাফল *= p[i];
    ফেরত ফলাফল;
}

std::vector<পূর্ণসংখ্যা> MatShape::vec() ধ্রুবক
{
    যদি (dims < 0)
        ফেরত std::vector<পূর্ণসংখ্যা>(1, 0);
    ফেরত std::vector<পূর্ণসংখ্যা>(p, p + dims);
}

std::string MatShape::str() ধ্রুবক
{
    std::stringstream sstrm;
    যদি (empty()) {
        sstrm << "<empty>";
    } অন্যথা যদি (dims == 0) {
        sstrm << "<scalar>";
    } অন্যথা {
        sstrm << "[";
        জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ফেরত sstrm.str();
}

স্থির শূন্য finalizeBlockLayout(MatShape& size, পূর্ণসংখ্যা C=0)
{
    যদি (size.layout == তথ্য_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        পূর্ণসংখ্যা C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } অন্যথা {
        size.C = 0;
    }
    জন্য (পূর্ণসংখ্যা i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    যদি (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ধ্রুবক পূর্ণসংখ্যা* আকার_4, তথ্য_Layout layout_, পূর্ণসংখ্যা C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)dims_;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        p[i] = আকার_4 ? আকার_4[i] : 0;
    }
    finalizeBlockLayout(*এই, C_);
}

MatShape::MatShape(size_t dims_, পূর্ণসংখ্যা value, তথ্য_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)dims_;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*এই, 0);
}

MatShape::MatShape(std::initializer_list<পূর্ণসংখ্যা> shape)
{
    layout = তথ্য_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)shape.size();
    auto it = shape.begin();
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*এই, 0);
}

MatShape::MatShape(পূর্ণসংখ্যা dims_, পূর্ণসংখ্যা value, তথ্য_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*এই, 0);
}

MatShape::MatShape(ধ্রুবক std::vector<পূর্ণসংখ্যা>& shape_, তথ্য_Layout layout_, পূর্ণসংখ্যা C_)
{
    layout = layout_;
    size_t shape_আকার = shape_.size();
    CV_Assert(shape_আকার < (size_t)MAX_DIMS);
    dims = (পূর্ণসংখ্যা)shape_আকার;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*এই, C_);
}

MatShape::MatShape(ধ্রুবক পূর্ণসংখ্যা* begin, ধ্রুবক পূর্ণসংখ্যা* end, তথ্য_Layout layout_, পূর্ণসংখ্যা C_)
{
    layout = layout_;
    ptrdiff_t shape_আকার = end - begin;
    CV_Assert(0 <= shape_আকার && shape_আকার < MAX_DIMS);
    dims = (পূর্ণসংখ্যা)shape_আকার;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*এই, C_);
}

MatShape::MatShape(ধ্রুবক MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    জন্য (পূর্ণসংখ্যা i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ধ্রুবক MatShape& shape)
{
    যদি (এই != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        জন্য (পূর্ণসংখ্যা i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ফেরত *এই;
}

যৌক্তিক MatShape::hasSymbols() ধ্রুবক
{
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++) {
        যদি (p[i] < 0)
            ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

পূর্ণসংখ্যা MatShape::channels() ধ্রুবক
{
    CV_Assert(layout == তথ্য_LAYOUT_BLOCK || layout == তথ্য_LAYOUT_NCHW || layout == তথ্য_LAYOUT_NHWC);
    ফেরত layout == তথ্য_LAYOUT_BLOCK ? C : p[layout == তথ্য_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(তথ্য_Layout newLayout, পূর্ণসংখ্যা C0) ধ্রুবক
{
    CV_Assert(layout == তথ্য_LAYOUT_BLOCK || layout == তথ্য_LAYOUT_NCHW || layout == তথ্য_LAYOUT_NHWC);
    CV_Assert(newLayout == তথ্য_LAYOUT_BLOCK || newLayout == তথ্য_LAYOUT_NCHW || newLayout == তথ্য_LAYOUT_NHWC);

    MatShape newsize = *এই;
    newsize.layout = newLayout;

    যদি (newLayout == তথ্য_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        পূর্ণসংখ্যা Corig = channels();
        newsize.C = Corig;

        যদি (layout == তথ্য_LAYOUT_NHWC) {
            জন্য (পূর্ণসংখ্যা i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != তথ্য_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } অন্যথা যদি (layout == তথ্য_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        যদি (newLayout == তথ্য_LAYOUT_NHWC) {
            জন্য (পূর্ণসংখ্যা i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == তথ্য_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } অন্যথা {
        CV_Assert_N(C0 <= 1);

        যদি (newLayout == layout)
            ফেরত newsize;

        // NHWC => NCHW
        যদি (newLayout == তথ্য_LAYOUT_NCHW) {
            জন্য (পূর্ণসংখ্যা i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } অন্যথা {
            // NCHW => NHWC
            জন্য (পূর্ণসংখ্যা i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ফেরত newsize;
}

স্থির পূর্ণসংখ্যা normalizeShapeAxis(পূর্ণসংখ্যা axis, পূর্ণসংখ্যা dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ফেরত axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
স্থির MatShape toSemantic(ধ্রুবক MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ফেরত shape.toLayout(তথ্য_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
স্থির MatShape toBlockOf(MatShape ফলাফল, ধ্রুবক MatShape& src, ধ্রুবক অক্ষর* op)
{
    যদি (ফলাফল.dims < 2 || ফলাফল.p[0] != src.p[0] || ফলাফল.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(ফলাফল.dims + 1, (পূর্ণসংখ্যা)MatShape::MAX_DIMS, "too many dimensions");
    ফলাফল.layout = তথ্য_LAYOUT_NCHW;
    ফেরত ফলাফল.toLayout(তথ্য_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ধ্রুবক std::vector<পূর্ণসংখ্যা>& axes) ধ্রুবক
{
    CV_Assert(dims >= 0);
    যদি (layout == তথ্য_LAYOUT_BLOCK)
        ফেরত toBlockOf(toSemantic(*এই).squeeze(axes), *এই, "squeeze");
    যৌক্তিক remove[MAX_DIMS] = {মিথ্যা};
    যদি (axes.empty())
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    জন্য (পূর্ণসংখ্যা a : axes)
    {
        পূর্ণসংখ্যা i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = সত্য;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ফলাফল;
    ফলাফল.dims = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
        যদি (!remove[i])
            ফলাফল.p[ফলাফল.dims++] = p[i];
    ফেরত ফলাফল;
}

MatShape MatShape::unsqueeze(ধ্রুবক std::vector<পূর্ণসংখ্যা>& axes) ধ্রুবক
{
    CV_Assert(dims >= 0);
    যদি (layout == তথ্য_LAYOUT_BLOCK)
        ফেরত toBlockOf(toSemantic(*এই).unsqueeze(axes), *এই, "unsqueeze");
    ধ্রুবক পূর্ণসংখ্যা outDims = dims + (পূর্ণসংখ্যা)axes.size();
    CV_CheckLE(outDims, (পূর্ণসংখ্যা)MAX_DIMS, "unsqueeze: too many dimensions");
    যৌক্তিক insert[MAX_DIMS] = {মিথ্যা};
    জন্য (পূর্ণসংখ্যা a : axes)
    {
        পূর্ণসংখ্যা i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = সত্য;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ফলাফল;
    ফলাফল.dims = outDims;
    জন্য (পূর্ণসংখ্যা i = 0, j = 0; i < outDims; i++)
        ফলাফল.p[i] = insert[i] ? 1 : p[j++];
    ফেরত ফলাফল;
}

MatShape MatShape::flatten(পূর্ণসংখ্যা startAxis, পূর্ণসংখ্যা endAxis) ধ্রুবক
{
    CV_Assert(dims >= 0);
    যদি (layout == তথ্য_LAYOUT_BLOCK)
        ফেরত toBlockOf(toSemantic(*এই).flatten(startAxis, endAxis), *এই, "flatten");
    যদি (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ফেরত MatShape({1});
    }
    পূর্ণসংখ্যা start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    জন্য (পূর্ণসংখ্যা i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((দ্বিগুণ)merged, (দ্বিগুণ)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape ফলাফল;
    ফলাফল.dims = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
    {
        যদি (i == start)
            ফলাফল.p[ফলাফল.dims++] = (পূর্ণসংখ্যা)merged;
        অন্যথা যদি (i < start || i > end)
            ফলাফল.p[ফলাফল.dims++] = p[i];
    }
    ফেরত ফলাফল;
}

MatShape MatShape::reshape(ধ্রুবক MatShape& newShape, যৌক্তিক allowZero) ধ্রুবক
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    যদি (layout == তথ্য_LAYOUT_BLOCK)
        ফেরত toBlockOf(toSemantic(*এই).reshape(newShape, allowZero), *এই, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape ফলাফল;
    ফলাফল.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, ফলাফল.p);
    পূর্ণসংখ্যা inferred = -1;
    int64 known = 1;
    জন্য (পূর্ণসংখ্যা i = 0; i < ফলাফল.dims; i++)
    {
        পূর্ণসংখ্যা& sz = ফলাফল.p[i];
        যদি (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            চালু;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        যদি (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ধ্রুবক int64 যোগ_3 = (int64)যোগ();
    যদি (inferred >= 0)
    {
        CV_Check((দ্বিগুণ)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((দ্বিগুণ)যোগ_3, যোগ_3 % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((দ্বিগুণ)(যোগ_3/known), (দ্বিগুণ)INT_MAX, "reshape: the result is too big");
        ফলাফল.p[inferred] = (পূর্ণসংখ্যা)(যোগ_3/known);
    }
    অন্যথা যদি (known != যোগ_3)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)যোগ_3));
    ফেরত ফলাফল;
}

MatShape MatShape::expand(ধ্রুবক MatShape& another) ধ্রুবক
{
    যদি (dims == 0)
        ফেরত another;
    যদি (another.dims == 0)
        ফেরত *এই;

    যদি ((layout == তথ্য_LAYOUT_NCHW || layout == তথ্য_LAYOUT_NHWC) &&
        (another.layout == তথ্য_LAYOUT_NCHW || another.layout == তথ্য_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != তথ্য_LAYOUT_BLOCK && another.layout != তথ্য_LAYOUT_BLOCK);

    MatShape ফলাফল;

    যদি (dims < 0 || another.dims < 0)
        ফেরত ফলাফল;

    ফলাফল = *এই;
    ফলাফল.dims = std::max(dims, another.dims);
    ফলাফল.layout = layout == তথ্য_LAYOUT_UNKNOWN ? another.layout :
        layout == তথ্য_LAYOUT_ND && (another.layout == তথ্য_LAYOUT_NCHW ||
        another.layout == তথ্য_LAYOUT_NHWC) ? another.layout : layout;
    জন্য (পূর্ণসংখ্যা i = ফলাফল.dims-1; i >= 0; i--) {
        পূর্ণসংখ্যা i1 = i - (ফলাফল.dims - dims);
        পূর্ণসংখ্যা i2 = i - (ফলাফল.dims - another.dims);
        পূর্ণসংখ্যা sz1 = i1 < 0 ? 1 : p[i1];
        পূর্ণসংখ্যা sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        ফলাফল.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ফেরত ফলাফল;
}

/////////////////////////// MatAllocator ////////////////////////////

শূন্য MatAllocator::map(UMat_তথ্য*, AccessFlag) ধ্রুবক
{
}

শূন্য MatAllocator::unmap(UMat_তথ্য* u) ধ্রুবক
{
    যদি(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

শূন্য MatAllocator::download(UMat_তথ্য* u, শূন্য* dstptr,
         পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
         ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
         ধ্রুবক size_t dststep[]) ধ্রুবক
{
    যদি(!u)
        ফেরত;
    পূর্ণসংখ্যা isz[CV_MAX_DIM];
    uchar* srcptr = u->তথ্য;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ফেরত;
        যদি( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (পূর্ণসংখ্যা)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুবক Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    জন্য( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


শূন্য MatAllocator::upload(UMat_তথ্য* u, ধ্রুবক শূন্য* srcptr, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                    ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[],
                    ধ্রুবক size_t srcstep[]) ধ্রুবক
{
    যদি(!u)
        ফেরত;
    পূর্ণসংখ্যা isz[CV_MAX_DIM];
    uchar* dstptr = u->তথ্য;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ফেরত;
        যদি( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (পূর্ণসংখ্যা)sz[i];
    }

    Mat src(dims, isz, CV_8U, (শূন্য*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুবক Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    জন্য( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

শূন্য MatAllocator::copy(UMat_তথ্য* usrc, UMat_তথ্য* udst, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                  ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
                  ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[], যৌক্তিক /*sync*/) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি(!usrc || !udst)
        ফেরত;
    পূর্ণসংখ্যা isz[CV_MAX_DIM];
    uchar* srcptr = usrc->তথ্য;
    uchar* dstptr = udst->তথ্য;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ফেরত;
        যদি( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        যদি( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (পূর্ণসংখ্যা)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুবক Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    জন্য( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

বাফার_Pool_Controller* MatAllocator::get_বাফার_Pool_Controller(ধ্রুবক অক্ষর* id) ধ্রুবক
{
    CV_UNUSED(id);
    স্থির Dummy_বাফার_Pool_Controller dummy;
    ফেরত &dummy;
}

শ্রেণী StdMatAllocator CV_FINAL : সর্বজনীন MatAllocator
{
সর্বজনীন:
    UMat_তথ্য* allocate(পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type,
                       শূন্য* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        size_t যোগ = CV_ELEM_আকার(type);
        জন্য( পূর্ণসংখ্যা i = dims-1; i >= 0; i-- )
        {
            যদি( step )
            {
                যদি( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(যোগ <= step[i]);
                    যোগ = step[i];
                }
                অন্যথা
                    step[i] = যোগ;
            }
            যোগ *= sizes[i];
        }
        uchar* তথ্য = data0 ? (uchar*)data0 : (uchar*)fastMalloc(যোগ);
        UMat_তথ্য* u = নতুন UMat_তথ্য(এই);
        u->তথ্য = u->origdata = তথ্য;
        u->size = যোগ;
        যদি(data0)
            u->flags |= UMat_তথ্য::USER_ALLOCATED;

        ফেরত u;
    }

    যৌক্তিক allocate(UMat_তথ্য* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u) ফেরত মিথ্যা;
        ফেরত সত্য;
    }

    শূন্য deallocate(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        যদি( !(u->flags & UMat_তথ্য::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

স্থির
MatAllocator*& getDefaultAllocatorMatRef()
{
    স্থির MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ফেরত g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ফেরত getDefaultAllocatorMatRef();
}

শূন্য Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, নতুন StdMatAllocator())
}

//==================================================================================================

শূন্য set_আকার( Mat& m, পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, ধ্রুবক size_t* _steps, যৌক্তিক autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_আকার(m.flags);
    জন্য (পূর্ণসংখ্যা i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        যদি (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } অন্যথা যদি (autoSteps) {
            m.step[i] = autostep;
        } অন্যথা {
            m.step[i] = 0;
        }
    }

    যদি( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } অন্যথা {
        m.cols = m.rows = -1;
    }
}

পূর্ণসংখ্যা updateContinuityFlag(পূর্ণসংখ্যা flags, পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* size, ধ্রুবক size_t* step)
{
    পূর্ণসংখ্যা i, j;
    যদি (dims <= 1)
        ফেরত flags | Mat::CONTINUOUS_FLAG;
    জন্য( i = 0; i < dims; i++ )
    {
        যদি( size[i] > 1 )
            বিরতি;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    জন্য( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        যদি( step[j]*size[j] < step[j-1] )
            বিরতি;
    }

    যদি( j <= i && t == (uint64)(পূর্ণসংখ্যা)t )
        ফেরত flags | Mat::CONTINUOUS_FLAG;
    ফেরত flags & ~Mat::CONTINUOUS_FLAG;
}

শূন্য Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

শূন্য finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    পূর্ণসংখ্যা d = m.dims;
    যদি( d > 2 )
        m.rows = m.cols = -1;
    যদি(m.u)
        m.datastart = m.তথ্য = m.u->তথ্য;
    যদি( m.তথ্য && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        যদি( m.size[0] > 0 )
        {
            পূর্ণসংখ্যা lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            জন্য( পূর্ণসংখ্যা i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        অন্যথা
            m.dataend = m.datalimit;
    }
    অন্যথা
        m.dataend = m.datalimit = m.তথ্য;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *এই = _s;
}

Mat::Mat(আকার_2 _sz, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(আকার_2 _sz, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *এই = _s;
}

Mat::Mat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *এই = _s;
}

Mat::Mat(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _sz, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _sz, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *এই = _s;
}

Mat::Mat(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<পূর্ণসংখ্যা> _shape, পূর্ণসংখ্যা _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *এই = _s;
}

Mat::Mat(std::initializer_list<পূর্ণসংখ্যা> _shape, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *এই = _s;
}

Mat::Mat(ধ্রুবক Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), তথ্য(m.তথ্য),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    যদি( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      তথ্য((uchar*)তথ্য_3), datastart((uchar*)তথ্য_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(যোগ() == 0 || তথ্য != ফাঁকা);

    size_t esz = CV_ELEM_আকার(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    যদি( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    অন্যথা
    {
        CV_Assert( _step >= minstep );
        যদি (_step % esz1 != 0)
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

Mat::Mat(আকার_2 _sz, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      তথ্য((uchar*)তথ্য_3), datastart((uchar*)তথ্য_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(যোগ() == 0 || তথ্য != ফাঁকা);

    size_t esz = CV_ELEM_আকার(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    যদি( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    অন্যথা
    {
        CV_CheckGE(_step, minstep, "");

        যদি (_step % esz1 != 0)
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

Mat& Mat::operator = (ধ্রুবক Mat& m)
{
    যদি( এই != &m )
    {
        যদি( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        size = m.size;
        step = m.step;
        তথ্য = m.তথ্য;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    ফেরত *এই;
}

Mat Mat::clone() ধ্রুবক
{
    Mat m;
    copyTo(m);
    ফেরত m;
}

শূন্য Mat::assignTo( Mat& m, পূর্ণসংখ্যা _type ) ধ্রুবক
{
    যদি( _type < 0 )
        m = *এই;
    অন্যথা
        convertTo(m, _type);
}

শূন্য Mat::create(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type)
{
    _type &= TYPE_MASK;
    যদি( dims <= 2 && rows == _rows && cols == _cols && type() == _type && তথ্য )
        ফেরত;
    পূর্ণসংখ্যা sz[] = {_rows, _cols};
    create(2, sz, _type);
}

শূন্য Mat::create(আকার_2 _sz, পূর্ণসংখ্যা _type)
{
    create(_sz.height, _sz.width, _type);
}

শূন্য Mat::create_Same_আকার(প্রবেশ_Array m, পূর্ণসংখ্যা type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

শূন্য Mat::fit(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type)
{
    size_t old_যোগ_Bytes = u ? u->size : 0;
    size_t esz = CV_ELEM_আকার(_type), new_যোগ = _dims >= 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < _dims; i++)
        new_যোগ *= _sizes[i];
    size_t new_যোগ_Bytes = new_যোগ*esz;
    যদি (new_যোগ_Bytes > 0 && (!isContinuous() ||
                              new_যোগ_Bytes > old_যোগ_Bytes ||
                              তথ্য != datastart)) {
        create(_dims, _sizes, _type);
    } অন্যথা {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        পূর্ণসংখ্যা dummy_আকার = 0;
        set_আকার(*এই, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_আকার), nullptr, সত্য);
        finalizeHdr(*এই);
    }
}

শূন্য Mat::fit(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _shape, পূর্ণসংখ্যা _type)
{
    fit((পূর্ণসংখ্যা)_shape.size(), _shape.তথ্য(), _type);
}

শূন্য Mat::fit(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য Mat::fit(std::initializer_list<পূর্ণসংখ্যা> _shape, পূর্ণসংখ্যা _type)
{
    পূর্ণসংখ্যা new_shape[MatShape::MAX_DIMS];
    পূর্ণসংখ্যা new_ndims = (পূর্ণসংখ্যা)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    জন্য (পূর্ণসংখ্যা i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

শূন্য Mat::fit(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type)
{
    _type &= TYPE_MASK;
    পূর্ণসংখ্যা sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

শূন্য Mat::fit(আকার_2 _sz, পূর্ণসংখ্যা _type)
{
    fit(_sz.height, _sz.width, _type);
}

শূন্য Mat::fit_Same_আকার(প্রবেশ_Array m, পূর্ণসংখ্যা _type)
{
    fit(m.shape(), _type);
}

শূন্য Mat::addref()
{
    যদি( u )
        CV_XADD(&u->refcount, 1);
}

শূন্য Mat::release()
{
    যদি( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ফাঁকা;
    datastart = dataend = datalimit = তথ্য = 0;
    জন্য(পূর্ণসংখ্যা i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(পূর্ণসংখ্যা i) ধ্রুবক
{
    ফেরত step.p[i] / elemSize1();
}

যৌক্তিক Mat::empty() ধ্রুবক
{
    ফেরত তথ্য == 0 || যোগ() == 0;
}

size_t Mat::যোগ() ধ্রুবক
{
    যদি( dims <= 2 )
        ফেরত (size_t)rows * cols;
    size_t p = 1;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
        p *= size[i];
    ফেরত p;
}

size_t Mat::যোগ(পূর্ণসংখ্যা startDim, পূর্ণসংখ্যা endDim) ধ্রুবক
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    পূর্ণসংখ্যা endDim_ = endDim <= dims ? endDim : dims;
    জন্য( পূর্ণসংখ্যা i = startDim; i < endDim_; i++ )
        p *= size[i];
    ফেরত p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), তথ্য(m.তথ্য),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.তথ্য = ফাঁকা; m.datastart = ফাঁকা; m.dataend = ফাঁকা; m.datalimit = ফাঁকা;
    m.allocator = ফাঁকা;
    m.u = ফাঁকা;
}


Mat& Mat::operator = (Mat&& m)
{
    যদি (এই == &m)
      ফেরত *এই;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; তথ্য = m.তথ্য;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.তথ্য = ফাঁকা; m.datastart = ফাঁকা; m.dataend = ফাঁকা; m.datalimit = ফাঁকা;
    m.allocator = ফাঁকা;
    m.u = ফাঁকা;
    ফেরত *এই;
}


শূন্য Mat::create(পূর্ণসংখ্যা d0, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type)
{
    পূর্ণসংখ্যা sz1 = 1, d = d0;
    পূর্ণসংখ্যা i;
    যদি (d == 0) {
        d = 1;
        _sizes = (ধ্রুবক পূর্ণসংখ্যা*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    যদি( তথ্য && d == dims && _type == type() )
    {
        জন্য( i = 0; i < d; i++ )
            যদি( size[i] != _sizes[i] )
                বিরতি;
        যদি( i == d )
            ফেরত;
    }

    পূর্ণসংখ্যা _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (_sizes == (এই->size.p))
    {
        জন্য(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    যদি( d == 0 )
        ফেরত;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    set_আকার(*এই, d, _sizes, 0, সত্য);

    যদি( যোগ() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        যদি(!a)
            a = a0;
        চেষ্টা
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        ধরো (...)
        {
            যদি (a == a0)
                ছোড়ো;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_আকার(flags) );
    }

    addref();
    finalizeHdr(*এই);
    size.dims = dims = d0;
}

শূন্য Mat::create(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _sizes, পূর্ণসংখ্যা _type)
{
    create((পূর্ণসংখ্যা)_sizes.size(), _sizes.তথ্য(), _type);
}

শূন্য Mat::create(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type)
{
    যদি (_shape.dims < 0) {
        release();
        ফেরত;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য Mat::create(std::initializer_list<পূর্ণসংখ্যা> _shape, পূর্ণসংখ্যা _type)
{
    পূর্ণসংখ্যা new_shape[MatShape::MAX_DIMS];
    পূর্ণসংখ্যা new_ndims = (পূর্ণসংখ্যা)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    জন্য (পূর্ণসংখ্যা i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

শূন্য Mat::copy_আকার(ধ্রুবক Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

শূন্য Mat::deallocate()
{
    যদি(u)
    {
        UMat_তথ্য* u_ = u;
        u = ফাঁকা;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ধ্রুবক Mat& m, ধ্রুবক Range& _rowRange, ধ্রুবক Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    যদি( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        জন্য( পূর্ণসংখ্যা i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *এই = m(rs);
        ফেরত;
    }

    *এই = m;
    চেষ্টা
    {
        যদি( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            তথ্য += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        যদি( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            তথ্য += _colRange.start*elem_আকার();
            flags |= SUBMATRIX_FLAG;
        }
    }
    ধরো(...)
    {
        release();
        ছোড়ো;
    }

    updateContinuityFlag();

    যদি( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ধ্রুবক Mat& m, ধ্রুবক Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    তথ্য(m.তথ্য + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_আকার(flags);
    তথ্য += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    যদি( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    যদি( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, ধ্রুবক size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = তথ্য = (uchar*)তথ্য_3;
    যদি (_dims > 0 || তথ্য_3 != nullptr)
        set_আকার(*এই, _dims, _sizes, _steps, সত্য);
    finalizeHdr(*এই);
}


Mat::Mat(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _sizes, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, ধ্রুবক size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = তথ্য = (uchar*)তথ্য_3;
    পূর্ণসংখ্যা _dims = (পূর্ণসংখ্যা)_sizes.size();
    যদি (_dims > 0 || তথ্য_3 != nullptr)
        set_আকার(*এই, _dims, _sizes.তথ্য(), _steps, সত্য);
    finalizeHdr(*এই);
}

Mat::Mat(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, ধ্রুবক size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = তথ্য = (uchar*)তথ্য_3;
    যদি (_shape.dims >= 0) {
        set_আকার(*এই, _shape.dims, _shape.p, _steps, সত্য);
    }
    অন্যথা {
        CV_Assert(!তথ্য);
    }
    finalizeHdr(*এই);
}

Mat::Mat(std::initializer_list<পূর্ণসংখ্যা> _shape, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, ধ্রুবক size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    পূর্ণসংখ্যা new_shape[MatShape::MAX_DIMS];
    পূর্ণসংখ্যা _dims = (পূর্ণসংখ্যা)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    জন্য (পূর্ণসংখ্যা i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = তথ্য = (uchar*)তথ্য_3;
    যদি (_dims > 0 || তথ্য_3 != nullptr)
        set_আকার(*এই, _dims, new_shape, _steps, সত্য);
    finalizeHdr(*এই);
}

Mat::Mat(ধ্রুবক Mat& m, ধ্রুবক Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    পূর্ণসংখ্যা d = m.dims;

    CV_Assert(ranges);
    জন্য( পূর্ণসংখ্যা i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *এই = m;
    জন্য( পূর্ণসংখ্যা i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        যদি( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            তথ্য += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ধ্রুবক Mat& m, ধ্রুবক std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), তথ্য(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    পূর্ণসংখ্যা d = m.dims;

    CV_Assert((পূর্ণসংখ্যা)ranges.size() == d);
    জন্য (পূর্ণসংখ্যা i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *এই = m;
    জন্য (পূর্ণসংখ্যা i = 0; i < d; i++)
    {
        Range r = ranges[i];
        যদি (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            তথ্য += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(পূর্ণসংখ্যা d) ধ্রুবক
{
    CV_Assert( dims == 2 );
    Mat m = *এই;
    size_t esz = elem_আকার();
    পূর্ণসংখ্যা len;

    যদি( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.তথ্য += esz*d;
    }
    অন্যথা
    {
        len = std::min(rows + d, cols);
        m.তথ্য -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    যদি( size() != আকার_2(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ফেরত m;
}


শূন্য Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    যদি( isSubmatrix() )
        *এই = rowRange(0, size.p[0] - (পূর্ণসংখ্যা)nelems);
    অন্যথা
    {
        size.p[0] -= (পূর্ণসংখ্যা)nelems;
        যদি (dims == 2)
            rows = size.p[0];
        অন্যথা যদি (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


শূন্য Mat::ঠেলো_back(ধ্রুবক শূন্য* elem)
{
    size_t r = size.p[0];
    যদি( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_আকার();
    memcpy(তথ্য + r*step.p[0], elem, esz);
    size.p[0] = পূর্ণসংখ্যা(r + 1);
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    জন্য( পূর্ণসংখ্যা i = 1; i < dims; i++ )
        tsz *= size.p[i];
    যদি( esz < step.p[0] || tsz != (uint64)(পূর্ণসংখ্যা)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


শূন্য Mat::reserve(size_t nelems)
{
    ধ্রুবক size_t MIN_আকার = 64;

    CV_Assert( (পূর্ণসংখ্যা)nelems >= 0 );
    যদি( !isSubmatrix() && step.p[0] != 0 && তথ্য + step.p[0]*nelems <= datalimit )
        ফেরত;

    পূর্ণসংখ্যা r = size.p[0];
    যদি( (size_t)r >= nelems)
        ফেরত;

    MatShape newsize = size;
    size_t esz = elem_আকার();
    newsize.dims = std::max(newsize.dims, 1);
    যদি (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (পূর্ণসংখ্যা)nelems;
    size_t newbytes = newsize.যোগ() * esz;
    যদি (newbytes < MIN_আকার) {
        newsize.p[0] = 1;
        newsize.p[0] = পূর্ণসংখ্যা(MIN_আকার / newsize.যোগ() / esz);
    }

    Mat m(newsize, type());
    যদি( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *এই = m;
    size.p[0] = r;

    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend = তথ্য + step.p[0]*r;
}


শূন্য Mat::reserve_বাফার(size_t nbytes)
{
    size_t esz = 1;
    পূর্ণসংখ্যা mtype = CV_8UC1;
    যদি (!empty())
    {
        যদি (!isSubmatrix() && তথ্য + nbytes <= dataend)//Should it be datalimit?
            ফেরত;
        esz = elem_আকার();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#যদি SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    পূর্ণসংখ্যা newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#অন্যথা
    পূর্ণসংখ্যা newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    পূর্ণসংখ্যা newcols = (পূর্ণসংখ্যা)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


শূন্য Mat::resize(size_t nelems)
{
    পূর্ণসংখ্যা r = size.p[0];
    যদি( r == (পূর্ণসংখ্যা)nelems )
        ফেরত;
    CV_Assert( (পূর্ণসংখ্যা)nelems >= 0 );

    যদি( isSubmatrix() || তথ্য + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (পূর্ণসংখ্যা)nelems;
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


শূন্য Mat::resize(size_t nelems, ধ্রুবক Scalar& s)
{
    পূর্ণসংখ্যা saveRows = size.p[0];
    resize(nelems);

    যদি( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

শূন্য Mat::push_back(ধ্রুবক Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    যদি( delta == 0 )
        ফেরত;
    যদি( এই == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ফেরত;
    }
    যদি( !তথ্য )
    {
        *এই = elems.clone();
        ফেরত;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    যৌক্তিক eq = size == elems.size;
    size.p[0] = পূর্ণসংখ্যা(r);
    যদি( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    যদি( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_আকার();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    যদি (step0 < minstep) {
        যদি (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    যদি( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += পূর্ণসংখ্যা(delta);
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    যদি( isContinuous() && elems.isContinuous() )
        memcpy(তথ্য + r*step0, elems.তথ্য, elems.যোগ()*elems.elem_আকার());
    অন্যথা
    {
        Mat part = rowRange(পূর্ণসংখ্যা(r), পূর্ণসংখ্যা(r + delta));
        elems.copyTo(part);
    }
}


শূন্য Mat::locateROI( আকার_2& whole_আকার, Point& ofs ) ধ্রুবক
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elem_আকার(), minstep;
    ptrdiff_t delta1 = তথ্য - datastart, delta2 = dataend - datastart;

    যদি( delta1 == 0 )
        ofs.x = ofs.y = 0;
    অন্যথা
    {
        ofs.y = (পূর্ণসংখ্যা)(delta1/step[0]);
        ofs.x = (পূর্ণসংখ্যা)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( তথ্য == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    whole_আকার.height = (পূর্ণসংখ্যা)((delta2 - minstep)/step[0] + 1);
    whole_আকার.height = std::max(whole_আকার.height, ofs.y + rows);
    whole_আকার.width = (পূর্ণসংখ্যা)((delta2 - step*(whole_আকার.height-1))/esz);
    whole_আকার.width = std::max(whole_আকার.width, ofs.x + cols);
}

Mat& Mat::adjustROI( পূর্ণসংখ্যা dtop, পূর্ণসংখ্যা dbottom, পূর্ণসংখ্যা dleft, পূর্ণসংখ্যা dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    আকার_2 whole_আকার; Point ofs;
    size_t esz = elem_আকার();
    locateROI( whole_আকার, ofs );
    পূর্ণসংখ্যা row1 = std::min(std::max(ofs.y - dtop, 0), whole_আকার.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, whole_আকার.height));
    পূর্ণসংখ্যা col1 = std::min(std::max(ofs.x - dleft, 0), whole_আকার.width), col2 = std::max(0, std::min(ofs.x + cols + dright, whole_আকার.width));
    যদি(row1 > row2)
        std::swap(row1, row2);
    যদি(col1 > col2)
        std::swap(col1, col2);

    যদি (dims == 1) {
        তথ্য += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } অন্যথা {
        তথ্য += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ফেরত *এই;
}

Mat Mat::reshape(পূর্ণসংখ্যা new_cn, পূর্ণসংখ্যা new_rows) ধ্রুবক
{
    পূর্ণসংখ্যা cn = channels();
    Mat hdr = *এই;

    যদি( new_cn == 0 )
        new_cn = cn;

    যদি( dims > 2 )
    {
        যদি( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_আকার(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ফেরত hdr;
        }
        CV_Assert( new_rows > 0 );
        পূর্ণসংখ্যা sz[] = { new_rows, (পূর্ণসংখ্যা)(যোগ()*cn/new_rows) };
        ফেরত reshape(new_cn, 2, sz);
    }

    পূর্ণসংখ্যা যোগ_width = cols * cn;

    যদি( (new_cn > যোগ_width || যোগ_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * যোগ_width / new_cn;

    যদি( new_rows != 0 && new_rows != rows )
    {
        পূর্ণসংখ্যা যোগ_আকার_2 = যোগ_width * rows;
        যদি( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        যোগ_width = যোগ_আকার_2 / new_rows;

        যদি( যোগ_width * new_rows != যোগ_আকার_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = যোগ_width * elemSize1();
    } অন্যথা {
        hdr.size[0] = hdr.rows = rows;
        যদি (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_আকার(flags);
    }

    পূর্ণসংখ্যা new_width = যোগ_width / new_cn;

    যদি( new_width * new_cn != যোগ_width )
        CV_Error( cv::Error::Bad_অঙ্ক_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_আকার(hdr.flags);
    ফেরত hdr;
}

Mat Mat::reshape(পূর্ণসংখ্যা _cn, পূর্ণসংখ্যা _newndims, ধ্রুবক পূর্ণসংখ্যা* _newsz) ধ্রুবক
{
    যদি(_newndims == dims)
    {
        যদি(_newsz == 0)
            ফেরত reshape(_cn);
        যদি(_newndims == 2)
            ফেরত reshape(_cn, _newsz[0]);
    }

    যদি (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        যদি (_cn == 0)
            _cn = এই->channels();
        অন্যথা
            CV_Assert(_cn <= CV_CN_MAX);

        size_t যোগ_elem1_ref = এই->যোগ() * এই->channels();
        size_t যোগ_elem1 = _cn;

        Auto_বাফার<পূর্ণসংখ্যা, 4> newsz_buf( (size_t)_newndims );

        পূর্ণসংখ্যা m1_idx = -1;

        জন্য (পূর্ণসংখ্যা i = 0; i < _newndims; i++)
        {
            যদি (_newsz[i] >= 0) {
                যদি (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                অন্যথা
                    newsz_buf[i] = _newsz[i];
                যোগ_elem1 *= (size_t)newsz_buf[i];
            } অন্যথা {
                যদি (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_আকার, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        যদি (m1_idx >= 0) {
            যদি (যোগ_elem1 == 0) {
                CV_Assert(যোগ_elem1_ref == 0);
                যোগ_elem1 = 1;
            }
            CV_Assert(যোগ_elem1_ref % যোগ_elem1 == 0);
            newsz_buf[m1_idx] = (পূর্ণসংখ্যা)(যোগ_elem1_ref / যোগ_elem1);
        } অন্যথা যদি (যোগ_elem1 != যোগ_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *এই;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        set_আকার(hdr, _newndims, newsz_buf.তথ্য(), ফাঁকা, সত্য);

        ফেরত hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(পূর্ণসংখ্যা _cn, ধ্রুবক std::vector<পূর্ণসংখ্যা>& _newshape) ধ্রুবক
{
    পূর্ণসংখ্যা newdims = (পূর্ণসংখ্যা)_newshape.size();
    যদি(newdims == 0 && empty())
    {
        ফেরত *এই;
    }

    ফেরত reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(পূর্ণসংখ্যা _cn, ধ্রুবক MatShape& _newshape) ধ্রুবক
{
    যদি (_newshape.dims < 0) {
        পূর্ণসংখ্যা newshape[] = {0};
        ফেরত reshape(_cn, 1, newshape);
    }
    ফেরত reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(পূর্ণসংখ্যা _cn, std::initializer_list<পূর্ণসংখ্যা> newshape_) ধ্রুবক
{
    পূর্ণসংখ্যা newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    জন্য (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ফেরত reshape(_cn, (পূর্ণসংখ্যা)newshape_dims, newshape);
}

Mat Mat::reinterpret(পূর্ণসংখ্যা type) ধ্রুবক
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_আকার(এই->type()) == CV_ELEM_আকার(type));
    Mat m = *এই;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ফেরত m;
}

Mat Mat::diag(ধ্রুবক Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    পূর্ণসংখ্যা len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    যদি( d.cols == 1 )
        d.copyTo(md);
    অন্যথা
        transpose(d, md);
    ফেরত m;
}

পূর্ণসংখ্যা Mat::checkVector(পূর্ণসংখ্যা _elemChannels, পূর্ণসংখ্যা _depth, যৌক্তিক _requireContinuous) ধ্রুবক
{
    ফেরত তথ্য && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (পূর্ণসংখ্যা)(যোগ()*channels()/_elemChannels) : -1;
}


স্থির অন্তঃরেখ আকার_2 get_Continuous_আকার(পূর্ণসংখ্যা flags, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    যৌক্তিক has_int_overflow = sz >= INT_MAX;
    যৌক্তিক isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ফেরত (isContiguous && !has_int_overflow)
            ? আকার_2((পূর্ণসংখ্যা)sz, 1)
            : আকার_2(cols * widthScale, rows);
}

আকার_2 getContinuousSize2D(Mat& m1, পূর্ণসংখ্যা widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ফেরত get_Continuous_আকার(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
আকার_2 getContinuousSize2D(Mat& m1, Mat& m2, পূর্ণসংখ্যা widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ধ্রুবক আকার_2 sz1 = m1.size();
    যদি (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t যোগ_sz = m1.যোগ();
        CV_CheckEQ(যোগ_sz, m2.যোগ(), "");
        যৌক্তিক is_m1_vector = m1.cols == 1 || m1.rows == 1;
        যৌক্তিক is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)যোগ_sz;  // vector-column
        যৌক্তিক isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        যৌক্তিক has_int_overflow = ((int64)যোগ_sz * widthScale) >= INT_MAX;
        যদি (isContiguous && !has_int_overflow)
            যোগ = 1; // vector-row
        m1 = m1.reshape(0, যোগ);
        m2 = m2.reshape(0, যোগ);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ফেরত আকার_2(m1.cols * widthScale, m1.rows);
    }
    ফেরত get_Continuous_আকার(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

আকার_2 getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, পূর্ণসংখ্যা widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ধ্রুবক আকার_2 sz1 = m1.size();
    যদি (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t যোগ_sz = m1.যোগ();
        CV_CheckEQ(যোগ_sz, m2.যোগ(), "");
        CV_CheckEQ(যোগ_sz, m3.যোগ(), "");
        যৌক্তিক is_m1_vector = m1.cols == 1 || m1.rows == 1;
        যৌক্তিক is_m2_vector = m2.cols == 1 || m2.rows == 1;
        যৌক্তিক is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)যোগ_sz;  // vector-column
        যৌক্তিক isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        যৌক্তিক has_int_overflow = ((int64)যোগ_sz * widthScale) >= INT_MAX;
        যদি (isContiguous && !has_int_overflow)
            যোগ = 1; // vector-row
        m1 = m1.reshape(0, যোগ);
        m2 = m2.reshape(0, যোগ);
        m3 = m3.reshape(0, যোগ);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ফেরত আকার_2(m1.cols * widthScale, m1.rows);
    }
    ফেরত get_Continuous_আকার(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
