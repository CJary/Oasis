#ifndef OASIS_POLYNOMIAL_HPP
#define OASIS_POLYNOMIAL_HPP

#include "Expression.hpp"
#include "Variable.hpp"
#include <map>
#include <vector>
#include <utility>

namespace Oasis {

class Polynomial {
public:
    Polynomial(const Expression& expr, const Variable& var);

    static std::pair<Polynomial,Polynomial>
    Divide(const Polynomial& numerator,
           const Polynomial& denominator);

    std::vector<std::pair<double,int>> FactorLinear() const;

    double Evaluate(double r) const;

    Polynomial Derivative() const;

    std::unique_ptr<Expression> ToExpression(const Variable& var) const;

    int Degree() const;

private:
    std::map<int,double> coeffs_;
};

} // namespace Oasis

#endif // OASIS_POLYNOMIAL_HPP
