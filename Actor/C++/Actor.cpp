#define _USE_MATH_DEFINES // math shit
#include <SDL3/SDL.h>
#include <SDL_image.h>
#include "Actor.hpp"
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


std::string Actor::dir()//creates directory 
{
    using namespace std;
    namespace fs = std::filesystem;
    error_code err;
    //stringstream path;//varible for temporarily holding path
    //const size_t size = 1024;// path buffer size
    //char currdir[size];//buffer for path
    //bool dirworked= fs::current_path();// checks if the directory data can be taken from
    static fs::path curr_path = fs::current_path(err);
    //stringstream path(curr_path.string());
    stringstream path;
    path << curr_path.string() << "\\images\\" << imgname;
    if (err)
    {
        cout << "the images have not loaded. Error message: " << err.message();
    }
    string pathhalf = path.str();
    string endchar = pathhalf;
    return endchar;
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
void Actor::setWD(SDL_Renderer* renderer)
{
    using namespace std;
    rendertextture = IMG_LoadTexture(renderer, path.c_str());
    if (this->imgsize == nullptr)
    {
        cout << "\n" << "oopsie, spelt wrong or forgot .png";
        for (int counter = 0; counter < 50; counter++)
        {
            cout << "\n" << "you forgot .png or you spelt image name wrong";
        }
        //just to hammer in the message
        return;
    }
    width = (float)imgsize->w;
    height = (float)imgsize->h;
}
//int img_w = width;
//int img_h = height;
//int totalpixels;//total number of pixels in the image

/*
initlizes a 1d bitmap that is the width and height of the image, all values start at 0
*/
std::vector<bool> Actor::makebitmask()
{
    using namespace std;
    //this creates the bit mask that
    //(if good collision is initilized)
    //will be populated with bit data
    vector<bool> returnmask;
    if (imgsize != nullptr)
    {
        //totalpixels = imgsize->w * imgsize->h;
        vector<bool> mademask(static_cast<size_t>(imgsize->w) * static_cast<size_t>(imgsize->h), false);
        returnmask = mademask;
    }
    return returnmask;
}
//I AM WRITING THIS IN VIM!

/*
sees if a single triangle collides with another triangle and returen the first collision point as a pointer.
*/
bool Actor::singletricollide(const SDL_Vertex* tri1, const SDL_Vertex* tri2, SDL_FPoint* intersectionPoint) {
    // Inline point-in-triangle using barycentric coordinates
    auto point_in_triangle = [this](float px, float py, const SDL_Vertex* t) {
        x1 = t[0].position.x, y1 = t[0].position.y;
        x2 = t[1].position.x, y2 = t[1].position.y;
        x3 = t[2].position.x, y3 = t[2].position.y;
        denom = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
        if (denom == 0.0f) return false;
        a = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) / denom;
        b = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) / denom;
        c = 1.0f - a - b;
        return (a >= 0 && b >= 0 && c >= 0);
    };
    // Check if any point of one triangle is inside the other
    for (int i = 0; i < 3; ++i)
        if (point_in_triangle(tri1[i].position.x, tri1[i].position.y, tri2) ||
            point_in_triangle(tri2[i].position.x, tri2[i].position.y, tri1))
            return true;
    // Inline segment-segment intersection
    for (int i = 0; i < 3; ++i) {
        a1 = tri1[i].position, a2 = tri1[(i + 1) % 3].position;
        for (int j = 0; j < 3; ++j) {
            b1 = tri2[j].position, b2 = tri2[(j + 1) % 3].position;
            s1_x = a2.x - a1.x, s1_y = a2.y - a1.y;
            s2_x = b2.x - b1.x, s2_y = b2.y - b1.y;
            denom = (-s2_x * s1_y + s1_x * s2_y);
            if (denom == 0.0f) continue;
            s = (-s1_y * (a1.x - b1.x) + s1_x * (a1.y - b1.y)) / denom;
            t = (s2_x * (a1.y - b1.y) - s2_y * (a1.x - b1.x)) / denom;
            if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
                // Calculate the intersection point
                intersection_x = a1.x + (s * s1_x);
                intersection_y = a1.y + (s * s1_y);

                if (intersectionPoint) {
                    intersectionPoint->x = intersection_x;
                    intersectionPoint->y = intersection_y;
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
void Actor::rotatecollide()
{
    //std::cout << this->imgname<<"  x,y: " << x << "," << y << " imgcenter: " << imgcenter.x << "," << imgcenter.y << " this_x,this_y: " << this_x << "," << this_y << "\n";

    if (actortype == simple)
    {
        return;
    }
    //float L = sqrt(pow(width*scale/2, 2)+pow(height*scale/2, 2));
    //float O = pow(tan((height*scale)/(width * scale)), -1);
    //float D=180-2*O;
    thisangle = (float)angle * (float)(M_PI / 180);
    //float thisangle = angle * 360 / 2 * pi;
    this_x = x + imgcenter.x;
    this_y = y + imgcenter.y;
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderPoint(renderer, this_x-1, this_y-1);
    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //SDL_RenderPoint(renderer, this->x - 1, this->y - 1);
    //dodrawcollide = true;
    hw = (scale * width) / 2.0;
    hh = (scale * height) / 2.0;

    corn1[0] = (float)(-hw * cos(thisangle) - (-hh) * sin(thisangle)) + this_x;
    corn1[1] = (float)(-hw * sin(thisangle) + (-hh) * cos(thisangle)) + this_y;

    corn2[0] = (float)(hw * cos(thisangle) - (-hh) * sin(thisangle)) + this_x;
    corn2[1] = (float)(hw * sin(thisangle) + (-hh) * cos(thisangle)) + this_y;

    corn3[0] = (float)(hw * cos(thisangle) - hh * sin(thisangle)) + this_x;
    corn3[1] = (float)(hw * sin(thisangle) + hh * cos(thisangle)) + this_y;

    corn4[0] = (float)(-hw * cos(thisangle) - hh * sin(thisangle)) + this_x;
    corn4[1] = (float)(-hw * sin(thisangle) + hh * cos(thisangle)) + this_y;
    corner1 = { corn1[0] - 1, corn1[1] - 1, 2, 2 };
    corner2 = { corn2[0] - 1, corn2[1] - 1, 2, 2 };
    corner3 = { corn3[0] - 1, corn3[1] - 1, 2, 2 };
    corner4 = { corn4[0] - 1, corn4[1] - 1, 2, 2 };
    frontcent = { (corn1[0] + corn2[0]) / 2 - 1, (corn1[1] + corn2[1]) / 2 - 1, 2, 2 };
    /*
    SDL_Vertex rect1[3] = { {{corn4[0],corn4[1]}, {255,255,255,0}, {0,0}},
                            {{corn2[0],corn2[1]}, {255,255,255,0}, {0,0}},
                            {{corn1[0],corn1[1]}, {255,255,255,0}, {0,0}}, };
    */

    rect1[0] = { {corn4[0],corn4[1]}, {255,255,255,0}, {0,0} };
    rect1[1] = { {corn2[0],corn2[1]}, {255,255,255,0}, {0,0} };
    rect1[2] = { {corn1[0],corn1[1]}, {255,255,255,0}, {0,0} };

    /*
    SDL_Vertex rect2[3] = { {{corn2[0],corn2[1]}, {255,255,255,255}, {0,0}},
                            {{corn3[0],corn3[1]}, {255,255,255,255}, {0,0}},
                            {{corn4[0],corn4[1]}, {255,255,255,255}, {0,0}}, };
    */

    rect2[0] = { {corn2[0],corn2[1]}, {255,255,255,255}, {0,0} };
    rect2[1] = { {corn3[0],corn3[1]}, {255,255,255,255}, {0,0} };
    rect2[2] = { {corn4[0],corn4[1]}, {255,255,255,255}, {0,0} };

    collide1[0] = rect1[0];
    collide1[1] = rect1[1];
    collide1[2] = rect1[2];

    collide2[0] = rect2[0];
    collide2[1] = rect2[1];
    collide2[2] = rect2[2];
    this->notcalculated = true;
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
void Actor::init_good_collide()
{
    using namespace std;
    if (actortype == simple)
    {
        return;
    }
    auto start = chrono::high_resolution_clock::now();
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
    img_w = imgsize->w;
    img_h = imgsize->h;
    Uint8 alpha = 0;
    // Scans horizontally
    // Using a 1D vector for faster access to avoid overhead of 2D vector (faster cache access)
    //vector<bool> bitmask(static_cast<size_t>(img_h) * static_cast<size_t>(img_w), false);
    // Total number of pixels in the image
    //int total_pixels = img_h * img_w;
    // Assuming SDL_Surface uses 64-bit pixels (RGBA64, ARGB64, etc.)
    // Access the raw pixel data as Uint64*
    //Uint32* pixels = (Uint32*)pixeldata->pixels;
    Uint32 formatEnum = pixeldata->format;  // Directly access the format (32-bit identifier)
    //print out the pixel format
    const char* pixelformat = SDL_GetPixelFormatName((SDL_PixelFormat)formatEnum);
    //cout << filepath<<" Pixel format: " << SDL_GetPixelFormatName((SDL_PixelFormat)formatEnum) << std::endl;
    SDL_Surface* formatted = SDL_ConvertSurface(pixeldata, SDL_PIXELFORMAT_ABGR8888);
    Uint32* pixels = (Uint32*)formatted->pixels;
    Uint32 pixel;
    int index;
    int pitch = formatted->pitch / 4; // 32-bit pixels
    // Iterate through the image row by row
    for (int row = img_h - 1; row >= 0; row--)
    {
        for (int col = img_w - 1; col >= 0; col--)
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
            bitmask[index] = (alpha > 0);
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
    this->has_bitmask = true;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = duration_cast<chrono::milliseconds>(stop - start);
    //cout << duration.count() << endl;
}

/*
draws a persice line using SDL_RenderGeometry
Does have some displacement issues, but overall
better for lines than default function.
*/
void Actor::drawline(SDL_Renderer* renderer, SDL_FPoint p1, SDL_FPoint p2, float thickness, SDL_FColor color)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float length = std::hypot(dx, dy);
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
        verts[i].tex_coord = { 0.0f, 0.0f };
    }

    int indices[6] = { 0, 1, 2, 0, 2, 3 };
    SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
}

bool showgoodcollide;
SDL_Renderer* drawcolrenderer;
void Actor::showcollide(SDL_Renderer* ren)
{
    showgoodcollide = 1;
    drawcolrenderer = ren;
}
/*
simple algebra function to see if
two lines intersect, used in goodcollide
to find the area of bitmap overlap checking
*/
bool Actor::lineintersect(const SDL_FPoint& a1, const SDL_FPoint& a2,
    const SDL_FPoint& b1, const SDL_FPoint& b2,
    SDL_FPoint& intersectionPoint) {
    const float EPS = 1e-6f;
    float dx1 = a2.x - a1.x;
    float dy1 = a2.y - a1.y;
    float dx2 = b2.x - b1.x;
    float dy2 = b2.y - b1.y;

    float denominator = dx1 * dy2 - dy1 * dx2;
    if (std::fabs(denominator) < EPS) {
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
/*
gives if a point intersect with a triangle using simple math
*/
bool Actor::tricollpnt(const SDL_FPoint& p, const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c) {
    auto cross = [](const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c) -> float {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    };

    bool b1 = cross(p, a, b) < 0.0f;
    bool b2 = cross(p, b, c) < 0.0f;
    bool b3 = cross(p, c, a) < 0.0f;

    return (b1 == b2) && (b2 == b3);
}
/*
converts from a screen pixel to the bitmap coordinate that corrosponds to it to it
*/
bool tested = false;
void Actor::scrn_to_bmp_px(int p_x, int p_y, int& exit_x, int& exit_y)
{
    //if (this->scale == 0.0) {
    //    exit_x = exit_y = 0;
    //    return;
    //}

    double rads = fmod(angle, 360.0f) * (M_PI / 180.0f);

    int64_t centerX = (int64_t)this->x + (int64_t)imgcenter.x;
    int64_t centerY = (int64_t)this->y + (int64_t)imgcenter.y;




    double dx = static_cast<double>(p_x) - centerX;
    double dy = static_cast<double>(p_y) - centerY;

    double cosR = cos(-rads);
    double sinR = sin(-rads);
    double rotatedX = dx * cosR - dy * sinR;
    double rotatedY = dx * sinR + dy * cosR;

    double unscaledX = rotatedX / this->scale;
    double unscaledY = rotatedY / this->scale;

    double bitmapX = unscaledX + this->width * 0.5;
    double bitmapY = unscaledY + this->height * 0.5;
    //std::cout << "corner1: (" << corner1.x << ", " << corner1.y << ")\n";
    //std::cout << "screen input: (" << p_x << ", " << p_y << ")\n";
    //std::cout << "bitmap center: (" << centerX << ", " << centerY << ")\n";

    // Optional debug logging...
    //if (!tested && SDL_GetTicks() > 1000) {
    //    std::cout << "Bitmap Coords: (" << bitmapX << ", " << bitmapY << ")\n";
    //    tested = true;
    //}

    exit_x = static_cast<int>(std::floor(bitmapX));
    exit_y = static_cast<int>(std::floor(bitmapY));

    // Clamp to valid bitmap coordinates
    //exit_x = std::max(0, std::min(static_cast<int>(this->width) - 1, exit_x));
    //exit_y = std::max(0, std::min(static_cast<int>(this->height) - 1, exit_y));
}

bool Actor::pointinpolygon(const std::vector<SDL_FPoint>& polygon, const SDL_FPoint& p) {
    bool inside = false;
    size_t n = polygon.size();

    for (size_t i = 0, j = n - 1; i < n; j = i++) {
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

std::vector<SDL_FPoint> Actor::getallpolypoints(const std::vector<SDL_FPoint>& polygon, float step) {
    std::vector<SDL_FPoint> result;

    if (polygon.size() < 3)
        return result;

    float minX = polygon[0].x, maxX = polygon[0].x;
    float minY = polygon[0].y, maxY = polygon[0].y;

    for (const auto& pt : polygon) {
        minX = std::min(minX, pt.x);
        maxX = std::max(maxX, pt.x);
        minY = std::min(minY, pt.y);
        maxY = std::max(maxY, pt.y);
    }
    for (int yi = static_cast<int>(std::floor(minY / step)); yi <= std::ceil(maxY / step); ++yi) {
        for (int xi = static_cast<int>(std::floor(minX / step)); xi <= std::ceil(maxX / step); ++xi) {
            SDL_FPoint p = { xi * step + step / 2.0f, yi * step + step / 2.0f };
            if (pointinpolygon(polygon, p)) {
                result.push_back(p);
            }
        }
    }
    return result;
}
std::vector<SDL_FPoint> Actor::convex_hull(std::vector<SDL_FPoint> pts) {
    std::sort(pts.begin(), pts.end(), [](const SDL_FPoint& a, const SDL_FPoint& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
        });

    std::vector<SDL_FPoint> lower, upper;

    for (const auto& p : pts) {
        while (lower.size() >= 2) {
            auto& q = lower[lower.size() - 2];
            auto& r = lower[lower.size() - 1];
            if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0)
                lower.pop_back();
            else
                break;
        }
        lower.push_back(p);
    }

    for (int i = (int)pts.size() - 1; i >= 0; --i) {
        const auto& p = pts[i];
        while (upper.size() >= 2) {
            auto& q = upper[upper.size() - 2];
            auto& r = upper[upper.size() - 1];
            if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0)
                upper.pop_back();
            else
                break;
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());

    return lower;  // convex polygon in counter-clockwise order
}

bool Actor::testbasiccollide(Actor& collider)
{
    SDL_FPoint* dummypointer = NULL;
    if (singletricollide(rect1, collider.rect2, dummypointer) ||
        singletricollide(rect2, collider.rect2, dummypointer) ||
        singletricollide(rect1, collider.rect1, dummypointer))
    {
        return true;
    }
    return false;
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
bool Actor::goodcollide(Actor& collider)
{
    using namespace std;
    auto start2 = chrono::high_resolution_clock::now();
    //rads = abs(fmod(angle, 360.0f) * (M_PI / 180.0f));
    float normalizedAngle = fmod(angle, 360.0f);
    if (normalizedAngle < 0) normalizedAngle += 360.0f;
    rads = normalizedAngle * (M_PI / 180.0f);
    if (collider.actortype == simple || actortype == simple)
    {
        throw logic_error("Cannot use goodcollide on simple actor, try simplecollide");
        return false;
    }
    if (!testbasiccollide(collider))
    {
        return false;
    }
    //auto start = high_resolution_clock::now();
    bool collides = false;
    collider.rotatecollide();
    this->rotatecollide();
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
    vector<SDL_FPoint> polygon = {};
    SDL_FRect corners[4] = { corner1, corner2, corner3, corner4 };
    SDL_FRect othercorners[4] = { collider.corner1, collider.corner2, collider.corner3, collider.corner4 };
    SDL_FPoint collidepoint;
    SDL_FPoint collidepoints[16] = {};
    int colamount = 0;
    for (int collsides = 0; collsides < 4; collsides++)
    {
        l1p1 = { corners[collsides].x, corners[collsides].y };
        l1p2 = { corners[(collsides + 1) % 4].x, corners[(collsides + 1) % 4].y };
        for (int alllines = 0; alllines < 4; alllines++)
        {
            l2p1 = { othercorners[alllines].x, othercorners[alllines].y };
            l2p2 = { othercorners[(alllines + 1) % 4].x, othercorners[(alllines + 1) % 4].y };
            if (showgoodcollide)
            {
                SDL_SetRenderDrawColor(drawcolrenderer, 255, 255, 255, 255);
                SDL_RenderLine(drawcolrenderer, l2p1.x, l2p1.y, l2p2.x, l2p2.y);
                drawline(drawcolrenderer, l2p1, l2p2, 1, { 255,255,255 });
                SDL_SetRenderDrawColor(drawcolrenderer, 128, 128, 255, 255);
                SDL_SetRenderDrawColor(drawcolrenderer, collsides * 55, collsides * 50, collsides * 10, 255);
                SDL_RenderLine(drawcolrenderer, l1p1.x, l1p1.y, l1p2.x, l1p2.y);
                drawline(drawcolrenderer, l1p1, l1p2, 1, { 255,255,255 });
            }
            if (lineintersect(l1p1, l1p2, l2p1, l2p2, collidepoint))
            {
                collidepoints[colamount] = collidepoint;
                SDL_FPoint point1 = { collidepoint };
                polygon.push_back(point1);
                //std::cout << collidepoint.x << "  " << collidepoint.y<<"\n";
                colamount += 1;
                //cout << collidepoint.x << "  " << collidepoint.y<<"\n";
            }
            //cout << collidepoint.x << "  " << collidepoint.y << "\n";
            collidepoint = { 0,0 };
        }
    }

    vector<SDL_FRect> collidedcorners = {};
    vector<SDL_FRect> collidercollidedcorners = {};

    for (int counter = 3; counter >= 0; counter--)
    {
        SDL_FPoint currentpoint = { corners[counter].x, corners[counter].y };
        SDL_FPoint Ocurrentpoint = { othercorners[counter].x, othercorners[counter].y };
        //4 2 1
        //2 3 4
        if (tricollpnt(currentpoint, { collider.corner4.x, collider.corner4.y },
            { collider.corner2.x, collider.corner2.y }, { collider.corner1.x, collider.corner1.y })
            || (tricollpnt(currentpoint, { collider.corner2.x, collider.corner2.y },
                { collider.corner3.x, collider.corner3.y }, { collider.corner4.x, collider.corner4.y })))
        {
            collidedcorners.push_back(corners[counter]);
            SDL_FPoint point2 = { corners[counter].x, corners[counter].y };
            polygon.push_back(point2);
        }
        else {
        }

        if (tricollpnt(Ocurrentpoint, { corner4.x, corner4.y },
            { corner2.x, corner2.y }, { corner1.x, corner1.y })
            || (tricollpnt(Ocurrentpoint, { corner2.x, corner2.y },
                { corner3.x, corner3.y }, { corner4.x, corner4.y })))
        {
            SDL_FPoint point = { othercorners[counter].x, othercorners[counter].y };
            polygon.push_back(point);
            collidercollidedcorners.push_back(othercorners[counter]);
        }
        else {
        }
    }
    vector<SDL_FPoint> allinsidepoints;
    //vector<SDL_FPoint> allinsidepoints1;
    int bmp_x;
    int bmp_y;
    int o_bmp_x;
    int o_bmp_y;
    float bmp_width = this->width;
    float bmp_height = this->height;
    float o_bmp_width = collider.width;
    float o_bmp_height = collider.width;
    vector<SDL_FPoint> finalpoly;
    if (polygon.size() > 0) {
        finalpoly = convex_hull(polygon);
    }
    else {
        finalpoly = polygon;
    }

    /*
    for (SDL_FPoint drawpoly : polygon)
    {SDL_SetRenderDrawColor(renderer, 125, 125, 0, 255);
        SDL_RenderPoint(renderer, drawpoly.x, drawpoly.y);}
    for (SDL_FPoint drawpoly2 : finalpoly)
    {SDL_SetRenderDrawColor(renderer, 125, 0, 255, 255);
        SDL_RenderPoint(renderer, drawpoly2.x, drawpoly2.y);}
    */
    for (int drawinside = (int)(allinsidepoints = getallpolypoints(finalpoly, 1.0f)).size() - 1; drawinside >= 0; drawinside--)
    {
        collider.scrn_to_bmp_px((int)allinsidepoints[drawinside].x, (int)allinsidepoints[drawinside].y, o_bmp_x, o_bmp_y);
        this->scrn_to_bmp_px((int)allinsidepoints[drawinside].x, (int)allinsidepoints[drawinside].y, bmp_x, bmp_y);
        SDL_SetRenderDrawColor(drawcolrenderer, 0, 255, 0, 255);
        SDL_RenderPoint(drawcolrenderer, allinsidepoints[drawinside].x, allinsidepoints[drawinside].y);
        if (bmp_x >= 0 && bmp_x < bmp_width &&
            bmp_y >= 0 && bmp_y < bmp_height &&
            o_bmp_x >= 0 && o_bmp_x < o_bmp_width &&
            o_bmp_y >= 0 && o_bmp_y < o_bmp_height)
        {
            int64_t index = (int64_t)bmp_y * (int64_t)width + (int64_t)bmp_x;
            int64_t o_index = (int64_t)o_bmp_y * (int64_t)collider.width + (int64_t)o_bmp_x;
            //std::cout << "index1: " << index << " bitmask size: " << this->bitmask.size()
            //    << "  index2: " << o_index << " bitmask size: " << collider.bitmask.size() << "\n";
            if (o_index < (signed)collider.bitmask.size() && index < (signed)this->bitmask.size())
            {
                //this caps the max values because my lazy ass doesnt want to
                //figure out what's wrong with this function
                if (collider.bitmask[o_index] && this->bitmask[index]) {
                    if (showgoodcollide)
                    {
                        SDL_SetRenderDrawColor(drawcolrenderer, 255, 0, 0, 255);
                        SDL_RenderPoint(drawcolrenderer, allinsidepoints[drawinside].x, allinsidepoints[drawinside].y);
                    }
                    collides = true;
                    if (!showgoodcollide) break;
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
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = duration_cast<chrono::milliseconds>(stop2 - start2);
    //std::cout << duration2.count()<<"\n";
    return collides;
}
/*
update function for actors will not be drawn but still need math and updates
*/
void Actor::update(SDL_Renderer* renderer)
{
    if (bitmask.empty())
    {
        this->makebitmask();
    }
    if (old_x != x || old_y != y || old_angle != angle || !notcalculated)
    {
        rotatecollide();
    }
    if (setsize == false)
    {
        setWD(renderer);
        setsize = true;
    }
    imgcenter = { width / 2 * scale,height / 2 * scale };
    old_x = x;
    old_y = y;
    old_angle = angle;
}
/*
creates the texture of the actor that will be drawn to the screen if the texture has changed
*/
void Actor::maketextture(SDL_Renderer* renderer)
{

    coords = { x,y,width * scale,height * scale };
    if (!animated) return;
    if (imgname != oldimgname) path = dir();
    rendertextture = IMG_LoadTexture(renderer, path.c_str());
    oldimgname = imgname;
    oldrendertext = rendertextture;
}
void Actor::renderdebug(SDL_Renderer* renderer)
{
    float renderscale_x;
    float renderscale_y;
    SDL_GetRenderScale(renderer, &renderscale_x, &renderscale_y);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_FColor color = { 255, 0, 255, 255 };
    collide1[0].color = color;
    collide1[1].color = color;
    collide1[2].color = color;
    int textscale = 1;
    SDL_RenderGeometry(renderer, NULL, collide1, 3, NULL, 0);
    SDL_RenderGeometry(renderer, NULL, collide2, 3, NULL, 0);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderRect(renderer, &corner1);//red
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, corner1.x * renderscale_x - textscale, corner1.y * renderscale_y - textscale, "corner1");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderRect(renderer, &corner2);//blue
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, corner2.x * renderscale_x - textscale, corner2.y * renderscale_y - textscale, "corner2");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderRect(renderer, &corner3);//purple
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, corner3.x * renderscale_x - textscale, corner3.y * renderscale_y - textscale, "corner3");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderRect(renderer, &corner4);//yellow
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, corner4.x * renderscale_x - textscale, corner4.y * renderscale_y - textscale, "corner4");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);

    SDL_SetRenderDrawColor(renderer, 50, 255, 60, 255);
    SDL_RenderRect(renderer, &frontcent);//green
    SDL_SetRenderScale(renderer, (float)textscale, (float)textscale);
    SDL_RenderDebugText(renderer, frontcent.x * renderscale_x - textscale, frontcent.y * renderscale_y - textscale, "front");
    SDL_SetRenderScale(renderer, renderscale_x, renderscale_y);
}
/*
quickly and easily draw actor to specified renderer
automatically creating texture, texture coordnates,
x and y calculations, and image data.
TL;DR: draws actor to renderer
*/
void Actor::quickdraw(SDL_Renderer* renderer)
{
    using namespace std;
    if (this->imgsize == nullptr)
    {
        SDL_Log("Check the image name. did you forget.png?");
        return;
    }
    if (bitmask == vector<bool>(static_cast<size_t>(imgsize->w) * static_cast<size_t>(imgsize->h), false) && actortype >= normal)
    {
        /*this checks if the bitmap is
        unitilized and inits it if so*/
        this->init_good_collide();
    }
    if (old_x != x || old_y != y || old_angle != angle || notcalculated) rotatecollide();
    maketextture(renderer);
    imgcenter = { width / 2 * scale,height / 2 * scale };
    if (dodrawcollide) renderdebug(renderer);

    if (setsize == false) {
        setWD(renderer);
        setsize = true;
    }
    if (actortype != simple) {
        SDL_RenderTextureRotated(renderer, rendertextture, NULL, &coords, angle, &imgcenter, SDL_FLIP_NONE);
    }
    else {
        SDL_RenderTexture(renderer, rendertextture, NULL, &coords);
    }
    old_x = x;
    old_y = y;
    old_angle = angle;
}
/*
draws all points and simpe collision of the actor
*/
void Actor::drawcollide(SDL_Renderer* renderer, int R, int G, int B, int A)
{
    Uint8 orig_R, orig_G, orig_B, orig_A;
    float scale1, scale2;
    SDL_GetRenderDrawColor(renderer, &orig_R, &orig_G, &orig_B, &orig_A);
    SDL_GetRenderScale(renderer, &scale1, &scale2);
    SDL_SetRenderDrawColor(renderer, R, G, B, A);
    SDL_SetRenderScale(renderer, 2, 2);
    dodrawcollide = true;

    SDL_SetRenderScale(renderer, scale1, scale2);
    SDL_SetRenderDrawColor(renderer, orig_R, orig_G, orig_B, orig_A);
}
/*
basic rectangle collision, ignores angle, bitmap, and direction.
*/
bool Actor::rectcollide(SDL_FRect colliderect)
{
    //dumbass basic collison
    int rect_x = (int)coords.x, rect_y = (int)coords.y, rect_h = (int)coords.h, rect_w = (int)coords.w;
    const SDL_Rect actorrect = SDL_Rect{ rect_x, rect_y, rect_w, rect_h };
    int rect_x_o = (int)colliderect.x, rect_y_o = (int)colliderect.y, rect_h_o = (int)colliderect.h, rect_w_o = (int)colliderect.w;
    const SDL_Rect collisionrect = SDL_Rect{ rect_x_o, rect_y_o, rect_w_o, rect_h_o };
    return SDL_HasRectIntersection(&actorrect, &collisionrect);
}
