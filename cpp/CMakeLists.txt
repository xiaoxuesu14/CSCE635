cmake_minimum_required (VERSION 2.6)

project (EMILY_functions)
include_directories(../arduino/libraries/ESP)
add_executable(esp ../testFunctions/esp_test.cpp)

add_executable(esp_parse ../testFunctions/esp_parse_bytes_test.cpp)

include_directories(../arduino/libraries/emilyStatus)
add_executable(status_test ../testFunctions/status_test.cpp ../arduino/libraries/emilyStatus/emilyStatus.cpp)
