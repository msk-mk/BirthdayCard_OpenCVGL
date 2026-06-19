#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <string>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "Birthday Card"
#define TEXTURE_HEIGHT (512)
#define TEXTURE_WIDTH (512)

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void draw_pyramid();
void draw_square();
void draw_square_edge();
void draw_triangle();
void draw_mailedge_yoko();
void draw_mailedge_tate();
void draw_cylinder(float radius, float height, int sides);
void draw_balloon();
void draw_cube();
void draw_present();
void draw_note();
void display_string(char *str, void *font);
void page0();
void page1();
void page2();
void page3();
void page4();
void page5();
void page6();
void page7();
void page8();
void page9();

double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
int pagenum;
int background_color;
int envelope_color;
int design_num;

int main(int argc, char *argv[]){
    pagenum = 0;
    background_color = 0;
    envelope_color = 0;
	design_num = 0;

    init_GL(argc,argv);

    init();

    set_callback_functions();

    glutMainLoop();
    return 0;
}

void init_GL(int argc, char *argv[]){
  	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  	glutInitWindowSize(WINDOW_X,WINDOW_Y);
  	glutCreateWindow(WINDOW_NAME);
}

void init(){
	glClearColor(1.0, 1.0, 0.9, 1.0);
}

void set_callback_functions(){
  	glutDisplayFunc(glut_display);
  	glutKeyboardFunc(glut_keyboard);
  	glutMouseFunc(glut_mouse);
  	glutMotionFunc(glut_motion);
  	glutPassiveMotionFunc(glut_motion);
  	glutIdleFunc(glut_idle);
}

void glut_keyboard(unsigned char key, int x, int y){
    switch(key){
    	case 'q':
    	case 'Q':
    	case '\033':
        	exit(0);
    	case 'h':
			pagenum = (pagenum + 1) % 10; 
			break;
    	case 'g':
			pagenum = (pagenum - 1) % 10; 
			break;
    	case 'b':
    	if(pagenum == 1){
    		background_color = (background_color + 1) % 4; 
    	}
    	else if(pagenum == 2){
    		envelope_color = (envelope_color + 1) % 4; 
   	 	}
		else if(pagenum == 7){
    		design_num = (design_num + 1) % 2; 
    	}
    	break;
	}
  	glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
  	if(button == GLUT_LEFT_BUTTON){
    	if(state == GLUT_UP){
      		g_isLeftButtonOn = false;
    	}
		else if(state == GLUT_DOWN){
      		g_isLeftButtonOn = true;
    	}
  	}

  	if(button == GLUT_RIGHT_BUTTON){
    	if(state == GLUT_UP){
      		g_isRightButtonOn = false;
   		}
		else if(state == GLUT_DOWN){
      		g_isRightButtonOn = true;
    	}
  	}
}

void glut_motion(int x, int y){
  	static int px = -1, py = -1;
  	if(g_isLeftButtonOn == true){
    	if(px >= 0 && py >= 0){
      		g_angle1 += (double)-(x - px)/20;
      		g_angle2 += (double)(y - py)/20;
    	}
    	px = x;
    	py = y;
  	}
	else if(g_isRightButtonOn == true){
    	if(px >= 0 && py >= 0){
      		g_distance += (double)(y - py)/20;
    	}
    px = x;
    py = y;
  	}
	else{
    	px = -1;
    	py = -1;
  	}
  	glutPostRedisplay();
}

void display_string(char *str, void *font){
    unsigned int i;
    for(i = 0; i < strlen(str); i++) 
        glutBitmapCharacter(font, str[i]); 
}

void glut_display(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (cos(g_angle2)>0){
      	gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1), g_distance * sin(g_angle2), g_distance * cos(g_angle2) * cos(g_angle1), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else{
      	gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),g_distance * sin(g_angle2),g_distance * cos(g_angle2) * cos(g_angle1),0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	if(pagenum == 0){
    	page0();
    }
    else if(pagenum == 1){
      	page1();
    }
    else if(pagenum == 2){
      	page2();
    }
    else if(pagenum == 3){
      	page3();
    }
    else if(pagenum == 4){
      	page4();
    }
    else if(pagenum == 5){
      	page5();
    }
    else if(pagenum == 6){
      	page6();
    }
    else if(pagenum == 7){
      	page7();
    }
    else if(pagenum == 8){
      	page8();
    }
	else if(pagenum == 9){
      	page9();
    }
    glFlush();
    glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

void glut_idle(){
  	glutPostRedisplay();
}

void draw_square(){
    GLdouble pointA[] = {-0.5, 0.5, 0.0};
    GLdouble pointB[] = {0.5, 0.5, 0.0};
    GLdouble pointC[] = {0.5, -0.5, 0.0};
    GLdouble pointD[] = {-0.5, -0.5, 0.0};
    glBegin(GL_POLYGON);
    glVertex3dv(pointA);
    glVertex3dv(pointB);
    glVertex3dv(pointC);
    glVertex3dv(pointD);
    glEnd();
}

void draw_pyramid(){
  	GLdouble pointO[] = {0.0, 1.0, 0.0};
  	GLdouble pointA[] = {1.5, -1.0, 1.5};
  	GLdouble pointB[] = {-1.5, -1.0, 1.5};
  	GLdouble pointC[] = {-1.5, -1.0, -1.5};
  	GLdouble pointD[] = {1.5, -1.0, -1.5};

  	glBegin(GL_TRIANGLES);
  	glVertex3dv(pointO);
  	glVertex3dv(pointA);
  	glVertex3dv(pointB);
  	glEnd();

  	glBegin(GL_TRIANGLES);
  	glVertex3dv(pointO);
  	glVertex3dv(pointB);
  	glVertex3dv(pointC);
  	glEnd();

  	glBegin(GL_TRIANGLES);
  	glVertex3dv(pointO);
  	glVertex3dv(pointC);
  	glVertex3dv(pointD);
  	glEnd();

  	glBegin(GL_TRIANGLES);
  	glVertex3dv(pointO);
  	glVertex3dv(pointD);
  	glVertex3dv(pointA);
  	glEnd();
}

void draw_triangle(){
	GLdouble pointA[] = {0.0, 1.0, 0.0};
	GLdouble pointB[] = {0.0, 0.0, 0.0};
	GLdouble pointC[] = {1.0, 0.5, 0.0};	
	glBegin(GL_TRIANGLES);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glEnd();
}

void draw_square_edge(){
    glBegin(GL_LINES);
    
    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    
    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);
    
    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);
    
    glVertex3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);

    glEnd(); 
}

void draw_mailedge_yoko(){
	GLdouble rightup[] = {0.2, 0.1, 0.0};
	GLdouble rightdown[] = {0.15, 0.0, 0.0};
	GLdouble leftdown[] = {-0.05, 0.0, 0.0};
	GLdouble leftup[] = {0.0, 0.1, 0.0};
	glBegin(GL_POLYGON);
	glVertex3dv(rightup);
	glVertex3dv(rightdown);
	glVertex3dv(leftdown);
	glVertex3dv(leftup);
	glEnd();
}

void draw_mailedge_tate(){
	GLdouble rightup[] = {0.15, 0.2, 0.0};
	GLdouble rightdown[] = {0.1, 0.0, 0.0};
	GLdouble leftdown[] = {0.0, 0.0, 0.0};
	GLdouble leftup[] = {0.05, 0.2, 0.0};
	glBegin(GL_POLYGON);
	glVertex3dv(rightup);
	glVertex3dv(rightdown);
	glVertex3dv(leftdown);
	glVertex3dv(leftup);
	glEnd();
}

void draw_cylinder(float radius, float height, int sides) {
    double pi = 3.14159265358979323846;
	
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), height, radius * sin(angle));
    }
    glEnd();
   
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= sides; ++i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), height, radius * sin(angle));
        glVertex3f(radius * cos(angle), -height, radius * sin(angle));
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for (int i = sides; i >= 0; --i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), -height, radius * sin(angle));
    }
    glEnd();
}

void draw_ballon(){
	glPushMatrix();
  	glTranslated(0.0, 1.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.9, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(0.0, 0.4, 0.0);
	glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 0.6f, 36); 
	glPopMatrix();
}

void draw_cube(){
	GLdouble pointO[] = {-1.0, 1.0, -1.0};
	GLdouble pointA[] = {1.0, 1.0, -1.0};
	GLdouble pointB[] = {-1.0, -1.0, -1.0};
	GLdouble pointC[] = {1.0, -1.0, -1.0};
	GLdouble pointD[] = {-1.0, 1.0, 1.0};
    GLdouble pointE[] = {1.0, 1.0, 1.0};
	GLdouble pointF[] = {-1.0, -1.0, 1.0};
	GLdouble pointG[] = {1.0, -1.0, 1.0};

	glBegin(GL_POLYGON);
	glVertex3dv(pointO);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointA);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointE);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointE);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glVertex3dv(pointD);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointD);
	glVertex3dv(pointF);
	glVertex3dv(pointB);
	glVertex3dv(pointO);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointO);
	glVertex3dv(pointA);
	glVertex3dv(pointE);
	glVertex3dv(pointD);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glEnd();
}

void draw_ribbon(){
	glColor3d(0.741, 0.718, 0.420); 

	glPushMatrix();
  	glTranslated(-1.5, 1.0, 0.51);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.0, -0.51);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-0.99, 1.0, 0.0);
	glRotatef(90,0.0,1.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-2.01, 1.0, 0.0);
	glRotatef(-90,0.0,1.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.51, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 0.49, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.51, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(1.02,0.3,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 0.49, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(1.02,0.3,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.65, 1.65, -0.1);
	glRotatef(-45,0.0,1.0,0.0);
	glRotatef(-135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.65, 1.65, 0.1);
	glRotatef(45,0.0,1.0,0.0);
	glRotatef(-135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.4, 1.65, 0.1);
	glRotatef(-45,0.0,1.0,0.0);
	glRotatef(135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.4, 1.7, -0.1);
	glRotatef(45,0.0,1.0,0.0);
	glRotatef(135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();
}

void draw_present(){
	glPushMatrix();
  	glTranslated(-1.5, 1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();

	glPushMatrix();
  	draw_ribbon();
  	glPopMatrix();
}

void draw_note(){
	glPushMatrix();
	glRotated(30,0.0,0.0,1.0);
	glScaled(0.4,0.2,0.2);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();
	glPushMatrix();
    glTranslated(0.1, 0.5, 0.02);
    draw_cylinder(0.02f, 0.4f, 36); 
	glPopMatrix();
}

void page0(){
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.5,0.5,0.1);
	display_string("Let's Make", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-0.7,0.0,0.1);
	display_string("Your Original", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(0.1,-0.5,0.1);
	display_string("Birthday Card !", GLUT_BITMAP_TIMES_ROMAN_24);
    
	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page1(){
  	if(background_color==0){
		glClearColor(1.0, 1.0, 0.8, 1.0);
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-2.0,0.0,0.1);
		display_string("If you prefer this color, write '1' on the 1st line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else if(background_color==1){
    	glClearColor(0.8, 0.8, 1.0, 1.0);
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-2.0,0.0,0.1);
		display_string("If you prefer this color, write '2' on the 1st line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else if(background_color==2){
    	glClearColor(0.8, 1.0, 1.0, 1.0);
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-2.0,0.0,0.1);
		display_string("If you prefer this color, write '3' on the 1st line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else{
    	glClearColor(1.0, 0.8, 1.0, 1.0);
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-2.0,0.0,0.1);
		display_string("If you prefer this color, write '4' on the 1st line.", GLUT_BITMAP_HELVETICA_18);
  	}

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.0,0.5,0.1);
	display_string("Background Color", GLUT_BITMAP_TIMES_ROMAN_24);

    glPushMatrix();
	glColor3d(1.0,1.0,1.0); 
   	glTranslated(-2.0, 2.5, 0.0);
    glScalef(3.3,1.0,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(1.0,1.0,0.8); 
   	glTranslated(-2.3, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(0.8, 0.8, 1.0); 
   	glTranslated(-1.8, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(0.8, 1.0, 1.0); 
   	glTranslated(-1.3, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(1.0, 0.8, 1.0); 
   	glTranslated(-0.8, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-2.35, 2.2, 0.15);
	display_string("1", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-1.85, 2.2, 0.15);
	display_string("2", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-1.35, 2.2, 0.15);
	display_string("3", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-0.85, 2.2, 0.15);
	display_string("4", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.0, 2.2, 0.15);
	display_string("Which color do you like?", GLUT_BITMAP_HELVETICA_18);

  	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

   	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(0.2, 1.9, 0.1);
	display_string("b", GLUT_BITMAP_HELVETICA_18);
	glRasterPos3d(0.5, 1.9, 0.1);
	display_string("Color Change", GLUT_BITMAP_HELVETICA_18);
	
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(0.15,1.85,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page2(){
  	if(envelope_color==0){
    	glPushMatrix();
		glColor3d(1.0, 1.0, 0.8); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,1.0,0.5); 
   		glTranslated(1.5, 0.9, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-1.2,-0.1,0.1);
		display_string("If you prefer this color,", GLUT_BITMAP_HELVETICA_18);
    	glRasterPos3d(-1.2,-0.5,0.1);
		display_string("write '1' on the 2nd line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else if(envelope_color==1){
    	glPushMatrix();
		glColor3d(0.8, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(0.5, 0.5, 1.0); 
   		glTranslated(1.5, 0.9, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-1.2,-0.1,0.1);
		display_string("If you prefer this color,", GLUT_BITMAP_HELVETICA_18);
    	glRasterPos3d(-1.2,-0.5,0.1);
		display_string("write '2' on the 2nd line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else if(envelope_color==2){
    	glPushMatrix();
	  	glColor3d(0.8, 1.0, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
	  	glColor3d(0.0, 1.0,1.0); 
   		glTranslated(1.5, 0.9, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-1.2,-0.1,0.1);
	  	display_string("If you prefer this color,", GLUT_BITMAP_HELVETICA_18);
    	glRasterPos3d(-1.2,-0.5,0.1);
	  	display_string("write '3' on the 2nd line.", GLUT_BITMAP_HELVETICA_18);
  	}
  	else{
    	glPushMatrix();
		glColor3d(1.0, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,0.5,1.0); 
   		glTranslated(1.5, 0.9, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
    	glColor3d(0.0,0.0,0.0); 
    	glRasterPos3d(-1.2,-0.1,0.1);
		display_string("If you prefer this color,", GLUT_BITMAP_HELVETICA_18);
    	glRasterPos3d(-1.2,-0.5,0.1);
		display_string("write '4' on the 2nd line.", GLUT_BITMAP_HELVETICA_18);
  	}

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.8,0.5,0.1);
	display_string("Basic Color", GLUT_BITMAP_TIMES_ROMAN_24);

    glPushMatrix();
	glColor3d(1.0,1.0,1.0); 
   	glTranslated(-2.0, 2.5, 0.0);
    glScalef(3.3,1.0,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(1.0,1.0,0.8); 
   	glTranslated(-2.3, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(0.8, 0.8, 1.0); 
   	glTranslated(-1.8, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(0.8, 1.0, 1.0); 
   	glTranslated(-1.3, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glPushMatrix();
	glColor3d(1.0, 0.8, 1.0); 
   	glTranslated(-0.8, 2.3, 0.1);
    glScalef(0.5,0.5,1.0);
    draw_square();
  	glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-2.35, 2.2, 0.15);
	display_string("1", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-1.85, 2.2, 0.15);
	display_string("2", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-1.35, 2.2, 0.15);
	display_string("3", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-0.85, 2.2, 0.15);
	display_string("4", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.0, 2.2, 0.15);
	display_string("Which color do you like?", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(0.2, 1.9, 0.1);
	display_string("b", GLUT_BITMAP_HELVETICA_18);
	glRasterPos3d(0.5, 1.9, 0.1);
	display_string("Color Change", GLUT_BITMAP_HELVETICA_18);
	
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(0.15,1.85,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page3(){
  	if(envelope_color==0){
    	glPushMatrix();
		glColor3d(1.0, 1.0, 0.8); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,1.0,0.5); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}	
  	else if(envelope_color==1){
    	glPushMatrix();
		glColor3d(0.8, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(0.5, 0.5, 1.0); 
   		glTranslated(1.35, 0.8, 0.1);
   	 	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	else if(envelope_color==2){
    	glPushMatrix();
		glColor3d(0.8, 1.0, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(0.0, 1.0,1.0); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	else{
    	glPushMatrix();
		glColor3d(1.0, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,0.5,1.0); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	for(int i = 0; i < 7; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(1.55-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(1.85-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 7; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(1.55-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(1.85-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(-2.1, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(-2.1, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(2.0, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(2.0, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}
    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.1,-0.7,0.1);
	display_string("Write the recipient", GLUT_BITMAP_9_BY_15);
    glRasterPos3d(0.4,-0.85,0.1);
	display_string("on the 3rd line.", GLUT_BITMAP_9_BY_15);
    glRasterPos3d(-1.7,1.0,0.1);
	display_string("From", GLUT_BITMAP_HELVETICA_18);
    glRasterPos3d(0.2,-0.3,0.1);
	display_string("To", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3d(-1.8,-1.0,0.1);
	display_string("VIA AIR MAIL", GLUT_BITMAP_HELVETICA_18);

    glColor3d(0.6,0.6,0.6);
    glBegin(GL_LINES);

    glVertex3d(0.0, -0.5, 0.1);
    glVertex3d(1.9, -0.5, 0.1);

    glVertex3d(1.9, -0.5, 0.1);
    glVertex3d(1.9, 0.0, 0.1);

    glVertex3d(1.9, 0.0, 0.1);
    glVertex3d(0.0, 0.0, 0.1);

    glVertex3d(0.0, 0.0, 0.1);
    glVertex3d(0.0, -0.5, 0.1);
    glEnd();   

    glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix(); 
}

void page4(){
  	if(envelope_color==0){
    	glPushMatrix();
		glColor3d(1.0, 1.0, 0.8); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,1.0,0.5); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	else if(envelope_color==1){
    	glPushMatrix();
		glColor3d(0.8, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
   	 	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(0.5, 0.5, 1.0); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	else if(envelope_color==2){
    	glPushMatrix();
		glColor3d(0.8, 1.0, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(0.0, 1.0,1.0); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}
  	else{
    	glPushMatrix();
		glColor3d(1.0, 0.8, 1.0); 
    	glScalef(4.5,3.0,1.0);
    	draw_square();
  		glPopMatrix();
    	glPushMatrix();
		glColor3d(1.0,0.5,1.0); 
   		glTranslated(1.35, 0.8, 0.1);
    	glScalef(1.0,0.7,1.5);
    	draw_square();
  		glPopMatrix();
  	}

  	for(int i = 0; i < 7; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(1.55-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(1.85-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 7; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(1.55-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(1.85-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(-2.1, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(-2.1, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(2.0, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(2.0, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}

    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.7,0.55,0.1);
	display_string("Write the sender", GLUT_BITMAP_9_BY_15);
    glRasterPos3d(-1.7,0.4,0.1);
	display_string("on the 4th line.", GLUT_BITMAP_9_BY_15);
    glRasterPos3d(-1.7,1.0,0.1);
	display_string("From", GLUT_BITMAP_HELVETICA_18);
    glRasterPos3d(0.2,-0.3,0.1);
	display_string("To", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3d(-1.8,-1.0,0.1);
	display_string("VIA AIR MAIL", GLUT_BITMAP_HELVETICA_18);

    glColor3d(0.6,0.6,0.6);
    glBegin(GL_LINES);

    glVertex3d(-1.8, 0.7, 0.1);
    glVertex3d(0.0, 0.7, 0.1);

    glVertex3d(0.0, 0.7, 0.1);
    glVertex3d(0.0, 1.2, 0.1);

    glVertex3d(0.0, 1.2, 0.1);
    glVertex3d(-1.8, 1.2, 0.1);

    glVertex3d(-1.8, 1.2, 0.1);
    glVertex3d(-1.8, 0.7, 0.1);
    glEnd();  

    glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();  
}

void page5(){
    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.5,0.5,0.1);
	display_string("Music", GLUT_BITMAP_TIMES_ROMAN_24);
    glRasterPos3d(-1.5,0.0,0.1);
	display_string("What music do you want to play?", GLUT_BITMAP_HELVETICA_18);
    glRasterPos3d(-1.2,-0.2,0.1);
    display_string("Write one on the 5th line.", GLUT_BITMAP_HELVETICA_18);

  	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8,0.4,0.8);
	glTranslated(1.0,-1.8,0.0);
	glRotated(-15.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.6,0.4,0.8);
	glTranslated(1.6,-1.3,0.0);
	glRotated(10.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.4,0.8,0.8);
	glTranslated(1.9,-1.9,0.0);
	glRotated(-15.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.4,0.8,0.6);
	glTranslated(-1.0,1.8,0.0);
	glRotated(-15.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.8,0.8,0.4);
	glTranslated(-1.6,1.3,0.0);
	glRotated(-10.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.8,0.4,0.6);
	glTranslated(-1.9,1.9,0.0);
	glRotated(15.0,0.0,0.0,1.0);
	glScaled(0.6,0.6,0.6);
	draw_note();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page6(){
    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.8,0.5,0.1);
	display_string("Photograph", GLUT_BITMAP_TIMES_ROMAN_24);
    glRasterPos3d(-2.1,0.0,0.1);
	display_string("What photos do yo want to include on the card?", GLUT_BITMAP_HELVETICA_18);
    glRasterPos3d(-1.5,-0.2,0.1);
    display_string("Write five on lines 6 through 10.", GLUT_BITMAP_HELVETICA_18);

	glPushMatrix();
	glTranslated(-1.5,-1.5,0.0);
	glRotated(20.0,0.0,0.0,1.0);
	glPushMatrix();
	glColor3d(0.5,0.5,0.5);
	glScaled(0.7,0.5,0.2);
	draw_cube();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glTranslated(0.0,0.0,0.21);
	glScaled(1.4,0.6,1.0);
	draw_square();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.5,0.5,0.5);
	glTranslated(0.0,0.0,0.3);
	glRotated(90,1.0,0.0,0.0);
	draw_cylinder(0.3,0.1,36);
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glTranslated(0.0,0.0,0.4);
	glRotated(90,1.0,0.0,0.0);
	draw_cylinder(0.25,0.01,36);
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.5,0.5,0.5);
	glTranslated(-0.3,0.6,0.0);
	glScaled(0.2,0.1,0.1);
	draw_cube();
	glPopMatrix();
	glPopMatrix();

  	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page7(){
	glPushMatrix();
	glColor3d(1.0,1.0,1.0); 
   	glTranslated(-2.0, 2.5, 0.0);
    glScalef(3.3,1.0,1.0);
    draw_square();
  	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.7, 0.7);
   	glTranslated(-2.0, 2.1, 0.2);
    glScalef(0.22,0.22,0.22);
    draw_ballon();
  	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-2.4, 2.2, 0.1);
	display_string("1", GLUT_BITMAP_TIMES_ROMAN_24);

	glPushMatrix();
	glColor3f(1.0, 0.7, 0.7);
   	glTranslated(-0.7, 2.0, 0.2);
    glScalef(0.25,0.25,0.25);
    draw_present();
  	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.6, 2.2, 0.1);
	display_string("2", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.0, 2.2, 0.15);
	display_string("Which design do you like?", GLUT_BITMAP_HELVETICA_18);

	if(design_num==0){
		glPushMatrix();
		glTranslated(-1.5,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(1.0, 0.0, 1.0);
		draw_ballon();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.0,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.5, 0.0, 1.0);
		draw_ballon();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.5,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.0, 1.0, 0.5);
		draw_ballon();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.75,-0.9,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(1.0, 1.0, 0.0);
		draw_ballon();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.75,-0.9,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.0, 0.0, 1.0);
		draw_ballon();
		glPopMatrix();

		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3d(-2.2, -1.4, 0.1);
		display_string("If you prefer this design, write '1' on the 11th line.", GLUT_BITMAP_HELVETICA_18);
	}
	else if(design_num==1){
		glPushMatrix();
		glTranslated(-0.5,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(1.0, 0.0, 1.0);
		draw_present();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.9,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.5, 0.0, 1.0);
		draw_present();
		glPopMatrix();

		glPushMatrix();
		glTranslated(2.3,0.0,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.0, 1.0, 0.5);
		draw_present();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.2,-1.2,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(1.0, 1.0, 0.0);
		draw_present();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.6,-1.2,0.0);
		glScaled(0.6,0.6,0.6);
		glColor3d(0.0, 0.0, 1.0);
		draw_present();
		glPopMatrix();

		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3d(-2.2, -1.4, 0.1);
		display_string("If you prefer this design, write '2' on the 11th line.", GLUT_BITMAP_HELVETICA_18);
	}

  	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(0.2, 1.9, 0.1);
	display_string("b", GLUT_BITMAP_HELVETICA_18);
	glRasterPos3d(0.5, 1.9, 0.1);
	display_string("Design Change", GLUT_BITMAP_HELVETICA_18);
	
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(0.15,1.85,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page8(){
    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.5,0.5,0.1);
	display_string("Movie", GLUT_BITMAP_TIMES_ROMAN_24);
    glRasterPos3d(-2.1,0.0,0.1);
	display_string("What movie do yo want to include on the card?", GLUT_BITMAP_HELVETICA_18);
    glRasterPos3d(-1.25,-0.2,0.1);
    display_string("Write one on the 12th line.", GLUT_BITMAP_HELVETICA_18);

	glPushMatrix();
	glTranslated(-1.5,-1.5,1.0);
	glRotated(20.0,0.0,0.0,1.0);
	glScaled(0.8,0.8,0.8);

	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glScaled(1.5,1.1,1.0);
	draw_square();
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0,1.0,1.0);
	glTranslated(0.0,0.0,0.01);
	glScaled(0.8,0.9,1.0);
	draw_square();
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glTranslated(-0.17,-0.2,0.02);
	glScaled(0.4,0.4,1.0);
	draw_triangle();
	glPopMatrix();

	for(int i =0; i<5; i++){
		glPushMatrix();
	  	glTranslated(-0.6,0.4-0.2*i,0.1);
		glColor3d(1.0,1.0,1.0);
		glScaled(0.15,0.1,1.0);
		draw_square();
		glPopMatrix();
	}
	for(int i =0; i<5; i++){
		glPushMatrix();
	  	glTranslated(0.57,0.4-0.2*i,0.1);
		glColor3d(1.0,1.0,1.0);
		glScaled(0.15,0.1,1.0);
		draw_square();
		glPopMatrix();
	}
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page9(){
    glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.3,0.0,0.1);
	display_string("END", GLUT_BITMAP_TIMES_ROMAN_24);
    
	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}