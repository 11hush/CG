#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#define PI 3.141592653589793
int WIDTH = 400;
int HEIGHT = 400;

void ppmRead(char* filename, unsigned char* data, int* w, int* h) {
	char header[1024];
	FILE* fp = NULL;
	int line = 0;

	fp = fopen(filename, "rb");
	while (line < 2) {
		fgets(header, 1024, fp);
		if (header[0] != '#') {
			++line;
		}
	}
	sscanf(header, "%d %d\n", w, h);
	fgets(header, 20, fp);
	fread(data, (*w)*(*h) * 3, 1, fp);

	fclose(fp);
}
void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w*h * 3, 1, fp);

	fclose(fp);
}
void ellipse(int x0, int y0, int a, int b, double theta, unsigned char data[]);
double rad(double angle);
// test if the point is out of the window 
bool isOutofWindow(int x, int y, int xc, int yc);
// draw (x, y) and (-x, -y)
void draw(int x, int y, int xc, int yc, unsigned char* data);

int main() 
{
	int x, y, ra, rb;
	double theta; 	
	
	// Get ra, rb, x, y, and theta
	std::cout << "please input the semi-major axis a: ";
	std::cin >> ra;
	std::cout << "please input the semi-minor axis b: ";
	std::cin >> rb;
	std::cout << "please input the center" << std::endl;
	std::cout << "x: ";
	std::cin >> x;
	std::cout << "y: ";
	std::cin >> y;
	std::cout << "please input the rotation angle: ";
	std::cin >> theta;
	theta = rad(theta);

	// the input (x, y) takes the center of the window as origin
	// transform it to the viewport coordinate
	// [-WIDTH/2, WIDTH/2 - 1] * [-HEIGHT/2, HEIGHT/2 - 1] -> [0, WIDTH - 1] * [0, HEIGHT - 1]
	x += WIDTH / 2;
	y += HEIGHT / 2;

	// output buffer
	unsigned char data[HEIGHT*WIDTH*3];
	memset(data, 0, WIDTH * HEIGHT * 3);

	// draw the ellipse
	ellipse(x, y, ra, rb, theta, data);

	// output
	ppmWrite("result.ppm", data, WIDTH, HEIGHT);

	return 0;
}

void ellipse(int xc, int yc, int a, int b, double theta, unsigned char data[])
{
	// if a < b, set b to be the major axis
	if(a < b)	
	{
		std::swap(a, b);
		theta -= PI / 2;
	}
	// focal length
	double c = sqrt(a * a - b * b);
	// focus (xf, yf)
	double xf = c * cos(theta);
	double yf = c * sin(theta);

	// general formula of ellipse (suppose the center is at (0, 0))
	// Ax^2 + Bxy + Cy^2 + D = 0
	double A = a * a - xf * xf;
	double B = -2 * xf * yf;
	double C = a * a - yf * yf;
	double D = a * a * (yf * yf - A);

	// boundary point of area_8 and area_0
	double k1 = - B / (2 * C);
	double x1 = sqrt(-D / (A + B * k1 + C * k1 * k1));
	double y1 = k1 * x1;
	x1 = round(x1);	y1 = round(y1);

	// boundary point of area_0 and area_1
	double k2 = (2 * A - B) / (2 * C - B);
	double x2 = sqrt(- D / (A + B * k2 + C * k2 * k2));
	double y2 = k2 * x2;
	if(x1 * y2 - x2 * y1 < 0)
	{
		y2 = -y2;	x2 = -x2;
	}
	x2 = round(x2); y2 = round(y2);

	// boundary point of area_1 and area_2
	double k3 = -B / (2 * A);
	double y3 = sqrt(-D / (A * k3 * k3 + B * k3 + C));
	double x3 = k3 * y3;
	if(x1 * y3 - x3 * y1 < 0)	
	{
		x3 = -x3;	y3 = -y3;
	}
	x3 = round(x3);	y3 = round(y3);

	// boundary point of area_2 and area_3
	double k4 = -(2 * A + B) / (2 * C + B);
	double x4 = -sqrt(-D / (A + B * k4 + C * k4 * k4));
	double y4 = k4 * x4;
	if(x1 * y4 - x4 * y1 < 0)	
	{
		y4 = -y4;	x4 = -x4;
	}
	x4 = round(x4);	y4 = round(y4);

	// begin point
	int x_pos = x1, y_pos = y1;
	int x_mid = x_pos - 0.5, y_mid = y_pos + 1;

	double dpn = B * x_mid + 2 * C * y_mid + C;
	double dpnw = dpn - 2 * A * x_mid - B * y_mid + A - B;
	double dpn_n = 2 * C;
	double dpn_nw = 2 * C - B;
	double dpnw_n = dpn_nw;
	double dpnw_nw = 2 * (A - B + C);

	// region 0
	double p = A * x_mid * x_mid + B * x_mid * y_mid + C * y_mid * y_mid + D;
	while(y_pos < y2)
	{
		draw(x_pos, y_pos, xc, yc, data);
		y_pos++;
		if(p < 0)
		{
			p += dpn;
			dpn += dpn_n;	dpnw += dpnw_n;
		}
		else
		{
			x_pos -= 1;
			p += dpnw;
			dpn += dpn_nw;	dpnw += dpnw_nw;
		}
	}

	double dpw = dpnw - dpn + A + B + B / 2;
	dpnw += A - C;
	double dpw_w = 2 * A;
	double dpw_nw = 2 * A - B;
	double dpnw_w = dpw_nw;
	p += (dpw - dpn + C) / 2 + (A + C) / 4 - A;
	double cross2 = A - B + C;
	// region 1
	while(x_pos > x3)
	{
		draw(x_pos, y_pos, xc, yc, data);
		x_pos--;
		if(p < 0)
		{
			y_pos += 1;
			p += dpnw;
			dpw += dpw_nw;	dpnw += dpnw_nw;
		}
		else
		{
			p += dpw;
			dpw += dpw_w;	dpnw += dpnw_w;
		}
	}
	
	p += dpw - dpnw + 2 * C - B;
	dpw += B;
	double dpsw = dpw - dpnw + dpw + 4 * C - B;
	double dpsw_w = 2 * A + B;
	double dpsw_sw = 2 * (A + B + C);
	double dpw_sw = dpsw_w;
	double cross3 = A + B + C;
	// region 2
	while(x_pos > x4)
	{
		draw(x_pos, y_pos, xc, yc, data);
		x_pos -= 1;
		if(p < 0)
		{
			p += dpw;
			dpw += dpw_w;	dpsw += dpsw_w;
		}
		else
		{
			y_pos -= 1;
			p += dpsw;
			dpw += dpw_sw;	dpsw += dpsw_sw;
		}
	}

	double dps = dpsw - dpw - B;
	p += -dpsw / 2 + dps + A - (A + C - B) / 4;
	dpsw += C - A;
	dps += C - B / 2;
	double dps_s = 2 * C;
	double dps_sw = 2 * C + B;
	double dpsw_s = dps_sw;
	double cross4 = A + B;
	// region 3
	while(y_pos > -y1)
	{
		draw(x_pos, y_pos, xc, yc, data);
		y_pos -= 1;
		if(p < 0)
		{
			x_pos -= 1;
			p += dpsw;	
			dpsw += dpsw_sw;	dps += dps_sw;
		}
		else
		{
			p += dps;
			dpsw += dpsw_s;		dps += dps_s;
		}
	}
}

void draw(int x, int y, int xc, int yc, unsigned char* data)
{
	if(!isOutofWindow(x, y, xc, yc))
		data[(HEIGHT - (y + yc)) * WIDTH * 3 + (x + xc) * 3] = 255;
	if(!isOutofWindow(-x, -y, xc, yc))
		data[(HEIGHT - (-y + yc)) * WIDTH * 3 + (-x + xc) * 3] = 255;
}

bool isOutofWindow(int x, int y, int xc, int yc)
{
	return (y + yc < 0) || (y + yc >= HEIGHT) || (x + xc < 0) || (x + xc >= WIDTH);
}

double rad(double angle)
{
	return (1.0 * angle / 180 - int(angle / 180)) * PI;
}