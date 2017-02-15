#ifndef __PixelTracer_H_INCLUDED__   
#define __PixelTracer_H_INCLUDED__ 

class PixelTracer
{

public:
	int direction;
	int x;
	int y;

	const int max_x;
	const int max_y;

	PixelTracer(int x, int y, int mx, int my);

	void reset();
	void move();

	int getnewdirection();
	int getY();
	int getX();
	int returnCurrentColour();
};

#endif 
