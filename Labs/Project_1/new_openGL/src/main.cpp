// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #define GL_SILENCE_DEPRECATION
// #include <stdio.h>
// #include <stdlib.h>
// #include <iostream>

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <GLUT/glut.h>
// #include <math.h>
#include "main.h"


#define Pi 3.1415926
/*星球的公转*/
GLfloat fEarth        = 2.0f;
GLfloat fMoon         = 24.0f;
GLfloat fSun          = 2.0f;
GLfloat fMerrcury     = 5.0f;
GLfloat fMars         = 6.0f;
GLfloat fMarsSate1    = 20.0f;
GLfloat fMarsSate2    = 0.0f;

/*星球的自传*/
GLfloat Day;
GLfloat Month;
GLfloat EarthYear;

/*Lookat函数参数*/
Pos3D eye(10, 10, 0);
Pos3D center(0, 0, 0);
Pos3D up_vec(0, 0, 1);
// GLdouble eyex, eyey, eyez = 12;
// GLdouble centerx, centery, centerz;
// GLdouble upx, upy = 1, upz;
GLdouble radius;

/*公转轴*/
Axis rev_axi(0.0, 1.0, 0.0);
Pos3D earth_pos_ini(1.0, 0.0, 0.0);
GLfloat rx = 0.0, ry = 1, rz = 0.1;

/* Color */
ColorRGB sun_color(0.90234375, 0.15234375, 0.02734375);
ColorRGB earth_color(0.38671875, 0.76953125, 0.9765625);
ColorRGB moon_color(0.9921875, 0.828125, 0.1875);

/*鼠标宏定义*/
bool isLeftMousePress;
float MouseX;
float MouseY;
float ViewX = 0;
float ViewY = 0;

void display(void);
void spinDisplay(void);
void reshape(int w, int h);
void DisplaySun();
void DisplayEarthandMoon();
void DisplayMars();
void DisplayPlanet( const Planet& planet, const std::vector<Satellite>& satellites);
void getkey(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void GetInputMouse(int button, int state, int x, int y) {

    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN)
            isLeftMousePress = true;
        else if (state == GLUT_UP)
            isLeftMousePress = false;
    }

    MouseX = x;
    MouseY = y;
}

void GetMotionMouse(int x, int y) 
{    
    if(isLeftMousePress) 
    {
        ViewX += (y - MouseY) * 0.02f;
        ViewY += (x - MouseX) * 0.02f;
        MouseX = x;
        MouseY = y;
    }

    float distance = sqrt(pow(eye.x_, 2) + pow(eye.y_, 2) + pow(eye.z_, 2));
    eye.z_ = distance * cos(ViewX) * cos(ViewY);
    eye.x_ = distance * cos(ViewX) * sin(ViewY);
    eye.y_ = distance * sin(ViewX);
    glutPostRedisplay();
}


void Idle() {
    //设置自转速度
    Day += 1.0 / 4;
    Day = Day >= 360 ? (Day - 360) : Day; 
    //设置公转速度
    fEarth = fEarth >= 360 ? (fEarth - 360) : (fEarth + 0.3f);
    fMoon  = fMoon  >= 360 ? (fMoon - 360) : (fMoon  + 2.4f);
    fMerrcury  = fMerrcury  >= 360 ? (fMerrcury - 360) : (fMerrcury + 0.4f);
    fMars  = fMars  >= 360 ? (fMars - 360) : (fMars  + 0.5f);
    fMarsSate1  = fMarsSate1  >= 360 ? (fMarsSate1 - 360) : (fMarsSate1  + 3.0f);
    fMarsSate2  = fMarsSate2  >= 360 ? (fMarsSate2 - 360) : (fMarsSate2  + 2.8f);
    //重新显示
    display();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(720, 480);
    glutCreateWindow(argv[0]);

    //设置正确投影的函数
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(&Idle);
    glutKeyboardFunc(getkey);
    glutMouseFunc(GetInputMouse);
    //处理鼠标移动
    glutMotionFunc(GetMotionMouse);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.8f);
    glutMainLoop();
    
    return 0;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x_, eye.y_, eye.z_, center.x_, center.y_, center.z_, up_vec.x_, up_vec.y_, up_vec.z_);
    
    Planet earth(earth_color, rev_axi, 6.0, 0.8, true, 360, 1);
    Satellite moon(moon_color, rev_axi, 1.5, 0.3, true, 30, 1);
    std::vector<Satellite> earth_sate = {moon};
    DisplaySun();
    DisplayPlanet(earth, earth_sate);
    // DisplayEarthandMoon();
    // DisplayMars();

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x_, eye.y_, eye.z_, center.x_, center.y_, center.z_, up_vec.x_, up_vec.y_, up_vec.z_);
}

void DisplaySun()
{
    glPushMatrix();
        glColor3f(sun_color.r_, sun_color.g_, sun_color.b_);
        glutSolidSphere(1.5f, 20, 20);
    glPopMatrix();
}

void DisplayEarthandMoon()
{
    // Earth
    glPushMatrix();
    {
        glColor3f(earth_color.r_, earth_color.g_, earth_color.b_);
        // 星轨
        glRotatef(90, rev_axi.y_, -rev_axi.x_, 0.0);
        glutSolidTorus(0.01, 6.0, 100, 100);
        glRotatef(-90, rev_axi.y_, -rev_axi.x_, 0.0);
        // 公转
        glRotatef(Day, rev_axi.x_, rev_axi.y_, rev_axi.z_);      
        glTranslatef(6.0f, 0.0f, 0.0f);
        // 自转
        glPushMatrix();
        {
            glRotatef(Day, rev_axi.x_, rev_axi.y_, rev_axi.z_);       //zizhuan
            glutSolidSphere(0.8f, 20, 20);
        }
        glPopMatrix();
    
        // Moon
        glColor3f(moon_color.r_, moon_color.g_, moon_color.b_);
        // 星轨
        glRotatef(90, rev_axi.y_, -rev_axi.x_, 0.0);
        glutSolidTorus(0.01, 1.5, 100, 100);
        glRotatef(-90, rev_axi.y_, -rev_axi.x_, 0.0);
        // 绕地球旋转
        glRotatef(Day / 30.0 * 360.0, rev_axi.x_, rev_axi.y_, rev_axi.z_);
        glTranslatef(1.5, 0.0, 0.0);

        glutSolidSphere(0.3f, 20, 20);
    }
    glPopMatrix();
}

void DisplayMars()
{
    glPushMatrix();
    {
        glColor3f(0.0, 1.0, 1.0);
        glRotatef(30, -1.0, 0.0, 0.0);
        glutSolidTorus(0.01, 6.8, 100, 100);
        glRotatef(-30, -1.0, 0.0, 0.0);

        glRotatef(Day / 90.0 * 360.0, 0.0, 1.0, 1.7);
        glTranslatef(6.8, 0.0, 0.0);
        glutSolidSphere(0.5f, 20, 20);
    }
    glPopMatrix();
}

void DisplayPlanet( const Planet& planet, const std::vector<Satellite>& satellites)
{
    glPushMatrix();
    
    ColorRGB color = planet.GetColor();
    Axis axis = planet.GetAxis();
    glColor3f(color.r_, color.g_, color.b_);

    // 星轨
    double angle = -90;
    if(axis.z_)
        angle = -atan(sqrtf(axis.x_ * axis.x_ + axis.y_ + axis.y_) / axis.z_);
    glRotatef(angle, -axis.y_, axis.x_, 0);
    glutSolidTorus(0.01, planet.GetOrbRadius(), 100, 100);
    glRotatef(-angle, -axis.y_, axis.x_, 0);

    // 行星
    glRotatef(Day / planet.GetRevPeriod() * 360.0, axis.x_, axis.y_, axis.z_);
    double length = sqrt(axis.x_ * axis.x_ + axis.y_ * axis.y_);
    glTranslatef(-axis.y_ / length * planet.GetOrbRadius(), axis.x_ / length * planet.GetOrbRadius(), 0);
    glutSolidSphere(planet.GetRadius(), 20, 20);

    // 卫星
    if(satellites.size())
    {        
        for(auto s : satellites)
        {
            glPushMatrix();
            DisplayPlanet(s, std::vector<Satellite>());
            glPopMatrix();
        }
    }

    glPopMatrix();
}

void getkey(unsigned char key, int x, int y)
{
    if(key == 'w')
        eye.z_ += 0.2;
    else if(key == 's')
        eye.z_ -= 0.2;
    else if(key == 'a')
    {
        double l = sqrt(eye.x_ * eye.x_ + eye.y_ * eye.y_);
        eye.x_ += (eye.y_ / l) * 0.2; 
        eye.y_ += (-eye.x_ / l) * 0.2;
    }
    else if(key == 'd')
    {
        double l = sqrt(eye.x_ * eye.x_ + eye.y_ * eye.y_);
        eye.x_ += (-eye.y_ / l) * 0.2; 
        eye.y_ += (eye.x_ / l) * 0.2;
    }
}