#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "Sprite.hpp"

using namespace std;

class AnimatedSprite
{
    vector<Sprite*>frames; //List of all frames for animation
    int currentSprite=0; //Starting point on initial
    int timePerFrame; //in miliseconds
    int amountofFrames=0;

    bool Looping=true;

    //For time to change frame
    int lastTime=0;
    int changeFrame=0;

    Sprite* Cspr=nullptr; //Short for currentFrame
    Sprite* Lspr=nullptr; //Short for lastFrame

    SDL_Rect* sTransform; //The super transform, the transform(rect) of the super object so that the all sprites can be in sync

    bool StopRendering=false;
    public:
    bool PauseAnimation=false;
    AnimatedSprite(SDL_Renderer* r, const char* folderPath, const char* nameOfSprite, int _amountOfFrames, int _timePerFrame, SDL_Rect* _sTransform, bool _loop=true)
    {
        sTransform=_sTransform;
        amountofFrames=_amountOfFrames;
        Looping=_loop;
        timePerFrame=_timePerFrame;
        changeFrame=timePerFrame;

        //Creates the file path for all frames and creates all sprites then added in the frames vector\
        //Also turns all sprites to not render themselves
        for(int i=0;i<_amountOfFrames;i++)
        {
            stringstream ss;
            ss << folderPath << "/" << nameOfSprite << i << ".png";
            string filePath = ss.str();
            Sprite* frame = new Sprite(r, filePath.c_str(), sTransform->w, sTransform->h, sTransform->x, sTransform->y);
            frame->DoNotRenderSprite=true;
            frames.push_back(frame);
        }
        Cspr=frames[0]; //Sets current frame to first frame
        Lspr=frames[amountofFrames-1]; //Sets last frame to last frame
        Cspr->DoNotRenderSprite=false; //Sets the current Sprite to render itself
    }

    //Returns the frames vector
    vector<Sprite*> getSprites()
    {
        return frames;
    }

    //Updates all frames' transforms so that they will all be in sync
    void UpdateTransform()
    {
        for(auto frms:frames)
        {
            frms->setTransform(sTransform);
        }
    }

    //Stops this animation from rendering used for switching animations
    void DoNotRender()
    {
        for(auto spr:frames)
        {
            spr->DoNotRenderSprite=true;
        }
        StopRendering=true;
    }

    //Starts animation to running again
    void StartRendering()
    {
        StopRendering=false;
        Cspr=frames[0]; //Sets current frame to first frame
        Lspr=frames[amountofFrames-1]; //Sets last frame to last frame
        Cspr->DoNotRenderSprite=false; //Sets the current Sprite to render itself
        //cout << timePerFrame << endl;
    }

    //Changes what current frame is being shown
   void CycleSprites()
    {
        if (!StopRendering && !PauseAnimation)
        {
            if (SDL_GetTicks() >= changeFrame)
            {
                Lspr = Cspr;
                if (currentSprite < amountofFrames - 1)
                {
                    currentSprite += 1;
                }
                else
                {
                    if (Looping)
                    {
                        currentSprite = 0;
                    }
                }
                // cout << "currentSprite:" << currentSprite << endl;
                Cspr = frames[currentSprite];
                Cspr->DoNotRenderSprite = false;
                Lspr->DoNotRenderSprite = true;
                // Update changeFrame to the current time plus timePerFrame
                changeFrame = SDL_GetTicks() + timePerFrame;
            }
        }
    }
};



