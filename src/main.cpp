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

  std::uint8_t p1_score {0};
  std::uint8_t p2_score {0};

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

  const uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);
  if(!keyboard_state) {
    std::cout << "Could not get Keyboard state." << std::endl;
    return 3;
  }

  SDL_Event e{};
  acorn::ecs::entity_pool pool{};
  acorn::ecs::component_store components {};

  auto const play_area = pool.create_entity();
  auto const ball = pool.create_entity();
  auto const paddle_one = pool.create_entity();
  auto const paddle_two = pool.create_entity();
  auto const player_one_score_area = pool.create_entity();
  auto const player_two_score_area = pool.create_entity();

  //setup paddle one
  position_t paddle_one_position {.x = 15.f, .y = 1340.f};
  components.insert_component(paddle_one, paddle_one_position);

  bounding_box_t paddle_one_bounding_box {.x = 15.f, .y = 1340.f, .w = 300.f, .h = 35.f};
  components.insert_component(paddle_one, paddle_one_bounding_box);

  texture_t paddle_one_texture {.texture = tex};
  components.insert_component(paddle_one, paddle_one_texture);

  //setup paddle two
  position_t paddle_two_position {.x = 15.f, .y = 100.f};
  components.insert_component(paddle_two, paddle_two_position);

  bounding_box_t paddle_two_bounding_box {.x = 15.f, .y = 100.f, .w = 300.f, .h = 35.f};
  components.insert_component(paddle_two, paddle_two_bounding_box);

  texture_t paddle_two_texture {.texture = tex};
  components.insert_component(paddle_two, paddle_two_texture);

  //setup ball
  position_t ball_position {.x = 325.f, .y = 600.f};
  components.insert_component(ball, ball_position);

  velocity_t ball_velocity {.x = .09f, .y = .25f};
  components.insert_component(ball, ball_velocity);

  bounding_box_t ball_bounding_box {.x = 325.f, .y = 1415.f, .w = 50.f, .h = 50.f};
  components.insert_component(ball, ball_bounding_box);

  texture_t ball_texture {.texture = tex};
  components.insert_component(ball, ball_texture);

  //setup play area bounds
  bounding_box_t player_area_box {.x = 6.f, .y = 0.f, .w = 688.f, .h = 1440.f};
  components.insert_component(play_area, player_area_box);

  //setup scoring area
  bounding_box_t player_one_score_area_bb {.x = 6.f, .y = 0.f, .w = 688.f, .h = 5.f};
  components.insert_component(player_one_score_area, player_one_score_area_bb);

  bounding_box_t player_two_score_area_bb {.x = 6.f, .y = 1435.f, .w = 688.f, .h = 5.f};
  components.insert_component(player_two_score_area, player_two_score_area_bb);

  auto const ball_physics_system = [&components, ball]{
    auto& ball_position = components.get_component<position_t>(ball);
    auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
    auto const& ball_velocity = components.get_component<velocity_t>(ball);

    ball_position.x += ball_velocity.x;
    ball_position.y += ball_velocity.y;

    ball_bounding_box.x = ball_position.x;
    ball_bounding_box.y = ball_position.y;
  };

  auto const ball_check_boundary_collision_system = [&components, ball, play_area] {
    auto& ball_position = components.get_component<position_t>(ball);
    auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
    auto& ball_velocity = components.get_component<velocity_t>(ball);

    auto const& play_area_bounding_box = components.get_component<bounding_box_t>(play_area);

    if(ball_bounding_box.x < play_area_bounding_box.x) {
      ball_bounding_box.x = play_area_bounding_box.x;
      ball_position.x = play_area_bounding_box.x;
      ball_velocity.x *= -1;
    } else if (ball_bounding_box.x + ball_bounding_box.w > play_area_bounding_box.x + play_area_bounding_box.w) {
      ball_bounding_box.x = play_area_bounding_box.x + play_area_bounding_box.w - ball_bounding_box.w - 1;
      ball_position.x = play_area_bounding_box.x + play_area_bounding_box.w - ball_bounding_box.w - 1;
      ball_velocity.x *= -1;
    }

    if(ball_bounding_box.y < play_area_bounding_box.y) {
      ball_bounding_box.y = play_area_bounding_box.y;
      ball_position.y = play_area_bounding_box.y;
      ball_velocity.y *= -1;
    } else if(ball_bounding_box.y + ball_bounding_box.h > play_area_bounding_box.y + play_area_bounding_box.h) {
      ball_bounding_box.y = play_area_bounding_box.y + play_area_bounding_box.h - ball_bounding_box.h  - 1;
      ball_position.y = play_area_bounding_box.y + play_area_bounding_box.h - ball_bounding_box.h - 1;
      ball_velocity.y *= -1;
    }
  };

  auto const paddle_check_boundary_collision_system = [&components, play_area] (acorn::ecs::entity_t paddle) {
    auto& ball_position = components.get_component<position_t>(paddle);
    auto& ball_bounding_box = components.get_component<bounding_box_t>(paddle);

    auto const& play_area_bounding_box = components.get_component<bounding_box_t>(play_area);

    if(ball_bounding_box.x < play_area_bounding_box.x) {
      ball_bounding_box.x = play_area_bounding_box.x;
      ball_position.x = play_area_bounding_box.x;
    } else if (ball_bounding_box.x + ball_bounding_box.w > play_area_bounding_box.x + play_area_bounding_box.w) {
      ball_bounding_box.x = play_area_bounding_box.x + play_area_bounding_box.w - ball_bounding_box.w - 1;
      ball_position.x = play_area_bounding_box.x + play_area_bounding_box.w - ball_bounding_box.w - 1;
    }

    if(ball_bounding_box.y < play_area_bounding_box.y) {
      ball_bounding_box.y = play_area_bounding_box.y;
      ball_position.y = play_area_bounding_box.y;
    } else if(ball_bounding_box.y + ball_bounding_box.h > play_area_bounding_box.y + play_area_bounding_box.h) {
      ball_bounding_box.y = play_area_bounding_box.y + play_area_bounding_box.h - ball_bounding_box.h  - 1;
      ball_position.y = play_area_bounding_box.y + play_area_bounding_box.h - ball_bounding_box.h - 1;
    }
  };


  auto const render_ball_system = [&components, ball, renderer] {
    auto const ball_texture = components.get_component<texture_t>(ball);
    auto const ball_position = components.get_component<position_t>(ball);

    auto const rect = SDL_Rect{.x = static_cast<int>(ball_position.x), .y = static_cast<int>(ball_position.y), .w = 50, .h = 50};
    SDL_RenderCopy(renderer, ball_texture.texture, nullptr, &rect);
  };

  auto const render_paddle_one_system = [&components, paddle_one, renderer] {
    auto const paddle_one_texture = components.get_component<texture_t>(paddle_one);
    auto const paddle_one_position = components.get_component<position_t>(paddle_one);

    auto const rect = SDL_Rect{.x = static_cast<int>(paddle_one_position.x), .y = static_cast<int>(paddle_one_position.y), .w = 300, .h = 35};
    SDL_RenderCopy(renderer, paddle_one_texture.texture, nullptr, &rect);
  };

  auto const render_paddle_two_system = [&components, paddle_two, renderer] {
    auto const paddle_two_texture = components.get_component<texture_t>(paddle_two);
    auto const paddle_two_position = components.get_component<position_t>(paddle_two);

    auto const rect = SDL_Rect{.x = static_cast<int>(paddle_two_position.x), .y = static_cast<int>(paddle_two_position.y), .w = 300, .h = 35};
    SDL_RenderCopy(renderer, paddle_two_texture.texture, nullptr, &rect);
  };

  auto const render_pong_debug_system = [&components, ball, renderer] {
    auto const ball_bounding_box = components.get_component<bounding_box_t>(ball);

    auto const rect = SDL_Rect{.x = static_cast<int>(ball_bounding_box.x), .y = static_cast<int>(ball_bounding_box.y), .w = static_cast<int>(ball_bounding_box.w), .h = static_cast<int>(ball_bounding_box.h)};

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  };

  auto const render_play_area_debug_system = [&components, renderer, play_area]() {
    auto const bounding_box = components.get_component<bounding_box_t>(play_area);

    auto const rect = SDL_Rect{.x = static_cast<int>(bounding_box.x), .y = static_cast<int>(bounding_box.y), .w = static_cast<int>(bounding_box.w), .h = static_cast<int>(bounding_box.h)};

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  };

  auto const render_paddle_debug_system = [&components, renderer](acorn::ecs::entity_t paddle) {
    auto const bounding_box = components.get_component<bounding_box_t>(paddle);

    auto const rect = SDL_Rect{.x = static_cast<int>(bounding_box.x), .y = static_cast<int>(bounding_box.y), .w = static_cast<int>(bounding_box.w), .h = static_cast<int>(bounding_box.h)};

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  };

  auto const render_score_area_debug_system = [&components, renderer, player_one_score_area, player_two_score_area]() {
    auto const p1_bounding_box = components.get_component<bounding_box_t>(player_one_score_area);
    auto const p2_bounding_box = components.get_component<bounding_box_t>(player_two_score_area);

    auto const rect = SDL_Rect{.x = static_cast<int>(p1_bounding_box.x), .y = static_cast<int>(p1_bounding_box.y), .w = static_cast<int>(p1_bounding_box.w), .h = static_cast<int>(p1_bounding_box.h)};
    auto const rect2 = SDL_Rect{.x = static_cast<int>(p2_bounding_box.x), .y = static_cast<int>(p2_bounding_box.y), .w = static_cast<int>(p2_bounding_box.w), .h = static_cast<int>(p2_bounding_box.h)};

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  };

  auto const check_ball_collision_with_paddle_system = [&components, ball, paddle_one, paddle_two] {
    auto const check_collision = [&components, ball](acorn::ecs::entity_t paddle) {
      auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
      auto& ball_velocity = components.get_component<velocity_t>(ball);
      auto& ball_position = components.get_component<position_t>(ball);

      auto const& paddle_bounding_box = components.get_component<bounding_box_t>(paddle);

      if((ball_bounding_box.x < paddle_bounding_box.x + paddle_bounding_box.w && 
          ball_bounding_box.x + ball_bounding_box. w > paddle_bounding_box.x &&
          ball_bounding_box.y < paddle_bounding_box.y + paddle_bounding_box.h &&
          ball_bounding_box.y + ball_bounding_box.h > paddle_bounding_box.y)) {

      ball_velocity.x *= -1;
      ball_velocity.y *= -1;

      }
    };

    check_collision(paddle_one);
    check_collision(paddle_two);
  };

  auto const check_ball_collision_with_score_area_system = [&p1_score, &p2_score, &components, ball, player_one_score_area, player_two_score_area] {
    auto const check_collision = [&components, ball](acorn::ecs::entity_t paddle) {
      auto& ball_bounding_box = components.get_component<bounding_box_t>(ball);
      auto& ball_velocity = components.get_component<velocity_t>(ball);
      auto& ball_position = components.get_component<position_t>(ball);

      auto const& paddle_bounding_box = components.get_component<bounding_box_t>(paddle);

      if((ball_bounding_box.x < paddle_bounding_box.x + paddle_bounding_box.w && 
          ball_bounding_box.x + ball_bounding_box. w > paddle_bounding_box.x &&
          ball_bounding_box.y < paddle_bounding_box.y + paddle_bounding_box.h &&
          ball_bounding_box.y + ball_bounding_box.h > paddle_bounding_box.y)) {

          ball_position.x = 325.f;
          ball_position.y = 600.f;
          
          ball_bounding_box.x = 325.f;
          ball_bounding_box.y = 600.f;

          ball_velocity.y *= -1;

          return true;
      }
      return false;
    };

    if(check_collision(player_one_score_area)) {
      p1_score += 1;
      std::cout << "Player One score: " << static_cast<int>(p1_score) << "\n";
    }
    if(check_collision(player_two_score_area)) {
      p2_score += 1;
      std::cout << "Player Two score: " << static_cast<int>(p2_score) << "\n";
    }
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

    //Poll keyboard for input
    auto& p1_bounding_box = components.get_component<bounding_box_t>(paddle_one);
    auto& p1_position = components.get_component<position_t>(paddle_one);
  
    auto& p2_bounding_box = components.get_component<bounding_box_t>(paddle_two);
    auto& p2_position = components.get_component<position_t>(paddle_two);
    if(keyboard_state[SDL_SCANCODE_S]) {
      p1_bounding_box.x += 1.0f;
      p1_position.x += 1.0f;
    }
  
    if(keyboard_state[SDL_SCANCODE_A]) {
      p1_bounding_box.x -= 1.0f;
      p1_position.x -= 1.0f;
    }
    
    if(keyboard_state[SDL_SCANCODE_L]) {
      p2_bounding_box.x += 1.0f;
      p2_position.x += 1.0f;
    }
  
    if(keyboard_state[SDL_SCANCODE_K]) {
      p2_bounding_box.x -= 1.0f;
      p2_position.x -= 1.0f;
    }
    paddle_check_boundary_collision_system(paddle_one);
    paddle_check_boundary_collision_system(paddle_two);
    ball_physics_system();
    check_ball_collision_with_score_area_system();
    ball_check_boundary_collision_system();
    check_ball_collision_with_paddle_system();

    SDL_RenderClear(renderer);
    render_ball_system(); 
    render_paddle_one_system();
    render_paddle_two_system();
    render_pong_debug_system();
    render_paddle_debug_system(paddle_one);
    render_paddle_debug_system(paddle_two);
    render_play_area_debug_system();
    render_score_area_debug_system();
    SDL_RenderPresent(renderer);
  } 
  return 0;
}
