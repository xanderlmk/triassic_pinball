//modified by Xander Reyes, Andres Botello
//date:
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
//

//USH THIS
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
Image main_background("main_screen.jpg");
Image leaderboard_background("leaderboard_screen.jpg");
Image control_background("controls_background.jpg");
Image end_background("end_screen.jpg");
string leaderboard = "leaderboard.txt";
string leaderboard_names[10] = {"", "", "", "", "", "", "", "", "", ""};
string leaderboard_scores[10] = {"", "", "", "", "", "", "", "", "", ""};

extern int alex_feature, score, lives, summonshapes, boxcol;
extern float velocity[2];
extern bool summonball;
extern double point;
extern Triangle t1;
extern Triangle t2;
extern Triangle t3;
extern Triangle t4;
extern Triangle t5;
extern Triangle t6;
extern Triangle t7;
extern Triangle t8;
extern Triangle t9;
extern Triangle t10;
extern Triangle flipper1;
extern Triangle flipper2;
extern void draw_triangle(Triangle triangle, unsigned char color[]);
extern void draw_circle(float r, float cx, float cy, unsigned char color[]);
extern void moving_circle(float *cx, float *cy, float *vx, float *vy, int i);
extern void show_stats(int score, int lives, int a);
extern void circle_teleport(float *ballx, float *bally, float csx, float csy,
                float r, float *vx, float *vy, float crx, float cry);
extern void box_in_circles(float *boxx, float *boxy, float *vx, float *vy);
class Global {
    public:
	int xres, yres;
	int n;
    int pr;
    int new_highscore;
    int name_counter;
    GLuint texture;
    GLuint rocktxt;
    GLuint grassbkg;
    GLuint rock2txt;
    GLuint background_texture;
    GLuint leaderboard_texture;
    GLuint control_texture;
    GLuint end_texture;
	unsigned int pause;
	unsigned int mainmenu;
    unsigned int map;
	unsigned int issa_feature;
    bool updated_score;
	Global(){
	    xres = 800;
	    yres = 700;
	    n = 0;
	    pause = 0;
	    mainmenu = 0;
        map = 0;
	    issa_feature = 0;
        updated_score = false;
        new_highscore = -1;
        name_counter = 0;
	}
} g;
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
    bool ballSpawned;
    bool init;
	Box(){
	    w = 5.0f;
	    h = 5.0f;
	    pos[0] = 570;
	    pos[1] = 350;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
        ballSpawned = 0;
        init = 0;
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
    highbox2(20.0f,float(g.yres), 0, 10, 0.0f, 0.0f),
    highbox3(10.0f,float(g.yres), 440, 10, 0.0f, 0.0f),
    widebox1(230.0f,25.0f, 200, g.yres, 0.0f, 0.0f),
    box1(47.0f ,20.0f ,280, 10, 0.0f, 0.0f),
    box2(47.0f ,20.0f ,60, 10, 0.0f, 0.0f),
    boxcir(20.0f ,20.0f ,200, 550, 0.0f, 0.0f),
    greenbox(8.0f, 10.0f, 170, 12, 0.0f, 0.0f),
    boxes[2],
    start(10.0f, 300.0f, 550.0f, 250.0f, 0.0f, 0.0f),
    start2(10.0f, float(g.yres) , 595.0f, g.yres, 0.0f, 0.0f),
    start3(10.0f, 75.0f, 550.0f, 600.0f, 0.0f, 0.0f),
    out1(300.0f, 10.0f, 300.0f, 695.0f, 0.0f, 0.0f),
    out2(10.0f, float(g.yres), 5.0f, g.yres, 0.0f, 0.0f),
    bot1(76.0f, 50.0f, 75.0f, 0.0f, 0.0f, 0.0f),
    bot2(76.0f, 50.0f, 475.0f, 0.0f, 0.0f, 0.0f),
    Obs(15.0f, 15.0f, 125, 650 , 0.0f, 0.0f),
    safeBox(100.0f, 5.0f, 175.0f, 5.0f, 0.0f, 0.0f),
    safeBox2(100.0f, 5.0f, 275.0f, 5.0f, 0.0f, 0.0f),
    particles[1000],
    ball1[MAX_BALLS] {
        Box(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
        Box(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
        Box(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
        //Box(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
        //Box(5.0f,5.0f, 415, 100, velocity[0], velocity[1]),
    },
    ball2[MAX_BALLS] {
        Box(5.0f,5.0f, 575, 100, velocity[0], velocity[1]),
        Box(5.0f,5.0f, 575, 100, velocity[0], velocity[1]),
        Box(5.0f,5.0f, 575, 100, velocity[0], velocity[1]),
        //Box(5.0f,5.0f, 575, 100, velocity[0], velocity[1]),
        //Box(5.0f,5.0f, 575, 100, velocity[0], velocity[1]),
    };

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
  cir2(18.0f, 115 ,175, 0.0f, 0.0f),
  cir3(18.0f, 215 ,175, 0.0f, 0.0f),
  cir4(25.0f, 210 ,400, 0.0f, 0.0f),
  cirsnd(20.0f, 60 ,300, 0.0f, 0.0f),
  cirrcv(15.0f, 50 ,600, 0.0f, 0.0f),
  circle1(50.0f, 125, 575, 0.0f, 0.0f),
  circle2(30.0f, 200, 425, 0.0f, 0.0f),
  circle3(30.0f, 350, 550, 0.0f, 0.0f),
  circle4(30.0f, 300, 350, 0.0f, 0.0f);

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
    srand(time(NULL));
    ball1[0].ballSpawned = 1;
    ball2[0].ballSpawned = 1;
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
void make_particle(int x, int y) { 
    particles[g.pr].w = 1;
    particles[g.pr].h = 1;
    particles[g.pr].pos[0] = x;
    particles[g.pr].pos[1] = y;
    particles[g.pr].vel[0] = ((float)rand() / (float)RAND_MAX) - 0.1;
    particles[g.pr].vel[1] = ((float)rand() / (float)RAND_MAX) * 0.2 - 0.1;
    ++g.pr;
}

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
		g.mainmenu = select_option(g.xres, g.yres, e->xbutton.x, g.yres - e->xbutton.y);
	    } else if (g.mainmenu == 1) {
            if (g.map == 0) {
                g.map = select_map(g.xres, g.yres, e->xbutton.x, g.yres - e->xbutton.y);
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
int more = 0;
int X11_wrapper::check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        if (g.mainmenu == 5 && g.new_highscore != -1 && key != XK_Escape) {
            //
            if (g.name_counter > 2) {
                //update leaderboard.txt
                //
                g.mainmenu = 0;
                g.map = 0;
                g.new_highscore = -1;
                g.name_counter = 0;
                g.updated_score = false;
                save_leaderboard(leaderboard, leaderboard_names, leaderboard_scores);
            }
            leaderboard_names[g.new_highscore] += key;
            g.name_counter++;

        } else {
	switch (key) {
	    case XK_p:
		g.pause = manage_pstate(g.pause);
		break;
	    case XK_a:
		    // Key 2 and shift are both pressed down
			alex_feature ^= 1;
			if (alex_feature) {
			cout << "alex feature mode\n";
		}
		break;
        case XK_d:
            debugGeorge = !debugGeorge;
            cout << "Debug mode toggled" << endl;
        break;
	    case XK_3:
		if (XK_Shift_L && g.mainmenu == 1) {
		    g.issa_feature = !g.issa_feature;
		}
		break;
        case XK_q:
        if (g.issa_feature) {
            lives = 0;
        }
        break;
        case XK_t:
        if (g.issa_feature) {
            score = atoi(leaderboard_scores[0].c_str()) + 1;
        }
        break;
        case XK_y:
        if (g.issa_feature) {
            score = rand()%
                (atoi(leaderboard_scores[0].c_str()) + 1);
        }
        break;
        case XK_u:
        if (g.issa_feature) {
            score = 0;
        }
        break;
	    case XK_space:
        for (int i = 0; i < MAX_BALLS; i++) {
            if (g.map == 1) {
                velocity[1] = 8.0f;
                if (ball1[i].ballSpawned == 1 && ball1[i].init == 0) {
		            ball1[i].vel[1] = velocity[1];
                    ball1[i].init = 1;
                }

            }
            else if (g.map == 2) {
                velocity[1] = 11.0f;
                if (ball2[i].ballSpawned == 1 && ball2[i].init == 0) {
                    ball2[i].vel[1] = velocity[1];
                    ball2[i].init = 1;
                }
            }
        }
		break;
	    case XK_e:
        if (XK_Shift_L && g.mainmenu != 0) {
		    g.mainmenu = 0;
            g.map = 0;
            g.updated_score = false;
			score = 0;
			lives = 3;
		}
		break;
        case XK_m:
        //Buy multiball
        if (score >= 1000) {
            if (g.map == 1) {
                for (int i = 0; i < MAX_BALLS; i++) {
                    if (ball1[i].ballSpawned == 0) {
                        ball1[i].ballSpawned = 1;
                        makeBall(i, &ball1[i].ballSpawned, &ball1[i].pos[0], &ball1[i].pos[1], &ball1[i].vel[1]); 
                        score = score - 1000;
                        summonshapes = 0;
                        more++;
                        break;
                    }
                }
            }
            if (g.map == 2) {
                for (int i = 0; i < MAX_BALLS; i++) {
                    if (ball2[i].ballSpawned == 0) {
                        ball2[i].ballSpawned = 1;
                        makeBall(i, &ball2[i].ballSpawned, &ball2[i].pos[0], &ball2[i].pos[1], &ball2[i].vel[1]); 
                        score = score - 1000;
                        break;
                    }
                }
            }
        }
        break;
        case XK_b:
        //Boost mode
        if (score >= 250) {
            for (int i = 0; i < MAX_BALLS; i++) {
                if (g.map == 1) {
                    for (int j = 0; j < 5; j++) {
                        if (ball1[i].ballSpawned) {
                            make_particle(ball1[i].pos[0], ball1[i].pos[1]);
                            ball1[i].vel[1] = 8.0f;
                        }
                    }
                }
                if (g.map == 2) {
                    for (int j = 0; j < 5; j++) {
                        if (ball2[i].ballSpawned) {
                            make_particle(ball2[i].pos[0], ball2[i].pos[1]);
                            ball2[i].vel[1] = 10.0f;
                        }
                    }
                }
            }
            score = score - 250;
        }
        break;
        case XK_s:
        //Savior Mode
        if (saviorActive == 0 && score >= 500) {
            saviorActive = 1;
            score = score - 500;
        }
        break;
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
    }

    if (e->type == KeyRelease) {
    switch (key) {
    	break;
        case XK_Left:
            leftFlipper = 0;
            break;
        case XK_Right:
            rightFlipper = 0;
            break;

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

    glGenTextures(1, &g.background_texture);
    w = main_background.width;
    h = main_background.height;
    glBindTexture(GL_TEXTURE_2D, g.background_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, main_background.data);

    glGenTextures(1, &g.leaderboard_texture);
    w = leaderboard_background.width;
    h = leaderboard_background.height;
    glBindTexture(GL_TEXTURE_2D, g.leaderboard_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, leaderboard_background.data);

    glGenTextures(1, &g.control_texture);
    w = control_background.width;
    h = control_background.height;
    glBindTexture(GL_TEXTURE_2D, g.control_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, control_background.data);
    
    glGenTextures(1, &g.end_texture);
    w = end_background.width;
    h = end_background.height;
    glBindTexture(GL_TEXTURE_2D, g.end_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, end_background.data);

    read_leaderboard(leaderboard, leaderboard_names, leaderboard_scores);

}
float Gravity = 0.085;

void physics()
{
    if(!g.pause){
        for (int i = 0; i < MAX_BALLS; i++) {
            if (ball1[i].init == 1) {
	            if (ball1[i].vel[0] <= 9.0f || ball1[i].vel[1] <= 9.0f) {
	        	ball1[i].pos[0] += ball1[i].vel[0];
	        	ball1[i].pos[1] += ball1[i].vel[1];
	        	ball1[i].vel[1] -= Gravity;
	            }
	        }
            if (ball2[i].init == 1) {
	            if (ball2[i].vel[0] <= 9.0f || ball2[i].vel[1] <= 9.0f) {
	        	ball2[i].pos[0] += ball2[i].vel[0];
	        	ball2[i].pos[1] += ball2[i].vel[1];
	        	ball2[i].vel[1] -= Gravity;
	            }
	        }
        }
    for (int i = 0; i < g.pr; i++) {
        particles[i].pos[0] += particles[i].vel[0];
        particles[i].pos[1] += particles[i].vel[1];
        particles[i].vel[1] -= Gravity;
        //check if particle is off screen...
        if (particles[i].pos[1] < 0.0) {
        particles[i] = particles[--g.pr];
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
    if (g.map == 1) {
	int a = 0,b = 1,c = 2,d = 3;
	if (alex_feature) {
		moving_circle(&cir4.c[0], &cir4.c[1], &cir4.vel[0], &cir4.vel[1], a);
		moving_circle(&cir2.c[0], &cir2.c[1], &cir2.vel[0], &cir2.vel[1], b);
		moving_circle(&cir1.c[0], &cir1.c[1], &cir1.vel[0], &cir1.vel[1], c);
		moving_circle(&cir3.c[0], &cir3.c[1], &cir3.vel[0], &cir3.vel[1], d);
	}
    for (int i = 0; i < MAX_BALLS; i++) {

    /* Box Collisions*/
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, highbox1.pos[0],
        highbox1.pos[1], highbox1.w, highbox1.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, highbox2.pos[0],
        highbox2.pos[1], highbox2.w, highbox2.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, widebox1.pos[0],
        widebox1.pos[1], widebox1.w, widebox1.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, highbox3.pos[0],
        highbox3.pos[1], highbox3.w, highbox3.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, box1.pos[0],
        box1.pos[1], box1.w, box1.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, box2.pos[0],
        box2.pos[1], box2.w, box2.h, &ball1[i].vel[0], &ball1[i].vel[1]);
    /* Box moving in Circles */
    	boxcol = 2;
	if (alex_feature)
    	box_in_circles(&boxcir.pos[0], &boxcir.pos[1], &boxcir.vel[0], &boxcir.vel[1]); 
    box_collision(&ball1[i].pos[0], &ball1[i].pos[1], ball1[i].w, boxcir.pos[0],
        boxcir.pos[1], boxcir.w, boxcir.h, &ball1[i].vel[0], &ball1[i].vel[1]);
	boxcol = 0;

    if (saviorActive) {
        if (ball1[i].pos[1] - ball1[i].w <= safeBox.pos[1] + safeBox.h) {
            ball1[i].vel[1] = 8.0f;
            saviorActive = 0;
        }
    }

   /* CIRCLES */
    circle_collision(&ball1[i].pos[0], &ball1[i].pos[1],halfcir.c[0], halfcir.c[1],
            halfcir.r, &ball1[i].vel[0], &ball1[i].vel[1]);

    circle_collision(&ball1[i].pos[0], &ball1[i].pos[1],cir1.c[0], cir1.c[1],
            cir1.r, &ball1[i].vel[0], &ball1[i].vel[1]);
    circle_collision(&ball1[i].pos[0], &ball1[i].pos[1],cir2.c[0], cir2.c[1],
            cir2.r, &ball1[i].vel[0], &ball1[i].vel[1]);
    circle_collision(&ball1[i].pos[0], &ball1[i].pos[1],cir3.c[0], cir3.c[1],
            cir3.r, &ball1[i].vel[0], &ball1[i].vel[1]);
    circle_collision(&ball1[i].pos[0], &ball1[i].pos[1],cir4.c[0], cir4.c[1],
            cir4.r, &ball1[i].vel[0], &ball1[i].vel[1]);
    /* CIRCLE TELEPORTATION */
    circle_teleport(&ball1[i].pos[0], &ball1[i].pos[1] , cirsnd.c[0], cirsnd.c[1],
                cirsnd.r, &ball1[i].vel[0], &ball1[i].vel[1], cirrcv.c[0], cirrcv.c[1]);
    /* For hypotenus facing left downwards and facing upwards */
    triangle_collision( t1, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    triangle_collision( t3, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    triangle_collision( t4, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    triangle_collision( t5, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    triangle_collision( t6, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    triangle_collision( t7, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    if (ball1[i].pos[0] <= 300.0f && ball1[i].pos[1] >= 395.0f) {
	    if (!more) {
	    summonshapes = 1;
		}
		else {
    		if (ball1[more].pos[0] <= 300.0f && ball1[more].pos[1] >= 395.0f) {
				summonshapes = 1;
			}
		}
	}

    if (summonshapes && ball1[i].ballSpawned) {
    	triangle_collision( t8, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    	triangle_collision( t9, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
		if (ball1[i].pos[0] >= 350.0f && ball1[i].pos[1] <= 450.0f) 
			 ball1[i].pos[1] = 450.0f;
    }

    flipping(g.map, &ball1[i].pos[0], &ball1[i].pos[1], &ball1[i].vel[0], &ball1[i].vel[1]);

    /* Flipper collision */
    if (!leftFlipper) {
    triangle_collision( flipper1, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
	}
    if (!rightFlipper) {	
    triangle_collision( flipper2, &ball1[i].pos[0], &ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
	}
   
    
    /* For hypotenus facing right downwards */
    xtriangle_collision( t2, ball1[i].pos[0], ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    xtriangle_collision( t10, ball1[i].pos[0], ball1[i].pos[1],
            &ball1[i].vel[0], &ball1[i].vel[1]);
    // Checking for wall collision
    if (ball1[i].pos[0] - ball1[i].w < 0)
	ball1[i].vel[0] = -ball1[i].vel[0];

    else if (ball1[i].pos[0] + ball1[i].w > g.xres)
	ball1[i].vel[0] = -ball1[i].vel[0];

    if (ball1[i].pos[1] + ball1[i].h > g.yres)
	ball1[i].vel[1] = -ball1[i].vel[1];
     
    if (ball1[i].pos[1] < - 30) {
        ball1[i].ballSpawned = 0;
        ball1[i].pos[0] = 415;
        ball1[i].pos[1] = 100;
        ball1[i].vel[0] = 0;
        ball1[i].vel[1] = 0;
        ball1[i].init = 0;
        summonshapes = 0;
		if (more > 0)
			more--;
    }
        if (ball1[i].vel[0] != 0.0f || ball1[i].vel[1] != 0.0f) {
                point += 0.01;
                        if (point >= 0.10f){
                                score += 1;
                                point = 0.00f;
                        }
         }
    }
    for (int i = 0; i < MAX_BALLS; i++) {
        if (ball1[i].ballSpawned == 0) {
            noBalls = 1;
        } else {
            noBalls = 0;
            break;
        }
    }
    if (noBalls == 1) {
        ball1[0].ballSpawned = 1;
        lives--;
        /*if(lives == 0) {
            lives = 3;
            score = 0;
        }*/
    }
    }
    // SECOND
    if (g.map == 2) {

        for (int i = 0; i < MAX_BALLS; i++) {
			if (alex_feature) {
				moving_circle(&circle2.c[0], &circle2.c[1], &circle2.vel[0], &circle2.vel[1], 4);
				moving_circle(&circle3.c[0], &circle3.c[1], &circle3.vel[0], &circle3.vel[1], 5);
			}

            rotate_point(0.01f, &Obs.pos[0], &Obs.pos[1], 125, 575);

            if (ball2[i].pos[0] < 550.0f) {
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, start3.pos[0],
                start3.pos[1], start3.w, start3.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            }
    
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, start.pos[0],
                start.pos[1], start.w, start.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, start2.pos[0],
                start2.pos[1], start2.w, start2.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, out1.pos[0],
                out1.pos[1], out1.w, out1.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, out2.pos[0],
                out2.pos[1], out2.w, out2.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, bot1.pos[0],
                bot1.pos[1], bot1.w, bot1.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, bot2.pos[0],
                bot2.pos[1], bot2.w, bot2.h, &ball2[i].vel[0], &ball2[i].vel[1]);
            box_collision(&ball2[i].pos[0], &ball2[i].pos[1], ball2[i].w, Obs.pos[0],
                Obs.pos[1], Obs.w, Obs.h, &ball2[i].vel[0], &ball2[i].vel[1]);

            if (saviorActive) {
                if (ball2[i].pos[1] - ball2[i].w <= safeBox2.pos[1] + safeBox2.h) {
                    ball2[i].vel[1] = 8.0f;
                    saviorActive = 0;
                }
            }

            circle_collision(&ball2[i].pos[0], &ball2[i].pos[1],circle1.c[0], circle1.c[1],
                    circle1.r, &ball2[i].vel[0], &ball2[i].vel[1]);
            circle_collision(&ball2[i].pos[0], &ball2[i].pos[1],circle2.c[0], circle2.c[1],
                    circle2.r, &ball2[i].vel[0], &ball2[i].vel[1]);
            circle_collision(&ball2[i].pos[0], &ball2[i].pos[1],circle3.c[0], circle3.c[1],
                    circle3.r, &ball2[i].vel[0], &ball2[i].vel[1]);
            circle_collision(&ball2[i].pos[0], &ball2[i].pos[1],circle4.c[0], circle4.c[1],
                    circle4.r, &ball2[i].vel[0], &ball2[i].vel[1]);

            flipping(g.map, &ball2[i].pos[0], &ball2[i].pos[1], &ball2[i].vel[0], &ball2[i].vel[1]);

            triangle_collision( Gt1, &ball2[i].pos[0], &ball2[i].pos[1], // flipper1
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt2, &ball2[i].pos[0], &ball2[i].pos[1], // flipper2
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt3, &ball2[i].pos[0], &ball2[i].pos[1], // t1select_map(g.xres, g.yres, e->xbutton.x, g.yres - e->xbutton.y);
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt5, &ball2[i].pos[0], &ball2[i].pos[1], // t1
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt7, &ball2[i].pos[0], &ball2[i].pos[1], // flipper1
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt9, &ball2[i].pos[0], &ball2[i].pos[1], // flipper2
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            triangle_collision( Gt10, &ball2[i].pos[0], &ball2[i].pos[1], // t1
                    &ball2[i].vel[0], &ball2[i].vel[1]);

            xtriangle_collision( Gt4, ball2[i].pos[0], ball2[i].pos[1],
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            xtriangle_collision( Gt6, ball2[i].pos[0], ball2[i].pos[1],
                    &ball2[i].vel[0], &ball2[i].vel[1]);
            xtriangle_collision( Gt8, ball2[i].pos[0], ball2[i].pos[1],
                    &ball2[i].vel[0], &ball2[i].vel[1]);

            if (ball2[i].pos[0] - ball2[i].w < 0) {
                ball2[i].vel[0] = -ball2[i].vel[0];
                points += 5;
            }

            else if (ball2[i].pos[0] + ball2[i].w > g.xres)
                ball2[i].vel[0] = -ball2[i].vel[0];

            if (ball2[i].pos[1] + ball2[i].w > g.yres)
                ball2[i].vel[1] = -ball2[i].vel[1];

        if (ball2[i].pos[1] < - 30) {
        ball2[i].ballSpawned = 0;
        ball2[i].pos[0] = 575;
        ball2[i].pos[1] = 100;
        ball2[i].vel[0] = 0;
        ball2[i].vel[1] = 0;
        ball2[i].init = 0;
        summonshapes = 0;
        }
        if (ball2[i].vel[0] != 0.0f || ball2[i].vel[1] != 0.0f) {
            point += 0.01;
            if (point >= 0.10f){
                score += 1;
                credit += 0;
                point = 0.00f;
            }
        }
        }
        for (int i = 0; i < MAX_BALLS; i++) {
        if (ball2[i].ballSpawned == 0) {
            noBalls = 1;
        } else {
            noBalls = 0;
            break;
        }
        }
        if (noBalls == 1) {
            ball2[0].ballSpawned = 1;
            lives--;
            /*if(lives == 0) {
                lives = 3;
                score = 0;
                credit = 0;
            }*/
        }
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
    if (lives <=0 && g.mainmenu == 1) {
        g.mainmenu = 5;
        g.map = 0;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    if (g.mainmenu == 0) {
        //draw start menu
        draw_texture(g.background_texture, g.xres, g.yres);
        Rect titleprompt[5];
        for (int i=0; i<4; i++) {
            render_menu(titleprompt[i], i, g.xres, g.yres);
        }
        render_title(titleprompt[5],g.xres,g.yres);
        return;
    } else if (g.mainmenu == 2) {
        //controls menu
        draw_texture(g.control_texture, g.xres, g.yres);
        return;
    } else if (g.mainmenu == 3) {
        //leaderboard menu
        draw_texture(g.leaderboard_texture, g.xres, g.yres);
        print_leaderboard_boxes(g.xres, g.yres);
        print_leaderboard(leaderboard_names, leaderboard_scores, g.xres, g.yres);
        return;
    } else if (g.mainmenu == 5) {
        //death screen
        if (!g.updated_score) {
            g.new_highscore = update_leaderboard(leaderboard_names,
                    leaderboard_scores, g.xres, g.yres, score);
            g.updated_score = true;
        }
        draw_texture(g.end_texture, g.xres, g.yres);
        print_leaderboard_boxes(g.xres, g.yres);
        print_end_screen(g.xres, g.yres, g.new_highscore);
        print_leaderboard(leaderboard_names, leaderboard_scores, g.xres, g.yres);
        return;
    } else if (g.mainmenu == 4) {
        //exit game
        return;
    } else if (g.mainmenu == 1) {
        //render actual game

    if (g.map == 0) {
        draw_texture(g.background_texture, g.xres, g.yres);
        render_map_select(g.xres, g.yres);
    }
    Rect r[2];
    unsigned char tridef[3] { 115, 80, 50};
    unsigned char cirdef[3] { 150, 150, 150};
    unsigned char def[3] { 115, 80, 50};
    unsigned char col[3] { 126, 100, 255};
    
    if (g.map == 1) {

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
    glBindTexture(GL_TEXTURE_2D, g.rocktxt);
        if (saviorActive) {
            unsigned char safeCol[3] = {255, 255, 0};
            draw_box(safeBox, safeCol);
        }
        draw_box(highbox1, def);
        draw_box(highbox2, def);
        draw_box(highbox3, def);
        draw_box(widebox1, def);
        draw_box(box1, def);
        draw_box(box2, def);
        draw_box(boxcir, def);
        int posy = 350;
        for (int i = 0; i < 2; i++) {
                boxes[i].pos[1] = posy;
                boxes[i].pos[0] = 525;
                draw_box(boxes[i], col);
                posy = posy - 50;
                boxes[i].w = 50;
                boxes[i].h = 15;
        }
     
    /* create triangle*/
    draw_triangle(t1, tridef);
    draw_triangle(t2, tridef);
    draw_triangle(t3, tridef);
    draw_triangle(t4, tridef);
    draw_triangle(t5, tridef);
    draw_triangle(t6, tridef);
    draw_triangle(t7, tridef);
    draw_triangle(t10, tridef);
    if (summonshapes) {
    	draw_triangle(t8, tridef);
    	draw_triangle(t9, tridef);
    }

    flipperRotate(g.map);
    
	glBindTexture(GL_TEXTURE_2D, 0);
    /* create right flipper*/
    draw_triangle(flipper1, tridef);
    /* create left flipper*/
    draw_triangle(flipper2, tridef);

    //Draw Half Circle
    int n = 40; 
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glBindTexture(GL_TEXTURE_2D, g.rocktxt);
    glColor3ub(115, 80, 50);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n/2+1; i++) {
	halfcir.x = halfcir.r*cos(angle);
	halfcir.y = halfcir.r*sin(angle);
	glTexCoord2f(sin(angle) / 2.0 + 0.5, cos(angle) / 2.0 + 0.5);
	glVertex2f(halfcir.x+halfcir.c[0],halfcir.y + halfcir.c[1]);
	angle += inc;
    }
    glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
    r[1].bot = g.yres-35;
    r[1].left = g.xres/2;
    r[1].center = -5;
    ggprint8b(&r[1], 20, 0x00ffff00, "Alex's Feature -  a");

    show_stats(score, lives, 1);

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


    glBindTexture(GL_TEXTURE_2D, g.rock2txt);
    draw_circle(cir1.r,cir1.c[0],cir1.c[1], cirdef);
    draw_circle(cir2.r,cir2.c[0],cir2.c[1], cirdef);
    draw_circle(cir3.r,cir3.c[0],cir3.c[1], cirdef);
    draw_circle(cir4.r,cir4.c[0],cir4.c[1], cirdef);
	glBindTexture(GL_TEXTURE_2D, 0);

    unsigned char telCol[3] = {0, 0, 0};
    draw_circle(cirsnd.r,cirsnd.c[0],cirsnd.c[1], telCol);
    draw_circle(cirrcv.r,cirrcv.c[0], cirrcv.c[1], telCol);

    for (int i = 0; i < MAX_BALLS; i++) {
        if (ball1[i].ballSpawned == 1) {
            glPushMatrix();
            glColor3ub(0,0,0);
            glTranslatef(ball1[i].pos[0], ball1[i].pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-ball1[i].w, -ball1[0].h);
            glVertex2f(-ball1[i].w,  ball1[0].h);
            glVertex2f( ball1[i].w,  ball1[0].h);
            glVertex2f( ball1[i].w, -ball1[0].h);
            glEnd();
            glPopMatrix();
        }
    }

    }

    if (g.map == 2) {
    
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


    unsigned char startdef[3] {100, 100, 100};
    glBindTexture(GL_TEXTURE_2D, g.rocktxt);
    if (saviorActive) {
        unsigned char safeCol[3] = {255, 255, 0};
        draw_box(safeBox2, safeCol);
    }
    draw_box(start3, startdef);
    draw_box(start, def);
    draw_box(start2, def);
    draw_box(out1, def);
    draw_box(out2, def);
    draw_box(bot1, def);
    draw_box(bot2, def);
    draw_box(Obs, def);
    
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

    flipperRotate(g.map);

    draw_triangle(GflipL, tridef);
    draw_triangle(GflipR, tridef);

    /*draw_triangle(Gt11, tridef);
    draw_triangle(Gt12, tridef);*/
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    
        //glVertex2f(x, y);
    glColor3ub(150, 150, 150);

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    //int n = 15; 
    //double angle = 0.0;
    //double inc = (2.0*3.14)/n;
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

    glBindTexture(GL_TEXTURE_2D, g.texture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(600, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(600,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
	glBindTexture(GL_TEXTURE_2D, g.rocktxt);
	int posy = 350;
	int posx = 700;
        for (int i = 0; i < 2; i++) {
                boxes[i].pos[1] = posy;
                boxes[i].pos[0] = posx;
                draw_box(boxes[i], col);
                posy = posy - 50;
                boxes[i].w = 50;
                boxes[i].h = 15;
        }    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    show_stats( score, lives, 0); 

    for (int i = 0; i < MAX_BALLS; i++) {
        if (ball2[i].ballSpawned == 1) {
            glPushMatrix();
            glColor3ub(135, 189, 255);
            glTranslatef(ball2[i].pos[0], ball2[i].pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-ball2[i].w, -ball2[i].h);
            glVertex2f(-ball2[i].w,  ball2[i].h);
            glVertex2f( ball2[i].w,  ball2[i].h);
            glVertex2f( ball2[i].w, -ball2[i].h);
            glEnd();
            glPopMatrix();
        }
    }

    }
    
}
    //Draw particle.
    for (int i = 0; i < g.pr; i++) {
	    glPushMatrix();
	    glColor3ub(255, 110, 0); // Changed to variables
        //glColor3f(boxr, boxg, boxb);
	    glTranslatef(particles[i].pos[0], particles[i].pos[1], 0.0f);
	    glBegin(GL_QUADS);
		glVertex2f(-particles[i].w, -particles[i].h);
		glVertex2f(-particles[i].w,  particles[i].h);
		glVertex2f( particles[i].w,  particles[i].h);
		glVertex2f( particles[i].w, -particles[i].h);
	    glEnd();
	    glPopMatrix();
    }

    if (!g.issa_feature) {
        string issa_msg = "Press 3 to enter Issa's feature mode";
        char temp[50];
        Rect msg;
        create_text(msg, issa_msg, temp, g.xres-140, g.yres-30);
    } else if (g.issa_feature) {
        string issa_msg = "Press 3 to exit Issa's feature mode";
        string map_msg1 = "This is table 1";
        string map_msg2 = "This is table 2";
        string q_key = "Press q to reduce lives to zero";
        string t_key = "Press t to change score to high score + 1";
        string y_key = "Press y to change score to random number";
        string u_key = "Press u to change score to zero";
        char temp[50];
        Rect msg;
        create_text(msg, issa_msg, temp, g.xres-140, g.yres-30);
        if (g.map == 1) {
            create_text(msg, map_msg1, temp, g.xres-140, g.yres-45);
        } else if (g.map == 2) {
            create_text(msg, map_msg2, temp, g.xres-140, g.yres-45);
        }
        create_text(msg, q_key, temp, g.xres-140, g.yres-60);
        create_text(msg, t_key, temp, g.xres-140, g.yres-75);
        create_text(msg, y_key, temp, g.xres-140, g.yres-90);
        create_text(msg, u_key, temp, g.xres-140, g.yres-105);
    }
}
