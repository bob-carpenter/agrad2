#include <iostream>
#include <stan/diff/rev/var.hpp>

int main() {
  using stan::diff::var;
  var a = 5;
  var b = 13;
  var ab = a * b;
  ab *= 3;

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
