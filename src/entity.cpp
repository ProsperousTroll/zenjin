#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

void Entity::Player::move(){
   //pos = Vector2Add(pos, Vector2Scale(vel, delta));
   //bounds = {pos.x, pos.y, bounds.width, bounds.height};
   bounds.x += vel.x * delta;
   bounds.y += vel.y * delta;
}

bool Entity::Player::isOnWall(){
   // TODO: implement wall/floor logic without for loops.
   //for(auto& tile : world.tileMap){
   //   if(CheckCollisionRecs({bounds.x - 2, bounds.y, bounds.width + 4, bounds.height - 15}, tile.bounds)){
   //      if(vel.x > 0){
   //         wallDir = 1;
   //         vel.x = Clamp(vel.x, -maxSpeed, 0);
   //      }

   //      if(vel.x < 0){
   //         wallDir = -1;
   //         vel.x = Clamp(vel.x, 0, maxSpeed);
   //      }
   //      return true;
   //   }
   //}
   return false;
}

bool Entity::Player::isOnFloor(){
   /*
   for(auto& tile : world.tileMap){
      if(CheckCollisionRecs(bounds, tile.bounds) && tile.type == GROUND){
         bounds.y -= Vector2Distance({bounds.x + 32, bounds.y + 128}, {bounds.x + 32, tile.bounds.y}) / 4;
         return (bounds.x + bounds.y / 2 >= tile.bounds.x) && (bounds.x + bounds.y / 2 <= tile.bounds.x + tile.bounds.y);
      }
   }
   */
   for(int i{0}; i < MAPSCALE; ++i){
      for(int j{0}; j < MAPSCALE / 2; ++j){
         if(CheckCollisionRecs(bounds, world.map[i][j].bounds) && world.map[i][j].type == GROUND){
            bounds.y -= Vector2Distance({bounds.x + 32, bounds.y + 128}, {bounds.x + 32, world.map[i][j].bounds.y}) / 4;
            return (bounds.x + bounds.y / 2 >= world.map[i][j].bounds.x) && (bounds.x + bounds.y / 2 <= world.map[i][j].bounds.x + world.map[i][j].bounds.y);
         }
      }
   }
   return false;
}

void Entity::Player::update(){
   //std::cout << wallDir << '\n';

   //input direction and x movement
   // actually wtf gross
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   if(inputDir != 0){
         vel.x = Lerp(vel.x, inputDir * maxSpeed, accel * delta);
   } else vel.x = Lerp(vel.x, 0.0f, friction * delta);

   
   /*
    * For debugging, essentially noclip.
    *
   if(IsKeyDown(KEY_W)){
      vel.y = -400;
   } else if (IsKeyDown(KEY_S)){
      vel.y = 400;
   } else vel.y = 0;
   */

   // jumping and gravity
   if(!isOnFloor()){
      vel.y += gravity * delta;
   } else if (isOnFloor() && IsKeyDown(KEY_W)){
      vel.y = jumpPower * 75;
   } else vel.y = 0;


   // camera follow player
   world.cam.target = Vector2Lerp(world.cam.target, {bounds.x, bounds.y}, 8 * delta);

   // clamp speed
   vel.x = Clamp(vel.x, -maxSpeed, maxSpeed);
   vel.y = Clamp(vel.y, -fallSpeed, fallSpeed);

   // apply velocity
   move();

   // reset player
   if(IsKeyPressed(KEY_BACKSPACE)){
      bounds.x = getResolution().x / 2;
      bounds.y = getResolution().y / 2;
      vel.y = 0;
   }
}

void Entity::Player::draw(){
   DrawRectangleRec(bounds, YELLOW);
}

// ------------------------- Floor -------------------------- //

void Entity::Tile::draw(){
   switch(type){
      default:
         DrawRectangleRec(bounds, SKYBLUE); break;
      case GROUND:
         DrawRectangleRec(bounds, DARKGREEN); break;
   }
}
