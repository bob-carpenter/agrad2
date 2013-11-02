#ifndef STAN__DIFF__REV__VAR_HPP
#define STAN__DIFF__REV__VAR_HPP

#include <limits>
#include <ostream>
#include <stan/diff/rev/edge.hpp>
#include <stan/diff/rev/expression_graph.hpp>
#include <stan/diff/rev/typedefs.hpp>

namespace stan {
  namespace diff {

    template <typename T>
    struct expression {
      const T& cast() const {
        return static_cast<const T&>(*this);
      }
      double value() const {
        return cast().value();
      }
      void add_edges(expression_graph& eg, index_t fx_adjoint, double dfx_de) const {
        cast().add_edges(eg,dfx_de);
      }
      std::ostream& print(std::ostream& o) const {
        return cast().print(o);
      }
    };


    // FIXME:  make anonymous
    template <typename T1, typename T2>
    struct multiply 
      : public expression<multiply<T1,T2> > {

      double val_;
      const T1& arg1_;
      const T2& arg2_;

      multiply(const T1& arg1, const T2& arg2)
        : val_(arg1_.value() * arg2_.value()),
          arg1_(arg1),
          arg2_(arg2) {
      }
      
      void add_edges(expression_graph& eg, index_t fx_adjoint, double dfx_dx) const {
        arg1_.add_edges(eg, fx_adjoint, dfx_dx * arg2_.value());
        arg2_.add_edges(eg, fx_adjoint, dfx_dx * arg1_.value());
      }

      double value() const {
        return val_;
      }

      std::ostream& print(std::ostream& o) const {
        o << "multiply(";
        arg1_.print(o);
        o << ",";
        arg2_.print(o);
        o << ")";
        return o;
      }

    };


    template <typename T1, typename T2>
    inline multiply<T1,T2> 
    operator*(const expression<T1>& arg1, const expression<T2>& arg2) {
      return multiply<T1,T2>(arg1.cast(),arg2.cast());
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& o, const expression<T>& e) {
      return e.print(o);
    }

    struct evar 
      : public expression<evar> {

      double val_;
      index_t adjoint_;

      evar(double val, index_t adjoint)
        : val_(val),
          adjoint_(adjoint) {
      }
      evar(double val) 
      : val_(val),
        // ?? does this reservation have to be so eager?
        adjoint_(graph_.reserve_next_adjoint()) {
      }
      evar(const evar& x)
        : val_ (x.val_),
          adjoint_(x.adjoint_) {
      }
      template <typename T>
      evar(const expression<T>& expr)
        : val_(expr.value()),
          adjoint_(graph_.reserve_next_adjoint()) {
        add_edges(graph_,adjoint_,1.0);
        std::cout << "hello from ctor" << std::endl;
      }

      ~evar() { }

      // can't find if implicit
      template <typename T>
      evar& operator=(const expression<T>& expr) {
        std::cout << "hello from operator=" << std::endl;
        val_ = expr.value();
        adjoint_ = graph_.reserve_next_adjoint();
        add_edges(graph_,adjoint_,1.0);
        return *this;
      }

      evar& operator=(double x) {
        val_ = x;
        return *this;
      }

      evar& operator=(const evar& x) {
        val_ = x.val_;
        adjoint_ = x.adjoint_;
        return *this;
      }

      void add_edges(expression_graph& eg, index_t fx_adjoint, double dfx_dx) const {
        eg.add_edge(adjoint_,fx_adjoint,dfx_dx);
      }

      double value() const {
        return val_;
      }

      size_t adjoint() const {
        return adjoint_;
      }

      std::ostream& print(std::ostream& o) const {
        return o << val_ << ":" << adjoint_;
      }

    };



  }
}

#endif
