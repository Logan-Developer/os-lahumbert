#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "GameBridge.hpp"
#include "AnimationManager.hpp"
#include "Physics.hpp"
#include "Collision.hpp"

using namespace std;


class Enemy: public AnimationManager, public Physics, public Collision
{
    vector<string> tags = {"ground", "player"};
    const int WALKSPEED = 180;
    SDL_Rect pTransform;
    
    public:
    Enemy(Bridge* b, const char* filePath, const char* name, int frames, int width, int height, int posX = 0, int posY = 0)
    :AnimationManager(b, makeTransform(width, height, posX, posY)), Physics(posX, posY, 0, 10, 0.0, 500.0), 
    Collision(&pTransform, "enemy", &tags)
    {
        b->AddCollisionToGame(this);
        b->AddPhysicsToGame(this, &pTransform);
        AddAnimation("Test", b->GetRenderer(), "gfx/enem_anim", "enemy", 2, 750, &pTransform);
        SetCurrentAnimation("Test");
    }

    SDL_Rect* makeTransform(int width, int height, int posX, int posY)
    {
        pTransform.x = posX;
        pTransform.y = posY;
        pTransform.w = width;
        pTransform.h = height;

        return &pTransform;
    }

    void movement(){
        vx = -WALKSPEED;
    }

    void OnCollision(SDL_Rect* objCollided)
    {
        int lowerBoundary = pTransform.y + pTransform.h;
        
        if(lowerBoundary >= objCollided->y){
            y = objCollided->y - pTransform.h;
            vy = 0;
            stopY = true;
        }
    }

    virtual void NoCollision()
    {
        stopY = false;
        stopX = false;
    }
};