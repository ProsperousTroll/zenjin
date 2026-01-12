#include "config.hpp"
#include "zenjin.hpp"
#include "raylib.h"
#include "raymath.h"

bool Entity::Player::isOnCeiling(){
   int cx_l{(int)(bounds.x) / 64};
   int cx_r{(int)(bounds.x+64) / 64};
   int cy{(int)(bounds.y)/64};
   return ( cx_l >= 0 && cy >= 0 && cx_l < MAPWIDTH && cy < MAPHEIGHT && world.map[cx_l][cy].type == GROUND ) || ( cx_r >= 0 && cy >= 0 && cx_r < MAPWIDTH && cy < MAPHEIGHT && world.map[cx_r][cy].type == GROUND );
}

bool Entity::Player::isOnWall(){
   int cy1{(int)(bounds.y) / 64};
   int cy2{(int)(bounds.y + 64) / 64};
   int cx_l{(int)(bounds.x) / 64};
   int cx_r{(int)(bounds.x + 64) / 64};
   return  (
      (cx_l >= 0 && cx_l < MAPWIDTH && world.map[cx_l][cy1].type == GROUND) ||
      (cx_r >= 0 && cx_r < MAPWIDTH && world.map[cx_r][cy1].type == GROUND) ||
      (cx_l >= 0 && cx_l < MAPWIDTH && world.map[cx_l][cy2].type == GROUND) ||
      (cx_r >= 0 && cx_r < MAPWIDTH && world.map[cx_r][cy2].type == GROUND) 
   ); 
}

bool Entity::Player::isOnFloor(){
   // TODO: though it shouldn't be possible, it'd be nice if
   // the player object didn't flip the fuck out if it's outside
   // the map bounds.
   int cx_l{(int)(bounds.x)/64};
   int cx_r{(int)(bounds.x+64)/64};
   int cy{(int)(bounds.y+128)/64};
   if(( cx_l >= 0 && cx_l < MAPWIDTH && cy < MAPHEIGHT && world.map[cx_l][cy].type == GROUND ) || ( cx_r > 0 && cx_r < MAPWIDTH && cy < MAPHEIGHT && world.map[cx_r][cy].type == GROUND )){
      bounds.y -= Vector2Distance({bounds.x + 32, bounds.y + 128}, {bounds.x + 32, world.map[cx_r][cy].bounds.y});
      return true;
   }
   return false;
}

void Entity::Player::move(){
   bounds.x += vel.x * delta;
   bounds.y += vel.y * delta;

   if(isOnCeiling()){
      vel.y = -vel.y;
   }

}

void Entity::Player::update(){

   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

#ifdef DEBUG

   /*
    * Noclip, print current tile map and pixel coords.
    */

   // int cx = ( bounds.x )/64; 
   // int cy = ( bounds.y+128 )/64;
   // std::cout << "Current location on map - X: " << cx << " Y: " << cy << '\n';
   // std::cout << "Current pixel coords - X: " << bounds.x << " Y: " << bounds.y << '\n';

   if(inputDir != 0){
      vel.x = inputDir * 1000;
   } else vel.x = 0;

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
      vel.y = jumpPower;
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
      bounds.x = getResolution().x;
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
