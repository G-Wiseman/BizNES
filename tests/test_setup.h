#pragma once
#include "Bus.h"
#include "mos6502.h"
#include <memory>

class TestSetup
{
public:
	TestSetup();
	std::unique_ptr<mos6502> cpu;
	std::unique_ptr<Bus> bus;

private:

};

TestSetup::TestSetup() {
	bus = std::make_unique<Bus>(); 
	cpu = std::make_unique<mos6502>(bus.get());
	bus->connect_cpu(cpu.get());
}
