#include "../include/Oasis/Polynomial.hpp"
#include "../include/Oasis/Real.hpp"
#include "../include/Oasis/Add.hpp"
#include "../include/Oasis/Multiply.hpp"
#include "../include/Oasis/Exponent.hpp"

namespace Oasis {

Polynomial::Polynomial(const Expression& expr, const Variable& var) {
  // TODO: recursively traverse `expr`:
  //   - if Add: merge sub‐polys
  //   - if Multiply<Real,Exponent<var,Real>>: read coef * var^exp
  //   - if Real: degree 0
  //   - if var alone: degree 1
  // Fill coeffs_[degree] = value.
}

std::pair<Polynomial,Polynomial>
Polynomial::Divide(const Polynomial& N, const Polynomial& D) {
  Polynomial Q{{},}; // start zero
  Polynomial R = N;
  while (R.Degree() >= D.Degree()) {
    int degDiff = R.Degree() - D.Degree();
    double coef = R.coeffs_.rbegin()->second / D.coeffs_.rbegin()->second;
    // term = coef * x^(degDiff)
    Q.coeffs_[degDiff] += coef;
    // subtract (coef * x^(degDiff)) * D from R
    for (auto [e,c] : D.coeffs_) {
      R.coeffs_[e+degDiff] -= c * coef;
      if (std::abs(R.coeffs_[e+degDiff]) < 1e-12)
        R.coeffs_.erase(e+degDiff);
    }
  }
  return {Q, R};
}

std::vector<std::pair<double,int>>
Polynomial::FactorLinear() const {
  // TODO: use a root‐finding method (e.g. Durand–Kerner or even naive
  // scanning if you restrict to integer roots). Return each root with mult=1.
  return {};
}

double Polynomial::Evaluate(double r) const {
  double sum = 0;
  for (auto [e,c] : coeffs_)
    sum += c * std::pow(r, e);
  return sum;
}

Polynomial Polynomial::Derivative() const {
  Polynomial D;
  for (auto [e,c] : coeffs_) {
    if (e>0) D.coeffs_[e-1] = c * e;
  }
  return D;
}

std::unique_ptr<Expression>
Polynomial::ToExpression(const Variable& var) const {
  std::unique_ptr<Expression> acc = std::make_unique<Real>(0.0);
  for (auto [e,c] : coeffs_) {
    // term = c * (var^e)
    std::unique_ptr<Expression> term;
    if (e == 0) {
      term = std::make_unique<Real>(c);
    } else {
      term = std::make_unique<Multiply<Expression,Expression>>(
         Real{c}, Exponent<Expression,Real>{ var, Real{double(e)} }
      );
    }
    acc = std::make_unique<Add<Expression,Expression>>(*acc, *term);
  }
  return acc;
}

int Polynomial::Degree() const {
  if (coeffs_.empty()) return 0;
  return coeffs_.rbegin()->first;
}

} // namespace Oasis
