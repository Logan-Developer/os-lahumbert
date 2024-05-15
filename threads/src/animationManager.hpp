#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "GameBridge.hpp"
#include "AnimatedSprite.hpp"

using namespace std;

class AnimationManager
{
    Bridge* b;
    SDL_Rect* sTransform;
    map<string, AnimatedSprite*> animationList;
    string currentAnimation="";
    public:
    AnimationManager(Bridge* _b, SDL_Rect* _sTransform)
    {
        b=_b;
        sTransform = _sTransform;
    }

    void AddAnimation(string _nameOfAnimation, SDL_Renderer* r, const char* folderPath, const char* nameOfSprite, int _amountOfFrames, int _timePerFrame,
                      SDL_Rect* _sTransform, bool _looping=true)
    {
        AnimatedSprite* newAnim = new AnimatedSprite(r,folderPath,nameOfSprite,_amountOfFrames,_timePerFrame, _sTransform,_looping);
        newAnim->DoNotRender();
        b->AddAnimatedSpriteToGame(newAnim);
        animationList.insert(make_pair(_nameOfAnimation, newAnim));
    }

    void SetCurrentAnimation(string animationName)
    {
        if(animationName!=currentAnimation)
        {
            if(animationList[animationName]!=nullptr)
            {
                if(currentAnimation.length()!=0)
                {
                    animationList[currentAnimation]->DoNotRender();
                }
                animationList[animationName]->StartRendering();
                currentAnimation=animationName;
            }
            else
            {
                cout << "Could not find that animation" << endl;
            }
        }
    }

    void AnimationFacingLeft(bool dir)
    {
        for(auto sprite: animationList[currentAnimation]->getSprites())
        {
            sprite->IsFacingLeft(dir);
        }
    }
};
