#include <symengine/complex.h>
#include <symengine/add.h>
#include <symengine/infinity.h>
#include <symengine/pow.h>
#include <symengine/nan.h>

namespace SymEngine
{

Constant::Constant(const std::string &name)
    : name_{name} {SYMENGINE_ASSIGN_TYPEID()}

      hash_t Constant::__hash__() const
{
    hash_t seed = SYMENGINE_CONSTANT;
    hash_combine<std::string>(seed, name_);
    return seed;
}

bool Constant::__eq__(const Basic &o) const
{
    if (is_a<Constant>(o))
        return name_ == down_cast<const Constant &>(o).name_;
    return false;
}

int Constant::compare(const Basic &o) const
{
    SYMENGINE_ASSERT(is_a<Constant>(o))
    const Constant &s = down_cast<const Constant &>(o);
    if (name_ == s.name_)
        return 0;
    return name_ < s.name_ ? -1 : 1;
}

#define DEFINE_CONSTANT(t, n, d)                                               \
    RCP<const t> n = []() {                                                    \
        static const RCP<const t> c = d;                                       \
        return c;                                                              \
    }()

DEFINE_CONSTANT(Integer, zero, integer(0));
DEFINE_CONSTANT(Integer, one, integer(1));
DEFINE_CONSTANT(Integer, minus_one, integer(-1));
DEFINE_CONSTANT(Integer, two, integer(2));

DEFINE_CONSTANT(Number, I, Complex::from_two_nums(*zero, *one));

DEFINE_CONSTANT(Constant, pi, constant("pi"));
DEFINE_CONSTANT(Constant, E, constant("E"));
DEFINE_CONSTANT(Constant, EulerGamma, constant("EulerGamma"));
DEFINE_CONSTANT(Constant, Catalan, constant("Catalan"));
DEFINE_CONSTANT(Constant, GoldenRatio, constant("GoldenRatio"));

DEFINE_CONSTANT(Infty, Inf, Infty::from_int(1));
DEFINE_CONSTANT(Infty, NegInf, Infty::from_int(-1));
DEFINE_CONSTANT(Infty, ComplexInf, Infty::from_int(0));

DEFINE_CONSTANT(NaN, Nan, make_rcp<NaN>());

// Global variables declared in functions.cpp
// Look over https://github.com/sympy/symengine/issues/272
// for further details
DEFINE_CONSTANT(Basic, i2, integer(2));

namespace
{
RCP<const Basic> sqrt_(const RCP<const Basic> &arg)
{
    return pow(arg, div(one, i2));
}
} // namespace

DEFINE_CONSTANT(Basic, i3, integer(3));
DEFINE_CONSTANT(Basic, i5, integer(5));
DEFINE_CONSTANT(Basic, im2, integer(-2));
DEFINE_CONSTANT(Basic, im3, integer(-3));
DEFINE_CONSTANT(Basic, im5, integer(-5));

DEFINE_CONSTANT(Basic, sq3, sqrt_(i3));
DEFINE_CONSTANT(Basic, sq2, sqrt_(i2));
DEFINE_CONSTANT(Basic, sq5, sqrt_(i5));

DEFINE_CONSTANT(Basic, C0, div(sub(sq3, one), mul(i2, sq2)));
DEFINE_CONSTANT(Basic, C1, div(one, i2));
DEFINE_CONSTANT(Basic, C2, div(sq2, i2));
DEFINE_CONSTANT(Basic, C3, div(sq3, i2));
DEFINE_CONSTANT(Basic, C4, div(add(sq3, one), mul(i2, sq2)));
DEFINE_CONSTANT(Basic, C5, div(sqrt_(sub(i5, sqrt_(i5))), integer(8)));
DEFINE_CONSTANT(Basic, C6, div(sub(sqrt_(i5), one), integer(4)));

DEFINE_CONSTANT(Basic, mC0, mul(minus_one, C0));
DEFINE_CONSTANT(Basic, mC1, mul(minus_one, C1));
DEFINE_CONSTANT(Basic, mC2, mul(minus_one, C2));
DEFINE_CONSTANT(Basic, mC3, mul(minus_one, C3));
DEFINE_CONSTANT(Basic, mC4, mul(minus_one, C4));
DEFINE_CONSTANT(Basic, mC5, mul(minus_one, C5));
DEFINE_CONSTANT(Basic, mC6, mul(minus_one, C6));

#undef DEFINE_CONSTANT

// sin_table[n] represents the value of sin(pi*n/12) for n = 0..23
RCP<const Basic> sin_table[]
    = {zero, C0,  C1,  C2,  C3,  C4,  one,       C4,  C3,  C2,  C1,  C0,
       zero, mC0, mC1, mC2, mC3, mC4, minus_one, mC4, mC3, mC2, mC1, mC0};

umap_basic_basic inverse_cst = {
    {C3, i3},
    {mC3, im3},
    {C2, mul(i2, i2)},
    {mC2, mul(im2, i2)},
    {C4, integer(12)},
    {mC4, integer(-12)},
    {C5, i5},
    {mC5, im5},
    {C6, integer(10)},
    {mC6, integer(-10)},
    {div(one, i2), integer(6)},
    {div(minus_one, i2), integer(-6)},
};

umap_basic_basic inverse_tct = {
    {div(one, sq3), mul(i2, i3)},
    {div(minus_one, sq3), mul(im2, i3)},
    {sq3, i3},
    {mul(minus_one, sq3), im3},
    {add(one, sq2), div(pow(i2, i3), i3)},
    {mul(minus_one, add(one, sq2)), div(pow(i2, i3), im3)},
    {sub(sq2, one), pow(i2, i3)},
    {sub(one, sq2), pow(im2, i3)},
    {sub(i2, sq3), mul(mul(i2, i2), i3)},
    {sub(sq3, i2), mul(mul(im2, i2), i3)},
    {sqrt(add(i5, mul(i2, sqrt(i5)))), div(i5, i2)},
    {mul(minus_one, sqrt(add(i5, mul(i2, sqrt(i5))))), div(im5, i2)},
    {one, pow(i2, i2)},
    {minus_one, mul(minus_one, pow(i2, i2))},
};

} // namespace SymEngine
