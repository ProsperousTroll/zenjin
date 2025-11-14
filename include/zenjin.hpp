#pragma once
#include <iostream>
#include "config.hpp"
#include "raylib.h"
#include <map>

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

namespace Entity {
   // tag enum for different object types.
   enum T {
      PLAYER, // a player controller
      NPC, // any kind of enemy, friend, or hostile
      OBJECT, // an interactable part of the world
      ITEM, // something that can be equipped
      WORLD, // world geometry and such
      UI, // what do you think?
   };

   class Tag {
      public:
         inline static int entCount{-1};
         int tag;
         T entType;

         Tag(T t){
            ++entCount;
            entType = t;
            tag = entCount;
         }
   };

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
            bounds.width = 64;
            bounds.height = 128;
            pos.x = WINWIDTH / 2 - bounds.width / 2;
            pos.y = WINHEIGHT / 2 - bounds.height / 2;
         }

         Vector2 vel;
         int inputDir{0};
         float maxSpeed{10};
         float accel{0.15};
         float friction{0.3};

         void move();
         bool isOnFloor();
         void update() override;
         void draw() override;
   };

   class Floor : public Object {
      public:
         Vector2 size{1000, 50};
         
         Floor(int x, int y, int w, int h){
            pos.x = x;
            pos.y = y;
            bounds = {pos.x, pos.y, (float)w, (float)h};
         }

         Floor(){
            pos.x = WINWIDTH / 2 - size.x / 2;
            pos.y = WINHEIGHT - size.y;
            bounds = {pos.x, pos.y, size.x, size.y};
         }

         void draw() override;
   };
}

// Map of every game object, sorted by tag.
inline std::map<Entity::Tag*, Entity::Object*> World{
   {new Entity::Tag(Entity::PLAYER), new Entity::Player()},
   {new Entity::Tag(Entity::WORLD), new Entity::Floor()},
};
