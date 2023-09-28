cmake_minimum_required(VERSION 3.0.0)
project(CompMath VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(tests)
add_subdirectory(source)

add_executable(CompMathexe CompMath.cpp)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (CompMathexe Eigen3::Eigen)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
