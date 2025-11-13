#include "states.hpp"
#include "config.hpp"
#include <raylib.h>


void States::TitleScreen::update(){
   if(IsKeyPressed(KEY_SPACE)){
      States::Manager::initManager()->setState(&game);
   }
}

void States::TitleScreen::draw(){
   ClearBackground(YELLOW);
   DrawText("WELCOME TO GAME", WINWIDTH / 2, WINHEIGHT / 2, 32, BLACK);
}

Vector2 recPos{0, WINHEIGHT / 2 - 64};
int inputDir{0};
float speed{0};
float maxSpeed{4};
float accel{0.3};

void States::MainGame::update(){
   if(IsKeyPressed(KEY_SPACE)){
      States::Manager::initManager()->setState(&title);
   }

   inputDir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   if((inputDir != 0) && (speed < maxSpeed)){
      speed += accel;
   } else if(inputDir == 0) {
      speed = 0;
   }
   recPos.x += inputDir * speed;
}

void States::MainGame::draw(){
   ClearBackground(GRAY);
   DrawFPS(WINWIDTH / 36, WINHEIGHT / 36);
   DrawRectangle(recPos.x, recPos.y, 64, 128, YELLOW);
}
