cmake_minimum_required(VERSION 3.5)
message("CMAKE VERSION: " "${CMAKE_VERSIONS}")

project (TimeSeries)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Project wide CXX flags
string(CONCAT CXX_IGNORES
    "-Wno-unknown-warning-option "
    # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105329
    "-Wno-restrict "
    # std::optional is triggering this a lot
    "-Wno-maybe-uninitialized "
    # imgui
    "-Wno-format-security "
    # GCC is overzealous
    "-Wno-dangling-reference "
    # The usual things I don't worry about
    "-Wno-unused-variable "
    "-Wno-unused-parameter "
    "-Wno-unused-but-set-variable "
    "-Wno-unused-private-field "
    "-Wno-unused-function "
    "-Wno-deprecated-enum-enum-conversion ")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)

find_package(ALGLIB REQUIRED)

find_package(Boost REQUIRED)

# --- GOOGLETEST --- #
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

# --- GOOGLE BENCHMARK --- #
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG main
)
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(benchmark)

# --- Eigen --- #
FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(eigen)

# --- libfixmath --- #
FetchContent_Declare(
    libfixmath
    GIT_REPOSITORY https://github.com/PetteriAimonen/libfixmath.git
    GIT_TAG master
)
FetchContent_MakeAvailable(libfixmath)

# --- project directories --- # 
include_directories(".")

list(APPEND SOURCE_DIRECTORIES
    "common"
    "fakeMarket"
    "filters"
    "stochasticModels"
    "timer"
)
foreach (DIR IN LISTS SOURCE_DIRECTORIES)
    add_subdirectory(${DIR})
endforeach()

add_executable(Main main.cpp)
target_link_libraries(Main PRIVATE Eigen3::Eigen)
