project(pmf)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES example.cpp FiveEval.cpp)
set(CMAKE_CXX_FLAGS "-g -Wall -O2")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(pmf ${SOURCE_FILES})
