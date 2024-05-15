#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL_mixer.h>
#include <SDL_mutex.h>
#include "Sprite.hpp"
#include "Physics.hpp"
#include "GameBridge.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Collision.hpp"
#include "AnimatedSprite.hpp"

using namespace std;

class Game:public Bridge
{
    SDL_Renderer *ren;
    SDL_Window *window;
    vector <Sprite*> spriteList;
    vector <pair<Physics*, SDL_Rect*>> physicsList;
    vector <Collision*> colliderList;
    vector <AnimatedSprite*> animatedSpriteList;
    Sprite* background;
    SDL_mutex *sprites_mutex;
    SDL_mutex *physics_mutex;
    SDL_mutex *collision_mutex;

    Mix_Chunk* sound;

    int scrollX = 0;
    int scrollY = 0;

    int windowWidth, windowHeight;

    bool done=false;


    //Makes the window that we put everything on also creates the renderer that draws everything on window
    int MakeWindow(string gameName, int w, int h)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
            cout << "Failed to initialize the SDL2 library\n";
            return -1;
        }

        if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
            cerr << "Failed to initialize SDL2 mixer: " << Mix_GetError() << endl;
            return -1;
        }

        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);

        w = mode.w; h = mode.h;
        window = SDL_CreateWindow(gameName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_FULLSCREEN);

        if(!window)
        {
            cout << "Failed to create window\n";
            return -1;
        }

        // SDL_Surface *window_surface = SDL_GetWindowSurface(window);

        // if(!window_surface)
        // {
        //     cout << "Failed to get the surface from the window\n";
        //     return -1;
        // }

        ren = SDL_CreateRenderer(window,-1,0);
        if (!ren)
        {
            cout << "Failed to create renderer: " << SDL_GetError() << endl;
            return -1;
        }

        if(!ren)
        {
            cerr << "Failed to create renderer\n";
            return -1;
        }

        // Insert audio file
        sound = Mix_LoadWAV("gfx/audio/ocean.wav");
        if(!sound){
            cerr << "Failed to load audio: " << Mix_GetError() << endl;
            return -1;
        }
        Mix_VolumeChunk(sound, 5);
        /*
        Mix_Music *getMusic(string fname){
            if(tracks.find(fname) == tracks.end()){
                Mix_Music *gMusic = Mix_LoadMUS(fname.c_str());
                if(gMusic)
                    tracks[fname] = gMusic;
            }
            return tracks[fname];
        }

        Mix_Chunk *getEffect(string fname){
            if(effects.find(fname) == effects.end()){
                Mix_Chunk *gChunk = Mix_LoadWAV(fname.c_str());
                if(gChunk)
                    effects[fname] = gChunk;
            }
            return effects[fname];
        }
        */

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        return 0;
    }
    public:
    Player *player;/// !!!Temp Change!!!
    Enemy *enemy;

    Game(string _gameName="My Game", int _widthOfWindow=680, int _hieghtOfWindow=480)
    {
        MakeWindow(_gameName, _widthOfWindow, _hieghtOfWindow);
        sprites_mutex = SDL_CreateMutex();
        physics_mutex = SDL_CreateMutex();
        collision_mutex = SDL_CreateMutex();
    }


    //Adds sprite to list of sprites to be used in game
    void AddSpriteToGame(Sprite* ent)
    {
        SDL_LockMutex(sprites_mutex);
        spriteList.push_back(ent);
        SDL_UnlockMutex(sprites_mutex);
    }

    //Adds Animated sprite so that sprites can be cycled in game
    void AddAnimatedSpriteToGame(AnimatedSprite* aniSpr)
    {
        SDL_LockMutex(sprites_mutex);
        for(auto spr: aniSpr->getSprites())
        {
            AddSpriteToGame(spr);
        }
        animatedSpriteList.push_back(aniSpr);
        SDL_UnlockMutex(sprites_mutex);
    }

    //Puts objects physics script along with corisponding transform into game so it can be updated durring loop
    void AddPhysicsToGame(Physics* phys, SDL_Rect* trans)
    {
        physicsList.push_back(make_pair(phys,trans));
    }

    //Adds llcoision object to the game
    void AddCollisionToGame(Collision* col)
    {
        colliderList.push_back(col);
    }

    //Check if an sprite needs to be changed
    void CycleAnimatedSprites()
    {
        for(auto aSprites: animatedSpriteList)
        {
            aSprites->CycleSprites();
            aSprites->UpdateTransform();
        }
    }
    //Renders the image then displays image onto window
    void RefreshSprites()
    {
        //Updates all animation sprites as needed
        CycleAnimatedSprites();
        //updates all sprites transform
        for(auto sprite: spriteList)
        {
        //    int backgroundWidth, backgroundHeight;
        //    background->getSpriteSize(&backgroundWidth, &backgroundHeight);
        //    sprite->scrollSprite(scrollX, backgroundWidth, background->getTransform()->x, windowWidth);

            // if (scrollX == 0 && scrollY == 0)
            //     sprite->scrollBackSpriteY();

            //updates all sprites transform
            if(!sprite->DoNotRenderSprite)
                if(!sprite->GetDirection())
                    SDL_RenderCopy(ren, sprite->getTexture(), sprite->getSRC(), sprite->getTransform());
                else
                    SDL_RenderCopyEx(ren, sprite->getTexture(), sprite->getSRC(), sprite->getTransform(), 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        //puts the updated sprite transforms on the screen
        SDL_RenderPresent(ren);
    }

    //Goes through all entities and runs their update function
    void UpdatePhysics(float dt)
    {
        for(auto phys: physicsList)
        {
            phys.first->update(dt, phys.second);
        }

    }

    //Checks if each colliders in the list have contacted each other
    void CheckCollisions()
    {
        for(auto col: colliderList)
        {
            if(!col->DoNotRegisterAnyCollision)
            {
                for(auto potCol: colliderList)
                {
                    if(potCol != col && !potCol->DoNotRegisterAnyCollision)
                    {
                        if(col->GetColliderTags() != nullptr)
                        {
                            string potColTag = potCol->GetObjectsTag();
                            for(auto colTag : *(col->GetColliderTags()))
                            {
                                if(colTag == potColTag)
                                {
                                    if(col->CheckCollision(potCol->GetCollider()))
                                        col->OnCollision(potCol->GetCollider());
                                    else
                                        col->NoCollision();
                                }
                            }
                        }
                        else
                        {
                            if(col->CheckCollision(potCol->GetCollider()))
                                col->OnCollision(potCol->GetCollider());
                            else
                                col->NoCollision();
                        }
                    }
                }
            }
        }
    }


    static int draw_Thread(void *lpFirst)
    {
        Game *fp=(Game *)lpFirst;
        while (!fp->done)
        {
            fp->Clear();
            // Lock to refresh sprites
            SDL_LockMutex(fp->sprites_mutex);
            fp->RefreshSprites();
            SDL_UnlockMutex(fp->sprites_mutex);
            // Unlock
            SDL_Delay(17);
            //Delay to some reasonable time to keep a good frame rate
        }
        return 0;
    }

    static int PhysicsUpdate_Thread(void *lpFirst)
    {
        Game *fp=(Game *)lpFirst;
        int lastTime=SDL_GetTicks();
        while(!fp->done)
        {
        int newTime=SDL_GetTicks();
        int dmillis=newTime-lastTime;
        float dt=(float)(newTime-lastTime)/1000.0;
        lastTime=newTime;
        // Lock to update physics
        SDL_LockMutex(fp->physics_mutex);
        fp->UpdatePhysics(dt);
        SDL_UnlockMutex(fp->physics_mutex);
        // Unlock
        int delayTime=33-dmillis;
        if(delayTime<1)delayTime=1;
        SDL_Delay(delayTime);
        //Some reasonable delay to keep physics accurate
        }
        return 0;
    }

    static int CollisionUpdate_Thread(void *lpFirst)
    {
        Game *fp=(Game *)lpFirst;
        while(!fp->done)
        {
            // Lock to check collisions
            SDL_LockMutex(fp->collision_mutex);
            fp->CheckCollisions();
            SDL_UnlockMutex(fp->collision_mutex);
            // Unlock
            int delayTime=33;
            SDL_Delay(delayTime);
            //Some reasonable delay to keep physics accurate
        }
        return 0;
    }

    SDL_Renderer* GetRenderer(){return ren;}
    Bridge* GetBridge(){return this;}
    //clears the window
    void Clear() {SDL_RenderClear(ren);}
    //destoys the window
    void Clean(){SDL_DestroyWindow(window);}

    //The Game Loop
    void Loop()
    {
        SDL_Event e;
        int lastTime=SDL_GetTicks();
        int firstTime=lastTime;
        float dt;

        //SDL_CreateThread(Game::draw_Thread,"DrawThread",this);
        SDL_CreateThread(Game::PhysicsUpdate_Thread, "PhysicsThread", this);
        SDL_CreateThread(Game::CollisionUpdate_Thread, "CollisionThread", this);

        // Loops the audio until game is closed out
        Mix_PlayChannel(-1, sound, -1);

        for(auto x:animatedSpriteList)
        {

        }

        while(!done)
        {
            if (SDL_PollEvent(&e))
            {
                if (e.type==SDL_WINDOWEVENT) // Here I could handle all window events
                {
                    if (e.window.event==SDL_WINDOWEVENT_CLOSE)
                        done=true;
                }

                if(e.type == SDL_KEYDOWN){ // Someone pressed a key
                    enemy->movement();
                    if(e.key.keysym.sym==SDLK_a) {
                        player->doMove(LEFT);
                        scrollX = 1;
                    }
                    else if(e.key.keysym.sym==SDLK_w) {
                        player->doMove(UP);
                        scrollY = 1;
                    }
                    else if(e.key.keysym.sym==SDLK_s) {
                        player->doMove(DOWN);
                        scrollY = -1;
                    }
                    else if(e.key.keysym.sym==SDLK_d) {
                        player->doMove(RIGHT);
                        scrollX = -1;
                    }
                    else if(e.key.keysym.sym==SDLK_i) {
                        player->doMove(I);
                    }
                    else if(e.key.keysym.sym == SDLK_SPACE){
                        cout << "jumping" << endl;
                        player->doMove(JUMP);
                        scrollY = 1;
                    }

                    else if(e.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                }
                else if (e.type == SDL_KEYUP)
                {
                    if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d)
                    {
                        scrollX = 0;
                    }
                    if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s  || e.key.keysym.sym == SDLK_SPACE)
                    {
                        scrollY = 0;
                    }
                    if (e.key.keysym.sym==SDLK_a)player->doMove(RLEFT);
                    if (e.key.keysym.sym==SDLK_w)player->doMove(RUP);
                    if (e.key.keysym.sym==SDLK_s)player->doMove(RDOWN);
                    if (e.key.keysym.sym==SDLK_d)player->doMove(RRIGHT);
                    if (e.key.keysym.sym==SDLK_i)player->doMove(RI);
                }
            }
            Clear();
            RefreshSprites();
            /*int newTime=SDL_GetTicks();
            dt=(float)(newTime-lastTime)/1000.0;
            lastTime=newTime;
            CheckCollisions();
            UpdatePhysics(dt); */
        }
        //!!!out of game loop!!!
        Clear();
        Clean();
    }

    void getWindowsize(int* w, int* h)
    {
        SDL_GetWindowSize(window, w, h);
    }

    // set the background image
    void setBackground(Sprite* bg)
    {
        background = bg;
        AddSpriteToGame(bg);
    }
};
