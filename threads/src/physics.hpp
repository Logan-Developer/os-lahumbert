#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;
class Physics
{
    protected:
    float x=0,y=0;
    float vx,vy=0;
    float ax,ay;
    bool stopY=false;
    bool stopX=false;

    virtual void updatePosition(float dt)
    {
        if(!stopX)
        {
            vx += ax * dt;
            x += vx * dt;
        }

        if(!stopY)
        {
            vy += ay * dt;
            y += vy * dt;
        }
    }

   public:

    //Sets initial forces, transX/transY just tells where object on the screen is initially,
    Physics(int transX, int transY, float xVel=0, float yVel=0, float xAcc=0, float yAcc=0)
    {
        x=transX;
        y=transY;
        vx=xVel;
        vy=yVel;
        ax=xAcc;
        ay=yAcc;
    }

    //Call every frame to update transform of object
    void update(float dt, SDL_Rect* transform)
    {
        updatePosition(dt);
        transform->x=(int)x;
        transform->y=(int)y;
        //cout << "Transform X:" + to_string(transform->x) + "Transform Y:" + to_string(transform->y) << endl;
    }
};
