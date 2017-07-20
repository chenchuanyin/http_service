cmake_minimum_required(VERSION 2.6)
project(msproxy_poco)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_C_COMPILER "/opt/gcc-6/bin/gcc")
    set(CMAKE_CXX_COMPILER "/opt/gcc-6/bin/g++")
    set(OS_TYPE "linux")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(OS_TYPE "mac")
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/bin")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ELSE()
    MESSAGE(STATUS "cannot support current system: ${CMAKE_SYSTEM_NAME}")
ENDIF()


include_directories("third_party")
include_directories("third_party/loglib/include")
include_directories("third_party/poco/include")
link_directories("${CMAKE_SOURCE_DIR}/third_party/poco/lib/${OS_TYPE}")
file(GLOB SOURCES "src/*.cc")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -O2 -std=c++11 -Wall ")

add_executable(ms_proxy ${SOURCES})
target_link_libraries(ms_proxy -lPocoFoundation -lPocoNet -lPocoUtil -lPocoData -lPocoDataMySQL -lPocoRedis)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/poco/lib/${OS_TYPE}/ DESTINATION ${CMAKE_SOURCE_DIR}/bin PATTERN "*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/loglib/lib/${OS_TYPE}/ DESTINATION ${CMAKE_SOURCE_DIR}/bin PATTERN "*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/lib/${OS_TYPE}/ DESTINATION ${CMAKE_SOURCE_DIR}/bin PATTERN "*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/conf/ DESTINATION ${CMAKE_SOURCE_DIR}/bin PATTERN "*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)


