#include "config.hpp"
#include "zenjin.hpp"
#include <raylib.h>
#include <string>
#include <unordered_map>

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

inline std::unordered_map<Entity::Tag, Entity::Object> World{
   {Entity::PLAYER, Entity::Player()},
};
