#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

void Entity::Player::move(){
   pos = Vector2Add(pos, Vector2Scale(vel, delta));
   bounds = {pos.x, pos.y, bounds.width, bounds.height};
}

bool Entity::Player::isOnFloor(){
   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground;
   for(auto& f : world.flr){
      if(CheckCollisionRecs(bounds, f.bounds)){
         ground = {foot.x, f.pos.y};
         pos.y -= Vector2Distance(foot, ground) / 4;
         return true;
      }
   }
   return false;
}

void Entity::Player::update(){

   //input direction and x movement
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   if(inputDir != 0){
      vel.x = Lerp(vel.x, inputDir * maxSpeed, accel * delta);
   } else vel.x = Lerp(vel.x, 0.0f, friction * delta);

   // jumping and gravity
   if(!isOnFloor()){
      vel.y += gravity * delta;
   } else if (isOnFloor() && IsKeyPressed(KEY_W)){
      vel.y = jumpPower * 75;
   } else vel.y = 0;

   // camera follow player
   world.cam.target = Vector2Lerp(world.cam.target, pos, 8 * delta);

   // clamp speed
   vel.x = Clamp(vel.x, -maxSpeed, maxSpeed);
   vel.y = Clamp(vel.y, -fallSpeed, fallSpeed);

   // apply velocity
   move();

   // reset player
   if(IsKeyPressed(KEY_BACKSPACE)){
      pos.x = WINWIDTH / 2;
      pos.y = WINHEIGHT / 2;
      vel.y = 0;
   }
}

void Entity::Player::draw(){
   DrawRectangleRec(bounds, YELLOW);
}

// ------------------------- Floor -------------------------- //

void Entity::Floor::draw(){
   DrawRectangleRec(bounds, BLUE);
}
