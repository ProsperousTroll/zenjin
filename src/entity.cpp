#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

void Entity::Player::move(){
   bounds.x += vel.x * delta;
   bounds.y += vel.y * delta;
}

bool Entity::Player::isOnWall(){
   // TODO: implement wall/floor logic without for loops.
   return false;
}

bool Entity::Player::isOnFloor(){
   /*
    * TODO: find it in your heart to remove this horse shit
    *
   for(int i{0}; i < MAPWIDTH; ++i){
      for(int j{0}; j < MAPHEIGHT; ++j){
         if(CheckCollisionRecs(bounds, world.map[i][j].bounds) && world.map[i][j].type == GROUND){
            bounds.y -= Vector2Distance({bounds.x + 32, bounds.y + 128}, {bounds.x + 32, world.map[i][j].bounds.y}) / 4;
            return (bounds.x + bounds.y / 2 >= world.map[i][j].bounds.x) && (bounds.x + bounds.y / 2 <= world.map[i][j].bounds.x + world.map[i][j].bounds.y);
         }
      }
   }
   */

   // THIS IS OPTIMIZED AND WORKS GREAT. HOLY FUCK. FINALLY.
   
   // TODO: though it shouldn't be possible, it'd be nice if
   // the player object didn't flip the fuck out if it's outside
   // the map bounds.
   int cx{(int)(bounds.x)/64};
   int cy = (int)(bounds.y+128)/64;
   if(cx >= 0 && cy < MAPHEIGHT && world.map[cx][cy].type == GROUND){
      bounds.y -= Vector2Distance({bounds.x + 32, bounds.y + 128}, {bounds.x + 32, world.map[cx][cy].bounds.y}) / 4;
      return true;
   }
   return false;
}

void Entity::Player::update(){

   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

#ifdef DEBUG

   /*
    * Noclip, print current tile map and pixel coords.
    */

   int cx = ( bounds.x )/64; 
   int cy = ( bounds.y+128 )/64;
   std::cout << "Current location on map - X: " << cx << " Y: " << cy << '\n';
   std::cout << "Current pixel coords - X: " << bounds.x << " Y: " << bounds.y << '\n';

   if(inputDir != 0){
      vel.x = inputDir * 1000;
   }

   if(IsKeyDown(KEY_W)){
      vel.y = -1000;
   } else if (IsKeyDown(KEY_S)){
      vel.y = 1000;
   } else vel.y = 0;
#else
   //input direction and x movement
   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   if(inputDir != 0){
         vel.x = Lerp(vel.x, inputDir * maxSpeed, accel * delta);
   } else vel.x = Lerp(vel.x, 0.0f, friction * delta);

   // jumping and gravity
   if(!isOnFloor()){
      vel.y += gravity * delta;
   } else if (isOnFloor() && IsKeyDown(KEY_W)){
      vel.y = jumpPower * 75;
   } else vel.y = 0;
#endif

   // camera follow player
   world.cam.target = Vector2Lerp(world.cam.target, {bounds.x, bounds.y}, 8 * delta);

   // clamp speed
   vel.x = Clamp(vel.x, -maxSpeed, maxSpeed);
   vel.y = Clamp(vel.y, -fallSpeed, fallSpeed);

   // apply velocity
   move();

   // reset player
   if(IsKeyPressed(KEY_BACKSPACE)){
      bounds.x = 0;
      bounds.y = 0;
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
