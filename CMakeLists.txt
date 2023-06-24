include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
)

FetchContent_MakeAvailable(asio)

cmake_minimum_required(VERSION 3.20)
project(cpp_tasks_bench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

FetchContent_Declare(
  concurrencpp
  GIT_REPOSITORY https://github.com/David-Haim/concurrencpp.git
)

FetchContent_MakeAvailable(concurrencpp)

add_executable(asioexe asio_bench/main.cpp)
target_include_directories(asioexe PRIVATE ${asio_SOURCE_DIR}/asio/include)

add_executable(concurrencppexe concurrencpp_bench/main.cpp)
target_link_libraries(concurrencppexe PRIVATE concurrencpp)

set(UNIFEX_NO_LIBURING ON CACHE BOOL "Turn off if your system has liburing" FORCE)
set(UNIFEX_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(BUILD_TESTING OFF CACHE BOOL " " FORCE)

FetchContent_Declare(
  libunifex
  GIT_REPOSITORY https://github.com/facebookexperimental/libunifex.git
  GIT_TAG main
)

FetchContent_MakeAvailable(libunifex)

add_executable(unifexexe unifex_bench/main.cpp)
target_include_directories(unifexexe PRIVATE ${libunifex_SOURCE_DIR}/include ${libunifex_BINARY_DIR}/include)
target_link_libraries(unifexexe PRIVATE unifex)

execute_process(COMMAND cargo build --release 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tokio_bench)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tokio_bench/target/release/tokio_bench)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tokio_bench/target/release/tokio_bench
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
