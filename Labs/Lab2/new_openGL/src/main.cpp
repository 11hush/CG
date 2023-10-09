// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

static GLfloat spin = 0.0;

void init(void);
void display(void);
void spinDisplay(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}

void init(void)
{
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
        glVertex2f(-0.9, 0.45);
        glVertex2f(0.9, 0.45);
        glVertex2f(0.9, 0.15);
        glVertex2f(-0.5, 0.15);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
        glVertex2f(-0.5, 0.15);
        glVertex2f(0.9, 0.15);
        glVertex2f(0.9, -0.15);
        glVertex2f(-0.5, -0.15);
        glVertex2f(-0.3, 0);
    glEnd();

    glColor3f(0, 0.47656, 0.23828);
    glBegin(GL_POLYGON);
        glVertex2f(-0.5, -0.15);
        glVertex2f(0.9, -0.15);
        glVertex2f(0.9, -0.45);
        glVertex2f(-0.9, -0.45);
    glEnd();

	glColor3f(0.77, 0.07, 0.11);
	glBegin(GL_TRIANGLES);
	    glVertex2f(-0.9, 0.45);
	    glVertex2f(-0.3, 0);
	    glVertex2f(-0.9, -0.45);
	glEnd();

    glutSwapBuffers();
}

void spinDisplay(void)
{
    spin += 2.0;
    if(spin > 360.0)
        spin -= 360.0;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN)
                glutIdleFunc(spinDisplay);
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN)
                glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}