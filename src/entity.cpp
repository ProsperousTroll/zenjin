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
   /* this is going to be really inefficient in a world with lots of entities.
    * todo: make this work good and with unlimited ammount of floor objects
    */

   /*
   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground;

   for (auto& e : World){
      if(e.first->entType == WORLD){
         ground = {foot.x, e.second->pos.y};
         std::cout << Vector2Angle(foot, ground) << std::endl;
         if(foot.y >= ground.y){
            pos.y -= Vector2Distance(foot, ground);
            return true;
         }
         DrawLineV(foot, ground, BLACK);
      }
   }
   */
   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground{0.0, 720 - 50};
   if(CheckCollisionRecs(bounds, world.flr.bounds)){
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

   // apply velocity
   move();
}

void Entity::Player::draw(){
   DrawRectangleRec(bounds, YELLOW);
}

// ------------------------- Floor -------------------------- //

void Entity::Floor::draw(){
   DrawRectangleRec(bounds, BLUE);
}
