#pragma once
#include <SDL3/SDL.h>
#include <SDL_image.h>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define pathlen 250
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define pathlen 4096
#endif

//#include <crtdbg.h>
//#ifdef _DEBUG

//#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define calloc(c,s) _calloc_dbg(c,s,_NORMAL_BLOCK, __FILE__, __LINE__)
//#define realloc(p,s) _realloc_dbg(p,s,_NORMAL_BLOCK, __FILE__, __LINE__)

//#define free(p) _free_dbg(p, _NORMAL_BLOCK)
//#endif


typedef struct{
	/*these are the initialy defined vatribles*/
	char* imgname;
	double x;
	double y;
	double angle;
	double scale;
	Uint8 alpha;
	/*these are for logic in functions*/
	char path[pathlen];
	char* oldimgname;//other path for comparing
	SDL_Surface* imgsize;// surface for getting image size quickly, you cloud use and loaded surface, but this is better for debugging
	//SDL_Surface* pixeldata;//surface with image data for access
	SDL_Texture* rendertextture;// texture to be rendered to the screen
	SDL_Texture* oldrendertext;//texture for comparing
	SDL_FRect coords;// coridanted stored in SDL_FRect
	int64_t width;//width of the image
	int64_t height;//height of the image
	SDL_FPoint imgcenter;
	bool setsize;
	bool textloaded;
	enum SDL_SCALEMODE scaletype;
} image;
typedef struct {
	const char* batch_imgname;//the name of the batch's texture's filename
	SDL_Texture* batch_texture;//the texture assigned to that batch
	SDL_Vertex* vertices;//all the verticies of the batch
	int* indices;

	unsigned int indicespot;
	unsigned int vertexspot;

	int vertexspace;
	int indicespace;
} batch;

enum ptr_value{
	UNALLOC=0,
	FINE=true,
	BADLOCAL= 2,
	DOUBLE_FREE=3,
	VOID=4,
	BADPTR=5,
	PTRISNULL=6
};

bool DEBUGLOGS = false;

#define NULLPTR ((uintptr_t)0xBULL)
#define BAD_PTR (uintptr_t)0xFFFFFFFFFFFFFFFF
#define H_UNALLOCATED (uintptr_t)0xFDFDFDFD 
#define S_UNALLOCATED (uintptr_t)0xCDCDCDCDCDCDCDCD 
#define BAD_LOCAL (uintptr_t)0xCCCCCCCC 
#define FREED_MEM (uintptr_t)0xDDDDDDDDDDDDDDDD  
#define WIN_UNALLOCATED (uintptr_t)0xBAADF00D 
static inline enum ptr_value valid_ptr_impl(void* ptr, bool hang_or_no, int calledline, const char* file)
{
	switch ((uintptr_t)ptr)
	{
	case NULLPTR:
		if (hang_or_no)SDL_Log("NULL ptr error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		return (enum ptr_value)VOID;
	case BAD_PTR:
		if (hang_or_no)SDL_Log("bad ptr error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		return (enum ptr_value)BADPTR;
	case BAD_LOCAL:
		if (hang_or_no)SDL_Log("bad local error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return(enum ptr_value)BADLOCAL;
	case H_UNALLOCATED:
		if (hang_or_no)SDL_Log("heap unallocated error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return(enum ptr_value)UNALLOC;
	case S_UNALLOCATED:
		if (hang_or_no)SDL_Log("stack unallocated error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return(enum ptr_value)UNALLOC;
	case WIN_UNALLOCATED:
		if (hang_or_no)SDL_Log("windows unallocated error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return(enum ptr_value)UNALLOC;
	case FREED_MEM:
		if (hang_or_no)SDL_Log("double free error (ptr was %p). called on line %i in file %s Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return(enum ptr_value)DOUBLE_FREE;
	case NULL:
		if (hang_or_no)SDL_Log("NULL error (ptr was %p). called on line %i in file %s. Hanging program. . .", (uintptr_t)ptr, calledline, file);
		if (hang_or_no)__debugbreak();
		return (enum ptr_value)PTRISNULL;
	default:
		return(enum ptr_value)FINE;
}
}
#define valid_ptr(ptr, hang_or_no) valid_ptr_impl(ptr, hang_or_no, __LINE__, __FILE__)
/*
typedef struct {
	image* target;
	batch** assignedbatch;
	SDL_Renderer* ren;
} addbatch;
*/

batch* allbatches;
int64_t numofbatches = 1;
#ifdef __cplusplus
extern "C" {
#endif
void dir(image* target, char* path, int64_t pathSize);
void create_img(image* targetimg, const char imgname[pathlen], double x, double y, double angle, double scale);
int maketexture(image* target, SDL_Renderer* renderer);
void draw(image* target, SDL_Renderer* renderer);
void benchdraw(SDL_Renderer* renderer, image* thingtodraw);
void addtobatch(image* target);
//void getbatchnames(char* namefiller);
void drawbatch(const char* batchname, SDL_Renderer* ren);
void benchbatchdraw(SDL_Renderer* ren, image* drawthis);
void resetbatch(const char* batchname);
void freebatches();
#ifdef __cplusplus
}
#endif