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

enum Move {UP,DOWN,RIGHT,LEFT,RUP,RDOWN,RRIGHT,RLEFT,JUMP,I,RI};
class Moveable {
  public:
  virtual void doMove(Move m)=0;
  virtual ~Moveable(){};
};


class Player: public AnimationManager,public Physics,public Collision
{
    vector<string>tags={"ground", "enemy"};
    const int WALKSPEED=300;
    SDL_Rect pTransform;
    bool isFacingLeft=false;

    public:
    Player(Bridge* b, int width, int height, int posX=0, int posY=0):
    AnimationManager(b,makeTransform(width, height, posX,posY)), Physics(posX,posY,0,10,0.0,500.0),
    Collision(&pTransform,"player",&tags)
    {
        b->AddCollisionToGame(this);
        b->AddPhysicsToGame(this, &pTransform);
        AddAnimation("Idle",b->GetRenderer(), "gfx/PlayerAnim/idle", "character", 2, 750,  &pTransform);
        AddAnimation("Walk",b->GetRenderer(), "gfx/PlayerAnim/walk", "character", 7, 200, &pTransform);
        SetCurrentAnimation("Idle");

        int windowWidth, windowHeight;
        b->getWindowsize(&windowWidth, &windowHeight);

    // x = windowWidth/2;
    // y = windowHeight/2;
    }

    SDL_Rect* makeTransform(int width, int height, int posX, int posY)
    {
        pTransform.x = posX;
        pTransform.y = posY;
        pTransform.w = width;
        pTransform.h = height;
        return &pTransform;
    }


    void doMove(Move m)
    {
        //if (m==UP) y-=5;
        //else if (m==DOWN) y+=5;
        if (m==LEFT)
        {
        vx=-WALKSPEED;
        isFacingLeft=true;
        SetCurrentAnimation("Walk");
        }
        else if (m==RIGHT)
        {
        vx=WALKSPEED;
        isFacingLeft=false;
        SetCurrentAnimation("Walk");
        }
        else if (m==JUMP) vy-=300;
        else if (m==RLEFT || m==RRIGHT)
        {
        SetCurrentAnimation("Idle");
        vx=0;
        }
        AnimationFacingLeft(isFacingLeft);
        //else if(m==I)
        //else if(m==RI)
    }

    void OnCollision(SDL_Rect* objCollided)
    {
        int lowerBoundery = pTransform.y + pTransform.h;

        if(lowerBoundery >= objCollided->y)
        {
            y=objCollided->y - pTransform.h;
            vy=0;
            stopY=true;
        }
    }

    //!!!temp change!!!
    virtual void NoCollision()
    {
        stopY=false;
        stopX=false;
    }
};
