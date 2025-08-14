#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Bus.h"
#include "mos6502.h"

TEST_CASE("Dummy test for Bus class") {
    Bus bus;
    // Example check - replace with your real test logic
    REQUIRE(true);  // This test always passes
}

TEST_CASE("Basic Get/Set Flags") {
    Bus bus = Bus();
    mos6502 cpu = mos6502(&bus);
    // Example dummy check: verify reset sets PC to 0 (or whatever your logic is)
    REQUIRE(1 == 0);  // Adjust if your Reset sets PC differently
}