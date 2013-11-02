#ifndef STAN__DIFF__REV__EDGE_HPP
#define STAN__DIFF__REV__EDGE_HPP

#include <ostream>
#include <stan/diff/rev/typedefs.hpp>

namespace stan {
  namespace diff {

   struct edge {
      const index_t fx_adjoint_;
      const index_t x_adjoint_;
      const double dfx_dx_;
      
      edge(index_t fx_adjoint, index_t x_adjoint, double dfx_dx) 
        : fx_adjoint_(fx_adjoint),
          x_adjoint_(x_adjoint),
          dfx_dx_(dfx_dx) {
      }
      
      ~edge() { }

    };
    

    std::ostream& operator<<(std::ostream& os, const edge& v) {
      return os << "edge("
                << v.fx_adjoint_
                << "," << v.x_adjoint_
                << "," << v.dfx_dx_
                << ")";
    }
    

  }
}

#endif
