#include "test_setup.h"

TestSetup::TestSetup() {
	bus = std::make_unique<Bus>();
	cpu = std::make_unique<mos6502>(bus.get());
	cart = std::make_unique<Cartridge>();
	ppu = std::make_unique<PPU>(cart.get());

	bus->connect_cartridge(cart.get());
	bus->connect_ppu(ppu.get());
	bus->connect_cpu(cpu.get());
}