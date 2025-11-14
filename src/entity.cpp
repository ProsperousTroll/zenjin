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
    * todo: make this cast a line from the bottom of the player bounds to the neareast floor rectangle...
    *
    * if the size of the line is 0, you are touching the floor. 
    */

   Vector2 foot{pos.x + 32, bounds.y + 128};
   Vector2 ground;

   for (auto& e : World){
      if(e.first->entType == WORLD){
         ground = {foot.x, e.second->pos.y};
         if(Vector2Distance(foot, ground) <= 2.5){
            return true;
         }
         DrawLineV(foot, ground, BLACK);
      }
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
