# AI Generated

cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

set(CMAKE_CXX_EXTENSIONS ON)

if(CMAKE_CXX_COMPILER MATCHES "clang" OR "$ENV{CXX}" MATCHES "clang")
    string(APPEND CMAKE_CXX_FLAGS " -stdlib=libc++")
    string(APPEND CMAKE_EXE_LINKER_FLAGS " -stdlib=libc++")
endif()

project(cppPhoton
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_MODULE_STD ON)

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


file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
    "src/*.cpp" 
)

file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS 
    "src/*.cppm"
    "src/*.ixx"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS 
    "include/*.h" 
    "include/*.hpp"
)

file(GLOB_RECURSE INLINE_IMPLEMENTATION_FILES CONFIGURE_DEPENDS 
    "include/*.ipp"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
    ${INLINE_IMPLEMENTATION_FILES}
)

if(MODULE_FILES)
    target_sources(${PROJECT_NAME}
        PRIVATE
            FILE_SET CXX_MODULES FILES ${MODULE_FILES}
    )
endif()

if(INLINE_IMPLEMENTATION_FILES)
    set_source_files_properties(${INLINE_IMPLEMENTATION_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG=1>
)

set(ALL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
foreach(HEADER ${HEADER_FILES} ${INLINE_IMPLEMENTATION_FILES})
    get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ALL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    CLI11::CLI11
    tomlplusplus::tomlplusplus
)