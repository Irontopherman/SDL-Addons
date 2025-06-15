#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL_image.h>
//#include <iostream>
#include "Actor.h"
static SDL_Window* window = NULL;
static SDL_Renderer* ren = NULL;
/* This function runs once at startup. */


bool show = false;
int width = 0;
int height = 0;
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    const char* result = SDL_GetHint("render.scale_quality");
    //SDL_Log("Current render.scale_quality: %s", result);
    if (!SDL_CreateWindowAndRenderer("Platformer", width = 1000, height = 800, false, &window, &ren)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}
bool* keystate;
void smoothinput() 
{
    show = false;
    keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_SPACE])
    {
        show = true;
    }
}
//tick sample rate: 286.3744
SDL_Event e;
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode keyevent = event->key.key;
        switch (keyevent)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
        }
        return SDL_APP_CONTINUE;
    }
    return SDL_APP_CONTINUE;
}
bool angleincrease = 1; 
bool actors_inited = false;
Actor animshower;
Actor angleshower;
Actor scaleshower;
Actor bitmaskshower;
Actor bitmaskcollider;
void init_actors()
{
    //animshower = create_actor("example.png", 200, 100, 0, 2.5f, 1 );
    create_actor(&animshower, "example.png", 200, 100, 0, 2.5f, 1);
    /*
    create_actor(&angleshower, "example.png", 75, 220, 0, 2.5f, 1);
    create_actor(&scaleshower, "example.png", 200, 220, 0, 2.5f, 1);
    create_actor(&bitmaskshower, "example.png", 325, 220, 0, 2.5f, 1);
    create_actor(&bitmaskcollider, "ship.png", 320, 245, 0, 0.5, 1);
    */
    //bitmaskshower = create_actor("example.png", 325, 220, 0, 2.5f, 1);
    //bitmaskcollider = create_actor("ship.png", 320, 245, 0, 0.5, 1);
}

void scalechange()
{
    if (angleincrease)
    {
        scaleshower.scale += 0.035;
        if (scaleshower.scale > 5) angleincrease = 0;
    }
    else {
        scaleshower.scale -= 0.035;
        if (scaleshower.scale < 2.5) angleincrease = 1;
    }
}


void rendertext()
{
    SDL_SetRenderScale(ren, 3, 3);
    SDL_RenderDebugText(ren, 52.5, 120, "Angle");
    SDL_RenderDebugText(ren, 150, 120, "Scale");
    SDL_RenderDebugText(ren, 225, 120, "Collision");
    SDL_RenderDebugText(ren, 100, 50, "Animation");
    SDL_RenderDebugText(ren, 150, 10, "Features include:");
    SDL_SetRenderScale(ren, 2, 2);
}
void showfeatures()
{
    animshower.animated = true;
    rendertext();
    //animshower.quickdraw(ren);
    quickdraw(&animshower, ren);
    //quickdraw(&angleshower, ren);
    angleshower.angle += 2;
    //quickdraw(&scaleshower, ren);
    //scaleshower.quickdraw(ren);
    scalechange();
    //quickdraw(&bitmaskshower, ren);
    //bitmaskshower.quickdraw(ren);
    //quickdraw(&bitmaskcollider, ren);
    //bitmaskcollider.quickdraw(ren);
    
    
    
    //bitmaskshower.angle++;
    //bitmaskshower.showcollide(ren);
    //bitmaskshower.goodcollide(bitmaskcollider);
}
void rendernote()
{
    SDL_RenderDebugText(ren, 10, 350, "Note: this will be resource intensive for learning. disable");
    SDL_RenderDebugText(ren, 10, 365, "the showcollide and goocollide functions if you want a");
    SDL_RenderDebugText(ren, 10, 380, "performance gauge, since they are for show.");
}
int64_t nextframe=0;
int frame = 1;
char animframe[256] = "Animation";
SDL_AppResult SDL_AppIterate(void* appstate)
{
    //SDL_Log(SDL_GetError());
    //using namespace std;
    if (!actors_inited)
    {
        init_actors();
        actors_inited = true;
    }
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_SetRenderScale(ren, 2, 2);
    SDL_RenderDebugText(ren, 10, 10, "Escape To Quit");
    SDL_RenderDebugText(ren, 10, 20, "Space For Features");
    rendernote();
    smoothinput();
    if(show)showfeatures();
    
    if (SDL_GetTicks() > nextframe&&show)
    {
        if (frame >= 7)frame = 0;
        frame++;
        animframe[0] = '\0';
        char framestr[32] = { 0 };
        //strcpy_s(animframe, sizeof(animframe), "Animation", sizeof("Animation"));
        strcpy_s(animframe, sizeof(animframe), "Animation");
        //animframe = "Animation";
        //animframe+frame;
        sprintf_s(framestr, sizeof(framestr), "%d", frame);
        strcat_s(animframe, sizeof(animframe), framestr);
        //animframe+".png";
        strcat_s(animframe, sizeof(animframe), ".png");
        //for (int uptstr = 13; uptstr >= 0; uptstr--) animshower.imgname[uptstr] = animframe[uptstr];
        strcpy_s(animshower.imgname, sizeof(animshower.imgname), animframe);
        //SDL_Log(animshower.imgname);
        //SDL_Log(animframe);
        //std::cout << animframe.c_str() << "\n";
        nextframe = SDL_GetTicks() + 100;
    }
    
    SDL_SetRenderScale(ren, 4.0f, 4.0f);
    SDL_SetRenderDrawColor(ren, 0, 125, 200, 255);
    SDL_RenderPresent(ren);
    SDL_Delay(8);
    SDL_SetWindowTitle(window, "Asteriods");
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
