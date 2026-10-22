// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "bufferpool.impl.hpp"

नामाकाश cv {

std::string layoutToString(DataLayout layout)
{
    परत
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

तार्किक चालक == (स्थिरांक MatShape& size1, स्थिरांक MatShape& size2)
{
    जर (size1.dims != size2.dims)
        परत असत्य;
    जर (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        परत असत्य;
    जर (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        परत असत्य;
    साठी (पूर्णांक i = 0; i < size1.dims; i++) {
        जर (size1.p[i] != size2.p[i])
            परत असत्य;
    }
    परत सत्य;
}

तार्किक चालक != (स्थिरांक MatShape& size1, स्थिरांक MatShape& size2)
{
    परत !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    परत MatShape(0);
}

शून्य MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    साठी (पूर्णांक i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

शून्य MatShape::resize(माप_प्रकार newSize, पूर्णांक value)
{
    CV_Assert(newSize < (माप_प्रकार)MAX_DIMS);
    पूर्णांक old_dims = std::max(dims, 0);
    dims = (पूर्णांक)newSize;
    साठी (पूर्णांक i = old_dims; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::reserve(माप_प्रकार)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

शून्य MatShape::assign(माप_प्रकार newSize, पूर्णांक value)
{
    CV_Assert(newSize < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)newSize;
    साठी (पूर्णांक i = 0; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::assign(पूर्णांक newSize, पूर्णांक value)
{
    assign((माप_प्रकार)newSize, value);
}

शून्य MatShape::assign(स्थिरांक पूर्णांक* begin, स्थिरांक पूर्णांक* end)
{
    assign_(begin, end);
}

शून्य MatShape::assign_(स्थिरांक पूर्णांक* begin, स्थिरांक पूर्णांक* end)
{
    सूचक_भेद_प्रकार newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (सूचक_भेद_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)newSize;
    साठी (पूर्णांक i = 0; i < dims; i++)
        p[i] = begin[i];
}

पूर्णांक* MatShape::begin() { परत p; }
स्थिरांक पूर्णांक* MatShape::begin() स्थिरांक { परत p; }
पूर्णांक* MatShape::end() { परत p + std::max(dims, 0); }
स्थिरांक पूर्णांक* MatShape::end() स्थिरांक { परत p + std::max(dims, 0); }
पूर्णांक& MatShape::back() { परत p[std::max(dims-1, 0)]; }
स्थिरांक पूर्णांक& MatShape::back() स्थिरांक { परत p[std::max(dims-1, 0)]; }

शून्य MatShape::push_back(पूर्णांक value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

शून्य MatShape::emplace_back(पूर्णांक value)
{
    push_back(value);
}

शून्य MatShape::insert(पूर्णांक* where, पूर्णांक value)
{
    पूर्णांक old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    साठी (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

शून्य MatShape::insert(पूर्णांक* where, माप_प्रकार मोजणी_2, पूर्णांक value)
{
    पूर्णांक old_dims = std::max(dims, 0);
    CV_Assert((माप_प्रकार)(old_dims+मोजणी_2) < (माप_प्रकार)MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णांक)(old_dims+मोजणी_2);
    साठी (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+मोजणी_2] = p[i];
    साठी (पूर्णांक i = 0; i < (पूर्णांक)मोजणी_2; i++)
        p[i+ofs] = value;
}

शून्य MatShape::insert(पूर्णांक* where, पूर्णांक मोजणी_2, पूर्णांक value)
{
    insert(where, (माप_प्रकार)मोजणी_2, value);
}

शून्य MatShape::insert(पूर्णांक* where, स्थिरांक पूर्णांक* begin, स्थिरांक पूर्णांक* end)
{
    घाला(where, begin, end);
}

शून्य MatShape::घाला(पूर्णांक* where, स्थिरांक पूर्णांक* begin, स्थिरांक पूर्णांक* end)
{
    पूर्णांक old_dims = std::max(dims, 0);
    सूचक_भेद_प्रकार delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णांक)(old_dims+delta);
    साठी (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+delta] = p[i];
    साठी (पूर्णांक i = 0; i < (पूर्णांक)delta; i++)
        p[i+ofs] = begin[i];
}

शून्य MatShape::erase(पूर्णांक* where)
{
    CV_Assert(dims > 0);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    जर (ofs == dims)
        परत;
    dims--;
    साठी (पूर्णांक i = (पूर्णांक)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

माप_प्रकार MatShape::total() स्थिरांक
{
    माप_प्रकार result = 1;
    जर (dims < 0)
        परत 0;
    साठी (पूर्णांक i = 0; i < dims; i++)
        result *= p[i];
    परत result;
}

std::vector<पूर्णांक> MatShape::vec() स्थिरांक
{
    जर (dims < 0)
        परत std::vector<पूर्णांक>(1, 0);
    परत std::vector<पूर्णांक>(p, p + dims);
}

std::string MatShape::str() स्थिरांक
{
    std::stringstream sstrm;
    जर (empty()) {
        sstrm << "<empty>";
    } अन्यथा जर (dims == 0) {
        sstrm << "<scalar>";
    } अन्यथा {
        sstrm << "[";
        साठी (पूर्णांक i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    परत sstrm.str();
}

स्थिर शून्य finalizeBlockLayout(MatShape& size, पूर्णांक C=0)
{
    जर (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        पूर्णांक C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } अन्यथा {
        size.C = 0;
    }
    साठी (पूर्णांक i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    जर (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(माप_प्रकार dims_, स्थिरांक पूर्णांक* size_, DataLayout layout_, पूर्णांक C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)dims_;
    साठी (पूर्णांक i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*हे, C_);
}

MatShape::MatShape(माप_प्रकार dims_, पूर्णांक value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)dims_;
    साठी (पूर्णांक i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*हे, 0);
}

MatShape::MatShape(std::initializer_list<पूर्णांक> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)shape.size();
    स्वतः it = shape.begin();
    साठी (पूर्णांक i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*हे, 0);
}

MatShape::MatShape(पूर्णांक dims_, पूर्णांक value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    साठी (पूर्णांक i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*हे, 0);
}

MatShape::MatShape(स्थिरांक std::vector<पूर्णांक>& shape_, DataLayout layout_, पूर्णांक C_)
{
    layout = layout_;
    माप_प्रकार shape_size = shape_.size();
    CV_Assert(shape_size < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)shape_size;
    साठी (पूर्णांक i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*हे, C_);
}

MatShape::MatShape(स्थिरांक पूर्णांक* begin, स्थिरांक पूर्णांक* end, DataLayout layout_, पूर्णांक C_)
{
    layout = layout_;
    सूचक_भेद_प्रकार shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (पूर्णांक)shape_size;
    साठी (पूर्णांक i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*हे, C_);
}

MatShape::MatShape(स्थिरांक MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    साठी (पूर्णांक i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::चालक = (स्थिरांक MatShape& shape)
{
    जर (हे != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        साठी (पूर्णांक i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    परत *हे;
}

तार्किक MatShape::hasSymbols() स्थिरांक
{
    साठी (पूर्णांक i = 0; i < dims; i++) {
        जर (p[i] < 0)
            परत सत्य;
    }
    परत असत्य;
}

पूर्णांक MatShape::channels() स्थिरांक
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    परत layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, पूर्णांक C0) स्थिरांक
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *हे;
    newsize.layout = newLayout;

    जर (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        पूर्णांक Corig = channels();
        newsize.C = Corig;

        जर (layout == DATA_LAYOUT_NHWC) {
            साठी (पूर्णांक i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } अन्यथा जर (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        जर (newLayout == DATA_LAYOUT_NHWC) {
            साठी (पूर्णांक i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } अन्यथा {
        CV_Assert_N(C0 <= 1);

        जर (newLayout == layout)
            परत newsize;

        // NHWC => NCHW
        जर (newLayout == DATA_LAYOUT_NCHW) {
            साठी (पूर्णांक i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } अन्यथा {
            // NCHW => NHWC
            साठी (पूर्णांक i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    परत newsize;
}

स्थिर पूर्णांक normalizeShapeAxis(पूर्णांक axis, पूर्णांक dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    परत axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
स्थिर MatShape toSemantic(स्थिरांक MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    परत shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
स्थिर MatShape toBlockOf(MatShape result, स्थिरांक MatShape& src, स्थिरांक अक्षर* op)
{
    जर (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (पूर्णांक)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    परत result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(स्थिरांक std::vector<पूर्णांक>& axes) स्थिरांक
{
    CV_Assert(dims >= 0);
    जर (layout == DATA_LAYOUT_BLOCK)
        परत toBlockOf(toSemantic(*हे).squeeze(axes), *हे, "squeeze");
    तार्किक हटाओ[MAX_DIMS] = {असत्य};
    जर (axes.empty())
    {
        साठी (पूर्णांक i = 0; i < dims; i++)
            हटाओ[i] = p[i] == 1;
    }
    साठी (पूर्णांक a : axes)
    {
        पूर्णांक i = normalizeShapeAxis(a, dims);
        CV_Check(a, !हटाओ[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        हटाओ[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    साठी (पूर्णांक i = 0; i < dims; i++)
        जर (!हटाओ[i])
            result.p[result.dims++] = p[i];
    परत result;
}

MatShape MatShape::unsqueeze(स्थिरांक std::vector<पूर्णांक>& axes) स्थिरांक
{
    CV_Assert(dims >= 0);
    जर (layout == DATA_LAYOUT_BLOCK)
        परत toBlockOf(toSemantic(*हे).unsqueeze(axes), *हे, "unsqueeze");
    स्थिरांक पूर्णांक outDims = dims + (पूर्णांक)axes.size();
    CV_CheckLE(outDims, (पूर्णांक)MAX_DIMS, "unsqueeze: too many dimensions");
    तार्किक insert[MAX_DIMS] = {असत्य};
    साठी (पूर्णांक a : axes)
    {
        पूर्णांक i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    साठी (पूर्णांक i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    परत result;
}

MatShape MatShape::flatten(पूर्णांक startAxis, पूर्णांक endAxis) स्थिरांक
{
    CV_Assert(dims >= 0);
    जर (layout == DATA_LAYOUT_BLOCK)
        परत toBlockOf(toSemantic(*हे).flatten(startAxis, endAxis), *हे, "flatten");
    जर (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        परत MatShape({1});
    }
    पूर्णांक start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    साठी (पूर्णांक i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((द्विगुण)merged, (द्विगुण)पूर्णांक_उच्च, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    साठी (पूर्णांक i = 0; i < dims; i++)
    {
        जर (i == start)
            result.p[result.dims++] = (पूर्णांक)merged;
        अन्यथा जर (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    परत result;
}

MatShape MatShape::reshape(स्थिरांक MatShape& newShape, तार्किक allowZero) स्थिरांक
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    जर (layout == DATA_LAYOUT_BLOCK)
        परत toBlockOf(toSemantic(*हे).reshape(newShape, allowZero), *हे, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    पूर्णांक inferred = -1;
    int64 known = 1;
    साठी (पूर्णांक i = 0; i < result.dims; i++)
    {
        पूर्णांक& sz = result.p[i];
        जर (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            सुरूठेवा;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        जर (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    स्थिरांक int64 total_ = (int64)total();
    जर (inferred >= 0)
    {
        CV_Check((द्विगुण)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((द्विगुण)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((द्विगुण)(total_/known), (द्विगुण)पूर्णांक_उच्च, "reshape: the result is too big");
        result.p[inferred] = (पूर्णांक)(total_/known);
    }
    अन्यथा जर (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (दीर्घ दीर्घ)known, (दीर्घ दीर्घ)total_));
    परत result;
}

MatShape MatShape::expand(स्थिरांक MatShape& another) स्थिरांक
{
    जर (dims == 0)
        परत another;
    जर (another.dims == 0)
        परत *हे;

    जर ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    जर (dims < 0 || another.dims < 0)
        परत result;

    result = *हे;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    साठी (पूर्णांक i = result.dims-1; i >= 0; i--) {
        पूर्णांक i1 = i - (result.dims - dims);
        पूर्णांक i2 = i - (result.dims - another.dims);
        पूर्णांक sz1 = i1 < 0 ? 1 : p[i1];
        पूर्णांक sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    परत result;
}

/////////////////////////// MatAllocator ////////////////////////////

शून्य MatAllocator::map(UMatData*, AccessFlag) स्थिरांक
{
}

शून्य MatAllocator::unmap(UMatData* u) स्थिरांक
{
    जर(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

शून्य MatAllocator::download(UMatData* u, शून्य* dstptr,
         पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
         स्थिरांक माप_प्रकार srcofs[], स्थिरांक माप_प्रकार srcstep[],
         स्थिरांक माप_प्रकार dststep[]) स्थिरांक
{
    जर(!u)
        परत;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    साठी( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        जर( sz[i] == 0 )
            परत;
        जर( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिरांक Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    साठी( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}


शून्य MatAllocator::upload(UMatData* u, स्थिरांक शून्य* srcptr, पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
                    स्थिरांक माप_प्रकार dstofs[], स्थिरांक माप_प्रकार dststep[],
                    स्थिरांक माप_प्रकार srcstep[]) स्थिरांक
{
    जर(!u)
        परत;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    साठी( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        जर( sz[i] == 0 )
            परत;
        जर( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, (शून्य*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिरांक Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    साठी( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

शून्य MatAllocator::copy(UMatData* usrc, UMatData* udst, पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
                  स्थिरांक माप_प्रकार srcofs[], स्थिरांक माप_प्रकार srcstep[],
                  स्थिरांक माप_प्रकार dstofs[], स्थिरांक माप_प्रकार dststep[], तार्किक /*sync*/) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर(!usrc || !udst)
        परत;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    साठी( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        जर( sz[i] == 0 )
            परत;
        जर( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        जर( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिरांक Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    साठी( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(स्थिरांक अक्षर* id) स्थिरांक
{
    CV_UNUSED(id);
    स्थिर DummyBufferPoolController dummy;
    परत &dummy;
}

वर्ग StdMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णांक dims, स्थिरांक पूर्णांक* sizes, पूर्णांक type,
                       शून्य* data0, माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        माप_प्रकार total = CV_ELEM_SIZE(type);
        साठी( पूर्णांक i = dims-1; i >= 0; i-- )
        {
            जर( step )
            {
                जर( data0 && step[i] != CV_AUTOSTEP )
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
        UMatData* u = नवीन UMatData(हे);
        u->data = u->origdata = data;
        u->size = total;
        जर(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        परत u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        जर(!u) परत असत्य;
        परत सत्य;
    }

    शून्य deallocate(UMatData* u) स्थिरांक CV_OVERRIDE
    {
        जर(!u)
            परत;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        जर( !(u->flags & UMatData::USER_ALLOCATED) )
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
    परत g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    परत getDefaultAllocatorMatRef();
}

शून्य Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, नवीन StdMatAllocator())
}

//==================================================================================================

शून्य setSize( Mat& m, पूर्णांक _dims, स्थिरांक पूर्णांक* _sz, स्थिरांक माप_प्रकार* _steps, तार्किक autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    साठी (पूर्णांक i = _dims-2; i >= 0; i--) {
        माप_प्रकार autostep = m.size[i+1]*m.step[i+1];
        जर (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा जर (autoSteps) {
            m.step[i] = autostep;
        } अन्यथा {
            m.step[i] = 0;
        }
    }

    जर( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.cols = m.rows = -1;
    }
}

पूर्णांक updateContinuityFlag(पूर्णांक flags, पूर्णांक dims, स्थिरांक पूर्णांक* size, स्थिरांक माप_प्रकार* step)
{
    पूर्णांक i, j;
    जर (dims <= 1)
        परत flags | Mat::CONTINUOUS_FLAG;
    साठी( i = 0; i < dims; i++ )
    {
        जर( size[i] > 1 )
            थांबा;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    साठी( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        जर( step[j]*size[j] < step[j-1] )
            थांबा;
    }

    जर( j <= i && t == (uint64)(पूर्णांक)t )
        परत flags | Mat::CONTINUOUS_FLAG;
    परत flags & ~Mat::CONTINUOUS_FLAG;
}

शून्य Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

शून्य finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    पूर्णांक d = m.dims;
    जर( d > 2 )
        m.rows = m.cols = -1;
    जर(m.u)
        m.datastart = m.data = m.u->data;
    जर( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        जर( m.size[0] > 0 )
        {
            पूर्णांक lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            साठी( पूर्णांक i = 0; i < d-1; i++ )
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

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *हे = _s;
}

Mat::Mat(Size _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *हे = _s;
}

Mat::Mat(पूर्णांक _dims, स्थिरांक पूर्णांक* _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(पूर्णांक _dims, स्थिरांक पूर्णांक* _sz, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *हे = _s;
}

Mat::Mat(स्थिरांक std::vector<पूर्णांक>& _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(स्थिरांक std::vector<पूर्णांक>& _sz, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *हे = _s;
}

Mat::Mat(स्थिरांक MatShape& _shape, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(स्थिरांक MatShape& _shape, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *हे = _s;
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type, स्थिरांक Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *हे = _s;
}

Mat::Mat(स्थिरांक Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    जर( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, शून्य* _data, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != रिक्त);

    माप_प्रकार esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols * esz;
    जर( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_Assert( _step >= minstep );
        जर (_step % esz1 != 0)
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

Mat::Mat(Size _sz, पूर्णांक _type, शून्य* _data, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != रिक्त);

    माप_प्रकार esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols*esz;
    जर( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_CheckGE(_step, minstep, "");

        जर (_step % esz1 != 0)
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

Mat& Mat::चालक = (स्थिरांक Mat& m)
{
    जर( हे != &m )
    {
        जर( m.u )
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
    परत *हे;
}

Mat Mat::clone() स्थिरांक
{
    Mat m;
    copyTo(m);
    परत m;
}

शून्य Mat::assignTo( Mat& m, पूर्णांक _type ) स्थिरांक
{
    जर( _type < 0 )
        m = *हे;
    अन्यथा
        convertTo(m, _type);
}

शून्य Mat::create(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    _type &= TYPE_MASK;
    जर( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        परत;
    पूर्णांक sz[] = {_rows, _cols};
    create(2, sz, _type);
}

शून्य Mat::create(Size _sz, पूर्णांक _type)
{
    create(_sz.height, _sz.width, _type);
}

शून्य Mat::createSameSize(आदान_Array m, पूर्णांक type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

शून्य Mat::fit(पूर्णांक _dims, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type)
{
    माप_प्रकार oldTotalBytes = u ? u->size : 0;
    माप_प्रकार esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    साठी (पूर्णांक i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    माप_प्रकार newTotalBytes = newTotal*esz;
    जर (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        पूर्णांक _dummy_size = 0;
        setSize(*हे, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, सत्य);
        finalizeHdr(*हे);
    }
}

शून्य Mat::fit(स्थिरांक std::vector<पूर्णांक>& _shape, पूर्णांक _type)
{
    fit((पूर्णांक)_shape.size(), _shape.data(), _type);
}

शून्य Mat::fit(स्थिरांक MatShape& _shape, पूर्णांक _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य Mat::fit(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक new_ndims = (पूर्णांक)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    साठी (पूर्णांक i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

शून्य Mat::fit(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    _type &= TYPE_MASK;
    पूर्णांक sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

शून्य Mat::fit(Size _sz, पूर्णांक _type)
{
    fit(_sz.height, _sz.width, _type);
}

शून्य Mat::fitSameSize(आदान_Array m, पूर्णांक _type)
{
    fit(m.shape(), _type);
}

शून्य Mat::addref()
{
    जर( u )
        CV_XADD(&u->refcount, 1);
}

शून्य Mat::release()
{
    जर( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = रिक्त;
    datastart = dataend = datalimit = data = 0;
    साठी(पूर्णांक i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

माप_प्रकार Mat::step1(पूर्णांक i) स्थिरांक
{
    परत step.p[i] / elemSize1();
}

तार्किक Mat::empty() स्थिरांक
{
    परत data == 0 || total() == 0;
}

माप_प्रकार Mat::total() स्थिरांक
{
    जर( dims <= 2 )
        परत (माप_प्रकार)rows * cols;
    माप_प्रकार p = 1;
    साठी( पूर्णांक i = 0; i < dims; i++ )
        p *= size[i];
    परत p;
}

माप_प्रकार Mat::total(पूर्णांक startDim, पूर्णांक endDim) स्थिरांक
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    माप_प्रकार p = 1;
    पूर्णांक endDim_ = endDim <= dims ? endDim : dims;
    साठी( पूर्णांक i = startDim; i < endDim_; i++ )
        p *= size[i];
    परत p;
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
    जर (हे == &m)
      परत *हे;

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
    परत *हे;
}


शून्य Mat::create(पूर्णांक d0, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type)
{
    पूर्णांक sz1 = 1, d = d0;
    पूर्णांक i;
    जर (d == 0) {
        d = 1;
        _sizes = (स्थिरांक पूर्णांक*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    जर( data && d == dims && _type == type() )
    {
        साठी( i = 0; i < d; i++ )
            जर( size[i] != _sizes[i] )
                थांबा;
        जर( i == d )
            परत;
    }

    पूर्णांक _sizes_backup[CV_MAX_DIM]; // #5991
    जर (_sizes == (हे->size.p))
    {
        साठी(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    जर( d == 0 )
        परत;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*हे, d, _sizes, 0, सत्य);

    जर( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        जर(!a)
            a = a0;
        प्रयत्न
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        पकडा (...)
        {
            जर (a == a0)
                फेका;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (माप_प्रकार)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*हे);
    size.dims = dims = d0;
}

शून्य Mat::create(स्थिरांक std::vector<पूर्णांक>& _sizes, पूर्णांक _type)
{
    create((पूर्णांक)_sizes.size(), _sizes.data(), _type);
}

शून्य Mat::create(स्थिरांक MatShape& _shape, पूर्णांक _type)
{
    जर (_shape.dims < 0) {
        release();
        परत;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य Mat::create(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक new_ndims = (पूर्णांक)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    साठी (पूर्णांक i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

शून्य Mat::copySize(स्थिरांक Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

शून्य Mat::deallocate()
{
    जर(u)
    {
        UMatData* u_ = u;
        u = रिक्त;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(स्थिरांक Mat& m, स्थिरांक Range& _rowRange, स्थिरांक Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    जर( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        साठी( पूर्णांक i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *हे = m(rs);
        परत;
    }

    *हे = m;
    प्रयत्न
    {
        जर( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        जर( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    पकडा(...)
    {
        release();
        फेका;
    }

    updateContinuityFlag();

    जर( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(स्थिरांक Mat& m, स्थिरांक Rect& roi)
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
    जर( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    जर( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(पूर्णांक _dims, स्थिरांक पूर्णांक* _sizes, पूर्णांक _type, शून्य* _data, स्थिरांक माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    जर (_dims > 0 || _data != nullptr)
        setSize(*हे, _dims, _sizes, _steps, सत्य);
    finalizeHdr(*हे);
}


Mat::Mat(स्थिरांक std::vector<पूर्णांक>& _sizes, पूर्णांक _type, शून्य* _data, स्थिरांक माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    पूर्णांक _dims = (पूर्णांक)_sizes.size();
    जर (_dims > 0 || _data != nullptr)
        setSize(*हे, _dims, _sizes.data(), _steps, सत्य);
    finalizeHdr(*हे);
}

Mat::Mat(स्थिरांक MatShape& _shape, पूर्णांक _type, शून्य* _data, स्थिरांक माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    जर (_shape.dims >= 0) {
        setSize(*हे, _shape.dims, _shape.p, _steps, सत्य);
    }
    अन्यथा {
        CV_Assert(!data);
    }
    finalizeHdr(*हे);
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type, शून्य* _data, स्थिरांक माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक _dims = (पूर्णांक)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    साठी (पूर्णांक i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    जर (_dims > 0 || _data != nullptr)
        setSize(*हे, _dims, new_shape, _steps, सत्य);
    finalizeHdr(*हे);
}

Mat::Mat(स्थिरांक Mat& m, स्थिरांक Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णांक d = m.dims;

    CV_Assert(ranges);
    साठी( पूर्णांक i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *हे = m;
    साठी( पूर्णांक i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        जर( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    जर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(स्थिरांक Mat& m, स्थिरांक std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    पूर्णांक d = m.dims;

    CV_Assert((पूर्णांक)ranges.size() == d);
    साठी (पूर्णांक i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *हे = m;
    साठी (पूर्णांक i = 0; i < d; i++)
    {
        Range r = ranges[i];
        जर (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    जर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(पूर्णांक d) स्थिरांक
{
    CV_Assert( dims == 2 );
    Mat m = *हे;
    माप_प्रकार esz = elemSize();
    पूर्णांक len;

    जर( d >= 0 )
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

    जर( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    परत m;
}


शून्य Mat::pop_back(माप_प्रकार nelems)
{
    CV_Assert( nelems <= (माप_प्रकार)size.p[0] );
    CV_Assert( dims >= 1 );

    जर( isSubmatrix() )
        *हे = rowRange(0, size.p[0] - (पूर्णांक)nelems);
    अन्यथा
    {
        size.p[0] -= (पूर्णांक)nelems;
        जर (dims == 2)
            rows = size.p[0];
        अन्यथा जर (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


शून्य Mat::ढकला_back(स्थिरांक शून्य* elem)
{
    माप_प्रकार r = size.p[0];
    जर( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    माप_प्रकार esz = elemSize();
    स_नकल(data + r*step.p[0], elem, esz);
    size.p[0] = पूर्णांक(r + 1);
    जर (dims == 2)
        rows = size.p[0];
    अन्यथा जर (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    साठी( पूर्णांक i = 1; i < dims; i++ )
        tsz *= size.p[i];
    जर( esz < step.p[0] || tsz != (uint64)(पूर्णांक)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


शून्य Mat::reserve(माप_प्रकार nelems)
{
    स्थिरांक माप_प्रकार MIN_SIZE = 64;

    CV_Assert( (पूर्णांक)nelems >= 0 );
    जर( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        परत;

    पूर्णांक r = size.p[0];
    जर( (माप_प्रकार)r >= nelems)
        परत;

    MatShape newsize = size;
    माप_प्रकार esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    जर (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (पूर्णांक)nelems;
    माप_प्रकार newbytes = newsize.total() * esz;
    जर (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = पूर्णांक(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    जर( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *हे = m;
    size.p[0] = r;

    जर (dims == 2)
        rows = size.p[0];
    अन्यथा जर (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


शून्य Mat::reserveBuffer(माप_प्रकार nbytes)
{
    माप_प्रकार esz = 1;
    पूर्णांक mtype = CV_8UC1;
    जर (!empty())
    {
        जर (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            परत;
        esz = elemSize();
        mtype = type();
    }

    माप_प्रकार nelems = (nbytes - 1) / esz + 1;

#जर SIZE_MAX > अच_पूर्णांक_उच्च
    CV_Assert(nelems <= माप_प्रकार(पूर्णांक_उच्च)*माप_प्रकार(पूर्णांक_उच्च));
    पूर्णांक newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x400*माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x100000 * माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x40000000 * माप_प्रकार(पूर्णांक_उच्च) ?
                  माप_प्रकार(पूर्णांक_उच्च) : 0x40000000 : 0x100000 : 0x400 : 1;
#अन्यथा
    पूर्णांक newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? 2 : 1;
#endif
    पूर्णांक newcols = (पूर्णांक)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


शून्य Mat::resize(माप_प्रकार nelems)
{
    पूर्णांक r = size.p[0];
    जर( r == (पूर्णांक)nelems )
        परत;
    CV_Assert( (पूर्णांक)nelems >= 0 );

    जर( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (पूर्णांक)nelems;
    जर (dims == 2)
        rows = size.p[0];
    अन्यथा जर (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


शून्य Mat::resize(माप_प्रकार nelems, स्थिरांक Scalar& s)
{
    पूर्णांक saveRows = size.p[0];
    resize(nelems);

    जर( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

शून्य Mat::push_back(स्थिरांक Mat& elems)
{
    माप_प्रकार r = size.p[0];
    माप_प्रकार delta = elems.size.p[0];
    जर( delta == 0 )
        परत;
    जर( हे == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        परत;
    }
    जर( !data )
    {
        *हे = elems.clone();
        परत;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    तार्किक eq = size == elems.size;
    size.p[0] = पूर्णांक(r);
    जर( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    जर( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    माप_प्रकार esz = elemSize();
    माप_प्रकार minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    माप_प्रकार step0 = step.p[0];
    जर (step0 < minstep) {
        जर (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    जर( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += पूर्णांक(delta);
    जर (dims == 2)
        rows = size.p[0];
    अन्यथा जर (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    जर( isContinuous() && elems.isContinuous() )
        स_नकल(data + r*step0, elems.data, elems.total()*elems.elemSize());
    अन्यथा
    {
        Mat part = rowRange(पूर्णांक(r), पूर्णांक(r + delta));
        elems.copyTo(part);
    }
}


शून्य Mat::locateROI( Size& wholeSize, Point& ofs ) स्थिरांक
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    माप_प्रकार esz = elemSize(), minstep;
    सूचक_भेद_प्रकार delta1 = data - datastart, delta2 = dataend - datastart;

    जर( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णांक)(delta1/step[0]);
        ofs.x = (पूर्णांक)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (पूर्णांक)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (पूर्णांक)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( पूर्णांक dtop, पूर्णांक dbottom, पूर्णांक dleft, पूर्णांक dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    माप_प्रकार esz = elemSize();
    locateROI( wholeSize, ofs );
    पूर्णांक row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    पूर्णांक col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    जर(row1 > row2)
        std::swap(row1, row2);
    जर(col1 > col2)
        std::swap(col1, col2);

    जर (dims == 1) {
        data += (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } अन्यथा {
        data += (row1 - ofs.y)*(std::सूचक_भेद_प्रकार)step + (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    परत *हे;
}

Mat Mat::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिरांक
{
    पूर्णांक cn = channels();
    Mat hdr = *हे;

    जर( new_cn == 0 )
        new_cn = cn;

    जर( dims > 2 )
    {
        जर( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            परत hdr;
        }
        CV_Assert( new_rows > 0 );
        पूर्णांक sz[] = { new_rows, (पूर्णांक)(total()*cn/new_rows) };
        परत reshape(new_cn, 2, sz);
    }

    पूर्णांक total_width = cols * cn;

    जर( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    जर( new_rows != 0 && new_rows != rows )
    {
        पूर्णांक total_size = total_width * rows;
        जर( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        जर( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.rows = rows;
        जर (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    पूर्णांक new_width = total_width / new_cn;

    जर( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_अंक_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    परत hdr;
}

Mat Mat::reshape(पूर्णांक _cn, पूर्णांक _newndims, स्थिरांक पूर्णांक* _newsz) स्थिरांक
{
    जर(_newndims == dims)
    {
        जर(_newsz == 0)
            परत reshape(_cn);
        जर(_newndims == 2)
            परत reshape(_cn, _newsz[0]);
    }

    जर (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        जर (_cn == 0)
            _cn = हे->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        माप_प्रकार total_elem1_ref = हे->total() * हे->channels();
        माप_प्रकार total_elem1 = _cn;

        AutoBuffer<पूर्णांक, 4> newsz_buf( (माप_प्रकार)_newndims );

        पूर्णांक m1_idx = -1;

        साठी (पूर्णांक i = 0; i < _newndims; i++)
        {
            जर (_newsz[i] >= 0) {
                जर (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (माप_प्रकार)newsz_buf[i];
            } अन्यथा {
                जर (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        जर (m1_idx >= 0) {
            जर (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णांक)(total_elem1_ref / total_elem1);
        } अन्यथा जर (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *हे;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), रिक्त, सत्य);

        परत hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(पूर्णांक _cn, स्थिरांक std::vector<पूर्णांक>& _newshape) स्थिरांक
{
    पूर्णांक newdims = (पूर्णांक)_newshape.size();
    जर(newdims == 0 && empty())
    {
        परत *हे;
    }

    परत reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(पूर्णांक _cn, स्थिरांक MatShape& _newshape) स्थिरांक
{
    जर (_newshape.dims < 0) {
        पूर्णांक newshape[] = {0};
        परत reshape(_cn, 1, newshape);
    }
    परत reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(पूर्णांक _cn, std::initializer_list<पूर्णांक> newshape_) स्थिरांक
{
    पूर्णांक newshape[MatShape::MAX_DIMS];
    माप_प्रकार i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (माप_प्रकार)MatShape::MAX_DIMS);
    स्वतः it = newshape_.begin();
    साठी (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    परत reshape(_cn, (पूर्णांक)newshape_dims, newshape);
}

Mat Mat::reinterpret(पूर्णांक type) स्थिरांक
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(हे->type()) == CV_ELEM_SIZE(type));
    Mat m = *हे;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    परत m;
}

Mat Mat::diag(स्थिरांक Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    पूर्णांक len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    जर( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    परत m;
}

पूर्णांक Mat::checkVector(पूर्णांक _elemChannels, पूर्णांक _depth, तार्किक _requireContinuous) स्थिरांक
{
    परत data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णांक)(total()*channels()/_elemChannels) : -1;
}


स्थिर ओळीत Size getContinuousSize_(पूर्णांक flags, पूर्णांक cols, पूर्णांक rows, पूर्णांक widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    तार्किक has_int_overflow = sz >= पूर्णांक_उच्च;
    तार्किक isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    परत (isContiguous && !has_int_overflow)
            ? Size((पूर्णांक)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    परत getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    स्थिरांक Size sz1 = m1.size();
    जर (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        तार्किक is_m1_vector = m1.cols == 1 || m1.rows == 1;
        तार्किक is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        पूर्णांक total = (पूर्णांक)total_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_overflow = ((int64)total_sz * widthScale) >= पूर्णांक_उच्च;
        जर (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        परत Size(m1.cols * widthScale, m1.rows);
    }
    परत getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    स्थिरांक Size sz1 = m1.size();
    जर (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        तार्किक is_m1_vector = m1.cols == 1 || m1.rows == 1;
        तार्किक is_m2_vector = m2.cols == 1 || m2.rows == 1;
        तार्किक is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        पूर्णांक total = (पूर्णांक)total_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_overflow = ((int64)total_sz * widthScale) >= पूर्णांक_उच्च;
        जर (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        परत Size(m1.cols * widthScale, m1.rows);
    }
    परत getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
