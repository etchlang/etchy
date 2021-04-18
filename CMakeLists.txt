cmake_minimum_required(VERSION 3.6)

project(etchy)

add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(libetch)

set(ETCHY_SRCS
	src/etchy/main.cpp
)

# etchy
add_executable(etchy ${ETCHY_SRCS})
target_compile_definitions(etchy PRIVATE ${ETCH_DEFINITIONS})
target_include_directories(etchy PRIVATE include libetch/include ${ETCH_INCLUDE_DIRS})
target_link_directories(etchy PRIVATE ${ETCH_LIBRARY_DIRS})
target_link_libraries(etchy $<TARGET_FILE:etch> ${ETCH_LIBS})
set_property(TARGET etchy PROPERTY CXX_STANDARD 17)
set_property(TARGET etchy PROPERTY CXX_STANDARD_REQUIRED ON)
add_dependencies(etchy etch)
