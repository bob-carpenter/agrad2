#include <iostream>
#include <stan/diff/rev/evar.hpp>

int main() {
  using stan::diff::evar;
  evar a = 5;
  evar b = 13;
  evar ab = a * b;
  ab = a * b;

  stan::diff::graph_.propagate_derivatives(ab.adjoint());

  std::cout << std::endl
            << "a=" << a
            << "; b=" << b
            << "; ab*3=" << ab
            << std::endl;

  std::cout << std::endl
            << stan::diff::graph_
            << std::endl;
}
