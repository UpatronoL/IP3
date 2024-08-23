/* window.c
 *
 * Last edited: 2022-04-10 17:19:05 by piumarta on debian
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <libgen.h>

#include "window.h"

#if 1 // change to 0 if the window is blank
#define RENDERER	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
#else
#define RENDERER	SDL_RENDERER_SOFTWARE
#endif

// create a Colour from hue angle (0..360), saturation (0..100) and value (0..100)

Colour HSV(double h, double s, double v)
{
    h  = fmod(h, 360.0);  if (h < 0) h += 360.0;
    s /= 100.0;      	  if (s < 0) s = 0; else if (s > 100) s = 100;
    v /= 100.0;      	  if (v < 0) v = 0; else if (v > 100) v = 100;

    double c= s * v;
    double x= c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m= v - c;
    double r, g, b;

    if      (  0 <= h && h <  60) r = c, g = x, b = 0;
    else if ( 60 <= h && h < 120) r = x, g = c, b = 0;
    else if (120 <= h && h < 180) r = 0, g = c, b = x;
    else if (180 <= h && h < 240) r = 0, g = x, b = c;
    else if (240 <= h && h < 300) r = x, g = 0, b = c;
    else                          r = c, g = 0, b = x;

    return RGB((r + m) * 255, (g + m) * 255, (b + m) * 255);
}

// default width and height of window; updated if size is changed

int width= 480, height= 640;

// events (keyboard, mouse, etc.) by default do nothing

static void _window_nullHandler() {}

struct window window = {
    0,
    0,
    0,
    GREY75,
    WHITE,
    BLACK,
    BLACK,
    0, 0,
    0,
    0,
    0,
    0,
    0,
    _window_nullHandler,
    _window_nullHandler,
    _window_nullHandler,
    _window_nullHandler,
    _window_nullHandler,
    _window_nullHandler,
};

// return the number of milliseconds elapsed since the program was started

unsigned long millisecondClock(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (unsigned long)tv.tv_sec * 1000 + (unsigned long)tv.tv_usec / 1000 - window.millisecondEpoch;
}

// change the drawing colour iff it is different from the current drawing colour
// return 1 if the colour is not transparent, or 0 if it is (drawing is pointless because it would be invisible)

int setRenderDrawColour(Colour colour)
{
    if (TRANSPARENT.bits == colour.bits) return 0;
    if (window.colourCurrent.bits != colour.bits) {
	SDL_SetRenderDrawColor(window.renderer, colour.sdl.r, colour.sdl.g, colour.sdl.b, colour.sdl.a);
	window.colourCurrent = colour;
    }
    return 1;
}

// create or resize the window

void size(int w, int h)
{
    if (window.window) { // resize the window
	SDL_SetWindowSize(window.window, w, h);
	width= w;
	height= h;
	SDL_DestroyRenderer(window.renderer);
	window.renderer= SDL_CreateRenderer(window.window, -1, RENDERER);
	if (0 == window.renderer) {
	    printf("could not create renderer: %s\n", SDL_GetError());
	    exit(1);
	}
	SDL_RenderSetLogicalSize(window.renderer, width, height);
	SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);
	window.changed= 1; // need update to redraw content
	return;
    }

    // create the window for the first time

    window.millisecondEpoch= millisecondClock();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not open window: %s\n", SDL_GetError());
        exit(1);
    }

    width= w;
    height= h;
    window.positionX= w/2;  // final position of last moveTo or drawLineTo
    window.positionY= h/2;

    window.window= SDL_CreateWindow("Window",
				  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				  width, height,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (0 == window.window) {
        fprintf(stderr, "could not open window: %s\n", SDL_GetError());
        exit(1);
    }

    window.renderer= SDL_CreateRenderer(window.window, -1, RENDERER);
    if (0 == window.renderer) {
	printf("could not create renderer: %s\n", SDL_GetError());
	exit(1);
    }
    SDL_RenderSetLogicalSize(window.renderer, w, h);
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window.renderer,
			   window.colourCurrent.sdl.r, window.colourCurrent.sdl.g,
			   window.colourCurrent.sdl.b, window.colourCurrent.sdl.a);

    if (TTF_Init() == -1) {
	printf("could not initialize TTF: %s\n", SDL_GetError());
	exit(1);
    }

    window.changed= 1; // need update to draw initial content
}

// set the text displayed in the window's title bar

void title(char *title)
{
    SDL_SetWindowTitle(window.window, title);
}

// change the background (canvas), fill (interior of solid shapes), or stroke (line) colour

void background(Colour colour)  { window.colourBackground = colour; }
void fill      (Colour colour)  { window.colourFill       = colour; }
void stroke    (Colour colour)  { window.colourStroke     = colour; }

void _drawPoint(int x, int y)
{
    SDL_RenderDrawPoint(window.renderer, x, y);
    window.changed= 1;
}

// draw a single point at (x, y) using the current stroke colour
// note: (0, 0) is the top left of the window, and Y increases downwards

void drawPoint(int x, int y)
{
    if (!setRenderDrawColour(window.colourStroke)) return;
    _drawPoint(x, y);
}

void _drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(window.renderer, x1, y1, x2, y2);
    window.changed= 1;
}

// draw a line from (x1, y1) to (x2, y2)
// note: (0, 0) is the top left of the window, and Y increases downwards

void drawLine(int x1, int y1, int x2, int y2)
{
    if (!setRenderDrawColour(window.colourStroke)) return;
    _drawLine(x1, y1, x2, y2);
}

// move the current drawing position to (x, y)
// note: (0, 0) is the top left of the window, and Y increases downwards

void moveTo(int x, int y)
{
    window.positionX= x;
    window.positionY= y;
}

// draw a line from the current position to (x, y) and update the current position
// note: (0, 0) is the top left of the window, and Y increases downwards

void drawLineTo(int x, int y)
{
    drawLine(window.positionX, window.positionY, x, y);
    moveTo(x, y);
}

void fillRectangle_(int left, int top, int right, int bottom, Colour colour)
{
    if (!setRenderDrawColour(colour)) return;
    SDL_Rect rect= { left, top, right-left, bottom-top };
    SDL_RenderFillRect(window.renderer, &rect);
    window.changed= 1;
}

// draw a solid rectangle with corners (left, top) and (right, bottom) using the current stroke colour
// note: (0, 0) is the top left of the window, and Y increases downwards

void fillRectangle(int left, int top, int right, int bottom)
{
    fillRectangle_(left, top, right, bottom, window.colourStroke);
}

// draw a box with corners (left, top) and (right, bottom) using the current stroke colour
// and fill the interior with the curren fill colour (if it is not TRANSPARENT)
// note: (0, 0) is the top left of the window, and Y increases downwards

void drawRectangle(int left, int top, int right, int bottom)
{
    fillRectangle_(left, top, right, bottom, window.colourFill);
    if (!setRenderDrawColour(window.colourStroke)) return;
    SDL_Rect rect= { left, top, right-left, bottom-top };
    SDL_RenderDrawRect(window.renderer, &rect);
    window.changed= 1;
}

void fillPolygon_(int nPoints, int *points, Colour colour)
{
    if (!setRenderDrawColour(colour)) return;
    if      (nPoints <= 0)  return;
    else if (nPoints == 1)  _drawPoint(points[0], points[1]);
    else if (nPoints == 2)  _drawLine(points[0], points[1], points[2], points[3]);
    else {
	int nxs, *xs= alloca(sizeof(int) * nPoints);
	int y, i, j, k;
	int y0= points[1], y1= y0;
	// find the vertical range of the polygon
	for (i= 1;  i < nPoints;  ++i) {
	    y= points[i*2+1];
	    if (y < y0) y0= y;
	    if (y > y1) y1= y;
	}
	// for each horizontal line in the vertical range, find the X coordinates of intersecting edges;
	// for each pair of non-overlapping X coordinates, fill the line between them
	for (y= y0;  y <= y1;  ++y) {
	    nxs= 0;
	    j= nPoints - 1;
	    for (i= 0;  i < nPoints;  j= i++)
		if ((points[i*2+1] < y && y <= points[j*2+1]) || (points[j*2+1] < y && y <= points[i*2+1])) {
		    xs[nxs++]= (int)rint(points[i*2+0] + ( ((double)y - points[i*2+1])
							   / ((double)points[j*2+1] - points[i*2+1])
							   * ((double)points[j*2+0] - points[i*2+0]) ));
		    // insertion sort inner loop to maintain X coordinates in ascending order
		    for (k= nxs - 1;  k && xs[k-1] > xs[k];  --k) {
			int tmp= xs[k-1];
			xs[k-1]= xs[k];
			xs[k]= tmp;
		    }
		}
	    for (i= 0;  i < nxs;  i += 2)
		_drawLine(xs[i], y, xs[i+1], y);
	}
    }
}

// draw a solid polygon using the current stroke colour.
// the polygon is specified as a list of points in the integer array points[nPoints*2] where
// even indexes are the X coordinates and the following odd index is the corresponding Y coordinate;
// in other words, each point is (points[i*2+0], points[i*2+1]) for i in 0 ... nPoints-1.
// the interior of the polygon is determined using the "even-odd rule": if the edges intersect then
// parts of the interior of the polygon will be hollow.
// note: (0, 0) is the top left of the window, and Y increases downwards

void fillPolygon(int nPoints, int *points)
{
    fillPolygon_(nPoints, points, window.colourStroke);
}

// draw a polygon using the current stroke colour and fill it with the current fill colour

void drawPolygon(int nPoints, int *points)
{
    fillPolygon_(nPoints, points, window.colourFill);
    if (!setRenderDrawColour(window.colourStroke)) return;
    if      (nPoints <= 0)  return;
    else if (nPoints == 1)  drawPoint(points[0], points[1]);
    else if (nPoints == 2)  drawLine(points[0], points[1], points[2], points[3]);
    else {
	moveTo(points[nPoints*2-2], points[nPoints*2-1]);
	for (int i= 0;  i < nPoints;  ++i)
	    drawLineTo(points[i*2+0], points[i*2+1]);
    }
}

// clear the entire window to the current background colour

void clear(void)
{
    if (!setRenderDrawColour(window.colourBackground)) return;
#if 0 // alpha not working?
    SDL_RenderClear(window.renderer);
    window.changed= 1;
#else
    SDL_Rect rect= { 0, 0, width, height };
    SDL_RenderFillRect(window.renderer, &rect);
    window.changed= 1;
#endif
}

// update the window contents if any visible drawing has occurred since the last update

void update(void)
{
    if (!window.changed) return;
    SDL_RenderPresent(window.renderer);
    window.changed= 0;
}

// set a handler 'callback function' for various kind of input events

void onKeyDown	(void (*handler)(int))		{ window.keyDownHandler   = handler; } // key pressed
void onKeyUp  	(void (*handler)(int))		{ window.keyUpHandler     = handler; } // key released
void onTextInput(void (*handler)(char *))	{ window.textInputHandler = handler; } // text entered
void onMouseMove(void (*handler)(int, int))	{ window.mouseMoveHandler = handler; } // mouse moved
void onMouseDown(void (*handler)(int, int))	{ window.mouseDownHandler = handler; } // mouse button pressed
void onMouseUp  (void (*handler)(int, int))	{ window.mouseUpHandler   = handler; } // mouse button released

static void _window_windowEvent(SDL_WindowEvent *e)
{
    switch (e->event) {
	case SDL_WINDOWEVENT_RESIZED:
	    size(e->data1, e->data2);
	    break;
    }
}

// pause the program for the given number of milliseconds
// input events are processed during the pause but do not interrupt the duration of the pause

void delay(int milliseconds)
{
    update();
    unsigned long mark= millisecondClock();
    for (;;) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
	    switch (e.type) {
		case SDL_QUIT:			exit(1);						break;
		case SDL_KEYDOWN:		window.keyDownHandler  (e.key.keysym.sym);		break;
		case SDL_KEYUP:			window.keyUpHandler    (e.key.keysym.sym);		break;
		case SDL_TEXTINPUT:		window.textInputHandler(e.text.text);			break;
		case SDL_MOUSEMOTION:		window.mouseMoveHandler(e.motion.x, e.motion.y);	break;
                case SDL_MOUSEBUTTONDOWN:	window.mouseDownHandler(e.motion.x, e.motion.y);	break;
                case SDL_MOUSEBUTTONUP:		window.mouseUpHandler(e.motion.x, e.motion.y);		break;
		case SDL_WINDOWEVENT:		_window_windowEvent(&e.window);				break;
            }
        }
	if ((millisecondClock() - mark) >= milliseconds) break;
	usleep(25); // 40 polls per second
    }
}

// set the font to be used when drawing text
// if path is nonzero then it is a string containing the path to a file containing a truetype font
// if path is zero then the file name "font.ttf" is used
// the font is scaled so that text is approximately size pixels high

void font(const char *path, int size)
{
    if (!path) path= "font.ttf";
    char *name= strdup(path);	// so that we can call font(fontName, newSize);
    if (window.currentFont) {
	TTF_CloseFont(window.currentFont);
	window.currentFont= 0;
	free(window.fontName);
	window.fontName= 0;
	window.fontSize= 0;
    }
    window.currentFont= TTF_OpenFont(name, size);
    if (0 == window.currentFont) {
	printf("could not load font: %s\n", SDL_GetError());
	exit(1);
    }
    window.fontSize= size;
    window.fontName= name;
}

// draw the given text at position (x, y) using the current stroke colour
// note: (0, 0) is the top left of the window, and Y increases downwards

void drawText(int x, int y, char *text)
{
    if (!window.currentFont) font(0, window.fontSize ? window.fontSize : 12);
    SDL_Surface *surface= 0;
    if (window.opaqueText) {
	surface= TTF_RenderText_Shaded(window.currentFont, text, window.colourStroke.sdl, window.colourFill.sdl);
    }
    else {
	surface= TTF_RenderText_Blended(window.currentFont, text, window.colourStroke.sdl);
    }
    SDL_Texture *texture= SDL_CreateTextureFromSurface(window.renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect= { x, y-window.fontSize, 0, 0 };
    SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(window.renderer, texture, 0, &rect);
    SDL_DestroyTexture(texture);
}
