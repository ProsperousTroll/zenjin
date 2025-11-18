#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

void Entity::Player::move(){
   pos.x += vel.x;
   pos.y += vel.y;

   bounds.x = pos.x;
   bounds.y = pos.y;
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
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

   //input direction and x movement
   if(inputDir != 0){
      vel.x = Lerp(vel.x, inputDir * maxSpeed * delta, accel * delta);
   } else vel.x = Lerp(vel.x, 0.0, friction * delta);

   // jumping and gravity
   if(!isOnFloor()){
      vel.y += gravity * delta;
   } else if (isOnFloor() && IsKeyPressed(KEY_W)){
      vel.y = jumpPower * delta;
   } else vel.y = 0;

   // camera follow player
   world.cam.target = Vector2Lerp(world.cam.target, pos, 8 * delta);

   // clamp speed
   vel.x = Clamp(vel.x, -maxSpeed * delta, maxSpeed * delta);
   vel.y = Clamp(vel.y, -fallSpeed * delta, fallSpeed * delta);

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
