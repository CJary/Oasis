#ifndef OASIS_PARTIALFRACTION_HPP
#define OASIS_PARTIALFRACTION_HPP

#include "Divide.hpp"
#include <memory>

namespace Oasis {

/**
 * Perform a partial‐fraction decomposition of a proper rational
 * Divide<Expression,Expression> in one variable.
 */
std::unique_ptr<Expression>
PartialFraction(const Divide<Expression,Expression>& fraction);

} // namespace Oasis

#endif // OASIS_PARTIALFRACTION_HPP
