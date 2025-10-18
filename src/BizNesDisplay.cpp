#include "BizNesDisplay.h"

BizNesDisplay::BizNesDisplay() {
	bus = std::make_unique<Bus>();
	cpu = std::make_unique<mos6502>(bus.get());
	cart = std::make_unique<Cartridge>();
	cart->load_from_ines("./assets/ROMs/nestest.nes");
	ppu = std::make_unique<PPU>(cart.get());

	bus->connect_cartridge(cart.get());
	bus->connect_ppu(ppu.get());
	bus->connect_cpu(cpu.get());

	
	
}

BizNesDisplay::~BizNesDisplay() {
	TTF_DestroyRendererTextEngine(text_engine);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

void BizNesDisplay::start() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "error with init " << std::endl;
		printf("Error with init %s \n", SDL_GetError());
		exit(1);
	}
	if (!TTF_Init()) {
		std::cout << "error with init ttf" << std::endl;
		exit(1);
	}

	font = TTF_OpenFont("./assets/fonts/Ticketing.ttf", 32);
	if (font == nullptr) {
		std::cout << "couldn't load font" << std::endl;
		exit(1);
	}
	
	window = SDL_CreateWindow("Window Name", 1920, 1080, 0);
	renderer = SDL_CreateRenderer(window, NULL);
	text_engine = TTF_CreateRendererTextEngine(renderer);
	running = true;
	while (running) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set draw color to black (R=0, G=0, B=0, A=255)
		SDL_RenderClear(renderer);  // Clear the screen with the draw color
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			}
		}
		renderCPUState(1100, 0);
		renderRAM(1100, 400);
		renderPPU(38, 60, 4);
		SDL_RenderPresent(renderer);
	}
}

void BizNesDisplay::renderRAM(int x, int y) {
	uint16_t start_addr = 0;
	const int cellSize = 35;
    const int cols = 16;
    const int rows = 16;
    const int row_offset = cellSize;
    const int col_offset = int(cellSize * 5);

    SDL_Color textColor = { 255, 255, 255, 255 };

    for (int row = 0; row < rows; ++row) {
        // --- Render address label on the left ---
        uint16_t addrLabel = start_addr + row * cols;
        std::stringstream addrSS;
        addrSS << "$" << std::hex << std::setw(4) << std::setfill('0') << addrLabel;

        renderText(addrSS.str(), x + 0, y + row * cellSize + row_offset, white);

        // --- Render each byte in the row ---
        for (int col = 0; col < cols; ++col) {
            uint16_t addr = addrLabel + col;
            uint8_t value = bus->cpuRead(addr);

            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);

            int drawX = x + (col)*cellSize + col_offset;
            int drawY = y + row * cellSize + row_offset;
            renderText(ss.str(), drawX, drawY, white);
        }
    }

    // Calculate border rectangle dimensions
    // Width: address label width + (cols * cellSize)
    // Height: (rows * cellSize) + row_offset (for vertical offset of labels)
    int borderX = x;
    int borderY = y;
    int borderWidth = col_offset + cols * cellSize;
    int borderHeight = rows * cellSize + row_offset;

    SDL_FRect borderRect = { borderX, borderY, borderWidth, borderHeight };

    // Set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the border rectangle
    SDL_RenderRect(renderer, &borderRect);
}

void BizNesDisplay::renderPPU(int x, int y, int scale) {
	SDL_FRect ScreenRect = { x, y, 256 * scale, 240 * scale};
	for (size_t x_i = 0; x_i < 256; x_i++)
	{
		for (size_t y_i = 0; y_i < 240; y_i++)
		{
			SDL_FRect pixel = { x + x_i * scale, y + y_i *scale, scale , scale };
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			if (x_i % 4 != y_i % 8) {
				SDL_SetRenderDrawColor(renderer, 10, 140, 140, 255);
			}
			SDL_RenderFillRect(renderer, &pixel);
		}
	}
}

void BizNesDisplay::renderCPUState(int x, int y) {
	const int cellSize = 50;
	int row = 0; 
	std::stringstream ss; 
	ss << "PC: $" << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(cpu->getPC());
	renderText(ss.str(), x, y + row * cellSize, magenta);
	
	row++;
	ss.str("");
	ss << "A: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu->getAccumulator());
	renderText(ss.str(), x, y + row*cellSize, red);
	
	row++;
	ss.str("");
	ss << "X: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu->getXReg());
	renderText(ss.str(), x, y + row * cellSize, blue);
	
	row++;
	ss.str("");
	ss << "Y: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu->getYReg());
	renderText(ss.str(), x, y + row * cellSize, yellow);
	
	
	// N V - B D I Z C
	row++; 
	int col = 0;
	renderText("N", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::N) ? green : red);
	renderText("V", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::V) ? green : red);
	renderText("-", x + col++ * cellSize, y + row * cellSize, gray);
	renderText("B", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::B) ? green : red);
	renderText("D", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::D) ? green : red);
	renderText("I", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::I) ? green : red);
	renderText("Z", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::Z) ? green : red);
	renderText("C", x + col++ * cellSize, y + row * cellSize, cpu->getFlag(mos6502::STATUS_FLAG_MASKS::C) ? green : red);
	
}

void BizNesDisplay::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dest = { x, y, surface->w, surface->h };
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}