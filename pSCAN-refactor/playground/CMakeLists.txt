# playgound 0, playgound 1
add_executable(other_play ./play.cpp)
add_executable(io_play ./graph_io.cpp)
add_executable(play_union_find_set ./play_thread_safe_union_find_set.cpp ThreadSafeDisjointSet.h)
add_executable(play2 play2.cpp)

# utility 1: binary_file_checker, check b_degree.bin, b_adj.bin
add_executable(binary_file_checker ./check_binary_files.cpp ../InputOutput.cpp)
target_compile_options(binary_file_checker PRIVATE -O3 -g)
target_compile_definitions(binary_file_checker PRIVATE WITHGPERFTOOLS=1)
target_link_libraries(binary_file_checker profiler)