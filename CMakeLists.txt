cmake_minimum_required(VERSION 3.19)
message("CMAKE VERSION: " "${CMAKE_VERSIONS}")

project (TimeSeries)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

find_package(ALGLIB REQUIRED)

# --- GOOGLETEST --- #
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# --- Eigen --- #
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Eigen)

# --- project directories --- # 
include_directories(".")

list(APPEND SOURCE_DIRECTORIES
    "common"
    "filters"
    "stochasticModels"
)
foreach (DIR IN LISTS SOURCE_DIRECTORIES)
    add_subdirectory(${DIR})
endforeach()

add_executable(Main main.cpp)
target_link_libraries(Main PRIVATE Eigen3::Eigen)
