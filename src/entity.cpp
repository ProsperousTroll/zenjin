#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

void Entity::Player::move(){
   pos = Vector2Add(pos, Vector2Scale(vel, delta));
   bounds = {pos.x, pos.y, bounds.width, bounds.height};
}

bool Entity::Player::isOnWall(){
   for(auto& f : world.flr){
      if(CheckCollisionRecs({bounds.x - 2, bounds.y, bounds.width + 4, bounds.height - 15}, f.bounds)){
         if(vel.x > 0){
            wallDir = 1;
            vel.x = Clamp(vel.x, -maxSpeed, 0);
            pos.x -= Vector2Distance({bounds.x + bounds.width, bounds.height / 2}, {f.bounds.x, bounds.height / 2});
         }

         if(vel.x < 0){
            wallDir = -1;
            vel.x = Clamp(vel.x, 0, maxSpeed);
            pos.x += Vector2Distance({bounds.x, bounds.height / 2}, {f.bounds.x + f.bounds.width, bounds.height / 2});
         }
         return true;
      }
   }
   return false;
}

bool Entity::Player::isOnFloor(){
   // this method is kinda just turning into a general collision check
   /*
   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground;
   for(auto& f : world.flr){
      ground = {foot.x, f.pos.y};
      if(CheckCollisionRecs(bounds, f.bounds)){
         if(vel.y < 0){
            vel.y *= -1;
            return false;
         }
         if(vel.y >= 0){
            pos.y -= Vector2Distance(foot, ground) / 4;
            return true;
         }
      }
   }
   */
   for(auto& f : world.flr){
      if(CheckCollisionRecs(bounds, f.bounds)){
         pos.y -= Vector2Distance({pos.x + 32, bounds.y + 128}, {pos.x + 32, f.pos.y}) / 4;
         return (pos.x + bounds.y / 2 >= f.bounds.x) && (pos.x + bounds.y / 2 <= f.bounds.x + f.bounds.y);
      }
   }
   return false;
}

void Entity::Player::update(){
   std::cout << wallDir << '\n';

   //input direction and x movement
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   if(!isOnWall()){
      wallDir = 0;
      if(inputDir != 0){
         vel.x = Lerp(vel.x, inputDir * maxSpeed, accel * delta);
      } else vel.x = Lerp(vel.x, 0.0f, friction * delta);
   } else if (wallDir == 1 && IsKeyDown(KEY_A)){
      vel.x = -maxSpeed;
   } else if (wallDir == -1 && IsKeyDown(KEY_D)){
      vel.x = maxSpeed;
   }

   
   // jumping and gravity
   /*
   if(IsKeyDown(KEY_W)){
      vel.y = -80;
   } else if (IsKeyDown(KEY_S)){
      vel.y = 80;
   } else vel.y = 0;
   isOnFloor();
   */
   if(!isOnFloor()){
      vel.y += gravity * delta;
   } else if (isOnFloor() && IsKeyDown(KEY_W)){
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
