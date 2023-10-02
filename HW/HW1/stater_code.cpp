#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#define PI 3.141592653589793
int WIDTH = 400;
int HEIGHT = 400;

void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w*h * 3, 1, fp);

	fclose(fp);
}

void oval(int x0, int y0, int a, int b, double theta, unsigned char data[]);
void rotate_n_draw(int x, int y, int x0, int y0, double cos_t, double sin_t, unsigned char data[]);
double rad(double angle);

int main() 
{
	int x, y, ra, rb;
	double theta; 	

	std::cout << "please input the semi-major axis a: ";
	std::cin >> ra;
	std::cout << "please input the semi-minor axis b: ";
	std::cin >> rb;
	std::cout << "please input the rotation angle theta: ";
	std::cin >> theta;

	theta = -rad(theta);
	WIDTH = std::max(WIDTH, 2 * std::max(ra, rb) + 200);
	HEIGHT = std::max(HEIGHT, 2 * std::max(ra, rb) + 200);

	x = WIDTH / 2;
	y = HEIGHT / 2;

	unsigned char data[HEIGHT*WIDTH*3];
	memset(data, 0, WIDTH * HEIGHT * 3);

	oval(x, y, ra, rb, theta, data);

	ppmWrite("test.ppm", data, WIDTH, HEIGHT);

	return 0;
}

void oval(int x0, int y0, int a, int b, double theta, unsigned char data[])
{
	int xi = 0, yi = b;
	std::cout << "(" << xi << ", " << yi << ")" << std::endl;
	double d = b * b + a * a * (-b + 0.25);
	double di = d;
	double cos_t = cos(theta), sin_t = sin(theta);
	std::cout << "cos:" << cos_t << ", sin:" << sin_t << std::endl;
	while(b * b * (xi + 1) < a * a * (yi - 0.5))
	{
		rotate_n_draw(xi, yi, x0, y0, cos_t, sin_t, data);

		if(di > 0)
		{
			di += b * b * (2 * xi + 3) + a * a * (2 - 2 * yi);
			yi--;
		}
		else
		{
			di += b * b * (2 * xi + 3);
		}
		xi++;
	}

	di = b * b * (xi + 0.5) * (xi + 0.5) + a * a * (yi - 1) * (yi - 1) - a * a * b * b;
	while(yi >= 0)
	{
		rotate_n_draw(xi, yi, x0, y0, cos_t, sin_t, data);

		if(di > 0)
		{
			di += a * a * (3 - 2 * yi);
		}
		else
		{
			di += a * a * (3 - 2 * yi) + b * b * (2 * xi + 2);
			xi++;
		}
		yi--;
	}
}

void rotate_n_draw(int x, int y, int x0, int y0, double cos_t, double sin_t, unsigned char data[])
{
	int x_pos = round(x * cos_t - y * sin_t);
	int y_pos = round(x * sin_t + y * cos_t);
	data[(y_pos + y0) * WIDTH * 3 + (x_pos + x0) * 3] = 255;
	x_pos = round(x * cos_t - (-y) * sin_t);
	y_pos = round(x * sin_t + (-y) * cos_t);
	data[(y_pos + y0) * WIDTH * 3 + (x_pos + x0) * 3] = 255;
	x_pos = round((-x) * cos_t - y * sin_t);
	y_pos = round((-x) * sin_t + y * cos_t);
	data[(y_pos + y0) * WIDTH * 3 + (x_pos + x0) * 3] = 255;
	x_pos = round((-x) * cos_t - (-y) * sin_t);
	y_pos = round((-x) * sin_t + (-y) * cos_t);
	data[(y_pos + y0) * WIDTH * 3 + (x_pos + x0) * 3] = 255;
}

double rad(double angle)
{
	return (angle / 180 - int(angle / 180)) * PI;
}