#ifndef __window_h
#define __window_h

#include <stdint.h>
#include <SDL2/SDL.h>
#if defined(__APPLE__)
# include <SDL2_ttf/SDL_ttf.h>
#else
# include <SDL2/SDL_ttf.h>
#endif

typedef union Colour Colour;

union Colour {
    uint32_t   bits;
    SDL_Color  sdl;
};

struct window {
    int            changed;
    SDL_Window    *window;
    SDL_Renderer  *renderer;
    Colour         colourBackground;
    Colour         colourFill;
    Colour         colourStroke;
    Colour         colourCurrent;
    int		   positionX, positionY;
    char	  *fontName;
    int		   fontSize;
    TTF_Font	  *currentFont;
    int	           opaqueText;
    unsigned long  millisecondEpoch;
    void          (*keyDownHandler  )(int code);
    void          (*keyUpHandler    )(int code);
    void          (*textInputHandler)(char *text);
    void          (*mouseMoveHandler)(int x, int y);
    void          (*mouseDownHandler)(int x, int y);
    void          (*mouseUpHandler)  (int x, int y);
};

#define RGBA(R,G,B,A)	((Colour){ .sdl= { (R), (G), (B), (A) } })
#define RGB(R,G,B)	RGBA(R, G, B, 0xFF)
#define GREY(P)		RGB((P)*255/100, (P)*255/100, (P)*255/100)

#define BLACK		RGB (0x00, 0x00, 0x00)
#define BLUE		RGB (0x00, 0x00, 0xFF)
#define GREEN		RGB (0x00, 0xFF, 0x00)
#define CYAN		RGB (0x00, 0xFF, 0xFF)
#define RED		RGB (0xFF, 0x00, 0x00)
#define MAGENTA		RGB (0xFF, 0x00, 0xFF)
#define YELLOW		RGB (0xFF, 0xFF, 0x00)
#define WHITE		RGB (0xFF, 0xFF, 0xFF)
#define GREY75		RGB (0xC0, 0xC0, 0xC0)
#define GREY50		RGB (0x80, 0x80, 0x80)
#define GREY25		RGB (0x40, 0x40, 0x40)
#define TRANSPARENT	RGBA(0x00, 0x00, 0x00, 0x00)

Colour HSV(double h, double s, double v);

extern struct window window;

extern int width, height;

void size(int w, int h);
void title(char *);

void background(Colour colour);
void fill      (Colour colour);
void stroke    (Colour colour);

void clear(void);

void drawPoint(int x, int y);
void drawLine(int left, int top, int right, int bottom);
void drawRectangle(int left, int top, int right, int bottom);
void fillRectangle(int left, int top, int right, int bottom);
void drawPolygon(int nPoints, int *points);
void fillPolygon(int nPoints, int *points);

void moveTo(int x, int y);
void drawLineTo(int x, int y);

void font(const char *path, int size);
void drawText(int x, int y, char *text);

void update(void);

void onKeyDown	(void (*handler)(int));
void onKeyUp  	(void (*handler)(int));
void onTextInput(void (*handler)(char *));
void onMouseMove(void (*handler)(int, int));
void onMouseDown(void (*handler)(int, int));
void onMouseUp  (void (*handler)(int, int));

void nullHandler();

void delay(int milliseconds);

unsigned long millisecondClock(void);

#endif // __window_h
