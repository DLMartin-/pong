#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <SDL2/SDL.h>

int main(int argc, char** argv) {

  if(auto const r = SDL_Init(SDL_INIT_EVERYTHING); r != 0) {
    std::puts("Error: Could not initialize SDL2.");
    return 1;
  }

  auto const window = SDL_CreateWindow("Pongchan", 0, 0, 240, 350, 0);

  auto const renderer = SDL_CreateRenderer(window, -1, 0);

  if(!window || !renderer) {
    std::puts("Could not create window or renderer.");
    return 2;
  } 

  SDL_Event e{};
  while(1) {
  while(SDL_PollEvent(&e)) {
    if(e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == SDLK_ESCAPE) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    } 
  }
  return 0;
}
