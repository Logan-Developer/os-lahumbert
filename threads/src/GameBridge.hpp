#pragma once
#include <SDL.h>
#include "Physics.hpp"
#include "Sprite.hpp"
#include "Collision.hpp"
#include "AnimatedSprite.hpp"

class Bridge
{
    public:
    // GetRender is created in game.hpp and any script can access it by including this script then creating a Bridge object
    virtual SDL_Renderer *GetRenderer()=0;
    virtual void AddSpriteToGame(Sprite*)=0;
    virtual void AddPhysicsToGame(Physics*, SDL_Rect*)=0;
    virtual void AddCollisionToGame(Collision*)=0;
    virtual void AddAnimatedSpriteToGame(AnimatedSprite*)=0;
    virtual void getWindowsize(int*, int*)=0;
};