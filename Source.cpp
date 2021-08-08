/**
* Josh Ryther
* CS-330
* Project One
**/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "BMP.h"

#define PI 3.1415927
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1

void lighting(void)
{
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 }; //Neutral/natural color lighting
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_position[] = { -1.0, -1.0, -5.0, 0.0 };
    GLfloat light_position2[] = { 1.0, 1.0, 5.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

GLuint LoadTexture(const char* filename)
{
    BMP info = BMP::BMP(filename);
    int width = info.GetWidth();
    int height = info.GetHeight();

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, width, height, 0, info.HasAlphaChannel() ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, info.GetPixels().data());

    return texture;
}

// 2D or 3D scene indicator
int dimension = 3;

float xpos = 15.0, ypos = 0.0, zpos = 5, xrot = 0, yrot = 90, angle = 00.0;
float lastx, lasty;

//speed of camera movement
float speed = 0.2f;

//camera position and angle function
void camera(void) {
    glRotatef(xrot, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos, -ypos, -zpos); //translate the screen to the position of our camera
}

/************************** draw_papertowel() **************************
  * This function will draw the cylinder
  *
  *   @parameter1: radius = The radius of cylinder
  *   @parameter2: height = Height of the cylinder
  *
  *   @return: Nothing
  */
void draw_papertowel(GLfloat radius, GLfloat height)
{
    GLuint papertowelTexture1 = LoadTexture("Resources/0079.bmp");
    GLuint papertowelTexture2 = LoadTexture("Resources/papertowel2.bmp");

    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, papertowelTexture1);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while (angle < 2 * PI) { // Rotate around the circle
        x = radius * cos(angle);
        y = radius * sin(angle);
        glTexCoord2f(angle, 0.0f);  glVertex3f(x, y, height);
        glTexCoord2f(angle, 1.0f); glVertex3f(x, y, 0.0);
        angle = angle + angle_stepsize;
    }
    glTexCoord2f(0.0f, 0.0f); glVertex3f(radius, 0.0, height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder **/
    glColor4f(1.0, 1.0, 1.0, 1.0); // Color
    glBegin(GL_POLYGON);
    angle = 0.0;
    while (angle < 2 * PI) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glEnd();

    /** Draw the plane coming out of the cylinder**/
    x = radius * cos(67.5);
    y = radius * sin(67.5);

    glBindTexture(GL_TEXTURE_2D, papertowelTexture2);
    glColor4f(1.0, 1.0, 1.0, 1.0); //Color
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, height); //Top right vertex
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, 0); //Bottom right vertex
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - 1.0, y, height); //Top left vertex
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - 1.0, y, 0); //Bottom left vertex
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/************************** draw_table() **************************
  * This function will draw the table
  *
  *   @parameter1: length = length of table
  *   @parameter2: width = width of table
  *   @parameter3: height = height of table
  *
  *   @return: Nothing
  */
void draw_table(GLfloat length, GLfloat width, GLfloat height){

/** Object Orientation
*  -------
* |       |
* |       |
* |       |
* |   O   |
* |       |
* |       |
* ---------
* 
**/
    GLuint woodTexture = LoadTexture("Resources/wood.bmp");

    /** Draw the table */
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    //glColor4f(0.212, 0.133, 0.0, 1.0);
    glBegin(GL_QUADS);

    // Front panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.0f);   // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 3.0f, 1.0f);  // Top left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.0f); // Bottom left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -3.0f, 1.0f);  // Bottom right

    // Back panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.25f);   // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 3.0f, 1.25f);  // Top left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.25f); // Bottom left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -3.0f, 1.25f);  // Bottom right

    // Top panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.0f);   // Right front
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.25f);  // Right back
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 3.0f, 1.25f); // Left back
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 3.0f, 1.0f);  // Left front

    // Bottom panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -3.0f, 1.0f);  // Front right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -3.0f, 1.25f);  // Back right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.25f); // Back left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.0f); // Front left


    // Left panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 3.0f, 1.25f);  // Top back
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 3.0f, 1.0f);  // Top front
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.0f); // Bottom front
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -3.0f, 1.25f); // Bottom back


    // Right Panel
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.0f);  // Top front
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 3.0f, 1.25f);  // Top back
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -3.0f, 1.25f); // Bottom back
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -3.0f, 1.0f); // Bottom front

    glEnd();  // End of drawing color-cube
    glDisable(GL_TEXTURE_2D);
}


/**************************** display() ******************************
 * The glut callback function to draw the polygons on the screen.
 *
 *   @parameter1: Nothing
 *
 *   @return: Nothing
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();


    if (dimension == 2) {
        gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glOrtho(0.0, 640, 480, 0.0f, 0.0f, 1.0f);
    }

    camera();
    draw_papertowel(0.3, 1.0);
    draw_table(2, 1, 5);
    glTranslatef(0, 1, 1);
    glutSolidTorus(0.05 , 0.1, 20, 20);
    glTranslatef(0, -2, -.25);
    glutSolidCube(0.5);
    glTranslatef(0, 2, .25);

    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

/**************************** reshape() ******************************
 * The glut callback function that responsible when the window is
 * resized.
 *
 *   @parameter1: width  = Current window's width
 *   @parameter2: height = Current window's height
 *
 *   @return: Nothing
 */
void reshape(int width, int height)
{
    if (width == 0 || height == 0) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)width / (GLdouble)height,
        1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

//Keyboard movement function
void keyboard(unsigned char key, int x, int y) {
    if (key == 'p')
    {
        if (dimension == 3) {
            dimension = 2;
        }
        else if (dimension == 2) {
            dimension = 3;
        }
    }
    if (key == 'q')
    {
        float xrotrad;
        xrotrad = (yrot / 180 * PI);
        ypos += float(cos(xrotrad)) * speed;
    }

    if (key == 'e')
    {
        float xrotrad;
        xrotrad = (yrot / 180 * PI);
        ypos -= float(cos(xrotrad)) * speed;
    }

    if (key == 'w')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI);
        xpos += float(sin(yrotrad)) * speed;
        zpos -= float(cos(yrotrad)) * speed;
        ypos -= float(sin(xrotrad)) * speed;
    }

    if (key == 's')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI);
        xpos -= float(sin(yrotrad)) * speed;
        zpos += float(cos(yrotrad)) * speed;
        ypos += float(sin(xrotrad)) * speed;
    }

    if (key == 'd')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos += float(cos(yrotrad)) * speed;
        zpos += float(sin(yrotrad)) * speed;
    }

    if (key == 'a')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos -= float(cos(yrotrad)) * speed;
        zpos -= float(sin(yrotrad)) * speed;
    }

    if (key == 27)
    {
        exit(0);
    }
}

//Mouse movement function
void mouseMovement(int x, int y) {
    int diffx = x - lastx; //check the difference between the current x and the last x position
    int diffy = y - lasty; //check the difference between the current y and the last y position
    lastx = x; //set lastx to the current x position
    lasty = y; //set lasty to the current y position
    xrot += (float)diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float)diffx;    //set the xrot to yrot with the addition of the difference in the x position
}

// Scroll wheel movement speed function
void mouseWheel(int button, int dir, int x, int y) {
    if (button == 3) { // Up on scrollwheel
        speed += 0.2f;
        if (speed > 3.0f) { //Speed caps at 3.0
            speed = 3.0f;
        }
    }
    else if (button == 4) { // Down on scrollwheel
        speed -= 0.2f;
        if (speed < 0.2f) { //Lowest speed it 0.2
            speed = 0.2f;
        }
    }
    //Terminal results showing speed change
    std::cout << "Button: " << button << std::endl;
    std::cout << "Speed: " << speed << std::endl << std::endl;
}

int main(int argc, char** argv)
{
    /** Initialize glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Milestone Week 4");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    lighting();
    glutReshapeFunc(reshape);
    glutIdleFunc(display);

    //Movement functions
    glutMouseFunc(mouseWheel);
    glutPassiveMotionFunc(mouseMovement);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}