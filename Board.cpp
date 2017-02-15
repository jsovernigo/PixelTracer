
#include <SDL.h>
#include <stdio.h>
#include "PixelTracer.h"
/*
 *This is a program which creates an object to move around an SDL screen, changing pixels based
 *on their proximity to other pixels already present on the screen.  This program includes SDL headers,
 *Standard IO headers, and the PixelTracer.cpp class file, which contains methods to do these functions.
 *@author Jsovernigo
 */
struct colour
{
	unsigned int r;
	unsigned int g;
	unsigned int b;

	/**
	 *
	 */
	colour(unsigned int r, unsigned int g, unsigned int b) : r(r), g(g), b(b)
	{
	
	}

	/**
	 *
	 */
	colour()
	{
		r = 0xff;
		g = 0xff;
		b = 0xff;
	}

	/**
	 *
	 */
	static colour* getblack()
	{
		return new colour(0x00, 0x00, 0x00);
	}

	/**
	 *
	 */
	static colour* makecolour(int which)
	{
		switch (which)
		{
			case(0) :
				return new colour(0xff, 0x00, 0x00); // red
				break;
			case(1) :
				return new colour(0xff, 0xa5, 0x00); // orange
				break;
			case(3) :
				return new colour(0xff, 0xff, 0x00); // yellow
				break;
			case(4) :
				return new colour(0x00, 0xff, 0x00); // green
				break;
			case(5) :
				return new colour(0x39, 0x64, 0xc3); // blue
				break;
			case(6) :
				return new colour(0x80, 0x00, 0x80); // violet
				break;
			default :
				return new colour(0xff, 0xff, 0xff); // white
				break;
		}
	}
};


const int sc_width = 200;
const int sc_height = 200;
const int sc_bpp = 4;


void setpixelcolour(SDL_Surface* screen, int x, int y,colour c)
{
	int ybpp;
	Uint32 colour;
	Uint32* pixel;

	ybpp = y*screen->pitch / sc_bpp;

	/* creates a colour to be mapped */
	colour = SDL_MapRGB(screen->format, c.r, c.g, c.b);

	pixel = (Uint32*)screen->pixels + ybpp + x;

	*pixel = colour;
}


bool isblack(SDL_Surface* screen, int x, int y)
{
	Uint32* pixel;
	int ybpp;

	ybpp = y*screen->pitch / sc_bpp;
	pixel = (Uint32*)screen->pixels + ybpp + x;

	/* checks if the pixel at x, y is html black */
	if (*pixel == SDL_MapRGB(screen->format, 0x00, 0x00, 0x00))
	{
		return true;
	}

	return false;
}

void recolour(SDL_Surface *screen, SDL_Window* window)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	setpixelcolour(screen, sc_width / 2, sc_height / 2, * colour::makecolour(8));
	SDL_UpdateWindowSurface(window);
}

PixelTracer* initialize()
{
	PixelTracer* tracer = new PixelTracer(sc_width / 2 - 1, sc_height / 2 - 1, sc_width, sc_height);
	return tracer;
}

void check(PixelTracer* pt, SDL_Surface* screen)
{
	int x = pt->getX();
	int y = pt->getY();
	int screeny = y*screen->pitch / sc_bpp;

	bool canleft = (x > 0);
	bool canright = (x < sc_width);
	bool canup = (y > 0);
	bool candown = (y < sc_height);

	if (canleft)
	{
		if (!isblack(screen, (x - 1), y))
		{
			setpixelcolour(screen, x, y, *colour::makecolour(pt->returnCurrentColour()));
			return;
		}
	}
	if (canright)
	{
		if (!isblack(screen, (x + 1), y))
		{
			setpixelcolour(screen, x, y, *colour::makecolour(pt->returnCurrentColour()));
			return;
		}
	}
	if (canup)
	{
		if (!isblack(screen, x, (y - 1)))
		{
			setpixelcolour(screen, x, y, *colour::makecolour(pt->returnCurrentColour()));
			return;
		}
	}
	if (candown)
	{
		if (!isblack(screen, x, (y + 1)))
		{
			setpixelcolour(screen, x, y, *colour::makecolour(pt->returnCurrentColour()));
			return;
		}
	}
	return;
}

/**
 *
 */
void step(PixelTracer* tracer, SDL_Surface* screen)
{
	tracer->move();
	check(tracer, screen);
}


int main(int argc, char* args[]) 
{
	SDL_Window* window = NULL;
	SDL_Surface* screen = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Pixel Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sc_width, sc_height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	screen = SDL_GetWindowSurface(window);
	recolour(screen, window);
	PixelTracer* tracer = initialize();
	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}
		step(tracer, screen);
		SDL_UpdateWindowSurface(window);
	}	
	return 0;
}
