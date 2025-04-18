//
// Created by Jary Chen on 4/17/25.
//
// Created by … on …
#include "Oasis/Substitute.hpp"
#include "Oasis/MatchCast.hpp"
#include "Oasis/Add.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Variable.hpp"
#include "Oasis/Real.hpp"

namespace Oasis {

std::unique_ptr<Expression>
Substitute(const Expression& expr,
           const Expression& target,
           const Expression& replacement)
{
    // 1) Base Case if the whole node matches
    if (expr.Equals(target)) {
        return replacement.Copy();
    }

    // 2) Leaf: Variable
    if (auto v = dynamic_cast<const Variable*>(&expr)) {
        return v->Copy();
    }

    // 3) Binary node: Add
    if (auto a = dynamic_cast<const Add<Expression>*>(&expr)) {
        auto L = Substitute(a->GetMostSigOp(),  target, replacement);
        auto R = Substitute(a->GetLeastSigOp(), target, replacement);
        return std::make_unique<Add<Expression>>(*L, *R);
    }

    // 4) Binary node: Multiply
    if (auto m = dynamic_cast<const Multiply<Expression>*>(&expr)) {
        auto L = Substitute(m->GetMostSigOp(),  target, replacement);
        auto R = Substitute(m->GetLeastSigOp(), target, replacement);
        return std::make_unique<Multiply<Expression>>(*L, *R);
    }

    if (auto e = dynamic_cast<const Exponent<Expression>*>(&expr)) {
        auto base = Substitute(e->GetMostSigOp(),  target, replacement);
        auto pow  = Substitute(e->GetLeastSigOp(), target, replacement);
        return std::make_unique<Exponent<Expression>>(*base, *pow);
    }

    if (auto mrv = dynamic_cast<const Multiply<Real,Variable>*>(&expr); mrv) {
        auto L = Substitute(mrv->GetMostSigOp(),  target, replacement);
        auto R = Substitute(mrv->GetLeastSigOp(), target, replacement);

        // both *L and *R must now be Real
        const auto& lreal = dynamic_cast<const Real&>(*L);
        const auto& rreal = dynamic_cast<const Real&>(*R);

        return std::make_unique<Multiply<Real,Real>>(lreal, rreal);
    }

    return expr.Copy();
}

} // namespace Oasis
