#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "Collision.hpp"
#include "Sprite.hpp"
#include "Trigger.hpp"
#include "Ground.hpp"
#include "GameBridge.hpp"


class Key: public Sprite, public Collision
{
    vector<string>tags = {"player"};
    Ground* g;
    public:
    Key(Ground* ground, Bridge* b, const char* filePath, int width, int height, int posX = 0, int posY = 0):
    Sprite(b->GetRenderer(), filePath, width, height, posX, posY), Collision(getTransform(),"key", &tags)
    {
        g = ground;
        b->AddCollisionToGame(this);
        b->AddSpriteToGame(this);
    }

    void OnCollision(SDL_Rect* collider)
    {
        DoNotRenderSprite = true;
        g->DoNotRenderSprite = false;
        g->DoNotRegisterAnyCollision = false;
    }
};