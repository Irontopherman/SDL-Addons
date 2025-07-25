﻿#pragma once
#define _USE_MATH_DEFINES // math shit
#include <math.h>
//#include <iostream>
//#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL_image.h>
//CUSTOM API
//CUSTOM API
//CUSTOM API
/*
Hello source code inspectors and people
who are confused on the documentation!
This API is made by a person who is
bad at coding, so be aware!
Also, shitty comments ahead, beware!
i am BAD at writing comments
(except when i am really good
at writing comments which is when
i am confused myself)
*/

/*
Atype is a appbreviation of Actor Type, because it specifies if the acotr should have (somewhat) complex math and physics connected to it.
*/
 enum Atype {
};
//this if for is you have actors with no rotation
//and dont want to do corner calculations
typedef struct {
    char imgname[256];
    float x;
    float y;
    float angle;
    float scale;
    enum Atype actortype;

    char path[1024];//final string path
    char* oldimgname;//other path for comparing
    SDL_Surface* imgsize;// surface for getting image size quickly, you cloud use and loaded surface, but this is better for debugging
    SDL_Surface* pixeldata;//sirfacre with image data for access
    SDL_Texture* rendertextture;// texture to be rendered to the screen
    SDL_FRect coords;// coridanted stored in SDL_FRect
    //string pathhalf = "";
    //int lastsec = 0;
    bool animated;
    int width;//width of the actor's image
    int height;//height of the actor's image
    //int againtime = 0;
    float turnangle;//a second angle varible for comparison
    bool setsize;//bool for if width and height are set
    float oldangle;// a third angle varible for angle comparison
    SDL_FPoint imgcenter;//sets the the images center for math with corners and other calculation
    //SDL_FRect testrect = {};
    //SDL_Vertex collision;
    //int width;
    //int height;
    SDL_FRect corner1;//first corner of actor at top left
    SDL_FRect corner2;//second corner of actor at top right
    SDL_FRect corner3;//third corner of actor at bottom left
    SDL_FRect corner4;//fourth corner of actor at bottom right
    SDL_FRect frontcent;//a point in between top left and top right points
    //bool onetest = false;
    SDL_Vertex collide1[3];// first collide trangle, combined with rect 2 makes a real rectangle
    SDL_Vertex collide2[3];// second collide triangle, combined with rect 1 makes a real rectangle
    bool dodrawcollide;// says whether of not to draw collision of the actor
    float old_x;//an old x varible for x var storing
    float old_y;//an old y varible for y var storing
    float old_angle;//an old angle varible for angle storing
    Uint64 mktime;// is the make time of the actor, also can be used for time storing
    SDL_Texture* oldrendertext;// varible for checking of the texture has changed 
    bool maskmade;// bool for whether or not the bitmask has been initlized
    bool has_bitmask;//another bool for checking if the bitmaks has 𝒓𝒆𝒂𝒍𝒍𝒚 been initilized
    bool notcalculated;// bool for whether or not the corners have been calculated
    float this_x;// this_x is the local x coord varible using the x of the surface itself added to normal x
    float this_y;// this_y is the local y coord varible using the y of the surface itself added to normal y
    double hw;//half the width of the surface, with added scale
    double hh;//half the height of the surface, with added scale
    int totalpixels;//total number of pixels in the image
    float x1, x2, x3, y1, y2, y3;//this varible is used for math
    float a, b, c, s, t, denom;//this varible is used for math
    float s1_x, s1_y, s2_x, s2_y;//this varible is used for math
    float intersection_x, intersection_y;//this varible is used for math
    SDL_FPoint a1, a2, b1, b2;//this varible is used for math
    float thisangle;
    float corn1[2], corn2[2], corn3[2], corn4[2];
    SDL_Vertex rect1[3];
    SDL_Vertex rect2[3];
    SDL_FRect pointrect;//used for showing collison points in goodcollide() function
    double rads;
    int img_w;
    int img_h;
    float centerX, centerY;
    float dx, dy;
    float cosR, sinR;
    float rotatedX, rotatedY;
    float unscaledX, unscaledY;
    float bitmapX, bitmapY;
    int intbitmapX;
    int intbitmapY;
    bool tested;
    bool showgoodcollide;
    SDL_Renderer* drawcolrenderer;
    bool* bitmask;
    enum SDL_SCALEMODE scaletype;
} Actor;
void dir(Actor* target, char* path, size_t pathSize);
void create_actor(Actor* madeactor, char imgname[256], float x, float y, float angle, float scale, enum Atype actortype);
/*
Actor is the main class of this file, made for pure conveincense, so that you
dont have to comb throguh half good documentation to find specific funtions
to be re-used multiple times. Also comes with simple and complex collision
detetcion with images and complex bitmaps, all entirely optional.
Initlized as (first to last)
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Image name, x, y, angle, scale, actor type
*/
/*
class Actor {
    
    yes this is all wrapped into
    a bigass class, once again
    not a good programmer here,
    first time using c++,
    so cut me some slack
    
public:
*/

    /*
    const char* imgname;//name of the image (self explainitory)
    std::string dir();
    float x;//x position of actor 
    float y;//y position of actor
    float angle = 0.0;//**note: angle is not wrapped to 360
    float scale;//the scale of the actor
    Atype actortype = normal;// actortype for customization and performance
    std::string path = dir();//final string path
    std::string oldimgname = path;//other path for comparing
    SDL_Surface* imgsize = IMG_Load(path.c_str());// surface for getting image size quickly, you cloud use and loaded surface, but this is better for debugging
    SDL_Surface* pixeldata = IMG_Load(path.c_str());//sirfacre with image data for access
    SDL_Texture* rendertextture;// texture to be rendered to the screen
    SDL_FRect coords = { x,y,100,100 };// coridanted stored in SDL_FRect
    //string pathhalf = "";
    //int lastsec = 0;
    bool animated=0;
    float width = 0;//width of the actor's image
    float height = 0;//height of the actor's image
    //int againtime = 0;
    float turnangle = angle;//a second angle varible for comparison
    bool setsize = false;//bool for if width and height are set
    float oldangle = 0;// a third angle varible for angle comparison
    SDL_FPoint imgcenter = { 0,0 };//sets the the images center for math with corners and other calculation
    //SDL_FRect testrect = {};
    //SDL_Vertex collision;
    SDL_FRect corner1 = { 0, 0, 2, 2 };//first corner of actor at top left
    SDL_FRect corner2 = { 0, 0, 2, 2 };//second corner of actor at top right
    SDL_FRect corner3 = { 0, 0, 2, 2 };//third corner of actor at bottom left
    SDL_FRect corner4 = { 0, 0, 2, 2 };//fourth corner of actor at bottom right
    SDL_FRect frontcent = { 0, 0, 2, 2 };//a point in between top left and top right points
    //bool onetest = false;
    SDL_Vertex collide1[3] = {};// first collide trangle, combined with rect 2 makes a real rectangle
    SDL_Vertex collide2[3] = {};// second collide triangle, combined with rect 1 makes a real rectangle
    bool dodrawcollide = false;// says whether of not to draw collision of the actor
    float old_x = 0;//an old x varible for x var storing
    float old_y = 0;//an old y varible for y var storing
    float old_angle = 0;//an old angle varible for angle storing
    Uint64 mktime = SDL_GetTicks();// is the make time of the actor, also can be used for time storing
    SDL_Texture* oldrendertext;// varible for checking of the texture has changed 
    bool maskmade = false;// bool for whether or not the bitmask has been initlized
    bool has_bitmask = false;//another bool for checking if the bitmaks has 𝒓𝒆𝒂𝒍𝒍𝒚 been initilized
    bool notcalculated = false;// bool for whether or not the corners have been calculated
    float this_x = x + imgcenter.x;// this_x is the local x coord varible using the x of the surface itself added to normal x
    float this_y = y + imgcenter.y;// this_y is the local y coord varible using the y of the surface itself added to normal y
    double hw = (scale * width) / 2.0;//half the width of the surface, with added scale
    double hh = (scale * height) / 2.0;//half the height of the surface, with added scale

    float x1, x2, x3, y1, y2, y3;//this varible is used for math
    float a, b, c, s, t, denom;//this varible is used for math
    float s1_x, s1_y, s2_x, s2_y;//this varible is used for math
    float intersection_x, intersection_y;//this varible is used for math
    SDL_FPoint a1, a2, b1, b2;//this varible is used for math
    float thisangle;
    float corn1[2], corn2[2], corn3[2], corn4[2];
    SDL_Vertex rect1[3];
    SDL_Vertex rect2[3];
    // To implement good hitboxes you will have a script that makes a mesh of triangles based on which pixels have opacity and which do not.

    SDL_FRect pointrect;//used for showing collison points in goodcollide() function

    */

    /*
    float centerX, centerY;
    float dx, dy;
    float cosR, sinR;
    float rotatedX, rotatedY;
    float unscaledX, unscaledY;
    float bitmapX, bitmapY;
    int intbitmapX;
    int intbitmapY;
    

    /*
    angle in radians, in a 0-360 range
    
    double rads = fmod(angle, 360.0f) * (3.1415926535897932384626433832795028841 / 180.0f);
    /*
    sets the width and height of the image and displays an error message is the images cannot be loaded
    
    int img_w;
    int img_h;
    */
    void setWD(Actor *target, SDL_Renderer* renderer);
    //int img_w = width;
    //int img_h = height;
    //int totalpixels;//total number of pixels in the image

    /*
    initlizes a 1d bitmap that is the width and height of the image, all values start at 0
    */
    void makebitmask(Actor* target, bool mask[], int masksize);
    //std::vector<bool>bitmask = makebitmask();// the bitmaks if a 1D vector bool that stores if each pixel of a model has an alpha channel or not, initlized to all false
    //I AM WRITING THIS IN VIM!

    /*
    sees if a single triangle collides with another triangle and returen the first collision point as a pointer.
    */
    bool singletricollide(Actor* target, const SDL_Vertex* tri1, const SDL_Vertex* tri2, SDL_FPoint* intersectionPoint);
    /*
    This function is built to place corners and front center points so that
    other calculations can creat triangles and other various uses. This
    function does have some expensive math, so it is left out for simple
    actors, and only works for normal and complex actors
    */
    void rotatecollide(Actor* target);
    /*
    this function goes thorugh all pixels in a image
    and if theyhave an alpha channel greater than one
    it adds that value as 1 to a bitmap, else it will
    make it 0. One of the more expensize math functions,
    and cannot be used on simple actors, because the
    function uses some expensize math. Takes about 14
    milliseconds to work, so preformance could be better
    */
    void init_good_collide(Actor* target);

    /*
    draws a persice line using SDL_RenderGeometry
    Does have some displacement issues, but overall
    better for lines than default function.
    */
    void drawline(Actor* target, SDL_Renderer* renderer, SDL_FPoint p1, SDL_FPoint p2, float thickness, SDL_FColor color);

    void showcollide(Actor* target, SDL_Renderer* ren);

    /*
    simple algebra function to see if
    two lines intersect, used in goodcollide
    to find the area of bitmap overlap checking
    */
    bool lineintersect(Actor* target, const SDL_FPoint* a1, const SDL_FPoint* a2,
        const SDL_FPoint* b1, const SDL_FPoint* b2,
        SDL_FPoint* intersectionPoint);
    /*
    converts from a screen pixel to the bitmap coordinate that corrosponds to it to it
    */
    //bool tested = false;
    void scrn_to_bmp_px(Actor* target, int p_x, int p_y, int* exit_x, int* exit_y);

    bool pointinpolygon(Actor* target, const SDL_FPoint polygon[], const SDL_FPoint p, int64_t polygonsize);

    void getallpolypoints(Actor* target, const SDL_FPoint polygon[], float step, SDL_FPoint fillarray[], int polysize, int fillarraysize, int* totalamountofpoints);

    int convex_hull(SDL_FPoint* pts, int n, SDL_FPoint** hull_out);

    bool testbasiccollide(Actor* target, Actor* collider);
    /*
    finds all points where two actors intersect with a rectangle on
    the outside of the image then loops through all collision points
    and compares them to the bitmap of the actor to see if the
    bitmaps collide, which means it ignores clear pixels. This function
    is very very slow currently, cannot be used with simple actors, will
    throw logic error if you try to use it with one.
    TL;DR: false of they dont collide, true if the opaque parts of the sprites do collide
    */
    bool goodcollide(Actor* target, Actor* collider);
    /*
    update function for actors will not be drawn but still need math and updates
    */
    void update(Actor* target, SDL_Renderer* renderer);
    /*
    creates the texture of the actor that will be drawn to the screen if the texture has changed
    */
    void maketextture(Actor* target, SDL_Renderer* renderer);
    void renderdebug(Actor* target, SDL_Renderer* renderer);
    bool tricollpnt(Actor* target, const SDL_FPoint* p, const SDL_FPoint* a, const SDL_FPoint* b, const SDL_FPoint* c);
    /*
    quickly and easily draw actor to specified renderer
    automatically creating texture, texture coordnates,
    x and y calculations, and image data.
    TL;DR: draws actor to renderer
    */
    void quickdraw(Actor* target, SDL_Renderer* renderer);
    /*
    draws all points and simpe collision of the actor
    */
    void drawcollide(Actor* target, SDL_Renderer* renderer, int R, int G, int B, int A);
    /*
    basic rectangle collision, ignores angle, bitmap, and direction.
    */
    bool rectcollide(Actor* target, SDL_FRect colliderect);
//};