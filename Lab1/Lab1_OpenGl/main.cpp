#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GLUT/glut.h>
#include <math.h>

#define PI 3.14159265

GLfloat sun_pos_x = 0;
GLfloat sun_pos_y = 0;
bool night = false;

void drawSun(int x, int y, int siz, int ap) {
    glBegin(GL_POLYGON);
    for(int i=0; i<ap; i++) {
        double ang = PI*2/ap*i;
        glVertex2d(siz * sin(ang) + x, siz * cos(ang) + y);
    }
    glEnd();
}

void Draw () {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1);
    /// отрисовка
    if (night)
        glColor3ub (128,128,0);
    else
        glColor3ub (190,245,116);
    glBegin (GL_QUADS);  /// трава
    glVertex2d (0, 0);
    glVertex2d (0, 400);
    glVertex2d (1000, 400);
    glVertex2d (1000, 0);
    glEnd ();
    
    if (night)
        glColor3ub (0,0,128);
    else
        glColor3ub (128, 166, 255);
    glBegin (GL_QUADS);  /// небо
    glVertex2d (0, 1000);
    glVertex2d (0, 400);
    glVertex2d (1000, 400);
    glVertex2d (1000, 1000);
    glEnd ();
    
    if (night)
        glColor3ub (110, 96, 42);
    else
        glColor3ub (161, 144, 76);
    glBegin (GL_QUADS);  /// дом
    glVertex2d (250, 250);
    glVertex2d (250, 450);
    glVertex2d (450, 450);
    glVertex2d (450, 250);
    glEnd ();
    
    
    if (night)
        glColor3ub (141, 54, 153);
    else
        glColor3ub (214, 77, 232);
    glBegin (GL_QUADS);  /// окно
    glVertex2d (350, 350);
    glVertex2d (350, 400);
    glVertex2d (400, 400);
    glVertex2d (400, 350);
    glEnd ();
    
    /// солнце/луна
    if (!night) {
        glColor3ub (242,229,85);
        drawSun(sun_pos_x, sun_pos_y, 55, 25);
    }
    else{
        glColor3ub (207,203,180);
        drawSun(sun_pos_x, sun_pos_y, 55, 25);
    }
    
    if (night)
        glColor3ub (145, 42, 42);
    else
        glColor3ub (209, 67, 67);
    glBegin (GL_TRIANGLES);  /// крыша
    glVertex2d (230, 450);
    glVertex2d (350, 550);
    glVertex2d (470, 450);
    glEnd ();
    
    
    glutSwapBuffers();
}

void ClearScreen () {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h) {
    if (h == 0)
        h = 1;
    
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    
    Draw ();
}

void TimerFunction (int value) {
    glutPostRedisplay();
    glutTimerFunc(10,TimerFunction,1);
    sun_pos_x-=2;
    if (sun_pos_x > 1055) {
        sun_pos_x = -55;
        if (night)
            night = false;
        else
            night = true;
    }
}

int main(int argc,char** argv) {
    sun_pos_x = 1055;
    sun_pos_y = 550;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Lab1");
    glutDisplayFunc(Draw);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(5, TimerFunction, 0.5);
    ClearScreen();
    glutMainLoop();
    return 0;
}
