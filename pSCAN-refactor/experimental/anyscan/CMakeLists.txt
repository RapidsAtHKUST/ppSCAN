set(ANY_SCAN_FILES AnySCANGraphSerial.cpp AnySCANGraphSerial.h main_exp_any_scan_serial.cpp ../../InputOutput.cpp)
add_executable(anySCANSerial ${ANY_SCAN_FILES})
target_compile_options(anySCANSerial PRIVATE -O3 -g)