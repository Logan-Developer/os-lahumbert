#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

class Sprite
{
    protected:
    SDL_Texture* TexToLoad;
    SDL_Rect src, transform;
    SDL_Renderer* renderer;
    const char* path;
    int facingLeft=false;

    int scrollSpeedX;
    // int scrollSpeedY;
    // int scrollAmountY;

    void setSpriteSRCProperties()
    {
        SDL_QueryTexture(TexToLoad, NULL, NULL, &src.w, &src.h); //gets the texture's width and height cut off so can display full image
        src.x = 0;
        src.y = 0;
    }

    //uses SDL image and loads a texture from a given file path and returns a sdl_texture
    SDL_Texture* loadTexture(const char* p_filePath)
    {
        SDL_Texture* texture = NULL;
        SDL_Surface* surface = IMG_Load(p_filePath);
        
        if(surface == NULL)
        {
            cout << "Failed to load a texture. Error: " << SDL_GetError() << endl;
            texture = IMG_LoadTexture(renderer, "gfx/noTexture.png");
        }

        // Convert surface to texture
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) 
        {
            cout << "Failed to create texture from surface(" << p_filePath << "): " << SDL_GetError() << endl;
            SDL_FreeSurface(surface);
            return nullptr;
        }

        // Free the surface, as it's no longer needed
        SDL_FreeSurface(surface);
        return texture;
    }

    // Initializes the sprite so that it is ready to be used in game (do it before the loop)
    void initSprite(SDL_Renderer *ren)
    {
        renderer = ren;
        TexToLoad = loadTexture(path);
        setSpriteSRCProperties();
    }

    public:
    bool DoNotRenderSprite = false;

    //creates the sprite and its properties
    Sprite(SDL_Renderer* ren, const char* p_filePath, int _width, int _length, int _x = 0, int _y = 0,  int _scrollSpeedX = 1)
    {
        path = p_filePath;
        initSprite(ren);
        transform.x = _x;
        transform.y = _y;
        transform.w = _width;
        transform.h = _length;
        scrollSpeedX = _scrollSpeedX;
        //scrollSpeedY = _scrollSpeedY;
        //scrollAmountY = 0;
    }

    // temp (fix seg fault)
    // ~Sprite() 
    // {
    //     SDL_DestroyTexture(TexToLoad);
    // }    

    SDL_Rect* getSRC(){return &src;}
    void setTransform(SDL_Rect *tr){transform=*tr;}
    SDL_Rect* getTransform(){return &transform;}
    SDL_Texture* getTexture(){return TexToLoad;}
    bool GetDirection(){return facingLeft;}
    void IsFacingLeft(bool b){facingLeft=b;}

    //Scrolls the sprite by the given amount
    void scrollSprite(int _x, int backgroundWidth, int backgroundX, int windowWidth)
    {
        // scroll only in the limit of the background
        if (backgroundX <= 0 || backgroundX + backgroundWidth >= windowWidth)
        {
            return;
        }
        transform.x += _x * scrollSpeedX;
        //transform.y += _y * scrollSpeedY;
        //scrollAmountY += _y;
    }

    void getSpriteSize(int* w, int* h)
    {
        *w = transform.w;
        *h = transform.h;
    }
    
    // //Scrolls the sprite Y based on the amount of scrolling that has been done
    // void scrollBackSpriteY()
    // {
    //     if (scrollAmountY > 0)
    //     {
    //         transform.y -= scrollSpeedY;
    //         scrollAmountY -= scrollSpeedY;

    //         if (scrollAmountY < 0)
    //         {
    //             scrollAmountY = 0;
    //         }
    //     }
    // }
};