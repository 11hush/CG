#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <math.h>
#include <vector>

class ColorRGB {
public:
    float r_;
    float g_;
    float b_;
    ColorRGB()=default;
    ColorRGB(float r, float g, float b) : r_(r), g_(g), b_(b) {};
};

class Pos3D {
public:
    double x_;
    double y_;
    double z_;
    Pos3D()=default;
    Pos3D(double x, double y, double z) : x_(x), y_(y), z_(z) {};
};

using Axis=Pos3D;

class CeleBody {
private:
    ColorRGB color_;
    Axis axis_;
    double orb_radius_;
    double radius_;
    bool clockwise_;
    double rev_period_;
    double rot_period_;
public:
    CeleBody()=default;
    CeleBody(   ColorRGB color, Axis axis, double orb_radius, double radius, bool isclockwise, 
                double rev_period, double rot_period)
        :   color_(color), axis_(axis), orb_radius_(orb_radius), radius_(radius), clockwise_(isclockwise), 
            rev_period_(rev_period), rot_period_(rot_period)
        {};
    ColorRGB GetColor() const { return color_; };
    Axis GetAxis() const { return axis_; };
    double GetOrbRadius() const { return orb_radius_; };
    double GetRadius() const { return radius_; };
    bool IsClockwise() const { return clockwise_; };
    double GetRevPeriod() const { return rev_period_; };
    double GetRotPeriod() const { return rot_period_; };
};

using Planet = CeleBody;
using Satellite = CeleBody;

