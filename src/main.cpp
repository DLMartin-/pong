#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

struct position_t {
  float x;
  float y;
};

struct physics_body_t {
  struct bounding_box_t {
    float x0;
    float y0;
    float x1;
    float y1;
  };
  bounding_box_t bbox;
  float velocity_x;
  float velocity_y;
};

struct point_value_t {
  unsigned int value;
};

struct sprite_t {
  SDL_Texture* texture;
};


int main(int argc, char** argv) {

  if(auto const r = SDL_Init(SDL_INIT_EVERYTHING); r != 0) {
    std::puts("Error: Could not initialize SDL2.");
    return 1;
  }

  auto const window = SDL_CreateWindow("Pongchan", 0, 0, 700, 1440, 0);

  auto const renderer = SDL_CreateRenderer(window, -1, 0);

  if(!window || !renderer) {
    std::puts("Could not create window or renderer.");
    return 2;
  } 

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    std::cout << "Hello" << std::endl;
    std::cout << IMG_GetError() << std::endl;
  }


  SDL_Texture* tex = IMG_LoadTexture(renderer, "./res/ball.png");

  if(!tex) {
    std::cout << IMG_GetError() << std::endl;
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
    }
    SDL_RenderClear(renderer);
    
    SDL_RenderPresent(renderer);
  } 
  return 0;
}
