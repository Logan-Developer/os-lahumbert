#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.hpp"
#include "Sprite.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "ground.hpp"
#include "key.hpp"

using namespace std;

// Declarations
void level1(Game &firstGame);
void level2(Game &firstGame);
void level3(Game &firstGame);

int main()
{
    Game firstGame;

    // Temporary way to call levels, will change later to read from file.
    //level1(firstGame);
    //level2(firstGame);
    level3(firstGame);

    return 0;
}


void level1(Game &firstGame){
    SDL_Renderer* renderer;
    Bridge* bridge = firstGame.GetBridge();
    renderer = firstGame.GetRenderer();
    // Level 1
    Sprite main_background(renderer, "gfx/main_background.png", 3640, 2160);
    Sprite background(renderer, "gfx/background1.png", 1920, 1080);
    firstGame.AddSpriteToGame(&background);

    Ground ground(bridge, "gfx/background1_ground.png", 909, 132, 1011, 948);
    Ground sub(bridge, "gfx/clear.png", 403, 250, 0, 639);
    Ground sub2(bridge, "gfx/clear.png", 483, 150, 403, 833);
    Ground water(bridge, "gfx/clear.png", 200, 150, 800, 900);
    Player player(bridge, 130, 253, 200, 400);


    firstGame.player = &player;
    firstGame.Loop();
}

// Level 2
void level2(Game &firstGame){
    SDL_Renderer* renderer;
    renderer = firstGame.GetRenderer();
    Bridge* bridge = firstGame.GetBridge();
    // Level 2
    Sprite background(renderer, "gfx/background2.png", 1920, 1080);
    firstGame.AddSpriteToGame(&background);

    Ground ground(bridge, "gfx/ground1.png", 805, 131, 0, 949);
    Ground ground2(bridge, "gfx/ground2.png", 650, 131, 1270, 949);
    Ground ladder(bridge, "gfx/bridge_ladder.png", 466, 131, 806, 949);

    ladder.DoNotRenderSprite = true;
    ladder.DoNotRegisterAnyCollision = true;

    Player player(bridge, 130, 253, 300, 944);
    Enemy enemy(bridge, "gfx/enem_anim", "enemy", 6, 430, 360, 1700, 500);
    Key key(&ladder, bridge, "gfx/red_button.png", 20, 20, 111, 734);

    firstGame.player = &player;
    firstGame.enemy = &enemy;
    firstGame.Loop();
}

void level3(Game &firstGame){
    SDL_Renderer* renderer;
    renderer = firstGame.GetRenderer();
    Bridge* bridge = firstGame.GetBridge();
    // Level 3
    Sprite main_background(renderer, "gfx/main_background.png", 3640, 2160);
    Sprite background(renderer, "gfx/background3.png", 1920, 1080);
    firstGame.AddSpriteToGame(&main_background);
    firstGame.AddSpriteToGame(&background);

    Ground ground(bridge, "gfx/ground3.png", 1910, 106, 0, 974);
    Ground roof(bridge, "gfx/shedroof.png", 302, 525, 1610, 160);
    Ground railing(bridge, "gfx/railing.png", 201, 240, 1440, 440);
    Ground ladder(bridge, "gfx/shed_ladder.png", 107, 500, 1350, 550);
    Key key(&ladder, bridge, "gfx/bridge_ladder.png", 135, 36, 11, 930);

    ladder.DoNotRenderSprite = true;
    ladder.DoNotRegisterAnyCollision = true;

    Player player(bridge, 120, 233, 600, 944);
    Enemy enemy(bridge, "gfx/enem_anim", "enemy", 6, 430, 360, 1700, 500);

    firstGame.player = &player;
    firstGame.enemy = &enemy;
    firstGame.Loop();
}