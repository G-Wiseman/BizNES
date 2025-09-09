#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "Bus.h"
#include "mos6502.h"
#include "test_setup.h"


TEST_CASE_METHOD(TestSetup, "AND Tests") {
	cpu->setPC(0x0000);
	SECTION("Immediate Addressing") {
		cpu->setAccumulator(0xf0);
		bus->cpuWrite(0, 0x29); bus->cpuWrite(1, 0x0f); // AND #15 (ox0f)
		cpu->step(); 
		REQUIRE(cpu->getAccumulator() == 0x00);
		CHECK(cpu->getFlag(mos6502::Z) == true);
		CHECK(cpu->getFlag(mos6502::N) == false);

		cpu->setAccumulator(0x0f);
		bus->cpuWrite(2, 0x29); bus->cpuWrite(3, 0x0f);
		cpu->step();
		REQUIRE(cpu->getAccumulator() == 0x0f);
		CHECK(cpu->getFlag(mos6502::Z) == false);
		CHECK(cpu->getFlag(mos6502::N) == false);

		cpu->setAccumulator(0xf0);
		bus->cpuWrite(4, 0x29); bus->cpuWrite(5, 0xff);
		cpu->step();
		REQUIRE(cpu->getAccumulator() == 0xf0);
		CHECK(cpu->getFlag(mos6502::Z) == false);
		CHECK(cpu->getFlag(mos6502::N) == true);
	}
}

TEST_CASE_METHOD(TestSetup, "EOR Tests") {
	cpu->setPC(0x0000);
	SECTION("Absolute Addressing") {
		bus->cpuWrite(0x0011, 0b10000001);
		bus->load_string("4D 11 00", 0x0000); // EOR $0011

		cpu->setAccumulator(0x0001);
		cpu->step(); 
		unsigned int res = static_cast<unsigned int>(cpu->getAccumulator());
		REQUIRE(res == 0b10000000);
		CHECK(cpu->getFlag(mos6502::Z) == false);
		CHECK(cpu->getFlag(mos6502::N) == true);
	}
}

TEST_CASE_METHOD(TestSetup, "ORA Tests") {
	cpu->setPC(0x0000);
	SECTION("Zeropage X") {
		bus->cpuWrite(0x0014, 0x10);
		bus->load_string("15 10", 0x0000); // EOR $0011
		cpu->setXReg(4);

		cpu->setAccumulator(0x0f);
		cpu->step();
		REQUIRE(cpu->getAccumulator() == 0x1f);
		REQUIRE(cpu->getFlag(mos6502::Z) == false);
		REQUIRE(cpu->getFlag(mos6502::N) == false);

	}
}

TEST_CASE_METHOD(TestSetup, "BIT Tests") {

}