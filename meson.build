# Declare project information
project(
    'linear_regressor',
    'cpp',
    version : files('VERSION'),
    default_options : [ 'cpp_std=c++17' ],
)

# Project root and build directories
root_dir = meson.project_source_root()
build_dir = meson.project_build_root()
include_dir = root_dir / 'include'
src_dir = root_dir / 'src'
tests_dir = root_dir / 'tests'
examples_dir = root_dir / 'examples'

# Insert the project version into the version header file
conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
    configuration : conf_data,
    input : include_dir / 'version.hpp.in',
    output : 'version.hpp',
)

lib_linear_regressor_headers = files(
    build_dir / 'version.hpp',
    include_dir / 'solution.hpp'
)
install_headers(lib_linear_regressor_headers, subdir : 'linear_regressor')

dep_gmp = dependency(
    'gmp',
    required : true,
    method : 'auto',
)

dep_gmpxx = dependency(
    'gmpxx',
    required : true,
    method : 'auto',
)

examples = [
    'version',
    'lab1',
]

# Copy the data for 'lab1' to the build directory
fs = import('fs')
fs.copyfile('examples' / 'lab1_data.csv', 'lab1_data.csv')

foreach example_name : examples
    executable(
        'example_' + example_name,
        files(
            examples_dir / (example_name + '.cpp'),
        ),
        dependencies : [dep_gmp, dep_gmpxx]
    )
endforeach

dep_gtest_main = dependency(
    'gtest_main',
    required : false,
    method : 'auto',
)

tests = [
    'version',
]

if dep_gtest_main.found()
    foreach test_name : tests
        test_executable = executable(
            'test_' + test_name,
            files(
                tests_dir / (test_name + '.test.cpp'),
            ),
            dependencies : dep_gtest_main,
        )
        test(test_name, test_executable)
    endforeach
else
    warning('Skipping tests due to missing dependencies')
endif
