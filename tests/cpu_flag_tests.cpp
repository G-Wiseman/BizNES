#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "Bus.h"
#include "mos6502.h"
#include "test_setup.h"


TEST_CASE_METHOD(TestSetup, "Accumulator Getters/Setters") {
    cpu->setAccumulator(0xaa); 
    REQUIRE(cpu->getAccumulator() == 0xaa);

    cpu->setAccumulator(0xbb);
    REQUIRE(cpu->getAccumulator() == 0xbb);
}

TEST_CASE_METHOD(TestSetup, "Program Counter Getters/Setters") {
    cpu->setPC(0x1234);
    REQUIRE(cpu->getPC() == 0x1234);

    cpu->setPC(0xABCD);
    REQUIRE(cpu->getPC() == 0xABCD);
}

TEST_CASE_METHOD(TestSetup,"X Register Getters/Setters") {
    cpu->setXReg(0x42);
    REQUIRE(cpu->getXReg() == 0x42);

    cpu->setXReg(0x99);
    REQUIRE(cpu->getXReg() == 0x99);
}

TEST_CASE_METHOD(TestSetup,"Y Register Getters/Setters") {
    cpu->setYReg(0x01);
    REQUIRE(cpu->getYReg() == 0x01);

    cpu->setYReg(0xFF);
    REQUIRE(cpu->getYReg() == 0xFF);
}

TEST_CASE_METHOD(TestSetup,"Status Flag Set and Get") {
    cpu->setFlag(mos6502::C, true);
    REQUIRE(cpu->getFlag(mos6502::C) == true);

    cpu->setFlag(mos6502::Z, true);
    REQUIRE(cpu->getFlag(mos6502::Z) == true);

    cpu->setFlag(mos6502::I, true);
    REQUIRE(cpu->getFlag(mos6502::I) == true);

    cpu->setFlag(mos6502::D, true);
    REQUIRE(cpu->getFlag(mos6502::D) == true);

    cpu->setFlag(mos6502::B, true);
    REQUIRE(cpu->getFlag(mos6502::B) == true);

    cpu->setFlag(mos6502::U, true);
    REQUIRE(cpu->getFlag(mos6502::U) == true);

    cpu->setFlag(mos6502::V, true);
    REQUIRE(cpu->getFlag(mos6502::V) == true);

    cpu->setFlag(mos6502::N, true);
    REQUIRE(cpu->getFlag(mos6502::N) == true);

    // Now unset them again and check
    cpu->setFlag(mos6502::C, false);
    REQUIRE(cpu->getFlag(mos6502::C) == false);

    cpu->setFlag(mos6502::Z, false);
    REQUIRE(cpu->getFlag(mos6502::Z) == false);

    cpu->setFlag(mos6502::I, false);
    REQUIRE(cpu->getFlag(mos6502::I) == false);

    cpu->setFlag(mos6502::D, false);
    REQUIRE(cpu->getFlag(mos6502::D) == false);

    cpu->setFlag(mos6502::B, false);
    REQUIRE(cpu->getFlag(mos6502::B) == false);

    cpu->setFlag(mos6502::U, false);
    REQUIRE(cpu->getFlag(mos6502::U) == false);

    cpu->setFlag(mos6502::V, false);
    REQUIRE(cpu->getFlag(mos6502::V) == false);

    cpu->setFlag(mos6502::N, false);
    REQUIRE(cpu->getFlag(mos6502::N) == false);
}


TEST_CASE_METHOD(TestSetup, "SEI/CLI Test") {
    cpu->setPC(0x0000);
    bus->write(0, 0x78); // SEC
    bus->write(1, 0x58); // CLC


    cpu->step();
    REQUIRE(cpu->getFlag(mos6502::STATUS_FLAG_MASKS::I));

    cpu->step();
    REQUIRE(~cpu->getFlag(mos6502::STATUS_FLAG_MASKS::I));
}


TEST_CASE_METHOD(TestSetup, "SEC/CLC Test") {
    cpu->setPC(0x0000);
    bus->write(0, 0x38); // SEC
    bus->write(1, 0x18); // CLC


    cpu->step();
    REQUIRE(cpu->getFlag(mos6502::STATUS_FLAG_MASKS::C));

    cpu->step();
    REQUIRE(~cpu->getFlag(mos6502::STATUS_FLAG_MASKS::C));
}

TEST_CASE_METHOD(TestSetup, "SED/CLD Test") {
    cpu->setPC(0x0000);
    bus->write(0, 0xF8); // SEC
    bus->write(1, 0xD8); // CLC
    
    
    cpu->step();
    REQUIRE(cpu->getFlag(mos6502::STATUS_FLAG_MASKS::D));

    cpu->step();
    REQUIRE(~cpu->getFlag(mos6502::STATUS_FLAG_MASKS::D));
}

