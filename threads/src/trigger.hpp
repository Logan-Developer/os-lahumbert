#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

class Trigger
{
    public:
    //Trigger(){};
    virtual bool IsTriggered()  = 0;
    virtual void WhenTriggered() = 0;
};
