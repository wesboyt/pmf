project(pmf)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES example.cpp)
set(CMAKE_CXX_FLAGS "-g -Wall -O2")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(pmf ${SOURCE_FILES})