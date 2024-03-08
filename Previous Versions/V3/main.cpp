#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


float px, py; // player position

// function to draw player on 2d map
void drawPlayer()
{
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}

// map geometry
int mapX = 8, mapY = 8, mapS = 64;
int map[] =
    {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1};        // 0's are empty spaces and 1s are walls

// function to draw map
void drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) // loop through x and y values for map
    {
        for (x = 0; x < mapX; x++)
        {
            if(map[y*mapX+x] == 1) // determines which space of the map we are on
            {
                glColor3f(1, 1, 1); // empty spaces are white
            } else{
                glColor3f(0, 0, 0); // walls are black
            }

            // vertecies of the map within the window
            xo = x * mapS;
            yo = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);
            glVertex2i(xo + 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + 1);
            glEnd();
        }
    }
}

// create display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();    // draw 2D map
    drawPlayer();   // draw player
    glutSwapBuffers();
}

// function to map the wasd keys to movement
void buttons(unsigned char key,int x, int y)
{
    if(key=='w') {
        py -= 5;
    }
    if(key=='a') {
        px -= 5;
    }
    if(key=='s') {
        py += 5;
    }
    if(key=='d') {
        px += 5;
    }
    glutPostRedisplay();
}

// initialize function
void init()
{
    glClearColor(0.3, 0.3, 0.3, 0); // background color
    gluOrtho2D(0, 1024, 512, 0); // size of window

    // initial player position
    px = 300;
    py = 300;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // display mode
    glutInitWindowSize(1024, 512);  // size of window
    glutCreateWindow("raycaster"); // create window with title 'raycaster'
    init(); // call init function
    glutDisplayFunc(display);   // call display function
    glutKeyboardFunc(buttons);  // call function to take keyboard inputs
    glutMainLoop(); // loop until app is closed
}