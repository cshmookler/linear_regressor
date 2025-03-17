// Standard includes
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

// External includes
#include "../include/solution.hpp"

int main() {
    linreg::equation_t equation;

    equation.push_back([](const std::vector<mpf_class>& input) {
        return input[0];
    });
    equation.push_back([](const std::vector<mpf_class>& input) {
        return input[1];
    });
    equation.push_back([](const std::vector<mpf_class>&) {
        return 1;
    });

    linreg::solution_maker_t solution_maker(equation);

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

        solution_maker.add_sample(inputs, output);
    }

    auto solution = solution_maker.get_solution();

    std::cout << "w1 = " << solution.value()[0] << std::endl;
    std::cout << "w2 = " << solution.value()[1] << std::endl;
    std::cout << "b  = " << solution.value()[2] << std::endl;
}
