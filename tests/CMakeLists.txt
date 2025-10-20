find_package(Catch2 CONFIG REQUIRED)

add_library(TestSources "cpu_flag_tests.cpp" "test_setup.h" "cpu_logic_tests.cpp" "bus_tests.cpp" "test_setup.cpp")
target_link_libraries(TestSources PRIVATE Catch2::Catch2WithMain)
target_link_libraries(TestSources PRIVATE NES_Core)

add_executable(TestRunner "test.cpp"  )
target_link_libraries(TestRunner PRIVATE Catch2::Catch2WithMain NES_Core TestSources)


include(CTest)
include(Catch)