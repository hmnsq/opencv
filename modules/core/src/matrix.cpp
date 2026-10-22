// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "bufferpool.impl.hpp"

namespace cv {

std::string layoutToString(तथ्य_Layout layout)
{
    वापस
        layout == तथ्य_LAYOUT_ND ? "ND" :
        layout == तथ्य_LAYOUT_NCHW ? "NCHW" :
        layout == तथ्य_LAYOUT_NHWC ? "NHWC" :
        layout == तथ्य_LAYOUT_BLOCK ? "BLOCK" :
        layout == तथ्य_LAYOUT_NCDHW ? "NCDHW" :
        layout == तथ्य_LAYOUT_NDHWC ? "NDHWC" :
        layout == तथ्य_LAYOUT_PLANAR ? "PLANAR" :
        layout == तथ्य_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

bool चालक == (स्थिर MatShape& size1, स्थिर MatShape& size2)
{
    अगर (size1.dims != size2.dims)
        वापस false;
    अगर (size1.layout != size2.layout &&
        size1.layout != तथ्य_LAYOUT_UNKNOWN &&
        size2.layout != तथ्य_LAYOUT_UNKNOWN)
        वापस false;
    अगर (size1.layout == तथ्य_LAYOUT_BLOCK &&
        size2.layout == तथ्य_LAYOUT_BLOCK &&
        size1.C != size2.C)
        वापस false;
    क्रम (पूर्णांक i = 0; i < size1.dims; i++) {
        अगर (size1.p[i] != size2.p[i])
            वापस false;
    }
    वापस true;
}

bool चालक != (स्थिर MatShape& size1, स्थिर MatShape& size2)
{
    वापस !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    वापस MatShape(0);
}

व्योम MatShape::सफा()
{
    dims = -1;
    layout = तथ्य_LAYOUT_UNKNOWN;
    C = 0;
    क्रम (पूर्णांक i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

व्योम MatShape::resize(माप_प्रकार new_आकार, पूर्णांक value)
{
    CV_Assert(new_आकार < (माप_प्रकार)MAX_DIMS);
    पूर्णांक old_dims = std::max(dims, 0);
    dims = (पूर्णांक)new_आकार;
    क्रम (पूर्णांक i = old_dims; i < dims; i++)
        p[i] = value;
}

व्योम MatShape::reserve(माप_प्रकार)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

व्योम MatShape::assign(माप_प्रकार new_आकार, पूर्णांक value)
{
    CV_Assert(new_आकार < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)new_आकार;
    क्रम (पूर्णांक i = 0; i < dims; i++)
        p[i] = value;
}

व्योम MatShape::assign(पूर्णांक new_आकार, पूर्णांक value)
{
    assign((माप_प्रकार)new_आकार, value);
}

व्योम MatShape::assign(स्थिर पूर्णांक* begin, स्थिर पूर्णांक* end)
{
    assign_(begin, end);
}

व्योम MatShape::assign_(स्थिर पूर्णांक* begin, स्थिर पूर्णांक* end)
{
    सूचक_भेद_प्रकार new_आकार = end - begin;
    CV_Assert(0 <= new_आकार && new_आकार < (सूचक_भेद_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)new_आकार;
    क्रम (पूर्णांक i = 0; i < dims; i++)
        p[i] = begin[i];
}

पूर्णांक* MatShape::begin() { वापस p; }
स्थिर पूर्णांक* MatShape::begin() स्थिर { वापस p; }
पूर्णांक* MatShape::end() { वापस p + std::max(dims, 0); }
स्थिर पूर्णांक* MatShape::end() स्थिर { वापस p + std::max(dims, 0); }
पूर्णांक& MatShape::back() { वापस p[std::max(dims-1, 0)]; }
स्थिर पूर्णांक& MatShape::back() स्थिर { वापस p[std::max(dims-1, 0)]; }

व्योम MatShape::push_back(पूर्णांक value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

व्योम MatShape::emplace_back(पूर्णांक value)
{
    push_back(value);
}

व्योम MatShape::insert(पूर्णांक* where, पूर्णांक value)
{
    पूर्णांक old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    क्रम (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

व्योम MatShape::insert(पूर्णांक* where, माप_प्रकार गिनती_2, पूर्णांक value)
{
    पूर्णांक old_dims = std::max(dims, 0);
    CV_Assert((माप_प्रकार)(old_dims+गिनती_2) < (माप_प्रकार)MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णांक)(old_dims+गिनती_2);
    क्रम (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+गिनती_2] = p[i];
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)गिनती_2; i++)
        p[i+ofs] = value;
}

व्योम MatShape::insert(पूर्णांक* where, पूर्णांक गिनती_2, पूर्णांक value)
{
    insert(where, (माप_प्रकार)गिनती_2, value);
}

व्योम MatShape::insert(पूर्णांक* where, स्थिर पूर्णांक* begin, स्थिर पूर्णांक* end)
{
    जोड़ो(where, begin, end);
}

व्योम MatShape::जोड़ो(पूर्णांक* where, स्थिर पूर्णांक* begin, स्थिर पूर्णांक* end)
{
    पूर्णांक old_dims = std::max(dims, 0);
    सूचक_भेद_प्रकार delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णांक)(old_dims+delta);
    क्रम (पूर्णांक i = old_dims-1; i >= (पूर्णांक)ofs; i--)
        p[i+delta] = p[i];
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)delta; i++)
        p[i+ofs] = begin[i];
}

व्योम MatShape::erase(पूर्णांक* where)
{
    CV_Assert(dims > 0);
    सूचक_भेद_प्रकार ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    अगर (ofs == dims)
        वापस;
    dims--;
    क्रम (पूर्णांक i = (पूर्णांक)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

माप_प्रकार MatShape::योग() स्थिर
{
    माप_प्रकार परिणाम = 1;
    अगर (dims < 0)
        वापस 0;
    क्रम (पूर्णांक i = 0; i < dims; i++)
        परिणाम *= p[i];
    वापस परिणाम;
}

std::vector<पूर्णांक> MatShape::vec() स्थिर
{
    अगर (dims < 0)
        वापस std::vector<पूर्णांक>(1, 0);
    वापस std::vector<पूर्णांक>(p, p + dims);
}

std::string MatShape::str() स्थिर
{
    std::stringstream sstrm;
    अगर (empty()) {
        sstrm << "<empty>";
    } अन्यथा अगर (dims == 0) {
        sstrm << "<scalar>";
    } अन्यथा {
        sstrm << "[";
        क्रम (पूर्णांक i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    वापस sstrm.str();
}

अटल व्योम finalizeBlockLayout(MatShape& size, पूर्णांक C=0)
{
    अगर (size.layout == तथ्य_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        पूर्णांक C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } अन्यथा {
        size.C = 0;
    }
    क्रम (पूर्णांक i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    अगर (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    सफा();
}

MatShape::MatShape(माप_प्रकार dims_, स्थिर पूर्णांक* आकार_3, तथ्य_Layout layout_, पूर्णांक C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)dims_;
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        p[i] = आकार_3 ? आकार_3[i] : 0;
    }
    finalizeBlockLayout(*यह, C_);
}

MatShape::MatShape(माप_प्रकार dims_, पूर्णांक value, तथ्य_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)dims_;
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*यह, 0);
}

MatShape::MatShape(std::initializer_list<पूर्णांक> shape)
{
    layout = तथ्य_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)shape.size();
    स्वतः it = shape.begin();
    क्रम (पूर्णांक i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*यह, 0);
}

MatShape::MatShape(पूर्णांक dims_, पूर्णांक value, तथ्य_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*यह, 0);
}

MatShape::MatShape(स्थिर std::vector<पूर्णांक>& shape_, तथ्य_Layout layout_, पूर्णांक C_)
{
    layout = layout_;
    माप_प्रकार shape_आकार = shape_.size();
    CV_Assert(shape_आकार < (माप_प्रकार)MAX_DIMS);
    dims = (पूर्णांक)shape_आकार;
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*यह, C_);
}

MatShape::MatShape(स्थिर पूर्णांक* begin, स्थिर पूर्णांक* end, तथ्य_Layout layout_, पूर्णांक C_)
{
    layout = layout_;
    सूचक_भेद_प्रकार shape_आकार = end - begin;
    CV_Assert(0 <= shape_आकार && shape_आकार < MAX_DIMS);
    dims = (पूर्णांक)shape_आकार;
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*यह, C_);
}

MatShape::MatShape(स्थिर MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    क्रम (पूर्णांक i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::चालक = (स्थिर MatShape& shape)
{
    अगर (यह != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        क्रम (पूर्णांक i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    वापस *यह;
}

bool MatShape::hasSymbols() स्थिर
{
    क्रम (पूर्णांक i = 0; i < dims; i++) {
        अगर (p[i] < 0)
            वापस true;
    }
    वापस false;
}

पूर्णांक MatShape::channels() स्थिर
{
    CV_Assert(layout == तथ्य_LAYOUT_BLOCK || layout == तथ्य_LAYOUT_NCHW || layout == तथ्य_LAYOUT_NHWC);
    वापस layout == तथ्य_LAYOUT_BLOCK ? C : p[layout == तथ्य_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(तथ्य_Layout newLayout, पूर्णांक C0) स्थिर
{
    CV_Assert(layout == तथ्य_LAYOUT_BLOCK || layout == तथ्य_LAYOUT_NCHW || layout == तथ्य_LAYOUT_NHWC);
    CV_Assert(newLayout == तथ्य_LAYOUT_BLOCK || newLayout == तथ्य_LAYOUT_NCHW || newLayout == तथ्य_LAYOUT_NHWC);

    MatShape newsize = *यह;
    newsize.layout = newLayout;

    अगर (newLayout == तथ्य_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        पूर्णांक Corig = channels();
        newsize.C = Corig;

        अगर (layout == तथ्य_LAYOUT_NHWC) {
            क्रम (पूर्णांक i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != तथ्य_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } अन्यथा अगर (layout == तथ्य_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        अगर (newLayout == तथ्य_LAYOUT_NHWC) {
            क्रम (पूर्णांक i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == तथ्य_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } अन्यथा {
        CV_Assert_N(C0 <= 1);

        अगर (newLayout == layout)
            वापस newsize;

        // NHWC => NCHW
        अगर (newLayout == तथ्य_LAYOUT_NCHW) {
            क्रम (पूर्णांक i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } अन्यथा {
            // NCHW => NHWC
            क्रम (पूर्णांक i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    वापस newsize;
}

अटल पूर्णांक normalizeShapeAxis(पूर्णांक axis, पूर्णांक dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    वापस axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
अटल MatShape toSemantic(स्थिर MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    वापस shape.toLayout(तथ्य_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
अटल MatShape toBlockOf(MatShape परिणाम, स्थिर MatShape& src, स्थिर अक्षर* op)
{
    अगर (परिणाम.dims < 2 || परिणाम.p[0] != src.p[0] || परिणाम.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(परिणाम.dims + 1, (पूर्णांक)MatShape::MAX_DIMS, "too many dimensions");
    परिणाम.layout = तथ्य_LAYOUT_NCHW;
    वापस परिणाम.toLayout(तथ्य_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(स्थिर std::vector<पूर्णांक>& axes) स्थिर
{
    CV_Assert(dims >= 0);
    अगर (layout == तथ्य_LAYOUT_BLOCK)
        वापस toBlockOf(toSemantic(*यह).squeeze(axes), *यह, "squeeze");
    bool हटाओ[MAX_DIMS] = {false};
    अगर (axes.empty())
    {
        क्रम (पूर्णांक i = 0; i < dims; i++)
            हटाओ[i] = p[i] == 1;
    }
    क्रम (पूर्णांक a : axes)
    {
        पूर्णांक i = normalizeShapeAxis(a, dims);
        CV_Check(a, !हटाओ[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        हटाओ[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = 0;
    क्रम (पूर्णांक i = 0; i < dims; i++)
        अगर (!हटाओ[i])
            परिणाम.p[परिणाम.dims++] = p[i];
    वापस परिणाम;
}

MatShape MatShape::unsqueeze(स्थिर std::vector<पूर्णांक>& axes) स्थिर
{
    CV_Assert(dims >= 0);
    अगर (layout == तथ्य_LAYOUT_BLOCK)
        वापस toBlockOf(toSemantic(*यह).unsqueeze(axes), *यह, "unsqueeze");
    स्थिर पूर्णांक outDims = dims + (पूर्णांक)axes.size();
    CV_CheckLE(outDims, (पूर्णांक)MAX_DIMS, "unsqueeze: too many dimensions");
    bool insert[MAX_DIMS] = {false};
    क्रम (पूर्णांक a : axes)
    {
        पूर्णांक i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = outDims;
    क्रम (पूर्णांक i = 0, j = 0; i < outDims; i++)
        परिणाम.p[i] = insert[i] ? 1 : p[j++];
    वापस परिणाम;
}

MatShape MatShape::flatten(पूर्णांक startAxis, पूर्णांक endAxis) स्थिर
{
    CV_Assert(dims >= 0);
    अगर (layout == तथ्य_LAYOUT_BLOCK)
        वापस toBlockOf(toSemantic(*यह).flatten(startAxis, endAxis), *यह, "flatten");
    अगर (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        वापस MatShape({1});
    }
    पूर्णांक start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    क्रम (पूर्णांक i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((द्विगुन)merged, (द्विगुन)पूर्णांक_उच्च, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = 0;
    क्रम (पूर्णांक i = 0; i < dims; i++)
    {
        अगर (i == start)
            परिणाम.p[परिणाम.dims++] = (पूर्णांक)merged;
        अन्यथा अगर (i < start || i > end)
            परिणाम.p[परिणाम.dims++] = p[i];
    }
    वापस परिणाम;
}

MatShape MatShape::reshape(स्थिर MatShape& newShape, bool allowZero) स्थिर
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    अगर (layout == तथ्य_LAYOUT_BLOCK)
        वापस toBlockOf(toSemantic(*यह).reshape(newShape, allowZero), *यह, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape परिणाम;
    परिणाम.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, परिणाम.p);
    पूर्णांक inferred = -1;
    int64 known = 1;
    क्रम (पूर्णांक i = 0; i < परिणाम.dims; i++)
    {
        पूर्णांक& sz = परिणाम.p[i];
        अगर (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            जारी;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        अगर (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    स्थिर int64 योग_3 = (int64)योग();
    अगर (inferred >= 0)
    {
        CV_Check((द्विगुन)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((द्विगुन)योग_3, योग_3 % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((द्विगुन)(योग_3/known), (द्विगुन)पूर्णांक_उच्च, "reshape: the result is too big");
        परिणाम.p[inferred] = (पूर्णांक)(योग_3/known);
    }
    अन्यथा अगर (known != योग_3)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (दीर्घ दीर्घ)known, (दीर्घ दीर्घ)योग_3));
    वापस परिणाम;
}

MatShape MatShape::expand(स्थिर MatShape& another) स्थिर
{
    अगर (dims == 0)
        वापस another;
    अगर (another.dims == 0)
        वापस *यह;

    अगर ((layout == तथ्य_LAYOUT_NCHW || layout == तथ्य_LAYOUT_NHWC) &&
        (another.layout == तथ्य_LAYOUT_NCHW || another.layout == तथ्य_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != तथ्य_LAYOUT_BLOCK && another.layout != तथ्य_LAYOUT_BLOCK);

    MatShape परिणाम;

    अगर (dims < 0 || another.dims < 0)
        वापस परिणाम;

    परिणाम = *यह;
    परिणाम.dims = std::max(dims, another.dims);
    परिणाम.layout = layout == तथ्य_LAYOUT_UNKNOWN ? another.layout :
        layout == तथ्य_LAYOUT_ND && (another.layout == तथ्य_LAYOUT_NCHW ||
        another.layout == तथ्य_LAYOUT_NHWC) ? another.layout : layout;
    क्रम (पूर्णांक i = परिणाम.dims-1; i >= 0; i--) {
        पूर्णांक i1 = i - (परिणाम.dims - dims);
        पूर्णांक i2 = i - (परिणाम.dims - another.dims);
        पूर्णांक sz1 = i1 < 0 ? 1 : p[i1];
        पूर्णांक sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        परिणाम.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    वापस परिणाम;
}

/////////////////////////// MatAllocator ////////////////////////////

व्योम MatAllocator::map(UMat_तथ्य*, AccessFlag) स्थिर
{
}

व्योम MatAllocator::unmap(UMat_तथ्य* u) स्थिर
{
    अगर(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

व्योम MatAllocator::download(UMat_तथ्य* u, व्योम* dstptr,
         पूर्णांक dims, स्थिर माप_प्रकार sz[],
         स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
         स्थिर माप_प्रकार dststep[]) स्थिर
{
    अगर(!u)
        वापस;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* srcptr = u->तथ्य;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        अगर( sz[i] == 0 )
            वापस;
        अगर( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिर Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    क्रम( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}


व्योम MatAllocator::upload(UMat_तथ्य* u, स्थिर व्योम* srcptr, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                    स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[],
                    स्थिर माप_प्रकार srcstep[]) स्थिर
{
    अगर(!u)
        वापस;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* dstptr = u->तथ्य;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        अगर( sz[i] == 0 )
            वापस;
        अगर( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, (व्योम*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिर Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    क्रम( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

व्योम MatAllocator::copy(UMat_तथ्य* usrc, UMat_तथ्य* udst, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                  स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
                  स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[], bool /*sync*/) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर(!usrc || !udst)
        वापस;
    पूर्णांक isz[CV_MAX_DIM];
    uchar* srcptr = usrc->तथ्य;
    uchar* dstptr = udst->तथ्य;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
        अगर( sz[i] == 0 )
            वापस;
        अगर( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        अगर( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (पूर्णांक)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    स्थिर Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    माप_प्रकार planesz = it.size;

    क्रम( माप_प्रकार j = 0; j < it.nplanes; j++, ++it )
        स_नकल(ptrs[1], ptrs[0], planesz);
}

बफर_Pool_Controller* MatAllocator::get_बफर_Pool_Controller(स्थिर अक्षर* id) स्थिर
{
    CV_UNUSED(id);
    अटल Dummy_बफर_Pool_Controller dummy;
    वापस &dummy;
}

श्रेणी StdMatAllocator CV_FINAL : खुला MatAllocator
{
खुला:
    UMat_तथ्य* allocate(पूर्णांक dims, स्थिर पूर्णांक* sizes, पूर्णांक type,
                       व्योम* data0, माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        माप_प्रकार योग = CV_ELEM_आकार(type);
        क्रम( पूर्णांक i = dims-1; i >= 0; i-- )
        {
            अगर( step )
            {
                अगर( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(योग <= step[i]);
                    योग = step[i];
                }
                अन्यथा
                    step[i] = योग;
            }
            योग *= sizes[i];
        }
        uchar* तथ्य = data0 ? (uchar*)data0 : (uchar*)fastMalloc(योग);
        UMat_तथ्य* u = नया UMat_तथ्य(यह);
        u->तथ्य = u->origdata = तथ्य;
        u->size = योग;
        अगर(data0)
            u->flags |= UMat_तथ्य::USER_ALLOCATED;

        वापस u;
    }

    bool allocate(UMat_तथ्य* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        अगर(!u) वापस false;
        वापस true;
    }

    व्योम deallocate(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        अगर( !(u->flags & UMat_तथ्य::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        मिटाओ u;
    }
};

अटल
MatAllocator*& getDefaultAllocatorMatRef()
{
    अटल MatAllocator* g_matAllocator = Mat::getStdAllocator();
    वापस g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    वापस getDefaultAllocatorMatRef();
}

व्योम Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, नया StdMatAllocator())
}

//==================================================================================================

व्योम set_आकार( Mat& m, पूर्णांक _dims, स्थिर पूर्णांक* _sz, स्थिर माप_प्रकार* _steps, bool autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_आकार(m.flags);
    क्रम (पूर्णांक i = _dims-2; i >= 0; i--) {
        माप_प्रकार autostep = m.size[i+1]*m.step[i+1];
        अगर (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा अगर (autoSteps) {
            m.step[i] = autostep;
        } अन्यथा {
            m.step[i] = 0;
        }
    }

    अगर( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.cols = m.rows = -1;
    }
}

पूर्णांक updateContinuityFlag(पूर्णांक flags, पूर्णांक dims, स्थिर पूर्णांक* size, स्थिर माप_प्रकार* step)
{
    पूर्णांक i, j;
    अगर (dims <= 1)
        वापस flags | Mat::CONTINUOUS_FLAG;
    क्रम( i = 0; i < dims; i++ )
    {
        अगर( size[i] > 1 )
            अवरोध;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    क्रम( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        अगर( step[j]*size[j] < step[j-1] )
            अवरोध;
    }

    अगर( j <= i && t == (uint64)(पूर्णांक)t )
        वापस flags | Mat::CONTINUOUS_FLAG;
    वापस flags & ~Mat::CONTINUOUS_FLAG;
}

व्योम Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

व्योम finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    पूर्णांक d = m.dims;
    अगर( d > 2 )
        m.rows = m.cols = -1;
    अगर(m.u)
        m.datastart = m.तथ्य = m.u->तथ्य;
    अगर( m.तथ्य && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        अगर( m.size[0] > 0 )
        {
            पूर्णांक lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            क्रम( पूर्णांक i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        अन्यथा
            m.dataend = m.datalimit;
    }
    अन्यथा
        m.dataend = m.datalimit = m.तथ्य;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *यह = _s;
}

Mat::Mat(आकार _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(आकार _sz, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *यह = _s;
}

Mat::Mat(पूर्णांक _dims, स्थिर पूर्णांक* _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(पूर्णांक _dims, स्थिर पूर्णांक* _sz, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *यह = _s;
}

Mat::Mat(स्थिर std::vector<पूर्णांक>& _sz, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(स्थिर std::vector<पूर्णांक>& _sz, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *यह = _s;
}

Mat::Mat(स्थिर MatShape& _shape, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(स्थिर MatShape& _shape, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *यह = _s;
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type, स्थिर Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *यह = _s;
}

Mat::Mat(स्थिर Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), तथ्य(m.तथ्य),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    अगर( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, व्योम* तथ्य_3, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      तथ्य((uchar*)तथ्य_3), datastart((uchar*)तथ्य_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(योग() == 0 || तथ्य != शून्य);

    माप_प्रकार esz = CV_ELEM_आकार(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols * esz;
    अगर( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_Assert( _step >= minstep );
        अगर (_step % esz1 != 0)
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

Mat::Mat(आकार _sz, पूर्णांक _type, व्योम* तथ्य_3, माप_प्रकार _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      तथ्य((uchar*)तथ्य_3), datastart((uchar*)तथ्य_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(योग() == 0 || तथ्य != शून्य);

    माप_प्रकार esz = CV_ELEM_आकार(_type), esz1 = CV_ELEM_SIZE1(_type);
    माप_प्रकार minstep = cols*esz;
    अगर( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    अन्यथा
    {
        CV_CheckGE(_step, minstep, "");

        अगर (_step % esz1 != 0)
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

Mat& Mat::चालक = (स्थिर Mat& m)
{
    अगर( यह != &m )
    {
        अगर( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        size = m.size;
        step = m.step;
        तथ्य = m.तथ्य;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    वापस *यह;
}

Mat Mat::clone() स्थिर
{
    Mat m;
    copyTo(m);
    वापस m;
}

व्योम Mat::assignTo( Mat& m, पूर्णांक _type ) स्थिर
{
    अगर( _type < 0 )
        m = *यह;
    अन्यथा
        convertTo(m, _type);
}

व्योम Mat::create(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    _type &= TYPE_MASK;
    अगर( dims <= 2 && rows == _rows && cols == _cols && type() == _type && तथ्य )
        वापस;
    पूर्णांक sz[] = {_rows, _cols};
    create(2, sz, _type);
}

व्योम Mat::create(आकार _sz, पूर्णांक _type)
{
    create(_sz.height, _sz.width, _type);
}

व्योम Mat::create_Same_आकार(निवेश_Array m, पूर्णांक type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

व्योम Mat::fit(पूर्णांक _dims, स्थिर पूर्णांक* _sizes, पूर्णांक _type)
{
    माप_प्रकार old_योग_Bytes = u ? u->size : 0;
    माप_प्रकार esz = CV_ELEM_आकार(_type), new_योग = _dims >= 0;
    क्रम (पूर्णांक i = 0; i < _dims; i++)
        new_योग *= _sizes[i];
    माप_प्रकार new_योग_Bytes = new_योग*esz;
    अगर (new_योग_Bytes > 0 && (!isContinuous() ||
                              new_योग_Bytes > old_योग_Bytes ||
                              तथ्य != datastart)) {
        create(_dims, _sizes, _type);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        पूर्णांक dummy_आकार = 0;
        set_आकार(*यह, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_आकार), nullptr, true);
        finalizeHdr(*यह);
    }
}

व्योम Mat::fit(स्थिर std::vector<पूर्णांक>& _shape, पूर्णांक _type)
{
    fit((पूर्णांक)_shape.size(), _shape.तथ्य(), _type);
}

व्योम Mat::fit(स्थिर MatShape& _shape, पूर्णांक _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

व्योम Mat::fit(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक new_ndims = (पूर्णांक)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    क्रम (पूर्णांक i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

व्योम Mat::fit(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    _type &= TYPE_MASK;
    पूर्णांक sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

व्योम Mat::fit(आकार _sz, पूर्णांक _type)
{
    fit(_sz.height, _sz.width, _type);
}

व्योम Mat::fit_Same_आकार(निवेश_Array m, पूर्णांक _type)
{
    fit(m.shape(), _type);
}

व्योम Mat::addref()
{
    अगर( u )
        CV_XADD(&u->refcount, 1);
}

व्योम Mat::release()
{
    अगर( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = शून्य;
    datastart = dataend = datalimit = तथ्य = 0;
    क्रम(पूर्णांक i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.सफा();
}

माप_प्रकार Mat::step1(पूर्णांक i) स्थिर
{
    वापस step.p[i] / elemSize1();
}

bool Mat::empty() स्थिर
{
    वापस तथ्य == 0 || योग() == 0;
}

माप_प्रकार Mat::योग() स्थिर
{
    अगर( dims <= 2 )
        वापस (माप_प्रकार)rows * cols;
    माप_प्रकार p = 1;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
        p *= size[i];
    वापस p;
}

माप_प्रकार Mat::योग(पूर्णांक startDim, पूर्णांक endDim) स्थिर
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    माप_प्रकार p = 1;
    पूर्णांक endDim_ = endDim <= dims ? endDim : dims;
    क्रम( पूर्णांक i = startDim; i < endDim_; i++ )
        p *= size[i];
    वापस p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), तथ्य(m.तथ्य),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.सफा(); m.step.सफा();
    m.तथ्य = शून्य; m.datastart = शून्य; m.dataend = शून्य; m.datalimit = शून्य;
    m.allocator = शून्य;
    m.u = शून्य;
}


Mat& Mat::चालक = (Mat&& m)
{
    अगर (यह == &m)
      वापस *यह;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; तथ्य = m.तथ्य;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.सफा(); m.step.सफा();
    m.तथ्य = शून्य; m.datastart = शून्य; m.dataend = शून्य; m.datalimit = शून्य;
    m.allocator = शून्य;
    m.u = शून्य;
    वापस *यह;
}


व्योम Mat::create(पूर्णांक d0, स्थिर पूर्णांक* _sizes, पूर्णांक _type)
{
    पूर्णांक sz1 = 1, d = d0;
    पूर्णांक i;
    अगर (d == 0) {
        d = 1;
        _sizes = (स्थिर पूर्णांक*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    अगर( तथ्य && d == dims && _type == type() )
    {
        क्रम( i = 0; i < d; i++ )
            अगर( size[i] != _sizes[i] )
                अवरोध;
        अगर( i == d )
            वापस;
    }

    पूर्णांक _sizes_backup[CV_MAX_DIM]; // #5991
    अगर (_sizes == (यह->size.p))
    {
        क्रम(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    अगर( d == 0 )
        वापस;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    set_आकार(*यह, d, _sizes, 0, true);

    अगर( योग() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        अगर(!a)
            a = a0;
        प्रयास
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        पकड़ो (...)
        {
            अगर (a == a0)
                फेंको;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (माप_प्रकार)CV_ELEM_आकार(flags) );
    }

    addref();
    finalizeHdr(*यह);
    size.dims = dims = d0;
}

व्योम Mat::create(स्थिर std::vector<पूर्णांक>& _sizes, पूर्णांक _type)
{
    create((पूर्णांक)_sizes.size(), _sizes.तथ्य(), _type);
}

व्योम Mat::create(स्थिर MatShape& _shape, पूर्णांक _type)
{
    अगर (_shape.dims < 0) {
        release();
        वापस;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

व्योम Mat::create(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक new_ndims = (पूर्णांक)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    क्रम (पूर्णांक i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

व्योम Mat::copy_आकार(स्थिर Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

व्योम Mat::deallocate()
{
    अगर(u)
    {
        UMat_तथ्य* u_ = u;
        u = शून्य;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(स्थिर Mat& m, स्थिर Range& _rowRange, स्थिर Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    अगर( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        क्रम( पूर्णांक i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *यह = m(rs);
        वापस;
    }

    *यह = m;
    प्रयास
    {
        अगर( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            तथ्य += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        अगर( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            तथ्य += _colRange.start*elem_आकार();
            flags |= SUBMATRIX_FLAG;
        }
    }
    पकड़ो(...)
    {
        release();
        फेंको;
    }

    updateContinuityFlag();

    अगर( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(स्थिर Mat& m, स्थिर Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    तथ्य(m.तथ्य + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    माप_प्रकार esz = CV_ELEM_आकार(flags);
    तथ्य += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    अगर( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    अगर( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(पूर्णांक _dims, स्थिर पूर्णांक* _sizes, पूर्णांक _type, व्योम* तथ्य_3, स्थिर माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = तथ्य = (uchar*)तथ्य_3;
    अगर (_dims > 0 || तथ्य_3 != nullptr)
        set_आकार(*यह, _dims, _sizes, _steps, true);
    finalizeHdr(*यह);
}


Mat::Mat(स्थिर std::vector<पूर्णांक>& _sizes, पूर्णांक _type, व्योम* तथ्य_3, स्थिर माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = तथ्य = (uchar*)तथ्य_3;
    पूर्णांक _dims = (पूर्णांक)_sizes.size();
    अगर (_dims > 0 || तथ्य_3 != nullptr)
        set_आकार(*यह, _dims, _sizes.तथ्य(), _steps, true);
    finalizeHdr(*यह);
}

Mat::Mat(स्थिर MatShape& _shape, पूर्णांक _type, व्योम* तथ्य_3, स्थिर माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = तथ्य = (uchar*)तथ्य_3;
    अगर (_shape.dims >= 0) {
        set_आकार(*यह, _shape.dims, _shape.p, _steps, true);
    }
    अन्यथा {
        CV_Assert(!तथ्य);
    }
    finalizeHdr(*यह);
}

Mat::Mat(std::initializer_list<पूर्णांक> _shape, पूर्णांक _type, व्योम* तथ्य_3, स्थिर माप_प्रकार* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णांक new_shape[MatShape::MAX_DIMS];
    पूर्णांक _dims = (पूर्णांक)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    स्वतः it = _shape.begin();
    क्रम (पूर्णांक i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = तथ्य = (uchar*)तथ्य_3;
    अगर (_dims > 0 || तथ्य_3 != nullptr)
        set_आकार(*यह, _dims, new_shape, _steps, true);
    finalizeHdr(*यह);
}

Mat::Mat(स्थिर Mat& m, स्थिर Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णांक d = m.dims;

    CV_Assert(ranges);
    क्रम( पूर्णांक i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *यह = m;
    क्रम( पूर्णांक i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        अगर( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            तथ्य += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    अगर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(स्थिर Mat& m, स्थिर std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), तथ्य(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    पूर्णांक d = m.dims;

    CV_Assert((पूर्णांक)ranges.size() == d);
    क्रम (पूर्णांक i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *यह = m;
    क्रम (पूर्णांक i = 0; i < d; i++)
    {
        Range r = ranges[i];
        अगर (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            तथ्य += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    अगर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(पूर्णांक d) स्थिर
{
    CV_Assert( dims == 2 );
    Mat m = *यह;
    माप_प्रकार esz = elem_आकार();
    पूर्णांक len;

    अगर( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.तथ्य += esz*d;
    }
    अन्यथा
    {
        len = std::min(rows + d, cols);
        m.तथ्य -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    अगर( size() != आकार(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    वापस m;
}


व्योम Mat::pop_back(माप_प्रकार nelems)
{
    CV_Assert( nelems <= (माप_प्रकार)size.p[0] );
    CV_Assert( dims >= 1 );

    अगर( isSubmatrix() )
        *यह = rowRange(0, size.p[0] - (पूर्णांक)nelems);
    अन्यथा
    {
        size.p[0] -= (पूर्णांक)nelems;
        अगर (dims == 2)
            rows = size.p[0];
        अन्यथा अगर (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


व्योम Mat::धकेलो_back(स्थिर व्योम* elem)
{
    माप_प्रकार r = size.p[0];
    अगर( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    माप_प्रकार esz = elem_आकार();
    स_नकल(तथ्य + r*step.p[0], elem, esz);
    size.p[0] = पूर्णांक(r + 1);
    अगर (dims == 2)
        rows = size.p[0];
    अन्यथा अगर (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    क्रम( पूर्णांक i = 1; i < dims; i++ )
        tsz *= size.p[i];
    अगर( esz < step.p[0] || tsz != (uint64)(पूर्णांक)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


व्योम Mat::reserve(माप_प्रकार nelems)
{
    स्थिर माप_प्रकार MIN_आकार = 64;

    CV_Assert( (पूर्णांक)nelems >= 0 );
    अगर( !isSubmatrix() && step.p[0] != 0 && तथ्य + step.p[0]*nelems <= datalimit )
        वापस;

    पूर्णांक r = size.p[0];
    अगर( (माप_प्रकार)r >= nelems)
        वापस;

    MatShape newsize = size;
    माप_प्रकार esz = elem_आकार();
    newsize.dims = std::max(newsize.dims, 1);
    अगर (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (पूर्णांक)nelems;
    माप_प्रकार newbytes = newsize.योग() * esz;
    अगर (newbytes < MIN_आकार) {
        newsize.p[0] = 1;
        newsize.p[0] = पूर्णांक(MIN_आकार / newsize.योग() / esz);
    }

    Mat m(newsize, type());
    अगर( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *यह = m;
    size.p[0] = r;

    अगर (dims == 2)
        rows = size.p[0];
    अन्यथा अगर (dims == 1)
        cols = size.p[0];
    dataend = तथ्य + step.p[0]*r;
}


व्योम Mat::reserve_बफर(माप_प्रकार nbytes)
{
    माप_प्रकार esz = 1;
    पूर्णांक mtype = CV_8UC1;
    अगर (!empty())
    {
        अगर (!isSubmatrix() && तथ्य + nbytes <= dataend)//Should it be datalimit?
            वापस;
        esz = elem_आकार();
        mtype = type();
    }

    माप_प्रकार nelems = (nbytes - 1) / esz + 1;

#अगर SIZE_MAX > अच_पूर्णांक_उच्च
    CV_Assert(nelems <= माप_प्रकार(पूर्णांक_उच्च)*माप_प्रकार(पूर्णांक_उच्च));
    पूर्णांक newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x400*माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x100000 * माप_प्रकार(पूर्णांक_उच्च) ? nelems > 0x40000000 * माप_प्रकार(पूर्णांक_उच्च) ?
                  माप_प्रकार(पूर्णांक_उच्च) : 0x40000000 : 0x100000 : 0x400 : 1;
#अन्यथा
    पूर्णांक newrows = nelems > माप_प्रकार(पूर्णांक_उच्च) ? 2 : 1;
#पूर्ण_अगर
    पूर्णांक newcols = (पूर्णांक)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


व्योम Mat::resize(माप_प्रकार nelems)
{
    पूर्णांक r = size.p[0];
    अगर( r == (पूर्णांक)nelems )
        वापस;
    CV_Assert( (पूर्णांक)nelems >= 0 );

    अगर( isSubmatrix() || तथ्य + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (पूर्णांक)nelems;
    अगर (dims == 2)
        rows = size.p[0];
    अन्यथा अगर (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


व्योम Mat::resize(माप_प्रकार nelems, स्थिर Scalar& s)
{
    पूर्णांक saveRows = size.p[0];
    resize(nelems);

    अगर( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

व्योम Mat::push_back(स्थिर Mat& elems)
{
    माप_प्रकार r = size.p[0];
    माप_प्रकार delta = elems.size.p[0];
    अगर( delta == 0 )
        वापस;
    अगर( यह == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        वापस;
    }
    अगर( !तथ्य )
    {
        *यह = elems.clone();
        वापस;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = पूर्णांक(r);
    अगर( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    अगर( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    माप_प्रकार esz = elem_आकार();
    माप_प्रकार minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    माप_प्रकार step0 = step.p[0];
    अगर (step0 < minstep) {
        अगर (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    अगर( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += पूर्णांक(delta);
    अगर (dims == 2)
        rows = size.p[0];
    अन्यथा अगर (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    अगर( isContinuous() && elems.isContinuous() )
        स_नकल(तथ्य + r*step0, elems.तथ्य, elems.योग()*elems.elem_आकार());
    अन्यथा
    {
        Mat part = rowRange(पूर्णांक(r), पूर्णांक(r + delta));
        elems.copyTo(part);
    }
}


व्योम Mat::locateROI( आकार& whole_आकार, Point& ofs ) स्थिर
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    माप_प्रकार esz = elem_आकार(), minstep;
    सूचक_भेद_प्रकार delta1 = तथ्य - datastart, delta2 = dataend - datastart;

    अगर( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णांक)(delta1/step[0]);
        ofs.x = (पूर्णांक)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( तथ्य == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    whole_आकार.height = (पूर्णांक)((delta2 - minstep)/step[0] + 1);
    whole_आकार.height = std::max(whole_आकार.height, ofs.y + rows);
    whole_आकार.width = (पूर्णांक)((delta2 - step*(whole_आकार.height-1))/esz);
    whole_आकार.width = std::max(whole_आकार.width, ofs.x + cols);
}

Mat& Mat::adjustROI( पूर्णांक dtop, पूर्णांक dbottom, पूर्णांक dleft, पूर्णांक dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    आकार whole_आकार; Point ofs;
    माप_प्रकार esz = elem_आकार();
    locateROI( whole_आकार, ofs );
    पूर्णांक row1 = std::min(std::max(ofs.y - dtop, 0), whole_आकार.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, whole_आकार.height));
    पूर्णांक col1 = std::min(std::max(ofs.x - dleft, 0), whole_आकार.width), col2 = std::max(0, std::min(ofs.x + cols + dright, whole_आकार.width));
    अगर(row1 > row2)
        std::swap(row1, row2);
    अगर(col1 > col2)
        std::swap(col1, col2);

    अगर (dims == 1) {
        तथ्य += (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } अन्यथा {
        तथ्य += (row1 - ofs.y)*(std::सूचक_भेद_प्रकार)step + (col1 - ofs.x)*(std::सूचक_भेद_प्रकार)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    वापस *यह;
}

Mat Mat::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिर
{
    पूर्णांक cn = channels();
    Mat hdr = *यह;

    अगर( new_cn == 0 )
        new_cn = cn;

    अगर( dims > 2 )
    {
        अगर( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_आकार(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            वापस hdr;
        }
        CV_Assert( new_rows > 0 );
        पूर्णांक sz[] = { new_rows, (पूर्णांक)(योग()*cn/new_rows) };
        वापस reshape(new_cn, 2, sz);
    }

    पूर्णांक योग_width = cols * cn;

    अगर( (new_cn > योग_width || योग_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * योग_width / new_cn;

    अगर( new_rows != 0 && new_rows != rows )
    {
        पूर्णांक योग_आकार_2 = योग_width * rows;
        अगर( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        योग_width = योग_आकार_2 / new_rows;

        अगर( योग_width * new_rows != योग_आकार_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = योग_width * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.rows = rows;
        अगर (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_आकार(flags);
    }

    पूर्णांक new_width = योग_width / new_cn;

    अगर( new_width * new_cn != योग_width )
        CV_Error( cv::Error::Bad_अंक_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_आकार(hdr.flags);
    वापस hdr;
}

Mat Mat::reshape(पूर्णांक _cn, पूर्णांक _newndims, स्थिर पूर्णांक* _newsz) स्थिर
{
    अगर(_newndims == dims)
    {
        अगर(_newsz == 0)
            वापस reshape(_cn);
        अगर(_newndims == 2)
            वापस reshape(_cn, _newsz[0]);
    }

    अगर (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        अगर (_cn == 0)
            _cn = यह->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        माप_प्रकार योग_elem1_ref = यह->योग() * यह->channels();
        माप_प्रकार योग_elem1 = _cn;

        Auto_बफर<पूर्णांक, 4> newsz_buf( (माप_प्रकार)_newndims );

        पूर्णांक m1_idx = -1;

        क्रम (पूर्णांक i = 0; i < _newndims; i++)
        {
            अगर (_newsz[i] >= 0) {
                अगर (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                योग_elem1 *= (माप_प्रकार)newsz_buf[i];
            } अन्यथा {
                अगर (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_आकार, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        अगर (m1_idx >= 0) {
            अगर (योग_elem1 == 0) {
                CV_Assert(योग_elem1_ref == 0);
                योग_elem1 = 1;
            }
            CV_Assert(योग_elem1_ref % योग_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णांक)(योग_elem1_ref / योग_elem1);
        } अन्यथा अगर (योग_elem1 != योग_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *यह;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        set_आकार(hdr, _newndims, newsz_buf.तथ्य(), शून्य, true);

        वापस hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(पूर्णांक _cn, स्थिर std::vector<पूर्णांक>& _newshape) स्थिर
{
    पूर्णांक newdims = (पूर्णांक)_newshape.size();
    अगर(newdims == 0 && empty())
    {
        वापस *यह;
    }

    वापस reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(पूर्णांक _cn, स्थिर MatShape& _newshape) स्थिर
{
    अगर (_newshape.dims < 0) {
        पूर्णांक newshape[] = {0};
        वापस reshape(_cn, 1, newshape);
    }
    वापस reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(पूर्णांक _cn, std::initializer_list<पूर्णांक> newshape_) स्थिर
{
    पूर्णांक newshape[MatShape::MAX_DIMS];
    माप_प्रकार i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (माप_प्रकार)MatShape::MAX_DIMS);
    स्वतः it = newshape_.begin();
    क्रम (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    वापस reshape(_cn, (पूर्णांक)newshape_dims, newshape);
}

Mat Mat::reinterpret(पूर्णांक type) स्थिर
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_आकार(यह->type()) == CV_ELEM_आकार(type));
    Mat m = *यह;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    वापस m;
}

Mat Mat::diag(स्थिर Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    पूर्णांक len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    अगर( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    वापस m;
}

पूर्णांक Mat::checkVector(पूर्णांक _elemChannels, पूर्णांक _depth, bool _requireContinuous) स्थिर
{
    वापस तथ्य && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णांक)(योग()*channels()/_elemChannels) : -1;
}


अटल अंतरभूत आकार get_Continuous_आकार(पूर्णांक flags, पूर्णांक cols, पूर्णांक rows, पूर्णांक widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    bool has_int_overflow = sz >= पूर्णांक_उच्च;
    bool isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    वापस (isContiguous && !has_int_overflow)
            ? आकार((पूर्णांक)sz, 1)
            : आकार(cols * widthScale, rows);
}

आकार getContinuousSize2D(Mat& m1, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    वापस get_Continuous_आकार(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
आकार getContinuousSize2D(Mat& m1, Mat& m2, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    स्थिर आकार sz1 = m1.size();
    अगर (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार योग_sz = m1.योग();
        CV_CheckEQ(योग_sz, m2.योग(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        पूर्णांक योग = (पूर्णांक)योग_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)योग_sz * widthScale) >= पूर्णांक_उच्च;
        अगर (isContiguous && !has_int_overflow)
            योग = 1; // vector-row
        m1 = m1.reshape(0, योग);
        m2 = m2.reshape(0, योग);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        वापस आकार(m1.cols * widthScale, m1.rows);
    }
    वापस get_Continuous_आकार(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

आकार getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णांक widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    स्थिर आकार sz1 = m1.size();
    अगर (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        माप_प्रकार योग_sz = m1.योग();
        CV_CheckEQ(योग_sz, m2.योग(), "");
        CV_CheckEQ(योग_sz, m3.योग(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        bool is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        पूर्णांक योग = (पूर्णांक)योग_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)योग_sz * widthScale) >= पूर्णांक_उच्च;
        अगर (isContiguous && !has_int_overflow)
            योग = 1; // vector-row
        m1 = m1.reshape(0, योग);
        m2 = m2.reshape(0, योग);
        m3 = m3.reshape(0, योग);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        वापस आकार(m1.cols * widthScale, m1.rows);
    }
    वापस get_Continuous_आकार(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
