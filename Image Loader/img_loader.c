/*______________
*|[]            |
*|  __________  |
*|  | Radio  |  |
*|  | Shack  |  |
*|  |________|  |
*|   ________   |
*|   [ [ ]  ]   |
*\___[_[_]__]___|
*/


#include "img_loader.h"
#define USE_MATH_DEFINES 1
#include <math.h>
#include <assert.h>
/*
enum ptr_value valid_ptr_impl(void* ptr, bool hang_or_no, int calledline)
{
    switch ((uintptr_t)ptr)
    {
    case NULLPTR:
        if (DEBUGLOGS)SDL_Log("NULL ptr error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        return (enum ptr_value)VOID;
    case BAD_PTR:
        if (DEBUGLOGS)SDL_Log("bad ptr error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        return (enum ptr_value)BADPTR;
    case BAD_LOCAL:
        if (DEBUGLOGS)SDL_Log("bad local error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return(enum ptr_value)BADLOCAL;
    case H_UNALLOCATED:
        if (DEBUGLOGS)SDL_Log("heap unallocated error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return(enum ptr_value)UNALLOC;
    case S_UNALLOCATED:
        if (DEBUGLOGS)SDL_Log("stack unallocated error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return(enum ptr_value)UNALLOC;
    case WIN_UNALLOCATED:
        if (DEBUGLOGS)SDL_Log("windows unallocated error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return(enum ptr_value)UNALLOC;
    case FREED_MEM:
        if (DEBUGLOGS)SDL_Log("double free error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return(enum ptr_value)DOUBLE_FREE;
    case NULL:
        if (DEBUGLOGS)SDL_Log("NULL error (ptr was %p). called on line %i. Hanging program. . .", (uintptr_t)ptr, calledline);
        if (hang_or_no)__debugbreak();
        return (enum ptr_value) PTRISNULL;
    default:
        return(enum ptr_value)FINE;
    }
}
*/
int remove_value(uintptr_t* arr, int size, uintptr_t value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            // Shift everything left to remove the value
            for (int j = i; j < size - 1; j++) {
                arr[j] = arr[j + 1];
            }
            arr[size - 1] = 0; // Optional: clear last slot
            return size - 1;   // Return new size
        }
    }
    return size; // Value not found; size unchanged
}



/*
these huge functions are extensive debug functions, becuase
i am an idoiot and this leaked heaps of memory (no longer does),
so if you see some leaked pointers, maybe take a gander at my bad
code and use some of these to figure it out. 
*/

static void* leaked_mem[224000];
int calllines[224000];
size_t memspot = 1;

static void* (*real_malloc)(size_t) = malloc;
static void* (*real_realloc)(void*, size_t) = realloc;
static void (*real_free)(void*) = free;

void* custom_malloc(size_t size, int line)
{
    void* mem = real_malloc(size);
    SDL_Log("memspot: %i", (int)memspot);
    calllines[memspot] = line;
    leaked_mem[memspot++] = (uintptr_t)mem;
    SDL_Log("malloc at %i", line);
    //SDL_Log("memspot: %i", memspot);
    
    return mem;
}
/*
void* custom_realloc(void* ptr, size_t size)
{
    void* mem = real_realloc(ptr, size);
    leaked_mem[memspot] = (uintptr_t)ptr;
    //SDL_Log("memspot:%i", memspot);
    
    return mem;
}
*/
void custom_free(void* ptr, int line) {
    SDL_Log("FREE CALLED");

    if (memspot > 0) {
        remove_value(calllines, memspot, (uintptr_t)line);
        memspot = remove_value(leaked_mem, memspot, (uintptr_t)ptr);
    }

    real_free(ptr);
}
void dump_bytes(void* ptr, size_t count) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < count; i++) {
        SDL_Log("Byte[%zu] = 0x%02X", i, p[i]);
    }
}
void view_leaked()
{
    calllines[0] = 1;
    calllines[1] = 2;
    for (int counter = (int)(sizeof(leaked_mem) / sizeof(leaked_mem[0])) - 1; counter >= 0; counter--)
    {
        if (leaked_mem[counter] != NULL)
        {
            SDL_Log("Pointer not freed correctly:%p", leaked_mem[counter]);
            //SDL_Log("int: %i", leaked_mem[counter]);
            //SDL_Log("string: %s\0", leaked_mem[counter]);
            uintptr_t raw = (uintptr_t)leaked_mem[counter];
            
            SDL_Log("array spot:%i", counter);
            SDL_Log("Line caught:%i", calllines[counter]);
            
            SDL_Log("As pointer address     : %p", leaked_mem[counter]);
            SDL_Log("As unsigned integer    : %u", (unsigned int)raw);
            SDL_Log("As signed integer      : %d", (int)raw);
            SDL_Log("As unsigned long long  : %llu", (unsigned long long)raw);
            SDL_Log("As signed long long    : %lld", (long long)raw);
            SDL_Log("As hex                 : 0x%llX", (unsigned long long)raw);
            SDL_Log("As float               : %f", *(float*)&raw);
            SDL_Log("As double              : %f", *(double*)&raw);  // only valid if ptr points to a double
            SDL_Log("As char                : %c", (char*)leaked_mem[counter]);     // dereferencing pointer
            //SDL_Log("As int (dereferenced)  : %d", *(int*)leaked_mem[counter]);      // assuming valid
            //SDL_Log("As float (dereferenced): %f", *(float*)leaked_mem[counter]);    // assuming valid
            SDL_Log("sizeof normal: %i", (int)sizeof(leaked_mem[counter]));
            SDL_Log("sizeof derefrenced: %i", (int)sizeof(raw));
            
            
        }
    }
}


//ORINGAL DRAW FUNCTIONS BEGIN---ORINGAL DRAW FUNCTIONS BEGIN---ORINGAL DRAW FUNCTIONS BEGIN---ORINGAL DRAW FUNCTIONS BEGIN---ORINGAL DRAW FUNCTIONS BEGIN---ORINGAL DRAW FUNCTIONS BEGIN---
void dir(image* target, char* path, int64_t pathSize) {
    char buffer[1024];

    if (GetCurrentDir(buffer, sizeof(buffer)) != NULL) {
        strcat_s(buffer, sizeof(buffer), "\\images\\");
        //SDL_Log(target->imgname);
        strcat_s(buffer, sizeof(buffer), target->imgname);
        strcpy_s(path, pathSize, buffer);
        SDL_Log("dir finshed, result:%s", path);
    }
    else {
        perror("GetCurrentDir failed");
    }
}
void create_img(image* targetimg, const char imgname[pathlen], double x, double y, double angle, double scale)
{
    targetimg->x = x;
    targetimg->y = y;
    targetimg->imgname = imgname;
    targetimg->angle = angle;
    targetimg->scale = scale;
    targetimg->scaletype=SDL_SCALEMODE_NEAREST;
    dir(targetimg, targetimg->path, pathlen);
    targetimg->imgsize = IMG_Load(targetimg->path);
    targetimg->setsize = false;
    targetimg->textloaded = false;
    SDL_Log("Image Made with name:%s", imgname);
}
void setWD(image* target, SDL_Renderer* renderer)
{
    //using namespace std;
    //target->rendertextture = IMG_LoadTexture(renderer, target->path);
    target->imgsize = IMG_Load(target->path);
    if (target->imgsize == NULL)
    {
    perror("Image not loaded, imgsize was NULL pointer");
    return;
    }
    target->width = (float)target->imgsize->w;
    target->height = (float)target->imgsize->h;
    target->setsize = false;
}
int maketexture(image* target, SDL_Renderer* renderer)
{
    target->imgcenter = (SDL_FPoint){ target->width / 2 * target->scale,target->height / 2 * target->scale };
    target->coords = (SDL_FRect){ target->x,target->y,target->width * target->scale,target->height * target->scale };
    if (target->imgname != target->oldimgname) { 
        dir(target, target->path, pathlen);
        SDL_Log("target width:%i, target height:%i", target->width, target->height);
        //*target->path = "C:\\Users\\chris\\source\\repos\\Tic Tac Toe\\images\\drawX.png";
        target->rendertextture = IMG_LoadTexture(renderer, target->path);
        SDL_SetTextureScaleMode(target->rendertextture, SDL_SCALEMODE_NEAREST);
        setWD(target, renderer);
        //target->imgsize = IMG_Load(target->path);
    }
    //SDL_Log("loading image path:%s", target->path);

    //if (target->rendertextture != target->oldrendertext || target->textloaded)target->rendertextture = IMG_LoadTexture(renderer, target->path);
    if (target->textloaded) target->textloaded = false;



    target->oldimgname = target->imgname;
    target->oldrendertext = target->rendertextture;
    return 1;
}
/*
quickly draws the image to the screen; about as boilerplate as you can get it here
*/
void draw(image* target, SDL_Renderer* renderer)
{
    //SDL_CreateThread();
    //SDL_Thread* thread = SDL_CreateThread(maketexture, "maketexture thread", (target, renderer));
    maketexture(target, renderer);
    //SDL_FPoint center = (SDL_FPoint) { target->width / 2 * target->scale, target->height / 2 * target->scale };
    if (target->setsize) {
        setWD(target, renderer);
        SDL_SetTextureScaleMode(target->rendertextture, SDL_SCALEMODE_NEAREST);
    }
    //SDL_RenderTexture(renderer, target->rendertextture, &target->coords, NULL);
    SDL_RenderTextureRotated(renderer, target->rendertextture, NULL, &target->coords, target->angle, &target->imgcenter, SDL_FLIP_NONE);
}
void benchdraw(SDL_Renderer* renderer, image* thingtodraw) {
    uint64_t start = SDL_GetPerformanceCounter();

    SDL_FRect dst = { 0, 0, 64, 64 };
    SDL_FPoint center = { 32, 32 };
    int drawamount = 10000;
    for (int i = 0; i < drawamount; ++i) {
        dst.x = (float)(rand() % 800);
        dst.y = (float)(rand() % 600);
        double angle = rand() % 360;
        float scale = rand() % 10;
        thingtodraw->angle = angle;
        thingtodraw->x = dst.x;
        thingtodraw->y = dst.y;
        thingtodraw->scale = scale;
        draw(thingtodraw, renderer);
        //SDL_RenderTextureRotated(renderer, texture, NULL, &dst, angle, &center, SDL_FLIP_NONE);
    }

    uint64_t end = SDL_GetPerformanceCounter();
    double elapsed_ms = (double)(end - start) * 1000.0 / SDL_GetPerformanceFrequency();

    SDL_Log("rendered %i rotated textures in %.3f ms\n", drawamount, elapsed_ms);
}
//ORINGAL DRAW FUNCTIONS END---ORINGAL DRAW FUNCTIONS END---ORINGAL DRAW FUNCTIONS END---ORINGAL DRAW FUNCTIONS END---ORINGAL DRAW FUNCTIONS END---ORINGAL DRAW FUNCTIONS END---
void resetbatch(const char* batchname)
{
    for (int allbats = numofbatches - 1; allbats >= 0; allbats--)
    {
        /*
        if (allbatches[allbats] == NULL)
        {
            return;
        }
        */
        /*
        const char* current_name = "<null>";
        if (allbatches[allbats].batch_imgname != NULL) {
            current_name = _strdup(allbatches[allbats].batch_imgname);
        }
        SDL_Log("allbats:%i, checking for:%s current checking:%s",
            allbats,
            batchname ? batchname : "<null>",
            current_name);
        */
        //SDL_Log("imgname being compared: %s", allbatches[allbats].batch_imgname);
        if (batchname == NULL)
        {
            perror("bathcname was nul in resetbatch");
            continue;
        }
        if (allbatches[allbats].batch_imgname == NULL)
        {
            if (DEBUGLOGS)SDL_Log("batch_imgname is nul, continuing. . .");
            continue;
        }
        if (valid_ptr(allbatches[allbats].batch_imgname, false)!=FINE)continue;
        
        //if (valid_ptr(batchname, false) != FINE)continue;

        if (strcmp(allbatches[allbats].batch_imgname,batchname) == 0)
        {
            free(allbatches[allbats].indices);
            free(allbatches[allbats].vertices);
            free(allbatches[allbats].batch_imgname);
            if (DEBUGLOGS)SDL_Log("FREED BATCH %s", batchname);
            SDL_DestroyTexture(allbatches[allbats].batch_texture);
            
            allbatches[allbats].indices = NULL;
            allbatches[allbats].vertices = NULL;
            allbatches[allbats].batch_imgname = NULL;
            allbatches[allbats].batch_texture = NULL;
            
            //free(&allbatches[allbats]);
            //memset(&allbatches[allbats], NULL, sizeof(batch));
            numofbatches--;
            continue;
        }
    }
}
void make_batch_texture(batch* target, SDL_Renderer* ren)
{
    if (target->batch_texture==NULL)//POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----POSSIBLE MEMORY LEAK-----
    {
        char fillablepath[pathlen];
        char buffer[1024];
        
        if (GetCurrentDir(buffer, sizeof(buffer)) != NULL) {
            strcat_s(buffer, sizeof(buffer), "\\images\\");
            strcat_s(buffer, sizeof(buffer), target->batch_imgname);
            strcpy_s(fillablepath, pathlen, buffer);
        }

        target->batch_texture = IMG_LoadTexture(ren, fillablepath);
    }
}
void drawbatch(const char* batchname, SDL_Renderer* ren)
{
    for (int findbatch = numofbatches-1; findbatch >= 0; findbatch--)
    {
        if (strcmp(allbatches[findbatch].batch_imgname, batchname) == 0)
        {
            /*
            if (findbatch % 4)
            {
                SDL_FColor red = {255.f, 0.f, 0.f, 255.f};
                SDL_Vertex hardcoded_tri[3] = { (SDL_Vertex){ {10, 10}, red, 1.f },
                (SDL_Vertex) { {200, 200}, red, 1.f},
                (SDL_Vertex) { {10, 200}, red, 1.f } };
                SDL_Vertex triangle[3] = {target.vertices[0], target.vertices[1],target.vertices[2] };
                triangle[0].color = (SDL_FColor){ 255.f, 0.f, 0.f, 255.f };
                triangle[1].color = (SDL_FColor){ 255.f, 0.f, 0.f, 255.f };
                triangle[2].color = (SDL_FColor){ 255.f, 0.f, 0.f, 255.f };
                for (int i = 0; i < 3; ++i) {
                    //SDL_Log("x:%f, y:%f", allbatches[findbatch].vertices[findbatch+i].position.x,allbatches[findbatch].vertices[findbatch+i].position.y);
                }
                //SDL_Log("-----------------------------------------------------------");
                //SDL_RenderGeometry(ren, NULL, hardcoded_tri, 3, NULL, 3);
                //SDL_RenderGeometry(ren, NULL, triangle, 3, NULL, 3);
            }
            */
            /*
            SDL_Log(
                "resetbatch called:\n"
                "  batch_imgname: %s\n"
                "  batch_texture: %p\n"
                "  indices pointer: %p, indicespot: %d\n"
                "  vertices pointer: %p, vertexspot: %d\n"
                "  next realloc sizes - indices: %d, vertices: %d\n",
                allbatches[findbatch].batch_imgname ? allbatches[findbatch].batch_imgname : "NULL",
                (void*)allbatches[findbatch].batch_texture,
                (void*)allbatches[findbatch].indices,
                allbatches[findbatch].indicespot,
                (void*)allbatches[findbatch].vertices,
                allbatches[findbatch].vertexspot,
                allbatches[findbatch].indicespot + 6,
                allbatches[findbatch].vertexspot + 4
            );
            */
            //allbatches[findbatch].batch_texture = IMG_LoadTexture(ren, "C:\\git\\SDL-Addons\\Actor\\C++\\Example\\images\\Example.png");
            make_batch_texture(&allbatches[findbatch], ren);

            if (DEBUGLOGS)SDL_Log("Drawing Batch");
            
            SDL_SetTextureScaleMode(allbatches[findbatch].batch_texture, SDL_SCALEMODE_NEAREST);

            SDL_RenderGeometry(ren, allbatches[findbatch].batch_texture , allbatches[findbatch].vertices, 
            allbatches[findbatch].vertexspot, allbatches[findbatch].indices, allbatches[findbatch].indicespot);
            //SDL_DestroyTexture(allbatches[findbatch].batch_texture);
            //SDL_Log("Texture: %p", (void*)target.batch_texture);
            //SDL_Log("Vertex[0]: %f, %f", target.vertices[0].position.x, target.vertices[0].position.y);
            return;
        }
    }
    //SDL_Log("No batches named %s found", batchname);
}
void addnewbatch(image* target, batch** assignedbatch, SDL_Renderer* ren)
{
    //_CrtSetBreakAlloc(25527865);
    //SDL_Log("adding new batch");
    batch *tempalloc = realloc(allbatches, (numofbatches + 1) * sizeof(batch));
    if (valid_ptr(tempalloc, true) !=FINE) {
        perror("could not allocate new batch");
        //free(tempalloc);
        return;
    }
    allbatches = tempalloc;
    memset(&allbatches[numofbatches], NULL, sizeof(batch));
    if (DEBUGLOGS)SDL_Log("batch count:%i", numofbatches);
    *assignedbatch = &allbatches[numofbatches]; 
    numofbatches++;
    
    (*assignedbatch)->vertexspace = 6;
    (*assignedbatch)->indicespace = 6;
    int* tempindice;
    SDL_Vertex* tempvertex;
    tempindice = realloc((*assignedbatch)->indices, 6 * sizeof(int));

    if (tempindice == NULL) {
        perror("malloc failed for indices in function add new batch");
        return;
    }

    tempvertex = realloc((*assignedbatch)->vertices, 6 * sizeof(SDL_Vertex));
    if (tempvertex == NULL) {
        perror("malloc failed for vertices in function add new batch");
        return;
    }
    (*assignedbatch)->indices = tempindice;
    (*assignedbatch)->vertices = tempvertex;
    
    //SDL_DestroyTexture((*assignedbatch)->batch_texture);
    if (target->imgname != NULL && target->imgname != BAD_PTR){
        //free((*assignedbatch)->batch_imgname);
        (*assignedbatch)->batch_imgname = _strdup(target->imgname);
    }
    else{
        free((*assignedbatch)->batch_imgname);
        (*assignedbatch)->batch_imgname = NULL;
        perror("Failed to strdup batch_imgname");
        return;
    }
    if (DEBUGLOGS)SDL_Log("setting vertexspot and indicespot to 0, prevous values vrtspt:%i, indcspot:%i",
    (*assignedbatch)->vertexspot, (*assignedbatch)->indicespot);
    (*assignedbatch)->vertexspot = 0;
    (*assignedbatch)->indicespot = 0;
}
void addtobatch(image* target, SDL_Renderer* ren)
{
    static int64_t recent_value;
    static char* last_target="if you see this value something messed up";
    /*
    if (DEBUGLOGS)
    {
        #define malloc(size) custom_malloc(size, __LINE__)
        #define realloc(ptr, size) custom_realloc(ptr, size)
        #define free(ptr) custom_free(ptr, __LINE__)
    }
    */
    bool found_premade_batch = false;
    //static bool firstrun = true;
    /*if (firstrun)
    {
        allbatches = NULL;
        SDL_Log("this should run once");
        firstrun = false;
    }
    */
    //image s = *target;
    //SDL_Texture* tex = target->rendertextture;
    batch* assignedbatch = NULLPTR;
    if (allbatches == NULL) {
        addnewbatch(target, &assignedbatch, ren);
    }
    else {
        if (target->imgname == 0)
        {
            if (DEBUGLOGS)SDL_Log("s.imgname is 0");
            return;
        }
        for (int64_t checkimg = numofbatches-1; checkimg >=0; checkimg--)
        {
            //SDL_Log("iterate");
            //SDL_Log("s img:%s bat img:%s", s.imgname, allbatches[checkimg].batch_imgname);
            //SDL_Log("checkimg=%lld, batch_imgname=%p, s.imgname=%p", checkimg,
            //    (void*)allbatches[checkimg].batch_imgname, (void*)s.imgname);

            //allbatches[checkimg].batch_imgname = NULLPTR;
            
            if (valid_ptr(allbatches[checkimg].batch_imgname, false) != FINE) {
                addnewbatch(target, &assignedbatch, ren);
                if (DEBUGLOGS)SDL_Log("in addtobatch .batchimgname was null");
                break;
            }
            
            /*
            if (valid_ptr(target->imgname, false) != FINE)
            {
                addnewbatch(target, &assignedbatch, ren);
                if (DEBUGLOGS)SDL_Log("in addtobatch target->imgname was null");
                break;
            }
            */
            //SDL_Log("%s", allbatches[checkimg].batch_imgname);
            //SDL_Log("checkimg=%lld", checkimg);
            //SDL_Log("batch_imgname=%p, target->imgname=%p", 
            //(void*)allbatches[checkimg].batch_imgname, (void*)target->imgname);
            bool match = strcmp(allbatches[checkimg].batch_imgname, target->imgname);
            
            bool lastmatch = false;
            int index = checkimg;
            
            if (strcmp(target->imgname, last_target) == 0) {
                if (recent_value >= 0 && recent_value < numofbatches) {
                    //SDL_Log("recent_value:%i", recent_value);
                    lastmatch = true;
                    index = recent_value;
                    match = 0;
                }
                else{
                    lastmatch = false;
                }
            }
            
            if (match==0|| lastmatch)
            {
                assignedbatch = &allbatches[index];
                //assert(assignedbatch != NULL);
                //assignedbatch->indices = NULL;
                //assignedbatch->vertices = NULL;
                /*
                free(assignedbatch->indices);
                assignedbatch->indices = NULL;
                free(assignedbatch->vertices);
                assignedbatch->vertices = NULL;
                */
                recent_value = checkimg;
                last_target = target->imgname;
                //valid_ptr(assignedbatch->indices, true);
                //SDL_Log("indices:%p", assignedbatch->indices);
                int* temppointer=realloc(
                    assignedbatch->indices,
                    (assignedbatch->indicespot + (int64_t)6) * sizeof(int)
                );
                if (temppointer == NULL) {
                    perror("could not allocate temppointer in function addtobatch");
                    return;
                }
                assignedbatch->indices = temppointer;
                //temppointer = NULL;
                SDL_Vertex* tempvertex = realloc(
                    assignedbatch->vertices,
                    (assignedbatch->vertexspot + (int64_t)6) * sizeof(SDL_Vertex)
                );
                if (tempvertex == NULL) {
                    perror("could not allocate tempvertex in function addtobatch");
                    return;
                }
                assignedbatch->vertices = tempvertex;
                assignedbatch->vertexspace += 6;
                assignedbatch->indicespace += 6;
                //tempvertex = NULL;
                found_premade_batch = true;
                break;
            }
        }
    }
    if (!found_premade_batch)
    {
        addnewbatch(target, assignedbatch, ren);
    }
    //SDL_FPoint center = target->imgcenter;
    float angle_rad = target->angle * (3.1415926535 / 180.0f);
    float cos_a = cosf(angle_rad);
    float sin_a = sinf(angle_rad);
    if (target->width == 0)
    {
        setWD(target, ren);
        //if (DEBUGLOGS)SDL_Log("SETTING WIDTH AND HEIGHT, SHOULD HAPPEN ONCE, POSSIBLE MEMORY LEAK HERE");
    }
    float w = target->width * target->scale;
    float h = target->height * target->scale;
    //SDL_Log("WIDTH:%i HEIGHT:%i", (int)target->width, (int)target->height);//---------------------------------------------------------------------------------------------------------
    SDL_FPoint local[4] = {
        {-w / 2, -h / 2},
        { w / 2, -h / 2},
        { w / 2,  h / 2},
        {-w / 2,  h / 2},
    };

    SDL_FPoint world[4];
    for (int j = 0; j < 4; ++j) {
        //float x = local[j].x;
        //float y = local[j].y;
        world[j].x = target->x + (local[j].x * cos_a - local[j].y * sin_a);
        world[j].y = target->y + (local[j].x * sin_a + local[j].y * cos_a);
    }
    //for (int j = 0; j < 4; ++j) {
    //    SDL_Log("world[%d]: x=%.2f y=%.2f", j, world[j].x, world[j].y);
    //}
    if (assignedbatch == NULL) {
        if (DEBUGLOGS)SDL_Log("NULL pointer found");
        return;
    }
    //checks for max int value
    if (assignedbatch->vertexspot == 3722304989) { 
        assignedbatch->vertexspot = 4;
        if (DEBUGLOGS)SDL_Log("assignedbatch->vertexspot was at max unsigned int");
    }
    //red for simple re-usability
    //SDL_Log("asigndbatch size is:%lli", sizeof(*assignedbatch->vertices));
    //SDL_Log("%p", assignedbatch->vertices);
    //SDL_Log("vi:%i", vi);
    if (valid_ptr(assignedbatch->vertices, false) != FINE )
    {
        if (DEBUGLOGS)SDL_Log("assignedbatch verticies is null in addtobatch");
        return;
    }
    //SDL_Log("assignedbatch: %p", (void*)assignedbatch);
    //SDL_Log("vertices: %p", (void*)assignedbatch->vertices);
    //SDL_Log("vertexspot: %lld", assignedbatch->vertexspot);
    //SDL_Log("verticies:%p, vi:%i, ii:%i, vertexspace:%i, indicespace:%i", assignedbatch->vertices, vi, ii, assignedbatch->vertexspace, assignedbatch->indicespace);
    //SDL_Log("verticies:%p v", assignedbatch->vertices);
    //if (vi > 60000 || vi < 0)SDL_Log("vi: %lld", vi);
    //SDL_Log("vi: %lld", vi);
    ///if (vi >60000)vi = 1;
    //SDL_Log("vi: %lld verticies:%p", vi, assignedbatch->vertices);
    //assigned the verticies to correct world positions
    assignedbatch->vertices[assignedbatch->vertexspot + 0] = (SDL_Vertex){ world[0], { 1.f, 1.f, 1.f, 1.f }, {0, 0} };
    assignedbatch->vertices[assignedbatch->vertexspot + 1] = (SDL_Vertex){ world[1], { 1.f, 1.f, 1.f, 1.f }, {1, 0} };
    assignedbatch->vertices[assignedbatch->vertexspot + 2] = (SDL_Vertex){ world[2], { 1.f, 1.f, 1.f, 1.f }, {1, 1} };
    assignedbatch->vertices[assignedbatch->vertexspot + 3] = (SDL_Vertex){ world[3], { 1.f, 1.f, 1.f, 1.f }, {0, 1} };

    //assigned the indices in order for rendering
    assignedbatch->indices[assignedbatch->indicespot + 0] = assignedbatch->vertexspot + 0;
    assignedbatch->indices[assignedbatch->indicespot + 1] = assignedbatch->vertexspot + 2;
    assignedbatch->indices[assignedbatch->indicespot + 2] = assignedbatch->vertexspot + 1;
    assignedbatch->indices[assignedbatch->indicespot + 3] = assignedbatch->vertexspot + 0;
    assignedbatch->indices[assignedbatch->indicespot + 4] = assignedbatch->vertexspot + 3;
    assignedbatch->indices[assignedbatch->indicespot + 5] = assignedbatch->vertexspot + 2;
    //SDL_Log("adding to vertexspot and indicespot, prevoius values vrtsspot:%i, indcespt:%i", assignedbatch->vertexspot, assignedbatch->indicespot);
    //adds to these so we know how many verticies and indicies there are
    assignedbatch->vertexspot += 4;
    assignedbatch->indicespot += 6;
    //SDL_Log("finished function addtobatch");
}
void benchbatchdraw(SDL_Renderer* ren, image* drawthis)
{
    uint64_t start = SDL_GetPerformanceCounter();
    static float angle = 0;
    for (int i = 0; i < 10000; ++i) {
        //draw(drawthis, ren);
        //SDL_FRect dst = { 0, 0, 64, 64 };
        //SDL_FPoint center = { 32, 32 };
        //dst.x = (float)(rand() % 800);
        //dst.y = (float)(rand() % 600);
        //double angle = rand() % 360;
        //float scale = rand() % 5;
        //drawthis->angle = angle;
        //drawthis->x = dst.x;
        //drawthis->y = dst.y;
        //drawthis->scale = scale;
        if (angle < 360)angle += 0.1;
        else angle = 0;
        drawthis->angle = angle;
        drawthis->x += (angle*2)-360;
        addtobatch(drawthis, ren);
        //SDL_RenderTextureRotated(renderer, texture, NULL, &dst, angle, &center, SDL_FLIP_NONE);
    }
    uint64_t end1 = SDL_GetPerformanceCounter();
    double elapsed_ms1 = (double)(end1 - start) * 1000.0 / SDL_GetPerformanceFrequency();
    drawbatch(drawthis->imgname, ren);
    resetbatch(drawthis->imgname);
    uint64_t end = SDL_GetPerformanceCounter();
    double elapsed_ms = (double)(end - start) * 1000.0 / SDL_GetPerformanceFrequency();

    SDL_Log("total render time for 10000 is % .3f ms, batch time : % .3f ms, render time : % .3f ms", elapsed_ms, elapsed_ms1, elapsed_ms- elapsed_ms1);
}
void freebatches() {
    for (int i = 0; i < numofbatches; ++i) {
        resetbatch(allbatches[i].batch_imgname);
    }
    free(allbatches);
    allbatches = NULL;
    numofbatches = 0;
    if (DEBUGLOGS)SDL_Log("FREED ALL BATCHES, LEAKED POINTERS:");
    if (DEBUGLOGS)view_leaked();
}