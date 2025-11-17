#include "zenjin.hpp"
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

// MAIN GAME
void States::MainGame::update(){
   if(IsKeyPressed(KEY_SPACE)){
      States::Manager::initManager()->setState(&title);
   }

   world.update();
}

void States::MainGame::draw(){
   ClearBackground(GRAY);
   // DrawFPS(WINWIDTH / 36, WINHEIGHT / 36);

   world.draw();
}
