#include <ddcPrint.h>
#include <ddcChars.h>
#include <ddcColors.h>
#include <math.h>

#define SBLOCK CS_BLOCK
#define BBLOCK CS_BLOCK

long SPY;
long SPX;

long height;
long width;

double r1 = 20;
double r2 = 20;
double m1 = 20;
double m2 = 10;
double a1 = 1.9;
double a2 = 1.9;
double a1v = 0;
double a2v = 0;
double g = 9.80;

void undraw_pendulum(int x1, int y1, int x2, int y2)
{
	draw_line(SPX+(x1*2), SPY+y1, SPX+(x2*2), SPY+y2, "  ");
	draw_rect(SPX+(x2*2)-1, SPY+y2-1, SPX+(x2*2)+1, SPY+y2, "  ");
}
void draw_pendulum(int x1, int y1, int x2, int y2)
{
	draw_line(SPX+(x1*2), SPY+y1, SPX+(x2*2), SPY+y2, CWHITE SBLOCK SBLOCK);
	draw_rect(SPX+(x2*2)-1, SPY+y2-1, SPX+(x2*2)+1, SPY+y2, CYELLOW BBLOCK BBLOCK);
}

double t = 0;
double st = 0.01;

double fun1(double x, double y, double z, double w)
{
	return (-g * (2 * m1 + m2) * sin(x) - m2 * g * sin(x - 2 * y) - 2 * sin(x - y) * m2 * (w * w) * r2 + (z * z) * r1 * cos(x - y)) /
			(r1 * (2 * m1 + m2 - m2 * cos(2 * x- 2 * y)));
}
double fun2(double x, double y, double z, double w)
{
	return (2 * sin(x - y) * ((z * z) * r1 * (m1 + m2) + g * (m1 + m2) * cos(x) + (w * w) * r2 * m2 * cos(x - y))) /
			(r2 * (2 * m1 + m2 - m2 * cos(2 * x - 2 * y)));
}

void update(void)
{

	
	double k1a1 = st * a1v;
	double k1a2 = st * a2v;
	double k1v1 = st * fun1(a1, a2, a1v, a2v);
	double k1v2 = st * fun2(a1, a2, a1v, a2v);

	double k2a1 = st * (a1v + (k1a1 / 2.0));
	double k2a2 = st * (a2v + (k1a2 / 2.0));
	double k2v1 = st * (fun1(a1+(k1a1/2.0), a2+(k1a2/2.0), a1v+(k1v1/2.0), a2v+(k1v2/2.0)));
	double k2v2 = st * (fun2(a1+(k1a1/2.0), a2+(k1a2/2.0), a1v+(k1v1/2.0), a2v+(k1v2/2.0)));

	double k3a1 = st * (a1v + (k2a1 / 2.0));
	double k3a2 = st * (a2v + (k2a2 / 2.0));
	double k3v1 = st * (fun1(a1+(k2a1/2.0), a2+(k2a2/2.0), a1v+(k2v1/2.0), a2v+(k2v2/2.0)));
	double k3v2 = st * (fun2(a1+(k2a1/2.0), a2+(k2a2/2.0), a1v+(k2v1/2.0), a2v+(k2v2/2.0)));

	double k4a1 = st * (a1v + k3a1);
	double k4a2 = st * (a2v + k3a2);
	double k4v1 = st * (fun1(a1+k3a1, a2+k3a2, a1v+k3v1, a2v+k3v2));
	double k4v2 = st * (fun2(a1+k3a1, a2+k3a2, a1v+k3v1, a2v+k3v2));

	a1 += (1.0/6.0) * (k1a1 + 2.0 * k2a1 + 2.0 * k3a1 + k4a1);
	a2 += (1.0/6.0) * (k1a2 + 2.0 * k2a2 + 2.0 * k3a2 + k4a2);
	a1v += (1.0/6.0) * (k1v1 + 2.0 * k2v1 + 2.0 * k3v1 + k4v1);
	a2v += (1.0/6.0) * (k1v2 + 2.0 * k2v2 + 2.0 * k3v2 + k4v2);

	a1v *= 0.9999;
	a2v *= 0.9999;

/*
	double T = ((m1 + m2)/2.0) * (r1*r1) * (a1v*a1v) + (m2 / 2.0) * (r2 * r2) * (a2v * a2v) + m2 * r1 * r2 * a1v * a2v * cos(a1 - a2);
	double U = -(m1 + m2) * r1 * g * cos(a1) - m2 * r2 * g * cos(a2);

	cursor_save();
	cursor_move_to(0, 0);
	ddPrintf("H: %f\n", T + U);
	cursor_restore();
*/


	double x1 = r1 * sin(a1);
	double y1 = r1 * cos(a1);

	double x2 = x1 + r2 * sin(a2);
	double y2 = y1 + r2 * cos(a2);

	draw_pendulum(x1, y1, x2, y2);
	draw_pendulum(0, 0, x1, y1);

	usleep(1000);

	undraw_pendulum(x1, y1, x2, y2);
	undraw_pendulum(0, 0, x1, y1);

	t += st;
}

int main(void)
{
	cursor_clear();
	cursor_home();

	width = cursor_get_width();
	height = cursor_get_height();

	r1 = (height/4)-2;
	r2 = r1;
	SPY = height/2;
	SPX = width/2;

	while (1)
	{
		update();
	}

	cursor_move_to(0, 40);

	return 0;
}
