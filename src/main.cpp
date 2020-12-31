#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "ecs/types.h"
#include "ecs/entity_pool.h"
#include "ecs/component_store.h"

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

  acorn::ecs::entity_pool pool{};
  acorn::ecs::entity_t e0 = pool.create_entity();
  acorn::ecs::entity_t e1 = pool.create_entity();
  acorn::ecs::component_store components {};


  position_t pos{ .x = 200.0f, .y = 320.0f};
  sprite_t sprite{.texture = tex};

  components.insert_component(e0, pos);
  components.insert_component(e0, sprite);

  position_t pos2{.x = 25.0f, .y = 100.0f};
  components.insert_component(e1, pos2);
  components.insert_component(e1, sprite);

  auto const position_component = components.get_component<position_t>(e0);
  auto const position_component2 = components.get_component<position_t>(e1);
  auto const sprite_component = components.get_component<sprite_t>(e0);
  auto const sprite_component2 = components.get_component<sprite_t>(e1);
  point_value_t pvc { .value = 0 };
  int i = 1;
  while(1) {
    ++i;
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
    
    SDL_Rect pos_rect{.x =static_cast<int>(position_component.x), .y = static_cast<int>(position_component.y), .w = 150, .h = 150};
    SDL_Rect pos_rect2{.x =static_cast<int>(position_component2.x), .y = static_cast<int>(position_component2.y), .w = 50, .h = 50};
    if(components.has_component<point_value_t>(e0)) {
      if(i % 200 == 0) {
        components.remove_component<point_value_t>(e0);
      }
      SDL_RenderCopy(renderer, sprite_component.texture, nullptr, &pos_rect); 
    } else {
      if(i % 200 == 0) {
        components.insert_component(e0, pvc);
      }
    }
    SDL_RenderCopy(renderer, sprite_component2.texture, nullptr, &pos_rect2); 
    SDL_RenderPresent(renderer);
  } 
  return 0;
}
