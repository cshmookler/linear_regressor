#pragma once

// Standard includes
#include <functional>
#include <string>
#include <vector>

// External includes
#include <cpp_result/all.hpp>
#include <eigen3/Eigen/Eigen>
#include <gmpxx.h>

namespace linreg {

// A linear equation is represented with an array of functions.

using num_t = mpf_class;
using term_t = num_t(const std::vector<num_t>&);
using equation_t = std::vector<std::function<term_t>>;
using solution_t = std::vector<num_t>;

class solution_maker_t {
    equation_t equation_;
    Eigen::Index square_height_;
    Eigen::Matrix<num_t, Eigen::Dynamic, Eigen::Dynamic> coeff_matrix_;
    Eigen::Matrix<num_t, Eigen::Dynamic, Eigen::Dynamic> const_vector_;

  public:
    solution_maker_t(const equation_t& equation)
    : equation_(equation)
    , square_height_(static_cast<Eigen::Index>(this->equation_.size())) {
        this->coeff_matrix_.resize(this->square_height_, this->square_height_);
        this->const_vector_.resize(this->square_height_, 1);
    }

    solution_maker_t(equation_t&& equation)
    : equation_(std::move(equation))
    , square_height_(static_cast<Eigen::Index>(this->equation_.size())) {
        this->coeff_matrix_.resize(this->square_height_, this->square_height_);
        this->const_vector_.resize(this->square_height_, 1);
    }

    res::result_t add_sample(
      const std::vector<num_t>& inputs, const num_t& output) {
        std::vector<num_t> terms(this->equation_.size());

        try {
            for (size_t i = 0; i < terms.size(); ++i) {
                auto term_generator = this->equation_[i];
                terms[i] = term_generator(inputs);
            }
        } catch (const std::exception& exception) {
            return RES_NEW_ERROR(
              "An exception occurred during term generation: "
              + std::string{ exception.what() });
        }

        for (Eigen::Index row = 0; row < this->square_height_; ++row) {
            for (Eigen::Index col = 0; col < this->square_height_; ++col) {
                this->coeff_matrix_(row, col) += terms[row] * terms[col];
            }
            this->const_vector_(row, 0) += terms[row] * output;
        }

        return res::success;
    }

    res::optional_t<solution_t> get_solution() const {
        solution_t solution(this->square_height_);

        auto matrix_solution =
          this->coeff_matrix_.inverse() * this->const_vector_;

        for (Eigen::Index i = 0; i < matrix_solution.size(); ++i) {
            solution[i] = matrix_solution(i, 0);
        }

        return solution;
    }
};

} // namespace linreg
