cmake_minimum_required (VERSION 2.6)


project (EMILY_functions)
include_directories(../libraries/ESP)
add_executable(esp_test ../libraries/ESP/test.cpp)
install(TARGETS esp_test DESTINATION bin)
