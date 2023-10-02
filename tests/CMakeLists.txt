# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library




#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#add_subdirectory(googletest-main)s


add_executable(differentiation_test differentiation_test.cpp) 
add_test(NAME differentiation_test COMMAND differentiation_test.cpp)
add_executable(interpolation_test interpolationTest.cpp) 
add_test(NAME interpolation_test COMMAND interpolationTest.cpp)
target_link_libraries(differentiation_test CompMath gtest gtest_main) 
target_link_libraries(interpolation_test CompMath gtest gtest_main) 