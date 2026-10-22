// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "bufferpool.impl.hpp"

নামাকাশ cv {

std::string layoutToString(DataLayout layout)
{
    ঘূৰাই
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

তার্কিক operator == (ধ্রুৱ MatShape& size1, ধ্রুৱ MatShape& size2)
{
    যদি (size1.dims != size2.dims)
        ঘূৰাই মিছা;
    যদি (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        ঘূৰাই মিছা;
    যদি (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ঘূৰাই মিছা;
    বাবে (পূর্ণাঙ্ক i = 0; i < size1.dims; i++) {
        যদি (size1.p[i] != size2.p[i])
            ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
}

তার্কিক operator != (ধ্রুৱ MatShape& size1, ধ্রুৱ MatShape& size2)
{
    ঘূৰাই !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ঘূৰাই MatShape(0);
}

শূন্য MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

শূন্য MatShape::resize(size_t newSize, পূর্ণাঙ্ক value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    পূর্ণাঙ্ক old_dims = std::max(dims, 0);
    dims = (পূর্ণাঙ্ক)newSize;
    বাবে (পূর্ণাঙ্ক i = old_dims; i < dims; i++)
        p[i] = value;
}

শূন্য MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

শূন্য MatShape::assign(size_t newSize, পূর্ণাঙ্ক value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)newSize;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
        p[i] = value;
}

শূন্য MatShape::assign(পূর্ণাঙ্ক newSize, পূর্ণাঙ্ক value)
{
    assign((size_t)newSize, value);
}

শূন্য MatShape::assign(ধ্রুৱ পূর্ণাঙ্ক* begin, ধ্রুৱ পূর্ণাঙ্ক* end)
{
    assign_(begin, end);
}

শূন্য MatShape::assign_(ধ্রুৱ পূর্ণাঙ্ক* begin, ধ্রুৱ পূর্ণাঙ্ক* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)newSize;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
        p[i] = begin[i];
}

পূর্ণাঙ্ক* MatShape::begin() { ঘূৰাই p; }
ধ্রুৱ পূর্ণাঙ্ক* MatShape::begin() ধ্রুৱ { ঘূৰাই p; }
পূর্ণাঙ্ক* MatShape::end() { ঘূৰাই p + std::max(dims, 0); }
ধ্রুৱ পূর্ণাঙ্ক* MatShape::end() ধ্রুৱ { ঘূৰাই p + std::max(dims, 0); }
পূর্ণাঙ্ক& MatShape::back() { ঘূৰাই p[std::max(dims-1, 0)]; }
ধ্রুৱ পূর্ণাঙ্ক& MatShape::back() ধ্রুৱ { ঘূৰাই p[std::max(dims-1, 0)]; }

শূন্য MatShape::push_back(পূর্ণাঙ্ক value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

শূন্য MatShape::emplace_back(পূর্ণাঙ্ক value)
{
    push_back(value);
}

শূন্য MatShape::insert(পূর্ণাঙ্ক* where, পূর্ণাঙ্ক value)
{
    পূর্ণাঙ্ক old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    বাবে (পূর্ণাঙ্ক i = old_dims-1; i >= (পূর্ণাঙ্ক)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

শূন্য MatShape::insert(পূর্ণাঙ্ক* where, size_t গুনতি_2, পূর্ণাঙ্ক value)
{
    পূর্ণাঙ্ক old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+গুনতি_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (পূর্ণাঙ্ক)(old_dims+গুনতি_2);
    বাবে (পূর্ণাঙ্ক i = old_dims-1; i >= (পূর্ণাঙ্ক)ofs; i--)
        p[i+গুনতি_2] = p[i];
    বাবে (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)গুনতি_2; i++)
        p[i+ofs] = value;
}

শূন্য MatShape::insert(পূর্ণাঙ্ক* where, পূর্ণাঙ্ক গুনতি_2, পূর্ণাঙ্ক value)
{
    insert(where, (size_t)গুনতি_2, value);
}

শূন্য MatShape::insert(পূর্ণাঙ্ক* where, ধ্রুৱ পূর্ণাঙ্ক* begin, ধ্রুৱ পূর্ণাঙ্ক* end)
{
    ঢোকাও(where, begin, end);
}

শূন্য MatShape::ঢোকাও(পূর্ণাঙ্ক* where, ধ্রুৱ পূর্ণাঙ্ক* begin, ধ্রুৱ পূর্ণাঙ্ক* end)
{
    পূর্ণাঙ্ক old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (পূর্ণাঙ্ক)(old_dims+delta);
    বাবে (পূর্ণাঙ্ক i = old_dims-1; i >= (পূর্ণাঙ্ক)ofs; i--)
        p[i+delta] = p[i];
    বাবে (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)delta; i++)
        p[i+ofs] = begin[i];
}

শূন্য MatShape::erase(পূর্ণাঙ্ক* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    যদি (ofs == dims)
        ঘূৰাই;
    dims--;
    বাবে (পূর্ণাঙ্ক i = (পূর্ণাঙ্ক)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() ধ্রুৱ
{
    size_t result = 1;
    যদি (dims < 0)
        ঘূৰাই 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
        result *= p[i];
    ঘূৰাই result;
}

std::vector<পূর্ণাঙ্ক> MatShape::vec() ধ্রুৱ
{
    যদি (dims < 0)
        ঘূৰাই std::vector<পূর্ণাঙ্ক>(1, 0);
    ঘূৰাই std::vector<পূর্ণাঙ্ক>(p, p + dims);
}

std::string MatShape::str() ধ্রুৱ
{
    std::stringstream sstrm;
    যদি (empty()) {
        sstrm << "<empty>";
    } অন্যথা যদি (dims == 0) {
        sstrm << "<scalar>";
    } অন্যথা {
        sstrm << "[";
        বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ঘূৰাই sstrm.str();
}

স্থির শূন্য finalizeBlockLayout(MatShape& size, পূর্ণাঙ্ক C=0)
{
    যদি (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        পূর্ণাঙ্ক C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } অন্যথা {
        size.C = 0;
    }
    বাবে (পূর্ণাঙ্ক i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    যদি (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ধ্রুৱ পূর্ণাঙ্ক* size_, DataLayout layout_, পূর্ণাঙ্ক C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)dims_;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*এতত্, C_);
}

MatShape::MatShape(size_t dims_, পূর্ণাঙ্ক value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)dims_;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*এতত্, 0);
}

MatShape::MatShape(std::initializer_list<পূর্ণাঙ্ক> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)shape.size();
    auto it = shape.begin();
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*এতত্, 0);
}

MatShape::MatShape(পূর্ণাঙ্ক dims_, পূর্ণাঙ্ক value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*এতত্, 0);
}

MatShape::MatShape(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& shape_, DataLayout layout_, পূর্ণাঙ্ক C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (পূর্ণাঙ্ক)shape_size;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*এতত্, C_);
}

MatShape::MatShape(ধ্রুৱ পূর্ণাঙ্ক* begin, ধ্রুৱ পূর্ণাঙ্ক* end, DataLayout layout_, পূর্ণাঙ্ক C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (পূর্ণাঙ্ক)shape_size;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*এতত্, C_);
}

MatShape::MatShape(ধ্রুৱ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    বাবে (পূর্ণাঙ্ক i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ধ্রুৱ MatShape& shape)
{
    যদি (এতত্ != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        বাবে (পূর্ণাঙ্ক i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ঘূৰাই *এতত্;
}

তার্কিক MatShape::hasSymbols() ধ্রুৱ
{
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++) {
        যদি (p[i] < 0)
            ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
}

পূর্ণাঙ্ক MatShape::channels() ধ্রুৱ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    ঘূৰাই layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, পূর্ণাঙ্ক C0) ধ্রুৱ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *এতত্;
    newsize.layout = newLayout;

    যদি (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        পূর্ণাঙ্ক Corig = channels();
        newsize.C = Corig;

        যদি (layout == DATA_LAYOUT_NHWC) {
            বাবে (পূর্ণাঙ্ক i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } অন্যথা যদি (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        যদি (newLayout == DATA_LAYOUT_NHWC) {
            বাবে (পূর্ণাঙ্ক i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } অন্যথা {
        CV_Assert_N(C0 <= 1);

        যদি (newLayout == layout)
            ঘূৰাই newsize;

        // NHWC => NCHW
        যদি (newLayout == DATA_LAYOUT_NCHW) {
            বাবে (পূর্ণাঙ্ক i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } অন্যথা {
            // NCHW => NHWC
            বাবে (পূর্ণাঙ্ক i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ঘূৰাই newsize;
}

স্থির পূর্ণাঙ্ক normalizeShapeAxis(পূর্ণাঙ্ক axis, পূর্ণাঙ্ক dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ঘূৰাই axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
স্থির MatShape toSemantic(ধ্রুৱ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ঘূৰাই shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
স্থির MatShape toBlockOf(MatShape result, ধ্রুৱ MatShape& src, ধ্রুৱ ৱর্ণ* op)
{
    যদি (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (পূর্ণাঙ্ক)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    ঘূৰাই result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& axes) ধ্রুৱ
{
    CV_Assert(dims >= 0);
    যদি (layout == DATA_LAYOUT_BLOCK)
        ঘূৰাই toBlockOf(toSemantic(*এতত্).squeeze(axes), *এতত্, "squeeze");
    তার্কিক remove[MAX_DIMS] = {মিছা};
    যদি (axes.empty())
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    বাবে (পূর্ণাঙ্ক a : axes)
    {
        পূর্ণাঙ্ক i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = সঁচা;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
        যদি (!remove[i])
            result.p[result.dims++] = p[i];
    ঘূৰাই result;
}

MatShape MatShape::unsqueeze(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& axes) ধ্রুৱ
{
    CV_Assert(dims >= 0);
    যদি (layout == DATA_LAYOUT_BLOCK)
        ঘূৰাই toBlockOf(toSemantic(*এতত্).unsqueeze(axes), *এতত্, "unsqueeze");
    ধ্রুৱ পূর্ণাঙ্ক outDims = dims + (পূর্ণাঙ্ক)axes.size();
    CV_CheckLE(outDims, (পূর্ণাঙ্ক)MAX_DIMS, "unsqueeze: too many dimensions");
    তার্কিক insert[MAX_DIMS] = {মিছা};
    বাবে (পূর্ণাঙ্ক a : axes)
    {
        পূর্ণাঙ্ক i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = সঁচা;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    বাবে (পূর্ণাঙ্ক i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    ঘূৰাই result;
}

MatShape MatShape::flatten(পূর্ণাঙ্ক startAxis, পূর্ণাঙ্ক endAxis) ধ্রুৱ
{
    CV_Assert(dims >= 0);
    যদি (layout == DATA_LAYOUT_BLOCK)
        ঘূৰাই toBlockOf(toSemantic(*এতত্).flatten(startAxis, endAxis), *এতত্, "flatten");
    যদি (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ঘূৰাই MatShape({1});
    }
    পূর্ণাঙ্ক start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    বাবে (পূর্ণাঙ্ক i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((দ্ৱিগুণ)merged, (দ্ৱিগুণ)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
    {
        যদি (i == start)
            result.p[result.dims++] = (পূর্ণাঙ্ক)merged;
        অন্যথা যদি (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    ঘূৰাই result;
}

MatShape MatShape::reshape(ধ্রুৱ MatShape& newShape, তার্কিক allowZero) ধ্রুৱ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    যদি (layout == DATA_LAYOUT_BLOCK)
        ঘূৰাই toBlockOf(toSemantic(*এতত্).reshape(newShape, allowZero), *এতত্, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    পূর্ণাঙ্ক inferred = -1;
    int64 known = 1;
    বাবে (পূর্ণাঙ্ক i = 0; i < result.dims; i++)
    {
        পূর্ণাঙ্ক& sz = result.p[i];
        যদি (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            অনুৱর্ত;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        যদি (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ধ্রুৱ int64 total_ = (int64)total();
    যদি (inferred >= 0)
    {
        CV_Check((দ্ৱিগুণ)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((দ্ৱিগুণ)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((দ্ৱিগুণ)(total_/known), (দ্ৱিগুণ)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (পূর্ণাঙ্ক)(total_/known);
    }
    অন্যথা যদি (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    ঘূৰাই result;
}

MatShape MatShape::expand(ধ্রুৱ MatShape& another) ধ্রুৱ
{
    যদি (dims == 0)
        ঘূৰাই another;
    যদি (another.dims == 0)
        ঘূৰাই *এতত্;

    যদি ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    যদি (dims < 0 || another.dims < 0)
        ঘূৰাই result;

    result = *এতত্;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    বাবে (পূর্ণাঙ্ক i = result.dims-1; i >= 0; i--) {
        পূর্ণাঙ্ক i1 = i - (result.dims - dims);
        পূর্ণাঙ্ক i2 = i - (result.dims - another.dims);
        পূর্ণাঙ্ক sz1 = i1 < 0 ? 1 : p[i1];
        পূর্ণাঙ্ক sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ঘূৰাই result;
}

/////////////////////////// MatAllocator ////////////////////////////

শূন্য MatAllocator::map(UMatData*, AccessFlag) ধ্রুৱ
{
}

শূন্য MatAllocator::unmap(UMatData* u) ধ্রুৱ
{
    যদি(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

শূন্য MatAllocator::download(UMatData* u, শূন্য* dstptr,
         পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
         ধ্রুৱ size_t srcofs[], ধ্রুৱ size_t srcstep[],
         ধ্রুৱ size_t dststep[]) ধ্রুৱ
{
    যদি(!u)
        ঘূৰাই;
    পূর্ণাঙ্ক isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ঘূৰাই;
        যদি( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (পূর্ণাঙ্ক)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুৱ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    বাবে( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


শূন্য MatAllocator::upload(UMatData* u, ধ্রুৱ শূন্য* srcptr, পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
                    ধ্রুৱ size_t dstofs[], ধ্রুৱ size_t dststep[],
                    ধ্রুৱ size_t srcstep[]) ধ্রুৱ
{
    যদি(!u)
        ঘূৰাই;
    পূর্ণাঙ্ক isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ঘূৰাই;
        যদি( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (পূর্ণাঙ্ক)sz[i];
    }

    Mat src(dims, isz, CV_8U, (শূন্য*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুৱ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    বাবে( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

শূন্য MatAllocator::copy(UMatData* usrc, UMatData* udst, পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
                  ধ্রুৱ size_t srcofs[], ধ্রুৱ size_t srcstep[],
                  ধ্রুৱ size_t dstofs[], ধ্রুৱ size_t dststep[], তার্কিক /*sync*/) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি(!usrc || !udst)
        ঘূৰাই;
    পূর্ণাঙ্ক isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        যদি( sz[i] == 0 )
            ঘূৰাই;
        যদি( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        যদি( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (পূর্ণাঙ্ক)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ধ্রুৱ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    বাবে( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ধ্রুৱ ৱর্ণ* id) ধ্রুৱ
{
    CV_UNUSED(id);
    স্থির DummyBufferPoolController dummy;
    ঘূৰাই &dummy;
}

শ্ৰেণী StdMatAllocator CV_FINAL : সার্ৱজনিক MatAllocator
{
সার্ৱজনিক:
    UMatData* allocate(পূর্ণাঙ্ক dims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type,
                       শূন্য* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ধ্রুৱ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        বাবে( পূর্ণাঙ্ক i = dims-1; i >= 0; i-- )
        {
            যদি( step )
            {
                যদি( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                অন্যথা
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = নূতন UMatData(এতত্);
        u->data = u->origdata = data;
        u->size = total;
        যদি(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        ঘূৰাই u;
    }

    তার্কিক allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u) ঘূৰাই মিছা;
        ঘূৰাই সঁচা;
    }

    শূন্য deallocate(UMatData* u) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        যদি( !(u->flags & UMatData::USER_ALLOCATED) )
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
    ঘূৰাই g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ঘূৰাই getDefaultAllocatorMatRef();
}

শূন্য Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, নূতন StdMatAllocator())
}

//==================================================================================================

শূন্য setSize( Mat& m, পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, ধ্রুৱ size_t* _steps, তার্কিক autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    বাবে (পূর্ণাঙ্ক i = _dims-2; i >= 0; i--) {
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

পূর্ণাঙ্ক updateContinuityFlag(পূর্ণাঙ্ক flags, পূর্ণাঙ্ক dims, ধ্রুৱ পূর্ণাঙ্ক* size, ধ্রুৱ size_t* step)
{
    পূর্ণাঙ্ক i, j;
    যদি (dims <= 1)
        ঘূৰাই flags | Mat::CONTINUOUS_FLAG;
    বাবে( i = 0; i < dims; i++ )
    {
        যদি( size[i] > 1 )
            ভঙ্গ;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    বাবে( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        যদি( step[j]*size[j] < step[j-1] )
            ভঙ্গ;
    }

    যদি( j <= i && t == (uint64)(পূর্ণাঙ্ক)t )
        ঘূৰাই flags | Mat::CONTINUOUS_FLAG;
    ঘূৰাই flags & ~Mat::CONTINUOUS_FLAG;
}

শূন্য Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

শূন্য finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    পূর্ণাঙ্ক d = m.dims;
    যদি( d > 2 )
        m.rows = m.cols = -1;
    যদি(m.u)
        m.datastart = m.data = m.u->data;
    যদি( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        যদি( m.size[0] > 0 )
        {
            পূর্ণাঙ্ক lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            বাবে( পূর্ণাঙ্ক i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        অন্যথা
            m.dataend = m.datalimit;
    }
    অন্যথা
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *এতত্ = _s;
}

Mat::Mat(Size _sz, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *এতত্ = _s;
}

Mat::Mat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *এতত্ = _s;
}

Mat::Mat(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _sz, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _sz, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *এতত্ = _s;
}

Mat::Mat(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<পূর্ণাঙ্ক> _shape, পূর্ণাঙ্ক _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *এতত্ = _s;
}

Mat::Mat(std::initializer_list<পূর্ণাঙ্ক> _shape, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *এতত্ = _s;
}

Mat::Mat(ধ্রুৱ Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    যদি( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, শূন্য* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != রিক্ত);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
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

Mat::Mat(Size _sz, পূর্ণাঙ্ক _type, শূন্য* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != রিক্ত);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
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

Mat& Mat::operator = (ধ্রুৱ Mat& m)
{
    যদি( এতত্ != &m )
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
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    ঘূৰাই *এতত্;
}

Mat Mat::clone() ধ্রুৱ
{
    Mat m;
    copyTo(m);
    ঘূৰাই m;
}

শূন্য Mat::assignTo( Mat& m, পূর্ণাঙ্ক _type ) ধ্রুৱ
{
    যদি( _type < 0 )
        m = *এতত্;
    অন্যথা
        convertTo(m, _type);
}

শূন্য Mat::create(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type)
{
    _type &= TYPE_MASK;
    যদি( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        ঘূৰাই;
    পূর্ণাঙ্ক sz[] = {_rows, _cols};
    create(2, sz, _type);
}

শূন্য Mat::create(Size _sz, পূর্ণাঙ্ক _type)
{
    create(_sz.height, _sz.width, _type);
}

শূন্য Mat::createSameSize(প্ৰবেশ_Array m, পূর্ণাঙ্ক type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

শূন্য Mat::fit(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    যদি (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } অন্যথা {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        পূর্ণাঙ্ক _dummy_size = 0;
        setSize(*এতত্, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, সঁচা);
        finalizeHdr(*এতত্);
    }
}

শূন্য Mat::fit(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _shape, পূর্ণাঙ্ক _type)
{
    fit((পূর্ণাঙ্ক)_shape.size(), _shape.data(), _type);
}

শূন্য Mat::fit(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য Mat::fit(std::initializer_list<পূর্ণাঙ্ক> _shape, পূর্ণাঙ্ক _type)
{
    পূর্ণাঙ্ক new_shape[MatShape::MAX_DIMS];
    পূর্ণাঙ্ক new_ndims = (পূর্ণাঙ্ক)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    বাবে (পূর্ণাঙ্ক i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

শূন্য Mat::fit(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type)
{
    _type &= TYPE_MASK;
    পূর্ণাঙ্ক sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

শূন্য Mat::fit(Size _sz, পূর্ণাঙ্ক _type)
{
    fit(_sz.height, _sz.width, _type);
}

শূন্য Mat::fitSameSize(প্ৰবেশ_Array m, পূর্ণাঙ্ক _type)
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
    u = রিক্ত;
    datastart = dataend = datalimit = data = 0;
    বাবে(পূর্ণাঙ্ক i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(পূর্ণাঙ্ক i) ধ্রুৱ
{
    ঘূৰাই step.p[i] / elemSize1();
}

তার্কিক Mat::empty() ধ্রুৱ
{
    ঘূৰাই data == 0 || total() == 0;
}

size_t Mat::total() ধ্রুৱ
{
    যদি( dims <= 2 )
        ঘূৰাই (size_t)rows * cols;
    size_t p = 1;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
        p *= size[i];
    ঘূৰাই p;
}

size_t Mat::total(পূর্ণাঙ্ক startDim, পূর্ণাঙ্ক endDim) ধ্রুৱ
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    পূর্ণাঙ্ক endDim_ = endDim <= dims ? endDim : dims;
    বাবে( পূর্ণাঙ্ক i = startDim; i < endDim_; i++ )
        p *= size[i];
    ঘূৰাই p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = রিক্ত; m.datastart = রিক্ত; m.dataend = রিক্ত; m.datalimit = রিক্ত;
    m.allocator = রিক্ত;
    m.u = রিক্ত;
}


Mat& Mat::operator = (Mat&& m)
{
    যদি (এতত্ == &m)
      ঘূৰাই *এতত্;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = রিক্ত; m.datastart = রিক্ত; m.dataend = রিক্ত; m.datalimit = রিক্ত;
    m.allocator = রিক্ত;
    m.u = রিক্ত;
    ঘূৰাই *এতত্;
}


শূন্য Mat::create(পূর্ণাঙ্ক d0, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type)
{
    পূর্ণাঙ্ক sz1 = 1, d = d0;
    পূর্ণাঙ্ক i;
    যদি (d == 0) {
        d = 1;
        _sizes = (ধ্রুৱ পূর্ণাঙ্ক*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    যদি( data && d == dims && _type == type() )
    {
        বাবে( i = 0; i < d; i++ )
            যদি( size[i] != _sizes[i] )
                ভঙ্গ;
        যদি( i == d )
            ঘূৰাই;
    }

    পূর্ণাঙ্ক _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (_sizes == (এতত্->size.p))
    {
        বাবে(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    যদি( d == 0 )
        ঘূৰাই;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*এতত্, d, _sizes, 0, সঁচা);

    যদি( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        যদি(!a)
            a = a0;
        যত্ন
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        গৃহাণ (...)
        {
            যদি (a == a0)
                ক্ষিপ;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*এতত্);
    size.dims = dims = d0;
}

শূন্য Mat::create(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _sizes, পূর্ণাঙ্ক _type)
{
    create((পূর্ণাঙ্ক)_sizes.size(), _sizes.data(), _type);
}

শূন্য Mat::create(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type)
{
    যদি (_shape.dims < 0) {
        release();
        ঘূৰাই;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য Mat::create(std::initializer_list<পূর্ণাঙ্ক> _shape, পূর্ণাঙ্ক _type)
{
    পূর্ণাঙ্ক new_shape[MatShape::MAX_DIMS];
    পূর্ণাঙ্ক new_ndims = (পূর্ণাঙ্ক)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    বাবে (পূর্ণাঙ্ক i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

শূন্য Mat::copySize(ধ্রুৱ Mat& m)
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
        UMatData* u_ = u;
        u = রিক্ত;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ধ্রুৱ Mat& m, ধ্রুৱ Range& _rowRange, ধ্রুৱ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
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
        বাবে( পূর্ণাঙ্ক i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *এতত্ = m(rs);
        ঘূৰাই;
    }

    *এতত্ = m;
    যত্ন
    {
        যদি( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        যদি( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    গৃহাণ(...)
    {
        release();
        ক্ষিপ;
    }

    updateContinuityFlag();

    যদি( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ধ্রুৱ Mat& m, ধ্রুৱ Rect& roi)
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


Mat::Mat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type, শূন্য* _data, ধ্রুৱ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    যদি (_dims > 0 || _data != nullptr)
        setSize(*এতত্, _dims, _sizes, _steps, সঁচা);
    finalizeHdr(*এতত্);
}


Mat::Mat(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _sizes, পূর্ণাঙ্ক _type, শূন্য* _data, ধ্রুৱ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    পূর্ণাঙ্ক _dims = (পূর্ণাঙ্ক)_sizes.size();
    যদি (_dims > 0 || _data != nullptr)
        setSize(*এতত্, _dims, _sizes.data(), _steps, সঁচা);
    finalizeHdr(*এতত্);
}

Mat::Mat(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, শূন্য* _data, ধ্রুৱ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    যদি (_shape.dims >= 0) {
        setSize(*এতত্, _shape.dims, _shape.p, _steps, সঁচা);
    }
    অন্যথা {
        CV_Assert(!data);
    }
    finalizeHdr(*এতত্);
}

Mat::Mat(std::initializer_list<পূর্ণাঙ্ক> _shape, পূর্ণাঙ্ক _type, শূন্য* _data, ধ্রুৱ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    পূর্ণাঙ্ক new_shape[MatShape::MAX_DIMS];
    পূর্ণাঙ্ক _dims = (পূর্ণাঙ্ক)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    বাবে (পূর্ণাঙ্ক i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    যদি (_dims > 0 || _data != nullptr)
        setSize(*এতত্, _dims, new_shape, _steps, সঁচা);
    finalizeHdr(*এতত্);
}

Mat::Mat(ধ্রুৱ Mat& m, ধ্রুৱ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    পূর্ণাঙ্ক d = m.dims;

    CV_Assert(ranges);
    বাবে( পূর্ণাঙ্ক i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *এতত্ = m;
    বাবে( পূর্ণাঙ্ক i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        যদি( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ধ্রুৱ Mat& m, ধ্রুৱ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    পূর্ণাঙ্ক d = m.dims;

    CV_Assert((পূর্ণাঙ্ক)ranges.size() == d);
    বাবে (পূর্ণাঙ্ক i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *এতত্ = m;
    বাবে (পূর্ণাঙ্ক i = 0; i < d; i++)
    {
        Range r = ranges[i];
        যদি (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(পূর্ণাঙ্ক d) ধ্রুৱ
{
    CV_Assert( dims == 2 );
    Mat m = *এতত্;
    size_t esz = elemSize();
    পূর্ণাঙ্ক len;

    যদি( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    অন্যথা
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    যদি( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ঘূৰাই m;
}


শূন্য Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    যদি( isSubmatrix() )
        *এতত্ = rowRange(0, size.p[0] - (পূর্ণাঙ্ক)nelems);
    অন্যথা
    {
        size.p[0] -= (পূর্ণাঙ্ক)nelems;
        যদি (dims == 2)
            rows = size.p[0];
        অন্যথা যদি (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


শূন্য Mat::ঠেলো_back(ধ্রুৱ শূন্য* elem)
{
    size_t r = size.p[0];
    যদি( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = পূর্ণাঙ্ক(r + 1);
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    বাবে( পূর্ণাঙ্ক i = 1; i < dims; i++ )
        tsz *= size.p[i];
    যদি( esz < step.p[0] || tsz != (uint64)(পূর্ণাঙ্ক)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


শূন্য Mat::reserve(size_t nelems)
{
    ধ্রুৱ size_t MIN_SIZE = 64;

    CV_Assert( (পূর্ণাঙ্ক)nelems >= 0 );
    যদি( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        ঘূৰাই;

    পূর্ণাঙ্ক r = size.p[0];
    যদি( (size_t)r >= nelems)
        ঘূৰাই;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    যদি (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (পূর্ণাঙ্ক)nelems;
    size_t newbytes = newsize.total() * esz;
    যদি (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = পূর্ণাঙ্ক(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    যদি( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *এতত্ = m;
    size.p[0] = r;

    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


শূন্য Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    পূর্ণাঙ্ক mtype = CV_8UC1;
    যদি (!empty())
    {
        যদি (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            ঘূৰাই;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#যদি SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    পূর্ণাঙ্ক newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#অন্যথা
    পূর্ণাঙ্ক newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    পূর্ণাঙ্ক newcols = (পূর্ণাঙ্ক)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


শূন্য Mat::resize(size_t nelems)
{
    পূর্ণাঙ্ক r = size.p[0];
    যদি( r == (পূর্ণাঙ্ক)nelems )
        ঘূৰাই;
    CV_Assert( (পূর্ণাঙ্ক)nelems >= 0 );

    যদি( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (পূর্ণাঙ্ক)nelems;
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


শূন্য Mat::resize(size_t nelems, ধ্রুৱ Scalar& s)
{
    পূর্ণাঙ্ক saveRows = size.p[0];
    resize(nelems);

    যদি( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

শূন্য Mat::push_back(ধ্রুৱ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    যদি( delta == 0 )
        ঘূৰাই;
    যদি( এতত্ == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ঘূৰাই;
    }
    যদি( !data )
    {
        *এতত্ = elems.clone();
        ঘূৰাই;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    তার্কিক eq = size == elems.size;
    size.p[0] = পূর্ণাঙ্ক(r);
    যদি( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    যদি( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    যদি (step0 < minstep) {
        যদি (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    যদি( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += পূর্ণাঙ্ক(delta);
    যদি (dims == 2)
        rows = size.p[0];
    অন্যথা যদি (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    যদি( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    অন্যথা
    {
        Mat part = rowRange(পূর্ণাঙ্ক(r), পূর্ণাঙ্ক(r + delta));
        elems.copyTo(part);
    }
}


শূন্য Mat::locateROI( Size& wholeSize, Point& ofs ) ধ্রুৱ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    যদি( delta1 == 0 )
        ofs.x = ofs.y = 0;
    অন্যথা
    {
        ofs.y = (পূর্ণাঙ্ক)(delta1/step[0]);
        ofs.x = (পূর্ণাঙ্ক)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (পূর্ণাঙ্ক)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (পূর্ণাঙ্ক)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( পূর্ণাঙ্ক dtop, পূর্ণাঙ্ক dbottom, পূর্ণাঙ্ক dleft, পূর্ণাঙ্ক dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    পূর্ণাঙ্ক row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    পূর্ণাঙ্ক col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    যদি(row1 > row2)
        std::swap(row1, row2);
    যদি(col1 > col2)
        std::swap(col1, col2);

    যদি (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } অন্যথা {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ঘূৰাই *এতত্;
}

Mat Mat::reshape(পূর্ণাঙ্ক new_cn, পূর্ণাঙ্ক new_rows) ধ্রুৱ
{
    পূর্ণাঙ্ক cn = channels();
    Mat hdr = *এতত্;

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
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ঘূৰাই hdr;
        }
        CV_Assert( new_rows > 0 );
        পূর্ণাঙ্ক sz[] = { new_rows, (পূর্ণাঙ্ক)(total()*cn/new_rows) };
        ঘূৰাই reshape(new_cn, 2, sz);
    }

    পূর্ণাঙ্ক total_width = cols * cn;

    যদি( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    যদি( new_rows != 0 && new_rows != rows )
    {
        পূর্ণাঙ্ক total_size = total_width * rows;
        যদি( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        যদি( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } অন্যথা {
        hdr.size[0] = hdr.rows = rows;
        যদি (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    পূর্ণাঙ্ক new_width = total_width / new_cn;

    যদি( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_অঙ্ক_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ঘূৰাই hdr;
}

Mat Mat::reshape(পূর্ণাঙ্ক _cn, পূর্ণাঙ্ক _newndims, ধ্রুৱ পূর্ণাঙ্ক* _newsz) ধ্রুৱ
{
    যদি(_newndims == dims)
    {
        যদি(_newsz == 0)
            ঘূৰাই reshape(_cn);
        যদি(_newndims == 2)
            ঘূৰাই reshape(_cn, _newsz[0]);
    }

    যদি (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        যদি (_cn == 0)
            _cn = এতত্->channels();
        অন্যথা
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = এতত্->total() * এতত্->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<পূর্ণাঙ্ক, 4> newsz_buf( (size_t)_newndims );

        পূর্ণাঙ্ক m1_idx = -1;

        বাবে (পূর্ণাঙ্ক i = 0; i < _newndims; i++)
        {
            যদি (_newsz[i] >= 0) {
                যদি (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                অন্যথা
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } অন্যথা {
                যদি (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        যদি (m1_idx >= 0) {
            যদি (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (পূর্ণাঙ্ক)(total_elem1_ref / total_elem1);
        } অন্যথা যদি (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *এতত্;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), রিক্ত, সঁচা);

        ঘূৰাই hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(পূর্ণাঙ্ক _cn, ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _newshape) ধ্রুৱ
{
    পূর্ণাঙ্ক newdims = (পূর্ণাঙ্ক)_newshape.size();
    যদি(newdims == 0 && empty())
    {
        ঘূৰাই *এতত্;
    }

    ঘূৰাই reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(পূর্ণাঙ্ক _cn, ধ্রুৱ MatShape& _newshape) ধ্রুৱ
{
    যদি (_newshape.dims < 0) {
        পূর্ণাঙ্ক newshape[] = {0};
        ঘূৰাই reshape(_cn, 1, newshape);
    }
    ঘূৰাই reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(পূর্ণাঙ্ক _cn, std::initializer_list<পূর্ণাঙ্ক> newshape_) ধ্রুৱ
{
    পূর্ণাঙ্ক newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    বাবে (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ঘূৰাই reshape(_cn, (পূর্ণাঙ্ক)newshape_dims, newshape);
}

Mat Mat::reinterpret(পূর্ণাঙ্ক type) ধ্রুৱ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(এতত্->type()) == CV_ELEM_SIZE(type));
    Mat m = *এতত্;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ঘূৰাই m;
}

Mat Mat::diag(ধ্রুৱ Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    পূর্ণাঙ্ক len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    যদি( d.cols == 1 )
        d.copyTo(md);
    অন্যথা
        transpose(d, md);
    ঘূৰাই m;
}

পূর্ণাঙ্ক Mat::checkVector(পূর্ণাঙ্ক _elemChannels, পূর্ণাঙ্ক _depth, তার্কিক _requireContinuous) ধ্রুৱ
{
    ঘূৰাই data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (পূর্ণাঙ্ক)(total()*channels()/_elemChannels) : -1;
}


স্থির অন্তঃস্থ Size getContinuousSize_(পূর্ণাঙ্ক flags, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    তার্কিক has_int_overflow = sz >= INT_MAX;
    তার্কিক isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ঘূৰাই (isContiguous && !has_int_overflow)
            ? Size((পূর্ণাঙ্ক)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, পূর্ণাঙ্ক widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ঘূৰাই getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, পূর্ণাঙ্ক widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ধ্রুৱ Size sz1 = m1.size();
    যদি (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        তার্কিক is_m1_vector = m1.cols == 1 || m1.rows == 1;
        তার্কিক is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)total_sz;  // vector-column
        তার্কিক isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        তার্কিক has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        যদি (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ঘূৰাই Size(m1.cols * widthScale, m1.rows);
    }
    ঘূৰাই getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, পূর্ণাঙ্ক widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ধ্রুৱ Size sz1 = m1.size();
    যদি (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        তার্কিক is_m1_vector = m1.cols == 1 || m1.rows == 1;
        তার্কিক is_m2_vector = m2.cols == 1 || m2.rows == 1;
        তার্কিক is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)total_sz;  // vector-column
        তার্কিক isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        তার্কিক has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        যদি (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ঘূৰাই Size(m1.cols * widthScale, m1.rows);
    }
    ঘূৰাই getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
