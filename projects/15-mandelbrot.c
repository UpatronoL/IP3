#include "window.h"

#define W 1024  // window width in screen coordinates
#define H  512  // window height in screen coordinates

int main(int argc, char **argv)
{
    double xmin = -2.5; // window origin in model coordinates
    double ymin = -1;   // window origin      "
    double w = 4;       // window width       "
    double h = 2;       // window height      "

    size(W, H);
    title("Mandelbrot");
    clear();
    update();    // make sure the window is on screen before clock starts
    delay(0);    // make sure the window is on screen before clock starts

    int maxiterations = 4000;

    double xmax = xmin + w;
    double ymax = ymin + h;

    // amount to increment x and y in model coords for each pixel on screen
    double dx = (xmax - xmin) / W;
    double dy = (ymax - ymin) / H;

    unsigned long start = millisecondClock();

    double y = ymin;
    for (int j = 0;  j < height;  j++) {
	double x = xmin;
	for (int i = 0;  i < width; i++) {
	    // as we iterate z = z^2 + c does z tend towards infinity?
	    double a = x;  // real part of z, initially real part of c
	    double b = y;  // imag part of z, initially imag part of c
	    int n = 0;
	    while (n < maxiterations) {
		double aa = a * a;
		double bb = b * b;
		double twoab = 2.0 * a * b;
		a = aa - bb + x;
		b = twoab + y;
		// |z| > 4 => z will diverge very rapidly
		if (aa + bb > 16.0) break; // i.e., sqrt(aa+bb) > 4
		n++;
	    }
	    // colour each pixel based on how soon it started to diverge.
	    // if z remained bounded, c is inside the set and so use black.
	    if (n == maxiterations) stroke(BLACK);
	    else stroke(HSV(200+n*12, 100, 100));
	    drawPoint(i, j);
	    x += dx;
	}
	if ((j & 3) == 3) update(); // update every 4096 pixels
	y += dy;
    }

    unsigned long stop = millisecondClock();
    printf("processing time: %g seconds\n", (stop-start) / 1000.0);

    delay(1000); // appreciate the pretty picture for a second

    return 0;
}
