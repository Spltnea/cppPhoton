cmake_minimum_required(VERSION 3.20)
project(cppPhoton VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(cppPhoton ${SOURCES})

target_include_directories(cppPhoton PRIVATE 
    include 
    ${LLVM_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs
    Core
    Analysis
    ExecutionEngine
    Target
    MC
    Support
    nativecodegen
)

target_link_libraries(cppPhoton PRIVATE ${llvm_libs})