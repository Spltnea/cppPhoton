# AI Generated

cmake_minimum_required(VERSION 3.25)

project(cppPhoton
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.4.2
)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(cli11 tomlplusplus)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(LLVM_LIBRARIES
    core
    support
    irreader
)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*.cxx" 
    "src/*.cc"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS 
    "include/*.h" 
    "include/*.hpp"
)

file(GLOB_RECURSE INLINE_IMPLEMENTATION_FILES CONFIGURE_DEPENDS 
    "include/*.ipp"
)

set(ALL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
foreach(HEADER ${HEADER_FILES} ${INLINE_IMPLEMENTATION_FILES})
    get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
    ${INLINE_IMPLEMENTATION_FILES}
)

if(INLINE_IMPLEMENTATION_FILES)
    set_source_files_properties(${IPP_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE ${LLVM_DEFINITIONS}
    $<$<CONFIG:Debug>:DEBUG=1>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ALL_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    CLI11::CLI11
    tomlplusplus::tomlplusplus
    ${LLVM_LIBRARIES}
)