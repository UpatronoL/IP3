#include "window.h"

#define NX 16 // number of horizontal tiles
#define NY  8 // vertical

#define W 1024 // window width
#define H  512 // window height

Colour framebuffer[H][W]; // shared framebuffer written by worker threads

struct Work // one item of work for a worker thread to complete
{
    double 	xmin, ymin;  // lower left corner of the space to plot
    double 	xmax, ymax;  // opposite corner of the space to plot
    int    	imin, imax;  // lower left screen coordinate of output tile
    int    	jmin, jmax;  // opposite corner of the output tile
};

const int maxiterations =  4000; // iterations after which we assume convergence
const double infinity   =  16.0; // magnitude(c) > 4.0 is guaranteed to diverge

void doWork(struct Work *w)
{
    double dx = (w->xmax - w->xmin) / (w->imax - w->imin);
    double dy = (w->ymax - w->ymin) / (w->jmax - w->jmin);
    double y = w->ymin;      // world Y (imag)
    for (int j = w->jmin;  j < w->jmax;  ++j) {     // screen Y
	double x = w->xmin;  // world X (real)
	for (int i = w->imin;  i < w->imax;  ++i) { // screen X
	    // iterate z = z^2 + (x + yi) starting from z = 0+0i
	    double a = x;
	    double b = y;
	    int n = 0;
	    while (n < maxiterations) {
		double aa = a * a;
		double bb = b * b;
		double twoab = 2.0 * a * b;
		a = aa - bb + x;
		b = twoab + y;
		if (aa + bb > infinity) break;  // diverged
		n++;
	    }
	    if   (n == maxiterations)      // bounded
		framebuffer[j][i] = BLACK;
	    else                           // diverging
		framebuffer[j][i] = HSV(200+n*12, 100.0, 100.0);
	    x += dx;
	}
	y += dy;
    }
}

struct Work *Work_new(double xmin, double ymin, double xmax, double ymax,
		      int imin, int jmin, int imax, int jmax)
{
    struct Work *w = calloc(1, sizeof(struct Work));
    w->xmin = xmin;
    w->ymin = ymin;
    w->xmax = xmax;
    w->ymax = ymax;
    w->imin = imin;
    w->jmin = jmin;
    w->imax = imax;
    w->jmax = jmax;
    return w;
}

void displayWork(struct Work *w)
{
    Colour c = BLACK;
    stroke(c);
    for (int j = w->jmin;  j < w->jmax;  ++j) {
	for (int i = w->imin;  i < w->imax;  ++i) {
	    if (c.bits != framebuffer[j][i].bits) {
		c = framebuffer[j][i];
		stroke(c);
	    }
	    drawPoint(i, j);
	}
    }
}

int main(int argc, char **argv)
{
    int doUpdates  = 1;  // non-zero if we should draw completed tiles one at a time
    int numThreads = 1;  // how many worker threads to create
    if (argc > 1) {
	numThreads = atoi(argv[1]);
	if (numThreads < 0) {  // negative thread count means no incremental updates
	    numThreads = -numThreads;
	    doUpdates = 0;
	}
	if (numThreads < 1 || numThreads > 10240) {
	    fprintf(stderr, "illegal number of threads: %i\n", numThreads);
	    exit(1);
	}
    }

    size(W, H);
    title("Mandelbrot");
    clear();
    delay(0);

    unsigned long start = millisecondClock();

    // a threaded implementation would start numThreads worker threads
    // here, all initially blocked on the work queue semaphore waiting
    // for work to arrive

    for (int x = 0;  x < NX;  ++x) {
	for (int y = 0;  y < NY;  ++y) {
	    double w = 4,           h = 2;
	    double xmin = -w*5/8,     ymin = -h/2;
	    double dx = w / NX,     dy = h / NY;
	    int    di = width / NX, dj = height / NY;

	    struct Work *work = Work_new(xmin + dx * (x + 0), ymin + dy * (y + 0),
					 xmin + dx * (x + 1), ymin + dy * (y + 1),
					 di * (x + 0),        dj * (y + 0),
					 di * (x + 1),        dj * (y + 1));

	    // complete the work immediately (a threaded implementation would
	    // generate and add all the work to the work queue)
	    doWork(work);

	    // display the result immediately (a threaded implementation would
	    // remove completed work from the results queue and then display it)
	    displayWork(work);

	    if (doUpdates) update();
	}
    }

    unsigned long stop = millisecondClock();

    printf("%i threads, processing time: %g seconds\n", numThreads, (stop-start) / 1000.0);

    delay(2000);

    return 0;
}
