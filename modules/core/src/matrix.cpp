// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "bufferpool.impl.hpp"

नामाकाश cv {

std::string layout_गन्तव्यम्_String(दत्त_Layout layout)
{
    प्रत्यागम
        layout == दत्त_LAYOUT_ND ? "ND" :
        layout == दत्त_LAYOUT_NCHW ? "NCHW" :
        layout == दत्त_LAYOUT_NHWC ? "NHWC" :
        layout == दत्त_LAYOUT_खण्डम् ? "BLOCK" :
        layout == दत्त_LAYOUT_NCDHW ? "NCDHW" :
        layout == दत्त_LAYOUT_NDHWC ? "NDHWC" :
        layout == दत्त_LAYOUT_PLANAR ? "PLANAR" :
        layout == दत्त_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

तार्किक संक्रियकः == (ध्रुव MatShape& size1, ध्रुव MatShape& size2)
{
    यदि (size1.dims != size2.dims)
        प्रत्यागम असत्य;
    यदि (size1.layout != size2.layout &&
        size1.layout != दत्त_LAYOUT_UNKNOWN &&
        size2.layout != दत्त_LAYOUT_UNKNOWN)
        प्रत्यागम असत्य;
    यदि (size1.layout == दत्त_LAYOUT_खण्डम् &&
        size2.layout == दत्त_LAYOUT_खण्डम् &&
        size1.C != size2.C)
        प्रत्यागम असत्य;
    प्रति (पूर्णाङ्क i = 0; i < size1.dims; i++) {
        यदि (size1.p[i] != size2.p[i])
            प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
}

तार्किक संक्रियकः != (ध्रुव MatShape& size1, ध्रुव MatShape& size2)
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
    layout = दत्त_LAYOUT_UNKNOWN;
    C = 0;
    प्रति (पूर्णाङ्क i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

शून्य MatShape::resize(size_t new_आकार, पूर्णाङ्क value)
{
    CV_Assert(new_आकार < (size_t)MAX_DIMS);
    पूर्णाङ्क old_dims = std::max(dims, 0);
    dims = (पूर्णाङ्क)new_आकार;
    प्रति (पूर्णाङ्क i = old_dims; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

शून्य MatShape::assign(size_t new_आकार, पूर्णाङ्क value)
{
    CV_Assert(new_आकार < (size_t)MAX_DIMS);
    dims = (पूर्णाङ्क)new_आकार;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        p[i] = value;
}

शून्य MatShape::assign(पूर्णाङ्क new_आकार, पूर्णाङ्क value)
{
    assign((size_t)new_आकार, value);
}

शून्य MatShape::assign(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    नियोजय(begin, end);
}

शून्य MatShape::नियोजय(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    ptrdiff_t new_आकार = end - begin;
    CV_Assert(0 <= new_आकार && new_आकार < (ptrdiff_t)MAX_DIMS);
    dims = (पूर्णाङ्क)new_आकार;
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
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    प्रति (पूर्णाङ्क i = old_dims-1; i >= (पूर्णाङ्क)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

शून्य MatShape::insert(पूर्णाङ्क* where, size_t गणनम्_2, पूर्णाङ्क value)
{
    पूर्णाङ्क old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+गणनम्_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (पूर्णाङ्क)(old_dims+गणनम्_2);
    प्रति (पूर्णाङ्क i = old_dims-1; i >= (पूर्णाङ्क)ofs; i--)
        p[i+गणनम्_2] = p[i];
    प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)गणनम्_2; i++)
        p[i+ofs] = value;
}

शून्य MatShape::insert(पूर्णाङ्क* where, पूर्णाङ्क गणनम्_2, पूर्णाङ्क value)
{
    insert(where, (size_t)गणनम्_2, value);
}

शून्य MatShape::insert(पूर्णाङ्क* where, ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    निवेशय(where, begin, end);
}

शून्य MatShape::निवेशय(पूर्णाङ्क* where, ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end)
{
    पूर्णाङ्क old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
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
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    यदि (ofs == dims)
        प्रत्यागम;
    dims--;
    प्रति (पूर्णाङ्क i = (पूर्णाङ्क)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::योग() ध्रुव
{
    size_t परिणाम = 1;
    यदि (dims < 0)
        प्रत्यागम 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        परिणाम *= p[i];
    प्रत्यागम परिणाम;
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

स्थिर शून्य finalize_खण्डम्_Layout(MatShape& size, पूर्णाङ्क C=0)
{
    यदि (size.layout == दत्त_LAYOUT_खण्डम्) {
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

MatShape::MatShape(size_t dims_, ध्रुव पूर्णाङ्क* आकार_3, दत्त_Layout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (पूर्णाङ्क)dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = आकार_3 ? आकार_3[i] : 0;
    }
    finalize_खण्डम्_Layout(*एतत्, C_);
}

MatShape::MatShape(size_t dims_, पूर्णाङ्क value, दत्त_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (पूर्णाङ्क)dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_खण्डम्_Layout(*एतत्, 0);
}

MatShape::MatShape(std::initializer_list<पूर्णाङ्क> shape)
{
    layout = दत्त_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (पूर्णाङ्क)shape.size();
    स्वचालित it = shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_खण्डम्_Layout(*एतत्, 0);
}

MatShape::MatShape(पूर्णाङ्क dims_, पूर्णाङ्क value, दत्त_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_खण्डम्_Layout(*एतत्, 0);
}

MatShape::MatShape(ध्रुव std::vector<पूर्णाङ्क>& shape_, दत्त_Layout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    size_t shape_आकार = shape_.size();
    CV_Assert(shape_आकार < (size_t)MAX_DIMS);
    dims = (पूर्णाङ्क)shape_आकार;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_खण्डम्_Layout(*एतत्, C_);
}

MatShape::MatShape(ध्रुव पूर्णाङ्क* begin, ध्रुव पूर्णाङ्क* end, दत्त_Layout layout_, पूर्णाङ्क C_)
{
    layout = layout_;
    ptrdiff_t shape_आकार = end - begin;
    CV_Assert(0 <= shape_आकार && shape_आकार < MAX_DIMS);
    dims = (पूर्णाङ्क)shape_आकार;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_खण्डम्_Layout(*एतत्, C_);
}

MatShape::MatShape(ध्रुव MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    प्रति (पूर्णाङ्क i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::संक्रियकः = (ध्रुव MatShape& shape)
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
    CV_Assert(layout == दत्त_LAYOUT_खण्डम् || layout == दत्त_LAYOUT_NCHW || layout == दत्त_LAYOUT_NHWC);
    प्रत्यागम layout == दत्त_LAYOUT_खण्डम् ? C : p[layout == दत्त_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::गन्तव्यम्_Layout(दत्त_Layout newLayout, पूर्णाङ्क C0) ध्रुव
{
    CV_Assert(layout == दत्त_LAYOUT_खण्डम् || layout == दत्त_LAYOUT_NCHW || layout == दत्त_LAYOUT_NHWC);
    CV_Assert(newLayout == दत्त_LAYOUT_खण्डम् || newLayout == दत्त_LAYOUT_NCHW || newLayout == दत्त_LAYOUT_NHWC);

    MatShape newsize = *एतत्;
    newsize.layout = newLayout;

    यदि (newLayout == दत्त_LAYOUT_खण्डम्) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        पूर्णाङ्क Corig = channels();
        newsize.C = Corig;

        यदि (layout == दत्त_LAYOUT_NHWC) {
            प्रति (पूर्णाङ्क i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != दत्त_LAYOUT_खण्डम्;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } अन्यथा यदि (layout == दत्त_LAYOUT_खण्डम्) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        यदि (newLayout == दत्त_LAYOUT_NHWC) {
            प्रति (पूर्णाङ्क i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == दत्त_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } अन्यथा {
        CV_Assert_N(C0 <= 1);

        यदि (newLayout == layout)
            प्रत्यागम newsize;

        // NHWC => NCHW
        यदि (newLayout == दत्त_LAYOUT_NCHW) {
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
स्थिर MatShape गन्तव्यम्_Semantic(ध्रुव MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    प्रत्यागम shape.गन्तव्यम्_Layout(दत्त_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
स्थिर MatShape गन्तव्यम्_खण्डम्_सम्बन्धि(MatShape परिणाम, ध्रुव MatShape& src, ध्रुव वर्ण* op)
{
    यदि (परिणाम.dims < 2 || परिणाम.p[0] != src.p[0] || परिणाम.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(परिणाम.dims + 1, (पूर्णाङ्क)MatShape::MAX_DIMS, "too many dimensions");
    परिणाम.layout = दत्त_LAYOUT_NCHW;
    प्रत्यागम परिणाम.गन्तव्यम्_Layout(दत्त_LAYOUT_खण्डम्, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ध्रुव std::vector<पूर्णाङ्क>& axes) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == दत्त_LAYOUT_खण्डम्)
        प्रत्यागम गन्तव्यम्_खण्डम्_सम्बन्धि(गन्तव्यम्_Semantic(*एतत्).squeeze(axes), *एतत्, "squeeze");
    तार्किक remove[MAX_DIMS] = {असत्य};
    यदि (axes.empty())
    {
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    प्रति (पूर्णाङ्क a : axes)
    {
        पूर्णाङ्क i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        यदि (!remove[i])
            परिणाम.p[परिणाम.dims++] = p[i];
    प्रत्यागम परिणाम;
}

MatShape MatShape::unsqueeze(ध्रुव std::vector<पूर्णाङ्क>& axes) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == दत्त_LAYOUT_खण्डम्)
        प्रत्यागम गन्तव्यम्_खण्डम्_सम्बन्धि(गन्तव्यम्_Semantic(*एतत्).unsqueeze(axes), *एतत्, "unsqueeze");
    ध्रुव पूर्णाङ्क बहिर्गमनम्_Dims = dims + (पूर्णाङ्क)axes.size();
    CV_CheckLE(बहिर्गमनम्_Dims, (पूर्णाङ्क)MAX_DIMS, "unsqueeze: too many dimensions");
    तार्किक insert[MAX_DIMS] = {असत्य};
    प्रति (पूर्णाङ्क a : axes)
    {
        पूर्णाङ्क i = normalizeShapeAxis(a, बहिर्गमनम्_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = सत्य;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = बहिर्गमनम्_Dims;
    प्रति (पूर्णाङ्क i = 0, j = 0; i < बहिर्गमनम्_Dims; i++)
        परिणाम.p[i] = insert[i] ? 1 : p[j++];
    प्रत्यागम परिणाम;
}

MatShape MatShape::flatten(पूर्णाङ्क startAxis, पूर्णाङ्क अन्तः_Axis) ध्रुव
{
    CV_Assert(dims >= 0);
    यदि (layout == दत्त_LAYOUT_खण्डम्)
        प्रत्यागम गन्तव्यम्_खण्डम्_सम्बन्धि(गन्तव्यम्_Semantic(*एतत्).flatten(startAxis, अन्तः_Axis), *एतत्, "flatten");
    यदि (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(अन्तः_Axis, अन्तः_Axis == 0 || अन्तः_Axis == -1, "flatten: axis is out of range");
        प्रत्यागम MatShape({1});
    }
    पूर्णाङ्क start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(अन्तः_Axis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    प्रति (पूर्णाङ्क i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((द्विगुण)merged, (द्विगुण)अधिकतम_पूर्णाङ्कः, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape परिणाम;
    परिणाम.dims = 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        यदि (i == start)
            परिणाम.p[परिणाम.dims++] = (पूर्णाङ्क)merged;
        अन्यथा यदि (i < start || i > end)
            परिणाम.p[परिणाम.dims++] = p[i];
    }
    प्रत्यागम परिणाम;
}

MatShape MatShape::reshape(ध्रुव MatShape& newShape, तार्किक allowZero) ध्रुव
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    यदि (layout == दत्त_LAYOUT_खण्डम्)
        प्रत्यागम गन्तव्यम्_खण्डम्_सम्बन्धि(गन्तव्यम्_Semantic(*एतत्).reshape(newShape, allowZero), *एतत्, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape परिणाम;
    परिणाम.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, परिणाम.p);
    पूर्णाङ्क inferred = -1;
    int64 ज्ञातम् = 1;
    प्रति (पूर्णाङ्क i = 0; i < परिणाम.dims; i++)
    {
        पूर्णाङ्क& sz = परिणाम.p[i];
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
        ज्ञातम् *= sz;
    }
    ध्रुव int64 योग_3 = (int64)योग();
    यदि (inferred >= 0)
    {
        CV_Check((द्विगुण)ज्ञातम्, ज्ञातम् != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((द्विगुण)योग_3, योग_3 % ज्ञातम् == 0, "reshape: the number of elements does not match");
        CV_CheckLE((द्विगुण)(योग_3/ज्ञातम्), (द्विगुण)अधिकतम_पूर्णाङ्कः, "reshape: the result is too big");
        परिणाम.p[inferred] = (पूर्णाङ्क)(योग_3/ज्ञातम्);
    }
    अन्यथा यदि (ज्ञातम् != योग_3)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (दीर्घ दीर्घ)ज्ञातम्, (दीर्घ दीर्घ)योग_3));
    प्रत्यागम परिणाम;
}

MatShape MatShape::expand(ध्रुव MatShape& another) ध्रुव
{
    यदि (dims == 0)
        प्रत्यागम another;
    यदि (another.dims == 0)
        प्रत्यागम *एतत्;

    यदि ((layout == दत्त_LAYOUT_NCHW || layout == दत्त_LAYOUT_NHWC) &&
        (another.layout == दत्त_LAYOUT_NCHW || another.layout == दत्त_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != दत्त_LAYOUT_खण्डम् && another.layout != दत्त_LAYOUT_खण्डम्);

    MatShape परिणाम;

    यदि (dims < 0 || another.dims < 0)
        प्रत्यागम परिणाम;

    परिणाम = *एतत्;
    परिणाम.dims = std::max(dims, another.dims);
    परिणाम.layout = layout == दत्त_LAYOUT_UNKNOWN ? another.layout :
        layout == दत्त_LAYOUT_ND && (another.layout == दत्त_LAYOUT_NCHW ||
        another.layout == दत्त_LAYOUT_NHWC) ? another.layout : layout;
    प्रति (पूर्णाङ्क i = परिणाम.dims-1; i >= 0; i--) {
        पूर्णाङ्क i1 = i - (परिणाम.dims - dims);
        पूर्णाङ्क i2 = i - (परिणाम.dims - another.dims);
        पूर्णाङ्क sz1 = i1 < 0 ? 1 : p[i1];
        पूर्णाङ्क sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        परिणाम.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    प्रत्यागम परिणाम;
}

/////////////////////////// MatAllocator ////////////////////////////

शून्य MatAllocator::map(UMat_दत्त*, AccessFlag) ध्रुव
{
}

शून्य MatAllocator::unmap(UMat_दत्त* u) ध्रुव
{
    यदि(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

शून्य MatAllocator::download(UMat_दत्त* u, शून्य* dstptr,
         पूर्णाङ्क dims, ध्रुव size_t sz[],
         ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
         ध्रुव size_t dststep[]) ध्रुव
{
    यदि(!u)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* srcptr = u->दत्त;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
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
    size_t planesz = it.size;

    प्रति( size_t j = 0; j < it.nplanes; j++, ++it )
        स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
}


शून्य MatAllocator::upload(UMat_दत्त* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                    ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                    ध्रुव size_t srcstep[]) ध्रुव
{
    यदि(!u)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* dstptr = u->दत्त;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
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
    size_t planesz = it.size;

    प्रति( size_t j = 0; j < it.nplanes; j++, ++it )
        स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
}

शून्य MatAllocator::copy(UMat_दत्त* usrc, UMat_दत्त* udst, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक /*sync*/) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि(!usrc || !udst)
        प्रत्यागम;
    पूर्णाङ्क isz[CV_MAX_DIM];
    uchar* srcptr = usrc->दत्त;
    uchar* dstptr = udst->दत्त;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
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
    size_t planesz = it.size;

    प्रति( size_t j = 0; j < it.nplanes; j++, ++it )
        स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
}

बफर_Pool_Controller* MatAllocator::get_बफर_Pool_Controller(ध्रुव वर्ण* id) ध्रुव
{
    CV_अप्रयुक्तम्(id);
    स्थिर Dummy_बफर_Pool_Controller dummy;
    प्रत्यागम &dummy;
}

वर्ग StdMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMat_दत्त* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* data0, size_t* चरणः, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        size_t योग = CV_ELEM_आकार(type);
        प्रति( पूर्णाङ्क i = dims-1; i >= 0; i-- )
        {
            यदि( चरणः )
            {
                यदि( data0 && चरणः[i] != CV_AUTOSTEP )
                {
                    CV_Assert(योग <= चरणः[i]);
                    योग = चरणः[i];
                }
                अन्यथा
                    चरणः[i] = योग;
            }
            योग *= sizes[i];
        }
        uchar* दत्त = data0 ? (uchar*)data0 : (uchar*)fastMalloc(योग);
        UMat_दत्त* u = नूतन UMat_दत्त(एतत्);
        u->दत्त = u->origdata = दत्त;
        u->size = योग;
        यदि(data0)
            u->flags |= UMat_दत्त::USER_ALLOCATED;

        प्रत्यागम u;
    }

    तार्किक allocate(UMat_दत्त* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        यदि(!u) प्रत्यागम असत्य;
        प्रत्यागम सत्य;
    }

    शून्य deallocate(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        यदि( !(u->flags & UMat_दत्त::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        विलोपय u;
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

शून्य Mat::समुच्चयः_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, नूतन StdMatAllocator())
}

//==================================================================================================

शून्य समुच्चयः_आकार( Mat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* चरणाः_2, तार्किक auto_चरणाः)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.चरणः[std::max(_dims-1, 0)] = CV_ELEM_आकार(m.flags);
    प्रति (पूर्णाङ्क i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.चरणः[i+1];
        यदि (चरणाः_2) {
            m.चरणः[i] = चरणाः_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा यदि (auto_चरणाः) {
            m.चरणः[i] = autostep;
        } अन्यथा {
            m.चरणः[i] = 0;
        }
    }

    यदि( _dims <= 2 )
    {
        m.स्तम्भसङ्ख्या = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.पङ्क्तिकाः = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.स्तम्भसङ्ख्या = m.पङ्क्तिकाः = -1;
    }
}

पूर्णाङ्क updateContinuityFlag(पूर्णाङ्क flags, पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* size, ध्रुव size_t* चरणः)
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
        यदि( चरणः[j]*size[j] < चरणः[j-1] )
            भङ्ग;
    }

    यदि( j <= i && t == (uint64)(पूर्णाङ्क)t )
        प्रत्यागम flags | Mat::CONTINUOUS_FLAG;
    प्रत्यागम flags & ~Mat::CONTINUOUS_FLAG;
}

शून्य Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, चरणः.p);
}

शून्य finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    पूर्णाङ्क d = m.dims;
    यदि( d > 2 )
        m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = -1;
    यदि(m.u)
        m.datastart = m.दत्त = m.u->दत्त;
    यदि( m.दत्त && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.चरणः[0];
        यदि( m.size[0] > 0 )
        {
            पूर्णाङ्क lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.चरणः[lastdim];
            प्रति( पूर्णाङ्क i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.चरणः[i];
        }
        अन्यथा
            m.dataend = m.datalimit;
    }
    अन्यथा
        m.dataend = m.datalimit = m.दत्त;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, _type);
}

Mat::Mat(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, _type);
    *एतत् = _s;
}

Mat::Mat(आकार _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.विस्तारः, _type );
}

Mat::Mat(आकार _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.विस्तारः, _type);
    *एतत् = _s;
}

Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sz, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *एतत् = _s;
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *एतत् = _s;
}

Mat::Mat(ध्रुव Mat& m)
    : flags(m.flags), dims(m.dims), पङ्क्तिकाः(m.पङ्क्तिकाः), स्तम्भसङ्ख्या(m.स्तम्भसङ्ख्या), दत्त(m.दत्त),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), चरणः(m.चरणः)
{
    यदि( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, शून्य* दत्त_3, size_t चरणः_3)
    : flags(MAGIC_VAL + (_type & TYPE_आवरणम्)), dims(2), पङ्क्तिकाः(पङ्क्तिकाः_3), स्तम्भसङ्ख्या(स्तम्भसङ्ख्या_3),
      दत्त((uchar*)दत्त_3), datastart((uchar*)दत्त_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(योग() == 0 || दत्त != रिक्त);

    size_t esz = CV_ELEM_आकार(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = स्तम्भसङ्ख्या * esz;
    यदि( चरणः_3 == AUTO_चरणः )
    {
        चरणः_3 = minstep;
    }
    अन्यथा
    {
        CV_Assert( चरणः_3 >= minstep );
        यदि (चरणः_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_चरणः, "Step must be a multiple of esz1");
        }
    }
    size[0] = पङ्क्तिकाः;
    size[1] = स्तम्भसङ्ख्या;
    चरणः[0] = चरणः_3;
    चरणः[1] = esz;
    datalimit = datastart + चरणः_3 * पङ्क्तिकाः;
    dataend = datalimit - चरणः_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(आकार _sz, पूर्णाङ्क _type, शून्य* दत्त_3, size_t चरणः_3)
    : flags(MAGIC_VAL + (_type & TYPE_आवरणम्)), dims(2), पङ्क्तिकाः(_sz.height), स्तम्भसङ्ख्या(_sz.विस्तारः),
      दत्त((uchar*)दत्त_3), datastart((uchar*)दत्त_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(योग() == 0 || दत्त != रिक्त);

    size_t esz = CV_ELEM_आकार(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = स्तम्भसङ्ख्या*esz;
    यदि( चरणः_3 == AUTO_चरणः || (चरणः_3 < minstep && पङ्क्तिकाः == 1))
    {
        चरणः_3 = minstep;
    }
    अन्यथा
    {
        CV_CheckGE(चरणः_3, minstep, "");

        यदि (चरणः_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_चरणः, "Step must be a multiple of esz1");
        }
    }
    size[0] = पङ्क्तिकाः;
    size[1] = स्तम्भसङ्ख्या;
    चरणः[0] = चरणः_3;
    चरणः[1] = esz;
    datalimit = datastart + चरणः_3*पङ्क्तिकाः;
    dataend = datalimit - चरणः_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::संक्रियकः = (ध्रुव Mat& m)
{
    यदि( एतत् != &m )
    {
        यदि( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        पङ्क्तिकाः = m.पङ्क्तिकाः;
        स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
        size = m.size;
        चरणः = m.चरणः;
        दत्त = m.दत्त;
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
    copy_गन्तव्यम्(m);
    प्रत्यागम m;
}

शून्य Mat::नियोजय_गन्तव्यम्( Mat& m, पूर्णाङ्क _type ) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convert_गन्तव्यम्(m, _type);
}

शून्य Mat::create(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type)
{
    _type &= TYPE_आवरणम्;
    यदि( dims <= 2 && पङ्क्तिकाः == पङ्क्तिकाः_3 && स्तम्भसङ्ख्या == स्तम्भसङ्ख्या_3 && type() == _type && दत्त )
        प्रत्यागम;
    पूर्णाङ्क sz[] = {पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3};
    create(2, sz, _type);
}

शून्य Mat::create(आकार _sz, पूर्णाङ्क _type)
{
    create(_sz.height, _sz.विस्तारः, _type);
}

शून्य Mat::create_Same_आकार(निवेशः_Array m, पूर्णाङ्क type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

शून्य Mat::fit(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type)
{
    size_t old_योग_अष्टकानि = u ? u->size : 0;
    size_t esz = CV_ELEM_आकार(_type), new_योग = _dims >= 0;
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++)
        new_योग *= _sizes[i];
    size_t new_योग_अष्टकानि = new_योग*esz;
    यदि (new_योग_अष्टकानि > 0 && (!isContinuous() ||
                              new_योग_अष्टकानि > old_योग_अष्टकानि ||
                              दत्त != datastart)) {
        create(_dims, _sizes, _type);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_आवरणम्) | CV_MAT_TYPE(_type);
        पूर्णाङ्क dummy_आकार = 0;
        समुच्चयः_आकार(*एतत्, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_आकार), शून्यसूचकः, सत्य);
        finalizeHdr(*एतत्);
    }
}

शून्य Mat::fit(ध्रुव std::vector<पूर्णाङ्क>& _shape, पूर्णाङ्क _type)
{
    fit((पूर्णाङ्क)_shape.size(), _shape.दत्त(), _type);
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
    स्वचालित it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

शून्य Mat::fit(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type)
{
    _type &= TYPE_आवरणम्;
    पूर्णाङ्क sz[] = {पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3};
    fit(2, sz, _type);
}

शून्य Mat::fit(आकार _sz, पूर्णाङ्क _type)
{
    fit(_sz.height, _sz.विस्तारः, _type);
}

शून्य Mat::fit_Same_आकार(निवेशः_Array m, पूर्णाङ्क _type)
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
    datastart = dataend = datalimit = दत्त = 0;
    प्रति(पूर्णाङ्क i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_आवरणम्) | MAGIC_VAL;
    dims = पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
    size.clear();
}

size_t Mat::step1(पूर्णाङ्क i) ध्रुव
{
    प्रत्यागम चरणः.p[i] / elemSize1();
}

तार्किक Mat::empty() ध्रुव
{
    प्रत्यागम दत्त == 0 || योग() == 0;
}

size_t Mat::योग() ध्रुव
{
    यदि( dims <= 2 )
        प्रत्यागम (size_t)पङ्क्तिकाः * स्तम्भसङ्ख्या;
    size_t p = 1;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
        p *= size[i];
    प्रत्यागम p;
}

size_t Mat::योग(पूर्णाङ्क startDim, पूर्णाङ्क अन्तः_Dim) ध्रुव
{
    CV_Assert( 0 <= startDim && startDim <= अन्तः_Dim);
    size_t p = 1;
    पूर्णाङ्क अन्तः_Dim_2 = अन्तः_Dim <= dims ? अन्तः_Dim : dims;
    प्रति( पूर्णाङ्क i = startDim; i < अन्तः_Dim_2; i++ )
        p *= size[i];
    प्रत्यागम p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), पङ्क्तिकाः(m.पङ्क्तिकाः), स्तम्भसङ्ख्या(m.स्तम्भसङ्ख्या), दत्त(m.दत्त),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), चरणः(m.चरणः)
{
    m.flags = MAGIC_VAL; m.dims = m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = 0;
    m.size.clear(); m.चरणः.clear();
    m.दत्त = रिक्त; m.datastart = रिक्त; m.dataend = रिक्त; m.datalimit = रिक्त;
    m.allocator = रिक्त;
    m.u = रिक्त;
}


Mat& Mat::संक्रियकः = (Mat&& m)
{
    यदि (एतत् == &m)
      प्रत्यागम *एतत्;

    release();
    flags = m.flags; dims = m.dims; पङ्क्तिकाः = m.पङ्क्तिकाः; स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या; दत्त = m.दत्त;
    size = m.size; चरणः = m.चरणः;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = 0;
    m.size.clear(); m.चरणः.clear();
    m.दत्त = रिक्त; m.datastart = रिक्त; m.dataend = रिक्त; m.datalimit = रिक्त;
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

    यदि( दत्त && d == dims && _type == type() )
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
    flags = (_type & CV_MAT_TYPE_आवरणम्) | MAGIC_VAL;
    समुच्चयः_आकार(*एतत्, d, _sizes, 0, सत्य);

    यदि( योग() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        यदि(!a)
            a = a0;
        यत्न
        {
            u = a->allocate(dims, size.p, _type, 0, चरणः.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        गृहाण (...)
        {
            यदि (a == a0)
                क्षिप;
            u = a0->allocate(dims, size.p, _type, 0, चरणः.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( चरणः[dims-1] == (size_t)CV_ELEM_आकार(flags) );
    }

    addref();
    finalizeHdr(*एतत्);
    size.dims = dims = d0;
}

शून्य Mat::create(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type)
{
    create((पूर्णाङ्क)_sizes.size(), _sizes.दत्त(), _type);
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
    स्वचालित it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

शून्य Mat::copy_आकार(ध्रुव Mat& m)
{
    dims = m.dims;
    स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
    पङ्क्तिकाः = m.पङ्क्तिकाः;
    size = m.size;
    चरणः = m.चरणः;
}

शून्य Mat::deallocate()
{
    यदि(u)
    {
        UMat_दत्त* u_ = u;
        u = रिक्त;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ध्रुव Mat& m, ध्रुव Range& _rowRange, ध्रुव Range& _colRange)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.पङ्क्तिकाः))));
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
        यदि( _rowRange != Range::all() && _rowRange != Range(0,पङ्क्तिकाः) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.पङ्क्तिकाः );
            size[0] = पङ्क्तिकाः = _rowRange.size();
            दत्त += चरणः*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        यदि( _colRange != Range::all() && _colRange != Range(0,स्तम्भसङ्ख्या) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.स्तम्भसङ्ख्या );
            size[dims > 1] = स्तम्भसङ्ख्या = _colRange.size();
            दत्त += _colRange.start*elem_आकार();
            flags |= SUBMATRIX_FLAG;
        }
    }
    गृहाण(...)
    {
        release();
        क्षिप;
    }

    updateContinuityFlag();

    यदि( पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0 )
    {
        release();
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
    }
}


Mat::Mat(ध्रुव Mat& m, ध्रुव Rect& roi)
    : flags(m.flags), dims(2), पङ्क्तिकाः(roi.height), स्तम्भसङ्ख्या(roi.विस्तारः),
    दत्त(m.दत्त + roi.y*m.चरणः[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_आकार(flags);
    दत्त += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.विस्तारः && roi.x + roi.विस्तारः <= m.स्तम्भसङ्ख्या &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.पङ्क्तिकाः );
    यदि( roi.विस्तारः < m.स्तम्भसङ्ख्या || roi.height < m.पङ्क्तिकाः )
        flags |= SUBMATRIX_FLAG;

    size[0] = पङ्क्तिकाः;
    size[1] = स्तम्भसङ्ख्या;
    चरणः[0] = m.चरणः[0];
    चरणः[1] = esz;
    updateContinuityFlag();

    addref();
    यदि( पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0 )
    {
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
        release();
    }
}


Mat::Mat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type, शून्य* दत्त_3, ध्रुव size_t* चरणाः_2)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = दत्त = (uchar*)दत्त_3;
    यदि (_dims > 0 || दत्त_3 != शून्यसूचकः)
        समुच्चयः_आकार(*एतत्, _dims, _sizes, चरणाः_2, सत्य);
    finalizeHdr(*एतत्);
}


Mat::Mat(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type, शून्य* दत्त_3, ध्रुव size_t* चरणाः_2)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = दत्त = (uchar*)दत्त_3;
    पूर्णाङ्क _dims = (पूर्णाङ्क)_sizes.size();
    यदि (_dims > 0 || दत्त_3 != शून्यसूचकः)
        समुच्चयः_आकार(*एतत्, _dims, _sizes.दत्त(), चरणाः_2, सत्य);
    finalizeHdr(*एतत्);
}

Mat::Mat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, शून्य* दत्त_3, ध्रुव size_t* चरणाः_2)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = दत्त = (uchar*)दत्त_3;
    यदि (_shape.dims >= 0) {
        समुच्चयः_आकार(*एतत्, _shape.dims, _shape.p, चरणाः_2, सत्य);
    }
    अन्यथा {
        CV_Assert(!दत्त);
    }
    finalizeHdr(*एतत्);
}

Mat::Mat(std::initializer_list<पूर्णाङ्क> _shape, पूर्णाङ्क _type, शून्य* दत्त_3, ध्रुव size_t* चरणाः_2)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    पूर्णाङ्क new_shape[MatShape::MAX_DIMS];
    पूर्णाङ्क _dims = (पूर्णाङ्क)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    स्वचालित it = _shape.begin();
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = दत्त = (uchar*)दत्त_3;
    यदि (_dims > 0 || दत्त_3 != शून्यसूचकः)
        समुच्चयः_आकार(*एतत्, _dims, new_shape, चरणाः_2, सत्य);
    finalizeHdr(*एतत्);
}

Mat::Mat(ध्रुव Mat& m, ध्रुव Range* ranges)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
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
            दत्त += r.start*चरणः.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        पङ्क्तिकाः = d == 2 ? size[0] : 1;
        स्तम्भसङ्ख्या = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ध्रुव Mat& m, ध्रुव std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), दत्त(0), datastart(0), dataend(0),
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
            दत्त += r.start*चरणः.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        पङ्क्तिकाः = d == 2 ? size[0] : 1;
        स्तम्भसङ्ख्या = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(पूर्णाङ्क d) ध्रुव
{
    CV_Assert( dims == 2 );
    Mat m = *एतत्;
    size_t esz = elem_आकार();
    पूर्णाङ्क len;

    यदि( d >= 0 )
    {
        len = std::min(स्तम्भसङ्ख्या - d, पङ्क्तिकाः);
        m.दत्त += esz*d;
    }
    अन्यथा
    {
        len = std::min(पङ्क्तिकाः + d, स्तम्भसङ्ख्या);
        m.दत्त -= चरणः[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.पङ्क्तिकाः = len;
    m.size[1] = m.स्तम्भसङ्ख्या = 1;
    m.चरणः[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    यदि( size() != आकार(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    प्रत्यागम m;
}


शून्य Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    यदि( isSubmatrix() )
        *एतत् = rowRange(0, size.p[0] - (पूर्णाङ्क)nelems);
    अन्यथा
    {
        size.p[0] -= (पूर्णाङ्क)nelems;
        यदि (dims == 2)
            पङ्क्तिकाः = size.p[0];
        अन्यथा यदि (dims == 1)
            स्तम्भसङ्ख्या = size.p[0];
        dataend -= nelems*चरणः.p[0];
    }
}


शून्य Mat::प्रक्षिप_पृष्ठभागः(ध्रुव शून्य* elem)
{
    size_t r = size.p[0];
    यदि( isSubmatrix() || dataend + चरणः.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_आकार();
    स्मृतिं_प्रतिलिख(दत्त + r*चरणः.p[0], elem, esz);
    size.p[0] = पूर्णाङ्क(r + 1);
    यदि (dims == 2)
        पङ्क्तिकाः = size.p[0];
    अन्यथा यदि (dims == 1)
        स्तम्भसङ्ख्या = size.p[0];
    dataend += चरणः.p[0];
    uint64 tsz = size.p[0];
    प्रति( पूर्णाङ्क i = 1; i < dims; i++ )
        tsz *= size.p[i];
    यदि( esz < चरणः.p[0] || tsz != (uint64)(पूर्णाङ्क)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


शून्य Mat::reserve(size_t nelems)
{
    ध्रुव size_t MIN_आकार = 64;

    CV_Assert( (पूर्णाङ्क)nelems >= 0 );
    यदि( !isSubmatrix() && चरणः.p[0] != 0 && दत्त + चरणः.p[0]*nelems <= datalimit )
        प्रत्यागम;

    पूर्णाङ्क r = size.p[0];
    यदि( (size_t)r >= nelems)
        प्रत्यागम;

    MatShape newsize = size;
    size_t esz = elem_आकार();
    newsize.dims = std::max(newsize.dims, 1);
    यदि (dims > 1 && r == 0 && चरणः.p[0] == 0) {
        चरणः.p[0] = size.p[1] * (dims == 2 ? esz : चरणः.p[1]);
    }
    newsize.p[0] = (पूर्णाङ्क)nelems;
    size_t newbytes = newsize.योग() * esz;
    यदि (newbytes < MIN_आकार) {
        newsize.p[0] = 1;
        newsize.p[0] = पूर्णाङ्क(MIN_आकार / newsize.योग() / esz);
    }

    Mat m(newsize, type());
    यदि( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copy_गन्तव्यम्(mpart);
    }

    *एतत् = m;
    size.p[0] = r;

    यदि (dims == 2)
        पङ्क्तिकाः = size.p[0];
    अन्यथा यदि (dims == 1)
        स्तम्भसङ्ख्या = size.p[0];
    dataend = दत्त + चरणः.p[0]*r;
}


शून्य Mat::reserve_बफर(size_t nbytes)
{
    size_t esz = 1;
    पूर्णाङ्क mtype = CV_8UC1;
    यदि (!empty())
    {
        यदि (!isSubmatrix() && दत्त + nbytes <= dataend)//Should it be datalimit?
            प्रत्यागम;
        esz = elem_आकार();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#यदि SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(अधिकतम_पूर्णाङ्कः)*size_t(अधिकतम_पूर्णाङ्कः));
    पूर्णाङ्क newrows = nelems > size_t(अधिकतम_पूर्णाङ्कः) ? nelems > 0x400*size_t(अधिकतम_पूर्णाङ्कः) ? nelems > 0x100000 * size_t(अधिकतम_पूर्णाङ्कः) ? nelems > 0x40000000 * size_t(अधिकतम_पूर्णाङ्कः) ?
                  size_t(अधिकतम_पूर्णाङ्कः) : 0x40000000 : 0x100000 : 0x400 : 1;
#अन्यथा
    पूर्णाङ्क newrows = nelems > size_t(अधिकतम_पूर्णाङ्कः) ? 2 : 1;
#यदि_अन्तः
    पूर्णाङ्क newcols = (पूर्णाङ्क)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


शून्य Mat::resize(size_t nelems)
{
    पूर्णाङ्क r = size.p[0];
    यदि( r == (पूर्णाङ्क)nelems )
        प्रत्यागम;
    CV_Assert( (पूर्णाङ्क)nelems >= 0 );

    यदि( isSubmatrix() || दत्त + चरणः.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (पूर्णाङ्क)nelems;
    यदि (dims == 2)
        पङ्क्तिकाः = size.p[0];
    अन्यथा यदि (dims == 1)
        स्तम्भसङ्ख्या = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)चरणः.p[0];

    //updateContinuityFlag(*this);
}


शून्य Mat::resize(size_t nelems, ध्रुव Scalar& s)
{
    पूर्णाङ्क save_पङ्क्तिकाः = size.p[0];
    resize(nelems);

    यदि( size.p[0] > save_पङ्क्तिकाः )
    {
        Mat part = rowRange(save_पङ्क्तिकाः, size.p[0]);
        part = s;
    }
}

शून्य Mat::push_back(ध्रुव Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    यदि( delta == 0 )
        प्रत्यागम;
    यदि( एतत् == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        प्रत्यागम;
    }
    यदि( !दत्त )
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
    size_t esz = elem_आकार();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : चरणः.p[1]);
    size_t step0 = चरणः.p[0];
    यदि (step0 < minstep) {
        यदि (size.p[0] <= 1)
            चरणः.p[0] = step0 = minstep;
    }

    यदि( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += पूर्णाङ्क(delta);
    यदि (dims == 2)
        पङ्क्तिकाः = size.p[0];
    अन्यथा यदि (dims == 1)
        स्तम्भसङ्ख्या = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    यदि( isContinuous() && elems.isContinuous() )
        स्मृतिं_प्रतिलिख(दत्त + r*step0, elems.दत्त, elems.योग()*elems.elem_आकार());
    अन्यथा
    {
        Mat part = rowRange(पूर्णाङ्क(r), पूर्णाङ्क(r + delta));
        elems.copy_गन्तव्यम्(part);
    }
}


शून्य Mat::locateROI( आकार& सम्पूर्णम्_आकार, Point& ofs ) ध्रुव
{
    CV_Assert( dims <= 2 && चरणः[0] > 0 );
    size_t esz = elem_आकार(), minstep;
    ptrdiff_t delta1 = दत्त - datastart, delta2 = dataend - datastart;

    यदि( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णाङ्क)(delta1/चरणः[0]);
        ofs.x = (पूर्णाङ्क)((delta1 - चरणः[0]*ofs.y)/esz);
        CV_DbgAssert( दत्त == datastart + ofs.y*चरणः[0] + ofs.x*esz );
    }
    minstep = (ofs.x + स्तम्भसङ्ख्या)*esz;
    सम्पूर्णम्_आकार.height = (पूर्णाङ्क)((delta2 - minstep)/चरणः[0] + 1);
    सम्पूर्णम्_आकार.height = std::max(सम्पूर्णम्_आकार.height, ofs.y + पङ्क्तिकाः);
    सम्पूर्णम्_आकार.विस्तारः = (पूर्णाङ्क)((delta2 - चरणः*(सम्पूर्णम्_आकार.height-1))/esz);
    सम्पूर्णम्_आकार.विस्तारः = std::max(सम्पूर्णम्_आकार.विस्तारः, ofs.x + स्तम्भसङ्ख्या);
}

Mat& Mat::adjustROI( पूर्णाङ्क dtop, पूर्णाङ्क dbottom, पूर्णाङ्क dleft, पूर्णाङ्क dright )
{
    CV_Assert( dims <= 2 && चरणः[0] > 0 );
    आकार सम्पूर्णम्_आकार; Point ofs;
    size_t esz = elem_आकार();
    locateROI( सम्पूर्णम्_आकार, ofs );
    पूर्णाङ्क row1 = std::min(std::max(ofs.y - dtop, 0), सम्पूर्णम्_आकार.height), row2 = std::max(0, std::min(ofs.y + पङ्क्तिकाः + dbottom, सम्पूर्णम्_आकार.height));
    पूर्णाङ्क col1 = std::min(std::max(ofs.x - dleft, 0), सम्पूर्णम्_आकार.विस्तारः), col2 = std::max(0, std::min(ofs.x + स्तम्भसङ्ख्या + dright, सम्पूर्णम्_आकार.विस्तारः));
    यदि(row1 > row2)
        std::swap(row1, row2);
    यदि(col1 > col2)
        std::swap(col1, col2);

    यदि (dims == 1) {
        दत्त += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        स्तम्भसङ्ख्या = col2 - col1;
        size.p[0] = स्तम्भसङ्ख्या;
    } अन्यथा {
        दत्त += (row1 - ofs.y)*(std::ptrdiff_t)चरणः + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        पङ्क्तिकाः = row2 - row1; स्तम्भसङ्ख्या = col2 - col1;
        size.p[0] = पङ्क्तिकाः; size.p[1] = स्तम्भसङ्ख्या;
        updateContinuityFlag();
    }
    प्रत्यागम *एतत्;
}

Mat Mat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_पङ्क्तिकाः) ध्रुव
{
    पूर्णाङ्क cn = channels();
    Mat hdr = *एतत्;

    यदि( new_cn == 0 )
        new_cn = cn;

    यदि( dims > 2 )
    {
        यदि( new_पङ्क्तिकाः == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn-1) << CV_CN_सरणम्);
            hdr.चरणः[dims-1] = CV_ELEM_आकार(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            प्रत्यागम hdr;
        }
        CV_Assert( new_पङ्क्तिकाः > 0 );
        पूर्णाङ्क sz[] = { new_पङ्क्तिकाः, (पूर्णाङ्क)(योग()*cn/new_पङ्क्तिकाः) };
        प्रत्यागम reshape(new_cn, 2, sz);
    }

    पूर्णाङ्क योग_विस्तारः = स्तम्भसङ्ख्या * cn;

    यदि( (new_cn > योग_विस्तारः || योग_विस्तारः % new_cn != 0) && new_पङ्क्तिकाः == 0 )
        new_पङ्क्तिकाः = पङ्क्तिकाः * योग_विस्तारः / new_cn;

    यदि( new_पङ्क्तिकाः != 0 && new_पङ्क्तिकाः != पङ्क्तिकाः )
    {
        पूर्णाङ्क योग_आकार_2 = योग_विस्तारः * पङ्क्तिकाः;
        यदि( !isContinuous() )
            CV_Error( cv::Error::Bad_चरणः,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        योग_विस्तारः = योग_आकार_2 / new_पङ्क्तिकाः;

        यदि( योग_विस्तारः * new_पङ्क्तिकाः != योग_आकार_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.पङ्क्तिकाः = new_पङ्क्तिकाः;
        hdr.चरणः[0] = योग_विस्तारः * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.पङ्क्तिकाः = पङ्क्तिकाः;
        यदि (dims <= 1)
            hdr.चरणः[0] = स्तम्भसङ्ख्या * CV_ELEM_आकार(flags);
    }

    पूर्णाङ्क new_विस्तारः = योग_विस्तारः / new_cn;

    यदि( new_विस्तारः * new_cn != योग_विस्तारः )
        CV_Error( cv::Error::Bad_अङ्कः_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.स्तम्भसङ्ख्या = new_विस्तारः;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn-1) << CV_CN_सरणम्);
    hdr.चरणः[1] = CV_ELEM_आकार(hdr.flags);
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

        size_t योग_elem1_ref = एतत्->योग() * एतत्->channels();
        size_t योग_elem1 = _cn;

        Auto_बफर<पूर्णाङ्क, 4> newsz_buf( (size_t)_newndims );

        पूर्णाङ्क m1_idx = -1;

        प्रति (पूर्णाङ्क i = 0; i < _newndims; i++)
        {
            यदि (_newsz[i] >= 0) {
                यदि (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                योग_elem1 *= (size_t)newsz_buf[i];
            } अन्यथा {
                यदि (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_आकार, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        यदि (m1_idx >= 0) {
            यदि (योग_elem1 == 0) {
                CV_Assert(योग_elem1_ref == 0);
                योग_elem1 = 1;
            }
            CV_Assert(योग_elem1_ref % योग_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णाङ्क)(योग_elem1_ref / योग_elem1);
        } अन्यथा यदि (योग_elem1 != योग_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *एतत्;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((_cn-1) << CV_CN_सरणम्);
        समुच्चयः_आकार(hdr, _newndims, newsz_buf.दत्त(), रिक्त, सत्य);

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
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    स्वचालित it = newshape_.begin();
    प्रति (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    प्रत्यागम reshape(_cn, (पूर्णाङ्क)newshape_dims, newshape);
}

Mat Mat::reinterpret(पूर्णाङ्क type) ध्रुव
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_आकार(एतत्->type()) == CV_ELEM_आकार(type));
    Mat m = *एतत्;
    m.flags = (m.flags & ~CV_MAT_TYPE_आवरणम्) | type;
    m.updateContinuityFlag();
    प्रत्यागम m;
}

Mat Mat::diag(ध्रुव Mat& d)
{
    CV_Assert( d.स्तम्भसङ्ख्या == 1 || d.पङ्क्तिकाः == 1 );
    पूर्णाङ्क len = d.पङ्क्तिकाः + d.स्तम्भसङ्ख्या - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    यदि( d.स्तम्भसङ्ख्या == 1 )
        d.copy_गन्तव्यम्(md);
    अन्यथा
        transpose(d, md);
    प्रत्यागम m;
}

पूर्णाङ्क Mat::check_सदिशम्(पूर्णाङ्क _elemChannels, पूर्णाङ्क गभीरता_2, तार्किक _requireContinuous) ध्रुव
{
    प्रत्यागम दत्त && (गभीरता() == गभीरता_2 || गभीरता_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((पङ्क्तिकाः == 1 || स्तम्भसङ्ख्या == 1) && channels() == _elemChannels) ||
                        (स्तम्भसङ्ख्या == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || चरणः.p[1] == चरणः.p[2]*size.p[2])))
    ? (पूर्णाङ्क)(योग()*channels()/_elemChannels) : -1;
}


स्थिर अन्तःस्थ आकार get_Continuous_आकार(पूर्णाङ्क flags, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क विस्तारः_Scale)
{
    int64 sz = (int64)स्तम्भसङ्ख्या * पङ्क्तिकाः * विस्तारः_Scale;
    तार्किक has_int_अतिप्रवाहः = sz >= अधिकतम_पूर्णाङ्कः;
    तार्किक isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    प्रत्यागम (isContiguous && !has_int_अतिप्रवाहः)
            ? आकार((पूर्णाङ्क)sz, 1)
            : आकार(स्तम्भसङ्ख्या * विस्तारः_Scale, पङ्क्तिकाः);
}

आकार getContinuousSize2D(Mat& m1, पूर्णाङ्क विस्तारः_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    प्रत्यागम get_Continuous_आकार(m1.flags,
                              m1.स्तम्भसङ्ख्या, m1.पङ्क्तिकाः, विस्तारः_Scale);
}
आकार getContinuousSize2D(Mat& m1, Mat& m2, पूर्णाङ्क विस्तारः_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ध्रुव आकार sz1 = m1.size();
    यदि (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t योग_sz = m1.योग();
        CV_CheckEQ(योग_sz, m2.योग(), "");
        तार्किक is_m1_सदिशम् = m1.स्तम्भसङ्ख्या == 1 || m1.पङ्क्तिकाः == 1;
        तार्किक is_m2_सदिशम् = m2.स्तम्भसङ्ख्या == 1 || m2.पङ्क्तिकाः == 1;
        CV_Assert(is_m1_सदिशम्); CV_Assert(is_m2_सदिशम्);
        पूर्णाङ्क योग = (पूर्णाङ्क)योग_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_अतिप्रवाहः = ((int64)योग_sz * विस्तारः_Scale) >= अधिकतम_पूर्णाङ्कः;
        यदि (isContiguous && !has_int_अतिप्रवाहः)
            योग = 1; // vector-row
        m1 = m1.reshape(0, योग);
        m2 = m2.reshape(0, योग);
        CV_Assert(m1.स्तम्भसङ्ख्या == m2.स्तम्भसङ्ख्या && m1.पङ्क्तिकाः == m2.पङ्क्तिकाः);
        प्रत्यागम आकार(m1.स्तम्भसङ्ख्या * विस्तारः_Scale, m1.पङ्क्तिकाः);
    }
    प्रत्यागम get_Continuous_आकार(m1.flags & m2.flags,
                              m1.स्तम्भसङ्ख्या, m1.पङ्क्तिकाः, विस्तारः_Scale);
}

आकार getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णाङ्क विस्तारः_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ध्रुव आकार sz1 = m1.size();
    यदि (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t योग_sz = m1.योग();
        CV_CheckEQ(योग_sz, m2.योग(), "");
        CV_CheckEQ(योग_sz, m3.योग(), "");
        तार्किक is_m1_सदिशम् = m1.स्तम्भसङ्ख्या == 1 || m1.पङ्क्तिकाः == 1;
        तार्किक is_m2_सदिशम् = m2.स्तम्भसङ्ख्या == 1 || m2.पङ्क्तिकाः == 1;
        तार्किक is_m3_सदिशम् = m3.स्तम्भसङ्ख्या == 1 || m3.पङ्क्तिकाः == 1;
        CV_Assert(is_m1_सदिशम्); CV_Assert(is_m2_सदिशम्); CV_Assert(is_m3_सदिशम्);
        पूर्णाङ्क योग = (पूर्णाङ्क)योग_sz;  // vector-column
        तार्किक isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        तार्किक has_int_अतिप्रवाहः = ((int64)योग_sz * विस्तारः_Scale) >= अधिकतम_पूर्णाङ्कः;
        यदि (isContiguous && !has_int_अतिप्रवाहः)
            योग = 1; // vector-row
        m1 = m1.reshape(0, योग);
        m2 = m2.reshape(0, योग);
        m3 = m3.reshape(0, योग);
        CV_Assert(m1.स्तम्भसङ्ख्या == m2.स्तम्भसङ्ख्या && m1.पङ्क्तिकाः == m2.पङ्क्तिकाः && m1.स्तम्भसङ्ख्या == m3.स्तम्भसङ्ख्या && m1.पङ्क्तिकाः == m3.पङ्क्तिकाः);
        प्रत्यागम आकार(m1.स्तम्भसङ्ख्या * विस्तारः_Scale, m1.पङ्क्तिकाः);
    }
    प्रत्यागम get_Continuous_आकार(m1.flags & m2.flags & m3.flags,
                              m1.स्तम्भसङ्ख्या, m1.पङ्क्तिकाः, विस्तारः_Scale);
}

} // cv::
