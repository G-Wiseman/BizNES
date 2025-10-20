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
