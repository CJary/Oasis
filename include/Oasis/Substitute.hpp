//
// Created by Jary Chen on 4/17/25.
//

#ifndef SUBSTITUTE_HPP
#define SUBSTITUTE_HPP

#include "Expression.hpp"
#include <memory>

namespace Oasis {

/**
 * Return a deep copy of `expr` where every sub‑expression equal to `target`
 * has been replaced by a deep‑copy of `replacement`.
 */
std::unique_ptr<Expression>
Substitute(const Expression& expr,
           const Expression& target,
           const Expression& replacement);

} // namespace Oasis

#endif //SUBSTITUTE_HPP
