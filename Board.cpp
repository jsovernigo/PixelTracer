
#include <SDL/SDL.h>
#include <stdio.h>
#include "PixelTracer.h"
/*
 *This is a program which creates an object to move around an SDL screen, changing pixels based
 *on their proximity to other pixels already present on the screen.  This program includes SDL headers,
 *Standard IO headers, and the PixelTracer.cpp class file, which contains methods to do these functions.
 *@author Jsovernigo
 */

#define sc_bpp 4
#define DEFAULT_WIDTH 200
#define DEFAULT_HEIGHT 200

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
			case(0):
				return new colour(0xff, 0x00, 0x00); // red
				break;
			case(1):
				return new colour(0xff, 0xa5, 0x00); // orange
				break;
			case(3):
				return new colour(0xff, 0xff, 0x00); // yellow
				break;
			case(4):
				return new colour(0x00, 0xff, 0x00); // green
				break;
			case(5):
				return new colour(0x39, 0x64, 0xc3); // blue
				break;
			case(6):
				return new colour(0x80, 0x00, 0x80); // violet
				break;
			case(7):
				return new colour(0xff, 0xff, 0xff);
				break;
			default :
				return new colour(0xff, 0xff, 0xff); // white
				break;
		}
	}
};


void setpixelcolour(SDL_Surface* screen, int x, int y, colour c)
{
	Uint32 colour;

	SDL_Rect* pixel;

	/* creates a colour to be mapped */
	colour = SDL_MapRGB(screen->format, c.r, c.g, c.b);

	pixel = new SDL_Rect();
	pixel->x = x;
	pixel->y = y;
	pixel->w = 1;
	pixel->h = 1;
	
	SDL_FillRect(screen, pixel, colour);
	SDL_UpdateRect(screen, x, y, 1, 1);
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

void recolour(SDL_Surface* screen, int sc_width, int sc_height)
{
	Uint32 black;

	black = SDL_MapRGB(screen->format, 0, 0, 0);
	SDL_FillRect(screen, NULL, black);

	setpixelcolour(screen, sc_width / 2, sc_height / 2, * colour::makecolour(5));
}

PixelTracer* initialize(int sc_width, int sc_height)
{
	PixelTracer* tracer = new PixelTracer(sc_width / 2 - 1, sc_height / 2 - 1, sc_width, sc_height);
	return tracer;
}

void check(PixelTracer* pt, SDL_Surface* screen, int sc_width, int sc_height)
{
	int x = pt->getX();
	int y = pt->getY();

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
void step(PixelTracer* tracer, SDL_Surface* screen, int sc_width, int sc_height)
{
	tracer->move();
	check(tracer, screen, sc_width, sc_height);
}


int main(int argc, char** argv)
{
	int quit;

	int sc_width;
	int sc_height;

	SDL_Surface* screen;

	PixelTracer* tracer;

	sc_width = DEFAULT_WIDTH;
	sc_height = DEFAULT_HEIGHT;

	if(argc > 2)
	{
		int temph;
		int tempw;

		tempw = atoi(argv[1]);
		temph = atoi(argv[2]);

		if(tempw > 0)
		{
			sc_width = tempw;
		}
		if(temph > 0)
		{
			sc_height = temph;
		}
	}

	quit = 0;

	/* inits the video mode for the screen */
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		puts("Could not open window.");
		exit(1);
	}

	/* gets a screen that we can write to. */
	screen = SDL_SetVideoMode(sc_width, sc_height, 0, SDL_HWPALETTE);
	if(screen == NULL)
	{
		puts("Error in creating screen");
		exit(1);
	}

	/* sets the screen to black. */
	recolour(screen, sc_width, sc_height);

	/* sets up the tracer. */
	tracer = initialize(sc_width, sc_height);

	while(!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.sym == SDLK_RETURN)
				{
					quit = true;
				}
			}
		}
	}


	quit = 0;
	/* loops until we want to quit. */
	while (!quit)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.sym == SDLK_q)
				{
					quit = true;
				}
			}
		}

		/* steps the tracer on the screen to a new position */
		step(tracer, screen, sc_width, sc_height);
	}

	SDL_Quit();

	return 0;
}
