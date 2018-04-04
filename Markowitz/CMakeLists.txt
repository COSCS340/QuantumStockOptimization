project(markowitz CXX)
cmake_minimum_required(VERSION 2.8)
#set(CMAKE_DISABLE_IN_SOURCE_BUILDS ON)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-std=c++0x" -ggdb3)

find_package(Boost COMPONENTS filesystem program_options system REQUIRED)
find_package(XACC REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${XACC_INCLUDE_DIRS})
include_directories(problem)

file (GLOB SRC ./*.cpp)

link_directories(${XACC_LIBRARY_DIR})

add_library(markowitz SHARED ${SRC})

target_link_libraries(markowitz ${Boost_LIBRARIES} ${XACC_LIBRARIES} pthread dl)

add_executable(Markowitz main.cpp)
target_link_libraries(Markowitz markowitz)

install(TARGETS Markowitz DESTINATION ${XACC_ROOT}/bin)
