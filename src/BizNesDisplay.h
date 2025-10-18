#pragma once
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>  
#include <iomanip>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include "Bus.h"
#include "mos6502.h"
#include "PPU.h"
#include "Cartridge.h"

class BizNesDisplay
{
public:
	BizNesDisplay();
	~BizNesDisplay();

public: // NES components
	std::unique_ptr<mos6502> cpu;
	std::unique_ptr<Bus> bus;
	std::unique_ptr<Cartridge> cart;
	std::unique_ptr<PPU> ppu;

public: // SDL components
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_TextEngine* text_engine = nullptr;
	TTF_Font* font = nullptr;


public: // starts up the main loop for the display
	void start();

private: // internal variables
	bool running = false;

public: // render various components of the display
	void renderCPUState(int x, int y);
	void renderPPU(int x, int y, int scale);
	void renderRAM(int x, int y);
	void renderText(const std::string& text, int x, int y, SDL_Color color);

private: // SDL Colors for Text
	inline static const SDL_Color black = { 0,   0,   0,   255 };
	inline static const SDL_Color white = { 255, 255, 255, 255 };
	inline static const SDL_Color red = { 255, 0,   0,   255 };
	inline static const SDL_Color green = { 0,   255, 0,   255 };
	inline static const SDL_Color blue = { 130, 140, 170, 255 };
	inline static const SDL_Color yellow = { 255, 255, 0,   255 };
	inline static const SDL_Color cyan = { 0,   255, 255, 255 };
	inline static const SDL_Color magenta = { 255, 0,   255, 255 };
	inline static const SDL_Color orange = { 255, 165, 0,   255 };
	inline static const SDL_Color purple = { 128, 0,   128, 255 };
	inline static const SDL_Color gray = { 128, 128, 128, 255 };
	inline static const SDL_Color brown = { 165, 42,  42,  255 };
};