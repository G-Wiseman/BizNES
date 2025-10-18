#include "PPU.h"

PPU::PPU() {
	cartridge = nullptr;
}

PPU::PPU(Cartridge* cart) {
	cartridge = cart;
}

uint8_t PPU::read(uint16_t addr14bit) {
	addr14bit &= 0x3fff; // Limit it to 14 bits
	if (addr14bit >= 0 && addr14bit <= 0x2fff) {
		return cartridge->ppuRead(addr14bit);
	}
}

void PPU::write(uint16_t addr14bit, uint8_t value) {
	addr14bit &= 0x3fff; // Limit it to 14 bits
	if (addr14bit >= 0 && addr14bit <= 0x2fff) {
		cartridge->ppuWrite(addr14bit, value);
	}
}



uint8_t PPU::cpuRead(uint16_t addr) {
	addr = addr % 8; // deals with mirroring
	switch (addr) {
	case 0:
	case 1:
		break;

	case 2:
		return ppu_status_read();
		break;

	case 3:
		break;

	case 4:
		return oam_data_read();
		break;

	case 5:
	case 6:
		break;

	case 7:
		return ppu_data_read();
		break;
	}
	return 0xff;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
	addr = addr % 8; // deals with mirroring
	switch (addr) {
	case 0:
		ppu_ctrl_write(data);
		break;
	case 1:
		ppu_mask_write(data);
		break;

	case 2:
		break;

	case 3:
		oam_addr_write(data);
		break;

	case 4:
		oam_data_write(data);
		break;

	case 5:
		ppu_scroll_write(data);
		break;

	case 6: 
		ppu_addr_write(data);
		break;

	case 7:
		ppu_data_write(data);
		break;
	}
}

void PPU::clock() {
	if (scanline == -1) {
		// dummy scanline, but still needs to make memory calls, used to shift registers?
	}
	if (scanline >= 0 && scanline <= 239) {
		visible_scanline();
	}
	else if (scanline == 240) { /*Idles*/ }
	else if (scanline >= 241 && scanline <= 260) {
		// VBLANK flag is set at tick 1, the second tick of scanline 241. where the NMI also occurs. 
		// PPU makes no memory accesses during this period, so PPU memory can be freely accessed. 
	}

	// Now increment the scanlines and cycles appropriately
	cycle++;
	if (cycle >= 341) {
		cycle = 0;
		scanline++;
	}
	if (scanline >= 261) {
		scanline = -1;
	}
}


void PPU::visible_scanline() {
	if (cycle == 0) {
		// Idle cycle. PPU bus is same CHR address used later to fetch low bg tile. Does this matter for emulation? Probably not?
	}
	else if (cycle <= 256) {
		// data for tile is fetched during this phase. 
		// - nametable byte
		// - attribute table byte
		// - pattern table tile low
		// - pattern table tile high (+8 bytes from pattern table tile low)

		// also will need to output a pixel to the frame_buffer, and call 
	}
	else if (cycle <= 320) {

	}
	else if (cycle <= 336) {

	}
	else if (cycle <= 340) {

	}
}


void PPU::ppu_ctrl_write(uint8_t data) {

}
void PPU::ppu_mask_write(uint8_t data) {

}
uint8_t PPU::ppu_status_read() {
	return 0xff;
}
void PPU::oam_addr_write(uint8_t data) {

}
void PPU::oam_data_write(uint8_t data) {

}
uint8_t PPU::oam_data_read() {
	return 0xff;
}
void PPU::ppu_scroll_write(uint8_t data) {

}
void PPU::ppu_addr_write(uint8_t data) {

}
void PPU::ppu_data_write(uint8_t data) {

}
uint8_t PPU::ppu_data_read() {
	return 0xff;
}
