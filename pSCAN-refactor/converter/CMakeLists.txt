# utility 0: converter, transform edge list txt file into b_degree.bin, b_adj.bin
add_executable(converter ./EdgeListToBinsConverter.cpp)
target_compile_options(converter PRIVATE -O3 -g)
#target_compile_definitions(converter PRIVATE WITHGPERFTOOLS=1)
#target_link_libraries(converter profiler)

add_executable(converterAnySCAN BinToAnySCANFormatConverter.cpp ../InputOutput.cpp)
target_compile_options(converterAnySCAN PRIVATE -O3 -g)
