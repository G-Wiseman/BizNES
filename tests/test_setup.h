#pragma once
#include "Bus.h"
#include "mos6502.h"
#include "Cartridge.h"
#include "PPU.h"
#include <memory>

class TestSetup
{
public:
	TestSetup();
	std::unique_ptr<mos6502> cpu;
	std::unique_ptr<Bus> bus;
	std::unique_ptr<Cartridge> cart;
	std::unique_ptr<PPU> ppu;

private:

};

TestSetup::TestSetup() {
	bus = std::make_unique<Bus>();
	cpu = std::make_unique<mos6502>(bus.get());
	cart = std::make_unique<Cartridge>();
	ppu = std::make_unique<PPU>(cart.get());

	bus->connect_cartridge(cart.get());
	bus->connect_ppu(ppu.get());
	bus->connect_cpu(cpu.get());
}
