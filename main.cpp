#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174533 // one degree in radians

float px, py, pdx, pdy, pa; // player position (player x, player y, player delta x, player delta y, player angle)

// function to draw player on 2d map
void drawPlayer()
{
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();

    // draw line showing direction player is facing
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
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
        1, 1, 1, 1, 1, 1, 1, 1}; // 0's are empty spaces and 1s are walls

// function to draw map
void drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) // loop through x and y values for map
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y * mapX + x] == 1) // determines which space of the map we are on
            {
                glColor3f(1, 1, 1); // empty spaces are white
            }
            else
            {
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

float dist(float ax, float ay, float bx, float by, float ang)
{ // distance of line using pythagorean theorem
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

// draw rays from player
void drawRays2D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    ra = pa - (DR * 30);
    if (ra < 0)
    {
        ra += 2 * PI;
    }
    if (ra > 2 * PI)
    {
        ra -= 2 * PI;
    }


    for (r = 0; r < 60; r++)
    {
        //---Check Horizontal Lines---//
        dof = 0;
        float disH = 10000000000, hx = px, hy = py;
        float aTan = -1 / tan(ra);
        if (ra > PI)
        {                                        // looking up
            ry = (((int)py >> 6) << 6) - 0.0001; // round y pos to nearing 64th value
            rx = (py - ry) * aTan + px;          // rays x value (length)
            yo = -64;                            // y offset = -64
            xo = -yo * aTan;                     // x offset is y offset times inverse tangent
        }

        if (ra < PI)
        {                                    // looking down
            ry = (((int)py >> 6) << 6) + 64; // round y pos to nearing 64th value
            rx = (py - ry) * aTan + px;      // rays x value (length)
            yo = 64;                         // y offset = -64
            xo = -yo * aTan;                 // x offset is y offset times inverse tangent
        }

        if (ra == 0 || r == PI)
        { // looking straight left or right
            rx = px;
            ry = py;
            dof = 8; // depth of field = 8
        }

        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
                dof = 8; // hit wall
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            } // next line
        }

        //---Check vertical Lines---//
        dof = 0;
        float disV = 10000000000, vx = px, vy = py;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3)
        {                                        // looking left
            rx = (((int)px >> 6) << 6) - 0.0001; // round x pos to nearing 64th value
            ry = (px - rx) * nTan + py;          // rays y value (length)
            xo = -64;                            // x offset = -64
            yo = -xo * nTan;                     // y offset is z offset times inverse tangent
        }

        if (ra < P2 || ra > P3)
        {                                    // looking right
            rx = (((int)px >> 6) << 6) + 64; // round x pos to nearing 64th value
            ry = (px - rx) * nTan + py;      // rays y value (length)
            xo = 64;                         // x offset = -64
            yo = -xo * nTan;                 // x offset is y offset times inverse tangent
        }

        if (ra == 0 || ra == PI)
        { // looking straight up or down
            rx = px;
            ry = py;
            dof = 8; // depth of field = 8
        }

        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
                dof = 8; // hit wall
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            } // next line
        }

        if (disV < disH)
        {
            rx = vx;
            ry = vy;
        }
        if (disH < disV)
        {
            rx = hx;
            ry = hy;
        }

        glColor3f(1, 0, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();

        ra += DR;
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        if (ra > 2 * PI)
        {
            ra -= 2 * PI;
        }
    }
}

// create display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();  // draw 2D map
    drawRays2D(); // draw rays
    drawPlayer(); // draw player
    glutSwapBuffers();
}

// function to map the wasd keys to movement
void buttons(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        pa -= 0.1;
        if (pa < 0)
        {
            pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'd')
    {
        pa += 0.1;
        if (pa > 2 * PI)
        {
            pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'w')
    {
        px += pdx;
        py += pdy;
    }
    if (key == 's')
    {
        px -= pdx;
        py -= pdy;
    }
    glutPostRedisplay();
}

// initialize function
void init()
{
    glClearColor(0.3, 0.3, 0.3, 0); // background color
    gluOrtho2D(0, 1024, 512, 0);    // size of window

    // initial player position
    px = 300;
    py = 300;

    // calculate delta x and delta y when program first runs
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // display mode
    glutInitWindowSize(1024, 512);               // size of window
    glutCreateWindow("raycaster");               // create window with title 'raycaster'
    init();                                      // call init function
    glutDisplayFunc(display);                    // call display function
    glutKeyboardFunc(buttons);                   // call function to take keyboard inputs
    glutMainLoop();                              // loop until app is closed
}