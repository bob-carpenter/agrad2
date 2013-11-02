#ifndef STAN__DIFF__REV__EXPRESSION_GRAPH_HPP
#define STAN__DIFF__REV__EXPRESSION_GRAPH_HPP

#include <ostream>
#include <vector>
#include <stan/diff/rev/edge.hpp>
#include <stan/diff/rev/typedefs.hpp>

namespace stan {
  namespace diff {

    struct expression_graph {
      index_t last_adjoint_;
      std::vector<edge> edges_;
      std::vector<double> adjoints_;
      
      expression_graph(index_t initial_capacity) 
        : last_adjoint_(0), // ensures 0 never used as adjoint
          edges_() {
        edges_.reserve(initial_capacity);
      } 
      
      ~expression_graph() { }
      
      void add_edge(index_t fx, index_t x, double dfx_dx) {
        edges_.push_back(edge(fx,x,dfx_dx));
      }

      index_t reserve_next_adjoint() {
        return ++last_adjoint_;
      }

      void propagate_derivatives(index_t fx) {
        adjoints_.resize(last_adjoint_ + 1, 0.0);
        adjoints_[fx] = 1.0;
        for (index_t i = edges_.size(); i-- > 0; )
          adjoints_[edges_[i].x_adjoint_] 
            += edges_[i].dfx_dx_ * adjoints_[edges_[i].fx_adjoint_];
      }

      double adjoint(index_t n) const {
        return adjoints_[n];
      }

      std::ostream& print(std::ostream& os) const {
        for (index_t i = 0; i < edges_.size(); ++i)
          os << "edge[" << i << "] = " << edges_[i]
             << std::endl;
        for (index_t i = 0; i < adjoints_.size(); ++i)
          os << "adjoint[" << i << "] = " << adjoints_[i]
             << std::endl;
        return os;
      }
      

    };

    std::ostream& operator<<(std::ostream& os, const expression_graph& eg) {
      return eg.print(os);
    }

    expression_graph graph_(10000);

  }
}

#endif
