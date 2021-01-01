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

struct velocity_t {
  float x;
  float y;
};

struct bounding_box_t {
  float x;
  float y;
  float w;
  float h;
};

struct texture_t {
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
  acorn::ecs::component_store components {};

  auto const ball = pool.create_entity();


  //setup ball
  position_t ball_position {.x = 15.f, .y = 15.f};
  components.insert_component(ball, ball_position);

  velocity_t ball_velocity {.x = .09f, .y = .25f};
  components.insert_component(ball, ball_velocity);

  bounding_box_t ball_bounding_box {.x = 15.f, .y = 15.f, .w = 50.f, .h = 50.f};
  components.insert_component(ball, ball_bounding_box);

  texture_t ball_texture {.texture = tex};
  components.insert_component(ball, ball_texture);

  auto const ball_physics_system = [&components, ball]{
    auto& ball_position = components.get_component<position_t>(ball);
    auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
    auto const& ball_velocity = components.get_component<velocity_t>(ball);

    ball_position.x += ball_velocity.x;
    ball_position.y += ball_velocity.y;

    ball_bounding_box.x = ball_position.x;
    ball_bounding_box.y = ball_position.y;
  };

  auto const ball_check_boundary_collision_system = [&components, ball] {
    auto& ball_position = components.get_component<position_t>(ball);
    auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
    auto& ball_velocity = components.get_component<velocity_t>(ball);

    if(ball_bounding_box.x < 0) {
      ball_bounding_box.x = 0;
      ball_position.x = 0;
      ball_velocity.x *= -1;
    } else if (ball_bounding_box.x + ball_bounding_box.w > 700) {
      ball_bounding_box.x = 700 - ball_bounding_box.w - 1;
      ball_position.x = 700 - ball_bounding_box.w - 1;
      ball_velocity.x *= -1;
    }

    if(ball_bounding_box.y < 0) {
      ball_bounding_box.y = 0;
      ball_position.y = 0;
      ball_velocity.y *= -1;
    } else if(ball_bounding_box.y + ball_bounding_box.h > 1440) {
      ball_bounding_box.y = 1440 - ball_bounding_box.h  - 1;
      ball_position.y = 1440 - ball_bounding_box.h - 1;
      ball_velocity.y *= -1;
    }
  };

  auto const render_ball_system = [&components, ball, renderer] {
    auto const ball_texture = components.get_component<texture_t>(ball);
    auto const ball_position = components.get_component<position_t>(ball);

    auto const rect = SDL_Rect{.x = static_cast<int>(ball_position.x), .y = static_cast<int>(ball_position.y), .w = 50, .h = 50};
    SDL_RenderCopy(renderer, ball_texture.texture, nullptr, &rect);
  };

  auto const render_pong_debug_system = [&components, ball, renderer] {
    auto const ball_bounding_box = components.get_component<bounding_box_t>(ball);

    auto const rect = SDL_Rect{.x = static_cast<int>(ball_bounding_box.x), .y = static_cast<int>(ball_bounding_box.y), .w = static_cast<int>(ball_bounding_box.w), .h = static_cast<int>(ball_bounding_box.h)};

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  };

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

    ball_physics_system();
    ball_check_boundary_collision_system();

    SDL_RenderClear(renderer);
    render_ball_system(); 
    render_pong_debug_system();
    SDL_RenderPresent(renderer);
  } 
  return 0;
}
