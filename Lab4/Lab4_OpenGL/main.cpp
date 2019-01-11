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

char *tex_name[] = /// массив с текстурами
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

GLfloat x_oct = 0;  ///  начальная координата окт
GLfloat v_x_oct = 0.5; /// скорость вращения
GLfloat y_oct = 0;
GLfloat v_y_oct = 0;

GLfloat light_pos [] = {0, 0, 0, 1}; /// позиция источника света
bool light_on = true;
GLfloat x_sfer = 0; /// кордината света
GLfloat v_x_sfer = 1; /// скорость поворота света
int sfer_mode = 1;

int alpha_mode = 1; /// режим прозрачности
float alpha = 1.0;

int mode = 1;

GLfloat opened = 0; /// граница раздвижения окт

GLint octahedron; /// индификатор дисплейного списка

GLuint textures[8];

GLvoid LoadTex() /// загрузка текстур
{
    AUX_RGBImageRec *tex; /// указатель на подгруженную текстуру
    glGenTextures (8, &textures[0]); /// генерируем 8 текстур в массив

    for(int i=0; i<8; i++)
    {
        tex = auxDIBImageLoad(tex_name[i]);
        glBindTexture (GL_TEXTURE_2D, textures[i]); /// выбор активного холста
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, tex->sizeX, tex->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data); /// натягиваем текстуру на холст
    }
}

void create_list() /// создаем дисплейный список
{
    float x=10, y=10, z=10, p=121; /// координаты вершин основного теугольника, p - куски х2

    octahedron = glGenLists(1); /// id дисплейного списка
    glNewList (octahedron, GL_COMPILE); /// начало заполнения дисп. списка (что\когда)

    for (int r = 0; r < 8; r++)
    {
        if (r == 4)
        {
            glRotatef(180,0.0,0.0,1.0); /// поворот на 180гр
        }
        glRotatef(90,0.0,1.0,0.0); /// поворот на 90гр
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

    glEndList(); /// дисп лсит заполенен
}

void Draw ()
{
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /// очистка буфера (цвета, глубина)


    /// Движение сферы и источника света
    glLoadIdentity (); /// загрузка 1-ой матрицы
    glTranslatef (0, 0, -20); /// откидываем точку вокуг которой гоняет сфера (центр окт)
    glRotatef ((x_sfer), 0, 1, 0); /// вращение источника света
    glTranslatef (0, 0, 25); /// откидываем сферу от себя
    glLightfv (GL_LIGHT0, GL_POSITION, light_pos); ///координаты света


    /// Отрисовка сферы
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();
    glColor3d (255, 234, 0);
    gluQuadricDrawStyle (quadObj, GLU_LINE);
    gluSphere (quadObj, 1, 20, 20); /// функция отрисовки сфера (наз, радиус, узлы вдоль и вокруг Oz)

    glLoadIdentity();
    glTranslatef (0, 0, -20); /// перенос в гулубь
    glRotatef ((x_oct), 0, 1, 0);
    glRotatef ((y_oct), 1, 0, 0);


    ///  Отрисовка Октаидра
    if(mode == 1)
    {

        glBegin (GL_TRIANGLES);
        glColor4f (1.0, 0.0, 0.0, alpha);  /// красная грань
        glNormal3d (-1, -1, 1);
        glVertex3f (0+opened, 10+opened, 0-opened);
        glVertex3f (0+opened, 0+opened, -10-opened);
        glVertex3f (10+opened, 0+opened, 0-opened);

        glColor4f (1.0, 0.5, 0.0, alpha);  /// оранжевый грань
        glNormal3d (-1, -1, -1);
        glVertex3f (0+opened, 10+opened, 0+opened);
        glVertex3f (10+opened, 0+opened, 0+opened);
        glVertex3f (0+opened, 0+opened, 10+opened);

        glColor4f (1.0, 1.0, 0.0, alpha);  /// желтая грань
        glNormal3d (1, -1, -1);
        glVertex3f (0-opened, 10+opened, 0+opened);
        glVertex3f (0-opened, 0+opened, 10+opened);
        glVertex3f (-10-opened, 0+opened, 0+opened);

        glColor4f (0.0, 1.0, 0.0, alpha);  /// зеленая грань
        glNormal3d (1, -1, -1);
        glVertex3f (0+opened, 0-opened, -10-opened);
        glVertex3f (10+opened, 0-opened, 0-opened);
        glVertex3f (0+opened, -10-opened, 0-opened);

        glColor4f (0.0, 1.0, 1.0, alpha);  /// голубая грань
        glNormal3d ( -1, 1, -1);
        glVertex3f (0+opened, 0-opened, 10+opened);
        glVertex3f (10+opened, 0-opened, 0+opened);
        glVertex3f (0+opened, -10-opened, 0+opened);

        glColor4f (0.0, 0.0, 0.5, alpha);  /// синяя грань
        glNormal3d (-1, -1, 1);
        glVertex3f (0-opened, 0-opened, 10+opened);
        glVertex3f (-10-opened, 0-opened, 0+opened);
        glVertex3f (0-opened, -10-opened, 0+opened);

        glColor4f (1.0, 0.0, 1.0, alpha);  /// фелетовая грань
        glNormal3d (-1, 1, -1);
        glVertex3f (0-opened, 10+opened, 0-opened);
        glVertex3f (0-opened, 0+opened, -10-opened);
        glVertex3f (-10-opened, 0+opened, 0-opened);

        glColor4f (1.0, 0.0, 0.0, alpha);  /// градинетная грань
        glNormal3d (1, 1, 1);
        glVertex3f (0-opened, 0-opened, -10-opened);
        glColor4f (1.0, 1.0, 0.0, alpha);
        glVertex3f (-10-opened, 0-opened, 0-opened);
        glColor4f (0.0, 0.0, 0.5, alpha);
        glVertex3f (0-opened, -10-opened, 0-opened);

        glEnd ();
    }

    if(mode == 2) /// 8 текстур
    {
        glEnable (GL_TEXTURE_2D); /// вкл режим обрабоки текстур
        glColor4f( 1.0, 1.0, 1.0, alpha); /// цвет с параметром прозрачности aplpha
        glBindTexture (GL_TEXTURE_2D, textures[0]);
        glBegin (GL_TRIANGLES);
        glNormal3d (-1, -1, 1);
        glTexCoord2f (0, 0); /// сопоставляем координату текстуры с вершиной треугольника
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

    if(mode == 3) /// 1 текстура
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

    if(mode == 4) /// дисплейный список
    {
        glColor4f(1.0,1.0,1.0,alpha);
        glCallList(octahedron);
    }

    glFlush ();
    ///glutSwapBuffers ();
}

void ChangeSize (GLsizei w, GLsizei h) /// измаенение параментров ширины/высоты когда меняем размер окна
{
    if (h == 0)
        h = 1; ///избежание деления на 0

    glViewport (0, 0, (GLsizei)w, (GLsizei)h); ///координаты окна

    glMatrixMode (GL_PROJECTION); ///матрица проекций
    glLoadIdentity ();
    gluPerspective (90, (GLdouble)w/h, 1, 150); /// создание облости видимости (90град обзор, соотношение сторон, мин дальность, макс дальность)
    glMatrixMode (GL_MODELVIEW); /// видовая матрица
    glViewport (0, 0, (GLdouble)w, (GLdouble)h); /// рисуем во всем окне

    Draw ();
}

void TimerFunction (int value) /// функция анимации по времени (вызываем в какой-то опр промежуток времени)
{
    glutPostRedisplay (); /// коgда что будет перерисовка

    x_oct = x_oct + v_x_oct;
    y_oct = y_oct + v_y_oct; /// каждые 10 мсек меняем координаты окт и света
    x_sfer = x_sfer + v_x_sfer;
    Draw ();
    glutTimerFunc (10, TimerFunction, 1); /// каждые 10 милисек вызваем Timer функцию, значение таймера
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
                sfer_mode --; /// остановка сферы
                v_x_sfer --;
            }
            else
            {
                sfer_mode = 1;
                v_x_sfer = 1;
            }
            break;
        case 'r':   ///прозрачноссть
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
    glutInit(&argc, argv); /// переносим из консоли все значения в OpenGL для выполнения всех параметров
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /// инициализация цветов
    glutInitWindowSize(1000, 1000); ///рамер окна
    glutCreateWindow("Lab3"); /// название окна
    glutDisplayFunc(Draw); /// функция отрисовка
    glutReshapeFunc(ChangeSize); /// функция изменения размеров
    glutTimerFunc(5, TimerFunction, 0.5); ///функция таймер милисек, интидикатор таймера
    glutKeyboardFunc(glutNormalKeys); /// функция макросво
    glEnable(GL_LIGHTING); /// используем свое освещения
    glEnable(GL_LIGHT0); /// вкл конктретный источник света 1/8
    glEnable(GL_COLOR_MATERIAL); /// используем цветные материалы
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); /// позиция света
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); /// подсветка внутри и снаружи
    glClearColor(0,0,0,0); /// цвет фона
    glEnable(GL_DEPTH_TEST); /// разрешаем глубину
    glEnable(GL_BLEND); /// подключение обрабоки прозрачности
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /// выбор параметров прозрачности
    create_list(); /// подключение дисп списков
    LoadTex(); /// загрузка текстур
    glutMainLoop(); ///главый цикл программы где все отрисовываем
    glDisable(GL_DEPTH_TEST); /// отключение обрабоки глубины
    return 0;
}
