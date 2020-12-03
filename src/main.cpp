#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

#include "ecs/component_cache.h"
#include "ecs/entity.h"
#include "ecs/entity_factory.h"

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
  
  ecs::entity_factory entity_factory{};

  auto const e0 = entity_factory.generate();
  auto const e1 = entity_factory.generate();
  auto const e2 = entity_factory.generate();

  ecs::component_cache<position_t> positions;
  ecs::component_cache<collision_t> collisions;
  ecs::component_cache<sayer_t> sayers;

  auto pos1 = position_t{.x = 15.0f, .y=190.0f};
  auto pos2 = position_t{.x = 91.0f, .y=11.0f};
  auto pos3 = position_t{.x = 481.0f, .y=592.0f};

  positions.insert(e0, pos1);
  positions.insert(e1, pos2);
  positions.insert(e2, pos3);

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
    //for(auto const& p : positions) {
    //  std::cout << p.x << ", " << p.y << std::endl;
    //}
    std::cout << positions.get(e0).x << ", " << positions.get(e0).y << std::endl;
    std::cout << positions.get(e1).x << ", " << positions.get(e1).y << std::endl;
    std::cout << positions.get(e2).x << ", " << positions.get(e2).y << std::endl;
    std::cout << "--------------------------" << std::endl;

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    } 
  }
  return 0;
}
