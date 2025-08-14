// NES_Emulator.cpp : Defines the entry point for the application.
//
#include "NES_Emulator.h"
#include "Bus.h"
#include "mos6502.h"
#include <SDL3_ttf/SDL_ttf.h> 
#include <string>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer; 
TTF_Font* font; 

int main()
{
    Bus bus = Bus();
    mos6502 cpu = mos6502(&bus);
    for (size_t i = 0; i <= 0xff; i++) {
        
    }
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cout << "error with init " << endl;
        printf("Error with init %s \n", SDL_GetError());
        exit(1); 
    }
    if (!TTF_Init()) {
        cout << "error with init ttf" << endl; 
        exit(1); 
    }


    window = SDL_CreateWindow("Window Name", 600, 400, 0);
    renderer = SDL_CreateRenderer(window, NULL);

    TTF_Font* font = TTF_OpenFont("./assets/fonts/Ticketing.ttf", 32);
    if (font == nullptr) {
        cout << "couldn't load font" << endl; 
        exit(1);
    }

    

    SDL_FRect rectan; 
    rectan.x = 10;
    rectan.y = 10;
    rectan.w = 50;
    rectan.h = 50;
    int x = 0;
    TTF_TextEngine* text_engine = TTF_CreateRendererTextEngine(renderer);


    bool running = true; 
    while (running) {
        SDL_Event event; 
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT: 
                running = false; 
                break;

            case SDL_EVENT_KEY_DOWN:
                std::cout << "The " << char(event.key.key) << " key was pressed." << endl;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff); 
        SDL_RenderClear(renderer);
        x++;

        TTF_Text* text = TTF_CreateText(text_engine, font, to_string(x).c_str(), 0);
        TTF_SetTextColorFloat(text, 50, 50, 50, 0xff);
        TTF_DrawRendererText(text, 20, 20); 

        SDL_RenderPresent(renderer);
        TTF_DestroyText(text); 
    }
    TTF_DestroyRendererTextEngine(text_engine);
    SDL_DestroyWindow(window); 
    TTF_Quit();
    SDL_Quit(); 
    return 0; 
}
