#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include "ecs/component_cache.h"
#include "ecs/entity.h"
#include "ecs/entity_factory.h"
#include <SDL2/SDL_image.h>
#include <vector>

#include "ecs/world.h"

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


void check_edge_of_screen_collisions(const std::vector<ecs::entity_t>& entities, const ecs::component_cache<position_t>& positions, const ecs::component_cache<physics_body_t>& physics_bodies) {
  for(auto const& e : entities) {
    auto const& pos = positions[e];
    if(pos.x <= 0.0f ||pos.x >= 700) {
      //collision with screen edge
      auto& pbody = physics_bodies[e];
      pbody.velocity_x = -pbody.velocity_x;
    }

    if(pos.y <= 0.0f || pos.y >= 1440) {
      auto& pbody = physics_bodies[e];
      pbody.velocity_y = -pbody.velocity_y;
    }
  }
}

void apply_physics(const std::vector<ecs::entity_t>& entities, const ecs::component_cache<position_t>& positions, const ecs::component_cache<physics_body_t>& physics_bodies, float dt) {
  for(auto const& e : entities) {
    auto& pos = positions[e];
    auto const& pbody = physics_bodies[e];

    pos.x += (pbody.velocity_x * dt);
    pos.y += (pbody.velocity_y * dt);
  }
}

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
  
  ecs::entity_factory entity_factory{};
  ecs::component_cache<position_t> positions;
  ecs::component_cache<physics_body_t> pbodies;

  auto const e0 = entity_factory.generate();
  auto pos = position_t{.x = 15.0f, .y=190.0f};
  positions[e0] = pos;

  auto pbody = physics_body_t{.bbox = {.x0 = 0.0f, .y0=0.0f, .x1 = 32.0f, .y1 = 32.0f}, .velocity_x = 4.3f, .velocity_y = 3.1f};
  pbodies[e0] = pbody;

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    std::cout << "Hello" << std::endl;
    std::cout << IMG_GetError() << std::endl;
  }


  SDL_Texture* tex = IMG_LoadTexture(renderer, "./res/ball.png");

  if(!tex) {
    std::cout << IMG_GetError() << std::endl;
  }

  ecs::world w{};
  auto en = w.create_entity();

  SDL_Event e{};
  while(1) {

    auto draw_func = [e0, &positions, &pbodies, renderer, tex] {
      auto const& pos = positions[e0];
      auto const& pbody = pbodies[e0];
 
      SDL_Rect screen_rect {.x = static_cast<int>(pos.x), .y = static_cast<int>(pos.y), .w = static_cast<int>(pbody.bbox.x1), .h =static_cast<int>( pbody.bbox.y1)};
      SDL_RenderCopy(renderer, tex, nullptr, &screen_rect);
    };
  
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
    apply_physics({e0}, positions, pbodies, 1.0f);
    check_edge_of_screen_collisions({e0}, positions, pbodies);
    draw_func();
    SDL_RenderPresent(renderer);
  } 
  return 0;
}
