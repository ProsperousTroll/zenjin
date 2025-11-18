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
   /*
    * todo: make this work with unlimited amount of floor objects
    */
   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground;
   if(CheckCollisionRecs(bounds, world.flr.bounds)){
      ground = {foot.x, world.flr.pos.y};
      pos.y -= Vector2Distance(foot, ground) / 4;
      return true;
   }
   return false;
}

void Entity::Player::update(){
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

   //input direction and x movement
   if(inputDir != 0){
      vel.x = Lerp(vel.x, inputDir * maxSpeed, accel);
   } else vel.x = Lerp(vel.x, 0.0, friction);

   // jumping and gravity
   if(!isOnFloor()){
      vel.y += 0.6;
   } else if (isOnFloor() && IsKeyPressed(KEY_W)){
      vel.y = -15;
   } else vel.y = 0;

   // camera follow player
   world.cam.target = Vector2Lerp(world.cam.target, pos, 0.25);

   // clamp speed
   vel.x = Clamp(vel.x, -maxSpeed, maxSpeed);

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
   DrawRectangleRec(bounds, RED);
}

// ------------------------- Floor -------------------------- //

void Entity::Floor::draw(){
   DrawRectangleRec(bounds, BLUE);
}
