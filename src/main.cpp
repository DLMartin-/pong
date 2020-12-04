#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include "ecs/component_cache.h"
#include "ecs/entity.h"
#include "ecs/entity_factory.h"
#include <SDL2/SDL_image.h>

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

  auto pos1 = position_t{.x = 15.0f, .y=190.0f};
  auto pos2 = position_t{.x = 91.0f, .y=11.0f};
  auto pos3 = position_t{.x = 201.0f, .y=92.0f};

  positions.insert(e0, pos1);
  positions.insert(e1, pos2);
  positions.insert(e2, pos3);
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

    SDL_RenderClear(renderer);

    SDL_Rect rect {};
    rect.x = pos1.x;
    rect.y = pos1.y;
    rect.h = 32;
    rect.w = 32;
    SDL_RenderCopy(renderer, tex, &rect, nullptr);

    SDL_RenderPresent(renderer);
    } 
  }
  return 0;
}
