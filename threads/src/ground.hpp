#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Sprite.hpp"
#include "Physics.hpp"
#include "Collision.hpp"
#include "GameBridge.hpp"

class Ground: public Sprite, public Collision
{
    public:
    Ground(Bridge* b, const char* filePath, int width, int height, int posX = 0, int posY = 0)
    : Sprite(b->GetRenderer(), filePath, width, height, posX, posY), Collision(getTransform(), "ground")
    {
        b->AddSpriteToGame(this);
        b->AddCollisionToGame(this);
    }
    //void OnCollision(SDL_Rect* obj){cout << "GroundDetect" << endl;}
};