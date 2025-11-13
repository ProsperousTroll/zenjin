#pragma once
#include <iostream>
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
   enum Tag {
      PLAYER, // a player controller
      NPC, // any kind of enemy, friend, or hostile
      OBJECT, // an interactable part of the world
      ITEM, // something that can be equipped
      WORLD, // world geometry and such
      UI, // what do you think?
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
            bounds.x = 64;
            bounds.y = 128;
            pos.x = WINWIDTH / 2 - bounds.x;
            pos.y = WINHEIGHT / 2 - bounds.y;
         }
         Vector2 vel;

         void move();

         void update() override;
         void draw() override;
   };
}

/*
inline std::unordered_map<Entity::Tag, Entity::Object> World{
 {Entity::PLAYER, Entity::Player()},
};
*/


