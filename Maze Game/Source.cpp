#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <gl/stb_image.h>
#include <GL/glut.h>

//using namespace std;

unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;

float ratio;
bool fullscreen = true;

int state= 0, score = 0, hight_score = 0;
bool pause = false;
float x = 0, z = 0;
float plus = -20;
float eyex = 0, eyey = 110, eyez = z + plus, downx = 0, downz = -30;
int yangle = 90;
float xMin = -30.5, xMax = 23.5, yMin = -10, yMax = 10, zMin = -58, zMax = 0;
float time = 0;
char s[50];
float nn = M_PI / 180;

//Funcations
void special_key(int, int, int);
void processKeys(unsigned char, int, int);
void timer(int);
void background();
void reshape(int, int);
void start();
void maze();
void create_hight_box(float x, float z);
void create_width_box(float x, float z);
void around();
void renderBitmapString(float, float, float, void*, const char*); //display text on the screen
void score_board();
void car();
void myDrow();
bool collision(int n);
void end();
void load(int);
void check(unsigned char*);


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 650);
    glutInitWindowPosition(200, 20);
    glutCreateWindow("Maze Game");
    glutFullScreen();

    background();
    glutDisplayFunc(myDrow);
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(special_key);

    glutMainLoop();

    return 0;
}


void background() {
    glClearColor(.3, .3, .3, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}
void reshape(int w, int h) {
    if (h == 0)h = 1;
    ratio = w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, ratio, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);

    if (state == 1 && !pause && time < 30)time += 0.1;
    else if(time >= 30) state = 3;
}
void special_key(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        fullscreen = !fullscreen;
        if (fullscreen) {
            glutFullScreen();
        }
        else {
            glutPositionWindow(100, 80);
            glutReshapeWindow(1000, 800);
        }
        break;
    case GLUT_KEY_DOWN:
        eyez++;
        break;
    case GLUT_KEY_UP:
        eyez--;
        break;
    case GLUT_KEY_LEFT:
        if (state == 1 && !pause && !collision(3)) {
            yangle = (yangle + 90) % 360;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (state == 1 && !pause && !collision(2)) {
            yangle = (yangle - 90) % 360;
        }
        break;
    default:
        break;
    }
}
void processKeys(unsigned char key, int X, int y) {
    switch (key)
    {
    case 13: // or Enter To Start 
        start();
        break;
    case 32: // space
        pause = !pause;
        break;
    case 'W':
    case 'w': // forward
        if (state == 1 && !pause && !collision(0)) {
            x += cos(yangle * nn); z -= sin(yangle * nn);
        }
        break;
    case 'S':
    case 's': // back
        if (state == 1 && !pause && !collision(1)) {
            x -= cos(yangle * nn); z += sin(yangle * nn);
        }
        break;
    case 'D':
    case 'd': // right
        if (state == 1 && !pause && !collision(2)) {
            yangle = (yangle - 5) % 360;
        }
        break;
    case 'A':
    case 'a': // left
        if (state == 1 && !pause && !collision(3)) {
            yangle = (yangle + 5) % 360;
        }
        break;
    case 'Q':
    case 'q': // up
        eyey++;
        break;
    case 'E':
    case 'e': // down
        eyey--;
        break;
    case 27:  // Escape for exit
        exit(0);
        break;
    default:
        break;
    }
}

void start() {
    if (state!= 1) {
        state = 1;

        x = 0; z = 0;
        yangle = 90;
        time = 0;

        eyey = 6;
        eyez = z + 3;
        downz = -1;
    }
}

void myDrow() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(eyex, eyey, eyez, 0, -5, downz, 0, 1, 0);
    
    car();
    score_board();

    glPushMatrix();
    glRotatef(-1 * (yangle - 90), 0, 1, 0);
    glTranslatef(-1 * x, 0, -1 * z);

    maze();
    around();
    end();

    glPopMatrix();

    glutSwapBuffers();
}

void maze() {

    //1
    create_width_box(xMin, zMin);
    create_width_box(xMin, zMin + 28);
    create_width_box(xMin, zMin + 42);
    create_width_box(xMin, zMin + 56);

    //2
    create_hight_box(xMin + 5, zMin + 7);
    create_hight_box(xMin + 5, zMin + 14);
    create_hight_box(xMin + 5, zMin + 21);
    create_hight_box(xMin + 5, zMin + 35);

    //3
    create_width_box(xMin + 7, zMin + 0);
    create_width_box(xMin + 7, zMin + 7);
    create_width_box(xMin + 7, zMin + 35);
    create_width_box(xMin + 5, zMin + 49);
    create_width_box(xMin + 7, zMin + 56);

    //4
    create_hight_box(xMin + 12, zMin + 14);
    create_hight_box(xMin + 12, zMin + 42);
    create_hight_box(xMin + 12, zMin + 49);

    //5
    create_width_box(xMin + 14, zMin + 0);
    create_width_box(xMin + 14, zMin + 7);
    create_width_box(xMin + 12, zMin + 21);
    create_width_box(xMin + 14, zMin + 35);
    create_width_box(xMin + 14, zMin + 56);

    //6
    create_hight_box(xMin + 19, zMin + 9);
    create_hight_box(xMin + 19, zMin + 21);
    create_hight_box(xMin + 19, zMin + 28);
    create_hight_box(xMin + 19, zMin + 42);

    //7
    create_width_box(xMin + 21, zMin + 21);
    create_width_box(xMin + 21, zMin + 42);
    create_width_box(xMin + 19, zMin + 49);
    create_width_box(xMin + 21, zMin + 56);

    //8
    create_hight_box(xMin + 26, zMin + 2);
    create_hight_box(xMin + 26, zMin + 14);
    create_hight_box(xMin + 26, zMin + 28);
    create_hight_box(xMin + 26, zMin + 35);

    //9
    create_width_box(xMin + 26, zMin + 0);
    create_width_box(xMin + 28, zMin + 7);
    create_width_box(xMin + 28, zMin + 35);
    create_width_box(xMin + 26, zMin + 49);

    //10
    create_hight_box(xMin + 33, zMin + 16);
    create_hight_box(xMin + 33, zMin + 42);
    create_hight_box(xMin + 33, zMin + 49);

    //11
    create_width_box(xMin + 33, zMin + 0);
    create_width_box(xMin + 33, zMin + 14);
    create_width_box(xMin + 33, zMin + 28);
    create_width_box(xMin + 35, zMin + 42);
    create_width_box(xMin + 33, zMin + 56);

    //12
    create_hight_box(xMin + 40, zMin + 2);
    create_hight_box(xMin + 40, zMin + 9);
    create_hight_box(xMin + 40, zMin + 35);

    //13
    create_width_box(xMin + 40, zMin + 0);
    create_width_box(xMin + 42, zMin + 14);
    create_width_box(xMin + 40, zMin + 21);
    create_width_box(xMin + 40, zMin + 28);
    create_width_box(xMin + 40, zMin + 49);
    create_width_box(xMin + 40, zMin + 56);

    //14
    create_hight_box(xMin + 47, zMin + 16);
    create_hight_box(xMin + 47, zMin + 30);
    create_hight_box(xMin + 47, zMin + 37);
    create_hight_box(xMin + 47, zMin + 44);

    //15
    create_width_box(xMin + 47, zMin + 0);
    create_width_box(xMin + 47, zMin + 7);
    create_width_box(xMin + 47, zMin + 28);
    create_width_box(xMin + 47, zMin + 56);
}

void create_hight_box(float x, float z) {

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(x + 1, yMin + 5, z + 3.5);
    glScalef(2,10,7);

    glutSolidCube(1);

    glPopMatrix();
}

void create_width_box(float x, float z) {

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(x + 3.5, yMin + 5, z + 1);
    glScalef(7, 10, 2);

    glutSolidCube(1);

    glPopMatrix();
}

void around() {

    load(1);
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);

    //right
    glTexCoord2d(10, 3);
    glVertex3f(xMax, yMin, zMax);
    glTexCoord2d(0, 3);
    glVertex3f(xMax, yMin, zMin);
    glTexCoord2d(0, 0);
    glVertex3f(xMax, yMax, zMin);
    glTexCoord2d(10, 0);
    glVertex3f(xMax, yMax, zMax);

    //left
    glTexCoord2d(10, 3);
    glVertex3f(xMin, yMin, zMax);
    glTexCoord2d(0, 3);
    glVertex3f(xMin, yMin, zMin);
    glTexCoord2d(0, 0);
    glVertex3f(xMin, yMax, zMin);
    glTexCoord2d(10, 0);
    glVertex3f(xMin, yMax, zMax);

    //bottom
    glTexCoord2d(10, 10);
    glVertex3f(xMin, yMin, zMax);
    glTexCoord2d(0, 10);
    glVertex3f(xMax, yMin, zMax);
    glTexCoord2d(0, 0);
    glVertex3f(xMax, yMin, zMin);
    glTexCoord2d(10, 0);
    glVertex3f(xMin, yMin, zMin);

    glEnd();
}

void car() {

    //glPushMatrix();

    //glTranslatef(x, 0, z);
    //glRotatef(yangle, 0, 1, 0);

    glColor3f(0, 0, 1);

    //right
    glBegin(GL_POLYGON);
    glVertex3f(.4, yMin + .25, zMax - 1.8);
    glVertex3f(.4, yMin + .5, zMax - 1.8);
    glVertex3f(.4, yMin + .75, zMax - 1.4);
    glVertex3f(.4, yMin + 1, zMax - 1.35);
    glVertex3f(.4, yMin + 1, zMax - 0.45);
    glVertex3f(.4, yMin + .75, zMax - 0.4);
    glVertex3f(.4, yMin + .75, zMax);
    glVertex3f(.4, yMin + .25, zMax);
    glEnd();

    //left
    glBegin(GL_POLYGON);
    glVertex3f(- .4, yMin + .25, zMax);
    glVertex3f(- .4, yMin + .25, zMax - 1.8);
    glVertex3f(- .4, yMin + .5, zMax - 1.8);
    glVertex3f(- .4, yMin + .75, zMax - 1.4);
    glVertex3f(- .4, yMin + 1, zMax - 1.35);
    glVertex3f(- .4, yMin + 1, zMax - 0.45);
    glVertex3f(- .4, yMin + .75, zMax - 0.4);
    glVertex3f(- .4, yMin + .75, zMax);
    glEnd();

    glBegin(GL_QUADS);
    //back
    glVertex3f(- .4, yMin + .25, zMax);
    glVertex3f(.4, yMin + .25, zMax);
    glVertex3f(.4, yMin + .75, zMax);
    glVertex3f(- .4, yMin + .75, zMax);

    //front
    glVertex3f(-.4, yMin + .25, zMax - 1.8);
    glVertex3f(.4, yMin + .25, zMax - 1.8);
    glVertex3f(.4, yMin + .75, zMax - 1.8);
    glVertex3f(-.4, yMin + .75, zMax - 1.8);

    glColor3f(0, 0.3, 1);
    //top car
    glVertex3f(- .4, yMin + 1, zMax - 0.45);
    glVertex3f(.4, yMin + 1, zMax - 0.45);
    glVertex3f(.4, yMin + 1, zMax - 1.35);
    glVertex3f(- .4, yMin + 1, zMax - 1.35);

    //back top
    glVertex3f(- .4, yMin + .75, zMax);
    glVertex3f(.4, yMin + .75, zMax);
    glVertex3f(.4, yMin + .75, zMax - 0.4);
    glVertex3f(- .4, yMin + .75, zMax - 0.4);

    //front top
    glVertex3f(-.4, yMin + .75, zMax - 1.4);
    glVertex3f(.4, yMin + .75, zMax - 1.4);
    glVertex3f(.4, yMin + .75, zMax - 1.8);
    glVertex3f(-.4, yMin + .75, zMax - 1.8);

    //back glass
    glColor3f(0.4, 0.4, 0.4);
    glVertex3f(- .4, yMin + .75, zMax - 0.4);
    glVertex3f(.4, yMin + .75, zMax - 0.4);
    glVertex3f(.4, yMin + 1, zMax - 0.45);
    glVertex3f(- .4, yMin + 1, zMax - 0.45);

    //front glass
    glVertex3f(-.4, yMin + .75, zMax - 1.4);
    glVertex3f(.4, yMin + .75, zMax - 1.4);
    glVertex3f(.4, yMin + 1, zMax - 1.35);
    glVertex3f(-.4, yMin + 1, zMax - 1.35);
    glEnd();

    //wheels
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.2);
    //back right wheel
    glTranslatef(.4, yMin + .25, zMax - 0.4);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 1.45);
    glPopMatrix();

    glPushMatrix();
    //back left wheel
    glTranslatef(- .4, yMin + .25, zMax - 0.4);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 1.45);
    glPopMatrix();

    glPushMatrix();
    //front right wheel
    glTranslatef(.4, yMin + .25, zMax - 1.4);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 1.45);
    glPopMatrix();

    glPushMatrix();
    //front left wheel
    glTranslatef(- .4, yMin + .25, zMax - 1.4);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.07, 0.13, 10, 1.45);
    glPopMatrix();

    //glPopMatrix();
}

int i = 0;
void end() {

    //end
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glVertex3f(xMin + 21, 0, zMin);
    glVertex3f(xMin + 26, 0, zMin);
    glVertex3f(xMin + 26, 0.5, zMin);
    glVertex3f(xMin + 21, 0.5, zMin);

    glColor3f(0, 0, 0);
    glVertex3f(xMin + 21, 0.5, zMin);
    glVertex3f(xMin + 26, 0.5, zMin);
    glVertex3f(xMin + 26, 1, zMin);
    glVertex3f(xMin + 21, 1, zMin);

    glColor3f(1, 1, 1);
    glVertex3f(xMin + 21, 1, zMin);
    glVertex3f(xMin + 26, 1, zMin);
    glVertex3f(xMin + 26, 1.5, zMin);
    glVertex3f(xMin + 21, 1.5, zMin);

    glColor3f(0, 0, 0);
    glVertex3f(xMin + 21, 1.5, zMin);
    glVertex3f(xMin + 26, 1.5, zMin);
    glVertex3f(xMin + 26, 2, zMin);
    glVertex3f(xMin + 21, 2, zMin);
    glEnd();

    //bottom left
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < 360; i += 5) {
        glVertex3f(xMin + 21 + (cos(i) / 4), 2, zMin + (sin(i) / 4));
        glVertex3f(xMin + 21 + (cos(i) / 4), yMin, zMin + (sin(i) / 4));
    }
    glEnd();

    //bottom right
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < 360; i += 5) {
        glVertex3f(xMin + 26 + (cos(i) / 4), 2, zMin + (sin(i) / 4));
        glVertex3f(xMin + 26 + (cos(i) / 4), yMin, zMin + (sin(i) / 4));
    }
    glEnd();


}

void score_board() {
    //Text Information in Frist Page
    if (state== 0) {
        glColor3f(0, 1, 0);
        renderBitmapString(0, 0, zMax-1, GLUT_BITMAP_TIMES_ROMAN_24, "START");
    }
    else if (state== 1 && !pause) {
        //Print Score
        glColor3f(0, 1, 0);
        sprintf(s, "Time :%d S", int(time));
        renderBitmapString(0, 0, zMax-1, GLUT_BITMAP_HELVETICA_18, s);
    }
    else if (state== 1 && pause) {
        //Print Score
        glColor3f(0, 1, 0);
        renderBitmapString(0, 0, zMax-1, GLUT_BITMAP_TIMES_ROMAN_24, "Paused");
    }
    else if (state== 2) {
        glColor3f(0, 1, 0);
        renderBitmapString(0, 0, zMax, GLUT_BITMAP_HELVETICA_18, "Win");

        glColor3f(1, 1, 0);
        renderBitmapString(-1, 0, zMax, GLUT_BITMAP_HELVETICA_18, "Play Again");

        glColor3f(1, 0, 1);
        sprintf(s, "Your Score: %d", (30 - int(time)));
        renderBitmapString(1, 0, zMax, GLUT_BITMAP_HELVETICA_18, s);
    }
    else if (state== 3) {
        glColor3f(1, 1, 0);
        renderBitmapString(-1, 0, zMax-1, GLUT_BITMAP_HELVETICA_18, "Play Again");

        glColor3f(1, 0, 0);
        renderBitmapString(1, 0, zMax-1, GLUT_BITMAP_HELVETICA_18, "GAME OVER");
    }
}

void renderBitmapString(float x, float y, float z, void* font, const char* string) {
    const char* c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

float Z = 0, X = 0;
bool collision(int n) {

    if (n == 0) {
        Z = z - (1.8 * sin(yangle * nn));
        X = x + (1.8 * cos(yangle * nn));
    }
    else if (n == 1) {
        Z = z;
        X = x;
    }
    else if (n == 2) {
        Z = z - (1.8 * sin(yangle * nn)) + (0.4 * cos(yangle * nn));
        X = x + (1.8 * cos(yangle * nn)) + (0.4 * sin(yangle * nn));
    }
    else if (n == 3) {
        Z = z - (1.8 * sin(yangle * nn)) - (0.4 * cos(yangle * nn));
        X = x + (1.8 * cos(yangle * nn)) - (0.4 * sin(yangle * nn));
    }

    //1
    if (Z > zMin + 2 && Z < zMin + 7) {
        if (X > xMin && X < xMin + 26) return false;
        if (X > xMin + 28 && X < xMin + 40) return false;
        if (X > xMin + 42 && X < xMin + 54) return false;
    }
    //2
    else if (Z > zMin + 9 && Z < zMin + 14) {
        if (X > xMin + 7 && X < xMin + 19) return false;
        if (X > xMin + 21 && X < xMin + 40) return false;
        if (X > xMin + 42 && X < xMin + 54) return false;
    }
    //3
    else if (Z > zMin + 16 && Z < zMin + 21) {
        if (X > xMin + 14 && X < xMin + 26) return false;
        if (X > xMin + 35 && X < xMin + 47) return false;
    }
    //4
    else if (Z > zMin + 23 && Z < zMin + 28) {
        if (X > xMin + 7 && X < xMin + 19) return false;
        if (X > xMin + 21 && X < xMin + 54) return false;
    }
    //5
    else if (Z > zMin + 30 && Z < zMin + 35) {
        if (X > xMin && X < xMin + 19) return false;
        if (X > xMin + 28 && X < xMin + 47) return false;
    }
    //6
    else if (Z > zMin + 37 && Z < zMin + 42) {
        if (X > xMin + 7 && X < xMin + 26) return false;
        if (X > xMin + 28 && X < xMin + 40) return false;
    }
    //7
    else if (Z > zMin + 44 && Z < zMin + 49) {
        if (X > xMin && X < xMin + 12) return false;
        if (X > xMin + 21 && X < xMin + 33) return false;
        if (X > xMin + 35 && X < xMin + 47) return false;
    }
    //8
    else if (Z > zMin + 51 && Z <= zMin + 58) {
        if (X > xMin && X < xMin + 12) return false;
        if (X > xMin + 14 && X < xMin + 33) return false;
        if (X > xMin + 35 && X < xMin + 54) return false;
    }

    //1
    if(X > xMin && X < xMin + 5) {
        if (Z > zMin + 2 && Z < zMin + 28) return false;
        else if (Z > zMin + 30 && Z < zMin + 42) return false;
        else if (Z > zMin + 44 && Z < zMin + 56) return false;
    }
    //2
    else if (X > xMin + 7 && X < xMin + 12) {
        if (Z > zMin + 2 && Z < zMin + 7) return false;
        else if (Z > zMin + 9 && Z < zMin + 35) return false;
        else if (Z > zMin + 37 && Z < zMin + 49) return false;
        else if (Z > zMin + 51 && Z < zMin + 56) return false;
    }
    //3
    else if (X > xMin + 14 && X < xMin + 19) {
        if (Z > zMin + 2 && Z < zMin + 7) return false;
        else if (Z > zMin + 9 && Z < zMin + 21) return false;
        else if (Z > zMin + 23 && Z < zMin + 35) return false;
        else if (Z > zMin + 37 && Z < zMin + 56) return false;
    }
    //4
    else if (X > xMin + 21 && X < xMin + 26) {
        if (Z < zMin) { state = 2; }
        else if (Z < zMin + 2) return false;
        else if (Z > zMin + 2 && Z < zMin + 21) return false;
        else if (Z > zMin + 23 && Z < zMin + 42) return false;
        else if (Z > zMin + 44 && Z < zMin + 49) return false;
        else if (Z > zMin + 51 && Z < zMin + 56) return false;
    }
    //5
    else if (X > xMin + 28 && X < xMin + 33) {
        if (Z > zMin + 2 && Z < zMin + 7) return false;
        else if (Z > zMin + 9 && Z < zMin + 35) return false;
        else if (Z > zMin + 37 && Z < zMin + 49) return false;
        else if (Z > zMin + 51 && Z <= zMin + 58) return false;
    }
    //6
    else if (X > xMin + 35 && X < xMin + 40) {
        if (Z > zMin + 2 && Z < zMin + 14) return false;
        else if (Z > zMin + 16 && Z < zMin + 28) return false;
        else if (Z > zMin + 30 && Z < zMin + 42) return false;
        else if (Z > zMin + 44 && Z < zMin + 56) return false;
    }
    //7
    else if (X > xMin + 42 && X < xMin + 47) {
        if (Z > zMin + 2 && Z < zMin + 14) return false;
        else if (Z > zMin + 16 && Z < zMin + 21) return false;
        else if (Z > zMin + 23 && Z < zMin + 28) return false;
        else if (Z > zMin + 30 && Z < zMin + 49) return false;
        else if (Z > zMin + 51 && Z < zMin + 56) return false;
    }
    //8
    else if (X > xMin + 49 && X < xMin + 54) {
        if (Z > zMin + 2 && Z < zMin + 7) return false;
        else if (Z > zMin + 9 && Z < zMin + 28) return false;
        else if (Z > zMin + 30 && Z < zMin + 56) return false;
    }
    
    return true;
}

void load(int imgnum) {
    if (imgnum == 1) {
        data = stbi_load("wall.bmp", &width, &height, &nrChannels, 0);
        check(data);
    }
}

void check(unsigned char* data) {
    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}