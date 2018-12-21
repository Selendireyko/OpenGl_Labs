#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GLUT/GLUT.h>
#include <math.h>

GLfloat x_oct = 0;  //  начальная координата окт
GLfloat v_x_oct = 0.5; // скорость вращения
GLfloat y_oct = 0;
GLfloat v_y_oct = 0;

GLfloat light_pos [] = {0, 0, 0, 1}; // позиция источника света
bool light_on = true;
GLfloat x_sfer = 0; // кордината света
GLfloat v_x_sfer = 1; // скорость поворота света
int sfer_mode = 1;

GLfloat opened = 0; // граница раздвижения окт

void Draw ()
{
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // очистка буфера (цвета, глубина)
    
    
    // Движение сферы и источника света
    glLoadIdentity (); // загрузка 1-ой матрицы
    glTranslatef (0, 0, -20); // откидываем точку вокуг которой гоняет сфера (центр окт)
    glRotatef ((x_sfer), 0, 1, 0); // вращение источника света
    glTranslatef (0, 0, 15); // откидываем сферу от себя
    glLightfv (GL_LIGHT0, GL_POSITION, light_pos); //координаты света
    
    
    // Отрисовка сферы
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();
    glColor3d (255, 234, 0);
    gluQuadricDrawStyle (quadObj, GLU_LINE);
    gluSphere (quadObj, 2.5, 50, 50); // функция отрисовки сфера (наз, радиус, узлы вдоль и вокруг Oz)
    
    
    //  Отрисовка Октаидра
    glLoadIdentity ();
    glTranslatef (0, 0, -20); // откидываем центр окт
    glRotatef ((x_oct), 0, 1, 0);  // движение окт
    glRotatef ((y_oct), 1, 0, 0);
    
    glBegin (GL_TRIANGLES);
    glColor3f (1.0, 0.0, 0.0);  // красная грань
    glNormal3d (-1, -1, 1);
    glVertex3f (0+opened, 10+opened, 0-opened);
    glVertex3f (0+opened, 0+opened, -10-opened);
    glVertex3f (10+opened, 0+opened, 0-opened);
    
    glColor3f (1.0, 0.5, 0.0);  // оранжевый грань
    glNormal3d (-1, -1, -1);
    glVertex3f (0+opened, 10+opened, 0+opened);
    glVertex3f (10+opened, 0+opened, 0+opened);
    glVertex3f (0+opened, 0+opened, 10+opened);
    
    glColor3f (1.0, 1.0, 0.0);  // желтая грань
    glNormal3d (1, -1, -1);
    glVertex3f (0-opened, 10+opened, 0+opened);
    glVertex3f (0-opened, 0+opened, 10+opened);
    glVertex3f (-10-opened, 0+opened, 0+opened);
    
    glColor3f (0.0, 1.0, 0.0);  // зеленая грань
    glNormal3d (1, -1, -1);
    glVertex3f (0+opened, 0-opened, -10-opened);
    glVertex3f (10+opened, 0-opened, 0-opened);
    glVertex3f (0+opened, -10-opened, 0-opened);
    
    glColor3f (0.0, 1.0, 1.0);  // голубая грань
    glNormal3d ( -1, 1, -1);
    glVertex3f (0+opened, 0-opened, 10+opened);
    glVertex3f (10+opened, 0-opened, 0+opened);
    glVertex3f (0+opened, -10-opened, 0+opened);
    
    glColor3f (0.0, 0.0, 0.5);  // синяя грань
    glNormal3d (-1, -1, 1);
    glVertex3f (0-opened, 0-opened, 10+opened);
    glVertex3f (-10-opened, 0-opened, 0+opened);
    glVertex3f (0-opened, -10-opened, 0+opened);
    
    glColor3f (1.0, 0.0, 1.0);  // фелетовая грань
    glNormal3d (-1, 1, -1);
    glVertex3f (0-opened, 10+opened, 0-opened);
    glVertex3f (0-opened, 0+opened, -10-opened);
    glVertex3f (-10-opened, 0+opened, 0-opened);
    
    glColor3f (1.0, 0.0, 0.0);  // градинетная грань
    glNormal3d (1, 1, 1);
    glVertex3f (0-opened, 0-opened, -10-opened);
    glColor3f (1.0, 1.0, 0.0);
    glVertex3f (-10-opened, 0-opened, 0-opened);
    glColor3f (0.0, 0.0, 0.5);
    glVertex3f (0-opened, -10-opened, 0-opened);
    
    glEnd ();
    
    // glFlush ();
    glutSwapBuffers ();
}

void ChangeSize (GLsizei w, GLsizei h) // измаенение параментров ширины/высоты когда меняем размер окна
{
    if (h == 0)
        h = 1; //избежание деления на 0
    
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //координаты окна
    
    glMatrixMode (GL_PROJECTION); //матрица проекций
    glLoadIdentity ();
    gluPerspective (90, (GLdouble)w/h, 1, 150); // создание облости видимости (90град обзор, соотношение сторон, мин дальность, макс дальность)
    glMatrixMode (GL_MODELVIEW); // видовая матрица
    glViewport (0, 0, (GLdouble)w, (GLdouble)h); // рисуем во всем окне
    
    Draw ();
}

void TimerFunction (int value) // функция анимации по времени (вызываем в какой-то опр промежуток времени)
{
    glutPostRedisplay (); // конда что будет перерисовка
    glutTimerFunc (10, TimerFunction, 1); // каждые 10 милисек вызваем Timer функцию, значение таймера
    x_oct = x_oct + v_x_oct;
    y_oct = y_oct + v_y_oct; // каждые 10 мсек меняем координаты окт и света
    x_sfer = x_sfer + v_x_sfer;
}

void glutNormalKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
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
            if (sfer_mode > 0)
            {
                sfer_mode = 0; // остановка сферы
                v_x_sfer = 0;
            }
            else
            {
                sfer_mode = 1;
                v_x_sfer = 1;
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
    std::cout << "press 'w' for turn up\n press 'a' for turn left\n press 's' for turn down\n press 'd' for turn right\n press 'space' for stop oct\n press 'e' for open oct\n press 'q' for close oct\n press 'c' for stop sfer\n press 'f' for turn off light\n press 'esc' for close app" << std::endl;
    glutInit(&argc, argv); // переносим из консоли все значения в OpenGL для выполнения всех параметров
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // инициализация цветов
    glutInitWindowSize(1000, 1000); //рамер окна
    glutCreateWindow("Lab3"); // название окна
    glutDisplayFunc(Draw); // функция отрисовка
    glutReshapeFunc(ChangeSize); // функция изменения размеров
    glutTimerFunc(5, TimerFunction, 0.5); //функция таймер милисек, интидикатор таймера
    glutKeyboardFunc(glutNormalKeys); // функция макросво
    glEnable(GL_LIGHTING); // используем свое освещения
    glEnable(GL_LIGHT0); // вкл конктретный источник света 1/8
    glEnable(GL_COLOR_MATERIAL); // используем цветные материалы
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); // позиция света
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // подсветка внутри и снаружи
    glClearColor(0,0,0,0); // цвет фона
    glEnable(GL_DEPTH_TEST); // разрешаем глубину
    glutMainLoop(); //главый цикл программы где все отрисовываем
    return 0;
}
