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
         float maxSpeed{800};
         float accel{10};
         float friction{15};
         float jumpPower{-20};
         float fallSpeed{1800};
         float gravity{55};

         void move();
         bool isOnFloor();
         void update() override;
         void draw() override;
   };

   class Floor : public Object {
      public:
         Vector2 size{1000, 50};
         
         Floor(int x, int y, int w, int h){
            pos = {(float)x, (float)y};
            bounds = {pos.x, pos.y, (float)w, (float)h};
         }

         Floor(){
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
         std::vector<Floor> flr;

         World(){
            init();
         }

         void init(){
            cam.zoom = 1.0f;
            cam.target = {(float)WINWIDTH / 2, (float)WINHEIGHT / 2};
            cam.offset = {(float)WINWIDTH / 2, (float)WINHEIGHT / 2};
            for(int i{0}; i != 15; ++i){
               flr.emplace_back((i*100)+50, 600, 100, 100);
            }

            for(int i{0}; i != 5; ++i){
               flr.emplace_back(i*50+50, 300, 50, 50);
            }

            for(int i{0}; i != 5; ++i){
               flr.emplace_back(i*50+650, 300, 50, 50);
            }

            for(int i{0}; i != 5; ++i){
               flr.emplace_back(i*50+1250, 300, 50, 50);
            }
         }
         
         void update(){
            plr.update();
         };
         void draw(){
            for (auto& f : flr){
               f.draw();
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
