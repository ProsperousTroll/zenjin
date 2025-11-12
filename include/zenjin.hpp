#pragma once
#include <iostream>
#include "raylib.h"
#include "states.hpp"
#include "config.hpp"

namespace Zenjin{
   inline void Log(const char* input){
      std::cout << input << '\n';
   }

   struct Window {
      int width;
      int height;
      char* name;

      Window(){
         width = WINWIDTH;
         height = WINHEIGHT;
         name = WINNAME;
      }
   };

   class Game {
         Window win;
         inline static States::Manager* stateManager{States::Manager::initManager()};

      public:
         Game(){
            // set title screen as first state
            stateManager->setState(&title); 
         }

         void run(){
            InitWindow(win.width, win.height, win.name);
            SetTargetFPS(TARGETFPS);
            stateManager->currentState->init();
            while(!WindowShouldClose()){
               stateManager->currentState->update();
               BeginDrawing();
               stateManager->currentState->draw();
               EndDrawing();
            }
            stateManager->currentState->unload();
            CloseWindow();
         }
   };
}
