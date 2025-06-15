#define _USE_MATH_DEFINES // math shit
#include <SDL3/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "Actor.h"
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
//this if for is you have actors with no rotation
//and dont want to do corner calculations

/*
Actor is the main class of this file, made for pure conveincense, so that you
dont have to comb throguh half good documentation to find specific funtions
to be re-used multiple times. Also comes with simple and complex collision
detetcion with images and complex bitmaps, all entirely optional.
Initlized as (first to last)
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Image name, x, y, angle, scale, actor type
*/
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
/*
enum Atype {
    simple,//this is for actors that dont need expensize vecotr math and calculas for points and angles and fancy crap like that.
    normal,// normal actors, use some expansive math, but nothing too crazy. Still perfor fine and can have thousands with no performace deficite
    /*Complex actors are not recommended to be used
    unless you have something really complex to calculate.
    Very inefficent, in fact im not sure why i made this an option,
    but it is here for those who wish to use it
    complex
};
*/


void dir(Actor* target, char *path, size_t pathSize) {
    char buffer[1024];

    if (GetCurrentDir(buffer, sizeof(buffer)) != NULL) {
        strcat_s(buffer, sizeof(buffer), "\\images\\");
        //SDL_Log(target->imgname);
        strcat_s(buffer, sizeof(buffer), target->imgname);
        strcpy_s(path, pathSize, buffer);
    }
    else {
        perror("GetCurrentDir failed");
    }

    //printf("finalthing\n");
}
    

/*
float centerX, centerY;
float dx, dy;
float cosR, sinR;
float rotatedX, rotatedY;
float unscaledX, unscaledY;
float bitmapX, bitmapY;
int intbitmapX;
int intbitmapY;
*/

/*
angle in radians, in a 0-360 range
*/
void setWD(Actor *target, SDL_Renderer* renderer)
{
    //using namespace std;
    target->rendertextture = IMG_LoadTexture(renderer, target->path);
    if (target->imgsize == NULL)
    {
        printf("oopsie, spelt wrong or forgot .png");
        for (int counter = 0; counter < 50; counter++)
        {
            printf("\nyou forgot.png or you spelt image name wrong");
        }
        //just to hammer in the message
        return;
    }
    target->width = (float)target->imgsize->w;
    target->height = (float)target->imgsize->h;
    target->setsize = true;
}
//int img_w = width;
//int img_h = height;
//int totalpixels;//total number of pixels in the image

/*
initlizes a 1d bitmap that is the width and height of the image, all values start at 0
*/
void makebitmask(Actor *target,bool mask[], int masksize)
{
    
    //using namespace std;
    //this creates the bit mask that
    //(if good collision is initilized)
    //will be populated with bit data
    //vector<bool> returnmask;
    if (target->imgsize != NULL)
    {
        //totalpixels = imgsize->w * imgsize->h;
        //vector<bool> mademask(static_cast<size_t>(target->imgsize->w) * static_cast<size_t>(target->imgsize->h), false);
        //returnmask = mademask;
    }
    for (int init = masksize-1; init >= 0; init--)
    {
        mask[init] = false;
    }
}
//I AM WRITING THIS IN VIM!

/*
sees if a single triangle collides with another triangle and returen the first collision point as a pointer.
*/
int point_in_triangle(Actor *target, float px, float py, const SDL_Vertex* t) {
    target->x1 = t[0].position.x, target->y1 = t[0].position.y;
    target->x2 = t[1].position.x, target->y2 = t[1].position.y;
    target->x3 = t[2].position.x, target->y3 = t[2].position.y;
    target->denom = (target->y2 - target->y3) * (target->x1 - target->x3) + (target->x3 - target->x2) * (target->y1 - target->y3);
    if (target->denom == 0.0f) return false;
    target->a = ((target->y2 - target->y3) * (px - target->x3) + (target->x3 - target->x2) * (py - target->y3)) / target->denom;
    target->b = ((target->y3 - target->y1) * (px - target->x3) + (target->x1 - target->x3) * (py - target->y3)) / target->denom;
    target->c = 1.0f - target->a - target->b;
    return (target->a >= 0 && target->b >= 0 && target->c >= 0);
};
bool singletricollide(Actor *target,const SDL_Vertex* tri1, const SDL_Vertex* tri2, SDL_FPoint* intersectionPoint) {
    // Inline point-in-triangle using barycentric coordinates
    // Check if any point of one triangle is inside the other
    for (int i = 0; i < 3; ++i)
        if (point_in_triangle(target, tri1[i].position.x, tri1[i].position.y, tri2) ||
            point_in_triangle(target, tri2[i].position.x, tri2[i].position.y, tri1))
            return true;
    // Inline segment-segment intersection
    for (int i = 0; i < 3; ++i) {
        target->a1 = tri1[i].position, target->a2 = tri1[(i + 1) % 3].position;
        for (int j = 0; j < 3; ++j) {
            target->b1 = tri2[j].position, target->b2 = tri2[(j + 1) % 3].position;
            target->s1_x = target->a2.x - target->a1.x, target->s1_y = target->a2.y - target->a1.y;
            target->s2_x = target->b2.x - target->b1.x, target->s2_y = target->b2.y - target->b1.y;
            target->denom = (-target->s2_x * target->s1_y + target->s1_x * target->s2_y);
            if (target->denom == 0.0f) continue;
            target->s = (-target->s1_y * (target->a1.x - target->b1.x) + target->s1_x * (target->a1.y - target->b1.y)) / target->denom;
            target->t = (target->s2_x * (target->a1.y - target->b1.y) - target->s2_y * (target->a1.x - target->b1.x)) / target->denom;
            if (target->s >= 0 && target->s <= 1 && target->t >= 0 && target->t <= 1) {
                // Calculate the intersection point
                target->intersection_x = target->a1.x + (target->s * target->s1_x);
                target->intersection_y = target->a1.y + (target->s * target->s1_y);

                if (intersectionPoint) {
                    intersectionPoint->x = target->intersection_x;
                    intersectionPoint->y = target->intersection_y;
                }
                return true;
            }
        }
    }
    return false;
}
/*
This function is built to place corners and front center points so that
other calculations can creat triangles and other various uses. This
function does have some expensive math, so it is left out for simple
actors, and only works for normal and complex actors
*/
void rotatecollide(Actor *trgt)
{
    //std::cout << this->imgname<<"  x,y: " << x << "," << y << " imgcenter: " << imgcenter.x << "," << imgcenter.y << " this_x,this_y: " << this_x << "," << this_y << "\n";

    if (trgt->actortype == 0)
    {
        return;
    }
    //float L = sqrt(pow(width*scale/2, 2)+pow(height*scale/2, 2));
    //float O = pow(tan((height*scale)/(width * scale)), -1);
    //float D=180-2*O;
    trgt->thisangle = (float)trgt->angle * (float)(M_PI / 180);
    //float thisangle = angle * 360 / 2 * pi;
    trgt->this_x = trgt->x + trgt->imgcenter.x;
    trgt->this_y = trgt->y + trgt->imgcenter.y;
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderPoint(renderer, this_x-1, this_y-1);
    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //SDL_RenderPoint(renderer, this->x - 1, this->y - 1);
    //dodrawcollide = true;
    trgt->hw = (trgt->scale * trgt->width) / 2.0;
    trgt->hh = (trgt->scale * trgt->height) / 2.0;

    trgt->corn1[0] = (float)(-trgt->hw * cos(trgt->thisangle) - (-trgt->hh) * sin(trgt->thisangle)) + trgt->this_x;
    trgt->corn1[1] = (float)(-trgt->hw * sin(trgt->thisangle) + (-trgt->hh) * cos(trgt->thisangle)) + trgt->this_y;

    trgt->corn2[0] = (float)(trgt->hw * cos(trgt->thisangle) - (-trgt->hh) * sin(trgt->thisangle)) + trgt->this_x;
    trgt->corn2[1] = (float)(trgt->hw * sin(trgt->thisangle) + (-trgt->hh) * cos(trgt->thisangle)) + trgt->this_y;

    trgt->corn3[0] = (float)(trgt->hw * cos(trgt->thisangle) - trgt->hh * sin(trgt->thisangle)) + trgt->this_x;
    trgt->corn3[1] = (float)(trgt->hw * sin(trgt->thisangle) + trgt->hh * cos(trgt->thisangle)) + trgt->this_y;

    trgt->corn4[0] = (float)(-trgt->hw * cos(trgt->thisangle) - trgt->hh * sin(trgt->thisangle)) + trgt->this_x;
    trgt->corn4[1] = (float)(-trgt->hw * sin(trgt->thisangle) + trgt->hh * cos(trgt->thisangle)) + trgt->this_y;
    trgt->corner1 = (SDL_FRect){ trgt->corn1[0] - 1, trgt->corn1[1] - 1, 2, 2 };
    trgt->corner2 = (SDL_FRect){ trgt->corn2[0] - 1, trgt->corn2[1] - 1, 2, 2 };
    trgt->corner3 = (SDL_FRect){ trgt->corn3[0] - 1, trgt->corn3[1] - 1, 2, 2 };
    trgt->corner4 = (SDL_FRect){ trgt->corn4[0] - 1, trgt->corn4[1] - 1, 2, 2 };
    trgt->frontcent = (SDL_FRect){ (trgt->corn1[0] + trgt->corn2[0]) / 2 - 1, (trgt->corn1[1] + trgt->corn2[1]) / 2 - 1, 2, 2 };
    /*
    SDL_Vertex rect1[3] = { {{corn4[0],corn4[1]}, {255,255,255,0}, {0,0}},
                            {{corn2[0],corn2[1]}, {255,255,255,0}, {0,0}},
                            {{corn1[0],corn1[1]}, {255,255,255,0}, {0,0}}, };
    */

    trgt->rect1[0] = (SDL_Vertex){ {trgt->corn4[0],trgt->corn4[1]}, {255,255,255,0}, {0,0} };
    trgt->rect1[1] = (SDL_Vertex){ {trgt->corn2[0],trgt->corn2[1]}, {255,255,255,0}, {0,0} };
    trgt->rect1[2] = (SDL_Vertex){ {trgt->corn1[0],trgt->corn1[1]}, {255,255,255,0}, {0,0} };

    /*
    SDL_Vertex rect2[3] = { {{corn2[0],corn2[1]}, {255,255,255,255}, {0,0}},
                            {{corn3[0],corn3[1]}, {255,255,255,255}, {0,0}},
                            {{corn4[0],corn4[1]}, {255,255,255,255}, {0,0}}, };
    */

    trgt->rect2[0] = (SDL_Vertex){ {trgt->corn2[0],trgt->corn2[1]}, {255,255,255,255}, {0,0} };
    trgt->rect2[1] = (SDL_Vertex){ {trgt->corn3[0],trgt->corn3[1]}, {255,255,255,255}, {0,0} };
    trgt->rect2[2] = (SDL_Vertex){ {trgt->corn4[0],trgt->corn4[1]}, {255,255,255,255}, {0,0} };

    trgt->collide1[0] = trgt->rect1[0];
    trgt->collide1[1] = trgt->rect1[1];
    trgt->collide1[2] = trgt->rect1[2];

    trgt->collide2[0] = trgt->rect2[0];
    trgt->collide2[1] = trgt->rect2[1];
    trgt->collide2[2] = trgt->rect2[2];
    trgt->notcalculated = true;
}
/*
this function goes thorugh all pixels in a image
and if theyhave an alpha channel greater than one
it adds that value as 1 to a bitmap, else it will
make it 0. One of the more expensize math functions,
and cannot be used on simple actors, because the
function uses some expensize math. Takes about 14
milliseconds to work, so preformance could be better
*/
void init_good_collide(Actor *target)
{
    //using namespace std;
    if (target->actortype == 0)
    {
        return;
    }
    //auto start = chrono::high_resolution_clock::now();
    /*
    //Uint8 alpha=0;
    //scans horizonally
    vector<vector<bool>> isclear(img_h, vector<bool>(img_w, false));
    ////to appease the c++ gods we are using vector
    for (int row = img_h - 1; row >= 0; row--)
    {
        for (int col = img_w - 1; col >= 0; col--)
        {
            int index = row * img_w + col;
            SDL_ReadSurfacePixel(pixeldata, row, col, NULL, NULL, NULL, &alpha);
            if (alpha > 0)
            {
                isclear[row][col] = true;
            }
            else
            {
                isclear[row][col] = false;
            }
            ///cout << "  ";
        }
        //cout << endl;
    }
    */
    // Uint8 to store the alpha value of each pixel
    target->img_w = target->imgsize->w;
    target->img_h = target->imgsize->h;
    Uint8 alpha = 0;
    // Scans horizontally
    // Using a 1D vector for faster access to avoid overhead of 2D vector (faster cache access)
    //vector<bool> bitmask(static_cast<size_t>(img_h) * static_cast<size_t>(img_w), false);
    // Total number of pixels in the image
    //int total_pixels = img_h * img_w;
    // Assuming SDL_Surface uses 64-bit pixels (RGBA64, ARGB64, etc.)
    // Access the raw pixel data as Uint64*
    //Uint32* pixels = (Uint32*)pixeldata->pixels;
    Uint32 formatEnum = target->pixeldata->format;  // Directly access the format (32-bit identifier)
    //print out the pixel format
    const char* pixelformat = SDL_GetPixelFormatName((SDL_PixelFormat)formatEnum);
    //cout << filepath<<" Pixel format: " << SDL_GetPixelFormatName((SDL_PixelFormat)formatEnum) << std::endl;
    SDL_Surface* formatted = SDL_ConvertSurface(target->pixeldata, SDL_PIXELFORMAT_ABGR8888);
    Uint32* pixels = (Uint32*)formatted->pixels;
    Uint32 pixel;
    int index;
    int pitch = formatted->pitch / 4; // 32-bit pixels
    // Iterate through the image row by row
    for (int row = target->img_h - 1; row >= 0; row--)
    {
        for (int col = target->img_w - 1; col >= 0; col--)
        {
            // Map 2D (row, col) to 1D index in the vector
            index = row * pitch + col;
            // Access the pixel data directly from the 64-bit pixel array
            pixel = pixels[index];
            // Extract alpha assuming a 64-bit pixel format where alpha is in the most significant 16 bits
            // This can be modified depending on how the pixel format stores the alpha value
            //printf("Pixel[%d,%d] = 0x%08X, alpha = %d\n", row, col, pixel, alpha
            alpha = (pixel >> 24) & 0xFF;  // Shifting to extract alpha value (adjust if necessary)

            // Set the "clear" status: true if alpha > 0 (i.e., not transparent), false if fully transparent
            //bitmask[index]= (alpha > 0);
            target->bitmask[index] = (alpha > 0);
            /*
            case(SDL_PIXELFORMAT_INDEX8):
                SDL_Palette* palette = SDL_GetSurfacePalette(imgsize);
                if (palette == nullptr)
                {
                    //idk why but shit gets
                    //weird and nullptr
                    cout <<filepath<<"\n";
                    break;
                }
                int index2D = row * pitch + col;
                Uint8 palette_index = pixels[index2D];

                SDL_Color color = palette->colors[palette_index];
                Uint8 alpha = color.a;

                //int flat_index = row * img_w + col; // For bitmask indexing
                bitmask[index] = (alpha < 0);
                //put code here
                break;
            }
            */
        }
    }
    //convert to 2d vector
    //vector<vector<bool>> isclear2d(img_w, vector<bool>(img_h, false));
    /*
    if (this->filepath == "mushroom.png")
    {
        for (int counter = 0; counter <= img_w - 1; counter++)
        {
            for (int counter2 = 0; counter2 <= img_h - 1; counter2++)
            {
                //cout << bitmask.at(static_cast<size_t>(counter) * static_cast<size_t>(width) + counter2)<<" ";
            }
            //cout << "\n";
        }
    }
    */
    //vector<bool> isclear2(static_cast<size_t>(img_h) * static_cast<size_t>(img_w), false);
    //int heights = 0;
    //int nextrow = 0;
    //cout << "\n" << img_w;
    //cout << "\n" << img_h;
    /*
    for (int conv_rows = (img_w * img_h-1); conv_rows >= 0; conv_rows--)
    {
        if ((conv_rows % img_w)==0)
        {
            for (int counter = img_w - 1; counter >= 0; counter--)
            {
                isclear2d[counter][nextrow]=isclear[conv_rows];
            }
            nextrow+=1;
        }
        //cout << conv_rows << "\n\n";
    }
    //cout << heights << "\n";
    */
    /*
    for (int curntrow= 0; curntrow <= (totalpixels / img_w) - 1; curntrow++)
    {
        //for (int scanrow = img_w-1; scanrow >= 0; scanrow--)
        //{
        //    isclear2d[scanrow][curntrow];
        //}
    }
    */
    //cout << imgname << "  " << bitmask.size()<<"\n";
    target->has_bitmask = true;
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = duration_cast<chrono::milliseconds>(stop - start);
    //cout << duration.count() << endl;
}

/*
draws a persice line using SDL_RenderGeometry
Does have some displacement issues, but overall
better for lines than default function.
*/
void drawline(Actor *target, SDL_Renderer* renderer, SDL_FPoint p1, SDL_FPoint p2, float thickness, SDL_FColor color)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float length = hypot(dx, dy);
    if (length < 1e-6f) return;

    // Center pixel offset to align with pixel grid
    float align_x = 0.5f;
    float align_y = 0.5f;

    // Normalize perpendicular vector for thickness
    float nx = -dy / length;
    float ny = dx / length;

    float half_thickness = thickness * 0.5f;
    float ox = nx * half_thickness;
    float oy = ny * half_thickness;

    // Create a rectangle (quad) centered on the line
    SDL_FPoint v0 = { p1.x + ox + align_x, p1.y + oy + align_y };
    SDL_FPoint v1 = { p1.x - ox + align_x, p1.y - oy + align_y };
    SDL_FPoint v2 = { p2.x - ox + align_x, p2.y - oy + align_y };
    SDL_FPoint v3 = { p2.x + ox + align_x, p2.y + oy + align_y };

    SDL_Vertex verts[4];
    verts[0].position = v0;
    verts[1].position = v1;
    verts[2].position = v2;
    verts[3].position = v3;

    for (int i = 0; i < 4; ++i) {
        verts[i].color = color;
        //verts[i].tex_coord = (SDL_Vertex->tex_coord){ 0.0f, 0.0f };
    }

    int indices[6] = { 0, 1, 2, 0, 2, 3 };
    SDL_RenderGeometry(renderer, NULL, verts, 4, indices, 6);
}


void showcollide(Actor *target, SDL_Renderer* ren)
{
    target->showgoodcollide = 1;
    target->drawcolrenderer = ren;
}
/*
simple algebra function to see if
two lines intersect, used in goodcollide
to find the area of bitmap overlap checking
*/
bool lineintersect(Actor *target, const SDL_FPoint a1, const SDL_FPoint a2,
    const SDL_FPoint b1, const SDL_FPoint b2,
    SDL_FPoint intersectionPoint) {
    const float EPS = 1e-6f;
    float dx1 = a2.x - a1.x;
    float dy1 = a2.y - a1.y;
    float dx2 = b2.x - b1.x;
    float dy2 = b2.y - b1.y;

    float denominator = dx1 * dy2 - dy1 * dx2;
    if (fabs(denominator) < EPS) {
        return false; // Parallel or coincident
    }

    float dx3 = b1.x - a1.x;
    float dy3 = b1.y - a1.y;

    float t = (dx3 * dy2 - dy3 * dx2) / denominator;
    float u = (dx3 * dy1 - dy3 * dx1) / denominator;

    if (t < -EPS || t > 1.0f + EPS || u < -EPS || u > 1.0f + EPS) {
        return false;
    }

    intersectionPoint.x = a1.x + t * dx1;
    intersectionPoint.y = a1.y + t * dy1;
    return true;
}
float cross(const SDL_FPoint a, const SDL_FPoint b, const SDL_FPoint c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
};
/*
gives if a point intersect with a triangle using simple math
*/
bool tricollpnt(Actor *target, const SDL_FPoint p, const SDL_FPoint a, const SDL_FPoint b, const SDL_FPoint c) {

    bool b1 = cross(p, a, b) < 0.0f;
    bool b2 = cross(p, b, c) < 0.0f;
    bool b3 = cross(p, c, a) < 0.0f;

    return (b1 == b2) && (b2 == b3);
}
/*
converts from a screen pixel to the bitmap coordinate that corrosponds to it to it
*/
//bool tested = false;
void scrn_to_bmp_px(Actor *target, int p_x, int p_y, int* exit_x, int* exit_y)
{
    //if (this->scale == 0.0) {
    //    exit_x = exit_y = 0;
    //    return;
    //}

    double rads = fmod(target->angle, 360.0f) * (M_PI / 180.0f);

    int64_t centerX = (int64_t)target->x + (int64_t)target->imgcenter.x;
    int64_t centerY = (int64_t)target->y + (int64_t)target->imgcenter.y;




    double dx = (double)(p_x) - centerX;
    double dy = (double)(p_y) - centerY;

    double cosR = cos(-rads);
    double sinR = sin(-rads);
    double rotatedX = dx * cosR - dy * sinR;
    double rotatedY = dx * sinR + dy * cosR;

    double unscaledX = rotatedX / target->scale;
    double unscaledY = rotatedY / target->scale;

    double bitmapX = unscaledX + target->width * 0.5;
    double bitmapY = unscaledY + target->height * 0.5;
    //std::cout << "corner1: (" << corner1.x << ", " << corner1.y << ")\n";
    //std::cout << "screen input: (" << p_x << ", " << p_y << ")\n";
    //std::cout << "bitmap center: (" << centerX << ", " << centerY << ")\n";

    // Optional debug logging...
    //if (!tested && SDL_GetTicks() > 1000) {
    //    std::cout << "Bitmap Coords: (" << bitmapX << ", " << bitmapY << ")\n";
    //    tested = true;
    //}

    exit_x = (int)(floor(bitmapX));
    exit_y = (int)(floor(bitmapY));

    // Clamp to valid bitmap coordinates
    //exit_x = std::max(0, std::min(static_cast<int>(this->width) - 1, exit_x));
    //exit_y = std::max(0, std::min(static_cast<int>(this->height) - 1, exit_y));
}

bool pointinpolygon(Actor *target, const SDL_FPoint polygon[], const SDL_FPoint p, int64_t polygonsize) {
    bool inside = false;
    int64_t n = polygonsize;

    for (int64_t i = 0, j = n - 1; i < n; j = i++) {
        float xi = polygon[i].x, yi = polygon[i].y;
        float xj = polygon[j].x, yj = polygon[j].y;

        if ((yi > p.y) != (yj > p.y)) {
            float intersectX = (xj - xi) * (p.y - yi) / (yj - yi) + xi;
            if (p.x < intersectX)
                inside = !inside;
        }
    }

    return inside;
}

void getallpolypoints(Actor* target, const SDL_FPoint polygon[], float step, 
    SDL_FPoint fillarray[], int polysize, int fillarraysize, int* totalamountofpoints) {
    //std::vector<SDL_FPoint> result;
    SDL_FPoint* result = (SDL_FPoint*){0};
    if (polysize < 3)
        return result;
    int resultspot = 0;
    float minX = polygon[0].x, maxX = polygon[0].x;
    float minY = polygon[0].y, maxY = polygon[0].y;
    totalamountofpoints = 0;
    for (int looppoly = polysize - 1; polysize >= 0;polysize--) {
        SDL_FPoint pt = polygon[looppoly];
        minX = min(minX, pt.x);
        maxX = max(maxX, pt.x);
        minY = min(minY, pt.y);
        maxY = max(maxY, pt.y);
    }
    for (int yi = (int)(floor(minY / step)); yi <= ceil(maxY / step); ++yi) {
        for (int xi = (int)(floor(minX / step)); xi <= ceil(maxX / step); ++xi) {
            SDL_FPoint p = { xi * step + step / 2.0f, yi * step + step / 2.0f };
            if (pointinpolygon(target, polygon, p, polysize)) {
                //result.push_back(p);
                totalamountofpoints++;
                result[resultspot] = p;
                resultspot++;
            }
        }
    }
    return result;
}


static int cmp_points(const void* a, const void* b) {
    SDL_FPoint* p = (SDL_FPoint*)a, * q = (SDL_FPoint*)b;
    return (p->x != q->x) ? (p->x > q->x) - (p->x < q->x)
        : (p->y > q->y) - (p->y < q->y);
}

//static float cross(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c) {
//    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
//}

int convex_hull(SDL_FPoint* pts, int n, SDL_FPoint** hull_out) {
    if (n < 3) { *hull_out = NULL; return 0; }

    qsort(pts, n, sizeof(SDL_FPoint), cmp_points);
    SDL_FPoint* hull = malloc(sizeof(SDL_FPoint) * ((int64_t)2 * n));
    int k = 0;

    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }

    *hull_out = hull;
    return k - 1;  // last point is duplicate of first
};


bool testbasiccollide(Actor* target, Actor* collider)
{
    SDL_FPoint* dummypointer = NULL;
    if (singletricollide(target, target->rect1, collider->rect2, dummypointer) ||
        singletricollide(target, target->rect2, collider->rect2, dummypointer) ||
        singletricollide(target, target->rect1, collider->rect1, dummypointer))
    {
        return true;
    }
    return false;
}
int count_points_in_bounding_rect(SDL_FPoint corners[4], float step) {
    float min_x = FLT_MAX, max_x = -FLT_MAX;
    float min_y = FLT_MAX, max_y = -FLT_MAX;

    // Compute axis-aligned bounding box of the 4 points
    for (int i = 0; i < 4; i++) {
        if (corners[i].x < min_x) min_x = corners[i].x;
        if (corners[i].x > max_x) max_x = corners[i].x;
        if (corners[i].y < min_y) min_y = corners[i].y;
        if (corners[i].y > max_y) max_y = corners[i].y;
    }

    int count = 0;
    for (float x = min_x; x <= max_x + 1e-6; x += step) {
        for (float y = min_y; y <= max_y + 1e-6; y += step) {
            count++;
        }
    }

    return count;
}
/*
finds all points where two actors intersect with a rectangle on
the outside of the image then loops through all collision points
and compares them to the bitmap of the actor to see if the
bitmaps collide, which means it ignores clear pixels. This function
is very very slow currently, cannot be used with simple actors, will
throw logic error if you try to use it with one.
TL;DR: false of they dont collide, true if the opaque parts of the sprites do collide
*/
bool goodcollide(Actor *target, Actor* collider)
{
    //using namespace std;
    //auto start2 = chrono::high_resolution_clock::now();
    //rads = abs(fmod(angle, 360.0f) * (M_PI / 180.0f));
    float normalizedAngle = fmod(target->angle, 360.0f);
    if (normalizedAngle < 0) normalizedAngle += 360.0f;
    target->rads = normalizedAngle * (M_PI / 180.0f);
    if (collider->actortype == 0 || target->actortype == 0)
    {
        //throw logic_error("Cannot use goodcollide on simple actor, try simplecollide");
        return false;
    }
    if (!testbasiccollide(target, collider))
    {
        return false;
    }
    //auto start = high_resolution_clock::now();
    bool collides = false;
    rotatecollide(&collider);
    rotatecollide(target);
    SDL_FPoint l1p1;
    SDL_FPoint l1p2;
    SDL_FPoint l2p1;
    SDL_FPoint l2p2;
    /*
            front

    corner1###line1##corner2#
    l                       l
    i                       i
    n                       n
    e                       e
    4                       2
    corner4###line3##corner3#
    */
    //vector<SDL_FPoint> polygon = {};
    SDL_FPoint polygon[16];
    int polyspot = 0;
    SDL_FRect corners[4] = { target->corner1, target->corner2, target->corner3, target->corner4 };
    SDL_FRect othercorners[4] = { collider->corner1, collider->corner2, collider->corner3, collider->corner4 };
    SDL_FPoint collidepoint =  (SDL_FPoint){0,0};
    SDL_FPoint collidepoints[16];
    int colamount = 0;
    for (int collsides = 0; collsides < 4; collsides++)
    {
        l1p1 = (SDL_FPoint){ corners[collsides].x, corners[collsides].y };
        l1p2 = (SDL_FPoint){ corners[(collsides + 1) % 4].x, corners[(collsides + 1) % 4].y };
        for (int alllines = 0; alllines < 4; alllines++)
        {
            l2p1 =(SDL_FPoint){ othercorners[alllines].x, othercorners[alllines].y };
            l2p2 = (SDL_FPoint){ othercorners[(alllines + 1) % 4].x, othercorners[(alllines + 1) % 4].y };
            if (target->showgoodcollide)
            {
                SDL_SetRenderDrawColor(target->drawcolrenderer, 255, 255, 255, 255);
                SDL_RenderLine(target->drawcolrenderer, l2p1.x, l2p1.y, l2p2.x, l2p2.y);
                drawline(target, target->drawcolrenderer, l2p1, l2p2, 1, (SDL_FColor){ 255,255,255 });
                SDL_SetRenderDrawColor(target->drawcolrenderer, 128, 128, 255, 255);
                SDL_SetRenderDrawColor(target->drawcolrenderer, collsides * 55, collsides * 50, collsides * 10, 255);
                SDL_RenderLine(target->drawcolrenderer, l1p1.x, l1p1.y, l1p2.x, l1p2.y);
                drawline(target, target->drawcolrenderer, l1p1, l1p2, 1, (SDL_FColor){ 255,255,255 });
            }
            if (lineintersect(target, l1p1, l1p2, l2p1, l2p2, collidepoint))
            {
                collidepoints[colamount] = collidepoint;
                SDL_FPoint point1 = collidepoint;
                polygon[polyspot] = point1;
                polyspot++;
                //polygon.push_back(point1);
                //std::cout << collidepoint.x << "  " << collidepoint.y<<"\n";
                colamount += 1;
                //cout << collidepoint.x << "  " << collidepoint.y<<"\n";
            }
            //cout << collidepoint.x << "  " << collidepoint.y << "\n";
            collidepoint = (SDL_FPoint){ 0,0 };
        }
    }

    //vector<SDL_FRect> collidedcorners = {};
    SDL_FRect collidedcorners[16];
    int collcornersspot = 0;
    //vector<SDL_FRect> collidercollidedcorners = {};
    SDL_FRect collidercollidedcorners[16];
    int o_collcornersspot = 0;
    for (int counter = 3; counter >= 0; counter--)
    {
        SDL_FPoint currentpoint = { corners[counter].x, corners[counter].y };
        SDL_FPoint Ocurrentpoint = { othercorners[counter].x, othercorners[counter].y };
        //4 2 1
        //2 3 4
        if (tricollpnt(target, currentpoint, (SDL_FPoint){ collider->corner4.x, collider->corner4.y },
            (SDL_FPoint){ collider->corner2.x, collider->corner2.y }, (SDL_FPoint){ collider->corner1.x, collider->corner1.y })
            || (tricollpnt(target, currentpoint, (SDL_FPoint){ collider->corner2.x, collider->corner2.y },
                (SDL_FPoint){ collider->corner3.x, collider->corner3.y }, (SDL_FPoint){ collider->corner4.x, collider->corner4.y })))
        {
            collidedcorners[collcornersspot] = corners[counter];
            collcornersspot++;
            //collidedcorners.push_back(corners[counter]);
            SDL_FPoint point2 = { corners[counter].x, corners[counter].y };
            //polygon.push_back(point2);
            polygon[polyspot] = point2;
        }

        if (tricollpnt(target, Ocurrentpoint, (SDL_FPoint){ target->corner4.x, target->corner4.y },
            (SDL_FPoint){ target->corner2.x, target->corner2.y }, (SDL_FPoint){ target->corner1.x, target->corner1.y })
            || (tricollpnt(target, Ocurrentpoint, (SDL_FPoint){ target->corner2.x, target->corner2.y },
                (SDL_FPoint){ target->corner3.x, target->corner3.y }, (SDL_FPoint){ target->corner4.x, target->corner4.y })))
        {
            SDL_FPoint point = { othercorners[counter].x, othercorners[counter].y };
            //polygon.push_back(point);
            polygon[polyspot] = point;
            //collidercollidedcorners.push_back(othercorners[counter]);
            collidercollidedcorners[o_collcornersspot] = othercorners[counter];
        }
        else {
        }
    }
    //vector<SDL_FPoint> allinsidepoints;
    //const int maxtotalpoints = count_points_in_bounding_rect(polygon, 0.5f);
    //SDL_FPoint allinsidepoints[maxtotalpoints];
    int maxtotalpoints = count_points_in_bounding_rect(polygon, 0.5f);
    SDL_FPoint* allinsidepoints = malloc(maxtotalpoints * sizeof(SDL_FPoint));
    if (!allinsidepoints) {
        SDL_Log("somthing messed up");
    }
    //vector<SDL_FPoint> allinsidepoints1;
    SDL_FPoint finalpoly[16];
    int bmp_x=0;
    int bmp_y=0;
    int o_bmp_x=0;
    int o_bmp_y=0;
    float bmp_width = target->width;
    float bmp_height = target->height;
    float o_bmp_width = collider->width;
    float o_bmp_height = collider->width;
    //vector<SDL_FPoint> finalpoly;
    //if (polygon.size() > 0) {
    //    finalpoly = convex_hull(target, polygon);
    //}
    //else {
    //    finalpoly = polygon;
    //}
    convex_hull(target, polygon, finalpoly);
    /*
    for (SDL_FPoint drawpoly : polygon)
    {SDL_SetRenderDrawColor(renderer, 125, 125, 0, 255);
        SDL_RenderPoint(renderer, drawpoly.x, drawpoly.y);}
    for (SDL_FPoint drawpoly2 : finalpoly)
    {SDL_SetRenderDrawColor(renderer, 125, 0, 255, 255);
        SDL_RenderPoint(renderer, drawpoly2.x, drawpoly2.y);}
    */
    int* amountofpoints=0;
    getallpolypoints(target, finalpoly, 0.5f, allinsidepoints, 16, maxtotalpoints, amountofpoints);
    for (int drawinside = (int)(allinsidepoints = amountofpoints-1); drawinside >= 0; drawinside--)
    {
        scrn_to_bmp_px(target, (int)allinsidepoints[drawinside].x, (int)allinsidepoints[drawinside].y, o_bmp_x, o_bmp_y);
        scrn_to_bmp_px(target, (int)allinsidepoints[drawinside].x, (int)allinsidepoints[drawinside].y, bmp_x, bmp_y);
        SDL_SetRenderDrawColor(target->drawcolrenderer, 0, 255, 0, 255);
        SDL_RenderPoint(target->drawcolrenderer, allinsidepoints[drawinside].x, allinsidepoints[drawinside].y);
        if (bmp_x >= 0 && bmp_x < bmp_width &&
            bmp_y >= 0 && bmp_y < bmp_height &&
            o_bmp_x >= 0 && o_bmp_x < o_bmp_width &&
            o_bmp_y >= 0 && o_bmp_y < o_bmp_height)
        {
            int64_t index = (int64_t)bmp_y * (int64_t)target->width + (int64_t)bmp_x;
            int64_t o_index = (int64_t)o_bmp_y * (int64_t)collider->width + (int64_t)o_bmp_x;
            //std::cout << "index1: " << index << " bitmask size: " << this->bitmask.size()
            //    << "  index2: " << o_index << " bitmask size: " << collider.bitmask.size() << "\n";
            if (o_index < (signed)collider->width*(signed)collider->height && index < (signed)target->width*(signed)target->height)
            {
                //this caps the max values because my lazy ass doesnt want to
                //figure out what's wrong with this function
                if (collider->bitmask[o_index] && target->bitmask[index]) {
                    if (target->showgoodcollide)
                    {
                        SDL_SetRenderDrawColor(target->drawcolrenderer, 255, 0, 0, 255);
                        SDL_RenderPoint(target->drawcolrenderer, allinsidepoints[drawinside].x, allinsidepoints[drawinside].y);
                    }
                    collides = true;
                    if (!target->showgoodcollide) break;
                }
            }
        }
    }
    /*
    SDL_SetRenderDrawColor(renderer, 100, 0, 100, 255);SDL_FPoint p1 = { collider.corner1.x, collider.corner1.y };
    SDL_FPoint p2 = { collider.corner2.x, collider.corner2.y };SDL_FPoint p3 = { collider.corner3.x, collider.corner3.y };
    SDL_FPoint p4 = { collider.corner4.x, collider.corner4.y };SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderLine(renderer, p2.x, p2.y, p3.x, p3.y);SDL_RenderLine(renderer, p3.x, p3.y, p4.x, p4.y);
    SDL_RenderLine(renderer, p1.x, p1.y, p4.x, p4.y);SDL_SetRenderDrawColor(renderer,0, 255, 0, 255);
    SDL_FPoint p1t = { corner1.x, corner1.y };SDL_FPoint p2t = { corner2.x, corner2.y };
    SDL_FPoint p3t = { corner3.x, corner3.y SDL_FPoint p4t = { corner4.x, corner4.y };
    SDL_RenderLine(renderer, p1t.x, p1t.y, p2t.x, p2t.y);SDL_RenderLine(renderer, p2t.x, p2t.y, p3t.x, p3t.y);
    SDL_RenderLine(renderer, p3t.x, p3t.y, p4t.x, p4t.y);SDL_RenderLine(renderer, p1t.x, p1t.y, p4t.x, p4t.y);
    */
    /*
    for (int counter = colamount - 1; counter >= 0; counter--)
    {
    SDL_SetRenderDrawColor(renderer,255, 0, 0, 255);
    SDL_FRect pointrect = { collidepoints[counter].x - 0.5,collidepoints[counter].y - 0.5,1,1};
    SDL_RenderRect(renderer, &pointrect);SDL_RenderGeometry(renderer, NULL, vertex2, 0, NULL, 0);
    SDL_RenderPoint(renderer, x, y);if (counter < collidedcorners.size())
    {SDL_RenderPoint(renderer, collidedcorners[counter].x, collidedcorners[counter].y);}
    if (counter < collidercollidedcorners.size())
    {SDL_RenderPoint(renderer, collidercollidedcorners[counter].x, collidercollidedcorners[counter].y);}
    SDL_RenderPoint(renderer, collidepoints[counter].x-1, collidepoints[counter].y-1);
    SDL_FRect pointrect = { collidepoints[counter].x,collidepoints[counter].y,1,1 };
    cout << pointrect.x<<"   "<<pointrect.y<<"\n";
    SDL_RenderRect(renderer, &pointrect);
    }
    */
    free(allinsidepoints);
    //auto stop2 = chrono::high_resolution_clock::now();
    //auto duration2 = duration_cast<chrono::milliseconds>(stop2 - start2);
    //std::cout << duration2.count()<<"\n";
    return collides;
}
/*
update function for actors will not be drawn but still need math and updates
*/
void update(Actor* target, SDL_Renderer* renderer)
{
    //if (target->bitmask.empty())
    //{
    //    makebitmask(target, target->has_bitmask, target->width*target->height);
    //}
    if (target->old_x != target->x || target->old_y != target->y || target->old_angle != target->angle || !target->notcalculated)
    {
        rotatecollide(target);
    }
    if (target->setsize == false)
    {
        setWD(target, renderer);
        target->setsize = true;
    }
    target->imgcenter = (SDL_FPoint){ target->width / 2 * target->scale,target->height / 2 * target->scale };
    target->old_x = target->x;
    target->old_y = target->y;
    target->old_angle = target->angle;
}
void renderdebug(Actor* target, SDL_Renderer* renderer)
{
    float renderscale_x;
    float renderscale_y;
    SDL_GetRenderScale(renderer, &renderscale_x, &renderscale_y);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_FColor color = { 255, 0, 255, 255 };
    target->collide1[0].color = color;
    target->collide1[1].color = color;
    target->collide1[2].color = color;
    int textscale = 1;
    SDL_RenderGeometry(renderer, NULL, target->collide1, 3, NULL, 0);
    SDL_RenderGeometry(renderer, NULL, target->collide2, 3, NULL, 0);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderRect(renderer, &target->corner1);//red
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, target->corner1.x * renderscale_x - textscale, target->corner1.y * renderscale_y - textscale, "corner1");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderRect(renderer, &target->corner2);//blue
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, target->corner2.x * renderscale_x - textscale, target->corner2.y * renderscale_y - textscale, "corner2");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderRect(renderer, &target->corner3);//purple
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, target->corner3.x * renderscale_x - textscale, target->corner3.y * renderscale_y - textscale, "corner3");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderRect(renderer, &target->corner4);//yellow
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, target->corner4.x * renderscale_x - textscale, target->corner4.y * renderscale_y - textscale, "corner4");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 50, 255, 60, 255);
    SDL_RenderRect(renderer, &target->frontcent);//green
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, target->frontcent.x * renderscale_x - textscale, target->frontcent.y * renderscale_y - textscale, "front");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);
}
/*
creates the texture of the actor that will be drawn to the screen if the texture has changed
*/
void maketextture(Actor* target, SDL_Renderer* renderer)
{

    target->coords = (SDL_FRect){ target->x,target->y,target->width * target->scale,target->height * target->scale };
    if (!target->animated) return;
    if (target->imgname != target->oldimgname) dir(target, target->path, 1024); //SDL_Log(target->imgname); SDL_Log(target->path);
    target->rendertextture = IMG_LoadTexture(renderer, target->path);
    target->oldimgname = target->imgname;
    target->oldrendertext = target->rendertextture;
}
/*
quickly and easily draw actor to specified renderer
automatically creating texture, texture coordnates,
x and y calculations, and image data.
TL;DR: draws actor to renderer
*/
void quickdraw(Actor* target, SDL_Renderer* renderer)
{
    //using namespace std;
    if (target->imgsize == NULL)
    {
        SDL_Log("Check the image name. did you forget .png?");
        return;
    }
    if (target->bitmask == false && target->actortype >= 1)
    {
        /*this checks if the bitmap is
        unitilized and inits it if so*/
        init_good_collide(target);
    }
    if (target->old_x != target->x || target->old_y != target->y || target->old_angle != target->angle || target->notcalculated) rotatecollide(target);
    
    maketextture(target, renderer);
    
    target->imgcenter = (SDL_FPoint){ target->width / 2 * target->scale,target->height / 2 * target->scale };

    if (target->dodrawcollide) renderdebug(target, renderer);

    if (target->setsize == false) setWD(target, renderer);

    if (target->actortype != 0) {
        SDL_SetTextureScaleMode(target->rendertextture, target->scaletype);
        SDL_RenderTextureRotated(renderer, target->rendertextture, NULL, &target->coords, target->angle, &target->imgcenter, SDL_FLIP_NONE);
    }
    else {
        SDL_RenderTexture(renderer, target->rendertextture, NULL, &target->coords);
    }
    target->old_x = target->x;
    target->old_y = target->y;
    target->old_angle = target->angle;
}
/*
draws all points and simpe collision of the actor
*/
void drawcollide(Actor* target,SDL_Renderer* renderer, int R, int G, int B, int A)
{
    Uint8 orig_R, orig_G, orig_B, orig_A;
    float scale1, scale2;
    SDL_GetRenderDrawColor(renderer, &orig_R, &orig_G, &orig_B, &orig_A);
    SDL_GetRenderScale(renderer, &scale1, &scale2);
    SDL_SetRenderDrawColor(renderer, R, G, B, A);
    SDL_SetRenderScale(renderer, 2, 2);
    target->dodrawcollide = true;

    SDL_SetRenderScale(renderer, scale1, scale2);
    SDL_SetRenderDrawColor(renderer, orig_R, orig_G, orig_B, orig_A);
}
int step = 0;

/*
basic rectangle collision, ignores angle, bitmap, and direction.
*/
bool rectcollide(Actor* target, SDL_FRect colliderect)
{
    //dumbass basic collison
    int rect_x = (int)target->coords.x, rect_y = (int)target->coords.y, rect_h = (int)target->coords.h, rect_w = (int)target->coords.w;
    const SDL_Rect actorrect = (SDL_Rect){ rect_x, rect_y, rect_w, rect_h };
    int rect_x_o = (int)colliderect.x, rect_y_o = (int)colliderect.y, rect_h_o = (int)colliderect.h, rect_w_o = (int)colliderect.w;
    const SDL_Rect collisionrect = (SDL_Rect){ rect_x_o, rect_y_o, rect_w_o, rect_h_o };
    return SDL_HasRectIntersection(&actorrect, &collisionrect);
}
void create_actor(Actor* madeactor, char imgname[256], float x, float y, float angle, float scale, enum Atype actortype)
{
    //Actor madeactor = { imgname, x, y, angle, scale, actortype }; 
    //SDL_Log((char*)imgname);
    //madeactor->imgname=imgname;
    strcpy_s(madeactor->imgname, sizeof(madeactor->imgname), imgname);
    madeactor->imgname[sizeof(madeactor->imgname) - 1] = '\0'; 
    madeactor->x=x;
    madeactor->y=y;
    madeactor->angle=angle;
    madeactor->scale=scale;
    madeactor->actortype=actortype; 
    madeactor->oldimgname;
    dir(madeactor, madeactor->path, sizeof(madeactor->path));
    //SDL_Log(madeactor->path);
    madeactor->imgsize = IMG_Load(madeactor->path);
    madeactor->pixeldata = IMG_Load(madeactor->path);
    madeactor->rendertextture;
    madeactor->coords = (SDL_FRect){ x,y,100,100 };
    //string pathhalf = "";
    //int lastsec = 0;
    madeactor->animated = false;
    madeactor->width = 0; //width of the actor's image
    madeactor->height = 0; //height of the actor's image
    //int againtime = 0;
    madeactor->turnangle = angle;//a second angle varible for comparison
    madeactor->setsize = false;//bool for if width and height are set
    madeactor->oldangle = 0;// a third angle varible for angle comparison
    madeactor->imgcenter = (SDL_FPoint){ 0,0 };//sets the the images center for math with corners and other calculation
    //SDL_FRect testrect = {};
    //SDL_Vertex collision;
    madeactor->corner1 = (SDL_FRect){ 0, 0, 2, 2 };//first corner of actor at top left
    madeactor->corner2 = (SDL_FRect){ 0, 0, 2, 2 };//second corner of actor at top right
    madeactor->corner3 = (SDL_FRect){ 0, 0, 2, 2 };//third corner of actor at bottom left
    madeactor->corner4 = (SDL_FRect){ 0, 0, 2, 2 };//fourth corner of actor at bottom right
    madeactor->frontcent = (SDL_FRect){ 0, 0, 2, 2 };//a point in between top left and top right points
    //bool onetest = false;
    //madeactor.collide1[3];// first collide trangle, combined with rect 2 makes a real rectangle
    //madeactor.collide2[3];// second collide triangle, combined with rect 1 makes a real rectangle
    madeactor->dodrawcollide = false;// says whether of not to draw collision of the actor
    madeactor->old_x = 0;//an old x varible for x var storing
    madeactor->old_y = 0;//an old y varible for y var storing
    madeactor->old_angle = 0;//an old angle varible for angle storing
    madeactor->mktime = SDL_GetTicks();// is the make time of the actor, also can be used for time storing
    madeactor->oldrendertext;// varible for checking of the texture has changed 
    madeactor->maskmade = false;// bool for whether or not the bitmask has been initlized
    madeactor->has_bitmask = false;//another bool for checking if the bitmaks has 𝒓𝒆𝒂𝒍𝒍𝒚 been initilized
    madeactor->notcalculated = false;// bool for whether or not the corners have been calculated
    madeactor->this_x = x + madeactor->imgcenter.x;// this_x is the local x coord varible using the x of the surface itself added to normal x
    madeactor->this_y = y + madeactor->imgcenter.y;// this_y is the local y coord varible using the y of the surface itself added to normal y
    madeactor->hw = (scale * madeactor->width) / 2.0;//half the width of the surface, with added scale
    madeactor->hh = (scale * madeactor->height) / 2.0;//half the height of the surface, with added scale
    madeactor->totalpixels;//total number of pixels in the image
    madeactor->bitmask = malloc(madeactor->width*madeactor->height * sizeof(bool));//i hope allocating this does come back to bite me in the ass
    //float x1, x2, x3, y1, y2, y3;//this varible is used for math
    //float a, b, c, s, t, denom;//this varible is used for math
    //float s1_x, s1_y, s2_x, s2_y;//this varible is used for math
    //float intersection_x, intersection_y;//this varible is used for math
    //SDL_FPoint a1, a2, b1, b2;//this varible is used for math
    //madeactor.thisangle;
    //float corn1[2], corn2[2], corn3[2], corn4[2];
    //madeactor.rect1[3];
    //madeactor.rect2[3];
    //madeactor.pointrect;//used for showing collison points in goodcollide() function
    madeactor->scaletype = SDL_SCALEMODE_NEAREST;
    madeactor->rads = fmod(angle, 360.0f) * (3.1415926535897932384626433832795028841 / 180.0f);
    //madeactor.img_w;
    //madeactor.img_h;
    //float centerX, centerY;
    //float dx, dy;
    //float cosR, sinR;
    //float rotatedX, rotatedY;
    //float unscaledX, unscaledY;
    //float bitmapX, bitmapY;
    //madeactor.intbitmapX;
    //madeactor.intbitmapY;
    madeactor->tested = false;
    //madeactor.showgoodcollide;
    //madeactor.drawcolrenderer;
    //std::vector<bool>bitmask;
    madeactor->bitmask;
}