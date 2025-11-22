#pragma once
#include <iostream>
#include <vector>
#include "config.hpp"
#include "raylib.h"

namespace States {
   class EmptyState{
      public:
         virtual void init(){}
         virtual void update(){}
         virtual void draw(){}
         virtual void unload(){}
   };

   class Manager {
         Manager(){};
         inline static Manager* man{nullptr};

      public:
         EmptyState* currentState;
         static Manager* initManager() {
            if(!man){
               man = new Manager();
            }
            return man;
         }

         EmptyState* getState(){
            return currentState;
         }
         
         void setState(EmptyState* s){
            currentState = s;
         }
   };

   class TitleScreen : public EmptyState {
      public:
         void update() override;
         void draw() override;
   };

   class MainGame : public EmptyState {
      public:
         void update() override;
         void draw() override;
   };
}

inline States::TitleScreen title;
inline States::MainGame game;

namespace Entity {
   // blank template for all game objects and entities.
   class Object {
      public:
         Vector2 pos;
         Rectangle bounds;
         Texture2D sprite;

         virtual void init(){}
         virtual void update(){}
         virtual void draw(){}
         virtual void unload(){}
   };

   class Player : public Object {
      public:
         Player(){
            bounds = {(float)WINWIDTH / 2 - 32, (float)WINHEIGHT / 2 - 64, 64, 128};
            pos = {bounds.x, bounds.y};
         }

         Vector2 vel;
         int inputDir{0};
         int wallDir{0};
         float maxSpeed{800};
         float accel{10};
         float friction{15};
         float jumpPower{-20};
         float fallSpeed{1800};
         float gravity{5500};

         void move();
         bool isOnFloor();
         bool isOnWall();
         void update() override;
         void draw() override;
   };

   class Tile : public Object {
      public:
         Vector2 size{64, 64};
         
         Tile(int x, int y, int w, int h){
            pos = {(float)x, (float)y};
            bounds = {pos.x, pos.y, (float)w, (float)h};
         }

         Tile(){
            pos.x = (float)WINWIDTH / 2 - size.x / 2;
            pos.y = (float)WINHEIGHT - size.y;
            bounds = {pos.x, pos.y, size.x, size.y};
         }

         void draw() override;
   };

   // master world class, contains all stack allocated objects
   class World {
      public:
         Player plr;
         Camera2D cam;
         // will be gone soon
         std::vector<Tile> tileMap;
         Tile tileM[25][25];

         World(){
            init();
         }

         void init(){
            cam.zoom = 1.0f;
            cam.target = {(float)WINWIDTH / 2, (float)WINHEIGHT / 2};
            cam.offset = {(float)WINWIDTH / 2, (float)WINHEIGHT / 2};
            /*
            for(int i{0}; i != 15; ++i){
               tileMap.emplace_back((i*100)+50, 600, 100, 100);
            }
            */
            for(int i{0}; i != 25; ++i){
               tileM[i][0].bounds.x = i * 64;
               tileM[0][i].bounds.y = i * 64;
            }
         }
         
         void update(){
            plr.update();
         };
         void draw(){
            for (auto& tile : tileM){
               tile->draw();
            }
            plr.draw();
         }

         void unload(){}
   };
}

inline Entity::World world;
inline double delta;

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
            // optionally set a target fps
            #ifdef TARGETFPS
            SetTargetFPS(TARGETFPS);
            #endif
            // pseudo-vsync
            #ifdef VSYNC
            int vsync{GetMonitorRefreshRate(MONITOR)};
            SetTargetFPS(vsync);
            #endif
            stateManager->currentState->init();
            while(!WindowShouldClose()){
               delta = GetFrameTime();
               stateManager->currentState->update();
               BeginDrawing();
               BeginMode2D(world.cam);
               stateManager->currentState->draw();
               EndMode2D();
               // draw FPS to screen independant of camera (if macro defined)
               #ifdef DRAWFPS
               DrawFPS(WINWIDTH / 36, WINHEIGHT / 36);
               #endif
               EndDrawing();
            }
            stateManager->currentState->unload();
            CloseWindow();
         }
   };
}
