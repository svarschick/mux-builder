cmake_minimum_required (VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
endif()

set(PROJECT_NAME Binary_Builder)

project(${PROJECT_NAME})

#CXX config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#src
set(MAIN_FILE main.cpp)
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/hdr")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${HEADER_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE HEADER_FILES "${HEADER_DIR}/*.h")
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${HEADER_FILES} ${SOURCE_FILES})

#Link boost
set(BOOST_ROOT "C:/cpp/boost_1_86_0")
set(Boost_NO_SYSTEM_PATHS TRUE)
find_package(Boost 1.86.0 REQUIRED COMPONENTS system)
if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Boost library not found")
endif()
