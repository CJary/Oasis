#include "../include/Oasis/PartialFraction.hpp"
#include "../include/Oasis/Polynomial.hpp"
#include "../include/Oasis/Add.hpp"
#include "../include/Oasis/Subtract.hpp"
#include "../include/Oasis/Multiply.hpp"
#include "../include/Oasis/RecursiveCast.hpp"

namespace Oasis {

std::unique_ptr<Expression>
PartialFraction(const Divide<Expression,Expression>& fraction)
{
    // 1) Simplify input and grab numerator & denominator
    auto frac = fraction.Simplify();
    auto& numExpr = frac->GetMostSigOp();
    auto& denExpr = frac->GetLeastSigOp();

    // 2) Pick the variable (e.g. “s”)
    Variable s{"s"};

    // 3) Build Polynomial objects
    Polynomial p{ numExpr, s };
    Polynomial q{ denExpr, s };

    // 4) Long‐divide if needed
    auto [quotientPoly, remainderPoly] = Polynomial::Divide(p, q);

    // 5) Factor the proper‐fraction denominator
    auto factors = q.FactorLinear();  // pairs of (root, multiplicity)
    auto qPrime = q.Derivative();

    // 6) Compute each residue A_i = R(r_i) / q′(r_i)
    std::vector<std::unique_ptr<Expression>> terms;
    for (auto [ri, mult] : factors) {
        double Ai = remainderPoly.Evaluate(ri) / qPrime.Evaluate(ri);
        auto num   = std::make_unique<Real>(Ai);
        auto denom = Subtract<Expression,Real>{ s, Real{ri} };
        terms.push_back( Divide<Expression,Real>{ *num, denom }.Generalize() );
    }

    // 7) Reassemble: polynomial part + sum of simple‐fraction terms
    std::unique_ptr<Expression> result = quotientPoly.ToExpression(s);
    for (auto& t : terms) {
        result = Add<Expression,Expression>{ *result, *t }.Simplify();
    }

    return result->Simplify();
}

} // namespace Oasis
