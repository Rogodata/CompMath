cmake_minimum_required(VERSION 3.0.0)
project(CompMath VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(tests)
add_subdirectory(source)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)


add_executable(CompMathexe tests/mainFiles/CompMath.cpp)
target_link_libraries (CompMathexe Eigen3::Eigen)
