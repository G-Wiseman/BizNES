// NES_Emulator.cpp : Defines the entry point for the application.
//
#include "NES_Emulator.h"
using namespace std;

SDL_Window* window;
SDL_Renderer* renderer; 
TTF_Font* font; 


SDL_Color black = { 0,   0,   0,   255 };
SDL_Color white = { 255, 255, 255, 255 };
SDL_Color red = { 255, 0,   0,   255 };
SDL_Color green = { 0,   255, 0,   255 };
SDL_Color blue = { 130, 140, 170, 255 };
SDL_Color yellow = { 255, 255, 0,   255 };
SDL_Color cyan = { 0,   255, 255, 255 };
SDL_Color magenta = { 255, 0,   255, 255 };
SDL_Color orange = { 255, 165, 0,   255 };
SDL_Color purple = { 128, 0,   128, 255 };
SDL_Color gray = { 128, 128, 128, 255 };
SDL_Color brown = { 165, 42,  42,  255 };

int main()
{
    Bus bus;
    mos6502 cpu = mos6502(&bus);
    Cartridge nestest = Cartridge();
    nestest.load_from_ines("./assets/ROMs/nestest.nes");
    
    bus.connect_cpu(&cpu);
    bus.connect_cartridge(&nestest);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cout << "error with init " << endl;
        printf("Error with init %s \n", SDL_GetError());
        exit(1); 
    }
    if (!TTF_Init()) {
        cout << "error with init ttf" << endl; 
        exit(1); 
    }
    window = SDL_CreateWindow("Window Name", 1600, 900, 0);
    renderer = SDL_CreateRenderer(window, NULL);

    TTF_Font* font = TTF_OpenFont("./assets/fonts/Ticketing.ttf", 32);
    if (font == nullptr) {
        cout << "couldn't load font" << endl; 
        exit(1);
    }
    int x = 0;
    TTF_TextEngine* text_engine = TTF_CreateRendererTextEngine(renderer);

    
    bool running = true; 
    bool stepping = false;
    cpu.setPC(0x0C000);
    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set draw color to black (R=0, G=0, B=0, A=255)
        SDL_RenderClear(renderer);  // Clear the screen with the draw color
        SDL_Event event; 
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT: 
                running = false; 
                break;

            case SDL_EVENT_KEY_DOWN:
                stepping = !stepping;
            }
        }

        if (stepping) {
            cpu.step();
        }

        renderRamGrid(renderer, cpu, bus, 0x00000, font, 80, 5);
        renderCPUState(renderer, cpu, bus, font, 900, 30);
        SDL_RenderPresent(renderer); // this is the buffer swap 
        
    }
    TTF_DestroyRendererTextEngine(text_engine);
    SDL_DestroyWindow(window); 
    TTF_Quit();
    SDL_Quit(); 
    return 0; 
}

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dest = { x, y, surface->w, surface->h };
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}


void renderRamGrid(SDL_Renderer* renderer, mos6502& cpu, Bus& bus, const uint16_t start_addr, TTF_Font* font, int x, int y) {
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

        renderText(renderer, addrSS.str(), x + 0, y + row * cellSize + row_offset, white, font);

        // --- Render each byte in the row ---
        for (int col = 0; col < cols; ++col) {
            uint16_t addr = addrLabel + col;
            uint8_t value = bus.cpuRead(addr);

            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);

            int drawX = x + (col)*cellSize + col_offset;
            int drawY = y + row * cellSize + row_offset;
            renderText(renderer, ss.str(), drawX, drawY, white, font);
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


void renderCPUState(SDL_Renderer* renderer, mos6502& cpu, Bus& bus, TTF_Font* font, int x, int y) {
    const int cellSize = 50;
    int row = 0; 
    std::stringstream ss; 
    ss << "PC: $" << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(cpu.getPC());
    renderText(renderer, ss.str(), x, y + row * cellSize, magenta, font);

    row++;
    ss.str("");
    ss << "A: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu.getAccumulator());
    renderText(renderer, ss.str(), x, y + row*cellSize, red, font);

    row++;
    ss.str("");
    ss << "X: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu.getXReg());
    renderText(renderer, ss.str(), x, y + row * cellSize, blue, font);

    row++;
    ss.str("");
    ss << "Y: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cpu.getYReg());
    renderText(renderer, ss.str(), x, y + row * cellSize, yellow, font);


    // N V - B D I Z C
    row++; 
    int col = 0;

    renderText(renderer, "N", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::N) ? green : red, font);
    renderText(renderer, "V", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::V) ? green : red, font);
    renderText(renderer, "-", x + col++ * cellSize, y + row * cellSize, gray, font);
    renderText(renderer, "B", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::B) ? green : red, font);
    renderText(renderer, "D", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::D) ? green : red, font);
    renderText(renderer, "I", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::I) ? green : red, font);
    renderText(renderer, "Z", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::Z) ? green : red, font);
    renderText(renderer, "C", x + col++ * cellSize, y + row * cellSize, cpu.getFlag(mos6502::STATUS_FLAG_MASKS::C) ? green : red, font);
    

}
