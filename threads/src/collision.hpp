#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

class Collision
{
    SDL_Rect* collisionBox; //Box that will actually detect collision, does not have to line up with sprite
    vector <string>* collisionTags; //Tags that will register collisions, if null will register every collision
    string tag; //This objects tag

    public:
    bool DoNotRegisterAnyCollision = false;

    Collision(SDL_Rect* _col, string _tag, vector <string>* _colTags = nullptr)
    {
        tag = _tag;
        collisionBox = _col;
        collisionTags = _colTags;
    }

    bool CheckCollision(SDL_Rect* other)
    {
        return SDL_HasIntersection(collisionBox, other);
    }

    virtual void OnCollision(SDL_Rect* collider){};
    //!!!Temp Change!!!
    virtual void NoCollision(){};

    SDL_Rect* GetCollider(){
        return collisionBox;
    }

    vector<string>* GetColliderTags(){
        return collisionTags;
    }

    string GetObjectsTag(){
        return tag;
    }
};