#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL_image.h>
#include <iostream>
#include "Actor.hpp"
static SDL_Window* window = NULL;
static SDL_Renderer* ren = NULL;
/* This function runs once at startup. */


bool show = false;
int width = 0;
int height = 0;
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Platformer", width = 1000, height = 800, SDL_WINDOW_FULLSCREEN == false, &window, &ren)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}
const bool* keystate = SDL_GetKeyboardState(NULL);
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
    using namespace std;
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
bool angleincrease = 1; int offset = 50;
Actor animshower = { "example.png", 150 + offset, 100, 0, 2.5f };
Actor angleshower = { "example.png", 25+offset, 220, 0, 2.5f };
Actor scaleshower = { "example.png", 150+offset, 220, 0, 2.5f};
Actor bitmaskshower = { "example.png", 275+offset, 220, 0, 2.5f };
Actor bitmaskcollider = { "ship.png", 270+offset, 245, 0, 0.5 };
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
    SDL_RenderDebugText(ren, 12.5 + offset, 120, "Angle");
    SDL_RenderDebugText(ren, 100 + offset, 120, "Scale");
    SDL_RenderDebugText(ren, 175 + offset, 120, "Collision");
    SDL_RenderDebugText(ren, 50 + offset, 50, "Animation");
    SDL_RenderDebugText(ren, 100, 10, "Features include:");
    SDL_SetRenderScale(ren, 2, 2);
}
void showfeatures()
{
    animshower.animated = true;
    rendertext();
    animshower.quickdraw(ren);
    angleshower.quickdraw(ren);
    angleshower.angle += 2;
    scaleshower.quickdraw(ren);
    scalechange();
    bitmaskshower.quickdraw(ren);
    bitmaskcollider.quickdraw(ren);
    bitmaskshower.angle++;
    //bitmaskshower.showcollide(ren);
    bitmaskshower.goodcollide(bitmaskcollider);
}
void rendernote()
{
    SDL_RenderDebugText(ren, 10, 350, "Note: this will be resource intensive for learning. disable");
    SDL_RenderDebugText(ren, 10, 365, "the showcollide and goocollide functions if you want a");
    SDL_RenderDebugText(ren, 10, 380, "performance gauge, since they are for show.");
}
int64_t nextframe=0;
int frame = 1;
std::string animframe = "animation";
SDL_AppResult SDL_AppIterate(void* appstate)
{
    using namespace std;
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
        animframe = "Animation";
        animframe.append(std::to_string(frame));
        animframe.append(".png");
        animshower.imgname = animframe.c_str();
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
