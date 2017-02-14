#include <stdlib.h>;
#include "PixelTracer.h"
/**
 *This file contains the class PixelTracer, an inheritable object which
 *provides methods for traversing an SDL window, in order to change pixels
 *on the screen based on their proximity to other coloured pixels in memory.
 *@author: Jsovernigo
 */
void PixelTracer::reset(){
	direction = getnewdirection();
	x = rand() % max_x;
	y = rand() % max_y;
}
int PixelTracer::getnewdirection(){
	return rand() % 4;
}
PixelTracer::PixelTracer(int x, int y, int mx, int my) :x(x), y(y), max_x(mx), max_y(my){
	direction = getnewdirection();
}
void PixelTracer::move(){
	int dir = direction;
	if (dir == 0)
		y--;
	else if (dir == 1)//right
		x++;
	
	else if (dir == 2)//down
		y++;
	else if (dir == 3)//left
		x--;
	if (x>max_x || x<0 || y>max_y || y<0){
		reset();
	}
}
int PixelTracer::getY(){
	return y;
}
int PixelTracer::getX(){
	return x;
}
int PixelTracer::returnCurrentColour() {
	reset();
	return rand() % 7;
}