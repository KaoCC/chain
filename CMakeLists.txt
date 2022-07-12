
cmake_minimum_required(VERSION 3.10)
project(chain CXX)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories("chain")
add_executable(chain_test tests/test.cpp)
target_link_libraries(chain_test ${CONAN_LIBS})
target_compile_features(chain_test PUBLIC cxx_std_17)

enable_testing()
add_test(NAME chain_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND chain_test)