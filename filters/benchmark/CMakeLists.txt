add_executable(stochasticEKFBenchmark stochasticEKFBenchmark.cpp)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG main
)
FetchContent_MakeAvailable(benchmark)


target_link_libraries(stochasticEKFBenchmark
    benchmark::benchmark
    filters
)
