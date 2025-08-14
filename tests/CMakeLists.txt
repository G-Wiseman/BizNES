find_package(Catch2 CONFIG REQUIRED)

add_executable(TestRunner "cpu_unit_tests.cpp")
target_link_libraries(TestRunner PRIVATE NES_core Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(TestRunner)