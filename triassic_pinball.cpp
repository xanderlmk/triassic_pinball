//modified by Xander Reyes, Andres Botello
//date:
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "abotello.h"
#include "isamara.h"
#include "xreyes.h"
#include "gtorres.h"
//some structures
Image img("dinosaurs.jpeg");
Image lvl2grass("grass1.jpg");
Image lvl2rock("rock1.jpg");
Image lvl2rock2("stone2.jpg");

extern Triangle t1;
extern Triangle t2;
extern Triangle t3;
extern Triangle t4;
extern Triangle t5;
extern Triangle t6;
extern Triangle t7;
extern Triangle t8;
extern Triangle t9;
extern Triangle flipper1;
extern Triangle flipper2;
extern void draw_triangle(Triangle triangle, unsigned char color[]);
extern void draw_circle(float r, float cx, float cy, unsigned char color[]);

class Global {
    public:
	int xres, yres;
	int n;
    GLuint texture;
    GLuint rocktxt;
    GLuint grassbkg;
    GLuint rock2txt;
	unsigned int pause;
	unsigned int mainmenu;
	unsigned int issa_feature;
	Global(){
	    xres = 800;
	    yres = 700;
	    n = 0;
	    pause = 0;
	    mainmenu = 0;
	    issa_feature = 0;

	}
} g;
extern int alex_feature, score, lives, summonshapes;
extern float velocity[2];
extern bool summonball;
extern double point;
class Box {
    public:
	float w;
	float h;
	float pos[2];
	float vel[2];
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	Box(){
	    w = 75.0f;
	    h = 15.0f;
	    pos[0] = 570;
	    pos[1] = 350;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
	}
	Box(float wid, float hgt, int x, int y, float v0, float v1){
	    w = wid;
	    h = hgt;
	    pos[0] = x;
	    pos[1] = y;
	    vel[0] = v0;
	    vel[1] = v1;
	}
} highbox1(50.0f,375.0f, 350, 10, 0.0f, 0.0f), 
    ball(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
    highbox2(20.0f,float(g.yres), 0, 10, 0.0f, 0.0f),
    highbox3(10.0f,float(g.yres), 440, 10, 0.0f, 0.0f),
    widebox1(230.0f,25.0f, 200, g.yres, 0.0f, 0.0f),
    box1(47.0f ,20.0f ,280, 10, 0.0f, 0.0f),
    box2(47.0f ,20.0f ,60, 10, 0.0f, 0.0f),
    greenbox(8.0f, 10.0f, 170, 12, 0.0f, 0.0f),
    boxes[2],
    start(10.0f, 300.0f, 550.0f, 250.0f, 0.0f, 0.0f),
    start2(10.0f, float(g.yres) , 595.0f, g.yres, 0.0f, 0.0f),
    out1(300.0f, 10.0f, 300.0f, 695.0f, 0.0f, 0.0f),
    out2(10.0f, float(g.yres), 5.0f, g.yres, 0.0f, 0.0f),
    bot1(75.0f, 50.0f, 75.0f, 0.0f, 0.0f, 0.0f),
    bot2(75.0f, 50.0f, 475.0f, 0.0f, 0.0f, 0.0f);



class Circle {
    public:
	float r, c[2], x, y, vel[2];
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	Circle(){
	    r = 70.0f;
	    c[0] = 600.0f;
	    c[1] = 25.0f;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
	}
	Circle(float radius, int cx,int cy, float v0, float v1) {
	    r = radius;
	    c[0] = cx;
	    c[1] = cy;
	    vel[0] = v0;
	    vel[1] = v1;
	}

} test_ball(10,g.xres/2,g.yres/2,0,0), 
  halfcir(50.0f, 350 ,375, 0.0f, 0.0f),
  cir1(18.0f, 165 ,245, 0.0f, 0.0f),
  cir2(18.0f, 115 ,210, 0.0f, 0.0f),
  cir3(18.0f, 215 ,210, 0.0f, 0.0f),
  cir4(25.0f, 210 ,400, 0.0f, 0.0f),
  circle1(30.0f, 125, 550, 0.0f, 0.0f),
  circle2(25.0f, 225, 450, 0.0f, 0.0f),
  circle3(25.0f, 350, 575, 0.0f, 0.0f),
  circle4(25.0f, 300, 350, 0.0f, 0.0f);

class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
    void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

void init_opengl(void);
void physics(void);
void render(void);



//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
    init_opengl(); 
    //Main loop
    int done = 0;
    while (!done) {
	//Process external events.
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    x11.check_mouse(&e);
	    done = x11.check_keys(&e);
	    if (g.mainmenu == 4)
		done = 1;
	}
	physics();
	render();
	x11.swapBuffers();
	usleep(2000);
    }
    cleanup_fonts();
    return 0;
}

X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w = g.xres, h = g.yres;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	cout << "\n\tcannot connect to X server\n" << endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	cout << "\n\tno appropriate visual found\n" << endl;
	exit(EXIT_FAILURE);
    }  
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask =
	ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask |
	PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);

    glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Triassic Pinball");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //window has been resized.
    g.xres = width;
    g.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != g.xres || xce.height != g.yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}
//-----------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;

    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
	    e->type != ButtonPress &&
	    e->type != MotionNotify) {
	//This is not a mouse event that we care about.
	return;
    }
    //
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    if (g.mainmenu == 0) {
		g.mainmenu = select_option(e->xbutton.x, g.yres - e->xbutton.y);
	    } else if (g.mainmenu == 1) {
		if (g.issa_feature) {
		    push_button(&test_ball.vel[0], g.xres, g.yres);
		}
	    }



	    //Left button was pressed.
	    //int y = g.yres - e->xbutton.y;
	    //make_particle(e->xbutton.x, g.yres - e->xbutton.y);
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed.
	    return;
	}
    }
    if (e->type == MotionNotify) {
	//The mouse moved!
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	    if (!g.pause){
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		//Code placed here will execute whenever the mouse moves.
	    }
	}
    }
}

void triangleCollisionLU()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
void triangleCollisionLD()
{
    float temp = ball.vel[1];
    ball.vel[1] = ball.vel[0]-1;
    ball.vel[0] = temp-1;
}
void triangleCollisionRU()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
void triangleCollisionRD()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
int X11_wrapper::check_keys(XEvent *e)
{
    leftFlipper = 0;
    rightFlipper = 0;
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
	switch (key) {
	    case XK_p:
		g.pause = manage_pstate(g.pause);
		break;
		if (XK_Shift_L)
		    // Key 2 and shift are both pressed down
		{
		    if (alex_feature == 0)
			alex_feature = 1;
		    else 
			alex_feature = 0;
		}
		break;
	    case XK_3:
		if (XK_Shift_L && g.mainmenu == 1) {
		    g.issa_feature = !g.issa_feature;
		}
		break;
	    case XK_space:
		summonball = true;
		velocity[1] = 8.0f;
		ball.vel[1] = velocity[1];
		break;
	    case XK_e:
        case XK_2:
        if (level1 == 1) {
            level2 = 1;
            level1 = 0;
        }
        else {
            level1 = 1;
            level2 = 0;
        }
        break;
		if (XK_Shift_L && g.mainmenu != 0) {
		    g.mainmenu = 0;
		}
		break;
            case XK_Left:
                leftFlipper = 1;
                break;
            case XK_Right:
                rightFlipper = 1;
                break;
	    case XK_Escape:
		//Escape key was pressed
		return 1;
	}
    }
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.2, 0.5, 0.2, 0.3);
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    glGenTextures(1, &g.texture);
    int w = img.width;
    int h = img.height;
    glBindTexture(GL_TEXTURE_2D, g.texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, img.data);

    glGenTextures(1, &g.rocktxt);

	w = lvl2rock.width;
   	h = lvl2rock.height;
   	glBindTexture(GL_TEXTURE_2D, g.rocktxt);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, lvl2rock.data);

    glGenTextures(1, &g.grassbkg);

    w = lvl2grass.width;
   	h = lvl2grass.height;
   	glBindTexture(GL_TEXTURE_2D, g.grassbkg);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, lvl2grass.data);

    glGenTextures(1, &g.rock2txt);
    
    w = lvl2rock2.width;
   	h = lvl2rock2.height;
   	glBindTexture(GL_TEXTURE_2D, g.rock2txt);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, lvl2rock2.data);
}
float Gravity = 0.075;

void physics()
{
    if(!g.pause){
	if (summonball) {
	    if (ball.vel[0] <= 9.0f || ball.vel[1] <= 9.0f) {
		ball.pos[0] += ball.vel[0];
		ball.pos[1] += ball.vel[1];
		ball.vel[1] -= Gravity;
	    }
	}
    if(g.issa_feature) {
	if (test_ball.vel[0] < 0) {
	    test_ball.vel[0] = 0;
	}
	if (test_ball.vel[0] > 0) {
	    test_ball.vel[0] -= Gravity;
	}
    }

    /* Box collision */
    extern void box_collision(float *ballx, float *bally, int ballw, 
            int boxx, int boxy, int w, int h, float *vx, float *vy);
    /* Circle Collision */
    extern void circle_collision(float *ballx, float *bally, 
            float cx, float cy, float r, float *vx, float *vy);
    /* For hypotenus facing left downwards and facing upwards */
    extern void triangle_collision( Triangle triangle, float *ballx,
            float *bally, float *vx, float *vy); 
    /* For hypotenus facing right downwards */
    extern void xtriangle_collision( Triangle triangle, float ballx,
            float bally, float *vx, float *vy);
    // FIRST  
    if (level1 == 1) {
	
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, highbox1.pos[0],
        highbox1.pos[1], highbox1.w, highbox1.h, &ball.vel[0], &ball.vel[1]);
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, highbox2.pos[0],
        highbox2.pos[1], highbox2.w, highbox2.h, &ball.vel[0], &ball.vel[1]);
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, widebox1.pos[0],
        widebox1.pos[1], widebox1.w, widebox1.h, &ball.vel[0], &ball.vel[1]);
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, highbox3.pos[0],
        highbox3.pos[1], highbox3.w, highbox3.h, &ball.vel[0], &ball.vel[1]);
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, box1.pos[0],
        box1.pos[1], box1.w, box1.h, &ball.vel[0], &ball.vel[1]);
    box_collision(&ball.pos[0], &ball.pos[1], ball.w, box2.pos[0],
        box2.pos[1], box2.w, box2.h, &ball.vel[0], &ball.vel[1]);

   /* CIRCLES */
    circle_collision(&ball.pos[0], &ball.pos[1],halfcir.c[0], halfcir.c[1],
            halfcir.r, &ball.vel[0], &ball.vel[1]);

    circle_collision(&ball.pos[0], &ball.pos[1],cir1.c[0], cir1.c[1],
            cir1.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],cir2.c[0], cir2.c[1],
            cir2.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],cir3.c[0], cir3.c[1],
            cir3.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],cir4.c[0], cir4.c[1],
            cir4.r, &ball.vel[0], &ball.vel[1]);
        
    /* For hypotenus facing left downwards and facing upwards */
    triangle_collision( t1, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( t3, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( t4, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( t5, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( t6, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( t7, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    if (ball.pos[0] <= 300.0f && ball.pos[1] >= 395.0f)
	    summonshapes = 1;

    if (summonshapes) {
    	triangle_collision( t7, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    	triangle_collision( t8, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    }
    /* Flipper collision */
    triangle_collision( flipper1, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( flipper2, &ball.pos[0], &ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    /* For hypotenus facing right downwards */
    xtriangle_collision( t2, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    // Checking for wall collision
    if (ball.pos[0] - ball.w < 0)
	ball.vel[0] = -ball.vel[0];

    else if (ball.pos[0] + ball.w > g.xres)
	ball.vel[0] = -ball.vel[0];

    if (ball.pos[1] + ball.h > g.yres)
	ball.vel[1] = -ball.vel[1];
     
    if (ball.pos[1] < - 30) {
        if (lives > 1)
            lives--;
        else {
            lives = 3;
            score = 0;
        }

        ball.pos[0] = 415;
        ball.pos[1] = 100;
        ball.vel[0] = 0;
        ball.vel[1] = 0;
        summonball = false;
        summonshapes = 0;
    }
        if (ball.vel[0] != 0.0f || ball.vel[1] != 0.0f) {
                point += 0.01;
                        if (point >= 0.10f){
                                score += 1;
                                point = 0.00f;
                        }
         }
    }
     // SECOND
     if (level2 == 1) {
    
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, start.pos[0],
            start.pos[1], start.w, start.h, &ball.vel[0], &ball.vel[1]);
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, start2.pos[0],
            start2.pos[1], start2.w, start2.h, &ball.vel[0], &ball.vel[1]);
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, out1.pos[0],
            out1.pos[1], out1.w, out1.h, &ball.vel[0], &ball.vel[1]);
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, out2.pos[0],
            out2.pos[1], out2.w, out2.h, &ball.vel[0], &ball.vel[1]);
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, bot1.pos[0],
            bot1.pos[1], bot1.w, bot1.h, &ball.vel[0], &ball.vel[1]);
        box_collision(&ball.pos[0], &ball.pos[1], ball.w, bot2.pos[0],
            bot2.pos[1], bot2.w, bot2.h, &ball.vel[0], &ball.vel[1]);

    circle_collision(&ball.pos[0], &ball.pos[1],circle1.c[0], circle1.c[1],
            circle1.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],circle2.c[0], circle2.c[1],
            circle2.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],circle3.c[0], circle3.c[1],
            circle3.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(&ball.pos[0], &ball.pos[1],circle4.c[0], circle4.c[1],
            circle4.r, &ball.vel[0], &ball.vel[1]);

    triangle_collision( Gt1, &ball.pos[0], &ball.pos[1], // flipper1
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt2, &ball.pos[0], &ball.pos[1], // flipper2
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt3, &ball.pos[0], &ball.pos[1], // t1
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt5, &ball.pos[0], &ball.pos[1], // t1
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt7, &ball.pos[0], &ball.pos[1], // flipper1
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt9, &ball.pos[0], &ball.pos[1], // flipper2
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( Gt10, &ball.pos[0], &ball.pos[1], // t1
            &ball.vel[0], &ball.vel[1]);
    
    
    
    xtriangle_collision( Gt4, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    xtriangle_collision( Gt6, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    xtriangle_collision( Gt8, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);

                if (ball.pos[0] - ball.w < 0) {
            ball.vel[0] = -ball.vel[0];
            points += 5;
        }

        else if (ball.pos[0] + ball.w > g.xres)
            ball.vel[0] = -ball.vel[0];

        if (ball.pos[1] + ball.w > g.yres)
            ball.vel[1] = -ball.vel[1];

     }
    }
}


void draw_box(Box box, unsigned char color [])
{
    glPushMatrix();
    box.set_color(color);
    glColor3ubv(box.color);
    glTranslatef(box.pos[0], box.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f);  glVertex2f(-box.w, -box.h);
    glTexCoord2f( 0.0f, 0.0f);  glVertex2f(-box.w,  box.h);
    glTexCoord2f( 1.0f, 0.0f);  glVertex2f( box.w,  box.h);
    glTexCoord2f( 1.0f, 1.0f);  glVertex2f( box.w, -box.h);
    glEnd();
    glPopMatrix();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (g.mainmenu == 0) {
	prompt titlescreen[4];
	Rect titleprompt[5];
	for (int i=0; i<4; i++) {
	    render_menu(titlescreen[i], titleprompt[i], i, g.xres, g.yres);
	}
	render_title(titleprompt[5],g.xres,g.yres);
	return;
    } else if (g.mainmenu == 4) {
	return;
    }


    if (g.issa_feature) {
	int n = 20;
	double angle = 0.0;
	double inc = (2.0*3.14)/n;
	glColor3ub(255,255, 100);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++) {
	    test_ball.x = test_ball.r*cos(angle);
	    test_ball.y = test_ball.r*sin(angle);
	    glVertex2f(test_ball.x+test_ball.c[0],test_ball.y + test_ball.c[1]);
	    angle += inc;
	}
	glEnd();
	draw_button(g.xres,g.yres);
    }
    Rect r[2];
    unsigned char tridef[3] { 115, 80, 50};
    unsigned char cirdef[3] { 150, 150, 150};
    unsigned char def[3] { 115, 80, 50};
    if (level1 == 1) {    
    glBindTexture(GL_TEXTURE_2D, g.texture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(450, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(450,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

     // Draw Box
    unsigned char balldef[3] { 0, 0, 0};
    unsigned char col[3] { 126, 100, 255};
        draw_box(highbox1, def);
        draw_box(highbox2, def);
        draw_box(highbox3, def);
        draw_box(widebox1, def);
        draw_box(box1, def);
        draw_box(box2, def);
        int posy = boxes[0].pos[1];
        for (int i = 0; i < 2; i++) {
                boxes[i].pos[1] = posy;
                draw_box(boxes[i], col);
                posy = posy - 50;
        }
     
    /* create triangle*/
    draw_triangle(t1, tridef);
    draw_triangle(t2, tridef);
    draw_triangle(t3, tridef);
    draw_triangle(t4, tridef);
    draw_triangle(t5, tridef);
    draw_triangle(t6, tridef);
    draw_triangle(t7, tridef);
    if (summonshapes) {
    	draw_triangle(t8, tridef);
    	draw_triangle(t9, tridef);
    }
    /* create right flipper*/
    draw_triangle(flipper1, tridef);
    /* create left flipper*/
    draw_triangle(flipper2, tridef);

    //Draw Half Circle
    int n = 40; 

    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glColor3ub(115, 80, 50);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n/2+1; i++) {
	halfcir.x = halfcir.r*cos(angle);
	halfcir.y = halfcir.r*sin(angle);
	glVertex2f(halfcir.x+halfcir.c[0],halfcir.y + halfcir.c[1]);
	angle += inc;
    }
    glEnd();
	
    r[1].bot = g.yres-35;
    r[1].left = g.xres/2;
    r[1].center = -5;
    ggprint8b(&r[1], 20, 0x00ffff00, "Alex's Feature -  Shift + 2");

    extern void show_stats(int score, int lives);
    show_stats(score, lives);

    glColor4f(0.2, 0.5, 0.2, 0.3);
    glPushMatrix();
    glTranslatef(greenbox.pos[0], greenbox.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-greenbox.w, -greenbox.h);
    glVertex2f(-greenbox.w,  greenbox.h);
    glVertex2f( greenbox.w,  greenbox.h);
    glVertex2f( greenbox.w, -greenbox.h);
    glEnd();
    glPopMatrix();

    draw_box(  ball  , balldef);


    draw_circle(cir1.r,cir1.c[0],cir1.c[1], cirdef);
    draw_circle(cir2.r,cir2.c[0],cir2.c[1], cirdef);
    draw_circle(cir3.r,cir3.c[0],cir3.c[1], cirdef);
    draw_circle(cir4.r,cir4.c[0],cir4.c[1], cirdef);



    }

    if (level2 == 1) {
    
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, g.grassbkg);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(0,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
   

    glBindTexture(GL_TEXTURE_2D, g.rocktxt);
    draw_box(start, def);
    draw_box(start2, def);
    draw_box(out1, def);
    draw_box(out2, def);
    draw_box(bot1, def);
    draw_box(bot2, def);
    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, g.rocktxt);
    draw_triangle(Gt1, tridef);
    draw_triangle(Gt2, tridef);
    draw_triangle(Gt3, tridef);
    draw_triangle(Gt4, tridef);
    draw_triangle(Gt5, tridef);
    draw_triangle(Gt6, tridef);
    draw_triangle(Gt7, tridef);
    draw_triangle(Gt8, tridef);
    draw_triangle(Gt9, tridef);
    draw_triangle(Gt10, tridef);
    /*draw_triangle(Gt11, tridef);
    draw_triangle(Gt12, tridef);*/
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    
        //glVertex2f(x, y);
    glColor3ub(150, 150, 150);
    glBindTexture(GL_TEXTURE_2D, g.rock2txt);
    draw_circle(circle1.r,circle1.c[0],circle1.c[1], cirdef);
    draw_circle(circle2.r,circle2.c[0],circle2.c[1], cirdef);
    draw_circle(circle3.r,circle3.c[0],circle3.c[1], cirdef);
    draw_circle(circle4.r,circle4.c[0],circle4.c[1], cirdef);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    if (leftFlipper) {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(150.0f, 50.0f);
        glVertex2f(225.0f, 100.0f);
        glFlush();
        glEnd();
    }
    else {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(150.0f, 50.0f);
        glVertex2f(225.0f, 25.0f);
        glFlush();
        glEnd();
    }


    if (rightFlipper) {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(400.0f, 50.0f);
        glVertex2f(325.0f, 100.0f);
        glFlush();
        glEnd();
    }
    else {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(400.0f, 50.0f);
        glVertex2f(325.0f, 25.0f);
        glFlush();
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, g.texture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(600, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(600,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);


    }

    // Summon Ball	
    //if (summonball) {
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(ball.pos[0], ball.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-ball.w, -ball.h);
    glVertex2f(-ball.w,  ball.h);
    glVertex2f( ball.w,  ball.h);
    glVertex2f( ball.w, -ball.h);
    glEnd();
    glPopMatrix();

    /*if (g.pause){
        r[0].bot = g.yres/1.5;
        r[0].left = g.xres/2;
        r[0].center = 0;
        ggprint8b(&r[6], 20, 0x00ff0000, "Pause Feature");
    }
    r[1].bot = g.yres-35;
    r[1].left = g.xres/2;
    r[1].center = -5;
    ggprint8b(&r[7], 20, 0x00ffff00, "Alex's Feature -  Shift + 2");
*/
    renderStats(stats[1], g.xres, g.yres, points);
}






