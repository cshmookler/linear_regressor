// Standard includes
#include <cpp_result/result.hpp>
#include <string>
#include <fstream>
#include <functional>
#include <vector>
#include <iostream>

// External includes
#include "../include/solution.hpp"

res::result_t read_file(
  const std::function<res::result_t(const std::vector<linreg::num_t>& inputs,
    const linreg::num_t& output)>& function) {
    std::ifstream file("correlated_data.csv");
    std::string row_str;
    std::getline(file, row_str); // Discard the first line
    while (std::getline(file, row_str)) {
        std::vector<linreg::num_t> inputs;
        linreg::num_t output;
        size_t left_bound = 0;
        for (size_t i = 0; i < row_str.size(); ++i) {
            if (row_str[i] != ',') {
                continue;
            }

            auto value = mpf_class(row_str.substr(left_bound, i - left_bound));

            if (inputs.size() == 2) {
                output = value;
                break;
            }

            inputs.emplace_back(value);
            left_bound = i + 1;
        }

        auto result = function(inputs, output);
        if (result.failure()) {
            return RES_TRACE(result.error());
        }
    }

    return res::success;
}

int main() {
    linreg::equation_t equation;

    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[0] * input[0] * input[0];
    });
    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[0] * input[0];
    });
    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[0];
    });
    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[1] * input[1] * input[1];
    });
    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[1] * input[1];
    });
    equation.push_back([](const std::vector<mpf_class>& input) -> mpf_class {
        return input[1];
    });
    equation.push_back([](const std::vector<mpf_class>&) -> mpf_class {
        return 1;
    });

    linreg::solution_maker_t solution_maker(equation);

    auto result =
      read_file([&solution_maker](const std::vector<linreg::num_t>& inputs,
                  const linreg::num_t& output) {
          auto result = solution_maker.add_sample(inputs, output);
          if (result.failure()) {
              result = RES_TRACE(result.error());
              return result;
          }
          return res::success;
      });
    if (result.failure()) {
        std::cerr << result.error() << std::endl;
        return 1;
    }

    auto solution = solution_maker.get_solution();
    if (solution.has_error()) {
        std::cerr << solution.error() << std::endl;
        return 1;
    }

    std::cout << solution.value()[0] << "x^3 + ";
    std::cout << solution.value()[1] << "x^2 + ";
    std::cout << solution.value()[2] << "x + ";
    std::cout << solution.value()[3] << "y^3 + ";
    std::cout << solution.value()[4] << "y^2 + ";
    std::cout << solution.value()[5] << "y + ";
    std::cout << solution.value()[6] << std::endl;

    auto analyzer = solution_maker.get_analyzer(solution.value());
    if (analyzer.has_error()) {
        std::cerr << analyzer.error() << std::endl;
    }

    result = read_file([&analyzer](const std::vector<linreg::num_t>& inputs,
                         const linreg::num_t& output) {
        auto result = analyzer->add_sample(inputs, output);
        if (result.failure()) {
            result = RES_TRACE(result.error());
            return result;
        }
        return res::success;
    });
    if (result.failure()) {
        std::cerr << result.error() << std::endl;
        return 1;
    }

    std::cout << "error: " << analyzer->get_error() << std::endl;
}
