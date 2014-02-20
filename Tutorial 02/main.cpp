//tutorial 02 http://twinklebear.github.io/sdl2%20tutorials/2013/08/17/lesson-2-dont-put-everything-in-main/

#include <iostream>
#include <SDL.h>
#include <string>
using namespace std;

void logSDLError(ostream , const string);

int main(int argc, char **argv)
{
	const int SCREEN_WIDTH  = 640;
	const int SCREEN_HEIGHT = 480;
	//create window and renderer
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)//initialize SDL subsystems
	{
		logSDLError(cout, "SDL_Init");
		return 1;
	}
	//window
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



	return 0;
}

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(ostream &os, const string &msg)//we add this function so we can error check with less redundant code
{
	os << msg << " error: " << SDL_GetError() << endl;
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
//c_str Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the string object. This array includes the same sequence of characters that make up the value of the string object plus an additional terminating null-character ('\0') at the end.
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren)
{
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());//load image from string (the address of the file) passed in
	//If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr)//if the image loaded and therefore the loadedImage ptr is not pointed at nullptr (this is less buggy than using NULL as NULL has a value of 0 where as nullptr is a universal null pointer literal)
	{
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);//load image into the texture ptr
		SDL_FreeSurface(loadedImage);//Frees the resources used by a previously created SDL_Surface. If the surface was created using SDL_CreateRGBSurfaceFrom then the pixel data is not freed.
		//Make sure converting went ok too
		if (texture == nullptr)//if the texture still points to nullptr then it failed to load and therefor calls the error func to provide error details
			logSDLError(cout, "CreateTextureFromSurface");
	}
	else//if the loadImage ptr didn't pass it's if check (because it was pointed to nullptr call error func
		logSDLError(cout, "LoadBMP");

	return texture;
}


/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;//we will use this rect as the destination that the sprite will be drawn at.
	dst.x = x;//the x that we want the pic at (specificaly where the top left of the pic will be)
	dst.y = y;//the y
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);// params are: texture, format, width of texture, height of texture)
	SDL_RenderCopy(ren, tex, NULL, &dst);// used to coppy a portion of a texture. Params are: renderer, texture, srcrect (the source SDL_Rect structure or NULL for the entire texture), dstrect (the destination SDL_Rect structure or NULL for the entire rendering target. The texture will be stretched to fill the given rectangle.)
}