/*#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GLUT/GLUT.h>*/

#include <math.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GL/glaux.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

char *tex_name[] = /// ������ � ����������
{
    "batman.bmp",
    "joker.bmp",
    "harly.bmp",
    "catwomen.bmp",
    "twoface.bmp",
    "pinguin.bmp",
    "ivy.bmp",
    "riddler.bmp",
};

GLfloat x_oct = 0;  ///  ��������� ���������� ���
GLfloat v_x_oct = 0.5; /// �������� ��������
GLfloat y_oct = 0;
GLfloat v_y_oct = 0;

GLfloat light_pos [] = {0, 0, 0, 1}; /// ������� ��������� �����
bool light_on = true;
GLfloat x_sfer = 0; /// ��������� �����
GLfloat v_x_sfer = 1; /// �������� �������� �����
int sfer_mode = 1;

int alpha_mode = 1; /// ����� ������������
float alpha = 1.0;

int mode = 1;

GLfloat opened = 0; /// ������� ����������� ���

GLint octahedron; /// ����������� ����������� ������

GLuint textures[8];

GLvoid LoadTex() /// �������� �������
{
    AUX_RGBImageRec *tex; /// ��������� �� ������������ ��������
    glGenTextures (8, &textures[0]); /// ���������� 8 ������� � ������

    for(int i=0; i<8; i++)
    {
        tex = auxDIBImageLoad(tex_name[i]);
        glBindTexture (GL_TEXTURE_2D, textures[i]); /// ����� ��������� ������
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, tex->sizeX, tex->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data); /// ���������� �������� �� �����
    }
}

void create_list() /// ������� ���������� ������
{
    float x=10, y=10, z=10, p=121; /// ���������� ������ ��������� �����������, p - ����� �2

    octahedron = glGenLists(1); /// id ����������� ������
    glNewList (octahedron, GL_COMPILE); /// ������ ���������� ����. ������ (���\�����)

    for (int r = 0; r < 8; r++)
    {
        if (r == 4)
        {
            glRotatef(180,0.0,0.0,1.0); /// ������� �� 180��
        }
        glRotatef(90,0.0,1.0,0.0); /// ������� �� 90��
        for (int i = 0; i < p; i+=2)
        {
            glNormal3f(1,1,1);
            glBegin(GL_POLYGON);

            glVertex3f (x/p*i, y-y/p*i, 0);
            glVertex3f (0, y-y/p*i, z/p*i);
            glVertex3f (0, y-y/p*(i+1), z/p*(i+1));
            glVertex3f (x/p*(i+1), y-y/p*(i+1),0);
            glEnd();
        }
    }

    glEndList(); /// ���� ���� ���������
}

void Draw ()
{
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /// ������� ������ (�����, �������)


    /// �������� ����� � ��������� �����
    glLoadIdentity (); /// �������� 1-�� �������
    glTranslatef (0, 0, -20); /// ���������� ����� ����� ������� ������ ����� (����� ���)
    glRotatef ((x_sfer), 0, 1, 0); /// �������� ��������� �����
    glTranslatef (0, 0, 25); /// ���������� ����� �� ����
    glLightfv (GL_LIGHT0, GL_POSITION, light_pos); ///���������� �����


    /// ��������� �����
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();
    glColor3d (255, 234, 0);
    gluQuadricDrawStyle (quadObj, GLU_LINE);
    gluSphere (quadObj, 1, 20, 20); /// ������� ��������� ����� (���, ������, ���� ����� � ������ Oz)

    glLoadIdentity();
    glTranslatef (0, 0, -20); /// ������� � ������
    glRotatef ((x_oct), 0, 1, 0);
    glRotatef ((y_oct), 1, 0, 0);


    ///  ��������� ��������
    if(mode == 1)
    {

        glBegin (GL_TRIANGLES);
        glColor4f (1.0, 0.0, 0.0, alpha);  /// ������� �����
        glNormal3d (-1, -1, 1);
        glVertex3f (0+opened, 10+opened, 0-opened);
        glVertex3f (0+opened, 0+opened, -10-opened);
        glVertex3f (10+opened, 0+opened, 0-opened);

        glColor4f (1.0, 0.5, 0.0, alpha);  /// ��������� �����
        glNormal3d (-1, -1, -1);
        glVertex3f (0+opened, 10+opened, 0+opened);
        glVertex3f (10+opened, 0+opened, 0+opened);
        glVertex3f (0+opened, 0+opened, 10+opened);

        glColor4f (1.0, 1.0, 0.0, alpha);  /// ������ �����
        glNormal3d (1, -1, -1);
        glVertex3f (0-opened, 10+opened, 0+opened);
        glVertex3f (0-opened, 0+opened, 10+opened);
        glVertex3f (-10-opened, 0+opened, 0+opened);

        glColor4f (0.0, 1.0, 0.0, alpha);  /// ������� �����
        glNormal3d (1, -1, -1);
        glVertex3f (0+opened, 0-opened, -10-opened);
        glVertex3f (10+opened, 0-opened, 0-opened);
        glVertex3f (0+opened, -10-opened, 0-opened);

        glColor4f (0.0, 1.0, 1.0, alpha);  /// ������� �����
        glNormal3d ( -1, 1, -1);
        glVertex3f (0+opened, 0-opened, 10+opened);
        glVertex3f (10+opened, 0-opened, 0+opened);
        glVertex3f (0+opened, -10-opened, 0+opened);

        glColor4f (0.0, 0.0, 0.5, alpha);  /// ����� �����
        glNormal3d (-1, -1, 1);
        glVertex3f (0-opened, 0-opened, 10+opened);
        glVertex3f (-10-opened, 0-opened, 0+opened);
        glVertex3f (0-opened, -10-opened, 0+opened);

        glColor4f (1.0, 0.0, 1.0, alpha);  /// ��������� �����
        glNormal3d (-1, 1, -1);
        glVertex3f (0-opened, 10+opened, 0-opened);
        glVertex3f (0-opened, 0+opened, -10-opened);
        glVertex3f (-10-opened, 0+opened, 0-opened);

        glColor4f (1.0, 0.0, 0.0, alpha);  /// ����������� �����
        glNormal3d (1, 1, 1);
        glVertex3f (0-opened, 0-opened, -10-opened);
        glColor4f (1.0, 1.0, 0.0, alpha);
        glVertex3f (-10-opened, 0-opened, 0-opened);
        glColor4f (0.0, 0.0, 0.5, alpha);
        glVertex3f (0-opened, -10-opened, 0-opened);

        glEnd ();
    }

    if(mode == 2) /// 8 �������
    {
        glEnable (GL_TEXTURE_2D); /// ��� ����� �������� �������
        glColor4f( 1.0, 1.0, 1.0, alpha); /// ���� � ���������� ������������ aplpha
        glBindTexture (GL_TEXTURE_2D, textures[0]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, -1, 1);
        glTexCoord2f (0, 0); /// ������������ ���������� �������� � �������� ������������
        glVertex3f (0+opened, 10+opened, 0-opened);
        glTexCoord2f (1, 0);
        glVertex3f (0+opened, 0+opened, -10-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (10+opened, 0+opened, 0-opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[1]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 10+opened, 0+opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0+opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, 0+opened, 10+opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[2]);
        glBegin (GL_TRIANGLES);
        glNormal3d (1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 10+opened, 0+opened);
        glTexCoord2f (1, 0);
        glVertex3f (0-opened, 0+opened, 10+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (-10-opened, 0+opened, 0+opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[3]);
        glBegin (GL_TRIANGLES);
        glNormal3d (1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 0-opened, -10-opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0-opened, 0-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, -10-opened, 0-opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[4]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, 1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 0-opened, 10+opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0-opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, -10-opened, 0+opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[5]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, -1, 1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 0-opened, 10+opened);
        glTexCoord2f (1, 0);
        glVertex3f (-10-opened, 0-opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0-opened, -10-opened, 0+opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[6]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, 1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 10+opened, 0-opened);
        glTexCoord2f (1, 0);
        glVertex3f (0-opened, 0+opened, -10-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (-10-opened, 0+opened, 0-opened);
        glEnd ();

        glBindTexture (GL_TEXTURE_2D, textures[7]);
        glBegin (GL_TRIANGLES);
        glNormal3d (1, 1, 1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 0-opened, -10-opened);
        glTexCoord2f (1, 0);
        glVertex3f (-10-opened, 0-opened, 0-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0-opened, -10-opened, 0-opened);
        glEnd ();
        glDisable (GL_TEXTURE_2D);
    }

    if(mode == 3) /// 1 ��������
    {
        glEnable (GL_TEXTURE_2D);
        glColor4f (1.0, 1.0, 1.0, alpha);
        glBindTexture (GL_TEXTURE_2D, textures[0]);
        glBegin (GL_TRIANGLES);
        glNormal3f (-1, -1, 1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 10+opened, 0-opened);
        glTexCoord2f (1, 0);
        glVertex3f (0+opened, 0+opened, -10-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (10+opened, 0+opened, 0-opened);

        glNormal3f (-1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 10+opened, 0+opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0+opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, 0+opened, 10+opened);

        glNormal3f (1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 10+opened, 0+opened);
        glTexCoord2f (1, 0);
        glVertex3f (0-opened, 0+opened, 10+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (-10-opened, 0+opened, 0+opened);

        glNormal3f (1, -1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 0-opened, -10-opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0-opened, 0-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, -10-opened, 0-opened);

        glNormal3f (-1, 1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0+opened, 0-opened, 10+opened);
        glTexCoord2f (1, 0);
        glVertex3f (10+opened, 0-opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0+opened, -10-opened, 0+opened);

        glNormal3f (-1, -1, 1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 0-opened, 10+opened);
        glTexCoord2f (1, 0);
        glVertex3f (-10-opened, 0-opened, 0+opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0-opened, -10-opened, 0+opened);

        glNormal3f (-1, 1, -1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 10+opened, 0-opened);
        glTexCoord2f (1, 0);
        glVertex3f (0-opened, 0+opened, -10-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (-10-opened, 0+opened, 0-opened);

        glNormal3f (1, 1, 1);
        glTexCoord2f (0, 0);
        glVertex3f (0-opened, 0-opened, -10-opened);
        glTexCoord2f (1, 0);
        glVertex3f (-10-opened, 0-opened, 0-opened);
        glTexCoord2f (0.5, 1);
        glVertex3f (0-opened, -10-opened, 0-opened);

        glEnd ();
        glDisable (GL_TEXTURE_2D);
    }

    if(mode == 4) /// ���������� ������
    {
        glColor4f(1.0,1.0,1.0,alpha);
        glCallList(octahedron);
    }

    glFlush ();
    ///glutSwapBuffers ();
}

void ChangeSize (GLsizei w, GLsizei h) /// ���������� ����������� ������/������ ����� ������ ������ ����
{
    if (h == 0)
        h = 1; ///��������� ������� �� 0

    glViewport (0, 0, (GLsizei)w, (GLsizei)h); ///���������� ����

    glMatrixMode (GL_PROJECTION); ///������� ��������
    glLoadIdentity ();
    gluPerspective (90, (GLdouble)w/h, 1, 150); /// �������� ������� ��������� (90���� �����, ����������� ������, ��� ���������, ���� ���������)
    glMatrixMode (GL_MODELVIEW); /// ������� �������
    glViewport (0, 0, (GLdouble)w, (GLdouble)h); /// ������ �� ���� ����

    Draw ();
}

void TimerFunction (int value) /// ������� �������� �� ������� (�������� � �����-�� ��� ���������� �������)
{
    glutPostRedisplay (); /// ��g�� ��� ����� �����������

    x_oct = x_oct + v_x_oct;
    y_oct = y_oct + v_y_oct; /// ������ 10 ���� ������ ���������� ��� � �����
    x_sfer = x_sfer + v_x_sfer;
    Draw ();
    glutTimerFunc (10, TimerFunction, 1); /// ������ 10 ������� ������� Timer �������, �������� �������
}

void glutNormalKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '1':
            mode = 1;
            break;
        case '2':
            mode = 2;
            break;
        case '3':
            mode = 3;
            break;
        case '4':
            mode = 4;
            break;
        case 'w':
            v_y_oct = -0.5;
            break;
        case 'a':
            v_x_oct = -0.5;
            break;
        case 's':
            v_y_oct = 0.5;
            break;
        case 'd':
            v_x_oct = 0.5;
            break;
        case 32:
            if (v_x_oct > 0 || v_y_oct > 0)
            {
                v_y_oct = 0;
                v_x_oct = 0;
            }
            else
            {
                v_x_oct = 0.5;
                v_y_oct = 0;
            }
            break;
        case 'e':
            opened += 0.75;
            break;
        case 'q':
            if (opened -0.75 >= 0)
                opened -= 0.75;
            break;
        case 'c':
            if (sfer_mode >= 0)
            {
                sfer_mode --; /// ��������� �����
                v_x_sfer --;
            }
            else
            {
                sfer_mode = 1;
                v_x_sfer = 1;
            }
            break;
        case 'r':   ///�������������
            if(alpha_mode)
            {
                alpha_mode = 0;
                alpha = 0.5;
                glDisable(GL_DEPTH_TEST);
            }
            else
            {
                alpha_mode = 1;
                alpha = 1.0;
                glEnable(GL_DEPTH_TEST);
            }
            break;
        case 'f':
            if (light_on)
                glDisable(GL_LIGHT0);
            else
                glEnable(GL_LIGHT0);
                light_on = !light_on;
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc,char** argv)
{
    std::cout << " press '1' for color mode\n press '2' for 8 texture mode\n press '3' for 1 texture mode\n press '4' for cut mode\n press 'w' for turn up\n press 'a' for turn left\n press 's' for turn down\n press 'd' for turn right\n press 'space' for stop oct\n press 'e' for open oct\n press 'q' for close oct\n press 'c' for stop sfer\n press 'f' for turn off light\n press 'r' for turn up prozrachnost\n press 'esc' for close app" << std::endl;
    glutInit(&argc, argv); /// ��������� �� ������� ��� �������� � OpenGL ��� ���������� ���� ����������
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /// ������������� ������
    glutInitWindowSize(1000, 1000); ///����� ����
    glutCreateWindow("Lab3"); /// �������� ����
    glutDisplayFunc(Draw); /// ������� ���������
    glutReshapeFunc(ChangeSize); /// ������� ��������� ��������
    glutTimerFunc(5, TimerFunction, 0.5); ///������� ������ �������, ����������� �������
    glutKeyboardFunc(glutNormalKeys); /// ������� ��������
    glEnable(GL_LIGHTING); /// ���������� ���� ���������
    glEnable(GL_LIGHT0); /// ��� ����������� �������� ����� 1/8
    glEnable(GL_COLOR_MATERIAL); /// ���������� ������� ���������
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); /// ������� �����
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); /// ��������� ������ � �������
    glClearColor(0,0,0,0); /// ���� ����
    glEnable(GL_DEPTH_TEST); /// ��������� �������
    glEnable(GL_BLEND); /// ����������� �������� ������������
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /// ����� ���������� ������������
    create_list(); /// ����������� ���� �������
    LoadTex(); /// �������� �������
    glutMainLoop(); ///������ ���� ��������� ��� ��� ������������
    glDisable(GL_DEPTH_TEST); /// ���������� �������� �������
    return 0;
}
