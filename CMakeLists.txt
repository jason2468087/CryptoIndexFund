cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CryptoIndexFund VERSION 1.0)


find_package(cpr REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(websocketpp REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(CryptoIndexFund.exe main.cpp ${SRC_FILES})

target_link_libraries(CryptoIndexFund.exe PRIVATE cpr::cpr OpenSSL::SSL websocketpp::websocketpp ${JSONCPP_LIBRARIES})
target_include_directories(CryptoIndexFund.exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
