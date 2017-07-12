cmake_minimum_required(VERSION 2.6)
project(msproxy_poco)

include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

file(GLOB SOURCES "src/*.cc")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -O2 -std=c++11 -Wall -lPocoFoundation -lPocoNet -lPocoUtil -lPocoData -lPocoDataMySQL -lPocoRedis")

add_executable(ms_proxy ${SOURCES})
