// NES_Emulator.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>  
#include <iomanip>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include "mos6502.h"
#include "Bus.h"
#include "Cartridge.h"


void renderRamGrid(SDL_Renderer* renderer, mos6502& cpu, Bus& bus, const uint16_t start_addr, TTF_Font* font, int x, int y);
void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color, TTF_Font* font);
void renderCPUState(SDL_Renderer* renderer, mos6502& cpu, Bus& bus, TTF_Font* font, int x, int y);

// TODO: Reference additional headers your program requires here.
