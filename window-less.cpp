#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


int width = 1024, height = 768, tileSize = 128;

char *basepath = SDL_GetBasePath();
string path = basepath;
const string direct = path;


void sdlError(ostream &os, const string &message)
{
	os << message << "error: " << SDL_GetError() << endl;
	SDL_Delay(2000);
}
SDL_Texture* loadTexture(const string &file, SDL_Renderer *renderer)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, file.c_str());
	if (texture == nullptr) {
		sdlError(cout, "LoadTexture");
	}
	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h) //scale to rectangle we input into function
{
	SDL_Rect destination; //destination of rectangle
	destination.x = x;
	destination.y = y;
	destination.w = w;
	destination.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &destination);

}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) // no scaling, width and height based on texture
{
	SDL_Rect destination; //destination of rectangle
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h); //figures out texture size and places it on the rectangle
	renderTexture(texture, renderer, x, y, w, h);

}

int limitHeight(int y, int height)
{

	if (y > height -75)
	{
		
		y = height - 75;
	}
	if (y < 0)
	{
		
		y = 0;
	}
	return y;
}
int limitWidth(int x,  int width)
{

	if (x > width-50)
	{
		
		x = width - 40;
	}
	if (x < 0)
	{
		
		x = 0;
	}
	return x;
}

int main(int argc, char* args[])
{
	srand(NULL);
	int xTile = width / tileSize;
	int yTile = height / tileSize;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		sdlError(cout, "SDL_Init");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Window-less", 100, 100, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		sdlError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	{
		if (renderer == nullptr)
		{
			sdlError(cout, "CreateRenderer");
			SDL_Quit();

			return 1;
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
	}


	SDL_Texture *background = loadTexture(direct + "backgroundspace.png", renderer);
	SDL_Texture *image = loadTexture(direct + "rocketman.png", renderer);
	SDL_Texture *startScreen = loadTexture(direct + "start.png", renderer);
	SDL_Texture *star = loadTexture(direct + "star.png", renderer);
	SDL_Texture *gameover = loadTexture(direct + "gameover.png", renderer);

	if (background == nullptr || image == nullptr)
	{
		SDL_Quit();
		return 1;
	}


	int textureWidth, textureHeight;
	SDL_QueryTexture(image, NULL, NULL, &textureWidth, &textureHeight);
	int x = width  - textureWidth;
	int y = height  - textureHeight;

	int starMax = 1;
	int starCurrent = 0;
	int moveSpeed = 50;
	bool quit = false;
	SDL_Event e;
	unsigned int count = 0;
	unsigned int currentTime;
	unsigned int lastTime = 0;
	int starSpawnX = 0;
	int starSpawnY = 0;

	int addTick = 0;
	int minusTick = 0;
	bool start = false;
	SDL_RenderClear(renderer);
	renderTexture(startScreen, renderer, 0, 0); // 0,0 fills screen
	SDL_RenderPresent(renderer);

	while (start == false)
	{

		while (SDL_PollEvent(&e)) // while checking event queue
		{
			//If user closes the window
			if (e.type == SDL_KEYDOWN)
			{
				SDL_RenderClear(renderer);
				start = true;
			}
		}
	}

	while (quit == false)
	{



		while (SDL_PollEvent(&e)) // while checking event queue
		{



			//If user closes the window
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//If user presses a key
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					SDL_RenderClear(renderer);
				case SDLK_w:
					y = y - moveSpeed;

					break;
				case SDLK_s:
					y = y + moveSpeed;

					break;
				case SDLK_d:
					x = x + moveSpeed;


					break;
				case SDLK_a:

					x = x - moveSpeed;


					break;
				}

			}
		}

		int checkRand = rand() % width;
		
		currentTime = SDL_GetTicks();

		if (currentTime > lastTime + 1000)
		{

			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			
			width = width - 40;
			height = height - 40;
			starSpawnX = starSpawnX - 25;
			starSpawnY = starSpawnY - 25;

			lastTime = currentTime;

		}

		SDL_SetWindowSize(window, width, height);

		x = limitWidth(x, width);
		y = limitHeight(y, height);
		SDL_RenderClear(renderer);


		for (int i = 0; i < xTile * yTile; i++) //renders the background in each tile location based on the size of the tiles desired
		{
			int x = i % xTile;
			int y = i / xTile;
			renderTexture(background, renderer, x * tileSize, y * tileSize, tileSize, tileSize);
		}
		renderTexture(image, renderer, x, y);


		if (starCurrent != starMax)
		{

			starSpawnX = rand() % width-100;
			starSpawnY = rand() % height-100;
			
			starCurrent = starCurrent + 1;
		}
		renderTexture(star, renderer, starSpawnX, starSpawnY);

		if (width < 1024)
		{
			if ((x >= starSpawnX && x <= starSpawnX + 128) && (y >= starSpawnY && y <= starSpawnY + 128))
			{
				starCurrent = 0;


				width = width + 50;
				height = height + 50;
				SDL_SetWindowSize(window, width, height);
				SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

			}
		}


		SDL_RenderPresent(renderer);



		if (height < 58)
		{
			SDL_RenderClear(renderer);

			SDL_SetWindowSize(window, 640, 480);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


			renderTexture(gameover, renderer, 0, 0);
			SDL_RenderPresent(renderer);

			SDL_Delay(3000);

			quit = true;
			
		}


	}
	while (quit == true)
	{


		cout << "Game Over." << endl;
		SDL_Delay(500);
		quit = false;
	}


	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); //closes window
	SDL_Quit();
	return 0;
}

