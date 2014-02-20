//http://www.willusher.io/sdl2%20tutorials/2013/08/18/lesson-3-sdl-extension-libraries/
//*note pointers: say we have 2 different pointers ip and ip2
/*
	to set a pointer to a value use *ip = 5; To set a pointer to a memory address use ip = ip2;

*/
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>//we are incorporating this external library so we can work with files other than bitmaps. Like with SDL there is a .lib that must be in the debug folder to run the debug .exe file
#include <string>
using namespace std;
//func prototypes
SDL_Texture* loadTexture(const string&, SDL_Renderer*);
void logSDLError(ostream&, const string&);
void renderTexture(SDL_Texture*, SDL_Renderer*, int, int,int, int);
void renderTexture(SDL_Texture*, SDL_Renderer*, int, int);//our overloaded function needs it's own prototype


int main(int argc, char **argv)
{
	//constants
	const int SCREEN_WIDTH  = 640;
	const int SCREEN_HEIGHT = 480;
	//We'll just be using square tiles for now
	const int TILE_SIZE = 40;

	//initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//initialize SDL_Images image loading subsys (it will do it itself when an image is loaded for the first time but this causes some slow down (or can))
	/*When loading an image for the first time SDL_image will automatically initliaze the necessary image loading subsystem, however this will cause some delay in 
	loading the image since SDL_image will have to perform its initialization setup first. If you’d like to initialize SDL_image earlier to avoid the delay when loading an image 
	type for the first time you can do so with IMG_Init. IMG_Init will return a bitmask of all the currently initialized image loaders, so we’ll perform an and with the flags we 
	passed to see if initialization was successful. Here we only initialize the PNG loader since that’s all we’ll need for this lesson. This initialization should be done after initializing SDL.
	*/

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		logSDLError(std::cout, "IMG_Init");
		return 1;

	}
	//window and renderer are created the same way as they were in tutorial 2
	SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//name, position to apear x, y, width of screen, height, flags for window (SDL_WINDOW_FULLSCREEN makes it fullscreen, the used one assumably makes the window visible (there is SDL_WINDOW_HIDDEN to make it hidden)
	if (window == nullptr)
	{
		logSDLError(cout, "CreateWindow");
		return 2;
	}
	//create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //name of window, -1 (calls first renderer that meets our requirements), hardware acceleration, vsync
	if (renderer == nullptr)
	{
		logSDLError(cout, "CreateRenderer");
		return 3;
	}

	//load textures
	SDL_Texture *background = loadTexture("res/background.png", renderer);
	SDL_Texture *image = loadTexture("res/image.png", renderer);
	//Make sure they both loaded ok
	if (background == nullptr || image == nullptr)
		return 4;

	//now we will tile the background. Since our tiles are going to be smaller (which means that we need more of them) than they were in lesson 2 we want to use an algorythm instead of typeing each position
	//Determine how many tiles we'll need to fill the screen
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;

	//Draw the tiles by calculating their positions
	for (int i = 0; i < xTiles * yTiles; ++i){
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,TILE_SIZE);
	}

	//now to draw the forground
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);

	SDL_RenderPresent(renderer);//draw the renderer on screen
	SDL_Delay(2000);

	return 0;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());//set memory 
	if (texture == nullptr)//if image not loaded then texture will point to nullptr	
		logSDLError(cout, "LoadTexture");//calls our log function to preint an error
	return texture;//if all went right a texture is returned
}

/*
the error function that we used in the last tutorial
*/

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << endl;
}


/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
//overloaded function to draw sprites to renderer without scaling
/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);//Use this function to query the attributes of a texture. Params SDL_Texture* texture, Uint32*  format,   int*         access,int*     width, int*     height) *note the reason we are using so many pointers for these functions is becauser SDL is actually written in C and not C++ which means pointers for things that C++ wouldn't need them for
	renderTexture(tex, ren, x, y, w, h);//calls our unoverloaded renderTexture function and since w and h are defined but not set (and vars and not pointers) they have a value of 0
}