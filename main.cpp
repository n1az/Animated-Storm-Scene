#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include<math.h>>
#include<iostream>
#include <cmath>
#include<random>
#include <bits/stdc++.h>
#define PI acos(-1)
std::default_random_engine re;
GLint refreshTimeMillis = 16;
GLdouble speed=0;
GLdouble x=0;
GLfloat i = 0.0f;
GLfloat j = 0.0f;
GLfloat k = 0.0f;GLfloat l = 0.0f;
GLdouble c1=0.0;GLdouble c2=0.0;
GLboolean right = true;
GLboolean stop = false;
GLboolean day = true;
GLfloat rainHeight = 0.0f;
/* Handler for window-repaint event. Call back when the window first appears and
whenever the window needs to be re-painted. */
void circle(GLfloat x, GLfloat y, GLfloat radius);
void drawCircle(GLdouble x, GLdouble y, GLfloat r, GLint steps);
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void Idle()
{
    glutPostRedisplay();//// marks the current window as needing to be redisplayed
}
void Timer(int value) {
	glutTimerFunc(30, Timer, 0);
	glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
        day=true;


        break;
        case GLUT_KEY_DOWN:
        day = false;

        break;
        case GLUT_KEY_RIGHT:
            //glLoadIdentity();
            stop = true;
        break;
        case GLUT_KEY_LEFT:
            //glLoadIdentity();
            stop = false;
        break;
    }
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON)
    {

        right=false;

    }
    if (button == GLUT_LEFT_BUTTON)
    {

        right=true;

    }
	glutPostRedisplay();
}
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}
void drawsky_day()
{
    glBegin(GL_QUADS);
    glColor3f(0.941, 0.866, 0.8);
    glVertex2f(-2,-2);
    glVertex2f(-2,2);
    glVertex2f(2,2);
    glVertex2f(2,-2);
    glEnd();
    //sun
    glColor3ub(253,216,53);
    drawCircle(-0.1,0.15,.15,100);
    glColor3ub(255,179,0);
    drawCircle(-0.1,0.15,.13,100);
	glColor3ub(251,140,0);
    drawCircle(-0.1,0.15,.11,100);
}

void drawsky_night()
{
    glBegin(GL_QUADS);
    glColor3f(0.145, 0.172, 0.415);
    glVertex2f(-2,-2);
    glVertex2f(-2,2);
    glVertex2f(2,2);
    glVertex2f(2,-2);
    glEnd();
    //moon
    glColor3ub(219, 217, 199);
    drawCircle(-0.1,0.15,.15,100);
    glColor3ub(197, 194, 170);
    drawCircle(-0.1,0.15,.13,100);
	glColor3ub(193, 188, 138);
    drawCircle(-0.1,0.15,.11,100);
}

void drawRoad(GLfloat x, GLfloat y, GLfloat z)
{
    //road
    glBegin(GL_QUADS);
    glColor3ub(38,50,56);
    glVertex2f(-2,-2);
    glVertex2f(-2,-.1);
    glVertex2f(2,-.1);
    glVertex2f(2,-2);
    glEnd();

    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(x,y,z);


    for(double m=0;m<200;m+=.4)
    {
        glBegin(GL_QUADS);
        glColor3ub(189,189,189);
        glVertex2f(m-4,-.54);
        glVertex2f(m-4,-.50);
        glVertex2f(m-3.8,-.50);
        glVertex2f(m-3.8,-.54);
        glEnd();
    }
    glPopMatrix();
}

void drawRain(GLfloat x, GLfloat y, GLfloat z)
{
    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(x,y,z);
    double m=0;
    double n=0;
    for(m;m<10;m+=.05)
    {
            glBegin(GL_QUADS);
            glColor3ub(144,164,174);
            glVertex2f(m-2,n+3.99);
            glVertex2f(m-2,n+4);
            glVertex2f(m-1.995,n+4);
            glVertex2f(m-1.995,n+3.99);
            glEnd();
            n-=0.005;
    }
    glPopMatrix();

}

class Rain {
private:
    GLdouble left, right, bottom, top;
	GLdouble *x, *y;
	GLdouble angle;
	GLdouble speed;
	GLint particle_count;

    std::uniform_real_distribution<GLdouble> *urd_horizontal, *urd_vertical;

	void initParticles() {
        for (int i = 0; i < particle_count; ++i) {
            x[i] = (*urd_horizontal)(re);
            y[i] = (*urd_vertical)(re);
        }
	}

public:
	Rain(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble angle, GLdouble speed, GLint particle_count) {
		this->angle = angle;
		this->speed = speed;
		this->particle_count = particle_count;

		urd_horizontal = new std::uniform_real_distribution<GLdouble>(left, right);
		urd_vertical = new std::uniform_real_distribution<GLdouble>(bottom, top);

		x = new GLdouble[particle_count];
		y = new GLdouble[particle_count];

		initParticles();
	}

	~Rain() {
        delete urd_horizontal;
        delete urd_vertical;

        delete[] x;
        delete[] y;
	}

	void setAngle(GLdouble angle) {
		this->angle = angle;
	}


    void update() {
        for (int i = 0; i < particle_count; ++i) {
            if (y[i] < bottom) {
                y[i] = (*urd_vertical)(re);
            }

            y[i] -= speed;
        }
	}

	void draw() {
	    for (int i = 0; i < particle_count; ++i) {
            glPushMatrix();
            glRotated(angle, 0, 0, 1);
            glTranslated(x[i], y[i], 0.0);

            glBegin(GL_QUADS);
                glVertex2f( 0.005f, -0.03f);
                glVertex2f( 0.005f,  0.03f);
                glVertex2f(-0.005f,  0.03f);
                glVertex2f(-0.005f,  -0.03f);
            glEnd();

            glPopMatrix();
	    }
	}
};
Rain *rain;
void sound()
{

    //PlaySound("rain.wav", NULL, SND_ASYNC|SND_FILENAME);
    PlaySound("r.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

}
void drawRiverNight()
{
    glEnable(GL_LIGHTING);//Enable Light Effect

    GLfloat global_ambient1[] = {1, 1, 4,0.5};//ambient RGBA intensity of light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient1);


    glBegin(GL_QUADS);
    glColor3ub(66, 118, 123);
    glVertex2f(-2,-2);
    glVertex2f(-2,-.1);
    glVertex2f(2,-.1);
    glVertex2f(2,-2);
    glEnd();

    glDisable(GL_LIGHTING);//Enable Light Effect
}

void display() {
    glClearColor(0.941, 0.866, 0.8, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

    glMatrixMode(GL_MODELVIEW);


    if(day)
    {
        drawsky_day();
    }
    else if(!day)
    {
        drawsky_night();
    }



    //clouds

    //clouds1
    glColor3ub(84,110,122);
    drawCircle(c1-0.6,.8,.12,100);
    drawCircle(c1-0.5,.8,.12,100);
    glColor3ub(96,125,139);
    drawCircle(c1-0.7,.8,.09,100);
    drawCircle(c1-0.6,.85,.12,100);

    //clouds2
    glColor3ub(96,125,139);
    drawCircle(c2+0.6,.6,.12,100);
    drawCircle(c2+0.5,.6,.09,100);
    glColor3ub(84,110,122);
    drawCircle(c2+0.7,.6,.09,100);
    drawCircle(c2+0.6,.65,.12,100);

//mountain1
    glBegin(GL_TRIANGLES);
    glColor3ub(85,139,47);
    glVertex2f(-.75,-.1);
    glVertex2f(-.4,.2);
    glVertex2f(.3,-.1);
    glEnd();
 //mountain2
    glBegin(GL_TRIANGLES);
    glColor3ub(56,142,60);
    glVertex2f(.3,-.1);
    glVertex2f(.6,.25);
    glVertex2f(1,-.1);
    glEnd();
//mountain3
    glBegin(GL_TRIANGLES);
    glColor3ub(46,125,50);
    glVertex2f(.6,-.1);
    glVertex2f(.8,.2);
    glVertex2f(1.3,-.1);
    glEnd();

    //mountain4
    glBegin(GL_TRIANGLES);
    glColor3ub(76,175,80);
    glVertex2f(-1.6,-.1);
    glVertex2f(-.75,.25);
    glVertex2f(0,-.1);
    glEnd();

    //mountain5
    glBegin(GL_TRIANGLES);
    glColor3ub(51,105,30);
    glVertex2f(-.2,-.1);
    glVertex2f(0.12,.22);
    glVertex2f(0.6,-.1);
    glEnd();
//mountain6
    glBegin(GL_TRIANGLES);
    glColor3ub(27,94,32);
    glVertex2f(0,-.1);
    glVertex2f(0.3,.24);
    glVertex2f(0.9,-.1);
    glEnd();

    //windmill-1
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-.62,-.1);
    glVertex2f(-.61,.2);
    glVertex2f(-.6,-.1);
    glEnd();

    drawCircle(-.61,.15,0.02,100);

    //windmill-2
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(.3,-.1);
    glVertex2f(.31,.2);
    glVertex2f(.32,-.1);
    glEnd();
    drawCircle(.31,.15,0.02,100);
    //windmill-3
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(0.7,-.1);
    glVertex2f(0.71,.2);
    glVertex2f(0.72,-.1);
    glEnd();
    drawCircle(.71,.15,0.02,100);
    //road
    drawRoad(x,0,0);



    //car
    //hood
    glBegin(GL_QUADS);
    glColor3ub(255,143,0);
    glVertex2f(-.25,-.5);
    glVertex2f(0,-.3);
    glVertex2f(0.3,-.34);
    glVertex2f(0.5,-.5);
    glEnd();
    //window
    glBegin(GL_QUADS);
    glColor3ub(128,222,234);
    glVertex2f(-.25,-.52);
    glVertex2f(0.01,-.31);
    glVertex2f(0.275,-.36);
    glVertex2f(0.45,-.52);
    glEnd();
    //head
    glBegin(GL_QUADS);
    glColor3ub(255,143,0);
    glVertex2f(-.55,-.55);
    glVertex2f(-.55,-.5);
    glVertex2f(-.5,-.47);
    glVertex2f(-.5,-.55);
    glEnd();
    //head-strip
    glBegin(GL_QUADS);
    glColor3ub(38,50,56);
    glVertex2f(-.55,-.54);
    glVertex2f(-.55,-.52);
    glVertex2f(-.5,-.50);
    glVertex2f(-.5,-.54);
    glEnd();

//head2
    glBegin(GL_QUADS);
    glColor3ub(255,111,0);
    glVertex2f(-.5,-.55);
    glVertex2f(-.5,-.47);
    glVertex2f(-0.1,-.4);
    glVertex2f(-0.1,-.55);
    glEnd();

    //door
    glBegin(GL_QUADS);
    glColor3ub(191,54,12);
    glVertex2f(-0.1,-.55);
    glVertex2f(-0.1,-.4);
    glVertex2f(0.27,-.4);
    glVertex2f(0.27,-.55);
    glEnd();
    //handle
    glBegin(GL_QUADS);
    glColor3ub(38,50,56);
    glVertex2f(0.1,-.46);
    glVertex2f(0.1,-.45);
    glVertex2f(0.14,-.45);
    glVertex2f(0.14,-.46);
    glEnd();
    //back
    glBegin(GL_QUADS);
    glColor3ub(239,108,0);
    glVertex2f(0.2,-.55);
    glVertex2f(0.28,-.37);
    glVertex2f(0.6,-.45);
    glVertex2f(0.6,-.55);
    glEnd();

//tires
    glColor3ub(84,110,122);
    drawCircle(-0.2,-0.5,.065,100);
    glColor3ub(0,137,123);
    drawCircle(-0.2,-0.5,.04,100);
    glColor3ub(84,110,122);
    drawCircle(0.5,-0.5,.065,100);
    glColor3ub(0,137,123);
    drawCircle(0.5,-0.5,.04,100);

    //c-1
    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(-.2f,-0.5f,1.0f);
    glRotatef(j,0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f( 0.0f, 0.03f);
    glVertex2f( 0.01f, 0.0f);
    glEnd();

    glPopMatrix();

    //c-2
    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(0.5f,-0.5f,1.0f);
    glRotatef(j,0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f( 0.0f, 0.03f);
    glVertex2f( 0.01f, 0.0f);
    glEnd();

    glPopMatrix();


    glLoadIdentity();//Reset the current matrix

    glPushMatrix(); //glPushMatrix copies the top matrix and pushes it onto the stack, while glPopMatrix pops the top matrix off the stack
    glTranslatef(-0.61f,0.15f,1.0f);
    glRotatef(i,0.0,0.0,1.0);
//w-1
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f( 0.0f, 0.1f);
    glVertex2f( 0.01f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( -0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( 0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();
    glPopMatrix();

//w-2
    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(0.31f,0.15f,1.0f);
    glRotatef(k,0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f( 0.0f, 0.1f);
    glVertex2f( 0.01f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( -0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( 0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();

    glPopMatrix();//while glPopMatrix pops the top matrix off the stack

    //w-3
    glLoadIdentity();//Reset the current matrix

    glPushMatrix();
    glTranslatef(0.71f,0.15f,1.0f);
    glRotatef(l,0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f( 0.0f, 0.1f);
    glVertex2f( 0.01f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( -0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(38,50,56);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f( 0.09f, -0.05f);
    glVertex2f( 0.02f, 0.0f);
    glEnd();

    glPopMatrix();

    glColor3ub(144,164,174);
    rain = new Rain(-2, 2, -5, 5, 0, 0.02, 250);
    rain->draw();
	rain->update();

    if(!stop)
    {
       speed-=0.00005f;
        if(speed<=-2.0)
        {
            speed=-.4f;
        }
    }
    else if(stop)
    {
        speed = 0;
    }

    i+=0.07f;
    j+=1.0f;
    k+=0.1f;
    l+=0.03f;
	//cout<<x;
	//glFlush();  // Render now
	//glFlush();  // Render now

     c1+=0.0002;
	if(c1>2.00)
    {
        c1=-1.00;
    }
    c2-=0.0002;
	if(c2<-2.00)
    {
        c2=1.00;
    }
    x+=0.0006;
	if(x>2.00)
    {
        x=-4.00;
    }
	glutSwapBuffers();

    // Render now
	//glFlush();  // Render now
}

void drawCircle(GLdouble x, GLdouble y, GLfloat r, GLint steps)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x,y);
    GLfloat angle;
    for(GLint i=0;i<=steps;++i)
    {
        angle = i *2.0f *PI / steps ;
        glVertex2f(cos(angle)*r+x,sin(angle)*r+y);

    }
    glEnd();
}
void circle(GLfloat x, GLfloat y, GLfloat radius) {
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	//glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
	int i;

	//GLfloat x=x; GLfloat y=y; GLfloat radius =r;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
    glColor3f(0.0,0.0,0.0);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)),
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();

	glFlush();  // Render now
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);                // Initialize GLUT
	glutCreateWindow("OpenGL Landscape "); // Create a window with the given title
	   // Set the window's initial width & height
	glutDisplayFunc(display);
	initGL();

    glutIdleFunc(Idle); // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialInput);
    //glutKeyboardFunc(handleKeypress);

    glutMouseFunc(handleMouse);
    //glutTimerFunc(100, drawRain, 0);
    sound();
	glutMainLoop();           // Enter the event-processing loop
	return 0;
}
