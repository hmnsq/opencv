// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "bufferpool.impl.hpp"

नामाकाश cv {

std::string layoutToString(DataLayout layout)
{
    प्रत्यागम
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

तार्किक चालक == (ध्रुव MatShape& size1, ध्रुव MatShape& size2)
{
    यदि (size1.dims != size2.dims)
        प्रत्यागम असत्य;
    यदि (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        प्रत्यागम असत्य;
    यदि (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        प्रत्यागम असत्य;
    प्रति (पूर्णाङ्क i = 0; i < size1.dims; i++) {
        यदि (size1.p[i] != size2.p[i])
            प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
}

तार्किक चालक != (ध्रुव MatShape& size1, ध्रुव MatShape& size2)
{
    प्रत्यागम !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    प्रत्यागम MatShape(0);
}

शून्य MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    प्रति (पूर्णाङ्क i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

शून्य MatShape::resize(माप_प्रकार newSize, पूर्णाङ्क value)
{
    CV_Assert(newSize < (माप_प्रकार)MAX_DIMS);
    पूर्णाङ्क old_dims = std::max(dims, 0);
    dims = (पूर्णाङ्क)newSize;
    प्रति (पूर्णाङ्क i = old_dims; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::reserve(माप_प्रकार)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

शून्य MatShape::assign(माप_प्रकार newSize, पूर्णाङ्क value)
{
    CV_Assert(newSize < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)newSize;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::assign(पूर्णाङ्क newSize, पूर्णाङ्क value)
{
    assign((माप_प्रकार)newSize, value);
}

शून्य MatShape::assign(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    assign_(begin, end);
}

शून्य MatShape::assign_(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    सूचक_भेद_प्रकार newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (सूचक_भेद_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)newSize;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        p[i] = begin[i];
}

पूर्णाङ्क* MatShape::begin() { प्रत्यागम p; }
ध्रुव पूर्णाङ्क* MatShape::begin() ध्रुव { प्रत्यागम p; }
पूर्णाङ्क* MatShape::end() { प्रत्यागम p + std::max(dims, 0); }
ध्रुव पूर्णाङ्क* MatShape::end() ध्रुव { प्रत्यागम p + std::max(dims, 0); }
पूर्णाङ्क& MatShape::back() { प्रत्यागम p[std::max(dims-1, 0)]; }
ध्रुव पूर्णाङ्क& MatShape::back() ध्रुव { प्रत्यागम p[std::max(dims-1, 0)]; }

शून्य MatShape::push_back(पूर्णाङ्क value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

शून्य MatShape::emplace_back(पूर्णाङ्क value)
{
    push_back(value);
}

शून्य MatShape::insert(पूर्णाङ्क* where, पूर्णाङ्क value)
{
    पूर्णाङ्क old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    प्रति (पूर्णाङ्क i = old_dims-1; i >= (पूर्णाङ्क)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

शून्य MatShape::insert(पूर्णाङ्क* where, माप_प्रकार गिनती_2, पूर्णाङ्क value)
{
    पूर्णाङ्क old_dims = std::max(dims, 0);
    CV_Assert((माप_प्रकार)(old_dims+गिनती_2) < (माप_प्रकार)MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णाङ्क)(old_dims+गिनती_2);
    प्रति (पूर्णाङ्क i = old_dims-1; i >= (पूर्णाङ्क)ofs; i--)
        p[i+गिनती_2] = p[i];
    प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)गिनती_2; i++)
        p[i+ofs] = value;
}

शून्य MatShape::insert(पूर्णाङ्क* where, पूर्णाङ्क गिनती_2, पूर्णाङ्क value)
{
    insert(where, (माप_प्रकार)गिनती_2, value);
}

शून्य MatShape::insert(पूर्णाङ्क* where, ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    जोड़ो(where, begin, end);
}

शून्य MatShape::जोड़ो(पूर्णाङ्क* where, ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    पूर्णाङ्क old_dims = std::max(dims, 0);
    सूचक_भेद_प्रकार delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णाङ्क)(old_dims+delta);
    प्रति (पूर्णाङ्क i = old_dims-1; i >= (पूर्णाङ्क)ofs; i--)
        p[i+delta] = p[i];
    प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)delta; i++)
        p[i+ofs] = begin[i];
}

शून्य MatShape::erase(पूर्णाङ्क* where)
{
    CV_Assert(dims > 0);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    यदि (ofs == dims)
        प्रत्यागम;
    dims--;
    प्रति (पूर्णाङ्क i = (पूर्णाङ्क)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

माप_प्रकार MatShape::total() ध्रुव
{
    माप_प्रकार result = 1;
    यदि (dims < 0)
        प्रत्यागम 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        result *= p[i];
    प्रत्यागम result;
}

std::vector<पूर्णाङ्क> MatShape::vec() ध्रुव
{
    यदि (dims < 0)
        प्रत्यागम std::vector<पूर्णाङ्क>(1, 0);
    प्रत्यागम std::vector<पूर्णाङ्क>(p, p + dims);
}

std::string MatShape::str() ध्रुव
{
    std::stringstream sstrm;
    यदि (empty()) {
        sstrm << "<empty>";
    } अन्यथा यदि (dims == 0) {
        sstrm << "<scalar>";
    } अन्यथा {
        sstrm << "[";
        प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    प्रत्यागम sstrm.str();
}

स्थिर शून्य finalizeBlockLayout(MatShape& size, पूर्णाङ्क C=0)
{
    यदि (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        पूर्णाङ्क C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } अन्यथा {
        size.C = 0;
    }
    प्रति (पूर्णाङ्क i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    यदि (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(माप_प्रकार dims_, ध्रुव पूर्णाङ्क* size_, DataLayout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*एतत्, C_);
}

MatShape::MatShape(माप_प्रकार dims_, पूर्णाङ्क value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*एतत्, 0);
}

MatShape::MatShape(std::initializer_list<पूर्णाङ्क> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)shape.size();
    स्वतः it = shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*एतत्, 0);
}

MatShape::MatShape(पूर्णाङ्क dims_, पूर्णाङ्क value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*एतत्, 0);
}

MatShape::MatShape(ध्रुव std::vector<पूर्णाङ्क>& shape_, DataLayout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    माप_प्रकार shape_size = shape_.size();
    CV_Assert(shape_size < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णाङ्क)shape_size;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*एतत्, C_);
}

MatShape::MatShape(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end, DataLayout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    सूचक_भेद_प्रकार shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (पूर्णाङ्क)shape_size;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*एतत्, C_);
}

MatShape::MatShape(ध्रुव MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    प्रति (पूर्णाङ्क i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::चालक = (ध्रुव MatShape& shape)
{
    यदि (एतत् != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        प्रति (पूर्णाङ्क i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    प्रत्यागम *एतत्;
}

तार्किक MatShape::hasSymbols() ध्रुव
{
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        यदि (p[i] < 0)
            प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

पूर्णाङ्क MatShape::channels() ध्रुव
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    प्रत्यागम layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, पूर्णाङ्क C0) ध्रुव
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *एतत्;
    newsize.layout = newLayout;

    यदि (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        पूर्णाङ्क Corig = channels();
        newsize.C = Corig;

        यदि (layout == DATA_LAYOUT_NHWC) {
            प्रति (पूर्णाङ्क i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } अन्यथा यदि (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        यदि (newLayout == DATA_LAYOUT_NHWC) {
            प्रति (पूर्णाङ्क i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } अन्यथा {
        CV_Assert_N(C0 <= 1);

        यदि (newLayout == layout)
            प्रत्यागम newsize;

        // NHWC => NCHW
        यदि (newLayout == DATA_LAYOUT_NCHW) {
            प्रति (पूर्णाङ्क i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } अन्यथा {
            // NCHW => NHWC
            प्रति (पूर्णाङ्क i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    प्रत्यागम newsize;
}

स्थिर पूर्णाङ्क normalizeShapeAxis(पूर्णाङ्क axis, पूर्णाङ्क dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    प्रत्यागम axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
स्थिर MatShape toSemantic(ध्रुव MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    प्रत्यागम shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
स्थिर MatShape toBlockOf(MatShape result, ध्रुव MatShape& src, ध्रुव वर्ण* op)
{
    यदि (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (पूर्णाङ्क)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    प्रत्यागम result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ध्रुव std::vector<पूर्णाङ्क>& axes) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == DATA_LAYOUT_BLOCK)
        प्रत्यागम toBlockOf(toSemantic(*एतत्).squeeze(axes), *एतत्, "squeeze");
    तार्किक हटाओ[MAX_DIMS] = {असत्य};
    यदि (axes.empty())
    {
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
            हटाओ[i] = p[i] == 1;
    }
    प्रति (पूर्णाङ्क a : axes)
    {
        पूर्णाङ्क i = normalizeShapeAxis(a, dims);
        CV_Check(a, !हटाओ[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        हटाओ[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        यदि (!हटाओ[i])
            result.p[result.dims++] = p[i];
    प्रत्यागम result;
}

MatShape MatShape::unsqueeze(ध्रुव std::vector<पूर्णाङ्क>& axes) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == DATA_LAYOUT_BLOCK)
        प्रत्यागम toBlockOf(toSemantic(*एतत्).unsqueeze(axes), *एतत्, "unsqueeze");
    ध्रुव पूर्णाङ्क outDims = dims + (पूर्णाङ्क)axes.size();
    CV_CheckLE(outDims, (पूर्णाङ्क)MAX_DIMS, "unsqueeze: too many dimensions");
    तार्किक insert[MAX_DIMS] = {असत्य};
    प्रति (पूर्णाङ्क a : axes)
    {
        पूर्णाङ्क i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    प्रति (पूर्णाङ्क i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    प्रत्यागम result;
}

MatShape MatShape::flatten(पूर्णाङ्क startAxis, पूर्णाङ्क endAxis) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == DATA_LAYOUT_BLOCK)
        प्रत्यागम toBlockOf(toSemantic(*एतत्).flatten(startAxis, endAxis), *एतत्, "flatten");
    यदि (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        प्रत्यागम MatShape({1});
    }
    पूर्णाङ्क start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    प्रति (पूर्णाङ्क i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((द्विगुण)merged, (द्विगुण)पूर्णांक_उच्च, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        यदि (i == start)
            result.p[result.dims++] = (पूर्णाङ्क)merged;
        अन्यथा यदि (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    प्रत्यागम result;
}

MatShape MatShape::reshape(ध्रुव MatShape& newShape, तार्किक allowZero) ध्रुव
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    यदि (layout == DATA_LAYOUT_BLOCK)
        प्रत्यागम toBlockOf(toSemantic(*एतत्).reshape(newShape, allowZero), *एतत्, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    पूर्णाङ्क inferred = -1;
    int64 known = 1;
    प्रति (पूर्णाङ्क i = 0; i < result.dims; i++)
    {
        पूर्णाङ्क& sz = result.p[i];
        यदि (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            अनुवर्त;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        यदि (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ध्रुव int64 total_ = (int64)total();
    यदि (inferred >= 0)
    {
        CV_Check((द्विगुण)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((द्विगुण)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((द्विगुण)(total_/known), (द्विगुण)पूर्णांक_उच्च, "reshape: the result is too big");
        result.p[inferred] = (पूर्णाङ्क)(total_/known);
    }
    अन्यथा यदि (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (दीर्घ दीर्घ)known, (दीर्घ दीर्घ)total_));
    प्रत्यागम result;
}

MatShape MatShape::expand(ध्रुव MatShape& another) ध्रुव
{
    यदि (dims == 0)
        प्रत्यागम another;
    यदि (another.dims == 0)
        प्रत्यागम *एतत्;

    यदि ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    यदि (dims < 0 || another.dims < 0)
        प्रत्यागम result;

    result = *एतत्;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    प्रति (पूर्णाङ्क i = result.dims-1; i >= 0; i--) {
        पूर्णाङ्क i1 = i - (result.dims - dims);
        पूर्णाङ्क i2 = i - (result.dims - another.dims);
        पूर्णाङ्क sz1 = i1 < 0 ? 1 : p[i1];
        पूर्णाङ्क sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    प्रत्यागम result;
}

/////////////////////////// MatAllocator ////////////////////////////

शून्य MatAllocator::map(UMatData*, AccessFlag) ध्रुव
{
}

शून्य MatAllocator::unmap(UMatData* u) ध्रुव
{
    यदि(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

शून्य MatAllocator::download(UMatData* u, शून्य* dstptr,
         पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
         ध्रुव माप_प्रकार srcofs[], ध्रुव माप_प्रकार srcstep[],
         ध्रुव माप_प्रकार dststep[]) ध्रुव
{
    यदि(!u)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        यदि( sz[i] == 0 )
            प्रत्यागम;
        यदि( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (पूर्णाङ्क)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ध्रुव Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    प्रति( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}


शून्य MatAllocator::upload(UMatData* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
                    ध्रुव माप_प्रकार dstofs[], ध्रुव माप_प्रकार dststep[],
                    ध्रुव माप_प्रकार srcstep[]) ध्रुव
{
    यदि(!u)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        यदि( sz[i] == 0 )
            प्रत्यागम;
        यदि( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णाङ्क)sz[i];
    }

    Mat src(dims, isz, CV_8U, (शून्य*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ध्रुव Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    प्रति( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

शून्य MatAllocator::copy(UMatData* usrc, UMatData* udst, पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
                  ध्रुव माप_प्रकार srcofs[], ध्रुव माप_प्रकार srcstep[],
                  ध्रुव माप_प्रकार dstofs[], ध्रुव माप_प्रकार dststep[], तार्किक /*sync*/) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि(!usrc || !udst)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        यदि( sz[i] == 0 )
            प्रत्यागम;
        यदि( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        यदि( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णाङ्क)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ध्रुव Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    प्रति( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ध्रुव वर्ण* id) ध्रुव
{
    CV_UNUSED(id);
    स्थिर DummyBufferPoolController dummy;
    प्रत्यागम &dummy;
}

वर्ग StdMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* data0, माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        माप_प्रकार total = CV_ELEM_SIZE(type);
        प्रति( पूर्णाङ्क i = dims-1; i >= 0; i-- )
        {
            यदि( step )
            {
                यदि( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                अन्यथा
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = नूतन UMatData(एतत्);
        u->data = u->origdata = data;
        u->size = total;
        यदि(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        प्रत्यागम u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        यदि(!u) प्रत्यागम असत्य;
        प्रत्यागम सत्य;
    }

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        यदि( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

स्थिर
MatAllocator*& getDefaultAllocatorMatRef()
{
    स्थिर MatAllocator* g_matAllocator = Mat::getStdAllocator();
    प्रत्यागम g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    प्रत्यागम getDefaultAllocatorMatRef();
}

शून्य Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, नूतन StdMatAllocator())
}

//==================================================================================================

शून्य setSize( Mat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव माप_प्रकार* _steps, तार्किक autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    प्रति (पूर्णाङ्क i = _dims-2; i >= 0; i--) {
        माप_प्रकार autostep = m.size[i+1]*m.step[i+1];
        यदि (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा यदि (autoSteps) {
            m.step[i] = autostep;
        } अन्यथा {
            m.step[i] = 0;
        }
    }

    यदि( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.cols = m.rows = -1;
    }
}

पूर्णाङ्क updateContinuityFlag(पूर्णाङ्क flags, पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* size, ध्रुव माप_प्रकार* step)
{
    पूर्णाङ्क i, j;
    यदि (dims <= 1)
        प्रत्यागम flags | Mat::CONTINUOUS_FLAG;
    प्रति( i = 0; i < dims; i++ )
    {
        यदि( size[i] > 1 )
            भङ्ग;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    प्रति( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        यदि( step[j]*size[j] < step[j-1] )
            भङ्ग;
    }

    यदि( j <= i && t == (uint64)(पूर्णाङ्क)t )
        प्रत्यागम flags | Mat::CONTINUOUS_FLAG;
    प्रत्यागम flags & ~Mat::CONTINUOUS_FLAG;
}

शून्य Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

शून्य finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    पूर्णाङ्क d = m.dims;
    यदि( d > 2 )
        m.rows = m.cols = -1;
    यदि(m.u)
        m.datastart = m.data = m.u->data;
    यदि( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        यदि( m.size[0] > 0 )
        {
            पूर्णाङ्क lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            प्रति( पूर्णाङ्क i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        अन्यथा
            m.dataend = m.datalimit;
    }
    अन्यथा
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *एतत् = _s;
}

Mat::Mat(Size _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *एतत् = _s;
}

Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *एतत् = _s;
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    यदि( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, शून्य* _data, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != रिक्त);

    माप_प्रकार esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols * esz;
    यदि( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_Assert( _step >= minstep );
        यदि (_step % esz1 != 0)
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

Mat::Mat(Size _sz, पूर्णाङ्क _type, शून्य* _data, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != रिक्त);

    माप_प्रकार esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols*esz;
    यदि( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_CheckGE(_step, minstep, "");

        यदि (_step % esz1 != 0)
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

Mat& Mat::चालक = (ध्रुव Mat& m)
{
    यदि( एतत् != &m )
    {
        यदि( m.u )
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
    प्रत्यागम *एतत्;
}

Mat Mat::clone() ध्रुव
{
    Mat m;
    copyTo(m);
    प्रत्यागम m;
}

शून्य Mat::assignTo( Mat& m, पूर्णाङ्क _type ) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convertTo(m, _type);
}

शून्य Mat::create(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type)
{
    _type &= TYPE_MASK;
    यदि( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        प्रत्यागम;
    पूर्णाङ्क sz[] = {_rows, _cols};
    create(2, sz, _type);
}

शून्य Mat::create(Size _sz, पूर्णाङ्क _type)
{
    create(_sz.height, _sz.width, _type);
}

शून्य Mat::createSameSize(निवेश_Array m, पूर्णाङ्क type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

शून्य Mat::fit(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    माप_प्रकार oldTotalBytes = u ? u->size : 0;
    माप_प्रकार esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    माप_प्रकार newTotalBytes = newTotal*esz;
    यदि (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        पूर्णाङ्क _dummy_size = 0;
        setSize(*एतत्, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, सत्य);
        finalizeHdr(*एतत्);
    }
}

शून्य Mat::fit(ध्रुव std::vector<पूर्णाङ्क>& _shape, पूर्णाङ्क _type)
{
    fit((पूर्णाङ्क)_shape.size(), _shape.data(), _type);
}

शून्य Mat::fit(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य Mat::fit(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type)
{
    पूर्णाङ्क new_shape[MatShape::MAX_DIMS];
    पूर्णाङ्क new_ndims = (पूर्णाङ्क)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

शून्य Mat::fit(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type)
{
    _type &= TYPE_MASK;
    पूर्णाङ्क sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

शून्य Mat::fit(Size _sz, पूर्णाङ्क _type)
{
    fit(_sz.height, _sz.width, _type);
}

शून्य Mat::fitSameSize(निवेश_Array m, पूर्णाङ्क _type)
{
    fit(m.shape(), _type);
}

शून्य Mat::addref()
{
    यदि( u )
        CV_XADD(&u->refcount, 1);
}

शून्य Mat::release()
{
    यदि( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = रिक्त;
    datastart = dataend = datalimit = data = 0;
    प्रति(पूर्णाङ्क i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

माप_प्रकार Mat::step1(पूर्णाङ्क i) ध्रुव
{
    प्रत्यागम step.p[i] / elemSize1();
}

तार्किक Mat::empty() ध्रुव
{
    प्रत्यागम data == 0 || total() == 0;
}

माप_प्रकार Mat::total() ध्रुव
{
    यदि( dims <= 2 )
        प्रत्यागम (माप_प्रकार)rows * cols;
    माप_प्रकार p = 1;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
        p *= size[i];
    प्रत्यागम p;
}

माप_प्रकार Mat::total(पूर्णाङ्क startDim, पूर्णाङ्क endDim) ध्रुव
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    माप_प्रकार p = 1;
    पूर्णाङ्क endDim_ = endDim <= dims ? endDim : dims;
    प्रति( पूर्णाङ्क i = startDim; i < endDim_; i++ )
        p *= size[i];
    प्रत्यागम p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = रिक्त; m.datastart = रिक्त; m.dataend = रिक्त; m.datalimit = रिक्त;
    m.allocator = रिक्त;
    m.u = रिक्त;
}


Mat& Mat::चालक = (Mat&& m)
{
    यदि (एतत् == &m)
      प्रत्यागम *एतत्;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = रिक्त; m.datastart = रिक्त; m.dataend = रिक्त; m.datalimit = रिक्त;
    m.allocator = रिक्त;
    m.u = रिक्त;
    प्रत्यागम *एतत्;
}


शून्य Mat::create(पूर्णाङ्क d0, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    पूर्णाङ्क sz1 = 1, d = d0;
    पूर्णाङ्क i;
    यदि (d == 0) {
        d = 1;
        _sizes = (ध्रुव पूर्णाङ्क*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    यदि( data && d == dims && _type == type() )
    {
        प्रति( i = 0; i < d; i++ )
            यदि( size[i] != _sizes[i] )
                भङ्ग;
        यदि( i == d )
            प्रत्यागम;
    }

    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    यदि (_sizes == (एतत्->size.p))
    {
        प्रति(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    यदि( d == 0 )
        प्रत्यागम;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*एतत्, d, _sizes, 0, सत्य);

    यदि( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        यदि(!a)
            a = a0;
        यत्न
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        गृहाण (...)
        {
            यदि (a == a0)
                क्षिप;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (माप_प्रकार)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*एतत्);
    size.dims = dims = d0;
}

शून्य Mat::create(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type)
{
    create((पूर्णाङ्क)_sizes.size(), _sizes.data(), _type);
}

शून्य Mat::create(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    यदि (_shape.dims < 0) {
        release();
        प्रत्यागम;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य Mat::create(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type)
{
    पूर्णाङ्क new_shape[MatShape::MAX_DIMS];
    पूर्णाङ्क new_ndims = (पूर्णाङ्क)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

शून्य Mat::copySize(ध्रुव Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

शून्य Mat::deallocate()
{
    यदि(u)
    {
        UMatData* u_ = u;
        u = रिक्त;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ध्रुव Mat& m, ध्रुव Range& _rowRange, ध्रुव Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    यदि( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        प्रति( पूर्णाङ्क i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *एतत् = m(rs);
        प्रत्यागम;
    }

    *एतत् = m;
    यत्न
    {
        यदि( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        यदि( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    गृहाण(...)
    {
        release();
        क्षिप;
    }

    updateContinuityFlag();

    यदि( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ध्रुव Mat& m, ध्रुव Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    माप_प्रकार esz = CV_ELEM_SIZE(flags);
    data += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    यदि( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    यदि( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type, शून्य* _data, ध्रुव माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    यदि (_dims > 0 || _data != nullptr)
        setSize(*एतत्, _dims, _sizes, _steps, सत्य);
    finalizeHdr(*एतत्);
}


Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type, शून्य* _data, ध्रुव माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    पूर्णाङ्क _dims = (पूर्णाङ्क)_sizes.size();
    यदि (_dims > 0 || _data != nullptr)
        setSize(*एतत्, _dims, _sizes.data(), _steps, सत्य);
    finalizeHdr(*एतत्);
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, शून्य* _data, ध्रुव माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    यदि (_shape.dims >= 0) {
        setSize(*एतत्, _shape.dims, _shape.p, _steps, सत्य);
    }
    अन्यथा {
        CV_Assert(!data);
    }
    finalizeHdr(*एतत्);
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type, शून्य* _data, ध्रुव माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णाङ्क new_shape[MatShape::MAX_DIMS];
    पूर्णाङ्क _dims = (पूर्णाङ्क)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    यदि (_dims > 0 || _data != nullptr)
        setSize(*एतत्, _dims, new_shape, _steps, सत्य);
    finalizeHdr(*एतत्);
}

Mat::Mat(ध्रुव Mat& m, ध्रुव Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णाङ्क d = m.dims;

    CV_Assert(ranges);
    प्रति( पूर्णाङ्क i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *एतत् = m;
    प्रति( पूर्णाङ्क i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        यदि( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ध्रुव Mat& m, ध्रुव std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    पूर्णाङ्क d = m.dims;

    CV_Assert((पूर्णाङ्क)ranges.size() == d);
    प्रति (पूर्णाङ्क i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *एतत् = m;
    प्रति (पूर्णाङ्क i = 0; i < d; i++)
    {
        Range r = ranges[i];
        यदि (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(पूर्णाङ्क d) ध्रुव
{
    CV_Assert( dims == 2 );
    Mat m = *एतत्;
    माप_प्रकार esz = elemSize();
    पूर्णाङ्क len;

    यदि( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    अन्यथा
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    यदि( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    प्रत्यागम m;
}


शून्य Mat::pop_back(माप_प्रकार nelems)
{
    CV_Assert( nelems <= (माप_प्रकार)size.p[0] );
    CV_Assert( dims >= 1 );

    यदि( isSubmatrix() )
        *एतत् = rowRange(0, size.p[0] - (पूर्णाङ्क)nelems);
    अन्यथा
    {
        size.p[0] -= (पूर्णाङ्क)nelems;
        यदि (dims == 2)
            rows = size.p[0];
        अन्यथा यदि (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


शून्य Mat::धकेलो_back(ध्रुव शून्य* elem)
{
    माप_प्रकार r = size.p[0];
    यदि( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    माप_प्रकार esz = elemSize();
    स_नकल(data + r*step.p[0], elem, esz);
    size.p[0] = पूर्णाङ्क(r + 1);
    यदि (dims == 2)
        rows = size.p[0];
    अन्यथा यदि (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    प्रति( पूर्णाङ्क i = 1; i < dims; i++ )
        tsz *= size.p[i];
    यदि( esz < step.p[0] || tsz != (uint64)(पूर्णाङ्क)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


शून्य Mat::reserve(माप_प्रकार nelems)
{
    ध्रुव माप_प्रकार MIN_SIZE = 64;

    CV_Assert( (पूर्णाङ्क)nelems >= 0 );
    यदि( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        प्रत्यागम;

    पूर्णाङ्क r = size.p[0];
    यदि( (माप_प्रकार)r >= nelems)
        प्रत्यागम;

    MatShape newsize = size;
    माप_प्रकार esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    यदि (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (पूर्णाङ्क)nelems;
    माप_प्रकार newbytes = newsize.total() * esz;
    यदि (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = पूर्णाङ्क(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    यदि( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *एतत् = m;
    size.p[0] = r;

    यदि (dims == 2)
        rows = size.p[0];
    अन्यथा यदि (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


शून्य Mat::reserveBuffer(माप_प्रकार nbytes)
{
    माप_प्रकार esz = 1;
    पूर्णाङ्क mtype = CV_8UC1;
    यदि (!empty())
    {
        यदि (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            प्रत्यागम;
        esz = elemSize();
        mtype = type();
    }

    माप_प्रकार nelems = (nbytes - 1) / esz + 1;

#यदि SIZE_MAX > अच_पूर्णांक_उच्च
    CV_Assert(nelems <= माप_प्रकार(पूर्णांक_उच्च)*माप_प्रकार(पूर्णांक_उच्च));
    पूर्णाङ्क newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x400*माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x100000 * माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x40000000 * माप_प्रकार(पूर्णांक_उच्च) ?
                  माप_प्रकार(पूर्णांक_उच्च) : 0x40000000 : 0x100000 : 0x400 : 1;
#अन्यथा
    पूर्णाङ्क newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? 2 : 1;
#endif
    पूर्णाङ्क newcols = (पूर्णाङ्क)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


शून्य Mat::resize(माप_प्रकार nelems)
{
    पूर्णाङ्क r = size.p[0];
    यदि( r == (पूर्णाङ्क)nelems )
        प्रत्यागम;
    CV_Assert( (पूर्णाङ्क)nelems >= 0 );

    यदि( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (पूर्णाङ्क)nelems;
    यदि (dims == 2)
        rows = size.p[0];
    अन्यथा यदि (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


शून्य Mat::resize(माप_प्रकार nelems, ध्रुव Scalar& s)
{
    पूर्णाङ्क saveRows = size.p[0];
    resize(nelems);

    यदि( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

शून्य Mat::push_back(ध्रुव Mat& elems)
{
    माप_प्रकार r = size.p[0];
    माप_प्रकार delta = elems.size.p[0];
    यदि( delta == 0 )
        प्रत्यागम;
    यदि( एतत् == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        प्रत्यागम;
    }
    यदि( !data )
    {
        *एतत् = elems.clone();
        प्रत्यागम;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    तार्किक eq = size == elems.size;
    size.p[0] = पूर्णाङ्क(r);
    यदि( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    यदि( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    माप_प्रकार esz = elemSize();
    माप_प्रकार minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    माप_प्रकार step0 = step.p[0];
    यदि (step0 < minstep) {
        यदि (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    यदि( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += पूर्णाङ्क(delta);
    यदि (dims == 2)
        rows = size.p[0];
    अन्यथा यदि (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    यदि( isContinuous() && elems.isContinuous() )
        स_नकल(data + r*step0, elems.data, elems.total()*elems.elemSize());
    अन्यथा
    {
        Mat part = rowRange(पूर्णाङ्क(r), पूर्णाङ्क(r + delta));
        elems.copyTo(part);
    }
}


शून्य Mat::locateROI( Size& wholeSize, Point& ofs ) ध्रुव
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    माप_प्रकार esz = elemSize(), minstep;
    सूचक_भेद_प्रकार delta1 = data - datastart, delta2 = dataend - datastart;

    यदि( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णाङ्क)(delta1/step[0]);
        ofs.x = (पूर्णाङ्क)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (पूर्णाङ्क)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (पूर्णाङ्क)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( पूर्णाङ्क dtop, पूर्णाङ्क dbottom, पूर्णाङ्क dleft, पूर्णाङ्क dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    माप_प्रकार esz = elemSize();
    locateROI( wholeSize, ofs );
    पूर्णाङ्क row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    पूर्णाङ्क col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    यदि(row1 > row2)
        std::swap(row1, row2);
    यदि(col1 > col2)
        std::swap(col1, col2);

    यदि (dims == 1) {
        data += (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } अन्यथा {
        data += (row1 - ofs.y)*(std::सूचक_भेद_प्रकार)step + (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    प्रत्यागम *एतत्;
}

Mat Mat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_rows) ध्रुव
{
    पूर्णाङ्क cn = channels();
    Mat hdr = *एतत्;

    यदि( new_cn == 0 )
        new_cn = cn;

    यदि( dims > 2 )
    {
        यदि( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            प्रत्यागम hdr;
        }
        CV_Assert( new_rows > 0 );
        पूर्णाङ्क sz[] = { new_rows, (पूर्णाङ्क)(total()*cn/new_rows) };
        प्रत्यागम reshape(new_cn, 2, sz);
    }

    पूर्णाङ्क total_width = cols * cn;

    यदि( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    यदि( new_rows != 0 && new_rows != rows )
    {
        पूर्णाङ्क total_size = total_width * rows;
        यदि( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        यदि( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.rows = rows;
        यदि (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    पूर्णाङ्क new_width = total_width / new_cn;

    यदि( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_अंक_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    प्रत्यागम hdr;
}

Mat Mat::reshape(पूर्णाङ्क _cn, पूर्णाङ्क _newndims, ध्रुव पूर्णाङ्क* _newsz) ध्रुव
{
    यदि(_newndims == dims)
    {
        यदि(_newsz == 0)
            प्रत्यागम reshape(_cn);
        यदि(_newndims == 2)
            प्रत्यागम reshape(_cn, _newsz[0]);
    }

    यदि (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        यदि (_cn == 0)
            _cn = एतत्->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        माप_प्रकार total_elem1_ref = एतत्->total() * एतत्->channels();
        माप_प्रकार total_elem1 = _cn;

        AutoBuffer<पूर्णाङ्क, 4> newsz_buf( (माप_प्रकार)_newndims );

        पूर्णाङ्क m1_idx = -1;

        प्रति (पूर्णाङ्क i = 0; i < _newndims; i++)
        {
            यदि (_newsz[i] >= 0) {
                यदि (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (माप_प्रकार)newsz_buf[i];
            } अन्यथा {
                यदि (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        यदि (m1_idx >= 0) {
            यदि (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णाङ्क)(total_elem1_ref / total_elem1);
        } अन्यथा यदि (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *एतत्;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), रिक्त, सत्य);

        प्रत्यागम hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(पूर्णाङ्क _cn, ध्रुव std::vector<पूर्णाङ्क>& _newshape) ध्रुव
{
    पूर्णाङ्क newdims = (पूर्णाङ्क)_newshape.size();
    यदि(newdims == 0 && empty())
    {
        प्रत्यागम *एतत्;
    }

    प्रत्यागम reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(पूर्णाङ्क _cn, ध्रुव MatShape& _newshape) ध्रुव
{
    यदि (_newshape.dims < 0) {
        पूर्णाङ्क newshape[] = {0};
        प्रत्यागम reshape(_cn, 1, newshape);
    }
    प्रत्यागम reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(पूर्णाङ्क _cn, std::initializer_list<पूर्णाङ्क> newshape_) ध्रुव
{
    पूर्णाङ्क newshape[MatShape::MAX_DIMS];
    माप_प्रकार i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (माप_प्रकार)MatShape::MAX_DIMS);
    स्वतः it = newshape_.begin();
    प्रति (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    प्रत्यागम reshape(_cn, (पूर्णाङ्क)newshape_dims, newshape);
}

Mat Mat::reinterpret(पूर्णाङ्क type) ध्रुव
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(एतत्->type()) == CV_ELEM_SIZE(type));
    Mat m = *एतत्;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    प्रत्यागम m;
}

Mat Mat::diag(ध्रुव Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    पूर्णाङ्क len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    यदि( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    प्रत्यागम m;
}

पूर्णाङ्क Mat::checkVector(पूर्णाङ्क _elemChannels, पूर्णाङ्क _depth, तार्किक _requireContinuous) ध्रुव
{
    प्रत्यागम data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णाङ्क)(total()*channels()/_elemChannels) : -1;
}


स्थिर अन्तःस्थ Size getContinuousSize_(पूर्णाङ्क flags, पूर्णाङ्क cols, पूर्णाङ्क rows, पूर्णाङ्क widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    तार्किक has_int_overflow = sz >= पूर्णांक_उच्च;
    तार्किक isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    प्रत्यागम (isContiguous && !has_int_overflow)
            ? Size((पूर्णाङ्क)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, पूर्णाङ्क widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    प्रत्यागम getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, पूर्णाङ्क widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ध्रुव Size sz1 = m1.size();
    यदि (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        तार्किक is_m1_vector = m1.cols == 1 || m1.rows == 1;
        तार्किक is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        पूर्णाङ्क total = (पूर्णाङ्क)total_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_overflow = ((int64)total_sz * widthScale) >= पूर्णांक_उच्च;
        यदि (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        प्रत्यागम Size(m1.cols * widthScale, m1.rows);
    }
    प्रत्यागम getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णाङ्क widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ध्रुव Size sz1 = m1.size();
    यदि (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        तार्किक is_m1_vector = m1.cols == 1 || m1.rows == 1;
        तार्किक is_m2_vector = m2.cols == 1 || m2.rows == 1;
        तार्किक is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        पूर्णाङ्क total = (पूर्णाङ्क)total_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_overflow = ((int64)total_sz * widthScale) >= पूर्णांक_उच्च;
        यदि (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        प्रत्यागम Size(m1.cols * widthScale, m1.rows);
    }
    प्रत्यागम getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
