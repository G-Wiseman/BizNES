#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "Bus.h"
#include "test_setup.h"

TEST_CASE_METHOD(TestSetup, "Bus Write Read") {
	bus->cpuWrite(0x0000, 0xff); 
	CHECK(bus->cpuRead(0x0000) == 0xff); 
	bus->cpuWrite(0x0000, 0xee); 
	CHECK(bus->cpuRead(0x0000) == 0xee);

	bus->cpuWrite(0x0001, 0xee);
	bus->cpuWrite(0x0002, 0x34); 
	CHECK(bus->cpuRead(0x0001) == 0xee); 
	CHECK(bus->cpuRead(0x0002) == 0x34);
}

TEST_CASE_METHOD(TestSetup, "Bus Load STRING") {
	bus->load_string("de ad be ef", 0x0000);
	CHECK(bus->cpuRead(0x0000) == 0xde);
	CHECK(bus->cpuRead(0x0001) == 0xad);
	CHECK(bus->cpuRead(0x0002) == 0xbe);
	CHECK(bus->cpuRead(0x0003) == 0xef);

	bus->load_string("12 34 56", 0x0000);
	CHECK(bus->cpuRead(0x0000) == 0x12);
	CHECK(bus->cpuRead(0x0001) == 0x34);
	CHECK(bus->cpuRead(0x0002) == 0x56);
	CHECK(bus->cpuRead(0x0003) == 0xef);
}