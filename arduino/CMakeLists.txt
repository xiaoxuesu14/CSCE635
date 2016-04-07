cmake_minimum_required (VERSION 2.6)


project (EMILY_functions)
include_directories(../libraries/ESP)
add_executable(esp_test ../libraries/ESP/test.cpp)
install(TARGETS esp_test DESTINATION bin)

add_executable(esp_test_parse ../libraries/ESP/test_parse_byte.cpp)
install(TARGETS esp_test_parse DESTINATION bin)

include_directories(../libraries/emilyStatus)
add_executable(status_test ../libraries/emilyStatus/test.cpp ../libraries/emilyStatus/emilyStatus.cpp)
install(TARGETS status_test DESTINATION bin)
