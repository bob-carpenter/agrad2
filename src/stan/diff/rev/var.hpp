#ifndef STAN__DIFF__REV__VAR_HPP
#define STAN__DIFF__REV__VAR_HPP

#include <limits>
#include <ostream>
#include <stan/diff/rev/edge.hpp>
#include <stan/diff/rev/expression_graph.hpp>
#include <stan/diff/rev/typedefs.hpp>

namespace stan {
  namespace diff {


    struct var {
      double val_;
      index_t adjoint_;

      var(double val, index_t adjoint)
        : val_(val),
          adjoint_(adjoint) {
      }

      var(double val) 
      : val_(val), 
        adjoint_(graph_.reserve_next_adjoint()) {
      }

      var(const var& v) 
      : val_(v.val_),
        adjoint_(v.adjoint_) {
      }

      var() 
        : val_(std::numeric_limits<double>::quiet_NaN()),
          // 0 is special value not otherwise used
          adjoint_(0) {
      }

      ~var() { }

      index_t adjoint() const {
        return adjoint_;
      }
      
      double value() const {
        return val_;
      }

      std::ostream& print(std::ostream& os) const {
        return os << val_ << ':' << adjoint_;
      }

      var& operator+=(const var& other) {
        index_t fx_adjoint = graph_.reserve_next_adjoint();
        graph_.add_edge(fx_adjoint, adjoint_, 1);
        graph_.add_edge(fx_adjoint, other.adjoint_, 1);
        val_ += other.val_;
        adjoint_ = fx_adjoint;
        return *this;
      }
      var& operator+=(double other) {
        index_t fx_adjoint = graph_.reserve_next_adjoint();
        graph_.add_edge(fx_adjoint, adjoint_, 1);
        val_ += other;
        adjoint_ = fx_adjoint;
        return *this;
      }

      var& operator*=(const var& other) {
        index_t fx_adjoint = graph_.reserve_next_adjoint();
        graph_.add_edge(fx_adjoint, adjoint_, other.val_);
        graph_.add_edge(fx_adjoint, other.adjoint_, val_);
        val_ *= other.val_;
        adjoint_ = fx_adjoint;
        return *this;
      }
      var& operator*=(double other) {
        index_t fx_adjoint = graph_.reserve_next_adjoint();
        graph_.add_edge(fx_adjoint, adjoint_, other);
        val_ *= other;
        adjoint_ = fx_adjoint;
        return *this;
      }
      
      
    };

    std::ostream& operator<<(std::ostream& os, const var& v) {
      return v.print(os);
    }

    var operator+(const var& a, const var& b) {
      var a_copy(a);
      return a_copy += b;
    }
    var operator+(const var& a, double b) {
      var a_copy(a);
      return a_copy += b;
    }
    var operator+(double a, const var& b) {
      return b + a;
    }

    var operator*(const var& a, const var& b) {
      var a_copy(a);
      return a_copy *= b;
    }
    var operator*(const var& a, double b) {
      var a_copy(a);
      return a_copy *= b;
    }
    var operator*(double a, const var& b) {
      return b * a;
    }



  }
}

#endif
