#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <vector>

void init();
void display();
void reshape(int w, int h);
void getkey(unsigned char key, int x, int y);

GLfloat Flag[4][2] = {  110.0 / 110,    80.0 / 110, 
                        -110.0 / 110,   80.0 / 110, 
                        -110.0 / 110,   -80.0 / 110, 
                        110.0 / 110,    -80.0 / 110    };
GLfloat Stripe1[4][2] = {   110.0 / 110,    40.0 / 110, 
                            110.0 / 110,    65.0 / 110, 
                            -110.0 / 110,   65.0 / 110, 
                            -110.0 / 110,   40.0 / 110    };
GLfloat Stripe2[4][2] = {   26.82 / 110,    (5.5 - 17.32) / 110,
                            -26.82 / 110,   (5.5 - 17.32) / 110,
                            -30.0/ 110,     -17.32 / 110,
                            30.0 / 110,     -17.32 / 110     };      


GLfloat White[3] = {1.0f, 1.0f, 1.0f};
GLfloat Blue[3] = {0.0f / 255, 56.0f / 255, 184.0f / 255};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(getkey);
    glutMainLoop();

    return 0;
}

void init()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)std::max(w, h), (GLsizei)std::max(w, h));
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3fv(White);
    glBegin(GL_POLYGON);
        for(int i = 0; i < 4; i++)
            glVertex2fv(Flag[i]);
    glEnd();

    glColor3fv(Blue);
    glBegin(GL_POLYGON);
        for(int i = 0; i < 4; i++)
            glVertex2fv(Stripe1[i]);
    glEnd();

    glLoadIdentity();
    glTranslatef(0, -105.0 / 110, 0);
    glBegin(GL_POLYGON);
        for(int i = 0; i < 4; i++)
            glVertex2fv(Stripe1[i]);
    glEnd();

    glLoadIdentity();
    for(int i = 0; i < 6; i++)
    {
        glBegin(GL_POLYGON);
            for(int i = 0; i < 4; i++)
                glVertex3fv(Stripe2[i]);
        glEnd();
        glRotatef(60.0f, 0, 0, 1);
    }

    glFlush();
    glutSwapBuffers();
}

void getkey(unsigned char key, int x, int y)
{
    if(key == 27)
        exit(0);
}