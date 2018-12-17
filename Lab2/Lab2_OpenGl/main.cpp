#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GLUT/glut.h>
#include <math.h>

#define PI 3.14159265

GLfloat sun_pos_x = 0;
GLfloat sun_pos_y = 0;
bool night = false;

static GLfloat vert[] = {
    0, 0,  /// трава
    0, 400,
    1000, 400,
    1000, 0,
    
    0, 1000,  ///  небо
    0, 400,
    1000, 400,
    1000, 1000,
    
    250, 250,  /// дом
    250, 450,
    450, 450,
    450, 250,
    
    230, 450,  ///  крыша
    350, 550,
    470, 450
};

static GLubyte colors_day [] = {
    190,245,116,  ///  трава
    190,245,116,
    190,245,116,
    190,245,116,
    
    128, 166, 255,  ///  небо
    128, 166, 255,
    128, 166, 255,
    128, 166, 255,
    
    161, 144, 76,  ///  дом
    161, 144, 76,
    161, 144, 76,
    161, 144, 76,
    
    209, 67, 67,  ///  крыша
    209, 67, 67,
    209, 67, 67
};

static GLubyte colors_night [] = {
    128,128,0,  /// трава
    128,128,0,
    128,128,0,
    128,128,0,
    
    0,0,128,   ///  небо
    0,0,128,
    0,0,128,
    0,0,128,
    
    110, 96, 42,  /// дом
    110, 96, 42,
    110, 96, 42,
    110, 96, 42,
    
    145, 42, 42,  ///  крыша
    145, 42, 42,
    145, 42, 42
};

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
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, (night)?colors_night:colors_day);
    
    /// отрисивка неба, травы, дома
    glDrawArrays(GL_QUADS, 0, 12);
    
    
    /// отрисовка солнца/луна
    if (!night) {
        glColor3ub (242,229,85);
        drawSun(sun_pos_x, sun_pos_y, 55, 25);
    }
    else{
        glColor3ub (207,203,180);
        drawSun(sun_pos_x, sun_pos_y, 55, 25);
    }
    
    /// отрисовка крыши
    glDrawArrays(GL_TRIANGLES, 12, 3);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
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
    if (sun_pos_x < -55) {
        sun_pos_x = 1055;
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
    glutCreateWindow("Lab2");
    glutDisplayFunc(Draw);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(5, TimerFunction, 0.5);
    ClearScreen();
    glutMainLoop();
    return 0;
}
