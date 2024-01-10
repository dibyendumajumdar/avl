cmake_minimum_required(VERSION 3.12)
project(AVL VERSION 0.0.1 LANGUAGES C)

option(ASAN "Controls whether address sanitizer should be enabled" OFF)

set(SRCS avl.h avl.c)

message("SOURCE dir is ${AVL_SOURCE_DIR}")
message("ASAN is ${ASAN}")

if (MSVC)
    source_group("Source Files" FILES ${SRCS})
endif ()

if (WIN32)
    # disable warnings about C string functions
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

include(CheckCCompilerFlag)
if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=return-type")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /we4047 /we4024 /we4133 /we4716")
endif()
if (ASAN)
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
    check_c_compiler_flag("-fsanitize=address" COMPILER_ASAN_SUPPORTED)
    if (COMPILER_ASAN_SUPPORTED AND NOT CMAKE_C_FLAGS_DEBUG MATCHES "-fsanitize=address")
       set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
    endif ()
endif ()

include(GNUInstallDirs)

set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

if (WIN32)
    set(LIBTYPE STATIC)
else()
    set(LIBTYPE SHARED)
endif()
add_library(libavl ${LIBTYPE}
        ${SRCS})
target_include_directories(libavl
        PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
        PUBLIC "${AVL_SOURCE_DIR}")
target_link_libraries(libavl ${EXTRA_LIBRARIES})
include(GenerateExportHeader)
generate_export_header(libavl)

add_executable(avltest avltest.c)
target_link_libraries(avltest libavl)
target_include_directories(avltest
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
        PRIVATE "${AVL_SOURCE_DIR}")
