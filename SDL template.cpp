    // SDL template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDL.h"
#include "TickRateMadeEasy.h"
#include "isometricProjection.h"
#include "Vector2.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

    int screenX = pow(2, 10);
    int screenY = pow(2, 9);

    SDL_Window* window;
    SDL_Renderer* renderer;
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1; // Exit with error code
        }

        window = SDL_CreateWindow(
            "My SDL Window",               // Window title
            SDL_WINDOWPOS_CENTERED,        // X position, centered
            SDL_WINDOWPOS_CENTERED,        // Y position, centered
            screenX,                           // Width
            screenY,                           // Height
            SDL_WINDOW_SHOWN                // Window flags
        );

        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit(); // Clean up SDL
            return 1; // Exit with error code
        }

        renderer = SDL_CreateRenderer(
            window,              // Window to associate with
            -1,                  // Use the first rendering driver
            SDL_RENDERER_ACCELERATED // Renderer flags
        );

        if (renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit(); // Clean up SDL
            return 1; // Exit with error code
        }
    }

    vertMap map(pow(2,5), pow(2, 4));
    map.generateHeights(4);
   // map.printRawData();
    SDL_Texture* texture = map.generateTexture(renderer);
    std::cout << "done!\n";

    bool running = true;

    SDL_Event event;

    const int tickRate = 60;

    const auto tickInterval = TRME_getTickInterval(tickRate);

    while (running) {
        auto startTime = TRME_getTimePoint();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
            }
            // Add more event handling as needed (e.g., keyboard input, mouse clicks)
        }

        SDL_RenderClear(renderer);
        
        // rendering stuff here
        {
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        }
        SDL_RenderPresent(renderer);
        TRME_sleepUntilNextTick(startTime, tickInterval);
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
// 
// Drop SDL2-2.30.3 in the same directory as the SLN
// 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
