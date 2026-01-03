#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include "config.hpp"
#include "raylib.h"

// Just one little helper... get X and Y window size from config
inline Vector2 getResolution(){ return {RESOLUTION * 16/9, RESOLUTION}; }

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
         Rectangle bounds;
         Texture2D sprite;

         virtual void init(){}
         virtual void update(){}
         virtual void draw(){}
         virtual void unload(){}
   };

   class Tile;

   class Player : public Object {
      public:
         Player(){
            bounds = {getResolution().x / 2 - 32, getResolution().y / 2 - 64, 64, 128};
            //pos = {bounds.x, bounds.y};
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

   enum TileType {
      GROUND,
      AIR,
   };

   class Tile : public Object {
      public:
         inline static int count{0};
         int size{64};
         int tag;
         TileType type;

         Tile(){
            bounds.width = size; bounds.height = size;
            this->tag = count;
            this->type = AIR;
            ++count;
         }

         static std::vector<Tile> createMap(int tileCount){
            std::vector<Tile> map;
            for(int i{0}; i < tileCount; ++i){
               map.emplace_back();
            }

            int i{0}, j{0};
            for(auto& tile : map){
               tile.bounds.x = i * tile.size;
               tile.bounds.y = j * tile.size;
               if(++i >= std::sqrt(Tile::count)){
                  i = 0;
                  ++j;
               }
            }
            return map;
         }

         void draw() override;
   };

   // master world class, contains all stack allocated objects
   class World {
      public:
         Player plr;
         Camera2D cam;
         Rectangle cullBox;
         Rectangle collisionBox;
         // std::vector<Tile> tileMap = Tile::createMap(225);
         Tile map[MAPSCALE][MAPSCALE / 2];

         World(){
            init();
         }

         void init(){
            cam.zoom = 1.0f;
            cam.target = {getResolution().x / 2, getResolution().y / 2};
            cam.offset = {getResolution().x / 2, getResolution().y / 2};

            cullBox.width = getResolution().x;
            cullBox.height = getResolution().y;

            collisionBox.width = plr.bounds.width + 64;
            collisionBox.height = plr.bounds.height + 64;

            for(int i{0}; i < MAPSCALE; ++i){
               for(int j{0}; j < MAPSCALE / 2; ++j){
                  map[i][j].bounds.x = 64 * i;
                  map[i][j].bounds.y = 64 * j;
               }
            }

            for(int i{0}; i < MAPSCALE; ++i){
               map[i][MAPSCALE / 2 - MAPSCALE / 4].type = GROUND;
            }

            /*
            int i{209};
            while(i < 224){
               ++i;
               tileMap[i].type = GROUND;
            }
            */
         }
         
         void update(){
            plr.update();

            cullBox.x = cam.target.x - getResolution().x / 2;
            cullBox.y = cam.target.y - getResolution().y / 2;

            collisionBox.x = plr.bounds.x - 32;
            collisionBox.y = plr.bounds.y - 32;
         };

         void draw(){
            // TODO: continue optimizing
            /*
            for (auto& tile : tileMap){
               // only draw what's on screen
               if(CheckCollisionRecs(cullBox, tile.bounds)){
                  tile.draw();
               }
            }
            */
            for(int i{0}; i < MAPSCALE; ++i){
               for(int j{0}; j < MAPSCALE / 2; ++j){
                  if(CheckCollisionRecs(cullBox, map[i][j].bounds)){
                     map[i][j].draw();
                  }
               }
            }

            DrawRectangleRec(collisionBox, RED);
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



   class Game {
         inline static States::Manager* stateManager{States::Manager::initManager()};

      public:
         Game(){
            // set title screen as first state
            stateManager->setState(&title); 
         }

         void run(){
            InitWindow(getResolution().x, getResolution().y, WINNAME);
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
               DrawFPS(getResolution().x / 36, getResolution().y / 36);
               #endif
               EndDrawing();
            }
            stateManager->currentState->unload();
            CloseWindow();
         }
   };
}
