#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

#include "ecs/componentpool.h"
#include "ecs/entity.h"

struct position_t {
  float x;
  float y;
};

struct collision_t {
  float x1;
  float x2;
  float y1;
  float y2;
};

struct sayer_t {
  char message[16];
};

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
  

  ecs::entity_t e0{0u};
  ecs::entity_t e1{1u};
  ecs::entity_t e2{2u};


  ecs::component_pool<position_t> positions;
  ecs::component_pool<collision_t> collisions;
  ecs::component_pool<sayer_t> sayers;


  auto& pos1 = positions.add(e0);
  pos1.x = 43;
  pos1.y = 91;

  auto& pos2 = positions.add(e1);
  pos2.x = 19;
  pos2.y = 192;

  auto& pos3 = positions.add(e2);
  pos3.x = 91;
  pos3.y = 252;

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

    std::cout << "--------------------------" << std::endl;
    for(auto const& p : positions) {
      std::cout << p.x << ", " << p.y << std::endl;
    }
    std::cout << "--------------------------" << std::endl;

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    } 
  }
  return 0;
}
