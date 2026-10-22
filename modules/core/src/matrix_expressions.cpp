// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

நாமாகாஶ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
நிலையான வெறுமை checkOperandsExist(மாறிலி Mat& a)
{
    எனில் (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

நிலையான வெறுமை checkOperandsExist(மாறிலி Mat& a, மாறிலி Mat& b)
{
    எனில் (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


வகுப்பு MatOp_Identity CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Identity() {}
    மெய்நிகர் ~MatOp_Identity() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு உண்மை; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, மாறிலி Mat& m);
};

நிலையான MatOp_Identity g_MatOp_Identity;

வகுப்பு MatOp_AddEx CV_FINAL : பொது MatOp
{
பொது:
    MatOp_AddEx() {}
    மெய்நிகர் ~MatOp_AddEx() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு உண்மை; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை add(மாறிலி Mat_கோவை& e1, மாறிலி Scalar& s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை subtract(மாறிலி Scalar& s, மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை multiply(மாறிலி Mat_கோவை& e1, இரட்டை s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை divide(இரட்டை s, மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    வெறுமை transpose(மாறிலி Mat_கோவை& e1, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை abs(மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, மாறிலி Mat& a, மாறிலி Mat& b, இரட்டை alpha, இரட்டை beta, மாறிலி Scalar& s=Scalar());
};

நிலையான MatOp_AddEx g_MatOp_AddEx;

வகுப்பு MatOp_Bin CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Bin() {}
    மெய்நிகர் ~MatOp_Bin() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு உண்மை; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை multiply(மாறிலி Mat_கோவை& e1, இரட்டை s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை divide(இரட்டை s, மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, எழுத்து op, மாறிலி Mat& a, மாறிலி Mat& b, இரட்டை scale=1);
    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, எழுத்து op, மாறிலி Mat& a, மாறிலி Scalar& s);
};

நிலையான MatOp_Bin g_MatOp_Bin;

வகுப்பு MatOp_Cmp CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Cmp() {}
    மெய்நிகர் ~MatOp_Cmp() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு உண்மை; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு cmpop, மாறிலி Mat& a, மாறிலி Mat& b);
    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு cmpop, மாறிலி Mat& a, இரட்டை alpha);
};

நிலையான MatOp_Cmp g_MatOp_Cmp;

வகுப்பு MatOp_GEMM CV_FINAL : பொது MatOp
{
பொது:
    MatOp_GEMM() {}
    மெய்நிகர் ~MatOp_GEMM() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு பொய்; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை add(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை subtract(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    வெறுமை transpose(மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    Size size(மாறிலி Mat_கோவை& கோவை) மாறிலி CV_OVERRIDE
    {
        திரும்பு Size(
            (கோவை.flags & GEMM_2_T) ? கோவை.b.rows : கோவை.b.cols,
            (கோவை.flags & GEMM_1_T) ? கோவை.a.cols : கோவை.a.rows
        );
    }

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு flags, மாறிலி Mat& a, மாறிலி Mat& b,
                         இரட்டை alpha=1, மாறிலி Mat& c=Mat(), இரட்டை beta=1);

};

நிலையான MatOp_GEMM g_MatOp_GEMM;

வகுப்பு MatOp_Invert CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Invert() {}
    மெய்நிகர் ~MatOp_Invert() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு பொய்; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை matmul(மாறிலி Mat_கோவை& expr1, மாறிலி Mat_கோவை& expr2, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு method, மாறிலி Mat& m);
};

நிலையான MatOp_Invert g_MatOp_Invert;

வகுப்பு MatOp_T CV_FINAL : பொது MatOp
{
பொது:
    MatOp_T() {}
    மெய்நிகர் ~MatOp_T() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு பொய்; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை multiply(மாறிலி Mat_கோவை& e1, இரட்டை s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;
    வெறுமை transpose(மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, மாறிலி Mat& a, இரட்டை alpha=1);
};

நிலையான MatOp_T g_MatOp_T;

வகுப்பு MatOp_Solve CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Solve() {}
    மெய்நிகர் ~MatOp_Solve() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு பொய்; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு method, மாறிலி Mat& a, மாறிலி Mat& b);
};

நிலையான MatOp_Solve g_MatOp_Solve;

வகுப்பு MatOp_Initializer CV_FINAL : பொது MatOp
{
பொது:
    MatOp_Initializer() {}
    மெய்நிகர் ~MatOp_Initializer() {}

    தருக்க elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி CV_OVERRIDE { திரும்பு பொய்; }
    வெறுமை assign(மாறிலி Mat_கோவை& கோவை, Mat& m, முழு type=-1) மாறிலி CV_OVERRIDE;

    வெறுமை multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி CV_OVERRIDE;

    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு method, Size sz, முழு type, இரட்டை alpha=1);
    நிலையான வெறுமை make_கோவை(Mat_கோவை& res, முழு method, முழு ndims, மாறிலி முழு* sizes, முழு type, இரட்டை alpha=1);
};

நிலையான MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, புதிய MatOp_Initializer())
}

நிலையான உள்வரி தருக்க isIdentity(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_Identity; }
நிலையான உள்வரி தருக்க isAddEx(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_AddEx; }
நிலையான உள்வரி தருக்க isScaled(மாறிலி Mat_கோவை& e) { திரும்பு isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
நிலையான உள்வரி தருக்க isBin(மாறிலி Mat_கோவை& e, எழுத்து c) { திரும்பு e.op == &g_MatOp_Bin && e.flags == c; }
நிலையான உள்வரி தருக்க isCmp(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_Cmp; }
நிலையான உள்வரி தருக்க isReciprocal(மாறிலி Mat_கோவை& e) { திரும்பு isBin(e,'/') && (!e.b.data || e.beta == 0); }
நிலையான உள்வரி தருக்க isT(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_T; }
நிலையான உள்வரி தருக்க isInv(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_Invert; }
நிலையான உள்வரி தருக்க isSolve(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
நிலையான உள்வரி தருக்க isMatProd(மாறிலி Mat_கோவை& e) { திரும்பு e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
நிலையான உள்வரி தருக்க isInitializer(மாறிலி Mat_கோவை& e) { திரும்பு e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


தருக்க MatOp::elementWise(மாறிலி Mat_கோவை& /*கோவை*/) மாறிலி
{
    திரும்பு பொய்;
}

வெறுமை MatOp::roi(மாறிலி Mat_கோவை& கோவை, மாறிலி Range& rowRange, மாறிலி Range& colRange, Mat_கோவை& e) மாறிலி
{
    எனில்( elementWise(கோவை) )
    {
        e = Mat_கோவை(கோவை.op, கோவை.flags, Mat(), Mat(), Mat(),
                    கோவை.alpha, கோவை.beta, கோவை.s);
        எனில்(கோவை.a.data)
            e.a = கோவை.a(rowRange, colRange);
        எனில்(கோவை.b.data)
            e.b = கோவை.b(rowRange, colRange);
        எனில்(கோவை.c.data)
            e.c = கோவை.c(rowRange, colRange);
    }
    இல்லையெனில்
    {
        Mat m;
        கோவை.op->assign(கோவை, m);
        e = Mat_கோவை(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

வெறுமை MatOp::diag(மாறிலி Mat_கோவை& கோவை, முழு d, Mat_கோவை& e) மாறிலி
{
    எனில்( elementWise(கோவை) )
    {
        e = Mat_கோவை(கோவை.op, கோவை.flags, Mat(), Mat(), Mat(),
                    கோவை.alpha, கோவை.beta, கோவை.s);
        எனில்(கோவை.a.data)
            e.a = கோவை.a.diag(d);
        எனில்(கோவை.b.data)
            e.b = கோவை.b.diag(d);
        எனில்(கோவை.c.data)
            e.c = கோவை.c.diag(d);
    }
    இல்லையெனில்
    {
        Mat m;
        கோவை.op->assign(கோவை, m);
        e = Mat_கோவை(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


வெறுமை MatOp::augAssignAdd(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m += temp;
}


வெறுமை MatOp::augAssignSubtract(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m -= temp;
}


வெறுமை MatOp::augAssignMultiply(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m *= temp;
}


வெறுமை MatOp::augAssignDivide(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m /= temp;
}


வெறுமை MatOp::augAssignAnd(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m &= temp;
}


வெறுமை MatOp::augAssignOr(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m |= temp;
}


வெறுமை MatOp::augAssignXor(மாறிலி Mat_கோவை& கோவை, Mat& m) மாறிலி
{
    Mat temp;
    கோவை.op->assign(கோவை, temp);
    m ^= temp;
}


வெறுமை MatOp::add(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( இது == e2.op )
    {
        இரட்டை alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        எனில்( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        இல்லையெனில்
            e1.op->assign(e1, m1);

        எனில்( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        இல்லையெனில்
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_கோவை(res, m1, m2, alpha, beta, s);
    }
    இல்லையெனில்
        e2.op->add(e1, e2, res);
}


வெறுமை MatOp::add(மாறிலி Mat_கோவை& expr1, மாறிலி Scalar& s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_கோவை(res, m1, Mat(), 1, 0, s);
}


வெறுமை MatOp::subtract(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( இது == e2.op )
    {
        இரட்டை alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        எனில்( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        இல்லையெனில்
            e1.op->assign(e1, m1);

        எனில்( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        இல்லையெனில்
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_கோவை(res, m1, m2, alpha, beta, s);
    }
    இல்லையெனில்
        e2.op->subtract(e1, e2, res);
}


வெறுமை MatOp::subtract(மாறிலி Scalar& s, மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_AddEx::make_கோவை(res, m, Mat(), -1, 0, s);
}


வெறுமை MatOp::multiply(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res, இரட்டை scale) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( இது == e2.op )
    {
        Mat m1, m2;

        எனில்( isReciprocal(e1) )
        {
            எனில்( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            இல்லையெனில்
                e2.op->assign(e2, m2);

            MatOp_Bin::make_கோவை(res, '/', m2, e1.a, scale/e1.alpha);
        }
        இல்லையெனில்
        {
            எழுத்து op = '*';
            எனில்( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            இல்லையெனில்
                e1.op->assign(e1, m1);

            எனில்( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            இல்லையெனில் எனில்( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            இல்லையெனில்
                e2.op->assign(e2, m2);

            MatOp_Bin::make_கோவை(res, op, m1, m2, scale);
        }
    }
    இல்லையெனில்
        e2.op->multiply(e1, e2, res, scale);
}


வெறுமை MatOp::multiply(மாறிலி Mat_கோவை& கோவை, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_AddEx::make_கோவை(res, m, Mat(), s, 0);
}


வெறுமை MatOp::divide(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res, இரட்டை scale) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( இது == e2.op )
    {
        எனில்( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_கோவை(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        இல்லையெனில்
        {
            Mat m1, m2;
            எழுத்து op = '/';

            எனில்( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            இல்லையெனில்
                e1.op->assign(e1, m1);

            எனில்( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            இல்லையெனில் எனில்( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            இல்லையெனில்
                e2.op->assign(e2, m2);
            MatOp_Bin::make_கோவை(res, op, m1, m2, scale);
        }
    }
    இல்லையெனில்
        e2.op->divide(e1, e2, res, scale);
}


வெறுமை MatOp::divide(இரட்டை s, மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_Bin::make_கோவை(res, '/', m, Mat(), s);
}


வெறுமை MatOp::abs(மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_Bin::make_கோவை(res, 'a', m, Mat());
}


வெறுமை MatOp::transpose(மாறிலி Mat_கோவை& கோவை, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_T::make_கோவை(res, m, 1);
}


வெறுமை MatOp::matmul(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    எனில்( இது == e2.op )
    {
        இரட்டை scale = 1;
        முழு flags = 0;
        Mat m1, m2;

        எனில்( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        இல்லையெனில் எனில்( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        இல்லையெனில்
            e1.op->assign(e1, m1);

        எனில்( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        இல்லையெனில் எனில்( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        இல்லையெனில்
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_கோவை(res, flags, m1, m2, scale);
    }
    இல்லையெனில்
        e2.op->matmul(e1, e2, res);
}


வெறுமை MatOp::invert(மாறிலி Mat_கோவை& கோவை, முழு method, Mat_கோவை& res) மாறிலி
{
    Mat m;
    கோவை.op->assign(கோவை, m);
    MatOp_Invert::make_கோவை(res, method, m);
}


Size MatOp::size(மாறிலி Mat_கோவை& கோவை) மாறிலி
{
    திரும்பு !கோவை.a.empty() ? கோவை.a.size() : கோவை.b.empty() ? கோவை.b.size() : கோவை.c.size();
}

முழு MatOp::type(மாறிலி Mat_கோவை& கோவை) மாறிலி
{
    CV_INSTRUMENT_REGION();

    திரும்பு !கோவை.a.empty() ? கோவை.a.type() : கோவை.b.empty() ? கோவை.b.type() : கோவை.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_கோவை::Mat_கோவை(மாறிலி Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_கோவை Mat_கோவை::row(முழு y) மாறிலி
{
    Mat_கோவை e;
    op->roi(*இது, Range(y, y+1), Range::all(), e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::col(முழு x) மாறிலி
{
    Mat_கோவை e;
    op->roi(*இது, Range::all(), Range(x, x+1), e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::diag(முழு d) மாறிலி
{
    Mat_கோவை e;
    op->diag(*இது, d, e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::operator()( மாறிலி Range& rowRange, மாறிலி Range& colRange ) மாறிலி
{
    Mat_கோவை e;
    op->roi(*இது, rowRange, colRange, e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::operator()( மாறிலி Rect& roi ) மாறிலி
{
    Mat_கோவை e;
    op->roi(*இது, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    திரும்பு e;
}

Mat Mat_கோவை::cross(மாறிலி Mat& m) மாறிலி
{
    திரும்பு ((Mat)*இது).cross(m);
}

இரட்டை Mat_கோவை::dot(மாறிலி Mat& m) மாறிலி
{
    திரும்பு ((Mat)*இது).dot(m);
}

Mat_கோவை Mat_கோவை::t() மாறிலி
{
    Mat_கோவை e;
    op->transpose(*இது, e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::inv(முழு method) மாறிலி
{
    Mat_கோவை e;
    op->invert(*இது, method, e);
    திரும்பு e;
}

Mat_கோவை Mat_கோவை::mul(மாறிலி Mat_கோவை& e, இரட்டை scale) மாறிலி
{
    Mat_கோவை en;
    op->multiply(*இது, e, en, scale);
    திரும்பு en;
}

Mat_கோவை Mat_கோவை::mul(மாறிலி Mat& m, இரட்டை scale) மாறிலி
{
    Mat_கோவை e;
    op->multiply(*இது, Mat_கோவை(m), e, scale);
    திரும்பு e;
}

Mat_கோவை operator + (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, b, 1, 1);
    திரும்பு e;
}

Mat_கோவை operator + (மாறிலி Mat& a, மாறிலி Scalar& s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), 1, 0, s);
    திரும்பு e;
}

Mat_கோவை operator + (மாறிலி Scalar& s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), 1, 0, s);
    திரும்பு e;
}

Mat_கோவை operator + (மாறிலி Mat_கோவை& e, மாறிலி Mat& m)
{
    Mat_கோவை en;
    e.op->add(e, Mat_கோவை(m), en);
    திரும்பு en;
}

Mat_கோவை operator + (மாறிலி Mat& m, மாறிலி Mat_கோவை& e)
{
    checkOperandsExist(m);
    Mat_கோவை en;
    e.op->add(e, Mat_கோவை(m), en);
    திரும்பு en;
}

Mat_கோவை operator + (மாறிலி Mat_கோவை& e, மாறிலி Scalar& s)
{
    Mat_கோவை en;
    e.op->add(e, s, en);
    திரும்பு en;
}

Mat_கோவை operator + (மாறிலி Scalar& s, மாறிலி Mat_கோவை& e)
{
    Mat_கோவை en;
    e.op->add(e, s, en);
    திரும்பு en;
}

Mat_கோவை operator + (மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2)
{
    Mat_கோவை en;
    e1.op->add(e1, e2, en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, b, 1, -1);
    திரும்பு e;
}

Mat_கோவை operator - (மாறிலி Mat& a, மாறிலி Scalar& s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), 1, 0, -s);
    திரும்பு e;
}

Mat_கோவை operator - (மாறிலி Scalar& s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), -1, 0, s);
    திரும்பு e;
}

Mat_கோவை operator - (மாறிலி Mat_கோவை& e, மாறிலி Mat& m)
{
    checkOperandsExist(m);
    Mat_கோவை en;
    e.op->subtract(e, Mat_கோவை(m), en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Mat& m, மாறிலி Mat_கோவை& e)
{
    checkOperandsExist(m);
    Mat_கோவை en;
    e.op->subtract(Mat_கோவை(m), e, en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Mat_கோவை& e, மாறிலி Scalar& s)
{
    Mat_கோவை en;
    e.op->add(e, -s, en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Scalar& s, மாறிலி Mat_கோவை& e)
{
    Mat_கோவை en;
    e.op->subtract(s, e, en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2)
{
    Mat_கோவை en;
    e1.op->subtract(e1, e2, en);
    திரும்பு en;
}

Mat_கோவை operator - (மாறிலி Mat& m)
{
    checkOperandsExist(m);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, m, Mat(), -1, 0);
    திரும்பு e;
}

Mat_கோவை operator - (மாறிலி Mat_கோவை& e)
{
    Mat_கோவை en;
    e.op->subtract(Scalar(0), e, en);
    திரும்பு en;
}

Mat_கோவை operator * (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_GEMM::make_கோவை(e, 0, a, b);
    திரும்பு e;
}

Mat_கோவை operator * (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), s, 0);
    திரும்பு e;
}

Mat_கோவை operator * (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), s, 0);
    திரும்பு e;
}

Mat_கோவை operator * (மாறிலி Mat_கோவை& e, மாறிலி Mat& m)
{
    Mat_கோவை en;
    e.op->matmul(e, Mat_கோவை(m), en);
    திரும்பு en;
}

Mat_கோவை operator * (மாறிலி Mat& m, மாறிலி Mat_கோவை& e)
{
    checkOperandsExist(m);
    Mat_கோவை en;
    e.op->matmul(Mat_கோவை(m), e, en);
    திரும்பு en;
}

Mat_கோவை operator * (மாறிலி Mat_கோவை& e, இரட்டை s)
{
    Mat_கோவை en;
    e.op->multiply(e, s, en);
    திரும்பு en;
}

Mat_கோவை operator * (இரட்டை s, மாறிலி Mat_கோவை& e)
{
    Mat_கோவை en;
    e.op->multiply(e, s, en);
    திரும்பு en;
}

Mat_கோவை operator * (மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2)
{
    Mat_கோவை en;
    e1.op->matmul(e1, e2, en);
    திரும்பு en;
}

Mat_கோவை operator / (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '/', a, b);
    திரும்பு e;
}

Mat_கோவை operator / (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_AddEx::make_கோவை(e, a, Mat(), 1./s, 0);
    திரும்பு e;
}

Mat_கோவை operator / (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '/', a, Mat(), s);
    திரும்பு e;
}

Mat_கோவை operator / (மாறிலி Mat_கோவை& e, மாறிலி Mat& m)
{
    Mat_கோவை en;
    e.op->divide(e, Mat_கோவை(m), en);
    திரும்பு en;
}

Mat_கோவை operator / (மாறிலி Mat& m, மாறிலி Mat_கோவை& e)
{
    checkOperandsExist(m);
    Mat_கோவை en;
    e.op->divide(Mat_கோவை(m), e, en);
    திரும்பு en;
}

Mat_கோவை operator / (மாறிலி Mat_கோவை& e, இரட்டை s)
{
    Mat_கோவை en;
    e.op->multiply(e, 1./s, en);
    திரும்பு en;
}

Mat_கோவை operator / (இரட்டை s, மாறிலி Mat_கோவை& e)
{
    Mat_கோவை en;
    e.op->divide(s, e, en);
    திரும்பு en;
}

Mat_கோவை operator / (மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2)
{
    Mat_கோவை en;
    e1.op->divide(e1, e2, en);
    திரும்பு en;
}

Mat_கோவை operator < (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LT, a, b);
    திரும்பு e;
}

Mat_கோவை operator < (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LT, a, s);
    திரும்பு e;
}

Mat_கோவை operator < (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GT, a, s);
    திரும்பு e;
}

Mat_கோவை operator <= (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LE, a, b);
    திரும்பு e;
}

Mat_கோவை operator <= (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LE, a, s);
    திரும்பு e;
}

Mat_கோவை operator <= (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GE, a, s);
    திரும்பு e;
}

Mat_கோவை operator == (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_EQ, a, b);
    திரும்பு e;
}

Mat_கோவை operator == (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_EQ, a, s);
    திரும்பு e;
}

Mat_கோவை operator == (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_EQ, a, s);
    திரும்பு e;
}

Mat_கோவை operator != (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_NE, a, b);
    திரும்பு e;
}

Mat_கோவை operator != (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_NE, a, s);
    திரும்பு e;
}

Mat_கோவை operator != (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_NE, a, s);
    திரும்பு e;
}

Mat_கோவை operator >= (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GE, a, b);
    திரும்பு e;
}

Mat_கோவை operator >= (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GE, a, s);
    திரும்பு e;
}

Mat_கோவை operator >= (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LE, a, s);
    திரும்பு e;
}

Mat_கோவை operator > (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GT, a, b);
    திரும்பு e;
}

Mat_கோவை operator > (மாறிலி Mat& a, இரட்டை s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_GT, a, s);
    திரும்பு e;
}

Mat_கோவை operator > (இரட்டை s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Cmp::make_கோவை(e, cv::CMP_LT, a, s);
    திரும்பு e;
}

Mat_கோவை min(மாறிலி Mat& a, மாறிலி Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'm', a, b);
    திரும்பு e;
}

Mat_கோவை min(மாறிலி Mat& a, இரட்டை s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'n', a, s);
    திரும்பு e;
}

Mat_கோவை min(இரட்டை s, மாறிலி Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'n', a, s);
    திரும்பு e;
}

Mat_கோவை max(மாறிலி Mat& a, மாறிலி Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'M', a, b);
    திரும்பு e;
}

Mat_கோவை max(மாறிலி Mat& a, இரட்டை s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'N', a, s);
    திரும்பு e;
}

Mat_கோவை max(இரட்டை s, மாறிலி Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'N', a, s);
    திரும்பு e;
}

Mat_கோவை operator & (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '&', a, b);
    திரும்பு e;
}

Mat_கோவை operator & (மாறிலி Mat& a, மாறிலி Scalar& s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '&', a, s);
    திரும்பு e;
}

Mat_கோவை operator & (மாறிலி Scalar& s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '&', a, s);
    திரும்பு e;
}

Mat_கோவை operator | (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '|', a, b);
    திரும்பு e;
}

Mat_கோவை operator | (மாறிலி Mat& a, மாறிலி Scalar& s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '|', a, s);
    திரும்பு e;
}

Mat_கோவை operator | (மாறிலி Scalar& s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '|', a, s);
    திரும்பு e;
}

Mat_கோவை operator ^ (மாறிலி Mat& a, மாறிலி Mat& b)
{
    checkOperandsExist(a, b);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '^', a, b);
    திரும்பு e;
}

Mat_கோவை operator ^ (மாறிலி Mat& a, மாறிலி Scalar& s)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '^', a, s);
    திரும்பு e;
}

Mat_கோவை operator ^ (மாறிலி Scalar& s, மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '^', a, s);
    திரும்பு e;
}

Mat_கோவை operator ~(மாறிலி Mat& a)
{
    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '~', a, Scalar());
    திரும்பு e;
}

Mat_கோவை abs(மாறிலி Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, 'a', a, Scalar());
    திரும்பு e;
}

Mat_கோவை abs(மாறிலி Mat_கோவை& e)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை en;
    e.op->abs(e, en);
    திரும்பு en;
}


Size Mat_கோவை::size() மாறிலி
{
    எனில்( isT(*இது) || isInv(*இது) )
        திரும்பு Size(a.rows, a.cols);
    எனில்( isSolve(*இது) )
        திரும்பு Size(b.cols, a.cols);
    எனில்( isInitializer(*இது) )
        திரும்பு a.size();
    திரும்பு op ? op->size(*இது) : Size();
}


முழு Mat_கோவை::type() மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( isInitializer(*இது) )
        திரும்பு a.type();
    எனில்( isCmp(*இது) )
        திரும்பு CV_MAKETYPE(CV_8U, a.channels());
    திரும்பு op ? op->type(*இது) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Identity::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    எனில்( _type == -1 || _type == e.a.type() )
        m = e.a;
    இல்லையெனில்
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

உள்வரி வெறுமை MatOp_Identity::make_கோவை(Mat_கோவை& res, மாறிலி Mat& m)
{
    res = Mat_கோவை(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_AddEx::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    எனில்( e.b.data )
    {
        எனில்( e.s == Scalar() || !e.s.isReal() )
        {
            எனில்( e.alpha == 1 )
            {
                எனில்( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                இல்லையெனில் எனில்( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                இல்லையெனில்
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            இல்லையெனில் எனில்( e.beta == 1 )
            {
                எனில்( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                இல்லையெனில்
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            இல்லையெனில்
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            எனில்( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        இல்லையெனில்
        {
            எனில் (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(வெற்று, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    இல்லையெனில் எனில்( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        எனில் (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(வெற்று, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        திரும்பு;
    }
    இல்லையெனில் எனில்( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    இல்லையெனில் எனில்( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    இல்லையெனில்
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    எனில்( dst.data != m.data )
        dst.convertTo(m, m.type());
}


வெறுமை MatOp_AddEx::add(மாறிலி Mat_கோவை& e, மாறிலி Scalar& s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


வெறுமை MatOp_AddEx::subtract(மாறிலி Scalar& s, மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

வெறுமை MatOp_AddEx::multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

வெறுமை MatOp_AddEx::divide(இரட்டை s, மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( isScaled(e) )
        MatOp_Bin::make_கோவை(res, '/', e.a, Mat(), s/e.alpha);
    இல்லையெனில்
        MatOp::divide(s, e, res);
}


வெறுமை MatOp_AddEx::transpose(மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( isScaled(e) )
        MatOp_T::make_கோவை(res, e.a, e.alpha);
    இல்லையெனில்
        MatOp::transpose(e, res);
}

வெறுமை MatOp_AddEx::abs(மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_கோவை(res, 'a', e.a, -e.s*e.alpha);
    இல்லையெனில் எனில்( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_கோவை(res, 'a', e.a, e.b);
    இல்லையெனில்
        MatOp::abs(e, res);
}

உள்வரி வெறுமை MatOp_AddEx::make_கோவை(Mat_கோவை& res, மாறிலி Mat& a, மாறிலி Mat& b, இரட்டை alpha, இரட்டை beta, மாறிலி Scalar& s)
{
    res = Mat_கோவை(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Bin::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    எனில்( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    இல்லையெனில் எனில்( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    இல்லையெனில் எனில்( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    இல்லையெனில் எனில்( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    இல்லையெனில் எனில்( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    இல்லையெனில் எனில்( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    இல்லையெனில் எனில்( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    இல்லையெனில் எனில்( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    இல்லையெனில் எனில்( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    இல்லையெனில் எனில்( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    இல்லையெனில் எனில்( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    இல்லையெனில்
        CV_Error(cv::Error::StsError, "Unknown operation");

    எனில்( dst.data != m.data )
        dst.convertTo(m, _type);
}

வெறுமை MatOp_Bin::multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    இல்லையெனில்
        MatOp::multiply(e, s, res);
}

வெறுமை MatOp_Bin::divide(இரட்டை s, மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_கோவை(res, e.a, Mat(), s/e.alpha, 0);
    இல்லையெனில்
        MatOp::divide(s, e, res);
}

உள்வரி வெறுமை MatOp_Bin::make_கோவை(Mat_கோவை& res, எழுத்து op, மாறிலி Mat& a, மாறிலி Mat& b, இரட்டை scale)
{
    res = Mat_கோவை(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

உள்வரி வெறுமை MatOp_Bin::make_கோவை(Mat_கோவை& res, எழுத்து op, மாறிலி Mat& a, மாறிலி Scalar& s)
{
    res = Mat_கோவை(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Cmp::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    எனில்( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    இல்லையெனில்
        cv::compare(e.a, e.alpha, dst, e.flags);

    எனில்( dst.data != m.data )
        dst.convertTo(m, _type);
}

உள்வரி வெறுமை MatOp_Cmp::make_கோவை(Mat_கோவை& res, முழு cmpop, மாறிலி Mat& a, மாறிலி Mat& b)
{
    res = Mat_கோவை(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

உள்வரி வெறுமை MatOp_Cmp::make_கோவை(Mat_கோவை& res, முழு cmpop, மாறிலி Mat& a, இரட்டை alpha)
{
    res = Mat_கோவை(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_T::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    எனில்( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

வெறுமை MatOp_T::multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

வெறுமை MatOp_T::transpose(மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( e.alpha == 1 )
        MatOp_Identity::make_கோவை(res, e.a);
    இல்லையெனில்
        MatOp_AddEx::make_கோவை(res, e.a, Mat(), e.alpha, 0);
}

உள்வரி வெறுமை MatOp_T::make_கோவை(Mat_கோவை& res, மாறிலி Mat& a, இரட்டை alpha)
{
    res = Mat_கோவை(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_GEMM::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    எனில்( dst.data != m.data )
        dst.convertTo(m, _type);
}

வெறுமை MatOp_GEMM::add(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    தருக்க i1 = isIdentity(e1), i2 = isIdentity(e2);
    இரட்டை alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    எனில்( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_கோவை(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    இல்லையெனில் எனில்( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_கோவை(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    இல்லையெனில் எனில்( இது == e2.op )
        MatOp::add(e1, e2, res);
    இல்லையெனில்
        e2.op->add(e1, e2, res);
}

வெறுமை MatOp_GEMM::subtract(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    தருக்க i1 = isIdentity(e1), i2 = isIdentity(e2);
    இரட்டை alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    எனில்( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_கோவை(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    இல்லையெனில் எனில்( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_கோவை(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    இல்லையெனில் எனில்( இது == e2.op )
        MatOp::subtract(e1, e2, res);
    இல்லையெனில்
        e2.op->subtract(e1, e2, res);
}

வெறுமை MatOp_GEMM::multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

வெறுமை MatOp_GEMM::transpose(மாறிலி Mat_கோவை& e, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

உள்வரி வெறுமை MatOp_GEMM::make_கோவை(Mat_கோவை& res, முழு flags, மாறிலி Mat& a, மாறிலி Mat& b,
                                 இரட்டை alpha, மாறிலி Mat& c, இரட்டை beta)
{
    res = Mat_கோவை(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Invert::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    எனில்( dst.data != m.data )
        dst.convertTo(m, _type);
}

வெறுமை MatOp_Invert::matmul(மாறிலி Mat_கோவை& e1, மாறிலி Mat_கோவை& e2, Mat_கோவை& res) மாறிலி
{
    எனில்( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_கோவை(res, e1.flags, e1.a, e2.a);
    இல்லையெனில் எனில்( இது == e2.op )
        MatOp::matmul(e1, e2, res);
    இல்லையெனில்
        e2.op->matmul(e1, e2, res);
}

உள்வரி வெறுமை MatOp_Invert::make_கோவை(Mat_கோவை& res, முழு method, மாறிலி Mat& m)
{
    res = Mat_கோவை(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Solve::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    எனில்( dst.data != m.data )
        dst.convertTo(m, _type);
}

உள்வரி வெறுமை MatOp_Solve::make_கோவை(Mat_கோவை& res, முழு method, மாறிலி Mat& a, மாறிலி Mat& b)
{
    res = Mat_கோவை(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

வெறுமை MatOp_Initializer::assign(மாறிலி Mat_கோவை& e, Mat& m, முழு _type) மாறிலி
{
    எனில்( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    எனில்( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    இல்லையெனில் எனில்( e.flags == '0' )
        m = Scalar();
    இல்லையெனில் எனில்( e.flags == '1' )
        m = Scalar(e.alpha);
    இல்லையெனில்
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

வெறுமை MatOp_Initializer::multiply(மாறிலி Mat_கோவை& e, இரட்டை s, Mat_கோவை& res) மாறிலி
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

உள்வரி வெறுமை MatOp_Initializer::make_கோவை(Mat_கோவை& res, முழு method, Size sz, முழு type, இரட்டை alpha)
{
    res = Mat_கோவை(getGlobalMatOpInitializer(), method, Mat(sz, type, (வெறுமை*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

உள்வரி வெறுமை MatOp_Initializer::make_கோவை(Mat_கோவை& res, முழு method, முழு ndims, மாறிலி முழு* sizes, முழு type, இரட்டை alpha)
{
    res = Mat_கோவை(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (வெறுமை*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_கோவை Mat::t() மாறிலி
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*இது);
    Mat_கோவை e;
    MatOp_T::make_கோவை(e, *இது);
    திரும்பு e;
}

Mat_கோவை Mat::inv(முழு method) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Invert::make_கோவை(e, method, *இது);
    திரும்பு e;
}


Mat_கோவை Mat::mul(உள்ளீடு_Array m, இரட்டை scale) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    எனில்( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_கோவை e;
    MatOp_Bin::make_கோவை(e, '*', *இது, b, scale);
    திரும்பு e;
}

Mat_கோவை Mat::zeros(முழு rows, முழு cols, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '0', Size(cols, rows), type);
    திரும்பு e;
}

Mat_கோவை Mat::zeros(Size size, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '0', size, type);
    திரும்பு e;
}

Mat_கோவை Mat::zeros(முழு ndims, மாறிலி முழு* sizes, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '0', ndims, sizes, type);
    திரும்பு e;
}

Mat_கோவை Mat::zeros(மாறிலி MatShape& shape, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '0', shape.dims, shape.p, type);
    திரும்பு e;
}

Mat_கோவை Mat::ones(முழு rows, முழு cols, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '1', Size(cols, rows), type);
    திரும்பு e;
}

Mat_கோவை Mat::ones(Size size, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '1', size, type);
    திரும்பு e;
}

Mat_கோவை Mat::ones(முழு ndims, மாறிலி முழு* sizes, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '1', ndims, sizes, type);
    திரும்பு e;
}

Mat_கோவை Mat::ones(மாறிலி MatShape& shape, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, '1', shape.dims, shape.p, type);
    திரும்பு e;
}

Mat_கோவை Mat::eye(முழு rows, முழு cols, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, 'I', Size(cols, rows), type);
    திரும்பு e;
}

Mat_கோவை Mat::eye(Size size, முழு type)
{
    CV_INSTRUMENT_REGION();

    Mat_கோவை e;
    MatOp_Initializer::make_கோவை(e, 'I', size, type);
    திரும்பு e;
}

வெறுமை Mat_கோவை::swap(Mat_கோவை& other)
{
    using std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

உள்ளீடு_Array_2::உள்ளீடு_Array_2(மாறிலி Mat_கோவை& கோவை)
{
    எனில் (!isIdentity(கோவை))
    {
        Mat result = கோவை;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_கோவை result_கோவை(result);
        swap(const_cast<Mat_கோவை&>(கோவை), result_கோவை);
    }
    CV_Assert(isIdentity(கோவை));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &கோவை.a);
}

} // cv::
